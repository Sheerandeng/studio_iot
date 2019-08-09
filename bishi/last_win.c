/*********************************************************************************
 *      Copyright:  (C) 2019 LingYun<lingyun@email.com>
 *                  All rights reserved.
 *
 *       Filename:  last_win.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(04/08/19)
 *         Author:  LingYun <lingyun@email.com>
 *      ChangeLog:  1, Release initial version on "04/08/19 16:30:32"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
    int         data;
    struct Node *next;
}LNode;

void mtkFunc(int n, int k, int m)
{
    int i;
    int total = n;
    LNode *p,*prevP,*head,*s;

    head = (LNode *)malloc(sizeof(LNode));
    head->data = 0;
    head->next = head;
    p = head;
    p->data = 1;
    p->next = p;
    for(i=2;i<=n;i++)
    {
        LNode *temp = (LNode *)malloc(sizeof(LNode));
        temp->data = i;
        temp->next = p->next;
        p->next = temp;
        p = p->next;
    }
    p = head;
    for(i=1;i<k;i++)
        p = p->next;
    prevP = head;
    while( total !=1 )
    {
        for(i=1;i<m;i++)
            p = p->next;
        while( prevP->next != p )
            prevP = prevP->next;
        prevP->next = prevP->next;
        s = p;
        p = p->next;
        free(s);
        total--;
    }
    printf("last win is %d\n",p->data);
    
}

int main(int argc, char **argv)
{
    mtkFunc(10,5,3);
    return 0;
}

