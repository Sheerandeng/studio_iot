/*********************************************************************************
 *      Copyright:  (C) 2019 LingYun<lingyun@email.com>
 *                  All rights reserved.
 *
 *       Filename:  josephus.c
 *    Description:  约瑟夫问题
 *                 
 *        Version:  1.0.0(04/08/19)
 *         Author:  LingYun <lingyun@email.com>
 *      ChangeLog:  1, Release initial version on "04/08/19 14:11:03"
 *                 
 ********************************************************************************/
#include <stdio.h>
#include <stdlib.h>

typedef struct _Node
{
    int           data;
    struct _Node  *Next;
}Node;

Node *create_list( int num ) // 尾插法
{
    Node    *head;
    Node    *now;
    Node    *new;

    int     i=1;

    head = (Node *)malloc( sizeof(Node) );
    now = head;
    
    if( num !=0 )
    {
        while( i<=num )
        {
            new = (Node *)malloc( sizeof(Node) );
            new->data = i++;
            now->Next = new;
            now = new;
        }
        new->Next = head->Next; // 首位相连
    }
    free(head);
    
    return new->Next;
}

int main()
{
    int n=41;
    int m=3;
    int i;

    Node *p=create_list(n);
    Node *temp;

    m%=n;

    while( p!=p->Next )
    {
        for( i=1; i<m-1; i++ )
        {
            p = p->Next;
        }
        printf("%d->",p->Next->data);
        temp = p->Next;
        p->Next = temp->Next;
        free(temp);
        p = p->Next;
    }
    printf("%d\n",p->data);
    return 0;
}
