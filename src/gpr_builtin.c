/*****************************************************************************
 *
 * gpr_builtin.c
 *
 *****************************************************************************
 * Copyright © 2020 Marc GIANNETTI
 *
 * Builtin macros/functions
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

#include "gpr_builtin.h"

/*****************************************************************************
 * Public functions
 *****************************************************************************/

int gpr_builtin_vscnprintf(char *buf, size_t size, const char *fmt, va_list args)
{
    int i;

    i = vsnprintf(buf, size, fmt, args);

    if (LIKELY(i < (ssize_t)size))
        return i;

    if (size != 0)
        return size - 1;

    return 0;
}

int gpr_builtin_scnprintf(char *buf, size_t size, const char *fmt, ...)
{
    va_list args;
    int i;

    va_start(args, fmt);
    i = gpr_builtin_vscnprintf(buf, size, fmt, args);
    va_end(args);

    return i;
}
