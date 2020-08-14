/*****************************************************************************
 *
 * gpr_err.c
 *
 *****************************************************************************
 * Copyright © 2020 Marc GIANNETTI
 *
 * Error management functions
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

#include "gpr_err.h"
#include "gpr_builtin.h"

#include <stdio.h>
#include <stdlib.h>

/*****************************************************************************
 * Private prototypes
 *****************************************************************************/

static const char *Err_Array[] =
{
    /* 000 */ "Success",
    /* 001 */ "Failure"
};

char *Cmpl_Err_Msg = NULL; // Complementary Error Message

/*****************************************************************************
 * Public functions
 *****************************************************************************/

const char *gpr_err_to_str(enum GPR_Err error)
{
    /* Check consistency */
    if (error < 0 || error >= GPR_ERR_NUMBERS)
        return "UNKNOWN";

    return Err_Array[error];
}

void gpr_err_allocate_cmpl_err(void)
{
    Cmpl_Err_Msg = (char *)malloc(CMPL_ERR_MSG_LEN + 1);
}

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

enum GPR_Err gpr_err_raise(enum GPR_Err err, char *psz_cmpl_err_msg)
{
    if (Cmpl_Err_Msg != NULL && psz_cmpl_err_msg != NULL && psz_cmpl_err_msg[0] != '\0')
        SCNPRINTF(Cmpl_Err_Msg, CMPL_ERR_MSG_LEN + 1, "%s", psz_cmpl_err_msg);

    return err;
}
