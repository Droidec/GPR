/******************************************************************************
 *
 * gpr_ut_net_server_tcp.c
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
#include <string.h>     // strerror
#include <sys/socket.h> // MSG_NOSIGNAL

#include "gpr_err.h"

#define BUF_SIZE 1024

int main()
{
    struct gpr_socket srv_sock;
    struct gpr_socket peer_sock;
    char sock_addr[INET6_ADDRSTRLEN] = {0};
    uint16_t sock_port = 0;
    char peer_addr[INET6_ADDRSTRLEN] = {0};
    uint16_t peer_port = 0;
    enum GPR_Err err;
    char buf[BUF_SIZE + 1] = {0};
    ssize_t bytes = 0;

    gpr_net_init_default_tcp_socket(&srv_sock, false);

    // For IPv6, "AF_INET6" and "::1" for loopback
    err = gpr_net_listen(&srv_sock, "127.0.0.1", "10000", SOMAXCONN);
    if (err != GPR_ERR_OK)
    {
        fprintf(stderr, "gpr_net_listen: [%s] %s\n", gpr_err_to_str(err), gpr_err_get_msg());
        return 1;
    }

    err = gpr_net_get_socket_info(&srv_sock, sock_addr, &sock_port);
    if (err != GPR_ERR_OK)
    {
        fprintf(stderr, "gpr_net_get_socket_info: [%s] %s\n", gpr_err_to_str(err), gpr_err_get_msg());
        gpr_net_close_socket(&srv_sock);
        return 1;
    }

    printf("Bind to <%s:%" PRIu16 ">\n", sock_addr, sock_port);
    printf("Socket state: %s\n", gpr_net_socket_state_to_str(srv_sock.state));

    err = gpr_net_accept(&srv_sock, &peer_sock);
    if (err != GPR_ERR_OK)
    {
        fprintf(stderr, "gpr_net_accept: [%s] %s\n", gpr_err_to_str(err), gpr_err_get_msg());
        gpr_net_close_socket(&srv_sock);
        gpr_net_close_socket(&peer_sock);
        return 1;
    }

    err = gpr_net_get_peer_info(&peer_sock, peer_addr, &peer_port);
    if (err != GPR_ERR_OK)
    {
        fprintf(stderr, "gpr_net_get_peer_info: [Errno %d] %s\n", errno, strerror(errno));
        gpr_net_close_socket(&srv_sock);
        gpr_net_close_socket(&peer_sock);
        return 1;
    }

    printf("Accepted connection from <%s:%" PRIu16 ">\n", peer_addr, peer_port);

    bytes = gpr_net_recv(&peer_sock, buf, BUF_SIZE, MSG_NOSIGNAL);
    if (bytes < 0)
    {
        fprintf(stderr, "gpr_net_recv: [Errno %d] %s\n", errno, strerror(errno));
        gpr_net_close_socket(&srv_sock);
        gpr_net_close_socket(&peer_sock);
        return 1;
    }

    printf("Message: %s\n", buf);
    gpr_net_close_socket(&srv_sock);
    gpr_net_close_socket(&peer_sock);

    return 0;
}
