/*****************************************************************************
 *
 * gpr_network.c
 *
 *****************************************************************************
 * Copyright © 2020 Marc GIANNETTI
 *
 * Network management functions
 *
 *****************************************************************************
 *
 * DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *             Version 2, December 2004
 *
 * Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
 *
 * Everyone is permitted to copy and distribute verbatim or modified
 * copies of this license document, and changing it is allowed as long
 * as the name is changed.
 *
 *            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 * TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 *
 * 0. You just DO WHAT THE FUCK YOU WANT TO.
 *
 *****************************************************************************/

#include "gpr_network.h"
#include "gpr_builtin.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*****************************************************************************
 * Private prototypes
 *****************************************************************************/

static enum GPR_Err search_peer_endpoint(struct gpr_socket *sock, const char * const addr, const char * const service);
static enum GPR_Err connect_to_peer(struct gpr_socket *sock);

/*****************************************************************************
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
    /* Check consistency */
    if (sock == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid GPR socket");

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

enum GPR_Err gpr_net_connect(struct gpr_socket *sock, const char * const addr, const char * const service)
{
    enum GPR_Err err;

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

    // TODO: Check if user didn't changed "addr" and "service" after first call, and raise error if so

    switch(sock->status)
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
            return gpr_err_raise(GPR_ERR_KO, "Network module error");
    }

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

/*****************************************************************************
 * Private functions
 *****************************************************************************/

/*****************************************************************************
 *
 * Search network addresses according to peer endpoint
 *
 * Parameters
 *     sock    : GPR socket to use
 *     addr    : Peer address/hostname to connect to
 *     Service : Peer service to connect to
 *
 * Return value
 *     On success, return GPR_ERR_OK and network addresses are stored inside
 *     the GPR socket
 *     On failure, an error is raised (See gpr_err_to_str)
 *     This function also set a complementary error message if initialized
 *     (See gpr_err_get_cmpl_err)
 *
 *****************************************************************************/
static enum GPR_Err search_peer_endpoint(struct gpr_socket *sock, const char * const addr, const char * const service)
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
 * Attempt to connect to peer by testing all network addressed stored inside
 * the given GPR socket
 *
 * Parameters
 *     sock    : GPR socket to use
 *     addr    : Peer address/hostname to connect to
 *     Service : Peer service to connect to
 *
 * Return value
 *     On success, return GPR_ERR_OK
 *     On failure, an error is raised (See gpr_err_to_str)
 *     This function also set a complementary error message if initialized
 *     (See gpr_err_get_cmpl_err)
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
