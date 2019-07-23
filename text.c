/*********************************************************************************
 *      Copyright:  (C) 2019 LingYun<lingyun@email.com>
 *                  All rights reserved.
 *
 *       Filename:  text.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(20/04/19)
 *         Author:  LingYun <lingyun@email.com>
 *      ChangeLog:  1, Release initial version on "20/04/19 20:13:19"
 *                 
 ********************************************************************************/
#include<stdio.h>
int main (int argc, char **argv)
{
        int       a = 0x12345678;

        int       *p1 = NULL;
        char    *p2 = NULL;

    p1 = &a;
    p2 = (char *)&a;
    printf("p1 address: %p p1 value: %p p1 point memory value: 0x%x\n", &p1, p1, *p1 );
    printf("p2 address: %p p2 value: %p p2 point memory value: 0x%x\n", &p2, p2, *p2 );
    p1 ++;          
    p2++;
    printf("p1 address: %p p1 value: %p p1 point memory value: 0x%x\n", &p1, p1, *p1 );

    return 0;

}
