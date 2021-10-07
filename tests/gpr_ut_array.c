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
#include "gpr_utils.h"

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

bool search_student_by_name(const struct student *st, const char *name)
{
    if (strncmp(st->name, name, STUDENT_NAME_MAX_LEN) == 0)
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
    struct student *st6 = new_student("Caroline", 1);
    err = gpr_array_replace(arr, free_student, st6, 1);
    assert(err == GPR_ERR_OK);
    gpr_array_map(arr, print_student);

    /* Free array */
    gpr_array_free(arr, free_student);

    GPR_FREE_ERR_MODULE

    return 0;
}
