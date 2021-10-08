/******************************************************************************
 *
 * \file gpr_log.h
 * \brief Log module
 * \details
 * This module offers a way to format log messages to standard output or in a
 * file\n
 *
 * A log level is assigned to each log message. You can use them as follows:
 * - GPR_LOG_DEBUG (Highly detailed output for debug purpose)
 * - GPR_LOG_INFO (Informational messages)
 * - GPR_LOG_WARNING (Nothing serious but might indicate problems)
 * - GPR_LOG_ERROR (An error occured but does not affect program)
 * - GPR_LOG_CRITICAL (Something bad happened and need immediate attention)
 *
 * The default log level is INFO, it means that only log message at INFO level
 * and below are processed, unless the module is configured differently
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

#ifndef H_GPR_LOG
#define H_GPR_LOG

#include "gpr_err.h"

#include <stdio.h>

/**
 * \brief Possible log levels that can be used to log messages
 */
enum GPR_Log
{
    GPR_LOG_DEBUG,    ///< Highly detailed output for debug purpose
    GPR_LOG_INFO,     ///< Informational messages
    GPR_LOG_WARNING,  ///< Nothing serious but might indicate problems
    GPR_LOG_ERROR,    ///< An error occured but does not affect program
    GPR_LOG_CRITICAL, ///< Something bad happened and need immediate attention
    GPR_LOG_NUMBERS,  ///< Number of log levels (DO NOT USE)
    GPR_LOG_DEFAULT   ///< Can be used to stay on default log level
};

/******************************************************************************
 *
 * \brief Configure log module by specifiying a filename where to redirect log
 * messages and/or change the current default log level
 *
 * \note The configuration of the log module is not mandatory if the default
 * behavior suits you
 *
 * \warning If a filename is specified, the standard output of your program
 * will be redirected to this filename
 *
 * \param filename Absolute or relative path to a filename (Can be NULL)
 * \param level    New default log level (Can be NULL)\n
 *                 Pass GPR_LOG_DEFAULT if you don't want to change it
 *
 * \return
 *     GPR_ERR_OK: Redirection to a file and/or default log level
 *     modification succeeded\n
 *     GPR_ERR_INVALID_PARAMETER: Invalid log level passed\n
 *     GPR_ERR_KO: Failed to redirect log messages to the given file\n
 *     This function also set a complementary error message if initialized
 *     (See \a gpr_err_get_cmpl_err)
 *
 *****************************************************************************/
enum GPR_Err gpr_log_configure(const char *filename, enum GPR_Log level);

/******************************************************************************
 *
 * \brief Stringify log level
 *
 * \param level Log level to stringify
 *
 * \return
 *     C-string representing log level
 *
 *****************************************************************************/
const char *gpr_log_level_to_str(enum GPR_Log level);

/******************************************************************************
 *
 * \brief Write a formatted C-string log message to standard output\n
 * Format is as follows:\n
 * [Date in milliseconds] [File:Line] [Function] Message
 *
 * \note An implicit carriage return is automatically appended at the end of
 * the formatted C-string message
 *
 * \note This function is not intended to be used directly. Use the macro
 * \a "GPR_LOG_MSG" instead
 *
 * \warning The message can't exceed GPR_LOG_MESSAGE_MAX_LEN bytes,
 * otherwise it will be truncated by this function
 *
 * \param level Log level
 * \param file  Current filename
 * \param line  Current line
 * \param func  Current function
 * \param fmt   Message format to write
 * \param ...   Optional arguments
 *
 * \return
 *     The return value is the number of characters which have been written
 *     to the standard output, counting header format but not including the
 *     trailing '\0' or the carriage return\n
 *     If a specific level has been configured, only messages with this
 *     specific level or below will be processed, otherwise the function
 *     does nothing and returns zero\n
 *     If an error occured, the function returns a negative number and
 *     nothing is written to the standard output
 *
 *****************************************************************************/
ssize_t gpr_log_msg(enum GPR_Log level, const char * const file, const int line, const char * const func, const char * const fmt, ...)
    __attribute__((format(printf, 5, 6)));

/**
 * \brief Maximum length of a log message
 */
#define GPR_LOG_MESSAGE_MAX_LEN 8192

/**
 * \brief Macro to log a message
 */
#define GPR_LOG_MSG(lvl, fmt, ...) gpr_log_msg(lvl, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)

/**
 * \brief Macro to log a debug message
 */
#define GPR_LOG_DEBUG(fmt, ...) gpr_log_msg(GPR_LOG_DEBUG, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)

/**
 * \brief Macro to log an info message
 */
#define GPR_LOG_INFO(fmt, ...) gpr_log_msg(GPR_LOG_INFO, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)

/**
 * \brief Macro to log a warning message
 */
#define GPR_LOG_WARN(fmt, ...) gpr_log_msg(GPR_LOG_WARNING, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)

/**
 * \brief Macro to log an error message
 */
#define GPR_LOG_ERR(fmt, ...) gpr_log_msg(GPR_LOG_ERROR, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)

/**
 * \brief Macro to log a critical message
 */
#define GPR_LOG_CRIT(fmt, ...) gpr_log_msg(GPR_LOG_CRITICAL, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)

#endif /* H_GPR_LOG */
