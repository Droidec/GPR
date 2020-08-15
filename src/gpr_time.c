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

#include "gpr_time.h"
#include "gpr_builtin.h"

#include <time.h>
#include <string.h>
#include <sys/time.h>

/*****************************************************************************
 * Public functions
 *****************************************************************************/

size_t gpr_time_get_date_sec(char * const buffer)
{
    time_t rawtime;
    struct tm dateinfo;

    memset(buffer, 0, GPR_DATE_SEC_LEN + 1);

    time(&rawtime);
    localtime_r(&rawtime, &dateinfo); // WARN: Not portable

    return strftime(buffer, GPR_DATE_SEC_LEN + 1, "%d/%m/%Y-%H:%M:%S", &dateinfo);
}

size_t gpr_time_get_date_millisec(char * const buffer)
{
    size_t sec_length;
    int ms_length;
    struct timeval timeinfo;

    memset(buffer, 0, GPR_DATE_MILLISEC_LEN + 1);

    sec_length = gpr_time_get_date_sec(buffer);

    if (UNLIKELY(sec_length == 0))
        return 0;

    gettimeofday(&timeinfo, NULL); // WARN: Not portable
    ms_length = SCNPRINTF(buffer + GPR_DATE_SEC_LEN, (GPR_DATE_MILLISEC_LEN - GPR_DATE_SEC_LEN) + 1, ".%.3ld", timeinfo.tv_usec / 1000L);

    if (UNLIKELY(ms_length <= 0))
        return 0;

    return sec_length + ms_length;
}
