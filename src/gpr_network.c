/******************************************************************************
 *
 * \file gpr_network.c
 * \brief Network module
 *
 ******************************************************************************
 *
 * \copyright Copyright (c) 2019-2021, GPR Team
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
 * * Neither the name of the GPR Team nor the names of its contributors may
 *   be used to endorse or promote products derived from this software without
 *   specific prior written permission.
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

#include "gpr_network.h"
#include "gpr_builtin.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/******************************************************************************
 * Private prototypes
 *****************************************************************************/

static enum GPR_Err search_peer_endpoint(struct gpr_socket *sock, const char *const addr, const char *const service);
static enum GPR_Err connect_to_peer(struct gpr_socket *sock);

/******************************************************************************
 * Public functions
 *****************************************************************************/

struct gpr_socket *gpr_net_create_socket(int domain, int type, int protocol, int flags)
{
    struct gpr_socket *sock = NULL;

    /* Allocate GPR socket */
    sock = (struct gpr_socket *)malloc(sizeof(struct gpr_socket));
    if (UNLIKELY(sock == NULL))
        return NULL;

    /* Initialize GPR socket */
    sock->socket = -1;
    sock->status = GPR_NET_NONE;
    memset(&(sock->info), 0, sizeof(struct addrinfo));
    sock->info.ai_family = domain;
    sock->info.ai_socktype = type;
    sock->info.ai_protocol = protocol;
    sock->info.ai_flags = flags;
    sock->result = NULL;
    sock->curr = NULL;

    return sock;
}

enum GPR_Err gpr_net_close_socket(struct gpr_socket *sock)
{
#ifdef DEBUG
    /* Check consistency */
    if (sock == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid GPR socket");
#endif

    /* Close GPR socket */
    if (sock->socket != -1)
        close(sock->socket);
    sock->socket = -1;

    /* Deallocate GPR socket components */
    if (sock->result != NULL)
        freeaddrinfo(sock->result);

    /* Deallocate GPR socket */
    free(sock);

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

enum GPR_Err gpr_net_connect(struct gpr_socket *sock, const char *const addr, const char *const service)
{
    enum GPR_Err err;

#ifdef DEBUG
    /* Check consistency */

    // Check GPR socket
    if (sock == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid GPR socket");

    // Check address
    if (addr == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid peer address/hostname");

    // Check service
    if (service == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid peer service");
#endif

    // TODO: Check if user didn't changed "addr" and "service" after first call, and raise error if so

    switch (sock->status)
    {
        case GPR_NET_NONE:
            /* First connection attempt */

            // Search peer endpoint
            err = search_peer_endpoint(sock, addr, service);
            if (err != GPR_ERR_OK)
                return err;

            // Network addresses have been found
            sock->status = GPR_NET_PENDING;

            /* fall through */

        case GPR_NET_PENDING:
            /* Connection in progress */
            err = connect_to_peer(sock);
            if (err != GPR_ERR_OK)
                return err;

            break;

        default:
            /* Should not happen */
            return gpr_err_raise(GPR_ERR_KO, "Impossible case reached");
    }

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

/******************************************************************************
 * Private functions
 *****************************************************************************/

/******************************************************************************
 *
 * \brief Search network addresses according to peer endpoint
 *
 * \param sock    GPR socket to use
 * \param addr    Peer address/hostname to connect to
 * \param Service Peer service to connect to
 *
 * \return
 *     GPR_ERR_OK: Network adresses found\n
 *     GPR_ERR_NETWORK_ERROR: No network addresses found
 *
 *****************************************************************************/
static enum GPR_Err search_peer_endpoint(struct gpr_socket *sock, const char *const addr, const char *const service)
{
    int err;

    /* Search peer endpoint */
    err = getaddrinfo(addr, service, &(sock->info), &(sock->result)); // WARN: Synchronous and no timeout
    if (err != 0)
        return gpr_err_raise(GPR_ERR_NETWORK_ERROR, "%s", gai_strerror(err));

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

/*****************************************************************************
 *
 * \brief Attempt to connect to peer by testing all network addressed stored
 * inside the given GPR socket
 *
 * \param sock    GPR socket to use
 * \param addr    Peer address/hostname to connect to
 * \param Service Peer service to connect to
 *
 * \return
 *     GPR_ERR_OK: The connection to one of the network addresses has been
 *     successful\n
 *     GPR_ERR_NETWORK_ERROR: All network addresses connection attempt
 *     failed
 *
 *****************************************************************************/
static enum GPR_Err connect_to_peer(struct gpr_socket *sock)
{
    int err;

    /* Connect GPR socket */
    for (sock->curr = sock->result; sock->curr != NULL; sock->curr = sock->curr->ai_next)
    {
        sock->socket = socket(sock->curr->ai_family, sock->curr->ai_socktype, sock->curr->ai_protocol);

        if (sock->socket == -1)
            continue;

        // TODO: If async, socket must be put in non blocking mode (fcntl)
        // If async, connect can return -1 with errno set to EINPROGRESS. connect can then be tested with a select

        err = connect(sock->socket, sock->curr->ai_addr, sock->curr->ai_addrlen);
        if (err != -1)
            break; // Reachable network address found

        close(sock->socket);
    }

    if (sock->curr == NULL)
    {
        // All network addresses are unreachable
        return gpr_err_raise(GPR_ERR_NETWORK_ERROR, "Peer is unreachable");
    }

    return gpr_err_raise(GPR_ERR_OK, NULL);
}
