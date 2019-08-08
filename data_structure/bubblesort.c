/*********************************************************************************
 *      Copyright:  (C) 2019 LingYun<lingyun@email.com>
 *                  All rights reserved.
 *
 *       Filename:  bubblesort.c
 *    Description:  冒泡排序
 *                 
 *        Version:  1.0.0(08/08/19)
 *         Author:  LingYun <lingyun@email.com>
 *      ChangeLog:  1, Release initial version on "08/08/19 14:25:29"
 *                 
 ********************************************************************************/

#include <stdio.h>

void bubblesort( int K[], int n )
{
    int i, j, temp;

    for( i=0; i<n-1; i++ )
    {
        for( j=i+1; j<n; j++ )
        {
            if( K[i]>K[j] )
            {
                temp = K[j];
                K[j] = K[i];
                K[i] = temp;
            }
        }
    }
}

int main()
{
    int i,a[10]={5,2,1,3,4,6,7,7,8,9};

    bubblesort(a,10);

    printf("排序后的结果是：");
    for( i=0; i<10; i++ )
    {
        printf("%d",a[i]);
    }

    printf("\n");

    return 0;
}

