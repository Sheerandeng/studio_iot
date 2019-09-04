#include <stdio.h>
#include <limits.h>

int find( int array[], int size )
{
	int max_num = INT_MIN;
	int second_max_num = INT_MIN;
	int i;

	for( i=0; i<size; i++ )
	{
		if( array[i] > max_num )
		{
			second_max_num = max_num;
			max_num = array[i];
		}
		else if( array[i] > second_max_num )
		{	
			second_max_num = array[i];
		}
	}
	return second_max_num;
}

void main()
{
	int array[]={1,2,3,4,5,6,7};
	int length = sizeof(array)/sizeof(array[0]) ;
	int a;
	a = find( array, length );
	printf("第二大的数为：%d\n",a);
}
