/*****************************************************************************
 *
 * gpr_ut.c
 *
 *****************************************************************************
 *
 * Unit tests on "gpr" module
 *
 *****************************************************************************/

#include <GPR/gpr.h>

#include <stdio.h>

int main()
{
    /* Initialize default modules */
    GPR_DEFAULT_INIT

    puts("See memory leaks with valgrind\n");

    /* Free all modules */
    GPR_FREE_LIBRARY
}
