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

#include <stdbool.h> // bool
#include <stddef.h>  // size_t
#include <stdlib.h>  // realloc, free

/*
 * Type for the elements of the array (required)
 */
// #define GPR_ARRTPL_TYPE

/*
 * Name of the array structure (required)
 */
// #define GPR_ARRTPL_ST

/*
 * Prefix for function names (required)
 *
 * func Suffix for function name
 */
// #define GPR_ARRTPL_FUNC(func)

// clang-format off
#if !defined(GPR_ARRTPL_TYPE) || !defined(GPR_ARRTPL_ST) || !defined(GPR_ARRTPL_FUNC)
    #error "Missing GPR_ARRTPL_TYPE, GPR_ARRTPL_ST or GPR_ARRTPL_FUNC as template parameters"
#endif
// clang-format on

/**
 * \brief Function to (re)allocate the \e GPR_ARRTPL_TYPE array
 *
 * \note By default, \c realloc is used
 *
 * \param[out] ptr  Pointer to the memory area to be reallocated (\c NULL if first allocation)
 * \param[in]  size Size for the array (re)allocation in bytes
 */
// clang-format off
#ifndef GPR_ARRTPL_REALLOC_FUNC
    #define GPR_ARRTPL_REALLOC_FUNC(ptr, size) realloc(ptr, size)
#endif
// clang-format on

/**
 * \brief Function to free the \e GPR_ARRTPL_TYPE array
 *
 * \note By default, \c free is used
 *
 * \param[out] ptr Pointer to the memory area to free
 */
// clang-format off
#ifndef GPR_ARRTPL_DEALLOC_FUNC
    #define GPR_ARRTPL_DEALLOC_FUNC(ptr) free(ptr)
#endif
// clang-format on

/**
 * \brief Templated dynamic array structure
 */
struct GPR_ARRTPL_ST
{
    size_t size;            ///< Size of the array (Number of elements)
    size_t capacity;        ///< Size of the array (Number of allocated elements)
    GPR_ARRTPL_TYPE *array; ///< Pointer on the array
};

/**
 * \brief Initializes a new templated array
 *
 * \param[out] arr      Templated array to initialize
 * \param[in]  capacity Number of elements to allocate
 *
 * \return Returns \c true if the templated array has been initialized or \c false otherwise
 */
static inline bool GPR_ARRTPL_FUNC(init)(struct GPR_ARRTPL_ST *arr, size_t capacity)
{
#ifdef DEBUG
    /* Check consistency */
    if (arr == NULL)
        return false;
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
        arr->array = (GPR_ARRTPL_TYPE *)GPR_ARRTPL_REALLOC_FUNC(NULL, capacity * sizeof(GPR_ARRTPL_TYPE));
        if (arr->array == NULL)
            return false;
    }

    return true;
}

/**
 * \brief Frees a templated array
 *
 * \param[out] arr Templated array to free
 */
static inline void GPR_ARRTPL_FUNC(free)(struct GPR_ARRTPL_ST *arr)
{
#ifdef DEBUG
    /* Check consistency */
    if (arr == NULL)
        return;
#endif

    /* Free templated array */
    if (arr->capacity != 0)
        GPR_ARRTPL_DEALLOC_FUNC(arr->array);
}

#undef GPR_ARRTPL_TYPE
#undef GPR_ARRTPL_ST
#undef GPR_ARRTPL_FUNC
#undef GPR_ARRTPL_REALLOC_FUNC
#undef GPR_ARRTPL_DEALLOC_FUNC

#endif /* H_GPR_ARRTPL */
