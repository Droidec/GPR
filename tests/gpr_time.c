/*****************************************************************************
*
* gpr_time.c: Test 
*
******************************************************************************
*
* Author: GIANNETTI Marc <derovor@gmail.com>
*
*****************************************************************************/

#include <GPR/gpr_time.h>

#include <stdio.h>
#include <string.h>
#include <assert.h>

int main()
{
    char buffer[DATE_IN_SEC + 1];

    memset(buffer, 0, DATE_IN_SEC + 1);

    /* Get current date */
    GPR_TIME_get_date_in_sec(buffer, DATE_IN_SEC + 1);

    printf("Date in sec: \"%s\"\n", buffer);

    printf("strlen: %zu\n", strlen(buffer));

    assert(strlen(buffer) == DATE_IN_SEC);

    return 0;
}
