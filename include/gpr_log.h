/*****************************************************************************
 *
 * gpr_log.h
 *
 *****************************************************************************
 * Copyright © 2020 Marc GIANNETTI
 *
 * Log management functions
 *
 * Log module offers the following possibilities:
 * - Write a formatted log message to standard output
 *
 * A log level is assigned to each log message. You can use them as follows:
 * - GPR_LOG_DEBUG (Highly detailed output for debug purpose)
 * - GPR_LOG_INFO (Report current events)
 * - GPR_LOG_WARNING (Something unexpected has happened, or could happen)
 * - GPR_LOG_ERR (The program failed to achieve a task)
 * - GPR_LOG_CRITICAL (A serious problem occured, the program may no longer
 *   work properly)
 *
 * The default log level is INFO, it means that only log message at INFO level
 * and below are processed, unless the module is configured otherwise
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

#ifndef H_GPR_LOG
#define H_GPR_LOG

#include "gpr_err.h"

#include <stdio.h>

// Log levels
enum GPR_Log
{
    GPR_LOG_DEBUG,
    GPR_LOG_INFO,
    GPR_LOG_WARNING,
    GPR_LOG_ERROR,
    GPR_LOG_CRITICAL,
    GPR_LOG_NUMBERS,
    GPR_LOG_DEFAULT
};

/*****************************************************************************
 *
 * Configure log module by specifiying a filename where to redirect log
 * messages and/or change the current default log level
 *
 * WARN : If a filename is specified, the standard output of your program
 * will be redirected to this filename
 *
 * NOTE : The configuration of the log module is not mandatory if the default
 * behavior suits you
 *
 * Parameters
 *     filename : Absolute or relative path to a filename (Can be NULL)
 *     level    : New default log level (Can be NULL).
 *                Pass GPR_LOG_DEFAULT if you don't want to change it
 *
 * Return value
 *     On success, return GPR_ERR_OK
 *     On failure, an error is raised (See gpr_err_to_str)
 *     This function also set a complementary error message if initialized
 *     (See gpr_err_get_cmpl_err)
 *
 *****************************************************************************/
enum GPR_Err gpr_log_configure(const char *filename, enum GPR_Log level);

/*****************************************************************************
 *
 * Stringify log level
 *
 * Parameters
 *     level : Log level to stringify
 *
 * Return value
 *     C-string representing log level
 *
 *****************************************************************************/
const char *gpr_log_level_to_str(enum GPR_Log level);

/*****************************************************************************
 *
 * Write a formatted C-string log message to standard output
 * Format is as follows:
 * [Date in milliseconds] [File:Line] [Function] Message
 *
 * WARN: The message can't exceed GPR_LOG_MESSAGE_MAX_LEN bytes,
 * otherwise it will be truncated by this function
 *
 * NOTE: An implicit carriage return is automatically appended at the end of
 * the formatted C-string message
 *
 * NOTE: This function is not intended to be used directly. Use the macro
 * "GPR_LOG_MSG" instead
 *
 * Parameters
 *     level : Log level
 *     file  : Current filename
 *     line  : Current line
 *     func  : Current function
 *     fmt   : Message format to write
 *     ...   : Optional arguments
 *
 * Return value
 *     The return value is the number of characters which have been written
 *     to the standard output, counting header format but not including the
 *     trailing '\0' or the carriage return.
 *     If a specific level has been configured, only messages with this
 *     specific level or below will be processed, otherwise the function
 *     does nothing and returns 0.
 *     If an error occured, the function returns a negative number and
 *     nothing is written to the standard output.
 *
 *****************************************************************************/
#define GPR_LOG_MESSAGE_MAX_LEN 512
ssize_t gpr_log_msg(enum GPR_Log level, const char * const file, const int line, const char * const func, const char * const fmt, ...) __attribute__ ((format (printf, 5, 6)));
#define GPR_LOG_MSG(lvl, fmt, ...) gpr_log_msg(lvl, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)

#endif /* H_GPR_LOG */
