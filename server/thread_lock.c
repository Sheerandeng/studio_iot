#include<stdio.h>











void *thread_work1(void *args);
void *thread_work2(void *args);

typedef struct worker_ctx_s
{
	int		shared_var;
	pthread_mutex_t	lock;
}worker_ctx_t;



int main(int argc, char **argv)
{
	worker_ctx_t	worker_ctx;
	pthread_t	tid;
	pthread_attr_t	thread_attr;

	worker_ctx.shared_var = 1000;
	pthread_mutex_init(&worker_ctx.lock,NULL);


	if( pthread_attr_init(&thread_attr) )
	{
		printf("pthread_attr_init() failure : %s\n",strerror(errno));
		return -1;
	}
	
	if( pthread_attr_setstacksize(&thread_attr,120*1024) )
	{
		printf("pthread_attr_setstacksize() failure : %s\n",strerror(errno));
		return -1;
	}

	if( pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED) )
	{
		printf("pthread_attr_setdetachstate() failure : %s\n",strerror(errno));
		return -1;
	}

	pthread_creat(&tid, &thread_attr, )
}
