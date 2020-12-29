/******************************************************************************
 *
 * \file gpr_err.h
 * \brief Error module
 * \details This module gathers errors that can be returned by GPR modules
 *
 ******************************************************************************
 *
 * \copyright Copyright (c) 2019-2021, GPR Team
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
 * * Neither the name of the GPR Team nor the names of its contributors may
 *   be used to endorse or promote products derived from this software without
 *   specific prior written permission.
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

#ifndef H_GPR_ERR
#define H_GPR_ERR

/**
 * \brief Length of the complementary error message buffer, not including the
 * trailing '\0'
 */
#define CMPL_ERR_MSG_LEN 512 // WARN : Not thread safe

/**
 * \brief Possible errors that can be returned by GPR modules
 */
enum GPR_Err
{
    /* 000 */ GPR_ERR_OK,                ///< Success
    /* 001 */ GPR_ERR_KO,                ///< Generic error
    /* 002 */ GPR_ERR_INVALID_PARAMETER, ///< One or multiple invalid parameter
    /* 003 */ GPR_ERR_MEMORY_FAILURE,    ///< Failure caused by a lack of memory
    /* 004 */ GPR_ERR_NETWORK_ERROR,     ///< A network error occured
    /* xxx */ GPR_ERR_NUMBERS            ///< Number of errors (DO NOT USE)
};

/******************************************************************************
 *
 * \brief Stringify error
 *
 * \param error Error to stringify
 *
 * \return
 *     C-string representing error
 *
 *****************************************************************************/
const char *gpr_err_to_str(enum GPR_Err error);

/******************************************************************************
 *
 * \brief Allocate complementary error message buffer
 *
 *****************************************************************************/
void gpr_err_allocate_cmpl_err(void);

/**
 * \brief Allocate the complementary error message buffer 
 */
#define GPR_INIT_ERR_MODULE gpr_err_allocate_cmpl_err();

/******************************************************************************
 *
 * \brief Free complementary error message buffer
 *
 *****************************************************************************/
void gpr_err_free_cmpl_err(void);

/**
 * \brief Free the complementary error message buffer 
 */
#define GPR_FREE_ERR_MODULE gpr_err_free_cmpl_err();

/******************************************************************************
 *
 * \brief Get last complementary error message
 *
 * \note This complementary error message can be positionned by functions from
 * various GPR modules
 *
 * \return
 *     C-string containing complementary error message
 *
 *****************************************************************************/
char *gpr_err_get_cmpl_err(void);

/******************************************************************************
 *
 * \brief Raise a GPR error and attempts to fill the complementary error
 * message buffer if possible
 *
 * \note A call to this function always reset the previous complementary error
 * message
 *
 * \warning The complementary error message can't exceed CMPL_ERR_MSG_LEN
 * bytes, otherwise it will be truncated by this function
 *
 * \param err GPR error to raise
 * \param fmt Complementary error message format (Can be NULL)
 * \param ... Optional arguments
 *
 * \return
 *     The GPR error specified in parameter
 *
 *****************************************************************************/
enum GPR_Err gpr_err_raise(enum GPR_Err err, const char *const fmt, ...) __attribute__((format(printf, 2, 3)));

#endif /* H_GPR_ERR */
