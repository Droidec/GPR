/******************************************************************************
 *
 * \file gpr_dlklist.h
 * \brief Doubly linked list module
 * \details
 * This module defines a generic doubly linked list composed of nodes\n
 * The list keeps a pointer on the first node and the last node\n
 * Each node has a pointer to the previous one and the next one\n
 * Data can be accessed through each node
 *
 * \verbatim
 *
 *        ┌───────────────────┐
 *        │ head size(2) tail │
 *        └──┼────────────┼───┘
 *        ┌──┼───┐     ┌──┼───┐
 * NULL <─> node <─────> node <─> NULL
 *        └──┼───┘     └──┼───┘
 *           v            v
 *          data         data
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

#ifndef H_GPR_DLKLIST
#define H_GPR_DLKLIST

#include <stdbool.h> // bool
#include <stddef.h>  // size_t

#include "gpr_err.h"

/**
 * \brief Doubly linked list component structure
 */
struct gpr_dlknode
{
    void *data;               ///< Pointed data (entry)
    struct gpr_dlknode *prev; ///< Previous node
    struct gpr_dlknode *next; ///< Next node
};

/**
 * \brief Doubly linked list structure
 */
struct gpr_dlklist
{
    size_t size;              ///< Size of the list (Number of nodes)
    struct gpr_dlknode *head; ///< Head of the list (First node)
    struct gpr_dlknode *tail; ///< Tail of the list (Last node)
};

/**
 * \brief Allocates and initializes a new doubly linked list
 *
 * \return Returns a pointer to the doubly linked list or \c NULL on allocation failure
 */
struct gpr_dlklist *gpr_dlklist_new(void);

/**
 * \brief Initializes a doubly linked list
 *
 * \param[out] list Doubly linked list to initialize
 */
void gpr_dlklist_init(struct gpr_dlklist *list);

/**
 * \brief Resets a doubly linked list by freeing all its nodes
 *
 * \note The callback function must deal with a \c void* [*node data*]
 *
 * \param[out] list      Doubly linked list where to free nodes
 * \param[in]  data_free Callback function that will be called to free data of
 *                       each node or \c NULL
 */
void gpr_dlklist_reset(struct gpr_dlklist *list, void (*data_free)());

/**
 * \brief Frees a doubly linked list and all its nodes
 *
 * \note The callback function must deal with a \c void* [*node data*]
 * \note Call this function if you allocated a list with #gpr_dlklist_new
 *
 * \param[out] list      Doubly linked list to free
 * \param[in]  data_free Callback function that will be called to free data of
 *                       each node or \c NULL
 */
void gpr_dlklist_free(struct gpr_dlklist *list, void (*data_free)());

/**
 * \brief Adds a new node at the beginning of a doubly linked list
 *
 * \param[out] list Doubly linked list where to add a new node at the beginning
 * \param[in]  data Data to point to for the new node
 *
 * \retval #GPR_ERR_OK The node has been added to the beginning of the list
 * \retval #GPR_ERR_INVALID_PARAMETER The list is \c NULL or the data is \c NULL
 *         (for \e DEBUG mode only)
 * \retval #GPR_ERR_MEMORY_FAILURE The new node allocation failed
 */
enum GPR_Err gpr_dlklist_push_front(struct gpr_dlklist *list, void *data);

/**
 * \brief Adds a new node at the end of a doubly linked list
 *
 * \param[out] list Doubly linked list where to add a new node at the end
 * \param[in]  data Data to point to for the new node
 *
 * \retval #GPR_ERR_OK The node has been added to the end of the list
 * \retval #GPR_ERR_INVALID_PARAMETER The list is \c NULL or the data is \c NULL
 *         (for \e DEBUG mode only)
 * \retval #GPR_ERR_MEMORY_FAILURE The new node allocation failed
 */
enum GPR_Err gpr_dlklist_push_back(struct gpr_dlklist *list, void *data);

/**
 * \brief Adds a new node at the requested position in a doubly linked list
 *
 * \param[out] list Doubly linked list where to add a new node
 * \param[in]  data Data to point to for the new node
 * \param[in]  pos  Position of the new node (starting at \e 0)
 *
 * \retval #GPR_ERR_OK The node has been added to the requested position in the list
 * \retval #GPR_ERR_INVALID_PARAMETER The list is \c NULL or the data is \c NULL
 *         or the position is invalid (for \e DEBUG mode only)
 * \retval #GPR_ERR_MEMORY_FAILURE The new node allocation failed
 */
enum GPR_Err gpr_dlklist_insert(struct gpr_dlklist *list, void *data, size_t pos);

/**
 * \brief Removes the first node of a doubly linked list
 *
 * \note The callback function must deal with a \c void* [*node data*]
 *
 * \param[out] list      Doubly linked list where to remove the first node
 * \param[in]  data_free Callback function that will be called to free data of the
 *                       node or \c NULL
 *
 * \retval #GPR_ERR_OK The first node has been removed from the list
 * \retval #GPR_ERR_INVALID_PARAMETER The list is \c NULL (for \e DEBUG mode only)
 * \retval #GPR_ERR_KO The list is empty
 */
enum GPR_Err gpr_dlklist_pop_front(struct gpr_dlklist *list, void (*data_free)());

/**
 * \brief Removes the last node of a doubly linked list
 *
 * \note The callback function must deal with a \c void* [*node data*]
 *
 * \param[out] list      Doubly linked list where to remove the last node
 * \param[in]  data_free Callback function that will be called to free data of the
 *                       node or \c NULL
 *
 * \retval #GPR_ERR_OK The last node has been removed from the list
 * \retval #GPR_ERR_INVALID_PARAMETER The list is \c NULL (for \e DEBUG mode only)
 * \retval #GPR_ERR_KO The list is empty
 */
enum GPR_Err gpr_dlklist_pop_back(struct gpr_dlklist *list, void (*data_free)());

/**
 * \brief Removes a node at the requested position in a doubly linked list
 *
 * \note The callback function must deal with a \c void* [*node data*]
 *
 * \param[out] list      Doubly linked list where to remove a node
 * \param[in]  data_free Callback function that will be called to free data of the
 *                       node or \c NULL
 * \param[in]  pos       Position of the node (starting at \e 0)
 *
 * \retval #GPR_ERR_OK The node at the request position has been removed from the list
 * \retval #GPR_ERR_INVALID_PARAMETER The list is \c NULL or the position is invalid
 *         (for \e DEBUG mode only)
 * \retval #GPR_ERR_KO The list is empty
 */
enum GPR_Err gpr_dlklist_remove(struct gpr_dlklist *list, void (*data_free)(), size_t pos);

/**
 * \brief Replaces the data pointed by a node at the requested position in a doubly linked list
 *
 * \note The callback function must deal with a \c void* [*node data*]
 *
 * \param[out] list      Doubly linked list where to replace the data pointed by a node
 * \param[in]  data_free Callback function that will be called to free the previous
 *                       data of the node or \c NULL
 * \param[in]  data      New Data to point to for the node
 * \param[in]  pos       Position of the node (starting at \e 0)
 *
 * \retval #GPR_ERR_OK The data pointed by the node at the request position has
 *         been replaced in the list
 * \retval #GPR_ERR_INVALID_PARAMETER The list is \c NULL or the data is \c NULL
 *         or the position is invalid (for \e DEBUG mode only)
 * \retval #GPR_ERR_KO The list is empty
 */
enum GPR_Err gpr_dlklist_replace(struct gpr_dlklist *list, void (*data_free)(), void *data, size_t pos);

/**
 * \brief Maps an action on the data of each node contained in a doubly linked list
 *
 * \note The callback function must deal with a \c void* [*node data*]
 *
 * \param[out] list     Doubly linked list where to map an action
 * \param[in]  data_map Callback function that will be called to map an action
 *                      on the data of each node
 */
void gpr_dlklist_map(struct gpr_dlklist *list, void (*data_map)());

/**
 * \brief Searches for a node according to the callback function return state in
 *        a doubly linked list
 *
 * \note The callback function must deal with two \c void* [*node data*, \p ctx]
 *       and returns a \c bool
 *
 * \param[out] list       Doubly linked list where to search for a node
 * \param[in] data_search Callback function that will be called to search for a node
 * \param[in] ctx         Context that will be given as a parameter to the
 *                        callback function
 * \param[out] pos        Position of the node found or \c NULL
 *
 * \return If the callback function returns \c true, the current node is returned
 *         with an update of the position variable if possible.\n
 *         Returns \c NULL if the end of the doubly linked list is reached
 */
struct gpr_dlknode *gpr_dlklist_search(const struct gpr_dlklist *list, bool (*data_search)(), const void *ctx, size_t *pos);

/**
 * \brief Checks if a doubly linked list is empty
 *
 * \param[in] list Doubly linked list to check
 *
 * \return Returns \c true if the doubly linked list is empty or not allocated,
 *         \c false otherwise
 */
bool gpr_dlklist_is_empty(const struct gpr_dlklist *list);

/**
 * \brief Counts the nodes of a doubly linked list
 *
 * \param[in] list Doubly linked list you want the size of
 *
 * \return Returns the doubly linked list size if allocated or \c 0 otherwise
 */
size_t gpr_dlklist_get_size(const struct gpr_dlklist *list);

/**
 * \brief Gets the head of a doubly linked list
 *
 * \param[in] list Doubly linked list you want the head of
 *
 * \return Returns the doubly linked list head if allocated or \c NULL otherwise
 */
struct gpr_dlknode *gpr_dlklist_get_head(const struct gpr_dlklist *list);

/**
 * \brief Gets the tail of a doubly linked list
 *
 * \param[in] list Doubly linked list you want the tail of
 *
 * \return Returns the doubly linked list tail if allocated or \c NULL otherwise
 */
struct gpr_dlknode *gpr_dlklist_get_tail(const struct gpr_dlklist *list);

/**
 * \brief Checks if a node contains data
 *
 * \param[in] node Node you want to check
 *
 * \return Returns \c true if the node contains data or \c false if node
 *         is not allocated or does not contain data
 */
bool gpr_dlklist_node_has_data(const struct gpr_dlknode *node);

/**
 * \brief Gets the node data
 *
 * \param[in] node Node you want the data of
 *
 * \return Returns the node data if allocated or \c NULL otherwise
 */
void *gpr_dlklist_node_data(const struct gpr_dlknode *node);

/**
 * \brief Gets the predecessor of a node
 *
 * \param[in] node Node you want the predecessor of
 *
 * \return Returns the node predecessor if allocated or \c NULL otherwise
 */
struct gpr_dlknode *gpr_dlklist_node_prev(const struct gpr_dlknode *node);

/**
 * \brief Gets the successor of a node
 *
 * \param[in] node Node you want the successor of
 *
 * \return Returns the node successor if allocated or \c NULL otherwise
 */
struct gpr_dlknode *gpr_dlklist_node_next(const struct gpr_dlknode *node);

/**
 * \brief Iterates over a list
 *
 * \param[out] node Node to use as a loop cursor
 * \param[in]  list List to loop on
 */
#define GPR_DLKLIST_FOR_EACH(node, list) \
    for (node = gpr_dlklist_get_head(list); gpr_dlklist_node_has_data(node); node = gpr_dlklist_node_next(node))

/**
 * \brief Iterates backwards over a list
 *
 * \param[out] node Node to use as a loop cursor
 * \param[in]  list List to loop on
 */
#define GPR_DLKLIST_FOR_EACH_REVERSE(node, list) \
    for (node = gpr_dlklist_get_tail(list); gpr_dlklist_node_has_data(node); node = gpr_dlklist_node_prev(node))

/**
 * \brief Iterates over data of a list
 *
 * \param[out] node Node to use as a loop cursor
 * \param[in]  list List to loop on
 * \param[out] data Data pointer to set
 */
#define GPR_DLKLIST_FOR_EACH_ENTRY(node, list, data)                                                             \
    for (node = gpr_dlklist_get_head(list), data = gpr_dlklist_node_data(node); gpr_dlklist_node_has_data(node); \
         node = gpr_dlklist_node_next(node), data = gpr_dlklist_node_data(node))

/**
 * \brief Iterates over data of a list, safe against removal of list entry
 *
 * \param[out] node Node to use as a loop cursor
 * \param[out] tmp  Temporary node to use as a temporary storage
 * \param[in]  list List to loop on
 * \param[out] data Data pointer to set
 */
#define GPR_DLKLIST_FOR_EACH_ENTRY_SAFE(node, tmp, list, data)                                                     \
    for (node = gpr_dlklist_get_head(list), data = gpr_dlklist_node_data(node), tmp = gpr_dlklist_node_next(node); \
         gpr_dlklist_node_has_data(node); node = tmp, data = gpr_dlklist_node_data(node), tmp = gpr_dlklist_node_next(node))

/**
 * \brief Iterates backwards over data of a list
 *
 * \param[out] node Node to use as a loop cursor
 * \param[in]  list List to loop on
 * \param[out] data Data pointer to set
 */
#define GPR_DLKLIST_FOR_EACH_ENTRY_REVERSE(node, list, data)                                                     \
    for (node = gpr_dlklist_get_tail(list), data = gpr_dlklist_node_data(node); gpr_dlklist_node_has_data(node); \
         node = gpr_dlklist_node_prev(node), data = gpr_dlklist_node_data(node))

/**
 * \brief Iterates backwards over data of a list, safe against removal of list entry
 *
 * \param[out] node Node to use as a loop cursor
 * \param[out] tmp  Temporary node to use as a temporary storage
 * \param[in]  list List to loop on
 * \param[out] data Data pointer to set
 */
#define GPR_DLKLIST_FOR_EACH_ENTRY_REVERSE_SAFE(node, tmp, list, data)                                             \
    for (node = gpr_dlklist_get_tail(list), data = gpr_dlklist_node_data(node), tmp = gpr_dlklist_node_prev(node); \
         gpr_dlklist_node_has_data(node); node = tmp, data = gpr_dlklist_node_data(node), tmp = gpr_dlklist_node_prev(node))

#endif /* H_GPR_DLKLIST */
