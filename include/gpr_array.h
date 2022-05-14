/******************************************************************************
 *
 * \file gpr_array.h
 * \brief Array module
 * \details This module defines a way to handle generic dynamic arrays
 *
 * \verbatim
 *
 * ┌─────────┐
 * │ size(3) │
 * │ elem    │
 * └──┼──────┘
 *    ├─────┐
 *    │ [0] ┼─> Element 0
 *    │ [1] ┼─> Element 1
 *    │ [2] ┼─> Element 2
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

#ifndef H_GPR_ARRAY
#define H_GPR_ARRAY

#include <stdbool.h> // bool
#include <stddef.h>  // size_t

#include "gpr_err.h"

/**
 * \brief Dynamic array structure
 */
struct gpr_array
{
    size_t size; ///< Size of the array (Number of allocated elements)
    void **elem; ///< Pointer on the array
};

/******************************************************************************
 *
 * \brief Allocate and initialize a new array
 *
 * \return
 *     On success, returns a pointer to the array\n
 *     On failure, returns NULL
 *
 *****************************************************************************/
struct gpr_array *gpr_array_new(void);

/******************************************************************************
 *
 * \brief Initialize an array
 *
 * \param arr Array to initialize
 *
 *****************************************************************************/
void gpr_array_init(struct gpr_array *arr);

/******************************************************************************
 *
 * \brief Reset an array (Free all its elements)
 *
 * \note The callback function must deal with a void * (element)
 *
 * \param arr       Array where to free elements
 * \param elem_free Callback function that will be called to free each element
 *                  of the array (Can be NULL)
 *
 *****************************************************************************/
void gpr_array_reset(struct gpr_array *arr, void (*elem_free)());

/******************************************************************************
 *
 * \brief Free an array and all its elements
 *
 * \note The callback function must deal with a void * (element)
 *
 * \param arr       Array to free
 * \param elem_free Callback function that will be called to free each element
 *                  of the array (Can be NULL)
 *
 *****************************************************************************/
void gpr_array_free(struct gpr_array *arr, void (*elem_free)());

/******************************************************************************
 *
 * \brief Add a new element at the beginning of an array
 *
 * \param arr  Array where to add a new element at the beginning
 * \param elem New element to add to the array
 *
 * \return
 *     GPR_ERR_OK: The element has been added to the beginning of the array\n
 *     GPR_ERR_INVALID_PARAMETER: The array is NULL or the element is NULL
 *     (DEBUG mode only)\n
 *     GPR_ERR_MEMORY_FAILURE: The new array allocation failed
 *
 *****************************************************************************/
enum GPR_Err gpr_array_push_front(struct gpr_array *arr, void *elem);

/******************************************************************************
 *
 * \brief Add a new element at the end of an array
 *
 * \param arr  Array where to add a new element at the end
 * \param elem New element to add to the array
 *
 * \return
 *     GPR_ERR_OK: The element has been added to the end of the array\n
 *     GPR_ERR_INVALID_PARAMETER: The array is NULL or the element is NULL
 *     (DEBUG mode only)\n
 *     GPR_ERR_MEMORY_FAILURE: The new array allocation failed
 *
 *****************************************************************************/
enum GPR_Err gpr_array_push_back(struct gpr_array *arr, void *elem);

/******************************************************************************
 *
 * \brief Add a new element at the requested index in an array
 *
 * \param arr   Array where to add a new element
 * \param elem  Element to add
 * \param index Insertion index in the array (starting at 0)
 *
 * \return
 *     GPR_ERR_OK: The element has been added to the requested index in
 *     the array\n
 *     GPR_ERR_INVALID_PARAMETER: The array is NULL or the element is NULL or
 *     the index is invalid (DEBUG mode only)\n
 *     GPR_ERR_MEMORY_FAILURE: The new array allocation failed
 *
 *****************************************************************************/
enum GPR_Err gpr_array_insert(struct gpr_array *arr, void *elem, size_t index);

/******************************************************************************
 *
 * \brief Remove the first element of an array
 *
 * \note The callback function must deal with a void * (element)
 *
 * \param arr       Array where to remove the first element
 * \param elem_free Callback function that will be called to free the element
 *                  (Can be NULL)
 *
 * \return
 *     GPR_ERR_OK: The first element has been removed from the array\n
 *     GPR_ERR_INVALID_PARAMETER: The array is NULL (DEBUG mode only)\n
 *     GPR_ERR_MEMORY_FAILURE: The new array allocation failed\n
 *     GPR_ERR_KO: The array is empty
 *
 *****************************************************************************/
enum GPR_Err gpr_array_pop_front(struct gpr_array *arr, void (*elem_free)());

/******************************************************************************
 *
 * \brief Remove the last element of an array
 *
 * \note The callback function must deal with a void * (element)
 *
 * \param arr       Array where to remove the last element
 * \param elem_free Callback function that will be called to free the element
 *                  (Can be NULL)
 *
 * \return
 *     GPR_ERR_OK: The last element has been removed from the array\n
 *     GPR_ERR_INVALID_PARAMETER: The array is NULL (DEBUG mode only)\n
 *     GPR_ERR_MEMORY_FAILURE: The new array allocation failed\n
 *     GPR_ERR_KO: The array is empty
 *
 *****************************************************************************/
enum GPR_Err gpr_array_pop_back(struct gpr_array *arr, void (*elem_free)());

/******************************************************************************
 *
 * \brief Remove an element at the requested index in an array
 *
 * \note The callback function must deal with a void * (element)
 *
 * \param arr       Array where to remove an element
 * \param elem_free Callback function that will be called to free the element
 *                  (Can be NULL)
 * \param index     Removal index in the array (starting at 0)
 *
 * \return
 *     GPR_ERR_OK: The element has been removed to the requested index in
 *     the array\n
 *     GPR_ERR_INVALID_PARAMETER: The array is NULL or the index is
 *     invalid (DEBUG mode only)\n
 *     GPR_ERR_MEMORY_FAILURE: The new array allocation failed\n
 *     GPR_ERR_KO: The array is empty
 *
 *****************************************************************************/
enum GPR_Err gpr_array_remove(struct gpr_array *arr, void (*elem_free)(), size_t index);

/******************************************************************************
 *
 * \brief Replace an element at the requested index in an array
 *
 * \note The callback function must deal with a void * (element)
 *
 * \param arr       Array where to replace an element
 * \param elem_free Callback function that will be called to free the previous
 *                  element (Can be NULL)
 * \param elem      New element to replace with
 * \param index     Replacement index in the array (starting at 0)
 *
 * \return
 *     GPR_ERR_OK: The element has been replaced to the requested index in
 *     the array\n
 *     GPR_ERR_INVALID_PARAMETER: The array is NULL or the element is NULL or
 *     the index is invalid (DEBUG mode only)\n
 *     GPR_ERR_KO: The array is empty
 *
 *****************************************************************************/
enum GPR_Err gpr_array_replace(struct gpr_array *arr, void (*elem_free)(), void *elem, size_t index);

/******************************************************************************
 *
 * \brief Map an action on each element of an array
 *
 * \note The callback function must deal with a void * (element)
 *
 * \param arr      Array where to map an action
 * \param elem_map Callback function that will be called to map an action
 *                 on each element of an array
 *
 *****************************************************************************/
void gpr_array_map(struct gpr_array *arr, void (*elem_map)());

/******************************************************************************
 *
 * \brief Search for an element according to the callback function return
 * state in an array
 *
 * \note The callback function must deal with two void * (element, context)
 *
 * \param arr         Array where to search for an element
 * \param elem_search Callback function that will be called to search for an
 *                    element
 * \param ctx         Context that will be given as a parameter to the
 *                    callback function
 * \param index       Index of the element found (Can be NULL)
 *
 * \return
 *     If the callback function returns true, the current element is returned
 *     with an update of the index variable if possible\n
 *     Return NULL if the end of the array is reached
 *
 *****************************************************************************/
void *gpr_array_search(const struct gpr_array *arr, bool (*elem_search)(), const void *ctx, size_t *index);

/******************************************************************************
 *
 * \brief Check if an array is empty
 *
 * \param arr Array to check
 *
 * \return
 *     True if the array is empty or not allocated\n
 *     False otherwise
 *
 *****************************************************************************/
bool gpr_array_is_empty(const struct gpr_array *arr);

/******************************************************************************
 *
 * \brief Get the size of an array (its number of elements)
 *
 * \param arr Array you want the size of
 *
 * \return
 *     The array size if allocated\n
 *     Zero otherwise
 *
 *****************************************************************************/
size_t gpr_array_get_size(const struct gpr_array *arr);

/******************************************************************************
 *
 * \brief Get the element at the requested index in the array
 *
 * \param arr   Array you want the element of
 * \param index Element index
 *
 * \return
 *     The array element if allocated\n
 *     NULL otherwise
 *
 *****************************************************************************/
void *gpr_array_get_elem(const struct gpr_array *arr, size_t index);

#endif /* H_GPR_ARRAY */
