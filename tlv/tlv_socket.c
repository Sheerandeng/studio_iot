/*********************************************************************************
 *      Copyright:  (C) 2019 LingYun<lingyun@email.com>
 *                  All rights reserved.
 *
 *       Filename:  tlv_unpack.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(07/04/19)
 *         Author:  LingYun <lingyun@email.com>
 *      ChangeLog:  1, Release initial version on "07/04/19 21:08:20"
 *                 
 ********************************************************************************/
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/socket.h> 
#include<netinet/in.h> 
#include<arpa/inet.h> 
#include<stdlib.h>

#define LISTEN_PORT     2001 

int main(int argc,char **argv)
{
    int                     rv = -1;
    int                     listen_fd,  client_fd = -1; 
    struct sockaddr_in      serv_addr; 
    socklen_t               cliaddr_len; 
    struct sockaddr_in      cli_addr; 
    char                    buf[1024]; 
    int                     on = 1;
    int                     ofset;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0); 
    if(listen_fd < 0 ) 
    {
        printf("create socket failure: %s\n", strerror(errno)); 
        return -1;
    }
    printf("\nsocket create fd[%d]\n", listen_fd); 


    memset(&serv_addr, 0, sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(LISTEN_PORT); 
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));

    if( bind(listen_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 )
    {
        printf("create socket failure: %s\n", strerror(errno)); 
        return -2; 
    }
    printf("\nsocket[%d] bind on port[%d] for all IP address ok\n", listen_fd, LISTEN_PORT); 

    listen(listen_fd, 10);
    
    printf("\nStart waiting and accept client connect...\n", listen_fd); 
    
    client_fd = accept(listen_fd, (struct sockaddr*)&cli_addr, &cliaddr_len);
    
    if(client_fd < 0) 
    {
        printf("accept new socket failure: %s\n", strerror(errno));
        return -2; 
    }
    printf("\naccept client[%s:%d] with fd [%d]\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), client_fd); 

    memset(buf, 0, sizeof(buf));
        
    ofset = 0;
        
    while(1)
    {
        if( (rv=read(client_fd, &buf[ofset], sizeof(buf)-ofset)) < 0)
        {
            printf("Read data from client socket[%d] failure: %s\n", client_fd, strerror(errno)); 
            close(client_fd); 
            exit(0); 
        }
        else if( rv == 0 ) 
        {
            printf("client socket[%d] disconnected\n", client_fd); 
            close(client_fd);
            exit(0); 
        }
        printf("Read data from client socket[%d]\n",client_fd);

        printf("\n");

        for(int i=0;i<rv;i++)
        {
        printf("%02x ",(unsigned char)buf[i]); 
        }
        
        ofset+=rv;
        
    }

    return 0;
}

