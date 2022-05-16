/******************************************************************************
 *
 * \file gpr_str.h
 * \brief String module
 * \details This module offers additional functions to manipulate C-strings
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

#ifndef H_GPR_STR
#define H_GPR_STR

#include <stddef.h> // size_t

/******************************************************************************
 *
 * \brief Convert in lowercase the first \a num characters of \a src to \a dst
 *
 * \note If the end of \a src is reached before \a num characters have been
 * converted, the functions stops conversion and returns the number of
 * characters of \a src string, without the trailing '\0'
 *
 * \note If a character can't be converted, it will be copied as is
 *
 * \note This function is overlapping safe
 *
 * \param dst Destination array where the content is to be copied
 * \param src C-string to be copied
 * \param num Maximum number of characters to be copied from \a src
 *
 * \return
 *     Number of characters written in dst
 *
 *****************************************************************************/
size_t gpr_str_tolower(char *dst, const char *src, size_t num);

/******************************************************************************
 *
 * \brief Convert in uppercase the first \a num characters of \a src to \a dst
 *
 * \note If the end of \a src is reached before \a num characters have been
 * converted, the functions stops conversion and returns the number of
 * characters of \a src string, without the trailing '\0'
 *
 * \note If a character can't be converted, it will be copied as is
 *
 * \note This function is overlapping safe
 *
 * \param dst Destination array where the content is to be copied
 * \param src C-string to be copied
 * \param num Maximum number of characters to be copied from \a src
 *
 * \return
 *     Number of characters written in \a dst
 *
 *****************************************************************************/
size_t gpr_str_toupper(char *dst, const char *src, size_t num);

/******************************************************************************
 *
 * \brief Trim leading spaces of \a str by moving C-string pointer until it
 * reaches the new start or a trailing '\0'
 *
 * \param str C-string to trim
 *
 * \return
 *     Pointer to the new start of \a str \n
 *     NULL if \a str is NULL (DEBUG mode only)
 *
 *****************************************************************************/
char *gpr_str_ltrim(char *str);
// TODO: trim according to a set of characters, not only spaces

/******************************************************************************
 *
 * \brief Trim trailing spaces of \a str by adding a new trailing '\0' at the
 * new end of the string
 *
 * \param str C-string to trim
 *
 * \return
 *     Pointer to \a str trimmed\n
 *     NULL if C-string is NULL (DEBUG mode only)
 *
 *****************************************************************************/
char *gpr_str_rtrim(char *str);
// TODO: trim according to a set of characters, not only spaces

/******************************************************************************
 *
 * \brief Trim leading and trailing spaces of \a str
 *
 * \note A call to this function is equivalent to a call of gpr_str_ltrim
 * and gpr_str_rtrim, in any order
 *
 * \param str C-string to trim
 *
 * \return
 *     Pointer to \a str trimmed\n
 *     NULL if C-string is NULL (DEBUG mode only)
 *
 *****************************************************************************/
char *gpr_str_trim(char *str);
// TODO: trim according to a set of characters, not only spaces

#endif
