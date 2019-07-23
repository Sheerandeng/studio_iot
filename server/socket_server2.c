#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>
#include<errno.h>
#include<unistd.h>
#include<getopt.h>
#include<stdlib.h>
#include<pthread.h>
#include<ctype.h>


#define     MSG_STR	"Hello studio client\n"
#define     BACKLOG	10

typedef	void *(THREAD_BODY)(void *thread_arg);

void *thread_worker(void *ctx);
int thread_start(pthread_t * thread_id, THREAD_BODY * thread_workbody, void *thread_arg);



void print_usage(char *progname)
{
	printf("%s usage: \n",progname);
	printf("-p(--port): sepcify server listen port.\n ");
	printf("-h(--help): print this help information.\n ");
	return;
}



int main(int argc, char **argv)
{
	int                 	socket_fd,client_fd = -1;
    	int                 	rv = -1;
	struct sockaddr_in	serv_addr;
    	struct sockaddr_in	cli_addr;
    	socklen_t           	cliaddr_len;
	int			port = 0;
	int			ch;
	int			on = 1;
	pid_t			pid;
	pthread_t		tid;
	pthread_t		lock;

	pthread_mutex_init(lock,NULL);

	struct option		opts[]={
		{"port",required_argument,NULL,'p'},
		{"help",no_argument,NULL,'h'},
		{NULL,0,NULL,0}
	};

	while( (ch=getopt_long(argc, argv, "p:h", opts, NULL)) != -1 )
	{
		switch(ch)
		{
			case 'p':
				port=atoi(optarg);
				break;

			case 'h':
				print_usage(argv[0]);
				return 0;
		}
	
	}

	if( !port )
	{
		print_usage(argv[0]);
		return 0;
	}


	socket_fd = socket(AF_INET , SOCK_STREAM, 0);
	if(socket_fd < 0)
	{
		printf("Create socket failure: %s\n",strerror(errno));
		return -1;
	}
		printf("socket create fd [%d]\n", socket_fd);

	setsockopt(socket_fd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if( bind(socket_fd,(struct sockaddr *)&serv_addr,sizeof(serv_addr) ) < 0)
	{
		printf("create socket failure: %s\n",strerror(errno));
		return -2;
	}
		printf("socket[%d] bind on port[%d] for all IP address ok\n",socket_fd,port);

	listen(socket_fd,BACKLOG);
	printf("Start to listen on port [%d]\n", port);

	while(1)
	{
		printf("Start waiting and accept new client connect...\n");

		client_fd = accept(socket_fd,(struct sockaddr*)&cli_addr,&cliaddr_len);
		if(client_fd < 0)
		{
			printf("accept new socket failure : %s\n",strerror(errno));
			return -2;
		}
			printf("Accept new client[%s:%d] with fd [%d]\n",inet_ntoa(cli_addr.sin_addr),ntohs(cli_addr.sin_port),client_fd);


		thread_start(&tid, thread_worker, (void *)client_fd);
	}

	close(socket_fd);
	return 0;

	pthread_mutex_destroy(lock);
}
	

int thread_start(pthread_t *thread_id, THREAD_BODY *thread_workbody, void *thread_arg)
{
	int			rv = -1;
	pthread_attr_t		thread_attr;

	if( pthread_attr_init(&thread_attr) )
	{
		printf("pthread_attr_init() failure : %s\n",strerror(errno));
		goto CleanUp;
	}
	
	if( pthread_attr_setstacksize(&thread_attr,120*1024) )
	{
		printf("pthread_attr_setstacksize() failure : %s\n",strerror(errno));
		goto CleanUp;
	}

	if( pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED) )
	{
		printf("pthread_attr_setdetachstate() failure : %s\n",strerror(errno));
		goto CleanUp;
	}

	/* Create the thread */

	if( pthread_create(thread_id, &thread_attr, thread_workbody, thread_arg) )
	{
		 printf("Create thread failure : %s\n", strerror(errno));
		 goto CleanUp;
	}

	rv = 0;

CleanUp:
	/* Destroy the attributes of thread */
	pthread_attr_destroy(&thread_attr);
	return rv;
}

void *thread_worker(void *ctx)
{
	int		client_fd;
	int		rv;
	char		buf[1024];
	int		i;

	pthread_mutex_lock(&ctx->lock);

	if( !ctx )
	{
		printf("Invalid input arguments in %s()\n",__FUNCTION__);
		pthread_exit(NULL);
	}
	
	client_fd = (int)ctx;
	printf("Child thread start to communicate with socket client...\n");
	while(1)
	{
		memset(buf,0,sizeof(buf));
		rv=read(client_fd,buf,sizeof(buf));
		if(rv<0)
		{
			printf("Read data from client socket_fd[%d] failure : %s and thread will exit.\n",client_fd,strerror(errno));
			close(client_fd);
			pthread_exit(NULL);
		}
		else if(rv==0)
		{
			printf("Socket[%d] get disconnected and thread will exit\n.",client_fd);
			close(client_fd);
			pthread_exit(NULL);
		}
		else if(rv>0)
		{
			printf("Read %d bytes data from Server : %s\n",rv,buf);
		}
		/* convert letter from lowercase to uppercase */
		for(i=0;i<rv;i++)
		{
			buf[i]=toupper(buf[i]);
		}

		rv=write(client_fd,buf,rv);
		if(rv<0)
		{
			printf("Write to client by socket_fd[%d] failure : %s and thread will exit.\n",client_fd,strerror(errno));
			close(client_fd);
			pthread_exit(NULL);
		}
	}

	ptread_mutex_unlock(&ctx->lock);
}




