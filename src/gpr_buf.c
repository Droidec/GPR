/******************************************************************************
 *
 * \file gpr_buf.c
 * \brief Buffer module
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

#include "gpr_buf.h"

#include <stdbool.h> // bool
#include <stdlib.h>  // malloc, free

#include "gpr_utils.h"

/******************************************************************************
 * Public functions
 *****************************************************************************/

struct gpr_buffer *gpr_buf_new(unsigned int size)
{
    struct gpr_buffer *buf = NULL;

    /* Allocate buffer */

    // Allocate structure
    buf = (struct gpr_buffer *)malloc(sizeof(struct gpr_buffer));
    if (UNLIKELY(buf == NULL))
        return NULL;

    // Allocate buffer
    buf->buf = (unsigned char *)malloc(sizeof(unsigned char) * size);
    if (UNLIKELY(buf->buf == NULL))
    {
        free(buf);
        return NULL;
    }

    /* Initialize buffer */
    gpr_buf_reset(buf);
    buf->size = size;

    return buf;
}

void gpr_buf_reset(struct gpr_buffer *buf)
{
#ifdef DEBUG
    /* Check consistency */
    if (buf == NULL)
        return;
#endif

    /* Reset offset pointers */
    buf->ofs_b = buf->buf;
    buf->ofs_e = buf->buf;
    buf->ofs_d = buf->buf;
}

void gpr_buf_free(struct gpr_buffer *buf)
{
#ifdef DEBUG
    /* Check consistency */
    if ((buf == NULL) || (buf->buf == NULL))
        return;
#endif

    /* Free buffer */
    free(buf->buf);

    /* Free structure */
    free(buf);
}

bool gpr_buf_is_empty(const struct gpr_buffer *buf)
{
#ifdef DEBUG
    /* Check consistency */
    if (buf == NULL)
        return true;
#endif

    return buf->ofs_b == buf->ofs_e ? true : false;
}

unsigned int gpr_buf_get_size(const struct gpr_buffer *buf)
{
#ifdef DEBUG
    /* Check consistency */
    if (buf == NULL)
        return 0;
#endif

    return buf->size;
}

unsigned int gpr_buf_get_used_size(const struct gpr_buffer *buf)
{
#ifdef DEBUG
    /* Check consistency */
    if (buf == NULL)
        return 0;
#endif

    return (buf->ofs_e - buf->ofs_b);
}

unsigned int gpr_buf_get_free_size(const struct gpr_buffer *buf)
{
#ifdef DEBUG
    /* Check consistency */
    if (buf == NULL)
        return 0;
#endif

    return (buf->size - (buf->ofs_e - buf->buf));
}

unsigned int gpr_buf_get_rest_size(const struct gpr_buffer *buf)
{
#ifdef DEBUG
    /* Check consistency */
    if (buf == NULL)
        return 0;
#endif

    return (buf->ofs_e - buf->ofs_d);
}
