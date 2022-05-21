/******************************************************************************
 *
 * \file gpr_err.h
 * \brief Error module
 * \details This module gathers errors that can be returned by GPR modules
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

#ifndef H_GPR_ERR
#define H_GPR_ERR

/**
 * \brief Length of the error message buffer, not including the trailing \c \0
 */
#define GPR_ERR_MSG_LEN 512

/**
 * \brief Possible errors that can be returned by GPR modules
 */
enum GPR_Err
{
    /* 000 */ GPR_ERR_OK,                ///< Success
    /* 001 */ GPR_ERR_KO,                ///< Generic error
    /* 002 */ GPR_ERR_INVALID_PARAMETER, ///< One or multiple invalid parameter
    /* 003 */ GPR_ERR_MEMORY_FAILURE,    ///< Failure caused by a lack of memory
    /* 004 */ GPR_ERR_LOOP_DETECTED,     ///< An infinite loop has been detected
    /* 005 */ GPR_ERR_NOT_IMPLEMENTED,   ///< Feature not implemented
    /* 006 */ GPR_ERR_NETWORK_ERROR,     ///< A network error occurred
    /* xxx */ GPR_ERR_NUMBERS            ///< Number of errors (*DO NOT USE*)
};

/**
 * \brief Gets error string
 *
 * \param[in] error Error to stringify
 *
 * \return Returns the C-string representing error
 */
const char *gpr_err_to_str(enum GPR_Err error);

/**
 * \brief Gets error message
 *
 * \note This error message can be positionned by functions from various GPR modules
 *
 * \return Returns the C-string containing error message
 */
char *gpr_err_get_msg(void);

/**
 * \brief Raises a GPR error and fills the error message buffer
 *
 * \note This function always resets the error message, even if \c NULL is passed
 *
 * \warning The error message can't exceed #GPR_ERR_MSG_LEN bytes, otherwise it will be
 *          truncated by this function
 *
 * \param[in] err GPR error to raise
 * \param[in] fmt Error message format or \c NULL
 * \param[in] ... Optional arguments
 *
 * \return Returns \p err
 */
enum GPR_Err gpr_err_raise(enum GPR_Err err, const char *fmt, ...) __attribute__((format(printf, 2, 3)));

#endif /* H_GPR_ERR */
