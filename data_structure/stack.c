/*********************************************************************************
 *      Copyright:  (C) 2019 LingYun<lingyun@email.com>
 *                  All rights reserved.
 *
 *       Filename:  stack.c
 *    Description:  二进制转十进制
 *                 
 *        Version:  1.0.0(06/08/19)
 *         Author:  LingYun <lingyun@email.com>
 *      ChangeLog:  1, Release initial version on "06/08/19 14:14:47"
 *                 
 ********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STACK_INIT_SIZE 20
#define STACKINCREMENT  10

typedef char    ElemType;

typedef struct
{
    ElemType     *base;
    ElemType     *top;
    int     stacksize;
}sqStack;


void initsatck( sqStack *s )
{
    s->base = (ElemType *)malloc(STACK_INIT_SIZE *sizeof(ElemType));
    if( !s->base )
    {
        exit(0);
    }

    s->top = s->base;
    s->stacksize = STACK_INIT_SIZE;
}

void push( sqStack *s, ElemType e )
{
    if( s->top - s->base >= s->stacksize ) // 栈已满
    {
        s->base = (ElemType *)realloc(s->base,(s->stacksize + STACKINCREMENT)*sizeof(ElemType));
        if( !s->base )
        {
            exit(0);
        }
    }

    *(s->top) = e;
    s->top++;
}

void pop( sqStack *s, ElemType *e )
{
    if( s->top == s->base ) // 栈已空
    {
        return;
    }
    *e = *--(s->top);
}

int stacklen( sqStack s) // 当不需要修改结构体里面的值，就不要写成指针
{
    return (s.top - s.base);
}

int main()
{
    ElemType    c;
    sqStack     s;
    int         len,i,sum = 0;

    initsatck(&s);

    printf("请输入二进制数，输入#符号表示结束！\n");
    scanf("%c",&c); // %c会一个一个输入进去，如果是%d它会一整个输入
    
    while( c != '#')
    {
        push(&s,c);
        scanf("%c",&c);
    }
    getchar(); // 把'\n'从键盘缓冲区中去掉，也相当于清空缓冲区

    len = stacklen(s);
    printf("栈的当前容量是：%d\n",len);

    for( i=0; i<len; i++)
    {
        pop(&s,&c);
        sum = sum + (c-48)*pow(2,i);
    }
    printf("转化为十进制数是：%d\n",sum);

    return 0;
}
