/******************************************************************************
 *
 * \file gpr_list.c
 * \brief Circular doubly linked list
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

#include "gpr_list.h"

#include <stdio.h>

/******************************************************************************
 * Private prototypes
 *****************************************************************************/

static enum GPR_Err list_add_entry(struct gpr_list *new, struct gpr_list *prev, struct gpr_list *next);

/******************************************************************************
 * Public functions
 *****************************************************************************/

void gpr_list_init(struct gpr_list *list)
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

enum GPR_Err gpr_list_push_front(struct gpr_list *new, struct gpr_list *head)
{
#ifdef DEBUG
    /* Check consistency */
    if (new == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid new entry");

    if (head == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid list head");
#endif

    /* Add entry */
    return list_add_entry(new, head, head->next);
}

enum GPR_Err gpr_list_push_back(struct gpr_list *new, struct gpr_list *head)
{
#ifdef DEBUG
    /* Check consistency */
    if (new == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid new entry");

    if (head == NULL)
        return gpr_err_raise(GPR_ERR_INVALID_PARAMETER, "Invalid list head");
#endif

    /* Add entry */
    return list_add_entry(new, head, head->prev);
}

/******************************************************************************
 * Private functions
 *****************************************************************************/

/******************************************************************************
 *
 * \brief Add a new entry according to previous and next entry
 *
 * \param new  New entry to be added
 * \param prev Previous entry of the new one
 * \param next Next entry of the new one
 *
 * \return
 *     GPR_ERR_OK: The entry has correctly been added\n
 *     GPR_ERR_INVALID_PARAMETER: The new entry is NULL, the previous entry is
 *     NULL or the next entry is NULL (DEBUG mode only)\n
 *
 *****************************************************************************/
static enum GPR_Err list_add_entry(struct gpr_list *new, struct gpr_list *prev, struct gpr_list *next)
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
