/******************************************************************************
 *
 * gpr_ut_network.c
 *
 ******************************************************************************
 *
 * Unit tests on "gpr_network" module
 *
 *****************************************************************************/

#include <GPR/gpr_network.h>

#include <stdio.h>
#include <assert.h>

int main()
{
    GPR_INIT_ERR_MODULE

    struct gpr_socket *sock;
    enum GPR_Err err;

    sock = gpr_net_create_socket(AF_INET, SOCK_STREAM, 0, 0);
    assert(sock != NULL);

    err = gpr_net_connect(sock, "www.google.com", "443");
    if (err != GPR_ERR_OK)
    {
        fprintf(stderr, "gpr_net_connect() : %s (%s)\n", gpr_err_to_str(err), gpr_err_get_cmpl_err());
        gpr_net_close_socket(sock);
        GPR_FREE_ERR_MODULE
        return 1;
    }

    printf("Connection successful!\n");

    err = gpr_net_close_socket(sock);
    if (err != GPR_ERR_OK)
    {
        fprintf(stderr, "gpr_net_close_socket(): %s (%s)\n", gpr_err_to_str(err), gpr_err_get_cmpl_err());
        GPR_FREE_ERR_MODULE
        return 1;
    }

    GPR_FREE_ERR_MODULE

    return 0;
}
