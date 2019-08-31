/*****************************************************************************
*
* gpr_time.c: Time manipulation functions
*
******************************************************************************
*
* Author: GIANNETTI Marc <derovor@gmail.com>
*
*****************************************************************************/

#include <gpr_time.h>
#include <stdio.h>

size_t GPR_TIME_get_date_in_sec(char *p_buffer, unsigned int i_buffer_len)
{
    time_t i_rawtime;
    struct tm *p_timeinfo;

    // WARN: Not thread safe (Use localtime_r instead)
    time(&i_rawtime);
    p_timeinfo = localtime(&i_rawtime);

    return strftime(p_buffer, i_buffer_len, "%d/%m/%Y-%H:%M:%S", p_timeinfo);
}
