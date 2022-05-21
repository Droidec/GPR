/******************************************************************************
 *
 * \file gpr_buf.h
 * \brief Buffer module
 * \details
 * This module defines a way to handle data by declaring buffers
 *
 * A buffer is declared on the heap, has a fixed size which cannot exceed
 * \c UINT_MAX bytes (~ 4Gb) and is composed of offset pointers (*ofs_*)
 *
 * \verbatim
 *
 * buf            ofs_b     ofs_d                    ofs_e
 * │                │         │                        │
 * v                v         v                        v
 * ├────────────────┼─────────┼────────────────────────┼─────────────────────┤
 *                                                     ======================
 *                                                           free size
 *                            =========================
 *                                    rest size
 *                  ===================================
 *                               used size
 * \endverbatim
 *
 * Inside the buffer is a used area delimited by two offset pointers:
 * - \c ofs_b represents the *b*egin pointer of the used area
 * - \c ofs_e represents the *e*nd pointer of the used area
 *
 * The used area itself contains the active data the program can work on,
 * using the \c ofs_d *d*ecoding pointer, which can can be freely moved
 *
 * The free area is delimited by the \c ofs_e pointer and the buffer size\n
 * The rest area is delimited by the \c ofs_d pointer and the \c ofs_e pointer
 *
 * As the buffer is composed of standard C data types, it can be used with any
 * standard functions, such as: \c read, \c send, \c recv, ...\n
 * The only additional step is to:
 * - Update the used area at successful reception by moving the \c ofs_e pointer
 * - Update the used area at successful sending by moving the \c ofs_b pointer
 *
 * When the buffer is full, the \c ofs_e pointer will be in overflow of 1 byte,
 * but arithmetic pointers will still work, even with cyclic memory addresses
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

#include <stdbool.h> // bool

#include "gpr_err.h"

/**
 * \brief Buffer structure
 */
struct gpr_buffer
{
    unsigned char *buf;   ///< Buffer pointer
    unsigned char *ofs_b; ///< Begin pointer of the used area
    unsigned char *ofs_e; ///< End pointer of the used area
    unsigned char *ofs_d; ///< Decode pointer of the used area
    unsigned int size;    ///< Size of the buffer (Number of bytes)
};

/**
 * \brief Allocates and initializes a new buffer
 *
 * \note Offset pointers are set at the very first byte of the buffer
 *
 * \param[in] size Size of the buffer to allocate (Number of bytes)
 *
 * \return Returns a buffer allocated and initialized or \c NULL on allocation failure
 */
struct gpr_buffer *gpr_buf_new(unsigned int size);

/**
 * \brief Allocates and initializes a new internal buffer
 *
 * \note Offset pointers are set at the very first byte of the buffer
 *
 * \param[out] buf Buffer to allocate and initialize
 * \param[in] size Size of the buffer to allocate (Number of bytes)
 *
 * \retval #GPR_ERR_OK The buffer has been allocated
 * \retval #GPR_ERR_MEMORY_FAILURE The buffer allocation failed
 */
enum GPR_Err gpr_buf_new_buffer(struct gpr_buffer *buf, unsigned int size);

/**
 * \brief Resets the offset pointers of a buffer
 *
 * \param[out] buf Buffer to reset
 */
void gpr_buf_reset(struct gpr_buffer *buf);

/**
 * \brief Frees a buffer
 *
 * \note Call this function if you allocated a buffer with #gpr_buf_new
 *
 * \param[out] buf Buffer to free
 */
void gpr_buf_free(struct gpr_buffer *buf);

/**
 * \brief Frees an internal buffer
 *
 * \note Call this function if you allocated a buffer with #gpr_buf_new_buffer
 *
 * \param[out] buf Buffer to free
 */
void gpr_buf_free_buffer(struct gpr_buffer *buf);

/**
 * \brief Checks if a buffer is empty
 *
 * \param[in] buf Buffer to check
 *
 * \return Returns \c true if the buffer is empty, \c false otherwise
 */
bool gpr_buf_is_empty(const struct gpr_buffer *buf);

/**
 * \brief Gets the size of a buffer
 *
 * \param[in] buf Buffer you want the size of
 *
 * \return Returns the size of the buffer in byte
 */
unsigned int gpr_buf_get_size(const struct gpr_buffer *buf);

/**
 * \brief Gets the used size of a buffer (offset between \c ofs_e and \c ofs_b)
 *
 * \param[in] buf Buffer you want the used size of
 *
 * \return Returns the used size of the buffer in byte
 */
unsigned int gpr_buf_get_used_size(const struct gpr_buffer *buf);

/**
 * \brief Gets the free size of a buffer (offset between \c ofs_e and buffer size)
 *
 * \param[in] buf Buffer you want the free size of
 *
 * \return Returns the free size of the buffer in byte
 */
unsigned int gpr_buf_get_free_size(const struct gpr_buffer *buf);

/**
 * \brief Gets the rest size of a buffer (offset between \c ofs_d and \c ofs_e)
 *
 * \param[in] buf Buffer you want the rest size of
 *
 * \return Returns the rest size of the buffer in byte
 */
unsigned int gpr_buf_get_rest_size(const struct gpr_buffer *buf);

#endif /* H_GPR_BUF */
