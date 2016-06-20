/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: someone
 *
 * Created on June 8, 2016, 9:12 AM
 */

#include <stdio.h>
#include <stdlib.h>

struct DblLnkdLstItem //double linked list item
{
    int data;
    struct DblLnkdLstItem *pp;
    struct DblLnkdLstItem *pn;
};

struct DblLnkdLstItem * arrToDblLnkdLst(const arr[], size_t sz) {
    struct DblLnkdLstItem *ph = NULL;
    do {
        struct DblLnkdLstItem *pc = NULL;
        size_t i = 0;
        if (0 >= sz) {
            //TODO  print an warning
            break;
        }
        for (; i < sz; i++) {
            struct DblLnkdLstItem *pi =
                    (struct DblLnkdLstItem*) malloc(sizeof (struct DblLnkdLstItem));
            if (NULL == pi) {
                //TODO print an error
                break;
            }

            pi->data = arr[i];
            if (NULL == pc) {
                pc = pi;
                pc->pp = NULL;
                pc->pn = NULL;
                ph = pc;
            } else {
                pc->pn = pi;
                pi->pp = pc;
                pi->pn = NULL;

                pc = pi;
            }
        }
    } while (0);

    return ph;
}

int main(int argc, char** argv) {
    int test_arr[] = {23, 32, 43, 54, 2, 86, 90};
    int i = 0;
    struct DblLnkdLstItem *p_head = NULL;
    printf("Before processing:\n");
    for (; i < sizeof(test_arr)/sizeof(test_arr[0]); i++)
    {
        printf("%d ", test_arr[i]);
    }
    printf("\n");

    p_head = arrToDblLnkdLst(test_arr, sizeof(test_arr)/sizeof(test_arr[0]));

    printf("After processing:\n");
    if (NULL != p_head)
    {
        do
        {
            struct DblLnkdLstItem *p_for_free = p_head;
            printf("%d ", p_head->data);
            p_head = p_head->pn;
            free(p_for_free);
        } while(p_head);
        p_head = NULL;

    } else
    {
        printf("empty linked list\n");
    }
    printf("\n");
    return (EXIT_SUCCESS);
}

