#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int find( char **strs)
{
	int i,j;
	static int num=0;

	char *ptr;

	ptr=strs[0];
	for( i=0; i<strlen(strs[0]); i++ )
	{
		if( *ptr>='a' && *ptr<='z' )
		{
			num++;
		}
		ptr++;
	}

	ptr=strs[1];
	for( j=0; j<strlen(strs[1]); j++ )
	{
		if( *ptr>='a' && *ptr<='z' )
		{
			num++;
		}
		ptr++;
	}
	
	return num;
}

int main()
{
	char	*strs[]={"a12222222222bc","abc"};
	int	number;

	number = find(strs);
	printf("总共有%d个字符\n",number);
	return 0;
}

