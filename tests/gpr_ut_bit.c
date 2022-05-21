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
#include "gpr_utils.h"

int main()
{
    unsigned char byte = 0xFE; // 1111 1110

    ASSERT(!gpr_bit_is_set(byte, 0));
    ASSERT(gpr_bit_is_set(byte, 1));
    ASSERT(gpr_bit_is_set(byte, 2));
    ASSERT(gpr_bit_is_set(byte, 3));
    ASSERT(gpr_bit_is_set(byte, 4));
    ASSERT(gpr_bit_is_set(byte, 5));
    ASSERT(gpr_bit_is_set(byte, 6));
    ASSERT(gpr_bit_is_set(byte, 7));

    return 0;
}
