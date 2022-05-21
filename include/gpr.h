/******************************************************************************
 *
 * \file gpr.h
 * \brief Library management
 * \details This module includes all the other modules and offers the
 * possibility to initialize the library in one go
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

#ifndef H_GPR
#define H_GPR

#include "gpr_array.h"
#include "gpr_bin.h"
#include "gpr_bit.h"
#include "gpr_buf.h"
#include "gpr_dlklist.h"
#include "gpr_err.h"
#include "gpr_klist.h"
#include "gpr_log.h"
#include "gpr_net.h"
#include "gpr_str.h"
#include "gpr_time.h"
#include "gpr_tree.h"
#include "gpr_utils.h"

/**
 * \brief Prints compilation informations of caller
 */
static inline void gpr_print_conf(void)
{
    /* C standard */
#ifdef __STDC_VERSION__
    printf("C standard: %ld\n", __STDC_VERSION__);
#else
    puts("C89/C90 standard");
#endif

    /* GNU extensions */
#ifdef _GNU_SOURCE
    puts("GNU extensions available");
#else
    puts("GNU extensions not available");
#endif

    /* Compilation mode */
#ifdef DEBUG
    puts("DEBUG mode enabled");
#else
    /* N.B. By default, cmake add -DNDEBUG in release mode,
     * which disable debug functionnalities such as assert */
    puts("RELEASE mode enabled");
#endif
}

#endif /* H_GPR */
