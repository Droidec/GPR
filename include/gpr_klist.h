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

#include <stdbool.h> // bool
#include <stddef.h>  // offsetof

#include "gpr_err.h"

/**
 * \brief Circular doubly linked list structure (Entry)
 */
struct gpr_klist
{
    struct gpr_klist *prev; ///< Previous element
    struct gpr_klist *next; ///< Next element
};

/**
 * \brief Casts a member of a structure out to the containing structure
 *
 * \note This is where black magic happens!
 *
 * \param[in] ptr    Pointer to the member
 * \param[in] type   Type of the container structure this is embedded in
 * \param[in] member Name of the member within this structure
 */
// clang-format off
#define GPR_CONTAINER_OF(ptr, type, member)                \
    ({                                                     \
        const typeof(((type *)0)->member) *__mptr = (ptr); \
        (type *)((char *)__mptr - offsetof(type, member)); \
    })
// clang-format on

/**
 * \brief Initializes a circular doubly linked list (at runtime)
 */
#define GPR_KLIST_INIT(name) \
    {                        \
        &(name), &(name)     \
    }

/**
 * \brief Initializes a circular doubly linked list (at compile time)
 *
 * \param list Circular doubly linked list to initialize
 */
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

/**
 * \brief Adds a new entry according to previous and next entry
 *
 * \warning This is only for internal list manipulation. Don't use it directly!
 *
 * \param[in] new  New entry to be added
 * \param[in] prev Previous entry of the new one
 * \param[in] next Next entry of the new one
 *
 * \retval #GPR_ERR_OK The entry has correctly been added
 * \retval #GPR_ERR_INVALID_PARAMETER The new entry is \c NULL, the previous entry is \c NULL
 *         or the next entry is \c NULL (for \e DEBUG mode only)
 */
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

/**
 * \brief Deletes an entry by making the \p prev and \p next entries point to each other
 *
 * \warning This is only for internal list manipulation. Don't use it directly!
 *
 * \param[in] prev Previous entry to join
 * \param[in] next Next entry to join
 *
 * \retval #GPR_ERR_OK The entry has correctly been deleted
 * \retval #GPR_ERR_INVALID_PARAMETER The previous entry is \c NULL or the next entry is \c NULL
 *         (for \e DEBUG mode only)
 */
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

/**
 * \brief Adds a new entry after the specified head
 *
 * \note Useful to implement stacks (*LIFO*)
 *
 * \param[in] new  New entry to be added
 * \param[in] head List head to add it after
 *
 * \retval #GPR_ERR_OK The entry has correctly been added
 * \retval #GPR_ERR_INVALID_PARAMETER The new entry is \c NULL or the list entry is \c NULL
 *         (for \e DEBUG mode only)
 */
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

/**
 * \brief Adds a new entry before the specified head
 *
 * \note Useful to implement queues (*FIFO*)
 *
 * \param[in] new  New entry to be added
 * \param[in] head List head to add it before
 *
 * \retval #GPR_ERR_OK The entry has correctly been added
 * \retval #GPR_ERR_INVALID_PARAMETER The new entry is \c NULL or the list entry is \c NULL
 *         (for \e DEBUG mode only)
 */
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

/**
 * \brief Deletes an entry from a list
 *
 * \warning The deleted entry will not be freed (*DIY*)
 *
 * \param[in] entry Entry to delete
 *
 * \retval #GPR_ERR_OK The entry has correctly been deleted
 * \retval #GPR_ERR_INVALID_PARAMETER The entry is \c NULL (for \e DEBUG mode only)
 */
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

/**
 * \brief Checks if list is empty
 *
 * \param[in] head List to check
 *
 * \return Returns \c true if the list is empty or not allocated, \c false otherwise
 */
static inline bool gpr_klist_is_empty(const struct gpr_klist *head)
{
#ifdef DEBUG
    /* Check consistency */
    if (head == NULL)
        return true;
#endif

    return head->next == head;
}

/**
 * \brief Gets the containing structure for this entry
 *
 * \param[in] ptr    Entry in the containing structure
 * \param[in] type   Type of the containing structure
 * \param[in] member Name of the GPR list within containing structure
 */
#define GPR_KLIST_ENTRY(ptr, type, member) GPR_CONTAINER_OF(ptr, type, member)

/**
 * \brief Gets the first element from a list
 *
 * \param[in] ptr    List head to take the element from
 * \param[in] type   Type of the containing structure
 * \param[in] member Name of the GPR list within containing structure
 */
#define GPR_KLIST_FIRST_ENTRY(ptr, type, member) GPR_KLIST_ENTRY((ptr)->next, type, member)

/**
 * \brief Gets the last element from a list
 *
 * \param[in] ptr    List head to take the element from
 * \param[in] type   Type of the containing structure
 * \param[in] member Name of the GPR list within containing structure
 */
#define GPR_KLIST_LAST_ENTRY(ptr, type, member) GPR_KLIST_ENTRY((ptr)->prev, type, member)

/**
 * \brief Gets the next element in a list
 *
 * \param[out] pos    Structure pointer to use as a loop cursor
 * \param[in]  member Name of the GPR list within containing structure
 */
#define GPR_KLIST_NEXT_ENTRY(pos, member) GPR_KLIST_ENTRY((pos)->member.next, typeof(*(pos)), member)

/**
 * \brief Gets the previous element in a list
 *
 * \param[out] pos    Structure pointer to use as a loop cursor
 * \param[in]  member Name of the GPR list within containing structure
 */
#define GPR_KLIST_PREV_ENTRY(pos, member) GPR_KLIST_ENTRY((pos)->member.prev, typeof(*(pos)), member)

/**
 * \brief Iterates over a list
 *
 * \param[out] pos  Entry pointer to use as a loop cursor
 * \param[in]  head Head of the list
 */
#define GPR_KLIST_FOR_EACH(pos, head) for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * \brief Iterates backwards over a list
 *
 * \param[out] pos  Entry pointer to use as a loop cursor
 * \param[in]  head Head of the list
 */
#define GPR_KLIST_FOR_EACH_REVERSE(pos, head) for (pos = (head)->prev; pos != (head); pos = pos->prev)

/**
 * \brief Tests if the entry points to the head of the list
 *
 * \param[in] pos    Structure pointer containing entry to test
 * \param[in] head   Head of the list
 * \param[in] member Name of the GPR list within containing structure
 */
#define GPR_KLIST_ENTRY_IS_HEAD(pos, head, member) (&pos->member == (head))

/**
 * \brief Iterates over a list of a given type
 *
 * \param[out] pos    Structure pointer to use as a loop cursor
 * \param[in]  head   Head of the list
 * \param[in]  member Name of the GPR list within containg structure
 */
#define GPR_KLIST_FOR_EACH_ENTRY(pos, head, member)                                                            \
    for (pos = GPR_KLIST_FIRST_ENTRY(head, typeof(*pos), member); !GPR_KLIST_ENTRY_IS_HEAD(pos, head, member); \
         pos = GPR_KLIST_NEXT_ENTRY(pos, member))

/**
 * \brief Iterates over a list of a given type, safe against removal of list entry
 *
 * \param[out] pos    Structure pointer to use as a loop cursor
 * \param[out] tmp    Structure pointer to use as a temporary storage
 * \param[in]  head   Head of the list
 * \param[in]  member Name of the GPR list within containg structure
 */
#define GPR_KLIST_FOR_EACH_ENTRY_SAFE(pos, tmp, head, member)                                              \
    for (pos = GPR_KLIST_FIRST_ENTRY(head, typeof(*pos), member), tmp = GPR_KLIST_NEXT_ENTRY(pos, member); \
         !GPR_KLIST_ENTRY_IS_HEAD(pos, head, member); pos = tmp, tmp = GPR_KLIST_NEXT_ENTRY(pos, member))

/**
 * \brief Iterates backwards over a list of a given type
 *
 * \param[out] pos    Structure pointer to use as a loop cursor
 * \param[in]  head   Head of the list
 * \param[in]  member Name of the GPR list within containg structure
 */
#define GPR_KLIST_FOR_EACH_ENTRY_REVERSE(pos, head, member)                                                   \
    for (pos = GPR_KLIST_LAST_ENTRY(head, typeof(*pos), member); !GPR_KLIST_ENTRY_IS_HEAD(pos, head, member); \
         pos = GPR_KLIST_PREV_ENTRY(pos, member))

/**
 * \brief Iterates backwards over a list of a given type, safe against removal of list entry
 *
 * \param[out] pos    Structure pointer to use as a loop cursor
 * \param[out] tmp    Structure pointer to use as a temporary storage
 * \param[in]  head   Head of the list
 * \param[in]  member Name of the GPR list within containg structure
 */
#define GPR_KLIST_FOR_EACH_ENTRY_REVERSE_SAFE(pos, tmp, head, member)                                     \
    for (pos = GPR_KLIST_LAST_ENTRY(head, typeof(*pos), member), tmp = GPR_KLIST_PREV_ENTRY(pos, member); \
         !GPR_KLIST_ENTRY_IS_HEAD(pos, head, member); pos = tmp, tmp = GPR_KLIST_PREV_ENTRY(pos, member))

#endif /* H_GPR_KLIST */
