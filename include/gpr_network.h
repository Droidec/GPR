/******************************************************************************
*
* gpr_network.h
*
*******************************************************************************
* Copyright © 2020 Marc GIANNETTI
*
* Network module
*
* This module offers the following possibilities:
* - Open network services by listening ports or connect to a remote host
* - Receive or send network messages with the possibily to encrypt content
*
* The primary goal of this module is to hide the painful manipulations of
* sockets and/or known SSL encryption libraries
*
* Any operation can be performed asynchronously if required
*
* --- /!\ WORK IN PROGRESS /!\ ---
*
*******************************************************************************
*
* Copyright (c) 2019-2021, GPR Team
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
******************************************************************************/

#ifndef H_GPR_NETWORK
#define H_GPR_NETWORK

#include "gpr_err.h"

#include <sys/socket.h> // socket
#include <netdb.h>      // getaddrinfo

enum GPR_Net
{
    GPR_NET_NONE,    // First connection attempt
    GPR_NET_PENDING, // Connection establishment in progress
    GPR_NET_OK,      // Establishment successful
    GPR_NET_ERR      // Establishment failure
};

struct gpr_socket
{
    int socket;              // C-socket communication endpoint
    enum GPR_Net status;     // GPR socket establishment progress
    struct addrinfo info;    // Communication criterias
    struct addrinfo *result; // List of available network adresses
    struct addrinfo *curr;   // Current network address being tested
};

/******************************************************************************
*
* Create a GPR socket used for communication
*
* NOTE : For more information about parameters, please consult getaddrinfo()
* manual
*
* Parameters
*     domain   : Address family (AF_INET, AF_INET6)
*     type     : Socket type (SOCK_STREAM, SOCK_DGRAM) (Can be 0)
*     protocol : Socket address protocol (Can be 0)
*     flags    : Additional flags (Can be 0)
*
* Return value
*     On success, return a GPR socket allocated and initialized
*     On failure, return NULL. This can occur if allocation failed
*
******************************************************************************/
struct gpr_socket *gpr_net_create_socket(int domain, int type, int protocol, int flags);

/******************************************************************************
*
* Close and unallocate a GPR socket
*
* Parameters
*     sock : GPR socket to close and unallocate
*
* Return value
*     - GPR_ERR_OK: The socket has been closed
*     - GPR_ERR_INVALID_PARAMETER: The socket is NULL
*
******************************************************************************/
enum GPR_Err gpr_net_close_socket(struct gpr_socket *sock);

/******************************************************************************
*
* Initiate a connection on a GPR socket
*
* Parameters
*     sock    : GPR socket to use
*     addr    : Peer address/hostname to connect to
*     Service : Peer service to connect to
*
* Return value
*     - GPR_ERR_OK: The connection has been established
*     - GPR_ERR_INVALID_PARAMETER: One of the parameter is NULL
*     - GPR_ERR_NETWORK_ERROR: A network error occured
*
******************************************************************************/
enum GPR_Err gpr_net_connect(struct gpr_socket *sock, const char *const addr, const char *const service);

// ssize_t gpr_net_recv();
// ssize_t gpr_net_send();

#endif /* H_GPR_NETWORK */
