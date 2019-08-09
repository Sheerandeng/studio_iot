/*********************************************************************************
 *      Copyright:  (C) 2019 LingYun<lingyun@email.com>
 *                  All rights reserved.
 *
 *       Filename:  text.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(05/08/19)
 *         Author:  LingYun <lingyun@email.com>
 *      ChangeLog:  1, Release initial version on "05/08/19 12:36:05"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <string.h>
void main()
{
    int     hash[512];
    char    str[]={"abGD_eiqngeow#i*gew*earr"};
    char    *ptr=NULL;
    int     i;

    memset(&hash,0,sizeof(hash));
    ptr = str;
    while(*ptr) // 输入
    {
        hash[*ptr]++ ;
        ptr++ ;
    }
    for(i=0; i<512; i++) // 输出
    {
        if(hash[i] > 0)
        {
            printf("%C%d",i,hash[i]);
        }
    }
    printf("\n");
}
