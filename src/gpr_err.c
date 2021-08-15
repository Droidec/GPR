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
#include "gpr_builtin.h"

#include <stdio.h>
#include <stdlib.h>

/******************************************************************************
 * Private prototypes
 *****************************************************************************/

/**
 * \brief Possible errors strings
 */
static const char *Err_Array[] = {
    /* 000 */ "Success",
    /* 001 */ "Failure",
    /* 002 */ "Invalid parameter",
    /* 003 */ "Memory failure",
    /* 004 */ "Network error",
};

/**
 * \brief Complementary error message
 */
char *Cmpl_Err_Msg = NULL;

/******************************************************************************
 * Public functions
 *****************************************************************************/

const char *gpr_err_to_str(enum GPR_Err error)
{
#ifdef DEBUG
    /* Check consistency */
    if (error < 0 || error >= GPR_ERR_NUMBERS)
        return "UNKNOWN";
#endif

    return Err_Array[error];
}

void gpr_err_allocate_cmpl_err(void) { Cmpl_Err_Msg = (char *)malloc(CMPL_ERR_MSG_LEN + 1); }

void gpr_err_free_cmpl_err(void)
{
    if (Cmpl_Err_Msg != NULL)
        free(Cmpl_Err_Msg);
}

char *gpr_err_get_cmpl_err(void)
{
    if (Cmpl_Err_Msg == NULL)
        return "";

    return Cmpl_Err_Msg;
}

enum GPR_Err gpr_err_raise(enum GPR_Err err, const char *const fmt, ...)
{
    va_list list;

    if (Cmpl_Err_Msg != NULL)
    {
        if (fmt != NULL)
        {
            va_start(list, fmt);
            VSCNPRINTF(Cmpl_Err_Msg, CMPL_ERR_MSG_LEN + 1, fmt, list);
            va_end(list);
        }
        else
        {
            Cmpl_Err_Msg[0] = '\0';
        }
    }

    return err;
}
