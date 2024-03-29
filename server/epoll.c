#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<ctype.h>
#include<time.h>
#include<pthread.h>
#include<libgen.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<getopt.h>
#include<netinet/in.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<sys/resource.h>

#define     ARRAY_SIZE(x)	(sizeof(x)/sizeof(x[0]))
#define     MAX_EVENTS      512

static  inline  void	print_usage(char *progname);
int	socket_server_init(char *listen_ip, int listen_port);
void    set_socket_rlimit(void);

int main(int argc, char **argv)
{
	int                 	listenfd,connfd = -1;
    int                 	serv_port = -1;
	int			            daemon_run = 0;
	char			        *progname = NULL;
	int			            opt;
	int			            rv;
	int			            i,j;
	int			            found;
	int			            maxfd = 0;
	char			        buf[1024];

    int                     epollfd;
    struct  epoll_event     event;
    struct  epoll_event     event_array[MAX_EVENTS];
    int                     events;

	
	struct option		long_options[] =
	{
		{"daemon",no_argument,NULL,'b'},
		{"port",required_argument,NULL,'p'},
		{"help",no_argument,NULL,'h'},
		{NULL,0,NULL,0}
	};

	progname = basename(argv[0]);
	
	/* basename(argv[0]) means delete './' to get name after './'  */

	/* Parser the command line parameters */

	while( (opt=getopt_long(argc, argv, "bp:h", long_options, NULL)) != -1 )
	{
		switch( opt )
		{
			case 'b':
					daemon_run=1;
					break;

			case 'p':
					serv_port=atoi(optarg);
					break;

			case 'h':	/* get help information */
					print_usage(progname);
					return EXIT_SUCCESS;
			
			default:
					break;
		}
	
	}

	if( !serv_port )
	{
		print_usage(progname);
		return -1;
	}

    set_socket_rlimit(); /* set max open socket count */

	if( (listenfd = socket_server_init(NULL, serv_port)) < 0 )
	{
		printf("ERROR: %s server listen on port %d failure\n", argv[0],serv_port);
		return -2;
	}
		printf("%s server start to listen on port %d\n", argv[0],serv_port);

	/* set program running on backgroud */


	if( daemon_run )
	{
		daemon(0,0);
	}
    
    if( (epollfd=epoll_create(MAX_EVENTS)) <0 )
    {
        printf("epool_create() failure: %s\n",strerror(errno));
        return -3;
    }
    
    //event.events = EPOLLIN | EPOLLET;
    event.events = EPOLLIN;
    event.data.fd = listenfd;

    if( epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &event) < 0 )
    {
        printf("epoll add listen socket failure: %s\n",strerror(errno));
        return -4;
    }

	for( ; ; )
	{	
		/* program will blocked here */
    
        events = epoll_wait(epollfd, event_array, MAX_EVENTS, -1);
        if(events < 0)
        {
            printf("epoll failure: %s\n",strerror(errno));
            break;
        }
        else if(events == 0)
        {
            printf("epoll get timeout\n");
            continue;
        }

        /* rv>0 is the active events count */

        for(i=0; i<events; i++)
        {
            if( (event_array[i].events&EPOLLERR) || (event_array[i].events&EPOLLHUP) );
            {
                printf("epoll_wait get error on fd[%d]: %s\n",event_array[i].data.fd,strerror(errno));
                epoll_ctl(epollfd, EPOLL_CTL_DEL, event_array[i].data.fd, NULL);
                close(event_array[i].data.fd);
            }
            
            /* listen socket get event means new client start connect now */

            if( event_array[i].data.fd == listenfd )
            {
                if( (connfd=accept(listenfd, (struct sockaddr *)NULL, NULL)) <0 )
                {
                    printf("accept new client failure: %s\n",strerror(errno));
                    continue;
                }

                event.data.fd = connfd;
                //event.events = EPOLLIN | EPOLLET;
                event.events = EPOLLIN;
                if( epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &event) < 0 )
                {
                    printf("epoll add client socket failure: %s\n",strerror(errno));
                    close(event_array[i].data.fd);
                    continue;
                }
                printf("epoll add new client socket[%d] ok.\n",connfd);
            }
            else /* already connected client socket get data incoming */
            {
                if( (rv=read(event_array[i].data.fd, buf, sizeof(buf))) <= 0 )
                {
                    printf("socket[%d] read failure or get disconnect and will be removed.\n",event_array[i].data.fd);
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, event_array[i].data.fd, NULL);
                    close(event_array[i].data.fd);
                    continue;
                }
                else
                {
                    printf("socket[%d] read get %d bytes data\n", event_array[i].data.fd, rv);

                    /* conver letter from lowercase to uppercase */
                    for(j=0; j<rv; j++)
                    {
                        buf[j]=toupper(buf[j]);
                    }
                    if( write(event_array[i].data.fd, buf, rv) <0 )
                    {
                        printf("socket[%d] write failure: %s\n",event_array[i].data.fd, strerror(errno));
                        epoll_ctl(epollfd, EPOLL_CTL_DEL, event_array[i].data.fd, NULL);
                        close(event_array[i].data.fd);
                    }
                }
            }/* for(i=0; i<rv; i++) */

        }/*while(1) */
    }
CleanUp:
        close(listenfd);
        return 0;
}




static inline void print_usage(char *progname)
{
	printf("Usage: %s [OPTION]...\n",progname);

	printf(" %s is a socket server program, which used to verify client and echo back string from it\n",progname);
	printf("\nMandatory arguments to long optins are mandatory for short options too:\n");

	printf("-b	[daemon	] set program running on background\n");
	printf("-p	[port	] socket server port address\n");
	printf("-h	[help	] display this help information");

	printf("\nExample: %s -b -p 8900\n",progname);
	return;
}



int socket_server_init(char *listen_ip, int listen_port)
{
	struct sockaddr_in		servaddr;
	int				        rv=0;
	int				        on=1;
	int				        listenfd;

	if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		printf("Use socket() to create a TCP socket failure: %s\n",strerror(errno));
		return -1;
	}

	/* Set socket port reuseable, fix 'Address already in use' bug when socket server restart */

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(listen_port);

	if( !listen_ip )  /* Listen all the local IP address */
	{
		servaddr.sin_addr.s_addr = htons(INADDR_ANY);
	}
	else  /* Listen the specified IP address */
	{
		if(inet_pton(AF_INET, listen_ip, &servaddr.sin_addr) <= 0 )
		{
			printf("inet_pton() set listen IP address failure.\n");
			rv = -2;
			goto CleanUp;
		}
	}
	
	if(bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		printf("Use bind() to bind the TCP socket failure: %s\n",strerror(errno));
		rv = -2;
		goto CleanUp;
	}

	if(listen(listenfd,13)<0)
	{
		printf("Use bind() to bind the TCP socket failure: %s\n",strerror(errno));
		rv = -4;
		goto CleanUp;
	}

CleanUp:
	if(rv<0)
		close(listenfd);
	else
		rv = listenfd;
	return rv;
}


/* Set open file description count to max */
void set_socket_rlimit(void)
{
    struct rlimit limit = {0};

    getrlimit(RLIMIT_NOFILE, &limit);
    limit.rlim_cur = limit.rlim_max;
    setrlimit(RLIMIT_NOFILE, &limit);

    printf("set socket open fd max count to %d\n",limit.rlim_max);
}












