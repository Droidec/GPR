/*****************************************************************************
 *
 * gpr_builtin.h
 *
 *****************************************************************************
 * Copyright © 2020 Marc GIANNETTI
 *
 * Builtin macros/functions
 *
 * This module offers builtin macros/functions for GPR modules
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

#ifndef H_GPR_BUILTIN
#define H_GPR_BUILTIN

#include <stdio.h>
#include <stdarg.h>

// Branch prediction optimization
#define LIKELY(x) __builtin_expect((x), 1)
#define UNLIKELY(x) __builtin_expect((x), 0)

/*****************************************************************************
 *
 * Format a string according to a list and place it in a buffer
 *
 * NOTE : This version of vsnprintf returns the length of the string actually
 * written in the buffer, according to truncation
 *
 * Parameters
 *     buf  : The buffer to place the result into
 *     size : The size of the buffer, including the trailing null space
 *     fmt  : The format string to use
 *     args : Arguments for the format string
 *
 * Return value
 *     The return value is the number of characters which have been written
 *     into the buffer not including the trailing '\0'. If the size of the
 *     buffer is equal to 0, the function returns 0
 *
 *****************************************************************************/
int gpr_builtin_vscnprintf(char *buf, size_t size, const char *fmt, va_list args);
#define VSCNPRINTF(buf, size, fmt, ...) gpr_builtin_vscnprintf(buf, size, fmt, ##__VA_ARGS__)

/*****************************************************************************
 *
 * Format a string and place it in a buffer
 *
 * NOTE : This version of snprintf returns the length of the string actually
 * written in the buffer, according to truncation
 *
 * Parameters
 *     buf  : The buffer to place the result into
 *     size : The size of the buffer, including the trailing null space
 *     fmt  : The format string to use
 *     ... : Arguments for the format string
 *
 * Return value
 *     The return value is the number of characters which have been written
 *     into the buffer not including the trailing '\0'. If the size of the
 *     buffer is equal to 0, the function returns 0
 *
 *****************************************************************************/
int gpr_builtin_scnprintf(char *buf, size_t size, const char *fmt, ...);
#define SCNPRINTF(buf, size, fmt, ...) gpr_builtin_scnprintf(buf, size, fmt, ##__VA_ARGS__)

#endif /* H_GPR_BUILTIN */
