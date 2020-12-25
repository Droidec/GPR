/*****************************************************************************
 *
 * gpr_dlklist.h
 *
 *****************************************************************************
 * Copyright © 2020 Marc GIANNETTI
 *
 * Double linked list management functions
 *
 * Double linked list module offers the following possibilities:
 * - Easily declare a double linked list
 * - Perform all sort of tasks on it
 *
 * This module declares a generic double linked list
 *
 *****************************************************************************
 *
 * DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 *             Version 2, December 2004
 *
 * Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
 *
 * Everyone is permitted to copy and distribute verbatim or modified
 * copies of this license document, and changing it is allowed as long
 * as the name is changed.
 *
 *            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
 * TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
 *
 * 0. You just DO WHAT THE FUCK YOU WANT TO.
 *
 *****************************************************************************/

#ifndef H_GPR_DLKLIST
#define H_GPR_DLKLIST

#include "gpr_err.h"

#include <stdio.h>
#include <stdbool.h>

struct gpr_dlknode
{
    void *data;               // Pointed data
    struct gpr_dlknode *prev; // Previous node
    struct gpr_dlknode *next; // Next node
};

struct gpr_dlklist
{
    size_t size;              // Size of the list
    struct gpr_dlknode *head; // Head of the list (First node)
    struct gpr_dlknode *tail; // Tail of the list (Last node)
};

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Allocation
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*****************************************************************************
 *
 * Create a new double linked list
 *
 * Parameters
 *     None
 *
 * Return value
 *     On success, return a pointer to the double linked list
 *     On failure, return NULL
 *
 *****************************************************************************/
struct gpr_dlklist *gpr_dlklist_create(void);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Deallocation
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*****************************************************************************
 *
 * Reset a double linked list (Free all its nodes)
 *
 * Parameters
 *     list      : Double linked list where to free nodes
 *     data_free : Callback function that will be called to free data of
 *                 each node. Can be NULL
 *
 * Return value
 *     None
 *
 *****************************************************************************/
void gpr_dlklist_reset(struct gpr_dlklist *list, void (*data_free)());

/*****************************************************************************
 *
 * Free a double linked list and all its nodes
 *
 * Parameters
 *     list      : Double linked list to free
 *     data_free : Callback function that will be called to free data of
 *                 each node. Can be NULL
 *
 * Return value
 *     None
 *
 *****************************************************************************/
void gpr_dlklist_free(struct gpr_dlklist *list, void (*data_free)());

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Manipulation
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*****************************************************************************
 *
 * Add a new node at the beginning of a double linked list
 *
 * NOTE: A watchdog prevents you to add data that is already pointed in
 * an other node
 *
 * Parameters
 *     list : Double linked list where to add a new node
 *     data : Data to point to for the new node
 *
 * Return value
 *     - GPR_ERR_OK: The node has been added to the beginning of the list
 *     - GPR_ERR_INVALID_PARAMETER: The list is NULL or the data is NULL
 *     - GPR_ERR_MEMORY_FAILURE: The new node allocation failed
 *     - GPR_ERR_KO: A duplicate has been detected (watchdog)
 *
 *****************************************************************************/
enum GPR_Err gpr_dlklist_push_front(struct gpr_dlklist *list, void *data);

/*****************************************************************************
 *
 * Add a new node at the end of a double linked list
 *
 * NOTE: A watchdog prevents you to add data that is already pointed in
 * an other node
 *
 * Parameters
 *     list : Double linked list where to add a new node
 *     data : Data to point to for the new node
 *
 * Return value
 *     - GPR_ERR_OK: The node has been added to the end of the list
 *     - GPR_ERR_INVALID_PARAMETER: The list is NULL or the data is NULL
 *     - GPR_ERR_MEMORY_FAILURE: The new node allocation failed
 *     - GPR_ERR_KO: A duplicate has been detected (watchdog)
 *
 *****************************************************************************/
enum GPR_Err gpr_dlklist_push_back(struct gpr_dlklist *list, void *data);

/*****************************************************************************
 *
 * Add a new node at the requested position in a double linked list
 *
 * NOTE: A watchdog prevents you to add data that is already pointed in
 * an other node
 *
 * Parameters
 *     list : Double linked list where to add a new node
 *     data : Data to point to for the new node
 *     pos  : Position of the new node (starting at 0)
 *
 * Return value
 *     - GPR_ERR_OK: The node has been added to the requested position in
 *         the list
 *     - GPR_ERR_INVALID_PARAMETER: The list is NULL or the data is NULL or
 *         the position is invalid
 *     - GPR_ERR_MEMORY_FAILURE: The new node allocation failed
 *     - GPR_ERR_KO: A duplicate has been detected (watchdog)
 *
 *****************************************************************************/
enum GPR_Err gpr_dlklist_insert(struct gpr_dlklist *list, void *data, size_t pos);

/*****************************************************************************
 *
 * Remove the first node of a double linked list
 *
 * Parameters
 *     list      : Double linked list where to remove a node
 *     data_free : Callback function that will be called to free data of the
 *                 node. Can be NULL
 *
 * Return value
 *     - GPR_ERR_OK: The first node has been removed from the list
 *     - GPR_ERR_INVALID_PARAMETER: The list is NULL
 *     - GPR_ERR_KO: The list is empty
 *
 *****************************************************************************/
enum GPR_Err gpr_dlklist_pop_front(struct gpr_dlklist *list, void (*data_free)());

/*****************************************************************************
 *
 * Remove the last node of a double linked list
 *
 * Parameters
 *     list      : Double linked list where to remove a node
 *     data_free : Callback function that will be called to free data of the
 *                 node. Can be NULL
 *
 * Return value
 *     - GPR_ERR_OK: The last node has been removed from the list
 *     - GPR_ERR_INVALID_PARAMETER: The list is NULL
 *     - GPR_ERR_KO: The list is empty
 *
 *****************************************************************************/
enum GPR_Err gpr_dlklist_pop_back(struct gpr_dlklist *list, void (*data_free)());

/*****************************************************************************
 *
 * Remove a node at the requested position in a double linked list
 *
 * Parameters
 *     list      : Double linked list where to remove a node
 *     data_free : Callback function that will be called to free data of the
 *                 node. Can be NULL
 *     pos       : Position of the node (starting at 0)
 *
 * Return value
 *     - GPR_ERR_OK: The node at the request position has been removed from
 *         the list
 *     - GPR_ERR_INVALID_PARAMETER: The list is NULL or the position is
 *         invalid
 *     - GPR_ERR_KO: The list is empty
 *
 *****************************************************************************/
enum GPR_Err gpr_dlklist_remove(struct gpr_dlklist *list, void (*data_free)(), size_t pos);

/*****************************************************************************
 *
 * Map an action on the data of each node contained in a double linked list
 *
 * Parameters
 *     list     : Double linked list where to remove a node
 *     data_map : Callback function that will be called to map an action
 *                on the data of each node
 *
 * Return value
 *     None
 *
 *****************************************************************************/
void gpr_dlklist_map(struct gpr_dlklist *list, void (*data_map)());

/*****************************************************************************
 *
 * Search for a node according the callback function return state in a double
 * linked list
 *
 * Parameters
 *     list        : Double linked list where to remove a node
 *     data_search : Callback function that will be called to map an action
 *                   on the data of each node
 *     pos         : Position of the node found. Can be NULL
 *
 * Return value
 *     If the callback function returns true, the current node is returned
 *     with an update of the position variable.
 *     Return NULL if the end of the double linked list is reached
 *
 *****************************************************************************/
struct gpr_dlknode *gpr_dlklist_search(struct gpr_dlklist *list, bool (*data_search)(), size_t *pos);

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Accessor
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*****************************************************************************
 *
 * Check if a double linked list is empty
 *
 * Parameters
 *     list : Double linked list to check
 *
 * Return value
 *     True if the double linked list is empty or not allocated
 *     False otherwise
 *
 *****************************************************************************/
bool gpr_dlklist_is_empty(struct gpr_dlklist *list);

/*****************************************************************************
 *
 * Get the size of a double linked list (its number of nodes)
 *
 * Parameters
 *     list : Double linked list you want the size of
 *
 * Return value
 *     The double linked list size if allocated
 *     0 otherwise
 *
 *****************************************************************************/
size_t gpr_dlklist_get_size(struct gpr_dlklist *list);

/*****************************************************************************
 *
 * Get the head of a double linked list
 *
 * Parameters
 *     list : Double linked list you want the head of
 *
 * Return value
 *     The double linked list head if allocated
 *     NULL otherwise
 *
 *****************************************************************************/
struct gpr_dlknode *gpr_dlklist_get_head(struct gpr_dlklist *list);

/*****************************************************************************
 *
 * Get the tail of a double linked list
 *
 * Parameters
 *     list : Double linked list you want the tail of
 *
 * Return value
 *     The double linked list tail if allocated
 *     NULL otherwise
 *
 *****************************************************************************/
struct gpr_dlknode *gpr_dlklist_get_tail(struct gpr_dlklist *list);

/*****************************************************************************
 *
 * Check if a node contain data
 *
 * Parameters
 *     node : Node you want to check
 *
 * Return value
 *     True if the node contains data
 *     False otherwise or node is not allocated
 *
 *****************************************************************************/
bool gpr_dlklist_node_has_data(struct gpr_dlknode *node);

/*****************************************************************************
 *
 * Get the node data
 *
 * Parameters
 *     node : Node you want the data of
 *
 * Return value
 *     The node data if allocated
 *     NULL otherwise
 *
 *****************************************************************************/
void *gpr_dlklist_node_data(struct gpr_dlknode *node);

/*****************************************************************************
 *
 * Get the predecessor of a node
 *
 * Parameters
 *     node : Node you want the predecessor of
 *
 * Return value
 *     The node predecessor if allocated
 *     NULL otherwise
 *
 *****************************************************************************/
struct gpr_dlknode *gpr_dlklist_node_prev(struct gpr_dlknode *node);

/*****************************************************************************
 *
 * Get the successor of a node
 *
 * Parameters
 *     node : Node you want the successor of
 *
 * Return value
 *     The node successor if allocated
 *     NULL otherwise
 *
 *****************************************************************************/
struct gpr_dlknode *gpr_dlklist_node_next(struct gpr_dlknode *node);

#endif /* H_GPR_DLKLIST */
