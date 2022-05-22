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

#include <errno.h>      // errno
#include <fcntl.h>      // fcntl
#include <netdb.h>      // addrinfo, getaddrinfo, freeaddrinfo, gai_strerror
#include <stdlib.h>     // malloc, free
#include <string.h>     // memset
#include <sys/socket.h> // socket, setsockopt, bind, listen, connect
#include <unistd.h>     // close

#include "gpr_err.h"
#include "gpr_utils.h"

static void close_socket(struct gpr_socket *sock);
static enum GPR_Err resolve_hostname(struct gpr_socket *sock, const char *addr, const char *service);
static enum GPR_Err prepare_server_socket(struct gpr_socket *sock);
static enum GPR_Err prepare_client_socket(struct gpr_socket *sock);
static enum GPR_Err listen_to_service(struct gpr_socket *sock, int backlog);
static enum GPR_Err connect_to_peer(struct gpr_socket *sock);

struct gpr_socket *gpr_net_new_socket(int domain, int type, int protocol, int flags, bool nonblock)
{
    struct gpr_socket *sock = NULL;

    /* Allocate GPR socket */
    sock = (struct gpr_socket *)malloc(sizeof(struct gpr_socket));
    if (UNLIKELY(sock == NULL))
        return NULL;

    /* Initialize GPR socket */
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

    /* Initialize GPR socket */
    sock->socket = GPR_INVALID_SOCKET;
    sock->state = GPR_NET_STATE_CLOSED;
    memset(&(sock->hints), 0, sizeof(struct addrinfo));
    sock->hints.ai_family = domain;
    sock->hints.ai_socktype = type;
    sock->hints.ai_protocol = protocol;
    sock->hints.ai_flags = flags;
    sock->nonblock = nonblock;
}

enum GPR_Err gpr_net_close_socket(struct gpr_socket *sock)
{
#ifdef DEBUG
    /* Check consistency */
    if (sock == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid GPR socket");
#endif

    /* Close GPR socket */
    close_socket(sock);

    /* Free GPR socket components */
    if (sock->res != NULL)
        freeaddrinfo(sock->res);
    sock->res = NULL;
    sock->cur = NULL;

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

void gpr_net_free_socket(struct gpr_socket *sock)
{
#ifdef DEBUG
    /* Check consistency */
    if (sock == NULL)
        return;
#endif

    /* Close GPR socket */
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

enum GPR_Err gpr_net_listen(struct gpr_socket *sock, const char *addr, const char *service, int backlog)
{
    enum GPR_Err err;

#ifdef DEBUG
    /* Check consistency */

    // Check GPR socket
    if (sock == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid GPR socket");

    // Check address
    if (addr == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid address/hostname");

    // Check service
    if (service == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid service");
#endif

    /* Resolve hostname */
    err = resolve_hostname(sock, addr, service);
    if (err != GPR_ERR_OK)
        return err;

    // Loop through network addresses and bind to the first we can
    for (sock->cur = sock->res; sock->cur != NULL; sock->cur = sock->cur->ai_next)
    {
        err = prepare_server_socket(sock);
        if (err != GPR_ERR_OK)
        {
            close_socket(sock);
            continue; // Abort current network address
        }

        err = listen_to_service(sock, backlog);
        if (err != GPR_ERR_OK)
        {
            close_socket(sock);
            continue; // Abort current network address
        }
    }

    // All done with network addresses
    freeaddrinfo(sock->res);
    sock->res = NULL;
    sock->cur = NULL;
    sock->state = GPR_NET_STATE_LISTENING;

    return err;
}

enum GPR_Err gpr_net_connect(struct gpr_socket *sock, const char *addr, const char *service)
{
    enum GPR_Err err;

#ifdef DEBUG
    /* Check consistency */

    // Check GPR socket
    if (sock == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid GPR socket");

    // Check address
    if (addr == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid address/hostname");

    // Check service
    if (service == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid service");
#endif

    /* Resolve hostname */
    err = resolve_hostname(sock, addr, service);
    if (err != GPR_ERR_OK)
        return err;

    // Loop through network addresses and bind to the first we can
    for (sock->cur = sock->res; sock->cur != NULL; sock->cur = sock->cur->ai_next)
    {
        err = prepare_client_socket(sock);
        if (err != GPR_ERR_OK)
        {
            close_socket(sock);
            continue; // Abort current network address
        }

        err = connect_to_peer(sock);
        if (err != GPR_ERR_OK)
        {
            close_socket(sock);
            continue; // Abort current network address
        }
    }

    // All done with network addresses
    freeaddrinfo(sock->res);
    sock->res = NULL;
    sock->cur = NULL;
    sock->state = GPR_NET_STATE_CONNECTED;

    return err;
}

/**
 * \brief Closes communication endpoint of a GPR socket
 *
 * \param[out] sock GPR socket to close
 */
static void close_socket(struct gpr_socket *sock)
{
#ifdef DEBUG
    /* Check consistency */
    if (sock == NULL)
        return;
#endif

    if (sock->socket != GPR_INVALID_SOCKET)
        close(sock->socket);
    sock->socket = GPR_INVALID_SOCKET;
    sock->state = GPR_NET_STATE_CLOSED;
}

/**
 * \brief Resolves hostname according to \p addr and \p service
 *
 * \todo The function \c getaddrinfo is known to be synchronous.
 *       If its really a problem, a dedicated thread should be considered here
 *
 * \param[in,out] sock    GPR socket containing communication criterias
 * \param[in]     addr    Address/hostname to bind/connect to
 * \param[in]     service Service to listen/connect to
 *
 * \return This function allocates and initializes a linked list of \c addrinfo
 *         structures in the GPR socket, one for each network address that matches
 *         \p addr and \p service
 *
 * \retval #GPR_ERR_OK Network addresses found
 * \retval #GPR_ERR_INVALID_PARAMETER The socket is \c NULL or the address is \c NULL
 *         or the service is \c NULL (for \e DEBUG mode only)
 * \retval #GPR_ERR_NETWORK_ERROR No network addresses found or an error occured
 */
static enum GPR_Err resolve_hostname(struct gpr_socket *sock, const char *addr, const char *service)
{
#ifdef DEBUG
    /* Check consistency */

    // Check GPR socket
    if (sock == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid GPR socket");

    // Check address
    if (addr == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid address/hostname");

    // Check service
    if (service == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid service");
#endif

    enum GPR_Err err;

    /* Search peer endpoint */
    err = getaddrinfo(addr, service, &(sock->hints), &(sock->res));
    if (err != 0)
        return gpr_err_raise(GPR_ERR_NETWORK_ERROR, "getaddrinfo: %s", gai_strerror(err)); // gai_strerror should be thread-safe

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

/**
 * \brief Prepares GPR socket for a server communication
 *
 * \param[in,out] sock GPR socket to prepare
 *
 * \return This function prepares the communication endpoint contained in the GPR socket.
 *         Its value should be different from #GPR_INVALID_SOCKET at exit
 *
 * \retval #GPR_ERR_OK The GPR socket is ready to listen
 * \retval #GPR_ERR_INVALID_PARAMETER The socket is \c NULL (for \e DEBUG mode only)
 * \retval #GPR_ERR_NETWORK_ERROR One of the steps failed
 */
static enum GPR_Err prepare_server_socket(struct gpr_socket *sock)
{
#ifdef DEBUG
    /* Check consistency */
    if (sock == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid GPR socket");
#endif

    int err, yes = 1;

    /* Get a file descriptor */
    sock->socket = socket(sock->cur->ai_family, sock->cur->ai_socktype, sock->cur->ai_protocol);
    if (sock->socket == GPR_INVALID_SOCKET)
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
 * \brief Prepares GPR socket for a client communication
 *
 * \param[in,out] sock GPR socket to prepare
 *
 * \return This function prepares the communication endpoint contained in the GPR socket.
 *         Its value should be different from #GPR_INVALID_SOCKET at exit
 *
 * \retval #GPR_ERR_OK The GPR socket is ready to connect
 * \retval #GPR_ERR_INVALID_PARAMETER The socket is \c NULL (for \e DEBUG mode only)
 * \retval #GPR_ERR_NETWORK_ERROR One of the steps failed
 */
static enum GPR_Err prepare_client_socket(struct gpr_socket *sock)
{
#ifdef DEBUG
    /* Check consistency */
    if (sock == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid GPR socket");
#endif

    /* Get a file descriptor */
    sock->socket = socket(sock->cur->ai_family, sock->cur->ai_socktype, sock->cur->ai_protocol);
    if (sock->socket == GPR_INVALID_SOCKET)
        return gpr_err_raise(GPR_ERR_NETWORK_ERROR, "socket: [Errno %d] %s", errno, strerror(errno));

    /* Non-blocking operations */
    // TODO : Handle async here

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

/**
 * \brief Binds and listens to the service contained in the GPR socket
 *
 * \param[in,out] sock    GPR socket to bind/listen to
 * \param[in]     backlog Number of connections allowed on the incoming accepted queue before
 *                        rejecting them (Please consult \c listen function and \c SOMAXCONN definition)
 *
 * \retval #GPR_ERR_OK The GPR socket is listening
 * \retval #GPR_ERR_INVALID_PARAMETER The socket is \c NULL (for \e DEBUG mode only)
 * \retval #GPR_ERR_NETWORK_ERROR One of the steps failed
 */
static enum GPR_Err listen_to_service(struct gpr_socket *sock, int backlog)
{
    int err;

#ifdef DEBUG
    /* Check consistency */
    if (sock == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid GPR socket");
#endif

    /* Bind service */
    err = bind(sock->socket, sock->cur->ai_addr, sock->cur->ai_addrlen);
    if (err < 0)
        return gpr_err_raise(GPR_ERR_NETWORK_ERROR, "bind: [Errno %d] %s", errno, strerror(errno));

    /* Listen to service */
    err = listen(sock->socket, backlog);
    if (err < 0)
        return gpr_err_raise(GPR_ERR_NETWORK_ERROR, "listen: [Errno %d] %s", errno, strerror(errno));

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

/**
 * \brief Connects to an \c addr on a \c service
 *
 * \param[in,out] sock GPR socket to connect to
 *
 * \retval #GPR_ERR_OK The GPR socket is connected
 * \retval #GPR_ERR_INVALID_PARAMETER The socket is \c NULL (for \e DEBUG mode only)
 * \retval #GPR_ERR_NETWORK_ERROR One of the steps failed
 */
static enum GPR_Err connect_to_peer(struct gpr_socket *sock)
{
    int err;

#ifdef DEBUG
    /* Check consistency */
    if (sock == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid GPR socket");
#endif

    // TODO: bind here if user wants a specific port

    /* Connect to peer */
    err = connect(sock->socket, sock->cur->ai_addr, sock->cur->ai_addrlen);
    if (err < 0)
        return gpr_err_raise(GPR_ERR_NETWORK_ERROR, "connect: [Errno %d] %s", errno, strerror(errno));

    return gpr_err_raise(GPR_ERR_OK, NULL);
}
