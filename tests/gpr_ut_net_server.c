/******************************************************************************
 *
 * gpr_ut_net_server.c
 *
 ******************************************************************************
 *
 * Unit tests on "gpr_net" module
 *
 *****************************************************************************/

#include "gpr_err.h"
#include "gpr_net.h"

#include <stdbool.h> // bool
#include <stdio.h>   // fprintf, printf
#include <unistd.h>  // sleep

int main()
{
    struct gpr_socket sock;
    enum GPR_Err err;

    gpr_net_init_socket(&sock, AF_INET, SOCK_STREAM, 0, 0, false);

    err = gpr_net_listen(&sock, "127.0.0.1", "9500", SOMAXCONN);
    if (err != GPR_ERR_OK)
    {
        fprintf(stderr, "%s: %s\n", gpr_err_to_str(err), gpr_err_get_msg());
        return 1;
    }

    printf("Socket state: %s\n", gpr_net_socket_state_to_str(sock.state));

    // Here, I'm listening port 9500 for 10 seconds
    sleep(10);

    gpr_net_close_socket(&sock);

    return 0;
}
