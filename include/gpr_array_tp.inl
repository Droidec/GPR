/******************************************************************************
 *
 * \file gpr_array_tp.inl
 * \brief Templated array module.
 * \details
 * This module defines a way to handle generic dynamic arrays with type checking
 * and automatic initialization and freeing of elements.
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
 * \copyright Copyright (c) 2019-2021, GPR Team
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
 * * Neither the name of the GPR Team nor the names of its contributors may
 *   be used to endorse or promote products derived from this software without
 *   specific prior written permission.
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

#include <stdbool.h> // WARN: Not portable
#include <stddef.h>

/*-----------------------------------------------------------------------------
 |      Template parameters
 ----------------------------------------------------------------------------*/

/**
 * \def GPR_TP_TYPE
 * \brief Type of the elements of the array, parameter required.
 */
// #define GPR_TP_TYPE                     // Required

/**
 * \def GPR_ARR_TYPE
 * \brief Name of the type of the array, parameter required.
 */
// #define GPR_ARR_TYPE                    // Required

/**
 * \def GPR_FUNC(func)
 * \brief Nomenclature of the name of the functions, required.
 */
// #define GPR_FUNC(func) gpr_array_ ## func // Required

#if !defined(GPR_TP_TYPE) || !defined(GPR_ARR_TYPE) || !defined(GPR_FUNC)
    #error "Missing GPR_ARR_TYPE or GPR_TP_TYPE as template parameters."
#endif

/**
 * \def GPR_EL_INIT_FUNC(elem)
 * \brief Function to init an element of the array, optional.
 */
// #define GPR_EL_INIT_FUNC                // Optional

/**
 * \def GPR_EL_FREE_FUNC(elem)
 * \brief Function to free an element of the array, optional.
 */
// #define GPR_EL_FREE_FUNC                // Optional

/**
 * \def GPR_EL_MOVE_FUNC(dest,src)
 * \brief Function to move (steal contents of a structure) an element of the array, optional.
 */
#ifndef GPR_EL_MOVE_FUNC
    #define GPR_EL_MOVE_FUNC(dest,src)      memcpy(dest,src,sizeof(GPR_TP_TYPE))
#endif // GPR_EL_MOVE_FUNC
/**
 * \def GPR_REALLOC_FUNC(arr,ptr,size)
 * \brief Function to reallocate some memory, optional.
 */
#ifndef GPR_REALLOC_FUNC
    #define GPR_REALLOC_FUNC(arr,ptr,size)  realloc(ptr,size)
#endif // GPR_REALLOC_FUNC
/**
 * \def GPR_DEALLOC_FUNC(arr,ptr,size)
 * \brief Function to deallocate some memory, optional.
 */
#ifndef GPR_DEALLOC_FUNC
    #define GPR_DEALLOC_FUNC(arr,ptr)       free(ptr)
#endif // GPR_DEALLOC_FUNC

/*-----------------------------------------------------------------------------
 |      Types
 ----------------------------------------------------------------------------*/

/**
 * \brief Type defined by this template.
 */
struct GPR_ARR_TYPE
{
    size_t size;        ///< Size of the array
    size_t capacity;    ///< Size allocated / could be deleted if just a raw array is needed
    GPR_TP_TYPE *array; ///< Pointed data
};


/*-----------------------------------------------------------------------------
 |      Constructors & Destructors
 ----------------------------------------------------------------------------*/


/******************************************************************************
 *
 * \brief Initialize the structure GPR_ARR_TYPE.
 *
 * \param arr       Structure to initialize
 * \param capacity  Number of elements tp pre-allocate
 *
 * \return
 *     Success of the operation
 *
 *****************************************************************************/
static inline bool GPR_FUNC(init)(struct GPR_ARR_TYPE *arr,size_t capacity)
{
    arr->size = 0;
    arr->capacity = capacity;

    if(arr->capacity == 0)
        arr->array = NULL;
    else
    {
        arr->array = GPR_REALLOC_FUNC(arr,NULL,capacity * sizeof(GPR_TP_TYPE));
        if(arr->array == NULL)
            return false;
    }

    return true;
}

/******************************************************************************
 *
 * \brief Free the structure GPR_ARR_TYPE.
 *
 * \warning     No reinitialization of the structure is performed. 
 *
 * \param arr   Structure to free
 *
 *****************************************************************************/
static inline void GPR_FUNC(free)(struct GPR_ARR_TYPE *arr)
{
    if(arr->capacity != 0)
    {
        #ifdef GPR_EL_FREE_FUNC
            for(GPR_TP_TYPE *it = arr->array;it != arr->array + arr->size;it++)
                GPR_EL_FREE_FUNC(it);
        #endif // GPR_EL_FREE_FUNC

        GPR_DEALLOC_FUNC(arr,arr->array);
    }
}


/*-----------------------------------------------------------------------------
 |      Size and capacity functions 
 ----------------------------------------------------------------------------*/


/******************************************************************************
 *
 * \brief Get the size of the array.
 *
 * \param arr   Array
 *
 * \return
 *     Size of the array...
 *
 *****************************************************************************/
static inline size_t GPR_FUNC(size)(struct GPR_ARR_TYPE *arr)
{
    return arr->size;
}

/******************************************************************************
 *
 * \brief Get the capacity of the array.
 *
 * \param arr   Array
 *
 * \return
 *     Capacity of the array ; memory pre-allocated.
 *
 *****************************************************************************/
static inline size_t GPR_FUNC(capacity)(struct GPR_ARR_TYPE *arr)
{
    return arr->capacity;
}

/******************************************************************************
 *
 * \brief Check if the array is empty.
 *
 * \param arr   Array
 *
 * \return
 *     Is the array empty.
 *
 *****************************************************************************/
static inline bool GPR_FUNC(empty)(struct GPR_ARR_TYPE *arr)
{
    return GPR_FUNC(size)(arr) == 0;
}

/******************************************************************************
 *
 * \brief Resize the buffer of the array.
 *
 * \param arr   Array
 * \param size  New capacity
 *
 * \return
 *     Success of the operation
 *
 *****************************************************************************/
static inline bool GPR_FUNC(reserve)(struct GPR_ARR_TYPE *arr,size_t capacity)
{
    if(arr->capacity >= capacity)
        return true;

    GPR_TP_TYPE *new_ptr = GPR_REALLOC_FUNC(arr,arr->array,capacity * sizeof(GPR_TP_TYPE));
    if(new_ptr == NULL)
        return false;
    else
    {
        arr->array = new_ptr;
        arr->capacity = capacity;
        return true;
    }
}

/******************************************************************************
 *
 * \brief Resize the array, initialize new elements and free the deleted ones.
 *
 * \param arr   Array
 * \param size  New size
 *
 * \return
 *     Success of the operation
 *
 *****************************************************************************/
static inline bool GPR_FUNC(resize)(struct GPR_ARR_TYPE *arr,size_t size)
{
    if(arr->capacity < size)
    {
        if(!GPR_FUNC(reserve)(arr,size))
            return false;
    }

    #ifdef GPR_EL_INIT_FUNC
        for(GPR_TP_TYPE *it = arr->array + arr->size;it != arr->array + size;it++)
        {
            if(!GPR_EL_INIT_FUNC(it))
            {
                it--;
                for(;it != arr->array + arr->size;it--)
                    GPR_EL_FREE_FUNC(it);

                return false;
            }
        }
    #endif // GPR_EL_INIT_FUNC

    #ifdef GPR_EL_FREE_FUNC
        for(GPR_TP_TYPE *it = arr->array + size;it != arr->array + arr->size;it++)
            GPR_EL_FREE_FUNC(it);
    #endif // GPR_EL_FREE_FUNC

    arr->size = size;
    return true;
}

/******************************************************************************
 *
 * \brief Resize the buffer of the array to match the size.
 *
 * \param arr   Array
 *
 * \return
 *     Success of the operation
 *
 *****************************************************************************/
static inline bool GPR_FUNC(shrink_to_fit)(struct GPR_ARR_TYPE *arr)
{
    if(arr->size == arr->capacity)
        return true;

    GPR_TP_TYPE *new_ptr = GPR_REALLOC_FUNC(arr,arr->array,arr->size);
    if(new_ptr == NULL)
        return false;
    else
    {
        arr->array = new_ptr;
        return true;
    }
}


/*-----------------------------------------------------------------------------
 |      Accessing functions
 ----------------------------------------------------------------------------*/


/******************************************************************************
 *
 * \brief Get the element at position 'id' on the array.
 *
 * \warning     No check on the validity of the position requested.
 *
 * \param arr   Array
 * \param id    Position requested
 *
 * \return
 *     "Iterator" of the position ; pointer to the element requested
 *
 *****************************************************************************/
static inline GPR_TP_TYPE* GPR_FUNC(get)(struct GPR_ARR_TYPE *arr,size_t id)
{
    return arr->array + id;
}

/******************************************************************************
 *
 * \brief Get the element at position 'id' on the array.
 *
 * \note        Validity of the position is checked.
 *
 * \param arr   Array
 * \param id    Position requested
 *
 * \return
 *     "Iterator" of the position ; pointer to the element requested
 *
 *****************************************************************************/
static inline GPR_TP_TYPE* GPR_FUNC(at)(struct GPR_ARR_TYPE *arr,size_t id)
{
    if(id < arr->size)
        return NULL;

    return GPR_FUNC(get)(arr,id);
}

/******************************************************************************
 *
 * \brief Get the element at front of the array.
 *
 * \param arr   Array
 *
 * \return
 *     "Iterator" of the front ; pointer to the first element
 *
 *****************************************************************************/
static inline GPR_TP_TYPE* GPR_FUNC(front)(struct GPR_ARR_TYPE *arr)
{
    return GPR_FUNC(get)(arr,0);
}

/******************************************************************************
 *
 * \brief Get the element at back of the array.
 *
 * \param arr   Array
 *
 * \return
 *     "Iterator" of the back ; pointer to the last element
 *
 *****************************************************************************/
static inline GPR_TP_TYPE* GPR_FUNC(back)(struct GPR_ARR_TYPE *arr)
{
    return GPR_FUNC(get)(arr,arr->size - 1);
}

/******************************************************************************
 *
 * \brief Get the iterator of the begin of the array.
 *
 * \param arr   Array
 *
 * \return
 *     "Iterator" of the front ; pointer to the last element
 *
 *****************************************************************************/
static inline GPR_TP_TYPE* GPR_FUNC(begin)(struct GPR_ARR_TYPE *arr)
{
    return arr->array;
}

/******************************************************************************
 *
 * \brief Get the iterator of the end of the array.
 *
 * \param arr   Array
 *
 * \return
 *     "Iterator" of the end ; pointer to the last element past one
 *
 *****************************************************************************/
static inline GPR_TP_TYPE* GPR_FUNC(end)(struct GPR_ARR_TYPE *arr)
{
    return arr->array + arr->size;
}


/*-----------------------------------------------------------------------------
 |      Modification functions
 ----------------------------------------------------------------------------*/


/******************************************************************************
 *
 * \brief Clear the array and call free function on all elements.
 *
 * \param arr   Array
 *
 *****************************************************************************/
static inline void GPR_FUNC(clear)(struct GPR_ARR_TYPE* arr)
{
    GPR_FUNC(resize)(arr,0);
}

/******************************************************************************
 *
 * \brief Swap the both arrays.
 *
 * \param arr   Array1
 * \param arr   Array2
 *
 *****************************************************************************/
static inline void GPR_FUNC(swap)(struct GPR_ARR_TYPE* a,struct GPR_ARR_TYPE* b)
{
    struct GPR_ARR_TYPE t = *a;
    *a = *b;
    *b = t;
}

/******************************************************************************
 *
 * \brief Insert an element before an element of the array.
 *
 * \note Always succeed if capacity < size
 *
 * \param arr   Array
 * \param pos   Element on the array where to insert (before)
 * \param elem  Element to insert
 *
 * \return
 *     Success of the operation
 *
 *****************************************************************************/
static inline bool GPR_FUNC(insert)(struct GPR_ARR_TYPE *arr,GPR_TP_TYPE *pos,GPR_TP_TYPE *elem)
{
    if(arr->size == arr->capacity)
    {
        GPR_TP_TYPE *old_array = arr->array;
        if(!GPR_FUNC(reserve)(arr,arr->size * 3 / 2 + 1)) // TODO : Could be parametrized
            return false;
        pos = arr->array + (pos - old_array);
    }

    for(GPR_TP_TYPE *it = arr->array + arr->size;it != pos;it--)
        GPR_EL_MOVE_FUNC(it,it-1);
    arr->size++;

    GPR_EL_MOVE_FUNC(pos,elem);
    return true;
}

/******************************************************************************
 *
 * \brief Delete the element at the position pos of the array (and free if needed).
 *
 * \param arr   Array
 * \param pos   Position of the array to delete
 *
 *****************************************************************************/
static inline void GPR_FUNC(erase)(struct GPR_ARR_TYPE *arr,GPR_TP_TYPE *pos)
{
    #ifdef GPR_EL_FREE_FUNC 
        GPR_EL_FREE_FUNC(pos);
    #endif // GPR_EL_FREE_FUNC

    arr->size--;
    for(GPR_TP_TYPE *it = pos;it != arr->array + arr->size;it++)
        GPR_EL_MOVE_FUNC(it,it+1);
}

/******************************************************************************
 *
 * \brief Insert an element to the end of the array.
 *
 * \note Always succeed if capacity < size
 *
 * \param arr   Array
 * \param elem  Element to insert
 *
 * \return
 *     Success of the operation
 *
 *****************************************************************************/
static inline bool GPR_FUNC(push_back)(struct GPR_ARR_TYPE* arr,GPR_TP_TYPE *elem)
{
    return GPR_FUNC(insert)(arr,arr->array + arr->size,elem);
}

/******************************************************************************
 *
 * \brief Delete the element at the end of the array (and free if needed).
 *
 * \param arr   Array
 *
 *****************************************************************************/
static inline void GPR_FUNC(pop_back)(struct GPR_ARR_TYPE* arr)
{
    GPR_FUNC(erase)(arr,arr->array + arr->size-1);
}

/******************************************************************************
 *
 * \brief Apply a function to all elements
 *
 * \param arr   Array
 * \param pos   Function to apply to the elements
 *
 *****************************************************************************/
static inline void GPR_FUNC(map)(struct GPR_ARR_TYPE *arr,void (*func)(GPR_TP_TYPE*))
{
    for(GPR_TP_TYPE *it = GPR_FUNC(begin)(arr);it != GPR_FUNC(end)(arr);it++)
        func(it);
}


/*-----------------------------------------------------------------------------
 |      Undefine template parameters
 ----------------------------------------------------------------------------*/


#undef GPR_TP_TYPE
#undef GPR_ARR_TYPE
#undef GPR_FUNC

#ifdef GPR_EL_INIT_FUNC
    #undef GPR_EL_INIT_FUNC
#endif // GPR_EL_INIT_FUNC
#ifdef GPR_EL_FREE_FUNC
    #undef GPR_EL_FREE_FUNC
#endif // GPR_EL_FREE_FUNC
#undef GPR_EL_MOVE_FUNC

#undef GPR_REALLOC_FUNC
#undef GPR_DEALLOC_FUNC
