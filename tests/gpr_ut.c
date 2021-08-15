/******************************************************************************
 *
 * gpr_ut.c
 *
 ******************************************************************************
 *
 * Unit tests on "gpr" module
 *
 *****************************************************************************/

#include "gpr.h"

#include <stdio.h>

int main()
{
    /* Allocate default modules */
    GPR_ALLOC_LIBRARY

    puts("See memory leaks with valgrind\n");

    /* Free all modules */
    GPR_FREE_LIBRARY
}
