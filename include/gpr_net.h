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
 * \c domain argument:
 * - AF_INET: IPv4 protocol
 * - AF_INET6: IPv6 protocol
 * - AF_UNSPEC: IPv4 or IPv6 protocol
 *
 * \c type argument:
 * - SOCK_STREAM: TCP socket
 * - SOCK_DGRAM: UDP socket
 * - SOCK_NONBLOCK: Linux specific, put socket in non-blocking mode
 *
 * \c protocol argument:
 * - Set to \c 0 if \c domain and \c type are set or a specific value otherwise
 *
 * \c flags argument:
 * - AI_PASSIVE: bind to local host
 *
 * \warning WORK IN PROGRESS
 *
 * Current interrogations:
 * - How to use gpr_net_listen function with localhost interface ?
 * - How to use gpr_net_connect function and choose the binding port (bind) ?
 * - strerror not thread-safe !
 * - bind/listen for UDP sockets ?
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

#include <netdb.h>   // addrinfo
#include <stdbool.h> // bool

#include "gpr_err.h"

/**
 * \brief GPR socket states showing network progression
 */
enum GPR_Net_State
{
    GPR_NET_STATE_CLOSED,     ///< Socket closed
    GPR_NET_STATE_CLOSING,    ///< Shutdown in progress
    GPR_NET_STATE_CONNECTING, ///< Connection establishment in progress
    GPR_NET_STATE_CONNECTED,  ///< Socket connected to peer
    GPR_NET_STATE_LISTENING,  ///< Socket listening service
    GPR_NET_STATE_NUMBERS     ///< Number of socket states (*DO NOT USE*)
};

struct gpr_socket
{
    int socket;               ///< Socket communication endpoint
    enum GPR_Net_State state; ///< Socket network progression
    struct addrinfo hints;    ///< Communication criterias
    struct addrinfo *res;     ///< List of available network addresses
    struct addrinfo *cur;     ///< Current network address being tested
    bool nonblock;            ///< Perform non-blocking operations
};

/**
 * \brief Invalid GPR socket reference value
 */
#define GPR_INVALID_SOCKET -1

/**
 * \brief Allocates and initializes a new GPR socket used for communication
 *
 * \note For more information about parameters, please consult \c socket and \c getaddrinfo
 * manuals
 *
 * \warning For Linux >= 2.6.27, \c SOCK_NONBLOCK can be used instead of \p nonblock parameter
 *
 * \param[in] domain    Address family
 * \param[in] type      Socket type
 * \param[in] protocol  Socket address protocol
 * \param[in] flags     Additional flags
 * \param[in] nonblock  Set socket in non-blocking state if true
 *
 * \return Returns a GPR socket allocated and initialized or \c NULL on allocation failure
 */
struct gpr_socket *gpr_net_new_socket(int domain, int type, int protocol, int flags, bool nonblock);

/**
 * \brief Initializes a GPR socket used for communication
 *
 * \note For more information about parameters, please consult \c socket and \c getaddrinfo
 * manuals
 *
 * \warning For Linux >= 2.6.27, \c SOCK_NONBLOCK can be used instead of \p nonblock parameter
 *
 * \param[out] sock     GPR socket to initialize
 * \param[in]  domain   Address family
 * \param[in]  type     Socket type
 * \param[in]  protocol Socket address protocol
 * \param[in]  flags    Additional flags
 * \param[in]  nonblock Set socket in non-blocking state if true
 */
void gpr_net_init_socket(struct gpr_socket *sock, int domain, int type, int protocol, int flags, bool nonblock);

/**
 * \brief Closes communication on a GPR socket
 *
 * \note This function can be called after a communication is established or in
 *       progress to stop communications and free ressources
 *
 * \param[out] sock GPR socket to close
 *
 * \retval #GPR_ERR_OK The socket has been closed
 * \retval #GPR_ERR_INVALID_PARAMETER The socket is \c NULL (for \e DEBUG mode only)
 */
enum GPR_Err gpr_net_close_socket(struct gpr_socket *sock);

/**
 * \brief Frees a GPR socket
 *
 * \note If a communication is established or in progress, this function shutdowns
 *       the GPR socket first
 *
 * \note Call this function if you allocated a GPR socket with #gpr_net_new_socket
 *
 * \param[out] sock GPR socket to free
 */
void gpr_net_free_socket(struct gpr_socket *sock);

/**
 * \brief Gets GPR socket state string
 *
 * \param[in] state State to stringify
 *
 * \return Returns the C-string representing GPR socket state
 */
const char *gpr_net_socket_state_to_str(enum GPR_Net_State state);

/**
 * \brief Listens to a \p service on the \c addr host (*Server side*)
 *
 * \note If the service is still in use (a fast restart of the server for example),
 *       this function will try to reuse the service as fast as possible with \c SO_REUSEADDR
 *
 * \param[in,out] sock    GPR socket to use
 * \param[in]     addr    Address/hostname to bind to
 * \param[in]     service Service number or known service name to listen to
 * \param[in]     backlog Number of connections allowed on the incoming accepted queue before
 *                        rejecting them (Please consult \c listen function and \c SOMAXCONN definition)
 *
 * \retval #GPR_ERR_OK The service is now listening
 * \retval #GPR_ERR_INVALID_PARAMETER The socket is \c NULL or the address is \c NULL
 *         or the service is \c NULL (for \e DEBUG mode only)
 * \retval #GPR_ERR_NETWORK_ERROR A network error occured. Consult error message with #gpr_err_get_msg
 */
enum GPR_Err gpr_net_listen(struct gpr_socket *sock, const char *addr, const char *service, int backlog);

#endif /* H_GPR_NETWORK */
