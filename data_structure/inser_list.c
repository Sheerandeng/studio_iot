/*********************************************************************************
 *      Copyright:  (C) 2019 LingYun<lingyun@email.com>
 *                  All rights reserved.
 *
 *       Filename:  inser_list.c
 *    Description:  链表的插入
 *                 
 *        Version:  1.0.0(18/09/19)
 *         Author:  LingYun <lingyun@email.com>
 *      ChangeLog:  1, Release initial version on "18/09/19 19:30:04"
 *                 
 ********************************************************************************/
#include <stdio.h>
#include <stdlib.h>

typedef struct link_node_s
{
    int                 data;
    struct link_node_s  *next;
}link_node_t,*link_list_t;

link_list_t CreateListHead( int n )
{
    link_list_t new;
    link_list_t now;
    link_list_t head;
    int i;

    head = (link_list_t)malloc( sizeof(link_node_t) );
    now = head;

    for( i=1; i<=n; i++ )
    {
        new = (link_list_t)malloc( sizeof(link_node_t) ); // 新结点
        new->data = i;
        new->next = NULL;
        now->next = new;
        now = now->next;
    }
    return head;
}

link_list_t inser_list( link_list_t head, int data )
{
    link_list_t new;
    link_list_t pre;
    link_list_t pcur;

    new = (link_list_t)malloc( sizeof(link_node_t) );
    new->data = data;
    
    pre = head;
    pcur = pre->next;

    while( pcur!=NULL && pcur->data <= data )
    {
        pre = pcur;
        pcur = pcur->next;
    }
    if( pcur==NULL )
    {
        new->next = NULL;
        pre->next = new;
        return head;
    }
    new->next = pcur;
    pre->next = new;

    return head;
}

int main()
{
    link_list_t ptr1;
    link_list_t ptr2;
    link_list_t head;

    ptr1 = CreateListHead( 6 );
    head = ptr1;

    printf("尾插法创建链表：\n");
    while( ptr1->next!=NULL )
    {
        printf("%d",ptr1->next->data);
        ptr1 = ptr1->next;
    }

    ptr2 = inser_list( head , 5 );

    printf("\n插入后的链表：\n");
    while( ptr2->next!=NULL )
    {
        printf("%d",ptr2->next->data);
        ptr2 = ptr2->next;
    }
    printf("\n");
}
