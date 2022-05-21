/******************************************************************************
 *
 * gpr_ut_err.c
 *
 ******************************************************************************
 *
 * Unit tests on "gpr_err" module
 *
 *****************************************************************************/

#include "gpr_err.h"

#include <stdio.h>  // printf

enum GPR_Err err_test1(void)
{
    return gpr_err_raise(GPR_ERR_KO, "This is an error test");
}

enum GPR_Err err_test2(void)
{
    return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "This is another error test");
}

enum GPR_Err err_test3(void)
{
    return gpr_err_raise(GPR_ERR_OK, NULL);
}

int main()
{
    enum GPR_Err err;

    err = err_test1();
    printf("Error: [%s] %s\n", gpr_err_to_str(err), gpr_err_get_msg());

    err = err_test2();
    printf("Error: [%s] %s\n", gpr_err_to_str(err), gpr_err_get_msg());

    err = err_test3();
    printf("Error: [%s] %s\n", gpr_err_to_str(err), gpr_err_get_msg());

    return 0;
}
