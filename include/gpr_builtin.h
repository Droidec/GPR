/******************************************************************************
 *
 * \file gpr_builtin.h
 * \brief Builtin macros/functions
 * \details This module offers builtin macros/functions to optimize and
 * simplify tasks for GPR modules
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

#ifndef H_GPR_BUILTIN
#define H_GPR_BUILTIN

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h> // offsetof

/**
 * \brief Optimize branch prediction \a x in favour of a "likely" side jump
 * instruction
 */
#define LIKELY(x) __builtin_expect((x), 1)

/**
 * \brief Optimize branch prediction \a x in favour of an "unlikely" side jump
 * instruction
 */
#define UNLIKELY(x) __builtin_expect((x), 0)

/**
 * \brief Cast a member of a structure out to the containing structure
 *
 * \param ptr    Pointer to the member
 * \param type   Type of the container structure this is embedded in
 * \param member Name of the member within this structure
 */
// clang-format off
#define CONTAINER_OF(ptr, type, member)                    \
    ({                                                     \
        const typeof(((type *)0)->member) *__mptr = (ptr); \
        (type *)((char *)__mptr - offsetof(type, member)); \
    })
// clang-format on

/******************************************************************************
 *
 * \brief Format a string according to a list and place it in a buffer
 *
 * \note This version of vsnprintf returns the length of the string actually
 * written in the buffer, according to truncation
 *
 * \param buf  The buffer to place the result into
 * \param size The size of the buffer, including the trailing '\0'
 * \param fmt  The format string to use
 * \param args Arguments for the format string
 *
 * \return
 *     The return value is the number of characters which have been written
 *     into the buffer not including the trailing '\0'\n
 *     If the size of the buffer is equal to 0, the function returns 0\n
 *     If an error occured, the function returns a negative number
 *
 *****************************************************************************/
int gpr_builtin_vscnprintf(char *buf, size_t size, const char *fmt, va_list args);

/**
 * \brief Macro for the \a gpr_builtin_vscnprintf function
 */
#define VSCNPRINTF(buf, size, fmt, args) gpr_builtin_vscnprintf(buf, size, fmt, args)

/******************************************************************************
 *
 * \brief Format a string and place it in a buffer
 *
 * \note This version of snprintf returns the length of the string actually
 * written in the buffer, according to truncation
 *
 * \param buf  The buffer to place the result into
 * \param size The size of the buffer, including the trailing '\0'
 * \param fmt  The format string to use
 * \param ...  Arguments for the format string
 *
 * \return
 *     The return value is the number of characters which have been written
 *     into the buffer not including the trailing '\0'\n
 *     If the size of the buffer is equal to 0, the function returns 0\n
 *     If an error occured, the function returns a negative number
 *
 *****************************************************************************/
int gpr_builtin_scnprintf(char *buf, size_t size, const char *fmt, ...) __attribute__((format(printf, 3, 4)));

/**
 * \brief Macro for the \a gpr_builtin_scnprintf function
 */
#define SCNPRINTF(buf, size, fmt, ...) gpr_builtin_scnprintf(buf, size, fmt, ##__VA_ARGS__)

#endif /* H_GPR_BUILTIN */
