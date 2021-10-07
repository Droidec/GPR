/******************************************************************************
 *
 * gpr_ut_arrtpl.c
 *
 ******************************************************************************
 *
 * Unit tests on "gpr_arrtpl" module
 *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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

#define GPR_ARRTPL_TYPE struct student
#define GPR_ARRTPL_ST student_array
#define GPR_ARRTPL_FUNC(func) st_arr_ ## func

#include "gpr_arrtpl.h"

int main()
{
    struct student_array arr;

    /* New array */
    arr = gpr_array_new();
    assert(arr != NULL);

    /* New students */
    struct student *st0 = new_student("Valentin", 0);
    struct student *st1 = new_student("Alfred", 20);
    struct student *st2 = new_student("Julius", 8);
    struct student *st3 = new_student("Isabelle", 14);
    struct student *st4 = new_student("Sophie", 17);
    struct student *st5 = new_student("Dominique", 2);

    /* Add students to array */


    /* Free array */
    gpr_array_free(&arr);

    return 0;
}
