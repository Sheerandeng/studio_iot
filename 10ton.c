/*********************************************************************************
 *      Copyright:  (C) 2019 LingYun<lingyun@email.com>
 *                  All rights reserved.
 *
 *       Filename:  1.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(12/05/19)
 *         Author:  LingYun <lingyun@email.com>
 *      ChangeLog:  1, Release initial version on "12/05/19 19:24:44"
 *                 
 ********************************************************************************/

#include<stdio.h>

int main()
{
    int m,n;
    int a[1024],i=0,j;
    printf("请输入需要转化的十进制数，以及需要转化为几进制：\n");
    scanf("%d%d",&m,&n);
    while(m != 0)
    {
        a[i++]=m%n;
        m/=n;
    }
    for(j=i-1;j>=0;j--)
    {
    printf("%d",a[j]);
    }

    printf("\n");

    return 0;
}
