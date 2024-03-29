/******************************************************************************
 *
 * \file gpr_str.c
 * \brief String module
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

#include "gpr_str.h"

#include <ctype.h>  // tolower, toupper, isspace
#include <stddef.h> // size_t
#include <string.h> // strlen

size_t gpr_str_tolower(char *dst, const char *src, size_t num)
{
    size_t count = 0;

#ifdef DEBUG
    /* Check consistency */
    if ((dst == NULL) || (src == NULL))
        return 0;
#endif

    for (size_t i = 0; i < num; i++)
    {
        if (src[i] == '\0')
            break;
        dst[i] = tolower(src[i]);
        count++;
    }

    return count;
}

size_t gpr_str_toupper(char *dst, const char *src, size_t num)
{
    size_t count = 0;

#ifdef DEBUG
    /* Check consistency */
    if ((dst == NULL) || (src == NULL))
        return 0;
#endif

    for (size_t i = 0; i < num; i++)
    {
        if (src[i] == '\0')
            break;
        dst[i] = toupper(src[i]);
        count++;
    }

    return count;
}

char *gpr_str_ltrim(char *str)
{
#ifdef DEBUG
    /* Check consistency */
    if (str == NULL)
        return NULL;
#endif

    // Empty string
    if (*str == '\0')
        return str;

    // Process from the start
    while (isspace(*str))
        str++;

    return str;
}

char *gpr_str_rtrim(char *str)
{
    char *end = NULL;

#ifdef DEBUG
    /* Check consistency */
    if (str == NULL)
        return NULL;
#endif

    // Empty string
    if (*str == '\0')
        return str;

    // Process from the end
    end = str + strlen(str);
    while (isspace(*--end))
        ;
    *(end + 1) = '\0';

    return str;
}

char *gpr_str_trim(char *str)
{
#ifdef DEBUG
    /* Check consistency */
    if (str == NULL)
        return NULL;
#endif

    // Apply leading and trailing functions
    return gpr_str_ltrim(gpr_str_rtrim(str));
}
