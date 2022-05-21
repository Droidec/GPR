/******************************************************************************
 *
 * \file gpr_err.c
 * \brief Error module
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

#include "gpr_err.h"

#include <stdarg.h> // va_list, va_start, va_end
#include <stdlib.h> // malloc, free

#include "gpr_utils.h"

/**
 * \brief Possible errors strings
 */
static const char *Err_Array[] = {
    /* 000 */ "Success",
    /* 001 */ "Failure",
    /* 002 */ "Invalid parameter",
    /* 003 */ "Memory failure",
    /* 004 */ "Loop detected",
    /* 005 */ "Not implemented",
    /* 006 */ "Network error",
};

/**
 * \brief Error message (\e thread-safe)
 */
__thread char Err_Msg[GPR_ERR_MSG_LEN + 1] = {0};

const char *gpr_err_to_str(enum GPR_Err error)
{
#ifdef DEBUG
    /* Check consistency */
    if ((error < 0) || (error >= GPR_ERR_NUMBERS))
        return "Unknown error";
#endif

    return Err_Array[error];
}

char *gpr_err_get_msg(void)
{
    return Err_Msg;
}

enum GPR_Err gpr_err_raise(enum GPR_Err err, const char *fmt, ...)
{
    va_list list;

    if (fmt != NULL)
    {
        va_start(list, fmt);
        VSCNPRINTF(Err_Msg, GPR_ERR_MSG_LEN + 1, fmt, list);
        va_end(list);
    }
    else
    {
        Err_Msg[0] = '\0';
    }

    return err;
}
