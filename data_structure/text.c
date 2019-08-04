/*********************************************************************************
 *      Copyright:  (C) 2019 LingYun<lingyun@email.com>
 *                  All rights reserved.
 *
 *       Filename:  text.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(03/08/19)
 *         Author:  LingYun <lingyun@email.com>
 *      ChangeLog:  1, Release initial version on "03/08/19 17:34:13"
 *                 
 ********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define OK 1;

typedef struct _Node 
{
    int           data;      // 数据域   
    struct _Node  *Next;     // 指针域，指针指向的是下一个结点
}Node;


Node* create_list_head( int num )
{
        Node    *head;
        Node    *p;
        int     i;

        srand( time(0) );   // 初始化随机数种子 

        head = (Node *)malloc( sizeof(Node) );
        head->Next = NULL;

        for( i=0; i<num; i++ )
        {
            p = (Node *)malloc( sizeof(Node) ); // 新结点
            p->data = rand()%100+1;
            p->Next = head->Next;
            head->Next = p; 
        }
        return head;
}

int get_mid_node( Node *head, int *e )
{
    Node   *search;
    Node   *mid;
    mid = search = head;

    while( search->Next != NULL )
    {
        if( search->Next->Next != NULL ) 
        {
            search = search->Next->Next;
            mid = mid->Next;
        }
        else
        {
            search = search->Next;
        }
    }
    *e = mid->data;

    return OK;
}


int main()
{
    Node     *L ;
    Node     *p ;
    int      *num;
    int      a;

    num = NULL;

    L = create_list_head(20);

    p = (Node *)malloc( sizeof(Node) ); // 新结点
    p = L->Next;

    while( p != NULL)
    {
        printf("%d\n",p->data);
        p = p->Next;
    }

    
    get_mid_node( L, &a );
    printf("The mid data is %d\n", a);
    
    return 0;
}




