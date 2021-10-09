/******************************************************************************
 *
 * gpr_ut_bit.c
 *
 ******************************************************************************
 *
 * Unit tests on "gpr_bit" module
 *
 *****************************************************************************/

#include "gpr_bit.h"

#include <assert.h> // assert

int main()
{
    unsigned char byte = 0xFE; // 1111 1110

    assert(!gpr_bit_is_set(byte, 0));
    assert(gpr_bit_is_set(byte, 1));
    assert(gpr_bit_is_set(byte, 2));
    assert(gpr_bit_is_set(byte, 3));
    assert(gpr_bit_is_set(byte, 4));
    assert(gpr_bit_is_set(byte, 5));
    assert(gpr_bit_is_set(byte, 6));
    assert(gpr_bit_is_set(byte, 7));

    return 0;
}
