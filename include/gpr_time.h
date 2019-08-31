/*****************************************************************************
*
* gpr_time.h: Time manipulation functions
*
******************************************************************************
*
* Author: GIANNETTI Marc <derovor@gmail.com>
*
*****************************************************************************/

#ifndef H_GPR_TIME
#define H_GPR_TIME

#include <time.h>

#define DATE_IN_SEC 19
size_t GPR_TIME_get_date_in_sec(char *p_buffer, unsigned int i_buffer_len);

#endif /* H_GPR_TIME */
