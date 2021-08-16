/******************************************************************************
 *
 * gpr_ut_list.c
 *
 ******************************************************************************
 *
 * Unit tests on "gpr_list" module
 *
 *****************************************************************************/

#include "gpr_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUDENT_NAME_MAX_LENGTH 20

struct student
{
    char name[STUDENT_NAME_MAX_LENGTH + 1];
    unsigned int mark;
    struct gpr_list head;
};

struct student *new_student(char *name, unsigned int mark)
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
    gpr_list_init(&st->head);

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

int main()
{
    GPR_ALLOC_ERR_MODULE

    struct student *cur_student = NULL;
    struct gpr_list list;
    gpr_list_init(&list);

    /* New students */
    struct student *st0 = new_student("Valentin", 0);
    struct student *st1 = new_student("Alfred", 20);
    struct student *st2 = new_student("Julius", 8);
    struct student *st3 = new_student("Isabelle", 14);
    struct student *st4 = new_student("Sophie", 17);
    struct student *st5 = new_student("Dominique", 2);

    /* Add students */
    gpr_list_push_front(&st0->head, &list);
    gpr_list_push_front(&st1->head, &list);
    gpr_list_push_front(&st2->head, &list);
    gpr_list_push_front(&st3->head, &list);
    gpr_list_push_front(&st4->head, &list);
    gpr_list_push_front(&st5->head, &list);

    /* Print students */
    GPR_LIST_FOR_EACH_ENTRY(cur_student, &list, head)
        print_student(cur_student);

    /* Free students */
    free(st0);
    free(st1);
    free(st2);
    free(st3);
    free(st4);
    free(st5);

    GPR_FREE_ERR_MODULE

    return 0;
}
