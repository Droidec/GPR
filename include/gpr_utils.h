/******************************************************************************
 *
 * \file gpr_utils.h
 * \brief Utils macros/functions
 * \details This module offers utils macros/functions to optimize and
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

#ifndef H_GPR_UTILS
#define H_GPR_UTILS

#include <stdarg.h> // va_list
#include <stddef.h> // size_t

/**
 * \brief Optimizes branch prediction \a x in favour of a *likely* side jump
 * instruction
 */
#define LIKELY(x) __builtin_expect((x), 1)

/**
 * \brief Optimizes branch prediction \a x in favour of an *unlikely* side jump
 * instruction
 */
#define UNLIKELY(x) __builtin_expect((x), 0)

/**
 * \brief Gets the number of elements in an array
 */
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x)[0])

/**
 * \brief Prevents the compiler to generate feedback in release mode
 *        with unit tests
 */
#ifdef DEBUG
#include <assert.h> // assert
#define ASSERT(x) assert(x)
#else
#define ASSERT(x)        \
    do                   \
    {                    \
        (void)sizeof(x); \
    } while (0)
#endif

/**
 * \brief Prevents the compiler to generate feedback for unused variables
 */
#ifdef __GNUC__
#define UNUSED(x) UNUSED_##x __attribute__((__unused__))
#else
#define UNUSED(x) UNUSED_##x
#endif

/**
 * \brief Prevents the compiler to generate feedback for unused functions
 */
#ifdef __GNUC__
#define UNUSED_FUNC(x) __attribute__((__unused__)) UNUSED_##x
#else
#define UNUSED_FUNC(x) UNUSED_##x
#endif

/**
 * \brief Formats a string according to a list and place it in a buffer
 *
 * \note This version of \c vsnprintf returns the length of the string actually
 * written in the buffer, according to truncation
 *
 * \param[out] buf  The buffer to place the result into
 * \param[in]  size The size of the buffer, including the trailing \c \0
 * \param[in]  fmt  The format string to use
 * \param[in]  args Arguments for the format string
 *
 * \return The return value is the number of characters which have been written
 *         into the buffer not including the trailing \c \0.\n
 *         If the size of the buffer is equal to \c 0, the function returns \c 0.\n
 *         If an error occurred, the function returns a negative number
 */
int gpr_utils_vscnprintf(char *buf, size_t size, const char *fmt, va_list args);

/**
 * \brief Macro for the #gpr_utils_vscnprintf function
 */
#define VSCNPRINTF(buf, size, fmt, args) gpr_utils_vscnprintf(buf, size, fmt, args)

/**
 * \brief Formats a string and place it in a buffer
 *
 * \note This version of \c snprintf returns the length of the string actually
 * written in the buffer, according to truncation
 *
 * \param[out] buf  The buffer to place the result into
 * \param[in]  size The size of the buffer, including the trailing \c \0
 * \param[in]  fmt  The format string to use
 * \param[in]  ...  Arguments for the format string
 *
 * \return The return value is the number of characters which have been written
 *         into the buffer not including the trailing \c \0.\n
 *         If the size of the buffer is equal to \c 0, the function returns \c 0.\n
 *         If an error occurred, the function returns a negative number
 */
int gpr_utils_scnprintf(char *buf, size_t size, const char *fmt, ...) __attribute__((format(printf, 3, 4)));

/**
 * \brief Macro for the #gpr_utils_scnprintf function
 */
#define SCNPRINTF(buf, size, fmt, ...) gpr_utils_scnprintf(buf, size, fmt, ##__VA_ARGS__)

#endif /* H_GPR_UTILS */
