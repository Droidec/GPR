/******************************************************************************
 *
 * \file gpr_dlklist.h
 * \brief Doubly linked list module
 * \details
 * This module defines a generic doubly linked list composed of nodes\n
 * The list keeps a pointer on the first node and the last node\n
 * Each node has a pointer to the previous one and the next one
 *
 * \verbatim
 *
 *        ┌───────────────────┐
 *        │ head size(2) tail │
 *        └──┼────────────┼───┘
 *        ┌──┼───┐     ┌──┼───┐
 * NULL <─┼ node ┼─────┼ node ┼─> NULL
 *        └──┼───┘     └──┼───┘
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

#include "gpr_err.h"

#include <stdio.h>
#include <stdbool.h> // WARN: Not portable

/**
 * \brief Doubly linked list component structure
 */
struct gpr_dlknode
{
    void *data;               ///< Pointed data
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

/******************************************************************************
 *
 * \brief Allocate and initialize a new doubly linked list
 *
 * \return
 *     On success, return a pointer to the doubly linked list\n
 *     On failure, return NULL
 *
 *****************************************************************************/
struct gpr_dlklist *gpr_dlklist_new(void);

/******************************************************************************
 *
 * \brief Initialize a doubly linked list
 *
 * \param list Doubly linked list to initialize
 *
 *****************************************************************************/
void gpr_dlklist_init(struct gpr_dlklist *list);

/******************************************************************************
 *
 * \brief Reset a doubly linked list (Free all its nodes)
 *
 * \note The callback function must deal with a void * (node data)
 *
 * \param list      Doubly linked list where to free nodes
 * \param data_free Callback function that will be called to free data of
 *                  each node (Can be NULL)
 *
 *****************************************************************************/
void gpr_dlklist_reset(struct gpr_dlklist *list, void (*data_free)());

/******************************************************************************
 *
 * \brief Free a doubly linked list and all its nodes
 *
 * \note The callback function must deal with a void * (node data)
 *
 * \param list      Doubly linked list to free
 * \param data_free Callback function that will be called to free data of
 *                  each node (Can be NULL)
 *
 *****************************************************************************/
void gpr_dlklist_free(struct gpr_dlklist *list, void (*data_free)());

/******************************************************************************
 *
 * \brief Add a new node at the beginning of a doubly linked list
 *
 * \param list Doubly linked list where to add a new node at the beginning
 * \param data Data to point to for the new node
 *
 * \return
 *     GPR_ERR_OK: The node has been added to the beginning of the list\n
 *     GPR_ERR_INVALID_PARAMETER: The list is NULL or the data is NULL
 *     (DEBUG mode only)\n
 *     GPR_ERR_MEMORY_FAILURE: The new node allocation failed
 *
 *****************************************************************************/
enum GPR_Err gpr_dlklist_push_front(struct gpr_dlklist *list, void *data);

/******************************************************************************
 *
 * \brief Add a new node at the end of a doubly linked list
 *
 * \param list Doubly linked list where to add a new node at the end
 * \param data Data to point to for the new node
 *
 * \return
 *     GPR_ERR_OK: The node has been added to the end of the list\n
 *     GPR_ERR_INVALID_PARAMETER: The list is NULL or the data is NULL
 *     (DEBUG mode only)\n
 *     GPR_ERR_MEMORY_FAILURE: The new node allocation failed
 *
 *****************************************************************************/
enum GPR_Err gpr_dlklist_push_back(struct gpr_dlklist *list, void *data);

/******************************************************************************
 *
 * \brief Add a new node at the requested position in a doubly linked list
 *
 * \param list Doubly linked list where to add a new node
 * \param data Data to point to for the new node
 * \param pos  Position of the new node (starting at 0)
 *
 * \return
 *     GPR_ERR_OK: The node has been added to the requested position in
 *     the list\n
 *     GPR_ERR_INVALID_PARAMETER: The list is NULL or the data is NULL or
 *     the position is invalid (DEBUG mode only)\n
 *     GPR_ERR_MEMORY_FAILURE: The new node allocation failed
 *
 *****************************************************************************/
enum GPR_Err gpr_dlklist_insert(struct gpr_dlklist *list, void *data, size_t pos);

/******************************************************************************
 *
 * \brief Remove the first node of a doubly linked list
 *
 * \note The callback function must deal with a void * (node data)
 *
 * \param list      Doubly linked list where to remove the first node
 * \param data_free Callback function that will be called to free data of the
 *                  node (Can be NULL)
 *
 * \return
 *     GPR_ERR_OK: The first node has been removed from the list\n
 *     GPR_ERR_INVALID_PARAMETER: The list is NULL (DEBUG mode only)\n
 *     GPR_ERR_KO: The list is empty
 *
 *****************************************************************************/
enum GPR_Err gpr_dlklist_pop_front(struct gpr_dlklist *list, void (*data_free)());

/******************************************************************************
 *
 * \brief Remove the last node of a doubly linked list
 *
 * \note The callback function must deal with a void * (node data)
 *
 * \param list      Doubly linked list where to remove the last node
 * \param data_free Callback function that will be called to free data of the
 *                  node (Can be NULL)
 *
 * \return
 *     GPR_ERR_OK: The last node has been removed from the list\n
 *     GPR_ERR_INVALID_PARAMETER: The list is NULL (DEBUG mode only)\n
 *     GPR_ERR_KO: The list is empty
 *
 *****************************************************************************/
enum GPR_Err gpr_dlklist_pop_back(struct gpr_dlklist *list, void (*data_free)());

/******************************************************************************
 *
 * \brief Remove a node at the requested position in a doubly linked list
 *
 * \note The callback function must deal with a void * (node data)
 *
 * \param list      Doubly linked list where to remove a node
 * \param data_free Callback function that will be called to free data of the
 *                  node (Can be NULL)
 * \param pos       Position of the node (starting at 0)
 *
 * \return
 *     GPR_ERR_OK: The node at the request position has been removed from
 *     the list\n
 *     GPR_ERR_INVALID_PARAMETER: The list is NULL or the position is
 *     invalid (DEBUG mode only)\n
 *     GPR_ERR_KO: The list is empty
 *
 *****************************************************************************/
enum GPR_Err gpr_dlklist_remove(struct gpr_dlklist *list, void (*data_free)(), size_t pos);

/******************************************************************************
 *
 * \brief Replace the data pointed by a node at the requested position in a
 * doubly linked list
 *
 * \note The callback function must deal with a void * (node data)
 *
 * \param list      Doubly linked list where to replace the data pointed by a
 *                  node
 * \param data_free Callback function that will be called to free the previous
 *                  data of the node (Can be NULL)
 * \param data      New Data to point to for the node
 * \param pos       Position of the node (starting at 0)
 *
 * \return
 *     GPR_ERR_OK: The data pointed by the node at the request position has
 *     been replaced in the list\n
 *     GPR_ERR_INVALID_PARAMETER: The list is NULL or the data is NULL or
 *     the position is invalid (DEBUG mode only)\n
 *     GPR_ERR_KO: The list is empty
 *
 *****************************************************************************/
enum GPR_Err gpr_dlklist_replace(struct gpr_dlklist *list, void (*data_free)(), void *data, size_t pos);

/******************************************************************************
 *
 * \brief Map an action on the data of each node contained in a doubly linked
 * list
 *
 * \note The callback function must deal with a void * (node data)
 *
 * \param list     Doubly linked list where to map an action
 * \param data_map Callback function that will be called to map an action
 *                 on the data of each node
 *
 *****************************************************************************/
void gpr_dlklist_map(struct gpr_dlklist *list, void (*data_map)());

/******************************************************************************
 *
 * \brief Search for a node according to the callback function return state in
 * a doubly linked list
 *
 * \note The callback function must deal with two void * (node data, context)
 *
 * \param list        Doubly linked list where to search for a node
 * \param data_search Callback function that will be called to search for a
 *                    node
 * \param ctx         Context that will be given as a parameter to the
 *                    callback function
 * \param pos         Position of the node found (Can be NULL)
 *
 * \return
 *     If the callback function returns true, the current node is returned
 *     with an update of the position variable if possible\n
 *     Return NULL if the end of the doubly linked list is reached
 *
 *****************************************************************************/
struct gpr_dlknode *gpr_dlklist_search(const struct gpr_dlklist *list, bool (*data_search)(), const void *ctx, size_t *pos);

/******************************************************************************
 *
 * \brief Check if a doubly linked list is empty
 *
 * \param list Doubly linked list to check
 *
 * \return
 *     True if the doubly linked list is empty or not allocated\n
 *     False otherwise
 *
 *****************************************************************************/
bool gpr_dlklist_is_empty(const struct gpr_dlklist *list);

/******************************************************************************
 *
 * \brief Get the size of a doubly linked list (its number of nodes)
 *
 * \param list Doubly linked list you want the size of
 *
 * \return
 *     The doubly linked list size if allocated\n
 *     Zero otherwise
 *
 *****************************************************************************/
size_t gpr_dlklist_get_size(const struct gpr_dlklist *list);

/******************************************************************************
 *
 * \brief Get the head of a doubly linked list
 *
 * \param list Doubly linked list you want the head of
 *
 * \return
 *     The doubly linked list head if allocated\n
 *     NULL otherwise
 *
 *****************************************************************************/
struct gpr_dlknode *gpr_dlklist_get_head(const struct gpr_dlklist *list);

/******************************************************************************
 *
 * \brief Get the tail of a doubly linked list
 *
 * \param list Doubly linked list you want the tail of
 *
 * \return
 *     The doubly linked list tail if allocated\n
 *     NULL otherwise
 *
 *****************************************************************************/
struct gpr_dlknode *gpr_dlklist_get_tail(const struct gpr_dlklist *list);

/******************************************************************************
 *
 * \brief Check if a node contain data
 *
 * \param node Node you want to check
 *
 * \return
 *     True if the node contains data\n
 *     False if node is not allocated or does not contain data
 *
 *****************************************************************************/
bool gpr_dlklist_node_has_data(const struct gpr_dlknode *node);

/******************************************************************************
 *
 * \brief Get the node data
 *
 * \param node Node you want the data of
 *
 * \return
 *     The node data if allocated\n
 *     NULL otherwise
 *
 *****************************************************************************/
void *gpr_dlklist_node_data(const struct gpr_dlknode *node);

/******************************************************************************
 *
 * \brief Get the predecessor of a node
 *
 * \param node Node you want the predecessor of
 *
 * \return
 *     The node predecessor if allocated\n
 *     NULL otherwise
 *
 *****************************************************************************/
struct gpr_dlknode *gpr_dlklist_node_prev(const struct gpr_dlknode *node);

/******************************************************************************
 *
 * \brief Get the successor of a node
 *
 * \param node Node you want the successor of
 *
 * \return
 *     The node successor if allocated\n
 *     NULL otherwise
 *
 *****************************************************************************/
struct gpr_dlknode *gpr_dlklist_node_next(const struct gpr_dlknode *node);

#endif /* H_GPR_DLKLIST */
