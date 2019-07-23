#include<stdio.h>       
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define     SERVER_IP       "192.168.0.16"
#define     SERVER_PORT     8888

int get_wendu(float * wendu);

int main(int argc ,int * argv)
{
        float       wendu;
        int         conn_fd = -1;
        int         rv = -1;
        char        buf[1024];
        char        buf1[1024];
        struct sockaddr_in  serv_addr;

        rv=get_wendu(&wendu);
        if(rv<0)
        {
            printf("get wendu failure. return value: %d\n",rv);
        }
        printf("get wendu : %f\n",wendu);



        conn_fd = socket(AF_INET,SOCK_STREAM,0);
        if(conn_fd < 0)
        {
            printf("Creat socket failure : %s\n",strerror(errno));
            return -1;
        }
            printf("Creat socket succeed .\n");

        memset(&serv_addr,0,sizeof(serv_addr));
        serv_addr.sin_family=AF_INET;
        serv_addr.sin_port=htons(SERVER_PORT);
        inet_aton(SERVER_IP,&serv_addr.sin_addr);

        if(connect(conn_fd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
        {
            printf("Connet failure : %s\n",strerror(errno));
            return -1;
        }
            printf("Connet to server [%s:%d] \n",SERVER_IP,SERVER_PORT);

        snprintf(buf1,sizeof(buf1),"%f",wendu);

        if(write(conn_fd,buf1,sizeof(buf1))<0)
        {
            printf("write failure : %S\n",strerror(errno));
            goto cleanup;
        }
            printf("Writed .\n");

        memset(buf,0,sizeof(buf));
        rv = read(conn_fd,buf,sizeof(buf));

        if(rv<0)
        {
            printf("Read data form server failure : %S\n",strerror(errno));
            goto cleanup;
        }
        else if(rv==0)
        {
            printf("Client connect to server disconnected .\n");
            goto cleanup;
        }
        printf("Read %d bytes data from server : '%S'\n",rv,buf);

cleanup:
        close(conn_fd);





        return 0;
}

int get_wendu(float * wendu)
{
        int     fd;
        char    buf[128];
        char    *ptr;

        fd=open("/sys/bus/w1/devices/28-041731f7c0ff/w1_slave",O_RDONLY);

        if(fd<0)
        {
            printf("open file failture:%s\n",strerror(errno));
            perror("open file failture\n");
        }

        memset(buf,0,sizeof(buf));

        read(fd,buf,sizeof(buf));
        printf("buf:%s\n",buf);
        ptr=strstr(buf,"t=");

        if(!ptr)
        {
        
            printf("can not find 't='\n");
            return -1;
        
        }
        ptr += 2;
        printf("ptr:%s\n",ptr);
        *wendu= atof(ptr)/1000;
        printf("temprature: %f\n",*wendu);

        close(fd);
        
}
