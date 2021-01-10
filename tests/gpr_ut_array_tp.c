/******************************************************************************
 *
 * gpr_ut_array_tp.c
 *
 ******************************************************************************
 *
 * Unit tests on "gpr_array_tp" module
 *
 *****************************************************************************/

#include <stdbool.h> // WARN: Not portable
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct student
{
    char *name;
    unsigned int mark;
};

static inline bool init_student(struct student *st,char *name,unsigned int mark)
{
    if(name == NULL)
        st->name = NULL;
    else
    {
        st->name = malloc(strlen(name) + 1);
        if(st->name == NULL)
            return false;
        strcpy(st->name,name);  
    }
    st->mark = mark;

    return true;
}

static inline bool init_default_student(struct student *st)
{
    st->name = NULL;
    st->mark = 0;

    return true;
}

static inline void free_student(struct student *st)
{
    if(st->name != NULL)
        free(st->name);
}

void print_student(struct student *st)
{
    if(st->name != NULL)
        printf("%s - %u\n", st->name, st->mark);
    else
        printf("(\\0) - %u\n", st->mark);
}

#define GPR_TP_TYPE         struct student
#define GPR_ARR_TYPE        student_array
#define GPR_FUNC(func)      st_arr_ ## func
#define GPR_EL_INIT_FUNC    init_default_student
#define GPR_EL_FREE_FUNC    free_student

#include "gpr_array_tp.inl"

/*****************************************************************************/


#include "gpr_builtin.h"
#include "gpr_err.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

// bool search_student_by_name(const struct student *st, const char *name)
// {
//     if (st->name == name)
//         return true;

//     if (strcmp(st->name, name) == 0)
//         return true;

//     return false;
// }

int main()
{
    GPR_INIT_ERR_MODULE

    struct student_array arr;
    void *elem = NULL;
    bool err;
    size_t index = 0;

    /* Create array */
    err = st_arr_init(&arr,4);
    assert(err);

    /* Create students */
    struct student st0,st1,st2,st3,st4,st5;
    init_student(&st0,"Valentin" ,  0);
    init_student(&st1,"Alfred"   , 20);
    init_student(&st2,"Julius"   ,  8);
    init_student(&st3,"Isabelle" , 14);
    init_student(&st4,"Sophie"   , 17);
    init_student(&st5,"Dominique",  2);

    /* Add students to array */
    printf("---Array of students---\n");
    err = st_arr_push_back(&arr,&st0);
    assert(err);
    err = st_arr_push_back(&arr,&st1);
    assert(err);
    err = st_arr_insert(&arr,st_arr_begin(&arr),&st2);
    assert(err);
    err = st_arr_insert(&arr,st_arr_begin(&arr),&st3);
    assert(err);
    err = st_arr_insert(&arr,st_arr_begin(&arr) + 2,&st4);
    assert(err);
    err = st_arr_insert(&arr,st_arr_begin(&arr),&st5);
    assert(err);

    /* Display array */
    st_arr_map(&arr, print_student);

    /* Search in array */
    // printf("\n---Searching students---\n");
    // elem = gpr_array_search(arr, search_student_by_name, "Julius", &index);
    // assert(elem != NULL);
    // assert(index == 2);
    // printf("Julius has been found at position %zu\n", index);

    /* Delete in array */
    printf("\n---Removing students---\n");
    st_arr_erase(&arr,st_arr_begin(&arr));
    st_arr_pop_back(&arr);
    st_arr_erase(&arr,st_arr_begin(&arr) + 2);
    st_arr_map(&arr, print_student);

    /* Replace in array */
    printf("\n---Replacing students---\n");

    struct student *it = st_arr_get(&arr,1);
    free_student(it);
    init_student(it,"Caroline", 1);
    st_arr_map(&arr, print_student);

    /* Free array */
    st_arr_free(&arr);

    GPR_FREE_ERR_MODULE

    return 0;
}
