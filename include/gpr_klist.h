/******************************************************************************
 *
 * \file gpr_klist.h
 * \brief Linux Kernel linked list module
 * \details
 * This module defines a generic circular doubly linked list inspired by
 * the Linux Kernel\n
 * To use such a list, one need to declare the list inside of the structure
 * to chain.\n
 * Entries can then be chained together and accessed with a little trick hide
 * behind some macros.
 *
 * \verbatim
 *
 *    ┌──────────────────────────────────────────────┐
 *    │              ┌────────┐  ┌────────┐          │
 *    │              │ Entry  │  │ Entry  │          │
 *    │              │        │  │        │          │
 *    │              │        │  │        │          │
 *    │              │        │  │        │          │
 *    │   ┌──────┐   │┌──────┐│  │┌──────┐│          │
 *    ├───> list <────> head <────> head <──── [...] ┤
 *    │   └──────┘   │└──────┘│  │└──────┘│          │
 *    │              │        │  │        │          │
 *    │              │        │  │        │          │
 *    │              └────────┘  └────────┘          │
 *    └──────────────────────────────────────────────┘
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

#ifndef H_GPR_KLIST
#define H_GPR_KLIST

#include "gpr_err.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h> // WARN: Not portable

/**
 * \brief GPR circular doubly linked list structure (Entry)
 */
struct gpr_klist
{
    struct gpr_klist *prev; ///< Previous element
    struct gpr_klist *next; ///< Next element
};

/**
 * \brief Cast a member of a structure out to the containing structure
 *
 * \note This is where black magic happens!
 *
 * \param ptr    Pointer to the member
 * \param type   Type of the container structure this is embedded in
 * \param member Name of the member within this structure
 */
// clang-format off
#define CONTAINER_OF(ptr, type, member)                    \
    ({                                                     \
        const typeof(((type *)0)->member) *__mptr = (ptr); \
        (type *)((char *)__mptr - offsetof(type, member)); \
    })
// clang-format on

/**
 * \brief Initialize a circular doubly linked list (at runtime)
 */
#define GPR_KLIST_INIT(name) \
    {                        \
        &(name), &(name)     \
    }

/******************************************************************************
 *
 * \brief Initialize a circular doubly linked list (at compile time)
 *
 * \param list circular doubly linked list to initialize
 *
 *****************************************************************************/
static inline void gpr_klist_init(struct gpr_klist *list)
{
#ifdef DEBUG
    /* Check consistency */
    if (list == NULL)
        return;
#endif

    /* Initialize list */
    list->prev = list;
    list->next = list;
}

/******************************************************************************
 *
 * \brief Add a new entry according to previous and next entry
 *
 * \note This is only only for internal list manipulation. Don't use it
 * directly
 *
 * \param new  New entry to be added
 * \param prev Previous entry of the new one
 * \param next Next entry of the new one
 *
 * \return
 *     GPR_ERR_OK: The entry has correctly been added\n
 *     GPR_ERR_INVALID_PARAMETER: The new entry is NULL, the previous entry is
 *     NULL or the next entry is NULL (DEBUG mode only)
 *
 *****************************************************************************/
static inline enum GPR_Err __list_add_entry(struct gpr_klist *new, struct gpr_klist *prev, struct gpr_klist *next)
{
#ifdef DEBUG
    /* Check consistency */
    if (new == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid new entry");

    if (prev == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid previous entry");

    if (next == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid next entry");
#endif

    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

/******************************************************************************
 *
 * \brief Delete an entry by making the prev/next entries point to each other
 *
 * \note This is only only for internal list manipulation. Don't use it
 * directly
 *
 * \param prev Previous entry to join
 * \param next Next entry to join
 *
 * \return
 *     GPR_ERR_OK: The entry has correctly been deleted\n
 *     GPR_ERR_INVALID_PARAMETER: The previous entry is NULL or the next entry
 *     is NULL (DEBUG mode only)
 *
 *****************************************************************************/
static inline enum GPR_Err __list_del_entry(struct gpr_klist *prev, struct gpr_klist *next)
{
#ifdef DEBUG
    /* Check consistency */
    if (prev == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid previous entry");

    if (next == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid next entry");
#endif

    next->prev = prev;
    prev->next = next;

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

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
 *     NULL (DEBUG mode only)
 *
 *****************************************************************************/
static inline enum GPR_Err gpr_klist_push_front(struct gpr_klist *new, struct gpr_klist *head)
{
#ifdef DEBUG
    /* Check consistency */
    if (new == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid new entry");

    if (head == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid list head");
#endif

    /* Add entry */
    return __list_add_entry(new, head, head->next);
}

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
 *     NULL (DEBUG mode only)
 *
 *****************************************************************************/
static inline enum GPR_Err gpr_klist_push_back(struct gpr_klist *new, struct gpr_klist *head)
{
#ifdef DEBUG
    /* Check consistency */
    if (new == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid new entry");

    if (head == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid list head");
#endif

    /* Add entry */
    return __list_add_entry(new, head->prev, head);
}

/******************************************************************************
 *
 * \brief Delete an entry from a list
 *
 * \note The deleted entry will not be freed (DIY)
 *
 * \param entry Entry to delete
 *
 * \return
 *     GPR_ERR_OK: The entry has correctly been deleted\n
 *     GPR_ERR_INVALID_PARAMETER: The entry is NULL (DEBUG mode only)
 *
 *****************************************************************************/
static inline enum GPR_Err gpr_klist_delete(struct gpr_klist *entry)
{
#ifdef DEBUG
    /* Check consistency */
    if (entry == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid entry");
#endif

    /* Delete entry */
    __list_del_entry(entry->prev, entry->next);
    entry->prev = entry;
    entry->next = entry;

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

/******************************************************************************
 *
 * \brief Check if list is empty
 *
 * \param head List to check
 *
 * \return
 *     True if the list is empty or not allocated
 *     False otherwise
 *
 *****************************************************************************/
static inline bool gpr_klist_is_empty(const struct gpr_klist *head)
{
#ifdef DEBUG
    /* Check consistency */
    if (entry == NULL)
        return true;
#endif

    return head->next == head;
}

/**
 * \brief Get the containing structure for this entry
 *
 * \param ptr    Entry in the containing structure
 * \param type   Type of the containing structure
 * \param member Name of the GPR list within containing structure
 */
#define GPR_KLIST_ENTRY(ptr, type, member) CONTAINER_OF(ptr, type, member)

/**
 * \brief Get the first element from a list
 *
 * \param ptr    List head to take the element from
 * \param type   Type of the containing structure
 * \param member Name of the GPR list within containing structure
 */
#define GPR_KLIST_FIRST_ENTRY(ptr, type, member) GPR_KLIST_ENTRY((ptr)->next, type, member)

/**
 * \brief Get the last element from a list
 *
 * \param ptr    List head to take the element from
 * \param type   Type of the containing structure
 * \param member Name of the GPR list within containing structure
 */
#define GPR_KLIST_LAST_ENTRY(ptr, type, member) GPR_KLIST_ENTRY((ptr)->prev, type, member)

/**
 * \brief Get the next element in a list
 *
 * \param pos    Structure pointer to use as a loop cursor
 * \param member Name of the GPR list within containing structure
 */
#define GPR_KLIST_NEXT_ENTRY(pos, member) GPR_KLIST_ENTRY((pos)->member.next, typeof(*(pos)), member)

/**
 * \brief Get the previous element in a list
 *
 * \param pos    Structure pointer to use as a loop cursor
 * \param member Name of the GPR list within containing structure
 */
#define GPR_KLIST_PREV_ENTRY(pos, member) GPR_KLIST_ENTRY((pos)->member.prev, typeof(*(pos)), member)

/**
 * \brief Iterate over a list
 *
 * \param pos  Entry pointer to use as a loop cursor
 * \param head Head of the list
 */
#define GPR_KLIST_FOR_EACH(pos, head) for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * \brief Iterate backwards over a list
 *
 * \param pos  Entry pointer to use as a loop cursor
 * \param head Head of the list
 */
#define GPR_KLIST_FOR_EACH_REVERSE(pos, head) for (pos = (head)->prev; pos != (head); pos = pos->prev)

/**
 * \brief Test if the entry points to the head of the list
 *
 * \param pos    Structure pointer containing entry to test
 * \param head   Head of the list
 * \param member Name of the GPR list within containing structure
 */
#define GPR_KLIST_ENTRY_IS_HEAD(pos, head, member) (&pos->member == (head))

/**
 * \brief Iterate over a list of a given type
 *
 * \param pos    Structure pointer to use as a loop cursor
 * \param head   Head of the list
 * \param member Name of the GPR list within containg structure
 */
#define GPR_KLIST_FOR_EACH_ENTRY(pos, head, member)                                                            \
    for (pos = GPR_KLIST_FIRST_ENTRY(head, typeof(*pos), member); !GPR_KLIST_ENTRY_IS_HEAD(pos, head, member); \
         pos = GPR_KLIST_NEXT_ENTRY(pos, member))

/**
 * \brief Iterate over a list of a given type, safe against removal of list entry
 *
 * \param pos    Structure pointer to use as a loop cursor
 * \param tmp    Structure pointer to use as a temporary storage
 * \param head   Head of the list
 * \param member Name of the GPR list within containg structure
 */
#define GPR_KLIST_FOR_EACH_ENTRY_SAFE(pos, tmp, head, member)                                              \
    for (pos = GPR_KLIST_FIRST_ENTRY(head, typeof(*pos), member), tmp = GPR_KLIST_NEXT_ENTRY(pos, member); \
         !GPR_KLIST_ENTRY_IS_HEAD(pos, head, member); pos = tmp, tmp = GPR_KLIST_NEXT_ENTRY(pos, member))

/**
 * \brief Iterate backwards over a list of a given type
 *
 * \param pos    Structure pointer to use as a loop cursor
 * \param head   Head of the list
 * \param member Name of the GPR list within containg structure
 */
#define GPR_KLIST_FOR_EACH_ENTRY_REVERSE(pos, head, member)                                                   \
    for (pos = GPR_KLIST_LAST_ENTRY(head, typeof(*pos), member); !GPR_KLIST_ENTRY_IS_HEAD(pos, head, member); \
         pos = GPR_KLIST_PREV_ENTRY(pos, member))

/**
 * \brief Iterate backwards over a list of a given type, safe against removal of list entry
 *
 * \param pos    Structure pointer to use as a loop cursor
 * \param tmp    Structure pointer to use as a temporary storage
 * \param head   Head of the list
 * \param member Name of the GPR list within containg structure
 */
#define GPR_KLIST_FOR_EACH_ENTRY_REVERSE_SAFE(pos, tmp, head, member)                                     \
    for (pos = GPR_KLIST_LAST_ENTRY(head, typeof(*pos), member), tmp = GPR_KLIST_PREV_ENTRY(pos, member); \
         !GPR_KLIST_ENTRY_IS_HEAD(pos, head, member); pos = tmp, tmp = GPR_KLIST_PREV_ENTRY(pos, member))

#endif /* H_GPR_KLIST */
