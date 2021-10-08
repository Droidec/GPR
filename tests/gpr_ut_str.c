/******************************************************************************
 *
 * gpr_ut_str.c
 *
 ******************************************************************************
 *
 * Unit tests on "gpr_str" module
 *
 *****************************************************************************/

#include "gpr_str.h"
#include "gpr_utils.h"

#include <stdio.h>
#include <string.h>

#define STRING_SIZE 20

int main()
{
    char string1[STRING_SIZE + 1] = {0};
    char string2[STRING_SIZE + 1] = {0};
    SCNPRINTF(string1, STRING_SIZE + 1, "Hello World");
    SCNPRINTF(string2, STRING_SIZE + 1, "   Hello World   ");

    gpr_str_tolower(string1, string1, strlen(string1));
    printf("%s\n", string1);

    gpr_str_toupper(string1, string1, strlen(string1));
    printf("%s\n", string1);

    printf("%s\n", gpr_str_trim(string2));

    return 0;
}
