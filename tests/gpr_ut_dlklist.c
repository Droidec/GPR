/*****************************************************************************
 *
 * gpr_ut_dlklist.c
 *
 *****************************************************************************
 *
 * Unit tests on "gpr_dlklist" module
 *
 *****************************************************************************/

#include <GPR/gpr_builtin.h>
#include <GPR/gpr_dlklist.h>

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
    GPR_INIT_ERR_MODULE

    struct gpr_dlklist *list = NULL;
    struct gpr_dlknode *node = NULL;
    enum GPR_Err err;
    size_t pos = 0;

    /* Create list */
    list = gpr_dlklist_create();
    assert(list != NULL);

    /* Create students */
    struct student *st0 = create_student("Valentin", 0);
    struct student *st1 = create_student("Alfred", 20);
    struct student *st2 = create_student("Julius", 8);
    struct student *st3 = create_student("Isabelle", 14);
    struct student *st4 = create_student("Sophie", 17);
    struct student *st5 = create_student("Dominique", 2);

    /* Add students to list */
    err = gpr_dlklist_push_back(list, st0);
    assert(err == GPR_ERR_OK);
    err = gpr_dlklist_push_back(list, st1);
    assert(err == GPR_ERR_OK);
    err = gpr_dlklist_push_front(list, st2);
    assert(err == GPR_ERR_OK);
    err = gpr_dlklist_push_front(list, st3);
    assert(err == GPR_ERR_OK);
    err = gpr_dlklist_insert(list, st4, 2);
    assert(err == GPR_ERR_OK);
    err = gpr_dlklist_push_front(list, st5);
    assert(err == GPR_ERR_OK);

    /* Display list */
    gpr_dlklist_map(list, print_student);

    /* Search in list */
    node = gpr_dlklist_search(list, search_student_by_name, "Julius", &pos);
    assert(node != NULL);
    assert(pos == 2);
    printf("Julius has been found at position %zu\n", pos);

    /* Free list */
    gpr_dlklist_free(list, free_student);

    GPR_FREE_ERR_MODULE

    return 0;
}
