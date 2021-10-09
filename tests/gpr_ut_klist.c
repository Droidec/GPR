/******************************************************************************
 *
 * gpr_ut_list.c
 *
 ******************************************************************************
 *
 * Unit tests on "gpr_Klist" module
 *
 *****************************************************************************/

#include "gpr_klist.h"
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
    struct gpr_klist head;
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
    gpr_klist_init(&st->head);

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

unsigned int insert_student_by_mark(struct student *st, struct gpr_klist *list, unsigned int list_size)
{
    struct student *cur_student = NULL;
    enum GPR_Err err;

    /* Specific case */
    if (list_size == 0)
    {
        err = gpr_klist_push_back(&st->head, list);
        assert(err == GPR_ERR_OK);
        return list_size + 1;
    }

    /* Insert student */
    GPR_KLIST_FOR_EACH_ENTRY (cur_student, list, head)
    {
        // Insert student before student with a greater mark
        if (cur_student->mark > st->mark)
        {
            err = gpr_klist_push_back(&st->head, &cur_student->head);
            assert(err == GPR_ERR_OK);
            return list_size + 1;
        }
    }

    // We have the best mark
    err = gpr_klist_push_back(&st->head, list);
    assert(err == GPR_ERR_OK);

    return list_size + 1;
}

int main()
{
    GPR_ALLOC_ERR_MODULE

    struct student *cur_student = NULL;
    struct student *tmp_student = NULL;
    struct student *worst_student = NULL;
    struct gpr_klist list = GPR_KLIST_INIT(list);
    unsigned int list_size = 0;
    enum GPR_Err err;

    /* New students */
    struct student *st0 = new_student("Valentin", 0);
    struct student *st1 = new_student("Alfred", 20);
    struct student *st2 = new_student("Julius", 8);
    struct student *st3 = new_student("Isabelle", 14);
    struct student *st4 = new_student("Sophie", 17);
    struct student *st5 = new_student("Dominique", 2);

    /* Insert students by mark (from worst to best) */
    list_size = insert_student_by_mark(st0, &list, list_size);
    list_size = insert_student_by_mark(st1, &list, list_size);
    list_size = insert_student_by_mark(st2, &list, list_size);
    list_size = insert_student_by_mark(st3, &list, list_size);
    list_size = insert_student_by_mark(st4, &list, list_size);
    list_size = insert_student_by_mark(st5, &list, list_size);

    /* Print students by mark (from worst to best) */
    printf("---Students (From worst to best)---\n");
    GPR_KLIST_FOR_EACH_ENTRY (cur_student, &list, head)
        print_student(cur_student);

    /* Print students by mark (from best to worst) */
    printf("\n---Students (From best to worst)---\n");
    GPR_KLIST_FOR_EACH_ENTRY_REVERSE (cur_student, &list, head)
        print_student(cur_student);

    /* Delete worst student */
    worst_student = GPR_KLIST_ENTRY(list.next, struct student, head);
    err = gpr_klist_delete(list.next);
    assert(err == GPR_ERR_OK);
    free_student(worst_student);

    /* Print students */
    printf("\n---Students (Deleted worst one)---\n");
    GPR_KLIST_FOR_EACH_ENTRY (cur_student, &list, head)
        print_student(cur_student);

    /* Free students */
    GPR_KLIST_FOR_EACH_ENTRY_SAFE (cur_student, tmp_student, &list, head)
        free_student(cur_student);

    GPR_FREE_ERR_MODULE

    return 0;
}
