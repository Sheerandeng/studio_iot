/*********************************************************************************
 *      Copyright:  (C) 2019 LingYun<lingyun@email.com>
 *                  All rights reserved.
 *
 *       Filename:  heapsort.c
 *    Description:  堆排序
 *                 
 *        Version:  1.0.0(22/08/19)
 *         Author:  LingYun <lingyun@email.com>
 *      ChangeLog:  1, Release initial version on "22/08/19 16:04:58"
 *                 
 ********************************************************************************/
#include <stdio.h>

//交换值
void swap( int *i, int *j )
{
    int     temp;

    temp = *i;
    *i = *j;
    *j = temp;
}

//堆排序
void HeapAdjust( int array[], int i, int len )
{
    //i表示双亲结点，j表示子结点

    int     j=i*2+1;

    while( j<len )
    {
        if( j+1<len && array[j]<array[j+1] )
        {
            j++;
        }

        if( array[i]>array[j] )
        {
            break;
        }
        else
        swap( &array[i],&array[j]);

        i = j;
        j = 2*i+1;
    }
}

void MakeHeap( int array[],int n )
{
    int     i;
    for( i=n/2-1; i>=0; i-- )
    {
        HeapAdjust( array, i, n );
    }
}


void HeapSort( int array[], int len )
{
    int     i=0;
    //步骤一
    MakeHeap( array, len );

    //步骤二
    for(i=len-1; i>=0; i--)
    {
        swap( &array[i], &array[0] );
        HeapAdjust( array, 0, i );
    }
}

void main()
{
    int i,a[9]={5,2,6,0,3,9,1,7,4};

    HeapSort(a,9);

    printf("排序后的结果是：");

    for( i=0; i<9; i++ )
    {
        printf("%d",a[i]);
    }
    printf("\n");
}
