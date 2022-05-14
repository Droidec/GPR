/******************************************************************************
 *
 * \file gpr_tree.h
 * \brief Tree module
 * \details
 * This module defines a n-ary tree composed of nodes\n
 * Each node has a pointer on the first of its children (vertical direction)
 * and siblings (horizontal direction)\n
 * Data from each node is a pair composed of a label and a value\n
 * The purpose of this structure is to prioritize the data, so that it can
 * then be easily transformed into a markup language
 *
 * \verbatim
 *
 * ┌──────┐
 * │ node │
 * └──┼───┘ ┌──────┐ ┌──────┐ ┌──────┐
 *    └─────┼ node ┼─┼ node ┼─┼ node │
 *          └──┼───┘ └──┼───┘ └──┼───┘ ┌──────┐ ┌──────┐ ┌──────┐ ┌──────┐
 *             │        │        └─────┼ node ┼─┼ node ┼─┼ node ┼─┼ node │
 *             │        │              └──────┘ └──────┘ └──────┘ └──────┘
 *             │        │     ┌──────┐
 *             │        └─────┼ node │
 *             │              └──────┘
 *             │     ┌──────┐ ┌──────┐
 *             └─────┼ node ┼─┼ node │
 *                   └──────┘ └──────┘
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

#ifndef H_GPR_TREE
#define H_GPR_TREE

#include <stddef.h> // size_t

#include "gpr_err.h"

/**
 * \brief Number of bytes for a tree node label
 */
#define GPR_TREE_NODE_LABEL_LEN 128

/**
 * \brief Number of bytes for a tree node value
 */
#define GPR_TREE_NODE_VALUE_LEN 256

/**
 * \brief Tree component structure
 */
struct gpr_tnode
{
    char label[GPR_TREE_NODE_LABEL_LEN + 1]; ///< Node label
    char value[GPR_TREE_NODE_VALUE_LEN + 1]; ///< Node value
    struct gpr_tnode *child;                 ///< Next child
    struct gpr_tnode *sibling;               ///< Next sibling
};

/******************************************************************************
 *
 * \brief Allocate and initialize a new tree node with a label but no value
 *
 * \warning The label can't exceed GPR_TREE_NODE_LABEL_LEN bytes, otherwise it
 * will be truncated by this function
 *
 * \param label Node's label to set as a C-string
 *
 * \return
 *     On success, returns a pointer to the tree node\n
 *     On failure, returns NULL
 *
 *****************************************************************************/
struct gpr_tnode *gpr_tree_new_node(const char *label);

/******************************************************************************
 *
 * \brief Allocate and initialize a new tree node with a label and a value
 *
 * \note A call to this function is equivalent to a call of gpr_tree_new_node
 * and gpr_tree_set_node_value, in that order
 *
 * \warning The label can't exceed GPR_TREE_NODE_LABEL_LEN bytes, otherwise it
 * will be truncated by this function
 *
 * \warning The value can't exceed GPR_TREE_NODE_VALUE_LEN bytes, otherwise it
 * will be truncated by this function
 *
 * \param label Node's label to set as a C-string
 * \param value Node's value to set as a C-string
 * \param ...   Optional arguments
 *
 * \return
 *     On success, returns a pointer to the tree node\n
 *     On failure, returns NULL
 *
 *****************************************************************************/
struct gpr_tnode *gpr_tree_new_vnode(const char *label, const char *value, ...) __attribute__((format(printf, 2, 3)));

/******************************************************************************
 *
 * \brief Allocate and initialize a new tree node with a label but no value,
 * and associate it to an \a origin node as a child
 *
 * \warning The label can't exceed GPR_TREE_NODE_LABEL_LEN bytes, otherwise it
 * will be truncated by this function
 *
 * \param origin Origin node to be associated with
 * \param label Node's label to set for child node as a C-string
 *
 * \return
 *     On success, returns a pointer to the child node\n
 *     On failure, returns NULL
 *
 *****************************************************************************/
struct gpr_tnode *gpr_tree_new_child(struct gpr_tnode *origin, const char *label);

/******************************************************************************
 *
 * \brief Allocate and initialize a new tree node with a label and a value,
 * and associate it to an \a origin node as a child
 *
 * \note A call to this function is equivalent to a call of gpr_tree_new_child
 * and gpr_tree_set_node_value, in that order
 *
 * \warning The label can't exceed GPR_TREE_NODE_LABEL_LEN bytes, otherwise it
 * will be truncated by this function
 *
 * \warning The value can't exceed GPR_TREE_NODE_VALUE_LEN bytes, otherwise it
 * will be truncated by this function
 *
 * \param origin Origin node to be associated with
 * \param label  Node's label to set for child node as a C-string
 * \param value  Node's value to set for child node as a C-string
 * \param ...    Optional arguments
 *
 * \return
 *     On success, returns a pointer to the child node\n
 *     On failure, returns NULL
 *
 *****************************************************************************/
struct gpr_tnode *gpr_tree_new_vchild(struct gpr_tnode *origin, const char *label, const char *value, ...)
    __attribute__((format(printf, 3, 4)));

/******************************************************************************
 *
 * \brief Allocate and initialize a new tree node with a label but no value,
 * and associate it to an \a origin node as a sibling
 *
 * \warning The label can't exceed GPR_TREE_NODE_LABEL_LEN bytes, otherwise it
 * will be truncated by this function
 *
 * \param origin Origin node to be associated with
 * \param label Node's label to set for sibling node as a C-string
 *
 * \return
 *     On success, returns a pointer to the sibling node\n
 *     On failure, returns NULL
 *
 *****************************************************************************/
struct gpr_tnode *gpr_tree_new_sibling(struct gpr_tnode *origin, const char *label);

/******************************************************************************
 *
 * \brief Allocate and initialize a new tree node with a label and a value,
 * and associate it to an \a origin node as a sibling
 *
 * \note A call to this function is equivalent to a call of
 * gpr_tree_new_sibling and gpr_tree_set_node_value, in that order
 *
 * \warning The label can't exceed GPR_TREE_NODE_LABEL_LEN bytes, otherwise it
 * will be truncated by this function
 *
 * \warning The value can't exceed GPR_TREE_NODE_VALUE_LEN bytes, otherwise it
 * will be truncated by this function
 *
 * \param origin Origin node to be associated with
 * \param label  Node's label to set for sibling node as a C-string
 * \param value  Node's value to set for sibling node as a C-string
 * \param ...    Optional arguments
 *
 * \return
 *     On success, returns a pointer to the sibling node\n
 *     On failure, returns NULL
 *
 *****************************************************************************/
struct gpr_tnode *gpr_tree_new_vsibling(struct gpr_tnode *origin, const char *label, const char *value, ...)
    __attribute__((format(printf, 3, 4)));

/******************************************************************************
 *
 * \brief Initialize tree node components
 *
 * \param node Tree node to initialize
 *
 *****************************************************************************/
void gpr_tree_init_node(struct gpr_tnode *node);

/******************************************************************************
 *
 * \brief Free a tree node
 *
 * \param node Tree node to free
 *
 *****************************************************************************/
void gpr_tree_free_node(struct gpr_tnode *node);

/******************************************************************************
 *
 * \brief Free a tree node and all its associated tree nodes
 *
 * \note This function performs a vertical free of the tree path
 * (siblings first)
 *
 * \param origin Origin node of the tree to free
 *
 *****************************************************************************/
void gpr_tree_free(struct gpr_tnode *origin);

/******************************************************************************
 *
 * \brief Add a \a child node to an \a origin node
 *
 * \param origin Origin node to be associated with
 * \param child  Child node to add
 *
 * \return
 *     GPR_ERR_OK: The child node has been associated to the origin node\n
 *     GPR_ERR_INVALID_PARAMETER: The origin node is NULL or the child node
 *     is NULL (DEBUG mode only)\n
 *     GPR_ERR_LOOP_DETECTED: The child node already contains the origin node
 *
 *****************************************************************************/
enum GPR_Err gpr_tree_add_child(struct gpr_tnode *origin, struct gpr_tnode *child);

/******************************************************************************
 *
 * \brief Add a \a sibling node to an \a origin node
 *
 * \param origin  Origin node to be associated with
 * \param sibling Sibling node to add
 *
 * \return
 *     GPR_ERR_OK: The sibling node has been associated to the origin node\n
 *     GPR_ERR_INVALID_PARAMETER: The origin node is NULL or the sibling node
 *     is NULL (DEBUG mode only)\n
 *     GPR_ERR_LOOP_DETECTED: The sibling node already contains the origin node
 *
 *****************************************************************************/
enum GPR_Err gpr_tree_add_sibling(struct gpr_tnode *origin, struct gpr_tnode *sibling);

/******************************************************************************
 *
 * \brief Set \a node value
 *
 * \warning The value can't exceed GPR_TREE_NODE_VALUE_LEN bytes, otherwise it
 * will be truncated by this function
 *
 * \param node  Node to be set
 * \param value Node's value to set as a C-string
 *
 * \return
 *     GPR_ERR_OK: The node value has been set\n
 *     GPR_ERR_INVALID_PARAMETER: The node is NULL or the value is NULL
 *     (DEBUG mode only)
 *
 *****************************************************************************/
enum GPR_Err gpr_tree_set_node_value(struct gpr_tnode *node, const char *value, ...) __attribute__((format(printf, 2, 3)));

/******************************************************************************
 *
 * \brief Search the first occurence of a node, according to its label, from
 * an \a origin node
 *
 * \note This function performs a vertical search of the tree path
 * (children first)
 *
 * \param origin Origin node from where to explore tree path
 * \param label  Node's label to search for
 *
 * \return
 *     A pointer to the first occurence of the node that match with the label
 *     A NULL pointer if no matching node found
 *
 *****************************************************************************/
struct gpr_tnode *gpr_tree_search_node_by_label(struct gpr_tnode *origin, const char *label);

/******************************************************************************
 *
 * \brief Search the first occurence of a node, according to its value, from
 * an \a origin node
 *
 * \note This function performs a vertical search of the tree path
 * (children first)
 *
 * \param origin Origin node from where to explore tree path
 * \param value  Node's value to search for
 *
 * \return
 *     A pointer to the first occurence of the node that match with the value
 *     A NULL pointer if no matching node found
 *
 *****************************************************************************/
struct gpr_tnode *gpr_tree_search_node_by_value(struct gpr_tnode *origin, const char *value);

/******************************************************************************
 *
 * \brief Convert a tree node and all its associated nodes in an XML byte
 * array.
 *
 * \param out    Byte array to place the result into
 * \param origin Origin node from where to start conversion
 * \param size   Number of bytes available in the byte array
 *
 * \return
 *     GPR_ERR_NOT_IMPLEMENTED: Feature not currently available (Should
 *     return the number of bytes written in the XML byte array)
 *
 *****************************************************************************/
enum GPR_Err gpt_tree_convert_to_xml(unsigned char *out, struct gpr_tnode *origin, size_t size);

/**
 * \brief Handle failure of tree parts\n
 * If \a node is NULL, the tree path starting from
 * the \a origin node is freed and the macro returns NULL
 *
 * \param node   Node to check
 * \param origin Origin node of the tree to free
 */
#define FREE_AND_RETURN_ON_TREE_FAILURE(node, origin) \
    do                                                \
    {                                                 \
        if (node == NULL)                             \
        {                                             \
            gpr_tree_free(origin);                    \
            return NULL;                              \
        }                                             \
    } while (0)

#endif /* H_GPR_TREE */
