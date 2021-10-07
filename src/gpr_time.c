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
#include "gpr_utils.h"

#include <time.h>
#include <string.h>
#include <sys/time.h>

/******************************************************************************
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
