
/*
 * test from National Institutes of Health (NIH) USA Washington DC
 * 
 * File:   main.c
 * Author: someone
 *
 * Created on June 7, 2016, 9:51 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* alias; /* '\0'-terminated C string */
    int specific_id;
} aliasID;

#define MAX_SOME_IDS 8

int (*GetSomeIDs(aliasID*** aIDs, int* sz))[MAX_SOME_IDS];

void print_id_and_aliases() {
    //int * someIDs, theFirst, *r;
    int sz = 0;
    int (*someIDs)[MAX_SOME_IDS];
    aliasID** aIDs = NULL;

    someIDs = GetSomeIDs(&aIDs, &sz); /* defined below */
    if (someIDs && aIDs) {
        int i = 0;
        for (; i < sz; i++) {
            if (NULL == aIDs[i]) {
                //TODO print an error
                free(aIDs);
                free(someIDs);
                return;
            }
            //assume aliase can be empty
            printf("%d: some_id=%d alias=%s id=%d\n", i, (*someIDs)[i], aIDs[i]->alias, aIDs[i]->specific_id);
            if (aIDs[i]->alias)
                free(aIDs[i]->alias);

            free(aIDs[i]);
        }
        free(aIDs);
        free(someIDs);
    }
}

static aliasID* GetNextAlias() {
    aliasID* aid = malloc(sizeof (aliasID));
    if (aid) {
        //there is no explanation how to generate aliases and ids it just example
        static const char* alias_prefix = "alias-";
        static int index = 0;

        aid->alias = malloc(strlen(alias_prefix) + 2); //assume not more then 10 different aliases
        if (NULL == aid->alias)
            return 0;

        sprintf(aid->alias, "%s%d", alias_prefix, index);
        aid->specific_id = index++;

        //assume not more then 10 different aliases
        if (index > 9) index = 0;
    }

    return aid;
}

static int GetNumberOfAliases() {
    return 9;
}

/*-------- GetSomeIDs-----*/
int (*GetSomeIDs(aliasID*** aIDs, int* sz))[MAX_SOME_IDS] {
    int i = 0;
    int* ids = NULL;
    if (NULL == aIDs) {
        //TODO print some error or warning
        return NULL;
    }

    /* The ids are defined here */
    ids = malloc(sizeof (int) * MAX_SOME_IDS);
    if (NULL == ids)
        return NULL;
    memset(ids, 0, sizeof (int) * MAX_SOME_IDS);

    *sz = GetNumberOfAliases();
    *aIDs = malloc(*sz * sizeof (aliasID*));
    if (NULL == *aIDs)
        return NULL;

    memset(*aIDs, 0, *sz * sizeof (aliasID*));

    for (; i < *sz && i < MAX_SOME_IDS; i++) {

        (*aIDs)[i] = GetNextAlias();
        if (NULL == (*aIDs)[i]) {
            //TODO print some error or exit or what to do?
            break;
        } else {
            ids[i] = ((*aIDs)[i])->specific_id;
        }
    }

    return (int(*)[MAX_SOME_IDS])ids;
}

int * ReorderIDs(int *x) {
    return &x[1];
}

typedef void (*func_ptr)(int *arr);

void array_modifaer(int arr[], size_t sz, func_ptr fn) {
    int i = 0;
    for (; i < sz; i++) {
        fn(&arr[i]);
    }
}

void inc_int(int*arr) {
    (*arr)++;
}

int main(int argc, char** argv) {
    int * someIDs, theFirst, *r;

    int arr[] = {1, 2, 3};

    printf(" %p %p\n", &arr, &arr[0]); //*(*parr));
    array_modifaer(arr, 3, inc_int);
    printf("%d %d %d\n", arr[0], arr[1], arr[2]);
    print_id_and_aliases();

    //r = ReorderIDs(someIDs);
    /* we want to use ‘theFirst’ and  ‘r’ here*/

    return 0;
}