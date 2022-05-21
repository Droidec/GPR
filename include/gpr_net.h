/******************************************************************************
 *
 * \file gpr_net.h
 * \brief Network module
 * \details
 * This module offers a way to open network services and/or connect to a
 * remote host, with a SSL layer if needed\n
 * The primary goal of this module is to hide the painful manipulation of
 * sockets and/or known SSL encryption libraries\n
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

#include <netdb.h> // addrinfo

#include "gpr_err.h"

/**
 * \brief GPR sockets statuses showing network progression
 */
enum GPR_Net
{
    GPR_NET_NONE,    ///< First connection attempt
    GPR_NET_PENDING, ///< Connection establishment in progress
    GPR_NET_OK,      ///< Establishment successful
    GPR_NET_ERR      ///< Establishment failure
};

/**
 * \brief GPR socket structure
 */
struct gpr_socket
{
    int socket;              ///< C-socket communication endpoint
    enum GPR_Net status;     ///< GPR socket establishment progress
    struct addrinfo info;    ///< Communication criterias
    struct addrinfo *result; ///< List of available network adresses
    struct addrinfo *curr;   ///< Current network address being tested
};

/**
 * \brief Allocates and initializes a new GPR socket used for communication
 *
 * \note For more information about parameters, please consult \c getaddrinfo
 * manual
 *
 * \param[in] domain   Address family
 * \param[in] type     Socket type or \c 0
 * \param[in] protocol Socket address protocol or \c 0
 * \param[in] flags    Additional flags or \c 0
 *
 * \return Returns a GPR socket allocated and initialized or \c NULL on allocation failure
 */
struct gpr_socket *gpr_net_new_socket(int domain, int type, int protocol, int flags);

/**
 * \brief Initializes a GPR socket used for communication
 *
 * \param[out] sock    GPR socket to initialize
 * \param[in] domain   Address family
 * \param[in] type     Socket type or \c 0
 * \param[in] protocol Socket address protocol or \c 0
 * \param[in] flags    Additional flags or \c 0
 */
void gpr_net_init_socket(struct gpr_socket *sock, int domain, int type, int protocol, int flags);

/**
 * \brief Closes a GPR socket
 *
 * \note Call this function to end an established connection on a GPR socket
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
 * \note Call this function if you allocated a GPR socket with #gpr_net_new_socket
 *
 * \param[out] sock GPR socket to free
 */
void gpr_net_free_socket(struct gpr_socket *sock);

/**
 * \brief Initiates a connection attempt with a GPR socket
 *
 * \param[in] sock    GPR socket to use
 * \param[in] addr    Peer address/hostname to connect to
 * \param[in] service Peer service to connect to
 *
 * \retval #GPR_ERR_OK The connection has been established
 * \retval #GPR_ERR_INVALID_PARAMETER One of the parameter is \c NULL (for \e DEBUG mode only)
 * \retval #GPR_ERR_NETWORK_ERROR A network error occurred
 */
enum GPR_Err gpr_net_connect(struct gpr_socket *sock, const char *addr, const char *service);

// ssize_t gpr_net_recv();
// ssize_t gpr_net_send();

#endif /* H_GPR_NETWORK */
