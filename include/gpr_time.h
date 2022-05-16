/******************************************************************************
 *
 * \file gpr_time.h
 * \brief Time module
 * \details This module offers additional functions to manipulate time
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

#ifndef H_GPR_TIME
#define H_GPR_TIME

#include <stddef.h> // size_t

/******************************************************************************
 *
 * \brief Copy in a buffer the current local date with format
 * "%d/%m/%Y-%H:%M:%S".\n
 * Read it as "day/month/year-hour:minutes:seconds"
 *
 * \warning The given buffer should be at least "GPR_DATE_SEC_LEN + 1" bytes
 * long
 *
 * \param buffer Buffer where to copy the current local date
 *
 * \return
 *     Returns the total number of characters copied in the buffer
 *     (not including the terminating null-character)\n
 *     If an error occurred, returns zero, and the content of the array
 *     pointed by buffer is indeterminate
 *
 *****************************************************************************/
size_t gpr_time_get_date_sec(char *buffer);

/**
 * \brief Number of bytes needed for the \a gpr_time_get_date_sec function
 * without the null character
 */
#define GPR_DATE_SEC_LEN 19

/******************************************************************************
 *
 * \brief Copy in a buffer the current local date with format
 * "%d/%m/%Y-%H:%M:%S.ms".\n
 * Read it as "day/month/year-hour:minutes:seconds.milliseconds"
 *
 * \warning The given buffer should be at least "GPR_DATE_MILLISEC_LEN + 1"
 * bytes long
 *
 * \param buffer Buffer where to copy the current local date
 *
 * \return
 *     Returns the total number of characters copied in the buffer
 *     (not including the terminating null-character)\n
 *     If an error occurred, returns zero, and the content of the array
 *     pointed by buffer is indeterminate
 *
 *****************************************************************************/
size_t gpr_time_get_date_millisec(char *buffer);

/**
 * \brief Number of bytes needed for the \a gpr_time_get_date_millisec function
 * without the null character
 */
#define GPR_DATE_MILLISEC_LEN 23

#endif /* H_GPR_TIME */
