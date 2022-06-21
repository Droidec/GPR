/******************************************************************************
 *
 * \file gpr_net.c
 * \brief Network module
 *
 ******************************************************************************
 *
 * \copyright Copyright (c) 2019, the respective contributors, as shown by the
 * AUTHORS file.
 *
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/

#include "gpr_net.h"

#include <arpa/inet.h>  // inet_ntop, ntohs
#include <errno.h>      // errno
#include <fcntl.h>      // fcntl
#include <netdb.h>      // addrinfo, getaddrinfo, freeaddrinfo, gai_strerror
#include <poll.h>       // poll
#include <stdint.h>     // uint16_t
#include <stdlib.h>     // malloc, free
#include <string.h>     // memset, memcpy
#include <sys/socket.h> // socket, setsockopt, getsockopt, bind, listen, connect, socklen_t, getsockname, getpeername, recv,
                        // recvfrom, send, sendto, sockaddr_storage
#include <unistd.h>     // close

#include "gpr_err.h"
#include "gpr_utils.h"

static void set_socket_type(struct gpr_socket *sock);
static void close_socket(struct gpr_socket *sock);
static enum GPR_Err resolve_hostname(struct gpr_socket *sock, const char *addr, const char *service);
static enum GPR_Err init_server_socket(struct gpr_socket *sock);
static enum GPR_Err init_client_socket(struct gpr_socket *sock, const char *local);
static enum GPR_Err listen_to_service(struct gpr_socket *sock, int backlog);
static enum GPR_Err connect_to_peer(struct gpr_socket *sock, const char *local);
static enum GPR_Err bind_to_local_service(struct gpr_socket *sock, const char *local);
static enum GPR_Err check_connection_progress(struct gpr_socket *sock);

struct gpr_socket *gpr_net_new_socket(int domain, int type, int protocol, int flags, bool nonblock)
{
    struct gpr_socket *sock = NULL;

    /* Allocate socket */
    sock = (struct gpr_socket *)malloc(sizeof(struct gpr_socket));
    if (UNLIKELY(sock == NULL))
        return NULL;

    /* Initialize socket */
    gpr_net_init_socket(sock, domain, type, protocol, flags, nonblock);

    return sock;
}

void gpr_net_init_socket(struct gpr_socket *sock, int domain, int type, int protocol, int flags, bool nonblock)
{
#ifdef DEBUG
    /* Check consistency */
    if (sock == NULL)
        return;
#endif

    /* Initialize socket info */
    sock->socket = GPR_NET_INVALID_SOCKET;
    sock->state = GPR_NET_STATE_CLOSED;
    sock->type = GPR_NET_TYPE_UNKNOWN;
    memset(&(sock->hints), 0, sizeof(struct addrinfo));
    sock->hints.ai_family = domain;
    sock->hints.ai_socktype = type;
    sock->hints.ai_protocol = protocol;
    sock->hints.ai_flags = flags;
    sock->res = NULL;
    sock->cur = NULL;
    sock->nonblock = nonblock;

    /* Initialize connection info */
    memset(&(sock->sock_info), 0, sizeof(struct sockaddr_storage));
    memset(&(sock->peer_info), 0, sizeof(struct sockaddr_storage));
}

void gpr_net_init_default_tcp_socket(struct gpr_socket *sock, bool nonblock)
{
    gpr_net_init_socket(sock, AF_UNSPEC, SOCK_STREAM, 0, AI_PASSIVE | AI_ADDRCONFIG, nonblock);
}

void gpr_net_init_default_udp_socket(struct gpr_socket *sock, bool nonblock)
{
    gpr_net_init_socket(sock, AF_UNSPEC, SOCK_DGRAM, 0, AI_PASSIVE | AI_ADDRCONFIG, nonblock);
}

void gpr_net_copy_socket(const struct gpr_socket *in, struct gpr_socket *out)
{
#ifdef DEBUG
    /* Check consistency */

    // Check socket in
    if (in == NULL)
        return;

    // Check socket out
    if (out == NULL)
        return;
#endif

    memcpy(out, in, sizeof(struct gpr_socket));
}

void gpr_net_close_socket(struct gpr_socket *sock)
{
#ifdef DEBUG
    /* Check consistency */
    if (sock == NULL)
        return;
#endif

    /* Close socket */
    close_socket(sock);

    /* Free socket components */
    if (sock->res != NULL)
        freeaddrinfo(sock->res);
    sock->res = NULL;
    sock->cur = NULL;
}

void gpr_net_free_socket(struct gpr_socket *sock)
{
#ifdef DEBUG
    /* Check consistency */
    if (sock == NULL)
        return;
#endif

    /* Close socket */
    gpr_net_close_socket(sock);

    /* Free socket */
    free(sock);
}

const char *gpr_net_socket_state_to_str(enum GPR_Net_State state)
{
    static const char *state_array[] = {"CLOSED", "CLOSING", "CONNECTING", "CONNECTED", "LISTENING"};

#ifdef DEBUG
    /* Check consistency */
    if ((state < 0) || (state >= GPR_NET_STATE_NUMBERS))
        return "UNKNOWN";
#endif

    return state_array[state];
}

const char *gpr_net_socket_type_to_str(enum GPR_Net_Type type)
{
    static const char *type_array[] = {"UNKNOWN", "CONNECTION-ORIENTED", "CONNECTION-LESS"};

#ifdef DEBUG
    /* Check consistency */
    if ((type < 0) || (type >= GPR_NET_TYPE_NUMBERS))
        return "UNKNOWN";
#endif

    return type_array[type];
}

enum GPR_Err gpr_net_listen(struct gpr_socket *sock, const char *addr, const char *service, int backlog)
{
    enum GPR_Err err;

#ifdef DEBUG
    /* Check consistency */

    // Check socket
    if (sock == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid socket");

    // Check service
    if (service == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid service");
#endif

    /* Resolve hostname */
    err = resolve_hostname(sock, addr, service);
    if (err != GPR_ERR_OK)
        return err;
    sock->cur = sock->res;

    // Loop through network addresses and bind to the first we can
    while (sock->cur != NULL)
    {
        err = init_server_socket(sock);
        if (err != GPR_ERR_OK)
        {
            close_socket(sock);
            sock->cur = sock->cur->ai_next;
            continue; // Abort current network address
        }

        err = listen_to_service(sock, backlog);
        if (err != GPR_ERR_OK)
        {
            close_socket(sock);
            sock->cur = sock->cur->ai_next;
            continue; // Abort current network address
        }

        break;
    }

    // All done with network addresses
    if (sock->cur != NULL)
    {
        socklen_t info_len = sizeof(struct sockaddr_storage);

        // Get socket info
        if (getsockname(sock->socket, (struct sockaddr *)&(sock->sock_info), &info_len) < 0)
        {
            close_socket(sock);
            return gpr_err_raise(GPR_ERR_NETWORK_ERROR, "getsockname: [Errno %d] %s", errno, strerror(errno));
        }

        sock->state = GPR_NET_STATE_LISTENING;
        set_socket_type(sock);
    }

    freeaddrinfo(sock->res);
    sock->res = NULL;
    sock->cur = NULL;

    return err;
}

enum GPR_Err gpr_net_connect(struct gpr_socket *sock, const char *addr, const char *service, const char *local)
{
    enum GPR_Err err;

#ifdef DEBUG
    /* Check consistency */

    // Check socket
    if (sock == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid socket");

    // Check address
    if (addr == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid address/hostname");

    // Check service
    if (service == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid service");
#endif

    /* Resolve hostname */
    if (sock->res == NULL)
    {
        err = resolve_hostname(sock, addr, service);
        if (err != GPR_ERR_OK)
            return err;
        sock->cur = sock->res;
    }

    // Loop through network addresses and bind to the first we can
    while (sock->cur != NULL)
    {
        err = init_client_socket(sock, local);
        if (err != GPR_ERR_OK)
        {
            close_socket(sock);
            sock->cur = sock->cur->ai_next;
            continue; // Abort current network address
        }

        err = connect_to_peer(sock, local);
        if (err != GPR_ERR_OK)
        {
            if (err == GPR_ERR_PENDING)
                return err; // Connection in progress
            close_socket(sock);
            sock->cur = sock->cur->ai_next;
            continue; // Abort current network address
        }

        break;
    }

    // All done with network addresses
    if (sock->cur != NULL)
    {
        socklen_t info_len = sizeof(struct sockaddr_storage);

        // Get socket info
        if (getsockname(sock->socket, (struct sockaddr *)&(sock->sock_info), &info_len) < 0)
        {
            close_socket(sock);
            return gpr_err_raise(GPR_ERR_NETWORK_ERROR, "getsockname: [Errno %d] %s", errno, strerror(errno));
        }

        // Get peer info
        if (getpeername(sock->socket, (struct sockaddr *)&(sock->peer_info), &info_len) < 0)
        {
            close_socket(sock);
            return gpr_err_raise(GPR_ERR_NETWORK_ERROR, "getpeername: [Errno %d] %s", errno, strerror(errno));
        }

        sock->state = GPR_NET_STATE_CONNECTED;
        set_socket_type(sock);
    }

    freeaddrinfo(sock->res);
    sock->res = NULL;
    sock->cur = NULL;

    return err;
}

enum GPR_Err gpr_net_accept(const struct gpr_socket *in, struct gpr_socket *out)
{
    socklen_t info_len = sizeof(struct sockaddr_storage);

#ifdef DEBUG
    /* Check consistency */

    // Check socket in
    if (in == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid in socket");

    // Check socket out
    if (out == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid out socket");
#endif

    /* Initialize out socket */
    gpr_net_init_socket(out, 0, 0, 0, 0, in->nonblock);

    /* Accept connection */
    out->socket = accept(in->socket, (struct sockaddr *)&(out->peer_info), &info_len);
    if (out->socket == GPR_NET_INVALID_SOCKET)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return gpr_err_raise(GPR_ERR_PENDING, "No connection to be accepted");
        else
            return gpr_err_raise(GPR_ERR_NETWORK_ERROR, "accept: [Errno %d] %s", errno, strerror(errno));
    }

    out->state = GPR_NET_STATE_CONNECTED;
    out->type = in->type;

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

ssize_t gpr_net_recv(const struct gpr_socket *sock, void *buf, size_t size, int flags)
{
#ifdef DEBUG
    /* Check consistency */

    // Check socket
    if (sock == NULL)
    {
        errno = EBADF;
        return -1;
    }

    // Check buffer
    if (buf == NULL)
    {
        errno = EFAULT;
        return -1;
    }
#endif

    switch (sock->type)
    {
        case GPR_NET_TYPE_CONNECTION_ORIENTED:
            return recv(sock->socket, buf, size, flags);

        case GPR_NET_TYPE_CONNECTION_LESS:
#ifdef UNIX
        {
            socklen_t info_len = sizeof(struct sockaddr_storage);
            return recvfrom(sock->socket, buf, size, flags, (struct sockaddr *)&(sock->peer_info), &info_len);
        }
#else
        return recvfrom(sock->socket, buf, size, flags, NULL, 0);
#endif

        default:
            errno = EBADF;
            return -1;
    }
}

ssize_t gpr_net_send(const struct gpr_socket *sock, void *buf, size_t size, int flags)
{
#ifdef DEBUG
    /* Check consistency */

    // Check socket
    if (sock == NULL)
    {
        errno = EBADF;
        return -1;
    }

    // Check buffer
    if (buf == NULL)
    {
        errno = EFAULT;
        return -1;
    }
#endif

    switch (sock->type)
    {
        case GPR_NET_TYPE_CONNECTION_ORIENTED:
            return send(sock->socket, buf, size, flags);

        case GPR_NET_TYPE_CONNECTION_LESS:
#ifdef UNIX
            return sendto(sock->socket, buf, size, flags, (struct sockaddr *)&(sock->peer_info), sizeof(struct sockaddr_storage));
#else
            return sendto(sock->socket, buf, size, flags, NULL, 0);
#endif

        default:
            errno = EBADF;
            return -1;
    }
}

enum GPR_Err gpr_net_get_socket_info(const struct gpr_socket *sock, char *addr, uint16_t *port)
{
#ifdef DEBUG
    /* Check consistency */

    // Check socket
    if (sock == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid socket");

    // Check address buffer
    if (addr == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid address buffer");

    // Check port buffer
    if (port == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid port buffer");
#endif

    if (sock->sock_info.ss_family == AF_INET)
    {
        struct sockaddr_in *s = (struct sockaddr_in *)&(sock->sock_info);
        inet_ntop(AF_INET, &(s->sin_addr), addr, INET6_ADDRSTRLEN);
        *port = ntohs(s->sin_port);
    }
    else if (sock->sock_info.ss_family == AF_INET6)
    {
        struct sockaddr_in6 *s = (struct sockaddr_in6 *)&(sock->sock_info);
        inet_ntop(AF_INET6, &(s->sin6_addr), addr, INET6_ADDRSTRLEN);
        *port = ntohs(s->sin6_port);
    }
    else
    {
        return gpr_err_raise(GPR_ERR_NETWORK_ERROR, "Unhandled socket family");
    }

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

enum GPR_Err gpr_net_get_peer_info(const struct gpr_socket *sock, char *addr, uint16_t *port)
{
#ifdef DEBUG
    /* Check consistency */

    // Check socket
    if (sock == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid socket");

    // Check address buffer
    if (addr == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid address buffer");

    // Check port buffer
    if (port == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid port buffer");
#endif

    if (sock->peer_info.ss_family == AF_INET)
    {
        struct sockaddr_in *s = (struct sockaddr_in *)&(sock->peer_info);
        inet_ntop(AF_INET, &(s->sin_addr), addr, INET6_ADDRSTRLEN);
        *port = ntohs(s->sin_port);
    }
    else if (sock->peer_info.ss_family == AF_INET6)
    {
        struct sockaddr_in6 *s = (struct sockaddr_in6 *)&(sock->peer_info);
        inet_ntop(AF_INET6, &(s->sin6_addr), addr, INET6_ADDRSTRLEN);
        *port = ntohs(s->sin6_port);
    }
    else
    {
        return gpr_err_raise(GPR_ERR_NETWORK_ERROR, "Unhandled socket family");
    }

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

/**
 * \brief Sets the socket type according to hostname resolution
 *
 * \param[out] sock Socket where to set type
 */
static void set_socket_type(struct gpr_socket *sock)
{
#ifdef DEBUG
    /* Check consistency */
    if (sock == NULL)
        return;
#endif

    if ((sock->cur->ai_socktype & SOCK_STREAM) || (sock->cur->ai_socktype & SOCK_SEQPACKET))
        sock->type = GPR_NET_TYPE_CONNECTION_ORIENTED;
    else
        sock->type = GPR_NET_TYPE_CONNECTION_LESS;
}

/**
 * \brief Closes communication endpoint of a socket
 *
 * \param[out] sock Socket to close
 */
static void close_socket(struct gpr_socket *sock)
{
#ifdef DEBUG
    /* Check consistency */
    if (sock == NULL)
        return;
#endif

    /* Close communication endpoint */
    if (sock->socket != GPR_NET_INVALID_SOCKET)
        close(sock->socket);
    sock->socket = GPR_NET_INVALID_SOCKET;
    sock->state = GPR_NET_STATE_CLOSED;

    /* Reset connection info */
    memset(&(sock->sock_info), 0, sizeof(struct sockaddr_storage));
    memset(&(sock->peer_info), 0, sizeof(struct sockaddr_storage));
}

/**
 * \brief Resolves hostname according to \p addr and \p service
 *
 * \todo The function \c getaddrinfo is known to be synchronous and blocking.
 *       If its really a problem, a dedicated thread should be considered here
 *
 * \param[in,out] sock    Socket containing communication criterias
 * \param[in]     addr    Address/hostname to bind/connect to
 * \param[in]     service Service to listen/connect to
 *
 * \return This function allocates and initializes a linked list of \c addrinfo
 *         structures in the socket, one for each network address that matches
 *         \p addr and \p service
 *
 * \retval #GPR_ERR_OK Network addresses found
 * \retval #GPR_ERR_INVALID_PARAMETER The socket is \c NULL or the service is \c NULL
 *         (for \e DEBUG mode only)
 * \retval #GPR_ERR_NETWORK_ERROR No network addresses found or an error occured
 */
static enum GPR_Err resolve_hostname(struct gpr_socket *sock, const char *addr, const char *service)
{
#ifdef DEBUG
    /* Check consistency */

    // Check socket
    if (sock == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid socket");

    // Check service
    if (service == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid service");
#endif

    enum GPR_Err err;

    /* Resolve hostname */
    err = getaddrinfo(addr, service, &(sock->hints), &(sock->res));
    if (err != 0)
        return gpr_err_raise(GPR_ERR_NETWORK_ERROR, "getaddrinfo: %s", gai_strerror(err)); // gai_strerror should be thread-safe

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

/**
 * \brief Initializes a socket for a server communication
 *
 * \param[in,out] sock Socket to prepare
 *
 * \return This function initializes the communication endpoint contained in the socket.
 *         Its value should be different from #GPR_NET_INVALID_SOCKET at exit
 *
 * \retval #GPR_ERR_OK The socket is ready to listen
 * \retval #GPR_ERR_INVALID_PARAMETER The socket is \c NULL (for \e DEBUG mode only)
 * \retval #GPR_ERR_NETWORK_ERROR One of the steps failed
 */
static enum GPR_Err init_server_socket(struct gpr_socket *sock)
{
    int err, yes = 1;

#ifdef DEBUG
    /* Check consistency */
    if (sock == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid socket");
#endif

    /* Get a file descriptor */
    sock->socket = socket(sock->cur->ai_family, sock->cur->ai_socktype, sock->cur->ai_protocol);
    if (sock->socket == GPR_NET_INVALID_SOCKET)
        return gpr_err_raise(GPR_ERR_NETWORK_ERROR, "socket: [Errno %d] %s", errno, strerror(errno));

    /* Set socket options */
    err = setsockopt(sock->socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    if (err < 0)
        return gpr_err_raise(GPR_ERR_NETWORK_ERROR, "setsockopt: [Errno %d] %s", errno, strerror(errno));

    /* Non-blocking operations */
    if (sock->nonblock)
    {
        err = fcntl(sock->socket, F_SETFL, O_NONBLOCK);
        if (err < 0)
            return gpr_err_raise(GPR_ERR_NETWORK_ERROR, "fcntl: [Errno %d] %s", errno, strerror(errno));
    }

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

/**
 * \brief Initializes a socket for a client communication
 *
 * \param[in,out] sock  Socket to initialize
 * \param[in]     local Local service number or known service name to bind to or \c NULL
 *
 * \return This function initializes the communication endpoint contained in the socket.
 *         Its value should be different from #GPR_NET_INVALID_SOCKET at exit
 *
 * \retval #GPR_ERR_OK The socket is ready to connect
 * \retval #GPR_ERR_INVALID_PARAMETER The socket is \c NULL (for \e DEBUG mode only)
 * \retval #GPR_ERR_NETWORK_ERROR One of the steps failed
 */
static enum GPR_Err init_client_socket(struct gpr_socket *sock, const char *local)
{
    int err, yes = 1;

#ifdef DEBUG
    /* Check consistency */
    if (sock == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid socket");
#endif

    /* Get a file descriptor */
    sock->socket = socket(sock->cur->ai_family, sock->cur->ai_socktype, sock->cur->ai_protocol);
    if (sock->socket == GPR_NET_INVALID_SOCKET)
        return gpr_err_raise(GPR_ERR_NETWORK_ERROR, "socket: [Errno %d] %s", errno, strerror(errno));

    /* Set socket options */
    if (local != NULL)
    {
        err = setsockopt(sock->socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        if (err < 0)
            return gpr_err_raise(GPR_ERR_NETWORK_ERROR, "setsockopt: [Errno %d] %s", errno, strerror(errno));
    }

    /* Non-blocking operations */
    if (sock->nonblock)
    {
        err = fcntl(sock->socket, F_SETFL, O_NONBLOCK);
        if (err < 0)
            return gpr_err_raise(GPR_ERR_NETWORK_ERROR, "fcntl: [Errno %d] %s", errno, strerror(errno));
    }

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

/**
 * \brief Binds and listens to the service contained in the socket
 *
 * \param[in,out] sock    Socket to bind/listen to
 * \param[in]     backlog Number of connections allowed on the incoming accepted queue before
 *                        rejecting them for connection-oriented sockets (Please consult
 *                        \c listen function and \c SOMAXCONN definition)
 *
 * \retval #GPR_ERR_OK The socket is listening
 * \retval #GPR_ERR_INVALID_PARAMETER The socket is \c NULL (for \e DEBUG mode only)
 * \retval #GPR_ERR_NETWORK_ERROR One of the steps failed
 */
static enum GPR_Err listen_to_service(struct gpr_socket *sock, int backlog)
{
    int err;

#ifdef DEBUG
    /* Check consistency */
    if (sock == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid socket");
#endif

    /* Bind service */
    err = bind(sock->socket, sock->cur->ai_addr, sock->cur->ai_addrlen);
    if (err < 0)
        return gpr_err_raise(GPR_ERR_NETWORK_ERROR, "bind: [Errno %d] %s", errno, strerror(errno));

    /* Listen to service (connection-oriented sockets only) */
    if ((sock->cur->ai_socktype & SOCK_STREAM) || (sock->cur->ai_socktype & SOCK_SEQPACKET))
    {
        err = listen(sock->socket, backlog);
        if (err < 0)
            return gpr_err_raise(GPR_ERR_NETWORK_ERROR, "listen: [Errno %d] %s", errno, strerror(errno));
    }

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

/**
 * \brief Performs a connection attempt to a remote peer
 *
 * \param[in,out] sock  Socket to connect to
 * \param[in]     local Local service number or known service name to bind to or \c NULL
 *
 * \retval #GPR_ERR_OK The socket is connected
 * \retval #GPR_ERR_INVALID_PARAMETER The socket is \c NULL (for \e DEBUG mode only)
 * \retval #GPR_ERR_PENDING The socket is nonblocking and the connection cannot be completed immediately.\n
 *         Please recall this function later
 * \retval #GPR_ERR_NETWORK_ERROR One of the steps failed
 */
static enum GPR_Err connect_to_peer(struct gpr_socket *sock, const char *local)
{
    int err;

#ifdef DEBUG
    /* Check consistency */
    if (sock == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid socket");
#endif

    /* Connect to peer */
    if (sock->state != GPR_NET_STATE_CONNECTING)
    {
        // Bind to local service
        if (local != NULL)
        {
            err = bind_to_local_service(sock, local);
            if (err != GPR_ERR_OK)
                return err;
        }

        // First connection attempt
        if (connect(sock->socket, sock->cur->ai_addr, sock->cur->ai_addrlen) < 0)
        {
            if (errno == EINPROGRESS)
            {
                sock->state = GPR_NET_STATE_CONNECTING;
                return check_connection_progress(sock);
            }
            else
            {
                return gpr_err_raise(GPR_ERR_NETWORK_ERROR, "connect: [Errno %d] %s", errno, strerror(errno));
            }
        }
    }
    else
    {
        // Check connection progress for nonblocking socket
        return check_connection_progress(sock);
    }

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

/**
 * \brief Binds to an identified local service
 *
 * \param sock  Socket to bind to
 * \param local Local service number or known service name to bind to or \c NULL
 *
 * \retval #GPR_ERR_OK The socket is binded
 * \retval #GPR_ERR_INVALID_PARAMETER The socket is \c NULL or the local service is \c NULL
 *         (for \e DEBUG mode only)
 * \retval #GPR_ERR_NETWORK_ERROR One of the steps failed
 */
static enum GPR_Err bind_to_local_service(struct gpr_socket *sock, const char *local)
{
    int err;
    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *cur;

#ifdef DEBUG
    /* Check consistency */

    // Check socket
    if (sock == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid socket");

    // Check local service
    if (local == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid local service");
#endif

    /* Resolve localhost */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = sock->cur->ai_family;
    hints.ai_socktype = sock->cur->ai_socktype;
    hints.ai_protocol = sock->cur->ai_protocol;
    hints.ai_flags = sock->cur->ai_flags;

    err = getaddrinfo(NULL, local, &hints, &res);
    if (err != 0)
        return gpr_err_raise(GPR_ERR_NETWORK_ERROR, "getaddrinfo: %s", gai_strerror(err)); // gai_strerror should be trhead-safe

    // Loop through network address and bind to the first we can
    for (cur = res; cur != NULL; cur = cur->ai_next)
    {
        err = bind(sock->socket, cur->ai_addr, cur->ai_addrlen);
        if (err == 0)
            break;
    }
    freeaddrinfo(res);

    if (cur == NULL)
        return gpr_err_raise(GPR_ERR_NETWORK_ERROR, "bind: [Errno %d] %s", errno, strerror(errno));

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

/**
 * \brief Checks the result of a connection attempt for a nonblocking socket
 *
 * \param[in] sock Socket to check
 *
 * \retval #GPR_ERR_OK The socket is connected
 * \retval #GPR_ERR_INVALID_PARAMETER The socket is \c NULL (for \e DEVEL mode only)
 * \retval #GPR_ERR_NETWORK_ERROR One of the steps failed
 */
static enum GPR_Err check_connection_progress(struct gpr_socket *sock)
{
    int ret, opt;
    struct pollfd fds[] = {{sock->socket, POLLOUT, 0}};

#ifdef DEBUG
    /* Check consistency */
    if (sock == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid socket");
#endif

    /* Check connection progress */
    ret = poll(fds, 1, 0);
    if (ret < 0)
        return gpr_err_raise(GPR_ERR_NETWORK_ERROR, "poll: [Errno %d] %s", errno, strerror(errno));
    else if (ret == 0)
        return gpr_err_raise(GPR_ERR_NETWORK_ERROR, "Connection to peer in progress");

    socklen_t opt_len = sizeof(int);
    if (getsockopt(sock->socket, SOL_SOCKET, SO_ERROR, &opt, &opt_len) < 0)
        return gpr_err_raise(GPR_ERR_NETWORK_ERROR, "getsockopt: [Errno %d] %s", errno,  strerror(errno));

    if (opt != 0)
        return gpr_err_raise(GPR_ERR_NETWORK_ERROR, "connect/getsockopt: [Errno %d] %s", errno, strerror(errno));

    return gpr_err_raise(GPR_ERR_OK, NULL);
}
