/******************************************************************************
 *
 * gpr_ut_log.c
 *
 ******************************************************************************
 *
 * Unit tests on "gpr_log" module
 *
 *****************************************************************************/

#include "gpr_log.h"

#include <stdio.h>
#include <assert.h>

int main(int argc, char *argv[])
{
    GPR_ALLOC_ERR_MODULE

    enum GPR_Err err;
    ssize_t written_len;

    if (argc > 1)
    {
        err = gpr_log_configure(argv[1], GPR_LOG_DEFAULT);
        if (err != GPR_ERR_OK)
        {
            fprintf(stderr, "gpr_log_configure(): %s (%s)\n", gpr_err_to_str(err), gpr_err_get_cmpl_err());
            GPR_FREE_ERR_MODULE
            return 1;
        }
    }

    // Default behavior
    written_len = GPR_LOG_MSG(GPR_LOG_DEBUG, "This is a DEBUG log message"); // Will not be displayed
    assert(written_len == 0);
    written_len = GPR_LOG_MSG(GPR_LOG_INFO, "This is an INFO log message");
    assert(written_len > 0);
    written_len = GPR_LOG_MSG(GPR_LOG_WARNING, "This is a WARNING log message");
    assert(written_len > 0);
    written_len = GPR_LOG_MSG(GPR_LOG_ERROR, "This is an ERROR log message");
    assert(written_len > 0);
    written_len = GPR_LOG_MSG(GPR_LOG_CRITICAL, "This is a CRITICAL log message");
    assert(written_len > 0);

    puts("Changing default log level to DEBUG");

    err = gpr_log_configure(NULL, GPR_LOG_DEBUG);
    if (err != GPR_ERR_OK)
    {
        fprintf(stderr, "gpr_log_configure(): %s (%s)\n", gpr_err_to_str(err), gpr_err_get_cmpl_err());
        GPR_FREE_ERR_MODULE
        return 1;
    }

    // Debug behavior
    written_len = GPR_LOG_DEBUG("This is a DEBUG log message");
    assert(written_len > 0);
    written_len = GPR_LOG_INFO("This is an INFO log message");
    assert(written_len > 0);
    written_len = GPR_LOG_WARN("This is a WARNING log message");
    assert(written_len > 0);
    written_len = GPR_LOG_ERR("This is an ERROR log message");
    assert(written_len > 0);
    written_len = GPR_LOG_CRIT("This is a CRITICAL log message");
    assert(written_len > 0);

    GPR_FREE_ERR_MODULE

    return 0;
}
