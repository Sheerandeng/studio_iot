#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>
#include<errno.h>
#include<unistd.h>
#include<getopt.h>
#include<stdlib.h>

#define     MSG_STR	"Hello studio client\n"
#define     BACKLOG	10

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
	
	struct option		opts[]=
	{
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

		pid = fork();
		if(pid < 0)
		{
			printf("fork() creat process failure: %s\n",strerror(errno));
			close(client_fd);
			continue;
		}
		else if(pid > 0)
		{
			/* Parent process close client fd and goes to accept new socket client again */
			close(client_fd);
			continue;
		}
		else if(0 == pid)
		{
			char		buf[1024];
			/* Child process close the listen socket fd */
			close(socket_fd);

			printf("Child process start to communicate with socket client...\n");
		

			memset(buf,0,sizeof(buf));
			if((rv=read(client_fd,buf,sizeof(buf))) < 0)
			{
				printf("Read data from client socket[%d] failure : %s\n", client_fd ,strerror(errno));
				close(client_fd);
				exit(0);
			}
			else if(rv == 0)
			{
				printf("client socket[%d] disconnected\n",client_fd);
				close(client_fd);
				exit(0);
			}
				printf("read %d bytes data from client[%d] and echo it back: '%s'\n",rv,client_fd,buf);

			if(write(client_fd,buf,rv) < 0)
			{
				printf("Write %d bytes data back to client[%d] failure : %s\n",rv,client_fd,strerror(errno));
				close(client_fd);
				exit(0);
			}

			sleep(1);
			printf("Close client socket[%d] and child process exit.\n",client_fd);
			close(client_fd);
			exit(0);
		}
	}

	close(socket_fd);

	return 0;
}



