/*****************************************************************************
 *
 * gpr_time.c
 *
 *****************************************************************************
 * Copyright © 2019 Marc GIANNETTI
 *
 * Time management functions
 *
 *****************************************************************************
 *
 * DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *             Version 2, December 2004
 *
 * Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
 *
 * Everyone is permitted to copy and distribute verbatim or modified
 * copies of this license document, and changing it is allowed as long
 * as the name is changed.
 *
 *            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 * TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 *
 * 0. You just DO WHAT THE FUCK YOU WANT TO.
 *
 *****************************************************************************/

#include <gpr_time.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>

size_t gpr_time_get_date_sec(char * const psz_buffer)
{
    time_t i_rawtime;
    struct tm dateinfo;

    memset(psz_buffer, 0, GPR_DATE_SEC_LEN + 1);

    time(&i_rawtime);
    localtime_r(&i_rawtime, &dateinfo); // WARN: Not portable

    return strftime(psz_buffer, GPR_DATE_SEC_LEN + 1, "%d/%m/%Y-%H:%M:%S", &dateinfo);
}

size_t gpr_time_get_date_millisec(char * const psz_buffer)
{
    size_t length;
    struct timeval timeinfo;

    memset(psz_buffer, 0, GPR_DATE_MILLISEC_LEN + 1);

    length = gpr_time_get_date_sec(psz_buffer);
    gettimeofday(&timeinfo, NULL); // WARN: Not portable

    return (length += snprintf(psz_buffer + GPR_DATE_SEC_LEN, (GPR_DATE_MILLISEC_LEN - GPR_DATE_SEC_LEN) + 1, ".%.3ld", timeinfo.tv_usec / 1000L));
}
