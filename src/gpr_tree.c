/******************************************************************************
 *
 * \file gpr_tree.c
 * \brief Tree module
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

#include "gpr_tree.h"

#include <stdarg.h>  // va_list, va_start, va_end
#include <stdbool.h> // bool
#include <stddef.h>  // size_t
#include <stdlib.h>  // malloc, free
#include <string.h>  // memset, strncmp

#include "gpr_err.h"
#include "gpr_utils.h"

/**
 * \brief Available searching patterns on node components that can be performed
 */
enum Search_By_Node
{
    SEARCH_BY_NODE_LABEL,  ///< Perform a search on node label
    SEARCH_BY_NODE_VALUE,  ///< Perform a search on node value
    SEARCH_BY_NODE_UNKNOWN ///< Search pattern limit (DO NOT USE)
};

static bool is_contained(struct gpr_tnode *node1, struct gpr_tnode *node2);
static struct gpr_tnode *search_by_node_component(struct gpr_tnode *origin, const char *match, enum Search_By_Node pattern);

struct gpr_tnode *gpr_tree_new_node(const char *label)
{
    struct gpr_tnode *node = NULL;

#ifdef DEBUG
    /* Check consistency */
    if (label == NULL)
        return NULL;
#endif

    /* Allocate tree node */
    node = (struct gpr_tnode *)malloc(sizeof(struct gpr_tnode));
    if (UNLIKELY(node == NULL))
        return NULL;

    /* Initialize tree node */
    gpr_tree_init_node(node);
    SCNPRINTF(node->label, GPR_TREE_NODE_LABEL_LEN + 1, "%s", label);

    return node;
}

struct gpr_tnode *gpr_tree_new_vnode(const char *label, const char *value, ...)
{
    struct gpr_tnode *node = NULL;
    va_list list;

#ifdef DEBUG
    /* Check consistency */
    if ((label == NULL) || (value == NULL))
        return NULL;
#endif

    /* Allocate and initialize tree node */
    node = gpr_tree_new_node(label);
    if (node == NULL)
        return NULL;

    /* Set node value */
    va_start(list, value);
    VSCNPRINTF(node->value, GPR_TREE_NODE_VALUE_LEN + 1, value, list);
    va_end(list);

    return node;
}

struct gpr_tnode *gpr_tree_new_child(struct gpr_tnode *origin, const char *label)
{
    struct gpr_tnode *node = NULL;

#ifdef DEBUG
    /* Check consistency */
    if ((origin == NULL) || (label == NULL))
        return NULL;
#endif

    /* Allocate and initialize tree node */
    node = gpr_tree_new_node(label);
    if (node == NULL)
        return NULL;

    /* Associate tree node as a child */
    if (gpr_tree_add_child(origin, node) != GPR_ERR_OK)
    {
        gpr_tree_free_node(node);
        return NULL;
    }

    return node;
}

struct gpr_tnode *gpr_tree_new_vchild(struct gpr_tnode *origin, const char *label, const char *value, ...)
{
    struct gpr_tnode *node = NULL;
    va_list list;

#ifdef DEBUG
    /* Check consistency */
    if ((origin == NULL) || (label == NULL) || (value == NULL))
        return NULL;
#endif

    /* Allocate and initialize child node */
    node = gpr_tree_new_child(origin, label);
    if (node == NULL)
        return NULL;

    /* Set node value */
    va_start(list, value);
    VSCNPRINTF(node->value, GPR_TREE_NODE_VALUE_LEN + 1, value, list);
    va_end(list);

    return node;
}

struct gpr_tnode *gpr_tree_new_sibling(struct gpr_tnode *origin, const char *label)
{
    struct gpr_tnode *node = NULL;

#ifdef DEBUG
    /* Check consistency */
    if ((origin == NULL) || (label == NULL))
        return NULL;
#endif

    /* Allocate and initialize tree node */
    node = gpr_tree_new_node(label);
    if (node == NULL)
        return NULL;

    /* Associate tree node as a sibling */
    if (gpr_tree_add_sibling(origin, node) != GPR_ERR_OK)
    {
        gpr_tree_free_node(node);
        return NULL;
    }

    return node;
}

struct gpr_tnode *gpr_tree_new_vsibling(struct gpr_tnode *origin, const char *label, const char *value, ...)
{
    struct gpr_tnode *node = NULL;
    va_list list;

#ifdef DEBUG
    /* Check consistency */
    if ((origin == NULL) || (label == NULL) || (value == NULL))
        return NULL;
#endif

    /* Allocate and initialize sibling node */
    node = gpr_tree_new_sibling(origin, label);
    if (node == NULL)
        return NULL;

    /* Set node value */
    va_start(list, value);
    VSCNPRINTF(node->value, GPR_TREE_NODE_VALUE_LEN + 1, value, list);
    va_end(list);

    return node;
}

void gpr_tree_init_node(struct gpr_tnode *node)
{
#ifdef DEBUG
    /* Check consistency */
    if (node == NULL)
        return;
#endif

    /* Initialize tree node */
    memset(node->label, 0, GPR_TREE_NODE_LABEL_LEN + 1);
    memset(node->value, 0, GPR_TREE_NODE_VALUE_LEN + 1);
    node->child = NULL;
    node->sibling = NULL;
}

void gpr_tree_free_node(struct gpr_tnode *node)
{
#ifdef DEBUG
    /* Check consistency */
    if (node == NULL)
        return;
#endif

    /* Free tree node */
    free(node);
}

void gpr_tree_free(struct gpr_tnode *origin)
{
#ifdef DEBUG
    /* Check consistency */
    if (origin == NULL)
        return;
#endif

    /* Free siblings */
    if (origin->sibling != NULL)
        gpr_tree_free(origin->sibling);

    /* Free children */
    if (origin->child != NULL)
        gpr_tree_free(origin->child);

    gpr_tree_free_node(origin);
}

enum GPR_Err gpr_tree_add_child(struct gpr_tnode *origin, struct gpr_tnode *child)
{
    struct gpr_tnode *scout = NULL;

#ifdef DEBUG
    /* Check consistency */
    if (origin == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid origin node");

    if (child == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid child node");
#endif

    // Loop detection
    if (is_contained(child, origin))
        return gpr_err_raise(GPR_ERR_LOOP_DETECTED, "Child node already contains origin node");

    /* Associate tree node as a child */
    if (origin->child != NULL)
    {
        // Searching last child node
        scout = origin->child;
        while (scout->sibling != NULL)
            scout = scout->sibling;
        scout->sibling = child;
    }
    else
    {
        // First child
        origin->child = child;
    }

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

enum GPR_Err gpr_tree_add_sibling(struct gpr_tnode *origin, struct gpr_tnode *sibling)
{
    struct gpr_tnode *scout = NULL;

#ifdef DEBUG
    /* Check consistency */
    if (origin == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid origin node");

    if (sibling == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid sibling node");
#endif

    // Loop detection
    if (is_contained(sibling, origin))
        return gpr_err_raise(GPR_ERR_LOOP_DETECTED, "Sibling node already contains origin node");

    /* Associate tree node as a sibling */
    if (origin->sibling != NULL)
    {
        // Searching last sibling node
        scout = origin->sibling;
        while (scout->sibling != NULL)
            scout = scout->sibling;
        scout->sibling = sibling;
    }
    else
    {
        // First child
        origin->sibling = sibling;
    }

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

enum GPR_Err gpr_tree_set_node_value(struct gpr_tnode *node, const char *value, ...)
{
    va_list list;

#ifdef DEBUG
    /* Check consistency */
    if (node == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid node");

    if (value == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid value");
#endif

    /* Set node value */
    va_start(list, value);
    VSCNPRINTF(node->value, GPR_TREE_NODE_VALUE_LEN + 1, value, list);
    va_end(list);

    return gpr_err_raise(GPR_ERR_OK, NULL);
}

struct gpr_tnode *gpr_tree_search_node_by_label(struct gpr_tnode *origin, const char *label)
{
    return search_by_node_component(origin, label, SEARCH_BY_NODE_LABEL);
}

struct gpr_tnode *gpr_tree_search_node_by_value(struct gpr_tnode *origin, const char *value)
{
    return search_by_node_component(origin, value, SEARCH_BY_NODE_VALUE);
}

enum GPR_Err gpt_tree_convert_to_xml(unsigned char *UNUSED(out), struct gpr_tnode *UNUSED(origin), size_t UNUSED(size))
{
    return gpr_err_raise(GPR_ERR_NOT_IMPLEMENTED, "Feature not currently available");
}

/**
 * \brief Checks if the tree path from \p node1 (included) contains \p node2
 *
 * \note This function performs a vertical search of the tree path (children first)
 *
 * \param[in] node1 Node from where to explore tree path
 * \param[in] node2 Node to look at
 *
 * \return returns \c true if the tree path from \p node1 (included) contains \p node2,
 *         \c false otherwise
 */
static bool is_contained(struct gpr_tnode *node1, struct gpr_tnode *node2)
{
#ifdef DEBUG
    /* Check consistency */
    if ((node1 == NULL) || (node2 == NULL))
        return false;
#endif

    if (node1 == node2)
        return true;

    // TODO: Allow this function to perform horizontal or vertical search
    if (node1->child != NULL)
        if (is_contained(node1->child, node2))
            return true;

    if (node1->sibling != NULL)
        if (is_contained(node1->sibling, node2))
            return true;

    return false;
}

/**
 * \brief Searches the first occurence of a node that match one of its components
 *        from an \p origin node
 *
 * \note This function performs a vertical search of the tree path (children first)
 *
 * \param[in] origin  Origin node from where to explore tree path
 * \param[in] match   C-string to search for
 * \param[in] pattern Searching pattern to perform
 *
 * \return Returns a pointer to the first occurence of the node that match one of its
 *     components or a \c NULL pointer if no matching node found
 */
static struct gpr_tnode *search_by_node_component(struct gpr_tnode *origin, const char *match, enum Search_By_Node pattern)
{
    struct gpr_tnode *scout = NULL;

#ifdef DEBUG
    /* Check consistency */
    if ((origin == NULL) || (match == NULL) || (pattern >= SEARCH_BY_NODE_UNKNOWN))
        return false;
#endif

    switch (pattern)
    {
        case SEARCH_BY_NODE_LABEL:
            if (strncmp(origin->label, match, GPR_TREE_NODE_LABEL_LEN + 1) == 0)
                return origin;
            break;

        case SEARCH_BY_NODE_VALUE:
            if (strncmp(origin->value, match, GPR_TREE_NODE_VALUE_LEN + 1) == 0)
                return origin;
            break;

        default:
            return NULL;
    }

    // TODO: Allow this function to perform horizontal or vertical search
    if (origin->child != NULL)
    {
        scout = search_by_node_component(origin->child, match, pattern);
        if (scout != NULL)
            return scout;
    }

    if (origin->sibling != NULL)
    {
        scout = search_by_node_component(origin->sibling, match, pattern);
        if (scout != NULL)
            return scout;
    }

    return NULL;
}
