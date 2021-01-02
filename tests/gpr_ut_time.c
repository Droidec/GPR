/******************************************************************************
 *
 * gpr_ut_time.c
 *
 ******************************************************************************
 *
 * Unit tests on "gpr_time" module
 *
 *****************************************************************************/

#include "gpr_time.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

int main()
{
    char date_sec[GPR_DATE_SEC_LEN + 1];
    char date_millisec[GPR_DATE_MILLISEC_LEN + 1];
    size_t date_sec_len;
    size_t date_millisec_len;

    /* gpr_time_get_date_sec */
    date_sec_len = gpr_time_get_date_sec(date_sec);
    printf("Current date in second precision: \"%s\"\n", date_sec);
    assert(strlen(date_sec) == date_sec_len);

    /* gpr_time_get_date_millisec */
    date_millisec_len = gpr_time_get_date_millisec(date_millisec);
    printf("Current date in millisecond precision: \"%s\"\n", date_millisec);
    assert(strlen(date_millisec) == date_millisec_len);

    return 0;
}
