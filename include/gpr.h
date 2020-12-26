/******************************************************************************
*
* gpr.h
*
*******************************************************************************
* Copyright © 2020 Marc GIANNETTI
*
* Library management macros
*
* This module offers generic macros for initializing/freeing functionnalities
* provided by GPR modules and include all of them
*
*******************************************************************************
*
* Copyright (c) 2019-2021, GPR Team
* All rights reserved.
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* * Neither the name of the GPR Team nor the names of its contributors may
*   be used to endorse or promote products derived from this software without
*   specific prior written permission.
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
******************************************************************************/

#ifndef H_GPR
#define H_GPR

#include "gpr_builtin.h"
#include "gpr_err.h"
#include "gpr_time.h"
#include "gpr_log.h"
#include "gpr_network.h"
#include "gpr_dlklist.h"

// Initialize all modules
#define GPR_INIT_LIBRARY GPR_INIT_ERR_MODULE

// Free all modules
#define GPR_FREE_LIBRARY GPR_FREE_ERR_MODULE

#endif /* H_GPR */
