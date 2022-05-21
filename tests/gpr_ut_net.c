/******************************************************************************
 *
 * gpr_ut_net.c
 *
 ******************************************************************************
 *
 * Unit tests on "gpr_net" module
 *
 *****************************************************************************/

#include "gpr_net.h"

#include <assert.h> // assert
#include <stdio.h>  // fprintf, printf

int main()
{
    struct gpr_socket *sock;
    enum GPR_Err err;

    sock = gpr_net_new_socket(AF_INET, SOCK_STREAM, 0, 0);
    assert(sock != NULL);

    err = gpr_net_connect(sock, "www.google.com", "443");
    if (err != GPR_ERR_OK)
    {
        fprintf(stderr, "gpr_net_connect() : [%s] %s\n", gpr_err_to_str(err), gpr_err_get_msg());
        gpr_net_close_socket(sock);
        gpr_net_free_socket(sock);
        return 1;
    }

    printf("Connection successful!\n");

    gpr_net_close_socket(sock);
    gpr_net_free_socket(sock);

    return 0;
}
