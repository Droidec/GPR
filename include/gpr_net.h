/******************************************************************************
 *
 * \file gpr_net.h
 * \brief Network module
 * \details
 * This module offers a way to open network services and/or connect to a
 * remote host, with a SSL layer if needed.\n
 * The primary goal of this module is to hide the painful manipulation of
 * sockets and/or known SSL encryption libraries
 *
 * \todo `strerror` is not thread-safe
 *
 * \warning WORK IN PROGRESS
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

#ifndef H_GPR_NETWORK
#define H_GPR_NETWORK

#include <netdb.h>      // addrinfo
#include <stdbool.h>    // bool
#include <stddef.h>     // size_t
#include <stdio.h>      // ssize_t
#include <stdint.h>     // uint16_t
#include <sys/socket.h> // sockaddr_storage

#include "gpr_err.h"

/**
 * \brief Socket states showing network progression
 */
enum GPR_Net_State
{
    GPR_NET_STATE_CLOSED,     ///< Socket closed
    GPR_NET_STATE_CLOSING,    ///< Shutdown in progress
    GPR_NET_STATE_CONNECTING, ///< Connection attempt in progress
    GPR_NET_STATE_CONNECTED,  ///< Socket connected to peer
    GPR_NET_STATE_LISTENING,  ///< Socket listening service
    GPR_NET_STATE_NUMBERS     ///< Number of socket states (*DO NOT USE*)
};

/**
 * \brief Socket types
 */
enum GPR_Net_Type
{
    GPR_NET_TYPE_UNKNOWN,             ///< Unknown socket type
    GPR_NET_TYPE_CONNECTION_ORIENTED, ///< Connection-oriented socket
    GPR_NET_TYPE_CONNECTION_LESS,     ///< Connection-less socket
    GPR_NET_TYPE_NUMBERS              ///< Number of connection types (*DO NOT USE*)
};

/**
 * \brief Socket structure
 */
struct gpr_socket
{
    /* Socket info */
    int socket;               ///< Socket communication endpoint
    enum GPR_Net_State state; ///< Socket network progression
    enum GPR_Net_Type type;   ///< Socket type
    struct addrinfo hints;    ///< Communication criterias
    struct addrinfo *res;     ///< List of available network addresses
    struct addrinfo *cur;     ///< Current network address being tested
    bool nonblock;            ///< Perform non-blocking operations

    /* Connection info */
    struct sockaddr_storage sock_info; ///< Socket informations
    struct sockaddr_storage peer_info; ///< Peer informations
};

/**
 * \brief Invalid socket reference value
 */
#define GPR_NET_INVALID_SOCKET -1

/**
 * \brief Allocates and initializes a new socket used for communication
 *
 * \note For more information about parameters, please consult \c socket and \c getaddrinfo
 * manuals
 *
 * \param[in] domain    Address family
 * \param[in] type      Socket type
 * \param[in] protocol  Socket address protocol
 * \param[in] flags     Additional flags
 * \param[in] nonblock  Nonblocking socket if true
 *
 * \return Returns a socket allocated and initialized or \c NULL on allocation failure
 */
struct gpr_socket *gpr_net_new_socket(int domain, int type, int protocol, int flags, bool nonblock);

/**
 * \brief Initializes a socket used for communication
 *
 * \note For more information about parameters, please consult \c socket and \c getaddrinfo
 * manuals
 *
 * \param[out] sock     Socket to initialize
 * \param[in]  domain   Address family
 * \param[in]  type     Socket type
 * \param[in]  protocol Socket address protocol
 * \param[in]  flags    Additional flags
 * \param[in]  nonblock Nonblocking socket if true
 */
void gpr_net_init_socket(struct gpr_socket *sock, int domain, int type, int protocol, int flags, bool nonblock);

/**
 * \brief Initializes a socket used for TCP communication
 *
 * \param[out] sock     Socket to initialize
 * \param[in]  nonblock Nonblocking socket if true
 */
void gpr_net_init_default_tcp_socket(struct gpr_socket *sock, bool nonblock);

/**
 * \brief Initializes a socket used for UDP communication
 *
 * \param[out] sock     Socket to initialize
 * \param[in]  nonblock Nonblocking socket if true
 */
void gpr_net_init_default_udp_socket(struct gpr_socket *sock, bool nonblock);

/**
 * \brief Copies content of socket \p in into socket \p out
 *
 * \param[in]  in  Socket to copy
 * \param[out] out Socket where to copy
 */
void gpr_net_copy_socket(const struct gpr_socket *in, struct gpr_socket *out);

/**
 * \brief Closes communication on a socket
 *
 * \note This function can be called after a communication is established or in
 *       progress to stop communications and free ressources
 *
 * \param[out] sock Socket to close
 */
void gpr_net_close_socket(struct gpr_socket *sock);

/**
 * \brief Frees a GPR socket
 *
 * \note If a communication is established or in progress, this function shutdowns
 *       the GPR socket first
 *
 * \note Call this function if you allocated a GPR socket with #gpr_net_new_socket
 *
 * \param[out] sock Socket to free
 */
void gpr_net_free_socket(struct gpr_socket *sock);

/**
 * \brief Gets socket state C-string
 *
 * \param[in] state State to stringify
 *
 * \return Returns the C-string representing socket state
 */
const char *gpr_net_socket_state_to_str(enum GPR_Net_State state);

/**
 * \brief Gets socket type C-string
 *
 * \param[in] type Type to stringify
 *
 * \return Returns the C-string representing socket type
 */
const char *gpr_net_socket_type_to_str(enum GPR_Net_Type type);

/**
 * \brief Listens to a \p service on the \p addr host (*Server side*)
 *
 * \note If the service is still in use (a fast restart of the server for example),
 *       this function will try to reuse the service as fast as possible with \c SO_REUSEADDR
 *
 * \param[in,out] sock    Socket to use
 * \param[in]     addr    Address/hostname to bind to or \c NULL (See \c AI_PASSIVE)
 * \param[in]     service Service number or known service name to listen to
 * \param[in]     backlog Number of connections allowed on the incoming accepted queue before
 *                        rejecting them for connection-oriented sockets (Please consult
 *                        \c listen function and \c SOMAXCONN definition)
 *
 * \retval #GPR_ERR_OK The socket is now listening
 * \retval #GPR_ERR_INVALID_PARAMETER The socket is \c NULL or the service is \c NULL
 *         (for \e DEBUG mode only)
 * \retval #GPR_ERR_NETWORK_ERROR A network error occured. Consult error message with #gpr_err_get_msg
 */
enum GPR_Err gpr_net_listen(struct gpr_socket *sock, const char *addr, const char *service, int backlog);

/**
 * \brief Connects to an \p addr on a \p service (*Client side*)
 *
 * \param[in,out] sock    Socket to use
 * \param[in]     addr    Adress/hostname to connect to
 * \param[in]     service Service number or known service name to listen to
 * \param[in]     local   Local service number or known service name to bind to or \c NULL
 *
 * \retval #GPR_ERR_OK The socket is now connected
 * \retval #GPR_ERR_INVALID_PARAMETER The socket is \c NULL or the address is \c NULL
 *         or the service is \c NULL (for \e DEBUG mode only)
 * \retval #GPR_ERR_PENDING The socket is nonblocking and the connection cannot be completed immediately.\n
 *         Please recall this function later
 * \retval #GPR_ERR_NETWORK_ERROR A network error occured. Consult error message with #gpr_err_get_msg
 */
enum GPR_Err gpr_net_connect(struct gpr_socket *sock, const char *addr, const char *service, const char *local);

/**
 * \brief Accepts incoming connection on \p in and fills \p out content with connection
 *        informations (*Server side*)
 *
 * \note This function initializes \p out before accepting a connection
 *
 * \note This function is intended for connection-oriented sockets only (See \c accept)
 *
 * \note If \p in is a nonblocking socket, \p out will also be nonblocking
 *
 * \param[in]  in  Socket where an incoming connection is to be accepted
 * \param[out] out Socket to fill with connection informations
 *
 * \retval #GPR_ERR_OK A connection has been accepted and \p out contains connection informations
 * \retval #GPR_ERR_INVALID_PARAMETER The \p in is \c NULL or the \p out is \c NULL
 *         (for \e DEBUG mode only)
 * \retval #GPR_ERR_PENDING The socket is nonblocking and there is no connection to be accepted.\n
 *         Please recall this function later
 * \retval #GPR_ERR_NETWORK_ERROR A network error occured. Consult error message with #gpr_err_get_msg
 */
enum GPR_Err gpr_net_accept(const struct gpr_socket *in, struct gpr_socket *out);

/**
 * \brief Receives a message from a socket
 *
 * \note This function should only be called after a successful call to:
 *       - #gpr_net_connect or #gpr_net_accept for connection-oriented sockets
 *       - #gpr_net_connect or #gpr_net_listen otherwise
 *
 * \param[in]  sock  Socket where to receive a message from
 * \param[out] buf   Buffer where to store the message
 * \param[in]  size  Size of the buffer (maximum bytes to be read)
 * \param[in]  flags Read options (See \c recv or \c recvfrom)
 *
 * \return Returns the number of bytes received or -1 if an error occured.
 *         In the event of an error, \c errno is set to indicate the error.\n
 *         When a stream socket peer has performed an orderly shutdown, the
 *         return value will be \c 0.\n
 *         If the socket is nonblocking, the return value may be \c 0 with errno
 *         sets to EAGAIN or EWOULDBLOCK
 */
ssize_t gpr_net_recv(const struct gpr_socket *sock, void *buf, size_t size, int flags);

/**
 * \brief Sends a message to another socket
 *
 * \note This function should be called after a successful call to:
 *       - #gpr_net_connect or #gpr_net_accept for connection-oriented sockets
 *       - #gpr_net_connect or #gpr_net_listen otherwise
 *
 * \param[in]  sock  Socket where to send a message to
 * \param[out] buf   Buffer containing the message to send
 * \param[in]  size  Size of the buffer
 * \param[in]  flags Send options (See \c send or \c sento)
 *
 * \return Returns the number of bytes sent or -1 if an error occured.
 *         In the event of an error, \c errno is set to indicate the error.\n
 *         If the socket is nonblocking, the return value may be \c 0 with errno
 *         sets to EAGAIN or EWOULDBLOCK
 */
ssize_t gpr_net_send(const struct gpr_socket *sock, void *buf, size_t size, int flags);

/**
 * \brief Returns socket informations
 *
 * \note This function should only be called after a successful call to:
 *       - #gpr_net_listen on server side
 *       - #gpr_net_connect on client side
 *
 * \warning The given \p addr should be at least `INET6_ADDRSTRLEN` bytes long
 *
 * \param[in]  sock Socket where to get informations from
 * \param[out] addr Buffer where to copy socket address
 * \param[out] port Buffer where to copy source port
 *
 * \retval #GPR_ERR_OK Socket informations have been copied
 * \retval #GPR_ERR_INVALID_PARAMETER The socket is \c NULL or the address buffer is \c NULL
 *         or the port buffer is \c NULL (for \e DEVEL mode only)
 * \retval #GPR_ERR_NETWORK_ERROR The socket family is not handled
 */
enum GPR_Err gpr_net_get_socket_info(const struct gpr_socket *sock, char *addr, uint16_t *port);

/**
 * \brief Returns peer informations from a socket
 *
 * \param[in]  sock Socket where to get peer informations from
 * \param[out] addr Buffer where to copy peer address
 * \param[out] port Buffer where to copy peer source/destination port
 *
 * \retval #GPR_ERR_OK Peer informations have been copied
 * \retval #GPR_ERR_INVALID_PARAMETER The socket is \c NULL or the address buffer is \c NULL
 *         or the port buffer is \c NULL (for \e DEVEL mode only)
 * \retval #GPR_ERR_NETWORK_ERROR The socket family is not handled
 */
enum GPR_Err gpr_net_get_peer_info(const struct gpr_socket *sock, char *addr, uint16_t *port);

#endif /* H_GPR_NETWORK */
