/******************************************************************************
 *
 * \file gpr_time.c
 * \brief Time module
 *
 ******************************************************************************
 *
 * \copyright Copyright (c) 2019, the respective contributors, as shown by the
 * AUTHORS file.
 *
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/

#include "gpr_time.h"

#include <stddef.h>   // size_t
#include <string.h>   // memset
#include <sys/time.h> // gettimeofday
#include <time.h>     // time_t, tm, time, localtime_r, strftime

#include "gpr_utils.h"

size_t gpr_time_get_date_sec(char *buffer)
{
    time_t raw_time;
    struct tm date_info;

#ifdef DEBUG
    /* Check consistency */
    if (buffer == NULL)
        return 0;
#endif

    memset(buffer, 0, GPR_DATE_SEC_LEN + 1);

    time(&raw_time);
    localtime_r(&raw_time, &date_info); // WARN: Not portable

    return strftime(buffer, GPR_DATE_SEC_LEN + 1, "%d/%m/%Y-%H:%M:%S", &date_info);
}

size_t gpr_time_get_date_millisec(char *buffer)
{
    size_t sec_len;
    int ms_len;
    struct timeval tv;

#ifdef DEBUG
    /* Check consistency */
    if (buffer == NULL)
        return 0;
#endif

    memset(buffer, 0, GPR_DATE_MILLISEC_LEN + 1);

    sec_len = gpr_time_get_date_sec(buffer);

    if (UNLIKELY(sec_len == 0))
        return 0;

    gettimeofday(&tv, NULL); // WARN: Not portable
    ms_len = SCNPRINTF(buffer + GPR_DATE_SEC_LEN, (GPR_DATE_MILLISEC_LEN - GPR_DATE_SEC_LEN) + 1, ".%.3ld", tv.tv_usec / 1000L);

    if (UNLIKELY(ms_len <= 0))
        return 0;

    return sec_len + ms_len;
}
