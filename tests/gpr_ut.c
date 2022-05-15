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

#include <stdio.h> // printf, puts

int main()
{
    /* Allocate default modules */
    GPR_ALLOC_LIBRARY

    printf("C standard: %ld\n", __STDC_VERSION__);
    puts("See memory leaks with valgrind");

    /* Free all modules */
    GPR_FREE_LIBRARY
}
