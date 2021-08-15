/******************************************************************************
 *
 * gpr_ut_array.c
 *
 ******************************************************************************
 *
 * Unit tests on "gpr_array" module
 *
 *****************************************************************************/

#include "gpr_array.h"
#include "gpr_builtin.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define STUDENT_NAME_MAX_LENGTH 20

struct student
{
    char name[STUDENT_NAME_MAX_LENGTH + 1];
    unsigned int mark;
};

struct student *create_student(char *name, unsigned int mark)
{
    struct student *st = NULL;

    /* Allocate student */
    st = (struct student *)malloc(sizeof(struct student));
    if (UNLIKELY(st == NULL))
        return NULL;

    /* Initialize student */
    memset(st->name, 0, STUDENT_NAME_MAX_LENGTH + 1);
    SCNPRINTF(st->name, STUDENT_NAME_MAX_LENGTH + 1, "%s", name);
    st->mark = mark;

    return st;
}

void free_student(struct student *st)
{
    if (st == NULL)
        return;

    free(st);
    return;
}

void print_student(struct student *st)
{
    printf("%s - %u\n", st->name, st->mark);
    return;
}

bool search_student_by_name(const struct student *st, const char *name)
{
    if (strncmp(st->name, name, STUDENT_NAME_MAX_LENGTH) == 0)
        return true;

    return false;
}

int main()
{
    GPR_ALLOC_ERR_MODULE

    struct gpr_array *arr = NULL;
    void *elem = NULL;
    enum GPR_Err err;
    size_t index = 0;

    /* Create array */
    arr = gpr_array_create();
    assert(arr != NULL);

    /* Create students */
    struct student *st0 = create_student("Valentin", 0);
    struct student *st1 = create_student("Alfred", 20);
    struct student *st2 = create_student("Julius", 8);
    struct student *st3 = create_student("Isabelle", 14);
    struct student *st4 = create_student("Sophie", 17);
    struct student *st5 = create_student("Dominique", 2);

    /* Add students to array */
    printf("---Array of students---\n");
    err = gpr_array_push_back(arr, st0);
    assert(err == GPR_ERR_OK);
    err = gpr_array_push_back(arr, st1);
    assert(err == GPR_ERR_OK);
    err = gpr_array_push_front(arr, st2);
    assert(err == GPR_ERR_OK);
    err = gpr_array_push_front(arr, st3);
    assert(err == GPR_ERR_OK);
    err = gpr_array_insert(arr, st4, 2);
    assert(err == GPR_ERR_OK);
    err = gpr_array_push_front(arr, st5);
    assert(err == GPR_ERR_OK);

    /* Display array */
    gpr_array_map(arr, print_student);

    /* Search in array */
    printf("\n---Searching students---\n");
    elem = gpr_array_search(arr, search_student_by_name, "Julius", &index);
    assert(elem != NULL);
    assert(index == 2);
    printf("Julius has been found at position %zu\n", index);

    /* Delete in array */
    printf("\n---Removing students---\n");
    err = gpr_array_pop_front(arr, free_student);
    assert(err == GPR_ERR_OK);
    err = gpr_array_pop_back(arr, free_student);
    assert(err == GPR_ERR_OK);
    err = gpr_array_remove(arr, free_student, 2);
    assert(err == GPR_ERR_OK);
    gpr_array_map(arr, print_student);

    /* Replace in array */
    printf("\n---Replacing students---\n");
    struct student *st6 = create_student("Caroline", 1);
    err = gpr_array_replace(arr, free_student, st6, 1);
    assert(err == GPR_ERR_OK);
    gpr_array_map(arr, print_student);

    /* Free array */
    gpr_array_free(arr, free_student);

    GPR_FREE_ERR_MODULE

    return 0;
}
