/*****************************************************************************
 *
 * gpr_log.c
 *
 *****************************************************************************
 * Copyright © 2020 Marc GIANNETTI
 *
 * Log management functions
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

#include "gpr_log.h"
#include "gpr_builtin.h"
#include "gpr_time.h"

#include <stdio.h>
#include <stdarg.h>

/*****************************************************************************
 * Private prototypes
 *****************************************************************************/

// Default log level
static enum GPR_Log Default_Log_Level = GPR_LOG_INFO;

/*****************************************************************************
 * Public functions
 *****************************************************************************/

enum GPR_Err gpr_log_configure(const char *psz_filename, enum GPR_Log level)
{
    /* Redirect output */
    if (psz_filename != NULL)
    {
        if (freopen(psz_filename, "w", stdout) == NULL)
            return gpr_err_raise(GPR_ERR_KO, "Log redirection failed");

        return gpr_err_raise(GPR_ERR_OK, "");
    }

    /* Change default log level */
    if (level != GPR_LOG_DEFAULT)
    {
        /* Check consistency */
        if (level < 0 || level >= GPR_LOG_NUMBERS)
            return gpr_err_raise(GPR_ERR_KO, "Default log level change failed");

        Default_Log_Level = level;
    }

    return gpr_err_raise(GPR_ERR_OK, "");
}

const char *gpr_log_level_to_str(enum GPR_Log level)
{
    /* Check consistency */
    if (level < 0 || level >= GPR_LOG_NUMBERS)
        return "UNKNOWN";

    const char *p_log_array[] = {"DEBUG", "INFO", "WARNING", "ERROR", "CRITICAL"};

    return p_log_array[level];
}

void gpr_log_msg(enum GPR_Log level, const char * const psz_file, const int i_line, const char * const psz_func, const char * const psz_fmt, ...)
{
    va_list list;
    int i_nBytes;
    char psz_msg[GPR_LOG_MESSAGE_MAX_LEN + 1];
    char psz_date[GPR_DATE_MILLISEC_LEN + 1];

    /* Check default log level */
    if (level < Default_Log_Level)
        return;

    /* Build header message */
    gpr_time_get_date_millisec(psz_date);
    i_nBytes = SCNPRINTF(psz_msg, GPR_LOG_MESSAGE_MAX_LEN + 1, "[%s] [%s] [%s:%d] [%s] ", psz_date, gpr_log_level_to_str(level), psz_file, i_line, psz_func);

    /* Build message */
    va_start(list, psz_fmt);
    i_nBytes += VSCNPRINTF(psz_msg + i_nBytes, GPR_LOG_MESSAGE_MAX_LEN + 1 - i_nBytes, psz_fmt, list);
    va_end(list);

    /* Write message on standard output */
    fprintf(stdout, "%s\n", psz_msg);

    /* Refresh standard output */
    fflush(stdout);

    return;
}
