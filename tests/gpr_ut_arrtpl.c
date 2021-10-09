/******************************************************************************
 *
 * gpr_ut_arrtpl.c
 *
 ******************************************************************************
 *
 * Unit tests on "gpr_arrtpl" module
 *
 *****************************************************************************/

#include "gpr_utils.h"

#include <stdio.h>  // printf
#include <stdlib.h> // malloc, free
#include <string.h> // memset
#include <assert.h> // assert

#define STUDENT_NAME_MAX_LEN 20

struct student
{
    char name[STUDENT_NAME_MAX_LEN + 1];
    unsigned int mark;
};

struct student *new_student(char *name, unsigned int mark)
{
    struct student *st = NULL;

    /* Allocate student */
    st = (struct student *)malloc(sizeof(struct student));
    if (UNLIKELY(st == NULL))
        return NULL;

    /* Initialize student */
    memset(st->name, 0, STUDENT_NAME_MAX_LEN + 1);
    SCNPRINTF(st->name, STUDENT_NAME_MAX_LEN + 1, "%s", name);
    st->mark = mark;

    return st;
}

void free_student(struct student *st)
{
    if (st == NULL)
        return;

    free(st);
}

void print_student(struct student *st)
{
    printf("%s - %u\n", st->name, st->mark);
}

#define GPR_ARRTPL_TYPE       struct student
#define GPR_ARRTPL_ST         student_array
#define GPR_ARRTPL_FUNC(func) gpr_arrtpl_##func

#include "gpr_arrtpl.h"

int main()
{
    struct student_array arr;
    bool err;

    /* New array */
    err = gpr_arrtpl_init(&arr, 5);
    assert(err);

    /* Free array */
    gpr_arrtpl_free(&arr);

    return 0;
}
