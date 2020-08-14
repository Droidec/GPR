/*****************************************************************************
 *
 * gpr_ut_log.c
 *
 *****************************************************************************
 *
 * Unit tests on "gpr_log" module
 *
 *****************************************************************************/

#include <GPR/gpr.h>
#include <GPR/gpr_log.h>

#include <stdio.h>

int main(int argc, char *argv[])
{
    GPR_DEFAULT_INIT

    enum GPR_Err err;

    if (argc > 1)
    {
        err = gpr_log_configure(argv[1], GPR_LOG_DEFAULT);
        if (err != GPR_ERR_OK)
        {
            fprintf(stderr, "gpr_log_configure(): %s (%s)\n", gpr_err_to_str(err), gpr_err_get_cmpl_err());
            GPR_FREE_LIBRARY
            return 1;
        }
    }

    // Default behavior
    GPR_LOG_MSG(GPR_LOG_DEBUG, "This is a DEBUG log message"); // Will not be displayed
    GPR_LOG_MSG(GPR_LOG_INFO, "This is an INFO log message");
    GPR_LOG_MSG(GPR_LOG_WARNING, "This is a WARNING log message");
    GPR_LOG_MSG(GPR_LOG_ERROR, "This is an ERROR log message");
    GPR_LOG_MSG(GPR_LOG_CRITICAL, "This is a CRITICAL log message");

    puts("Changing default log level to DEBUG\n");

    err = gpr_log_configure(NULL, GPR_LOG_DEBUG);
    if (err != GPR_ERR_OK)
    {
        fprintf(stderr, "gpr_log_configure(): %s (%s)\n", gpr_err_to_str(err), gpr_err_get_cmpl_err());
        GPR_FREE_LIBRARY
        return 1;
    }

    // Debug behavior
    GPR_LOG_MSG(GPR_LOG_DEBUG, "This is a DEBUG log message");
    GPR_LOG_MSG(GPR_LOG_INFO, "This is an INFO log message");
    GPR_LOG_MSG(GPR_LOG_WARNING, "This is a WARNING log message");
    GPR_LOG_MSG(GPR_LOG_ERROR, "This is an ERROR log message");
    GPR_LOG_MSG(GPR_LOG_CRITICAL, "This is a CRITICAL log message");

    GPR_FREE_LIBRARY

    return 0;
}
