/******************************************************************************
 *
 * \file gpr_buf.h
 * \brief Buffer module
 * \details
 * This modules defines a way to handle data by declaring buffers
 *
 * A buffer is declared on the heap, has a fixed size which cannot exceed
 * UINT_MAX bytes (~ 4Gb) and is composed of offset pointers (ofs_)
 *
 * \verbatim
 *
 * buf            ofs_b     ofs_d                    ofs_e
 * │                │         │                        │
 * v                v         v                        v
 * ├────────────────┼─────────┼────────────────────────┼─────────────────────┤
 *                  ===================================
 *                               used area
 * \endverbatim
 *
 * Inside the buffer is a used area delimited by two offset pointers:
 * - 'ofs_b' represents the *b*egin pointer of the used area
 * - 'ofs_e' represents the *e*nd pointer of the used area
 * The used area itself contains the active data the program can work on,
 * using the 'ofs_d' *d*ecoding pointer, which can can be freely moved
 *
 * The free area is delimited by the 'ofs_e' pointer and the buffer size
 * The rest area is delimited by the 'ofs_d' pointer and the 'ofs_e' pointer
 *
 * As the buffer is composed of standard C data types, it can be used with any
 * standard functions, such as: read, send, recv, ...
 * The only additional step is to:
 * - Update the used area at successful reception by moving the 'ofs_e' pointer
 * - Update the used area at successful sending by moving the 'ofs_b' pointer
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

#ifndef H_GPR_BUF
#define H_GPR_BUF

#include <stdbool.h> // WARN: Not portable

/**
 * \brief GPR buffer structure
 */
struct gpr_buffer
{
    unsigned char *buf;   ///< Buffer pointer
    unsigned char *ofs_b; ///< Begin pointer of the used area (Should always be <= ofs_e)
    unsigned char *ofs_e; ///< End pointer of the used area (Should always be >= ofs_b)
    unsigned char *ofs_d; ///< Decode pointer of the used area (Should always be >= ofs_b and <= ofs_e)
    unsigned int size;    ///< Size of the buffer (Number of bytes)
};

/******************************************************************************
 *
 * \brief Allocate and initialize a new buffer
 *
 * \n Offset pointers are set at the very first byte of the buffer
 *
 * \param size Size of the buffer to allocate (Number of bytes)
 *
 * \return
 *     On success, return a buffer allocated and initialized\n
 *     On failure, return NULL. This can occur if allocation failed
 *
 *****************************************************************************/
struct gpr_buffer *gpr_buf_new(unsigned int size);

/******************************************************************************
 *
 * \brief Reset the offset pointers of a buffer
 *
 * |param buf Buffer to reset
 *
 *****************************************************************************/
void gpr_buf_reset(struct gpr_buffer *buf);

/******************************************************************************
 *
 * \brief Free a buffer
 *
 * \param buf Buffer to free
 *
 *****************************************************************************/
void gpr_buf_free(struct gpr_buffer *buf);

/******************************************************************************
 *
 * \brief Check if a buffer is empty
 *
 * \param buf Buffer to check
 *
 * \return
 *     True if the buffer is empty\n
 *     False otherwise
 *
 *****************************************************************************/
bool gpr_buf_is_empty(const struct gpr_buffer *buf);

/******************************************************************************
 *
 * \brief Get the size of a buffer
 *
 * \param buf Buffer you want the size of
 *
 * \return
 *     The size of the buffer
 *
 *****************************************************************************/
unsigned int gpr_buf_get_size(const struct gpr_buffer *buf);

/******************************************************************************
 *
 * \brief Get the used size of a buffer (offset between ofs_e and ofs_b)
 *
 * \param buf Buffer you want the used size of
 *
 * \return
 *     The used size of the buffer
 *
 *****************************************************************************/
unsigned int gpr_buf_get_used_size(const struct gpr_buffer *buf);

/******************************************************************************
 *
 * \brief Get the free size of a buffer (offset between ofs_e and buffer size)
 *
 * \param buf Buffer you want the free size of
 *
 * \return
 *     The free size of the buffer
 *
 *****************************************************************************/
unsigned int gpr_buf_get_free_size(const struct gpr_buffer *buf);

/******************************************************************************
 *
 * \brief Get the rest size of a buffer (offset between ofs_d and ofs_e)
 *
 * \param buf Buffer you want the rest size of
 *
 * \return
 *     The rest size of the buffer
 *
 *****************************************************************************/
unsigned int gpr_buf_get_rest_size(const struct gpr_buffer *buf);

#endif /* H_GPR_BUF */
