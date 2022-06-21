/******************************************************************************
 *
 * gpr_ut_net_client_tcp.c
 *
 ******************************************************************************
 *
 * Unit tests on "gpr_net" module
 *
 *****************************************************************************/

#include "gpr_net.h"

#include <errno.h>      // errno
#include <inttypes.h>   // uint16_t, PRIu16
#include <netinet/in.h> // INET6_ADDRSTRLEN
#include <stdbool.h>    // bool
#include <stdio.h>      // fprintf, printf, ssize_t
#include <string.h>     // strlen, strerror
#include <sys/socket.h> // MSG_NOSIGNAL

#include "gpr_err.h"

int main()
{
    struct gpr_socket sock;
    char sock_addr[INET6_ADDRSTRLEN] = {0};
    uint16_t sock_port = 0;
    char peer_addr[INET6_ADDRSTRLEN] = {0};
    uint16_t peer_port = 0;
    enum GPR_Err err;
    char buf[] = "Hello World from a client";
    ssize_t bytes = 0;

    gpr_net_init_default_tcp_socket(&sock, false);

    // For IPv6, "AF_INET6" and "::1" for loopback
    err = gpr_net_connect(&sock, "127.0.0.1", "10000", "10001");
    if (err != GPR_ERR_OK)
    {
        fprintf(stderr, "gpr_net_connect: [%s] %s\n", gpr_err_to_str(err), gpr_err_get_msg());
        gpr_net_close_socket(&sock);
        return 1;
    }

    printf("Socket state: %s\n", gpr_net_socket_state_to_str(sock.state));

    err = gpr_net_get_socket_info(&sock, sock_addr, &sock_port);
    if (err != GPR_ERR_OK)
    {
        fprintf(stderr, "gpr_net_get_socket_info: [%s] %s\n", gpr_err_to_str(err), gpr_err_get_msg());
        gpr_net_close_socket(&sock);
        return 1;
    }

    err = gpr_net_get_peer_info(&sock, peer_addr, &peer_port);
    if (err != GPR_ERR_OK)
    {
        fprintf(stderr, "gpr_net_get_peer_info: [%s] %s\n", gpr_err_to_str(err), gpr_err_get_msg());
        gpr_net_close_socket(&sock);
        return 1;
    }

    printf("Connected to <%s:%" PRIu16 "> from <%s:%" PRIu16 ">\n", peer_addr, peer_port, sock_addr, sock_port);

    bytes = gpr_net_send(&sock, buf, strlen(buf), MSG_NOSIGNAL);
    if (bytes < 0)
    {
        fprintf(stderr, "gpr_net_send: [Errno %d] %s\n", errno, strerror(errno));
        gpr_net_close_socket(&sock);
        return 1;
    }

    printf("Message to <%s:%" PRIu16 ">: %s\n", peer_addr, peer_port, buf);
    gpr_net_close_socket(&sock);

    return 0;
}
