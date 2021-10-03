/******************************************************************************
 *
 * gpr_ut_bin.c
 *
 ******************************************************************************
 *
 * Unit tests on "gpr_bin" module
 *
 *****************************************************************************/

#include "gpr_bin.h"

#include <stdio.h>
#include <string.h>

int main()
{
    const char *string = "Hello World";
    size_t str_size = strlen(string);
    char str_hex_buf[(str_size * 2) + 1];
    memset(str_hex_buf, 0, (str_size * 2) + 1);

    gpr_bin_hexlify(string, str_size, str_hex_buf);

    printf("String: \"Hello World\"\n");
    printf("In hex: %s\n", str_hex_buf);

    unsigned long long value = 42;
    size_t val_size = sizeof(unsigned long long);
    char val_hex_buf[(val_size * 2) + 1];
    memset(val_hex_buf, 0, (val_size * 2) + 1);

    gpr_bin_hexlify(&value, val_size, val_hex_buf);

    printf("Value: %llu\n", value);
    printf("In hex: %s\n", val_hex_buf);

    return 0;
}
