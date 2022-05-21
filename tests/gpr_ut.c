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
    printf("C standard: %ld\n", __STDC_VERSION__);
    puts("See memory leaks with valgrind");
}
