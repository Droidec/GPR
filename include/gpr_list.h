/******************************************************************************
 *
 * \file gpr_list.h
 * \brief Circular doubly linked list
 * \details
 * This module defines a generic circular doubly linked list inspired by the
 * Linux Kernel
 *
 * // TODO : Actual implementation in not thread safe
 * // TODO : Check if this implementation is platform specific
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

#ifndef H_GPR_LIST
#define H_GPR_LIST

#include "gpr_err.h"
#include "gpr_builtin.h"

/**
 * \brief GPR circular doubly linked list structure (Entry)
 */
struct gpr_list
{
    struct gpr_list *prev; ///< Previous element
    struct gpr_list *next; ///< Next element
};

/**
 * \brief Initialize a GPR circular doubly linked list (at runtime)
 */
#define GPR_LIST_INIT(name) \
    {                       \
        &(name), &(name)    \
    }

/******************************************************************************
 *
 * \brief Initialize a GPR circular doubly linked list (at compile time)
 *
 * \param list GPR circular doubly linked list to initialize
 *
 *****************************************************************************/
void gpr_list_init(struct gpr_list *list);

/******************************************************************************
 *
 * \brief Add a new entry after the specified head
 *
 * \note Useful to implement stacks (LIFO)
 *
 * \param new  New entry to be added
 * \param head List head to add it after
 *
 * \return
 *     GPR_ERR_OK: The entry has correctly been added\n
 *     GPR_ERR_INVALID_PARAMETER: The new entry is NULL or the list entry is
 *     NULL (DEBUG mode only)\n
 *
 *****************************************************************************/
enum GPR_Err gpr_list_push_front(struct gpr_list *new, struct gpr_list *head);

/******************************************************************************
 *
 * \brief Add a new entry before the specified head
 *
 * \note Useful to implement queues (FIFO)
 *
 * \param new  New entry to be added
 * \param head List head to add it before
 *
 * \return
 *     GPR_ERR_OK: The entry has correctly been added\n
 *     GPR_ERR_INVALID_PARAMETER: The new entry is NULL or the list entry is
 *     NULL (DEBUG mode only)\n
 *
 *****************************************************************************/
enum GPR_Err gpr_list_push_back(struct gpr_list *new, struct gpr_list *head);

/******************************************************************************
 *
 * \brief Remove an entry from the list
 *
 * \note Useful to implement queues
 *
 * \param new  New entry to be added
 * \param head List head to add it before
 *
 * \return
 *     GPR_ERR_OK: The entry has correctly been added\n
 *     GPR_ERR_INVALID_PARAMETER: The new entry is NULL or the list entry is
 *     NULL (DEBUG mode only)\n
 *
 *****************************************************************************/
enum GPR_Err gpr_list_remove(struct gpr_list *entry);

/**
 * \brief Get the containing structure for this entry
 *
 * \param ptr    Entry in the containing structure
 * \param type   Type of the containing structure
 * \param member Name of the GPR list within containing structure
 */
#define GPR_LIST_ENTRY(ptr, type, member) CONTAINER_OF(ptr, type, member)

/**
 * \brief Get the first element from a list
 *
 * \param ptr    List head to take the element from
 * \param type   Type of the containing structure
 * \param member Name of the GPR list within containing structure
 */
#define GPR_LIST_FIRST_ENTRY(ptr, type, member) GPR_LIST_ENTRY((ptr)->next, type, member)

/**
 * \brief Get the last element from a list
 *
 * \param ptr    List head to take the element from
 * \param type   Type of the containing structure
 * \param member Name of the GPR list within containing structure
 */
#define GPR_LIST_LAST_ENTRY(ptr, type, member) GPR_LIST_ENTRY((ptr)->prev, type, member)

/**
 * \brief Get the next element in a list
 *
 * \param pos    Structure pointer to use as a loop cursor
 * \param member Name of the GPR list within containing structure
 */
#define GPR_LIST_NEXT_ENTRY(pos, member) GPR_LIST_ENTRY((pos)->member.next, typeof(*(pos)), member)

/**
 * \brief Get the previous element in a list
 *
 * \param pos    Structure pointer to use as a loop cursor
 * \param member Name of the GPR list within containing structure
 */
#define GPR_LIST_PREV_ENTRY(pos, member) GPR_LIST_ENTRY((pos)->member.prev, typeof(*(pos)), member)

/**
 * \brief Iterate over a list
 *
 * \param pos  Entry pointer to use as a loop cursor
 * \param head Head of the list
 */
#define GPR_LIST_FOR_EACH(pos, head) for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * \brief Test if the entry points to the head of the list
 *
 * \param pos    Structure pointer containing entry to test
 * \param head   Head of the list
 * \param member Name of the GPR list within containing structure
 */
#define GPR_LIST_ENTRY_IS_HEAD(pos, head, member) (&pos->member == (head))

/**
 * \brief Iterate over a list of a given type
 *
 * \param pos    Structure pointer to use as a loop cursor
 * \param head   Head of the list
 * \param member Name of the GPR list within containg structure
 */
#define GPR_LIST_FOR_EACH_ENTRY(pos, head, member)                                                           \
    for (pos = GPR_LIST_FIRST_ENTRY(head, typeof(*pos), member); !GPR_LIST_ENTRY_IS_HEAD(pos, head, member); \
         pos = GPR_LIST_NEXT_ENTRY(pos, member))

/**
 * \brief Iterate backwards over a list of a given type
 *
 * \param pos    Structure pointer to use as a loop cursor
 * \param head   Head of the list
 * \param member Name of the GPR list within containg structure
 */
#define GPR_LIST_FOR_EACH_ENTRY_REVERSE(pos, head, member)                                                  \
    for (pos = GPR_LIST_LAST_ENTRY(head, typeof(*pos), member); !GPR_LIST_ENTRY_IS_HEAD(pos, head, member); \
         pos = GPR_LIST_PREV_ENTRY(pos, member))

#endif /* H_GPR_LIST */
