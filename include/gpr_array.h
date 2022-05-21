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

/**
 * \brief Allocates and initializes a new array
 *
 * \return Returns a pointer to the array or \c NULL on allocation failure
 */
struct gpr_array *gpr_array_new(void);

/**
 * \brief Initializes an array
 *
 * \param[out] arr Array to initialize
 */
void gpr_array_init(struct gpr_array *arr);

/**
 * \brief Resets an array by freeing all its elements
 *
 * \note The callback function must deal with a \c void* [\e elem]
 *
 * \param[out] arr       Array where to free elements
 * \param[in]  elem_free Callback function that will be called to free each
 *                       element of the array or \c NULL
 */
void gpr_array_reset(struct gpr_array *arr, void (*elem_free)());

/**
 * \brief Frees an array and all its elements
 *
 * \note The callback function must deal with a \c void* [\e elem]
 * \note Call this function if you allocated an array with #gpr_array_new
 *
 * \param[out] arr       Array to free
 * \param[in]  elem_free Callback function that will be called to free each
 *                       element of the array or \c NULL
 */
void gpr_array_free(struct gpr_array *arr, void (*elem_free)());

/**
 * \brief Adds a new element at the beginning of an array
 *
 * \param[out] arr  Array where to add a new element at the beginning
 * \param[in]  elem New element to add to the array
 *
 * \retval #GPR_ERR_OK The element has been added to the beginning of the array
 * \retval #GPR_ERR_INVALID_PARAMETER The array is \c NULL or the element is \c NULL
 *         (for \e DEBUG mode only)
 * \retval #GPR_ERR_MEMORY_FAILURE The new array allocation failed
 */
enum GPR_Err gpr_array_push_front(struct gpr_array *arr, void *elem);

/**
 * \brief Adds a new element at the end of an array
 *
 * \param[out] arr  Array where to add a new element at the end
 * \param[in]  elem New element to add to the array
 *
 * \retval #GPR_ERR_OK The element has been added to the end of the array
 * \retval #GPR_ERR_INVALID_PARAMETER The array is \c NULL or the element is \c NULL
 *         (for \e DEBUG mode only)
 * \retval #GPR_ERR_MEMORY_FAILURE The new array allocation failed
 */
enum GPR_Err gpr_array_push_back(struct gpr_array *arr, void *elem);

/**
 * \brief Adds a new element at the requested index in an array
 *
 * \param[out] arr   Array where to add a new element
 * \param[in]  elem  Element to add
 * \param[in]  index Insertion index in the array (starting at \e 0)
 *
 * \retval #GPR_ERR_OK The element has been added to the requested index in the array
 * \retval #GPR_ERR_INVALID_PARAMETER The array is \c NULL or the element is \c NULL
 *         or the index is invalid (for \e DEBUG mode only)
 * \retval #GPR_ERR_MEMORY_FAILURE The new array allocation failed
 */
enum GPR_Err gpr_array_insert(struct gpr_array *arr, void *elem, size_t index);

/**
 * \brief Removes the first element of an array
 *
 * \note The callback function must deal with a \c void* [\e elem]
 *
 * \param[out] arr       Array where to remove the first element
 * \param[in]  elem_free Callback function that will be called to free the
 *                       element or \c NULL
 *
 * \retval #GPR_ERR_OK The first element has been removed from the array
 * \retval #GPR_ERR_INVALID_PARAMETER The array is \c NULL (for \e DEBUG mode only)
 * \retval #GPR_ERR_MEMORY_FAILURE The new array allocation failed
 * \retval #GPR_ERR_KO The array is empty
 */
enum GPR_Err gpr_array_pop_front(struct gpr_array *arr, void (*elem_free)());

/**
 * \brief Removes the last element of an array
 *
 * \note The callback function must deal with a \c void* [\e elem]
 *
 * \param[out] arr       Array where to remove the last element
 * \param[in]  elem_free Callback function that will be called to free the
 *                       element or \c NULL
 *
 * \retval #GPR_ERR_OK The last element has been removed from the array
 * \retval #GPR_ERR_INVALID_PARAMETER The array is \c NULL (for \e DEBUG mode only)
 * \retval #GPR_ERR_MEMORY_FAILURE The new array allocation failed
 * \retval #GPR_ERR_KO The array is empty
 */
enum GPR_Err gpr_array_pop_back(struct gpr_array *arr, void (*elem_free)());

/**
 * \brief Removes an element at the requested index in an array
 *
 * \note The callback function must deal with a \c void* [\e elem]
 *
 * \param[out] arr       Array where to remove an element
 * \param[in]  elem_free Callback function that will be called to free the
 *                       element or \c NULL
 * \param[in]  index     Removal index in the array (starting at \e 0)
 *
 * \retval #GPR_ERR_OK The element has been removed to the requested index in the array
 * \retval #GPR_ERR_INVALID_PARAMETER The array is \c NULL or the index is invalid
 *         (for \e DEBUG mode only)
 * \retval #GPR_ERR_MEMORY_FAILURE The new array allocation failed
 * \retval #GPR_ERR_KO The array is empty
 */
enum GPR_Err gpr_array_remove(struct gpr_array *arr, void (*elem_free)(), size_t index);

/**
 * \brief Replaces an element at the requested index in an array
 *
 * \note The callback function must deal with a \c void* [\e elem]
 *
 * \param[out] arr       Array where to replace an element
 * \param[in]  elem_free Callback function that will be called to free the
 *                       previous element or \c NULL
 * \param[in]  elem      New element to replace with
 * \param[in]  index     Replacement index in the array (starting at \e 0)
 *
 * \retval #GPR_ERR_OK The element has been replaced to the requested index in the array
 * \retval #GPR_ERR_INVALID_PARAMETER The array is \c NULL or the element is \c NULL
 *         or the index is invalid (for \e DEBUG mode only)
 * \retval #GPR_ERR_KO The array is empty
 */
enum GPR_Err gpr_array_replace(struct gpr_array *arr, void (*elem_free)(), void *elem, size_t index);

/**
 * \brief Maps an action on each element of an array
 *
 * \note The callback function must deal with a \c void* [\e elem]
 *
 * \param[out] arr      Array where to map an action
 * \param[in]  elem_map Callback function that will be called to map an action
 *                      on each element of an array
 */
void gpr_array_map(struct gpr_array *arr, void (*elem_map)());

/**
 * \brief Searches for an element according to the callback function return
 * state in an array
 *
 * \note The callback function must deal with two \c void* [\e elem, \p ctx]
 *       and returns a \c bool
 *
 * \param[out] arr         Array where to search for an element
 * \param[in]  elem_search Callback function that will be called to search for
 *                         an element
 * \param[in]  ctx         Context that will be given as a parameter to the
 *                         callback function
 * \param[out]  index      Index of the element found or \c NULL
 *
 * \return If the callback function returns \c true, the current element is
 *         returned with an update of the index variable if possible.\n
 *         Returns \c NULL if the end of the array is reached
 */
void *gpr_array_search(const struct gpr_array *arr, bool (*elem_search)(), const void *ctx, size_t *index);

/**
 * \brief Checks if an array is empty
 *
 * \param[in] arr Array to check
 *
 * \return Returns \c true if the array is empty or not allocated, \c false otherwise
 */
bool gpr_array_is_empty(const struct gpr_array *arr);

/**
 * \brief Counts the elements of an array
 *
 * \param[in] arr Array you want the size of
 *
 * \return Returns the array size if allocated or \c 0 otherwise
 */
size_t gpr_array_get_size(const struct gpr_array *arr);

/**
 * \brief Gets the element at the requested index in the array
 *
 * \param[in] arr   Array you want the element of
 * \param[in] index Element index
 *
 * \return Returns the array element if allocated or \c NULL otherwise
 */
void *gpr_array_get_elem(const struct gpr_array *arr, size_t index);

#endif /* H_GPR_ARRAY */
