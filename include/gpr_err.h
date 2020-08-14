/*****************************************************************************
 *
 * gpr_err.h
 *
 *****************************************************************************
 * Copyright © 2020 Marc GIANNETTI
 *
 * Error management functions
 *
 * Error module offers the following possibilities:
 * - Get the error code/string encountered by a GPR module
 * - Get a complementary error message if allocated
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

#ifndef H_GPR_ERR
#define H_GPR_ERR

#define CMPL_ERR_MSG_LEN 512

enum GPR_Err
{
    /* 000 */ GPR_ERR_OK,
    /* 001 */ GPR_ERR_KO,
    /* xxx */ GPR_ERR_NUMBERS
};

/*****************************************************************************
 *
 * Stringify error
 *
 * Parameters
 *     error : Error to stringify
 *
 * Return value
 *     C-string representing error
 *
 *****************************************************************************/
const char *gpr_err_to_str(enum GPR_Err error);

/*****************************************************************************
 *
 * Allocate complementary error message buffer
 *
 * Parameters
 *     None
 *
 * Return value
 *     None
 *
 *****************************************************************************/
void gpr_err_allocate_cmpl_err(void);

/*****************************************************************************
 *
 * Free complementary error message buffer
 *
 * Parameters
 *     None
 *
 * Return value
 *     None
 *
 *****************************************************************************/
void gpr_err_free_cmpl_err(void);

/*****************************************************************************
 *
 * Get last complementary error message
 *
 * NOTE : This complementary error message can be positionned by functions
 * from various GPR modules
 *
 * Parameters
 *     None
 *
 * Return value
 *     C-string containing complementary error message
 *
 *****************************************************************************/
char *gpr_err_get_cmpl_err(void);

/*****************************************************************************
 *
 * Raise a GPR error and fill complementary error message buffer if allocated
 *
 * WARN: The complementary error message can't exceed CMPL_ERR_MSG_LEN bytes,
 * otherwise it will be truncated by this function
 *
 * Parameters
 *     err              : GPR error to raise
 *     psz_cmpl_err_msg : Complementary error message to write
 *
 * Return value
 *     The GPR error specified in parameters
 *
 *****************************************************************************/
enum GPR_Err gpr_err_raise(enum GPR_Err err, char *psz_cmpl_err_msg);

#endif /* H_GPR_ERR */
