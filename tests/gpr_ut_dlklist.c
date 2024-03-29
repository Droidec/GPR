/******************************************************************************
 *
 * gpr_ut_dlklist.c
 *
 ******************************************************************************
 *
 * Unit tests on "gpr_dlklist" module
 *
 *****************************************************************************/

#include "gpr_dlklist.h"

#include <stdio.h>  // printf
#include <stdlib.h> // malloc, free
#include <string.h> // memset, strncmp

#include "gpr_utils.h"

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
    struct gpr_dlklist *list = NULL;
    struct gpr_dlknode *node = NULL;
    enum GPR_Err err;
    size_t pos = 0;

    /* New list */
    list = gpr_dlklist_new();
    ASSERT(list != NULL);

    /* New students */
    struct student *st0 = new_student("Valentin", 0);
    struct student *st1 = new_student("Alfred", 20);
    struct student *st2 = new_student("Julius", 8);
    struct student *st3 = new_student("Isabelle", 14);
    struct student *st4 = new_student("Sophie", 17);
    struct student *st5 = new_student("Dominique", 2);

    /* Add students to list */
    printf("---List of students---\n");
    err = gpr_dlklist_push_back(list, st0);
    ASSERT(err == GPR_ERR_OK);
    err = gpr_dlklist_push_back(list, st1);
    ASSERT(err == GPR_ERR_OK);
    err = gpr_dlklist_push_front(list, st2);
    ASSERT(err == GPR_ERR_OK);
    err = gpr_dlklist_push_front(list, st3);
    ASSERT(err == GPR_ERR_OK);
    err = gpr_dlklist_insert(list, st4, 2);
    ASSERT(err == GPR_ERR_OK);
    err = gpr_dlklist_push_front(list, st5);
    ASSERT(err == GPR_ERR_OK);

    /* Display list */
    gpr_dlklist_map(list, print_student);

    /* Search in list */
    printf("\n---Searching students---\n");
    node = gpr_dlklist_search(list, search_student_by_name, "Julius", &pos);
    ASSERT(node != NULL);
    ASSERT(pos == 2);
    printf("Julius has been found at position %zu\n", pos);

    /* Delete in list */
    printf("\n---Removing students---\n");
    err = gpr_dlklist_pop_front(list, free_student);
    ASSERT(err == GPR_ERR_OK);
    err = gpr_dlklist_pop_back(list, free_student);
    ASSERT(err == GPR_ERR_OK);
    err = gpr_dlklist_remove(list, free_student, 2);
    ASSERT(err == GPR_ERR_OK);
    gpr_dlklist_map(list, print_student);

    /* Replace in list */
    printf("\n---Replacing students---\n");
    struct student *st6 = new_student("Caroline", 1);
    err = gpr_dlklist_replace(list, free_student, st6, 1);
    ASSERT(err == GPR_ERR_OK);

    /* Display list */
    struct student *st = NULL;
    GPR_DLKLIST_FOR_EACH_ENTRY (node, list, st)
        print_student(st);

    /* Free list */
    gpr_dlklist_free(list, free_student);

    return 0;
}
