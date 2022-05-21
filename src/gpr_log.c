/******************************************************************************
 *
 * \file gpr_log.c
 * \brief Log module
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

#include "gpr_log.h"

#include <stdarg.h> // va_list, va_start, va_end
#include <stdio.h> // ssize_t

#include "gpr_err.h"
#include "gpr_utils.h"

/**
 * \brief Default log level
 */
static enum GPR_Log Default_Log_Level = GPR_LOG_INFO;

enum GPR_Err gpr_log_configure(const char *filename, enum GPR_Log level)
{
    /* Redirect output */
    if (filename != NULL)
    {
        if (freopen(filename, "w", stdout) == NULL)
            return gpr_err_raise(GPR_ERR_KO, "Log redirection failed");

        if (level == GPR_LOG_DEFAULT)
            return gpr_err_raise(GPR_ERR_OK, NULL);
    }

    /* Change default log level */
    if (level != GPR_LOG_DEFAULT)
    {
        /* Check consistency */
        if ((level < 0) || (level >= GPR_LOG_NUMBERS))
            return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid log level");

        Default_Log_Level = level;
    }

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

const char *gpr_log_level_to_str(enum GPR_Log level)
{
    static const char *log_array[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};

#ifdef DEBUG
    /* Check consistency */
    if ((level < 0) || (level >= GPR_LOG_NUMBERS))
        return "UNKNOWN";
#endif

    return log_array[level];
}

ssize_t gpr_log_msg(enum GPR_Log level, const char * const fmt, ...)
{
    va_list list;
    int msg_len;
    char msg[GPR_LOG_MESSAGE_MAX_LEN + 1];

#ifdef DEBUG
    /* Check consistency */
    if (fmt == NULL)
        return -1;
#endif

    /* Check default log level */
    if (level < Default_Log_Level)
        return 0;

    /* Build message */
    va_start(list, fmt);
    msg_len = VSCNPRINTF(msg, GPR_LOG_MESSAGE_MAX_LEN + 1, fmt, list);
    va_end(list);

    if (UNLIKELY(msg_len <= 0))
        return -1;

    /* Write message on standard output */
    fprintf(stdout, "%s\n", msg);

    return msg_len;
}

int gpr_log_flush(void)
{
    return fflush(stdout);
}
