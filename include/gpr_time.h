/*****************************************************************************
 *
 * gpr_time.h
 *
 *****************************************************************************
 * Copyright © 2019 Marc GIANNETTI
 *
 * Time management functions
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

#ifndef H_GPR_TIME
#define H_GPR_TIME

#include <stdio.h>

/*****************************************************************************
 *
 * Copy in a buffer the current local date with format "%d/%m/%Y-%H:%M:%S"
 * Read it as "day/month/year-hour:minutes:seconds"
 *
 * NOTE: The given buffer should be at least "GPR_DATE_SEC_LEN + 1" bytes
 * long and is zero initialized by this function
 *
 * Parameters
 *     psz_buffer: Buffer where to copy the current local date
 *
 * Return value
 *     Returns the total number of characters copied in the buffer
 *     (not including the terminating null-character).
 *     If an error occured, returns zero, and the content of the array
 *     pointed by psz_buffer is indeterminate.
 *
 *****************************************************************************/
#define GPR_DATE_SEC_LEN 19 // Number of bytes needed for "%d/%m/%Y-%H:%M:%S" without null-character
size_t gpr_time_get_date_sec(char * const psz_buffer);

/*****************************************************************************
 *
 * Copy in a buffer the current local date with format "%d/%m/%Y-%H:%M:%S.ms"
 * Read it as "day/month/year-hour:minutes:seconds.milliseconds"
 *
 * NOTE: The given buffer should be at least "GPR_DATE_MILLISEC_LEN + 1"
 * bytes long and is zero initialized by this function
 *
 * Parameters
 *     psz_buffer: Buffer where to copy the current local date
 *
 * Return value
 *     Returns the total number of characters copied in the buffer
 *     (not including the terminating null-character).
 *     If an error occured, returns zero, and the content of the array
 *     pointed by psz_buffer is indeterminate.
 *
 *****************************************************************************/
#define GPR_DATE_MILLISEC_LEN 23 // Number of bytes needed for "%d/%m/%Y-%H:%M:%S.ms" without null-character
size_t gpr_time_get_date_millisec(char * const psz_buffer);

#endif /* H_GPR_TIME */
