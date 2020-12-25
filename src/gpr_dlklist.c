/*****************************************************************************
 *
 * gpr_dlklist.c
 *
 *****************************************************************************
 * Copyright © 2020 Marc GIANNETTI
 *
 * Double linked list management functions
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

#include "gpr_dlklist.h"
#include "gpr_builtin.h"

#include <stdlib.h>

/*****************************************************************************
 * Private prototypes
 *****************************************************************************/

static struct gpr_dlknode *create_node(void *data);
static void free_node(struct gpr_dlknode *node, void (*data_free)());
static bool is_contained(struct gpr_dlklist *list, void *data);

/*****************************************************************************
 * Public functions
 *****************************************************************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Allocation
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

struct gpr_dlklist *gpr_dlklist_create(void)
{
    struct gpr_dlklist *list = NULL;

    /* Allocate list */
    list = (struct gpr_dlklist *)malloc(sizeof(struct gpr_dlklist));
    if (UNLIKELY(list == NULL))
        return NULL;

    /* Initialize list */
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Deallocation
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void gpr_dlklist_reset(struct gpr_dlklist *list, void (*data_free)())
{
    struct gpr_dlknode *scout = NULL;
    struct gpr_dlknode *next = NULL;

    /* Check consistency */
    if (list == NULL || list->head == NULL)
        return;

    /* Free list nodes */
    scout = gpr_dlklist_get_head(list);
    while (gpr_dlklist_node_has_data(scout))
    {
        next = gpr_dlklist_node_next(scout);
        free_node(scout, data_free);
        scout = next;
    }

    /* Reset list */
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;

    return;
}

void gpr_dlklist_free(struct gpr_dlklist *list, void (*data_free)())
{
    /* Check consistency */
    if (list == NULL)
        return;

    /* Reset list */
    gpr_dlklist_reset(list, data_free);

    /* Free list */
    free(list);

    return;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Manipulation
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

enum GPR_Err gpr_dlklist_push_front(struct gpr_dlklist *list, void *data)
{
    struct gpr_dlknode *node = NULL;
    struct gpr_dlknode *first = NULL;

    /* Check consistency */
    if (list == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid list");

    if (data == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid data");

    if (is_contained(list, data))
        return gpr_err_raise(GPR_ERR_KO, "Duplicate detected");

    /* Allocate node */
    node = create_node(data);
    if (UNLIKELY(node == NULL))
        return gpr_err_raise(GPR_ERR_MEMORY_FAILURE, "Node allocation failed");

    /* Expand list */

    // Simple case
    if (gpr_dlklist_is_empty(list))
    {
        list->head = node;
        list->tail = node;
        list->size++;
        return gpr_err_raise(GPR_ERR_OK, NULL);
    }

    // Push front
    first = gpr_dlklist_get_head(list);
    first->prev = node;
    node->next = first;
    list->head = node;
    list->size++;

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

enum GPR_Err gpr_dlklist_push_back(struct gpr_dlklist *list, void *data)
{
    struct gpr_dlknode *node = NULL;
    struct gpr_dlknode *last = NULL;

    /* Check consistency */
    if (list == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid list");

    if (data == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid data");

    if (is_contained(list, data))
        return gpr_err_raise(GPR_ERR_KO, "Duplicate detected");

    /* Allocate node */
    node = create_node(data);
    if (UNLIKELY(node == NULL))
        return gpr_err_raise(GPR_ERR_MEMORY_FAILURE, "Node allocation failed");

    /* Expand list */

    // Simple case
    if (gpr_dlklist_is_empty(list))
    {
        list->head = node;
        list->tail = node;
        list->size++;
        return gpr_err_raise(GPR_ERR_OK, NULL);
    }

    // Push back
    last = gpr_dlklist_get_tail(list);
    last->next = node;
    node->prev = last;
    list->tail = node;
    list->size++;

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

enum GPR_Err gpr_dlklist_insert(struct gpr_dlklist *list, void *data, size_t pos)
{
    struct gpr_dlknode *scout = NULL;
    struct gpr_dlknode *node = NULL;
    size_t counter = 0;

    /* Check consistency */
    if (list == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid list");

    if (data == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid data");

    if (pos > gpr_dlklist_get_size(list))
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid position");

    if (pos == 0)
        return gpr_dlklist_push_front(list, data);

    if (pos == gpr_dlklist_get_size(list))
        return gpr_dlklist_push_back(list, data);

    if (is_contained(list, data))
        return gpr_err_raise(GPR_ERR_KO, "Duplicate detected");

    /* Allocate node */
    node = create_node(data);
    if (UNLIKELY(node == NULL))
        return gpr_err_raise(GPR_ERR_MEMORY_FAILURE, "Node allocation failed");

    /* Expand list */

    // Search position
    scout = gpr_dlklist_get_head(list);
    while (counter != pos)
    {
        counter++;
        scout = gpr_dlklist_node_next(scout);
    }

    // Insert node
    node->next = scout;
    node->prev = scout->prev;
    scout->prev->next = node;
    scout->prev = node;
    list->size++;

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

enum GPR_Err gpr_dlklist_pop_front(struct gpr_dlklist *list, void (*data_free)())
{
    struct gpr_dlknode *first = NULL;

    /* Check consistency */
    if (list == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid list");

    if (gpr_dlklist_is_empty(list))
        return gpr_err_raise(GPR_ERR_KO, "Empty list");

    /* Shrink list */

    // Simple case
    if (gpr_dlklist_get_size(list) == 1)
    {
        gpr_dlklist_reset(list, data_free);
        return gpr_err_raise(GPR_ERR_OK, NULL);
    }

    // Pop front
    first = gpr_dlklist_get_head(list);
    list->head = gpr_dlklist_node_next(first);
    list->head->prev = NULL;
    list->size--;
    free_node(first, data_free);

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

enum GPR_Err gpr_dlklist_pop_back(struct gpr_dlklist *list, void (*data_free)())
{
    struct gpr_dlknode *last = NULL;

    /* Check consistency */
    if (list == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid list");

    if (gpr_dlklist_is_empty(list))
        return gpr_err_raise(GPR_ERR_KO, "Empty list");

    /* Shrink list */

    // Simple case
    if (gpr_dlklist_get_size(list) == 1)
    {
        gpr_dlklist_reset(list, data_free);
        return gpr_err_raise(GPR_ERR_OK, NULL);
    }

    // Pop back
    last = gpr_dlklist_get_tail(list);
    list->tail = gpr_dlklist_node_prev(last);
    list->tail->next = NULL;
    list->size--;
    free_node(last, data_free);

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

enum GPR_Err gpr_dlklist_remove(struct gpr_dlklist *list, void (*data_free)(), size_t pos)
{
    struct gpr_dlknode *scout = NULL;
    size_t counter = 0;

    /* Check consistency */
    if (list == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid list");

    if (pos >= gpr_dlklist_get_size(list))
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid position");

    if (pos == 0)
        return gpr_dlklist_pop_front(list, data_free);

    if (pos == gpr_dlklist_get_size(list) - 1)
        return gpr_dlklist_pop_back(list, data_free);

    /* Shrink list */

    // Search position
    scout = gpr_dlklist_get_head(list);
    while (counter != pos)
    {
        counter++;
        scout = gpr_dlklist_node_next(scout);
    }

    // Remove node
    scout->prev->next = scout->next;
    scout->next->prev = scout->prev;
    list->size--;
    free_node(scout, data_free);

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

void gpr_dlklist_map(struct gpr_dlklist *list, void (*data_map)())
{
    struct gpr_dlknode *scout = NULL;

    /* Check consistency */
    if (list == NULL || data_map == NULL)
        return;

    /* Map action */
    scout = gpr_dlklist_get_head(list);
    while (gpr_dlklist_node_has_data(scout))
    {
        data_map(gpr_dlklist_node_data(scout));
        scout = gpr_dlklist_node_next(scout);
    }

    return;
}

struct gpr_dlknode *gpr_dlklist_search(struct gpr_dlklist *list, bool (*data_search)(), size_t *pos)
{
    struct gpr_dlknode *scout = NULL;
    size_t counter = 0;

    /* Check consistency */
    if (list == NULL || data_search == NULL)
        goto end;

    /* Search node */
    scout = gpr_dlklist_get_head(list);
    while (gpr_dlklist_node_has_data(scout))
    {
        if (data_search(gpr_dlklist_node_data(scout)))
            goto end;

        counter++;
        scout = gpr_dlklist_node_next(scout);
    }
    counter = 0;

  end:
    if (pos)
        *pos = counter;
    return scout;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+ Accessor
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

bool gpr_dlklist_is_empty(struct gpr_dlklist *list)
{
    /* Check consistency */
    if (list == NULL || list->head == NULL)
        return true;

    return false;
}

size_t gpr_dlklist_get_size(struct gpr_dlklist *list)
{
    /* Check consistency */
    if (list == NULL)
        return 0;

    return list->size;
}

struct gpr_dlknode *gpr_dlklist_get_head(struct gpr_dlklist *list)
{
    /* Check consistency */
    if (list == NULL)
        return NULL;

    return list->head;
}

struct gpr_dlknode *gpr_dlklist_get_tail(struct gpr_dlklist *list)
{
    /* Check consistency */
    if (list == NULL)
        return NULL;

    return list->tail;
}

bool gpr_dlklist_node_has_data(struct gpr_dlknode *node)
{
    /* Check consistency */
    if (node == NULL || node->data == NULL)
        return false;

    return true;
}

void *gpr_dlklist_node_data(struct gpr_dlknode *node)
{
    /* Check consistency */
    if (node == NULL)
        return NULL;

    return node->data;
}

struct gpr_dlknode *gpr_dlklist_node_prev(struct gpr_dlknode *node)
{
    /* Check consistency */
    if (node == NULL)
        return NULL;

    return node->prev;
}

struct gpr_dlknode *gpr_dlklist_node_next(struct gpr_dlknode *node)
{
    /* Check consistency */
    if (node == NULL)
        return NULL;

    return node->next;
}

/*****************************************************************************
 * Private functions
 *****************************************************************************/

/*****************************************************************************
 *
 * Create a new node
 *
 * Parameters
 *     data : Data to point to for the new node
 *
 * Return value
 *     On success, return a pointer to the new node
 *     On failure, return NULL
 *
 *****************************************************************************/
static struct gpr_dlknode *create_node(void *data)
{
    struct gpr_dlknode *node = NULL;

    /* Check consistency */
    if (data == NULL)
        return NULL;

    /* Allocate node */
    node = (struct gpr_dlknode *)malloc(sizeof(struct gpr_dlknode));
    if (UNLIKELY(node == NULL))
        return NULL;

    /* Initialize node */
    node->data = data;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

/*****************************************************************************
 *
 * Free a node
 *
 * Parameters
 *     node      : Node to free
 *     data_free : Callback function that will be called to free data of the
 *                 node. Can be NULL
 *
 * Return value
 *     None
 *
 *****************************************************************************/
static void free_node(struct gpr_dlknode *node, void (*data_free)())
{
    /* Check consistency */
    if (node == NULL)
        return;

    /* Free data if necessary */
    if (node->data != NULL && data_free != NULL)
        data_free(node->data);

    free(node);

    return;
}

/*****************************************************************************
 *
 * Check if data is already pointed in a double linked list
 *
 * Parameters
 *     list : Double linked list to look at
 *     data : Data pointer to look for
 *
 * Return value
 *     True if the double linked list already contains data
 *     False otherwise
 *
 *****************************************************************************/
static bool is_contained(struct gpr_dlklist *list, void *data)
{
    struct gpr_dlknode *scout = NULL;

    if (gpr_dlklist_is_empty(list))
        return false;

    scout = gpr_dlklist_get_head(list);
    while (gpr_dlklist_node_has_data(scout))
    {
        if (data == gpr_dlklist_node_data(scout))
            return true;
        scout = gpr_dlklist_node_next(scout);
    }

    return false;
}
