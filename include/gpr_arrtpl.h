/******************************************************************************
 *
 * \file gpr_arrtpl.h
 * \brief Array template module
 * \details
 * This module defines a way to handle templated dynamic arrays
 *
 * \verbatim
 *
 * ┌─────────────┐
 * │ size(5)     │
 * │ capacity(8) │
 * │ array       │
 * └──┼──────────┘
 *    V
 *    ├─────┐
 *    │ [0] │ = Element 0
 *    │ [1] │ = Element 1
 *    │ [2] │ = Element 2
 *    │ [3] │ = Element 3
 *    │ [4] │ = Element 4
 *    │ [5] │   Uninitialized
 *    │ [6] │   Uninitialized
 *    │ [7] │   Uninitialized
 *    └─────┘
 *
 * \endverbatim
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

#ifndef H_GPR_ARRTPL
#define H_GPR_ARRTPL

#include <stdbool.h> // WARN: Not portable
#include <stdlib.h>
#include <stddef.h>

/**
 * \brief Type for the elements of the array (required)
 */
// #define GPR_ARRTPL_TYPE

/**
 * \brief Name of the array structure (required)
 */
// #define GPR_ARRTPL_ST

/**
 * \brief Prefix for function names (required)
 *
 * \param func Suffix for function name
 */
// #define GPR_ARRTPL_FUNC(func)

#if !defined(GPR_ARRTPL_TYPE) || !defined(GPR_ARRTPL_ST) || !defined(GPR_ARRTPL_FUNC)
    #error "Missing GPR_ARRTPL_TYPE, GPR_ARRTPL_ST or GPR_ARRTPL_FUNC as template parameters"
#endif

/**
 * \brief Function to (re)allocate the GPR_ARRTPL_TYPE array, by default "realloc" is used (optional)
 *
 * \param ptr Pointer to the memory area to be reallocated (NULL if first allocation)
 * \param size Size for the array (re)allocation in bytes
 */
#ifndef GPR_ARRTPL_REALLOC_FUNC
    #define GPR_ARRTPL_REALLOC_FUNC(ptr, size) realloc(ptr, size)
#endif

/**
 * \brief Function to free the GPR_ARRTPL_TYPE array, by default "free" is used (optional)
 *
 * \param ptr Pointer to the memory area to be reallocated (NULL if first allocation)
 * \param size Size for the array (re)allocation in bytes
 */
#ifndef GPR_ARRTPL_DEALLOC_FUNC
    #define GPR_ARRTPL_DEALLOC_FUNC(ptr) free(ptr)
#endif

/**
 * \brief Templated dynamic array structure
 */
struct GPR_ARRTPL_ST
{
    size_t size;            ///< Size of the array (Number of elements)
    size_t capacity;        ///< Size of the array (Number of allocated elements)
    GPR_ARRTPL_TYPE *array; ///< Pointer on the array
};

/******************************************************************************
 *
 * \brief Initialize a new templated array (GPR_ARRTPL_ST)
 *
 * \param arr Templated array to initialize
 * \param capacity Number of elements to allocate
 *
 * \return
 *     True if the templated array has been initialized\n
 *     False otherwise
 *
 *****************************************************************************/
static inline bool GPR_ARRTPL_FUNC(init)(struct GPR_ARRTPL_ST *arr, size_t capacity)
{
#ifdef DEBUG
    /* Check consistency */
    if (arr == NULL)
        return;
#endif

    /* Initialize templated array */
    arr->size = 0;
    arr->capacity = capacity;

    if (arr->capacity == 0)
    {
        arr->array = NULL;
    }
    else
    {
        arr->array = (struct GPR_ARRTPL_ST *)GPR_ARRTPL_REALLOC_FUNC(NULL, capacity * sizeof(GPR_ARRTPL_TYPE));
        if (arr->array == NULL)
            return false;
    }

    return true;
}

/******************************************************************************
 *
 * \brief Free a templated array (GPR_ARRTPL_ST)
 *
 * \param arr Templated array to free
 *
 *****************************************************************************/
static inline void GPR_ARRTPL_FUNC(free)(struct GPR_ARRTPL_ST *arr)
{
#ifdef DEBUG
    /* Check consistency */
    if (arr == NULL)
        return;
#endif

    /* Free templated array */
    if (arr->capacity != 0)
    {
        #ifdef GPR_ARRTPL_DEALLOC_MEMBER
            for(GPR_ARRTPL_TYPE *it = arr->array; it != arr->array + arr->size; it++)
                GPR_ARRTPL_DEALLOC_MEMBER(it);
        #endif

        GPR_ARRTPL_DEALLOC_FUNC(arr->array);
    }
}

#undef GPR_ARRTPL_TYPE
#undef GPR_ARRTPL_ST
#undef GPR_ARRTPL_FUNC
#undef GPR_ARRTPL_REALLOC_FUNC
#undef GPR_ARRTPL_DEALLOC_FUNC

#endif /* H_GPR_ARRTPL */
