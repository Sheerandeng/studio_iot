/*********************************************************************************
 *      Copyright:  (C) 2019 LingYun<lingyun@email.com>
 *                  All rights reserved.
 *
 *       Filename:  bubblesort.c
 *    Description:  真正的冒泡排序
 *                 
 *        Version:  1.0.0(08/08/19)
 *         Author:  LingYun <lingyun@email.com>
 *      ChangeLog:  1, Release initial version on "08/08/19 14:25:29"
 *                 
 ********************************************************************************/

#include <stdio.h>

void bubblesort( int K[], int n )
{
    int i, j, temp, count1=0, count2=0, flag;

    flag = 1;
    for( i=0; i<n-1 && flag; i++ )
    {
        for( j=n-1; j>i; j-- )
        {
            count1++; // 比较
            if( K[j-1]>K[j] )
            {
                count2++; // 移动
                temp = K[j-1];
                K[j-1] = K[j];
                K[j] = temp;
                flag = 1;
            }
            else
                flag = 0;
        }
    }
    printf("总共进行了%d次比较，进行了%d次移动！",count1,count2);
}

int main()
{
    int i,a[10]={1,0,2,3,4,5,6,7,8,9};

    bubblesort(a,10);

    printf("排序后的结果是：");
    for( i=0; i<10; i++ )
    {
        printf("%d",a[i]);
    }

    printf("\n");

    return 0;
}

