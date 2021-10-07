/******************************************************************************
 *
 * \file gpr_array.c
 * \brief Array module
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

#include "gpr_array.h"
#include "gpr_utils.h"

#include <stdlib.h>
#include <string.h>

/******************************************************************************
 * Public functions
 *****************************************************************************/

struct gpr_array *gpr_array_new(void)
{
    struct gpr_array *arr = NULL;

    /* Allocate array */
    arr = (struct gpr_array *)malloc(sizeof(struct gpr_array));
    if (UNLIKELY(arr == NULL))
        return NULL;

    /* Initialize array */
    gpr_array_init(arr);

    return arr;
}

void gpr_array_init(struct gpr_array *arr)
{
#ifdef DEBUG
    /* Check consistency */
    if (arr == NULL)
        return;
#endif

    /* Initialize array */
    arr->size = 0;
    arr->elem = NULL;
}

void gpr_array_reset(struct gpr_array *arr, void (*elem_free)())
{
#ifdef DEBUG
    /* Check consistency */
    if ((arr == NULL) || (arr->size == 0))
        return;
#endif

    /* Free each element */
    for (size_t index = 0; index < arr->size; index++)
        if ((arr->elem[index] != NULL) && (elem_free != NULL))
            elem_free(arr->elem[index]);

    /* Reset array */
    arr->size = 0;
    free(arr->elem);
    arr->elem = NULL;
}

void gpr_array_free(struct gpr_array *arr, void (*elem_free)())
{
#ifdef DEBUG
    /* Check consistency */
    if (arr == NULL)
        return;
#endif

    /* Reset array */
    gpr_array_reset(arr, elem_free);

    /* Free array */
    free(arr);
}

enum GPR_Err gpr_array_push_front(struct gpr_array *arr, void *elem)
{
    void **area = NULL;

#ifdef DEBUG
    /* Check consistency */
    if (arr == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid array");

    if (elem == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid element");
#endif

    /* (Re)allocate array */
    area = (void **)realloc(arr->elem, sizeof(void *) * (arr->size + 1));
    if (UNLIKELY(area == NULL))
        return gpr_err_raise(GPR_ERR_MEMORY_FAILURE, "Array allocation failed");

    /* Move down backside elements */
    memmove(area + 1, area, sizeof(void *) * arr->size); // Overlapping safe

    /* Push front */
    area[0] = elem;
    arr->size++;
    arr->elem = area;

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

enum GPR_Err gpr_array_push_back(struct gpr_array *arr, void *elem)
{
    void **area = NULL;

#ifdef DEBUG
    /* Check consistency */
    if (arr == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid array");

    if (elem == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid element");
#endif

    /* (Re)allocate array */
    area = (void **)realloc(arr->elem, sizeof(void *) * (arr->size + 1));
    if (UNLIKELY(area == NULL))
        return gpr_err_raise(GPR_ERR_MEMORY_FAILURE, "Array allocation failed");

    /* Push back */
    area[arr->size] = elem;
    arr->size++;
    arr->elem = area;

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

enum GPR_Err gpr_array_insert(struct gpr_array *arr, void *elem, size_t index)
{
    void **area = NULL;
    size_t back_size = 0;

#ifdef DEBUG
    /* Check consistency */
    if (arr == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid array");

    if (elem == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid element");

    if (index > gpr_array_get_size(arr))
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid index");
#endif

    if (index == 0)
        return gpr_array_push_front(arr, elem);

    if (index == gpr_array_get_size(arr))
        return gpr_array_push_back(arr, elem);

    /* (Re)allocate array */
    area = (void **)realloc(arr->elem, sizeof(void *) * (arr->size + 1));
    if (UNLIKELY(area == NULL))
        return gpr_err_raise(GPR_ERR_MEMORY_FAILURE, "Array allocation failed");

    /* Move down backside elements */
    back_size = gpr_array_get_size(arr) - index;
    if (back_size > 0)
        memmove(area + index + 1, area + index, sizeof(void *) * back_size); // Overlapping safe

    /* Insert element */
    area[index] = elem;
    arr->size++;
    arr->elem = area;

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

enum GPR_Err gpr_array_pop_front(struct gpr_array *arr, void (*elem_free)())
{
    void **area = NULL;

#ifdef DEBUG
    /* Check consistency */
    if (arr == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid array");
#endif

    if (gpr_array_is_empty(arr))
        return gpr_err_raise(GPR_ERR_KO, "Empty array");

    /* Simple case */
    if (gpr_array_get_size(arr) == 1)
    {
        gpr_array_reset(arr, elem_free);
        return gpr_err_raise(GPR_ERR_OK, NULL);
    }

    /* Free element */
    if (elem_free != NULL)
        elem_free(gpr_array_get_elem(arr, 0));
    arr->size--;

    /* Move top backside elements */
    memmove(arr->elem, arr->elem + 1, sizeof(void *) * (arr->size)); // Overlapping safe

    /* (Re)allocate array */
    area = (void **)realloc(arr->elem, sizeof(void *) * arr->size);
    if (UNLIKELY(area == NULL))
        return gpr_err_raise(GPR_ERR_MEMORY_FAILURE, "Array allocation failed");
    arr->elem = area;

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

enum GPR_Err gpr_array_pop_back(struct gpr_array *arr, void (*elem_free)())
{
    void **area = NULL;

#ifdef DEBUG
    /* Check consistency */
    if (arr == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid array");
#endif

    if (gpr_array_is_empty(arr))
        return gpr_err_raise(GPR_ERR_KO, "Empty array");

    /* Simple case */
    if (gpr_array_get_size(arr) == 1)
    {
        gpr_array_reset(arr, elem_free);
        return gpr_err_raise(GPR_ERR_OK, NULL);
    }

    /* Free element */
    if (elem_free != NULL)
        elem_free(gpr_array_get_elem(arr, gpr_array_get_size(arr) - 1));
    arr->size--;

    /* (Re)allocate array */
    area = (void **)realloc(arr->elem, sizeof(void *) * arr->size);
    if (UNLIKELY(area == NULL))
        return gpr_err_raise(GPR_ERR_MEMORY_FAILURE, "Array allocation failed");
    arr->elem = area;

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

enum GPR_Err gpr_array_remove(struct gpr_array *arr, void (*elem_free)(), size_t index)
{
    void **area = NULL;
    size_t back_size = 0;

#ifdef DEBUG
    /* Check consistency */
    if (arr == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid array");

    if (index >= gpr_array_get_size(arr))
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid index");
#endif

    if (gpr_array_is_empty(arr))
        return gpr_err_raise(GPR_ERR_KO, "Empty array");

    if (index == 0)
        return gpr_array_pop_front(arr, elem_free);

    if (index == gpr_array_get_size(arr) - 1)
        return gpr_array_pop_back(arr, elem_free);

    /* Free element */
    if (elem_free != NULL)
        elem_free(gpr_array_get_elem(arr, index));
    arr->size--;

    /* Move top backside elements */
    back_size = gpr_array_get_size(arr) - index;
    memmove(arr->elem + index, arr->elem + index + 1, sizeof(void *) * back_size); // Overlapping safe

    /* (Re)allocate array */
    area = (void **)realloc(arr->elem, sizeof(void *) * arr->size);
    if (UNLIKELY(area == NULL))
        return gpr_err_raise(GPR_ERR_MEMORY_FAILURE, "Array allocation failed");
    arr->elem = area;

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

enum GPR_Err gpr_array_replace(struct gpr_array *arr, void (*elem_free)(), void *elem, size_t index)
{
#ifdef DEBUG
    /* Check consistency */
    if (arr == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid array");

    if (elem == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid element");

    if (index >= gpr_array_get_size(arr))
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid index");
#endif

    if (gpr_array_is_empty(arr))
        return gpr_err_raise(GPR_ERR_KO, "Empty array");

    /* Free element */
    if (elem_free != NULL)
        elem_free(gpr_array_get_elem(arr, index));

    /* Replace element */
    arr->elem[index] = elem;

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

void gpr_array_map(struct gpr_array *arr, void (*elem_map)())
{
#ifdef DEBUG
    /* Check consistency */
    if ((arr == NULL) || (elem_map == NULL))
        return;
#endif

    /* Map action */
    for (size_t index = 0; index < arr->size; index++)
        elem_map(arr->elem[index]);
}

void *gpr_array_search(const struct gpr_array *arr, bool (*elem_search)(), const void *ctx, size_t *index)
{
#ifdef DEBUG
    /* Check consistency */
    if ((arr == NULL) || (elem_search == NULL))
        return NULL;
#endif

    /* Search element */
    for (size_t counter = 0; counter < gpr_array_get_size(arr); counter++)
    {
        if (elem_search(gpr_array_get_elem(arr, counter), ctx))
        {
            if (index)
                *index = counter;
            return gpr_array_get_elem(arr, counter);
        }
    }

    if (index)
        *index = 0;
    return NULL;
}

bool gpr_array_is_empty(const struct gpr_array *arr)
{
    /* Check consistency */
    if ((arr == NULL) || (arr->size == 0))
        return true;

    return false;
}

size_t gpr_array_get_size(const struct gpr_array *arr)
{
    /* Check consistency */
    if (arr == NULL)
        return 0;

    return arr->size;
}

void *gpr_array_get_elem(const struct gpr_array *arr, size_t index)
{
#ifdef DEBUG
    /* Check consistency */
    if ((arr == NULL) || (index >= gpr_array_get_size(arr)))
        return NULL;
#endif

    return arr->elem[index];
}
