/******************************************************************************
 *
 * gpr_ut_buf.c
 *
 ******************************************************************************
 *
 * Unit tests on "gpr_buf" module
 *
 *****************************************************************************/

#include "gpr_buf.h"

#include <assert.h> // assert
#include <stdio.h>  // printf
#include <string.h> // strlen, memcpy

#define BUFFER_SIZE 20

int main()
{
    struct gpr_buffer *buf = NULL;
    const char *string = "An incredible string";
    size_t string_len = strlen(string);

    buf = gpr_buf_new(BUFFER_SIZE);
    assert(buf != NULL);
    assert(string_len <= gpr_buf_get_free_size(buf));

    printf("Buffer size: %u bytes\n", gpr_buf_get_size(buf));
    printf("Buffer free size: %u bytes\n", gpr_buf_get_free_size(buf));
    printf("Buffer used size: %u bytes\n", gpr_buf_get_used_size(buf));
    printf("Buffer rest size: %u bytes\n", gpr_buf_get_rest_size(buf));
    printf("Buffer ofs_b: %p\n", buf->ofs_b);
    printf("Buffer ofs_e: %p\n", buf->ofs_e);
    printf("Buffer ofs_d: %p\n\n", buf->ofs_d);

    memcpy(buf->ofs_b, string, string_len);
    buf->ofs_e += string_len;

    printf("Buffer content: %.*s\n", gpr_buf_get_used_size(buf), buf->ofs_b);
    printf("Buffer size: %u bytes\n", gpr_buf_get_size(buf));
    printf("Buffer free size: %u bytes\n", gpr_buf_get_free_size(buf));
    printf("Buffer used size: %u bytes\n", gpr_buf_get_used_size(buf));
    printf("Buffer rest size: %u bytes\n", gpr_buf_get_rest_size(buf));
    printf("Buffer ofs_b: %p\n", buf->ofs_b);
    printf("Buffer ofs_e: %p\n", buf->ofs_e);
    printf("Buffer ofs_d: %p\n\n", buf->ofs_d);

    gpr_buf_free(buf);

    return 0;
}
