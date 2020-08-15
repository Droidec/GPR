/*****************************************************************************
 *
 * gpr.h
 *
 *****************************************************************************
 * Copyright © 2020 Marc GIANNETTI
 *
 * Library management macros
 *
 * This module offers generic macros for initializing/freeing functionnalities
 * provided by other GPR modules
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

#ifndef H_GPR
#define H_GPR

#include "gpr_err.h"

// Initialize error module
#define GPR_INIT_ERR gpr_err_allocate_cmpl_err();

// Free error module
#define GPR_FREE_ERR gpr_err_free_cmpl_err();

// Initialize all modules
#define GPR_INIT_LIBRARY GPR_INIT_ERR

// Free all modules
#define GPR_FREE_LIBRARY GPR_FREE_ERR

#endif /* H_GPR */
