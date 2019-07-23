/*********************************************************************************
 *      Copyright:  (C) 2019 LingYun<lingyun@email.com>
 *                  All rights reserved.
 *
 *       Filename:  comport.c
 *    Description:  串口编程
 *                 
 *        Version:  1.0.0(16/05/19)
 *         Author:  LingYun <lingyun@email.com>
 *      ChangeLog:  1, Release initial version on "16/05/19 18:58:42"
 *                 
 ********************************************************************************/
#include <stdio.h>              //标准输入输出定义
#include <fcntl.h>              //文件控制定义
#include <assert.h>
#include <termios.h>            //POSIX中断控制定义
#include <unistd.h>             //Unix标准函数定义
#include <string.h>
#include <stdlib.h>             //标准函数库定义
#include <sys/time.h>
#include <sys/types.h>
#include <errno.h>              //错误号定义

int uart_open(int fd,const char *pathname);
int uart_config(int fd,int baude,int c_flow, int bits, char parity, int stop);
int safe_read(int fd, char *vptr, size_t len);
int uart_read(int fd, char *r_buf, size_t lenth);//串口读取数据
int uart_close(int fd);

int main()
{
    static int fd;
    static int ret;

    char r_buf[1024];
    bzero(r_buf,1024);
    
    fd = uart_open(fd, "/dev/ttyUSB0");//选择的是ttyUSB0串口
    if(fd == -1)
    {
        fprintf(stderr,"open failed!");
        exit(EXIT_FAILURE);
    }
    printf("open successed!\n");
    
    if(uart_config(fd,4800,'N',8,'N',1) == -1)
    {
        fprintf(stderr,"configure failed!");
        exit(EXIT_FAILURE);
    }
    printf("config successed!\n");
    
    while (1)
    {
        ret = uart_read(fd,r_buf,1024);
        if(ret == -1)
        {
            fprintf(stderr, "uart_read failed!");
            exit(EXIT_FAILURE);
        }
        printf("buf:%s \n", r_buf);
    }
    
    ret = close(fd);
    if(ret == -1)
    {
        fprintf(stderr, "close failed!");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

int uart_open(int fd, const char *pathname)
{
    assert(pathname);//检查串口路径是否正确
    fd = open(pathname, O_RDWR | O_NOCTTY | O_NDELAY);//打开串口
    if(fd == -1)
    {
        perror("uart open failed!");
        return -1;
    }
    if(fcntl(fd, F_SETFL, 0)<0)//设置串口非阻塞
    {
        perror("fcntl failed!");
        return -1;
    }
    return fd;
}

int uart_config(int fd, int baude, int c_flow, int bits, char parity,int stop)
{
    struct termios uart;
    if(tcgetattr(fd,&uart) != 0)//获取结构体的属性，成功返回0
    {
        perror("tcgetattr failed!");
        return -1;
    }
    
    switch(baude)
    {
        case 4800:
                    cfsetispeed(&uart,B4800);//设置输入波特率
                    cfsetospeed(&uart,B4800);//设置输出波特率
                    break;
        case 9600:
                    cfsetispeed(&uart,B9600);
                    cfsetospeed(&uart,B9600);
                    break;
        case 19200:
                    cfsetispeed(&uart,B19200);
                    cfsetospeed(&uart,B19200);
                    break;
        case 38400:
                    cfsetispeed(&uart,B38400);
                    cfsetospeed(&uart,B38400);
                    break;
        default:
                    fprintf(stderr,"Unknown baude!");
                    return -1;
    }

    switch(c_flow)
    {
        case 'N':
        case 'n':
                    uart.c_cflag &= ~CRTSCTS;//不进行硬件流控制
                    break;
        case 'H':
        case 'h':
                    uart.c_cflag |= CRTSCTS;//进行硬件流控制
                    break;
        case 'S':
        case 's':
                    uart.c_cflag |= (IXON | IXOFF | IXANY);//进行软件流控制
                    break;
        default:
                    fprintf(stderr,"Unknown c_cflag!");
                    return -1;
    }

    switch(bits)
    {
        case 5:
                    uart.c_cflag &= ~CSIZE;//屏蔽其他标志位
                    uart.c_cflag |= CS5;//数据位为5位
                    break;
        case 6:
                    uart.c_cflag &= ~CSIZE;
                    uart.c_cflag |= CS6;
                    break;
        case 7:
                    uart.c_cflag &= ~CSIZE;
                    uart.c_cflag |= CS7;
                    break;
        case 8:
                    uart.c_cflag &= ~CSIZE;
                    uart.c_cflag |= CS8;
                    break;
        default:
                    fprintf(stderr,"Unknown bits!");
                    return -1;
    }

    switch(parity)
    {
        case 'n':
        case 'N':
                    uart.c_cflag &= ~PARENB;//PARENB：产生奇偶校验
                    uart.c_cflag &= ~INPCK;//INPCK：使奇偶校验起作用
                    break;
        case 's':
        case 'S':
                    uart.c_cflag &= ~PARENB;
                    uart.c_cflag &= ~CSTOPB;//使用两位停止位
                    break;
        case 'o':
        case 'O':
                    uart.c_cflag |= PARENB;
                    uart.c_cflag |= PARODD;//使用奇校验
                    uart.c_cflag |= INPCK;
                    uart.c_cflag |= ISTRIP;//使字符串剥离第八个字符，即校验位
                    break;
        case 'e':
        case 'E':
                    uart.c_cflag |= PARENB;
                    uart.c_cflag &= ~PARODD;//非奇校验，即偶校验
                    uart.c_cflag |= INPCK;
                    uart.c_cflag |= ISTRIP;
                    break;
        default:
                    fprintf(stderr,"Unknown parity!");
                    return -1;
    }

    switch(stop)
    {
        case 1:
                    uart.c_cflag &= ~CSTOPB;//CSTOPB：使用两位停止位
                    break;
        case 2:
                    uart.c_cflag |= CSTOPB;
                    break;
        default:
                    fprintf(stderr,"Unknown stop!");
                    return -1;
    }
    uart.c_oflag &= ~OPOST;//OPOST:表示数据经过处理后输出

    if(tcsetattr(fd,TCSANOW,&uart)<0)//激活配置，失败返回-1
    {
        perror("tcsetattr failed!");
        return -1;
    }
    uart.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG );//使串口工作在原始模式下
    uart.c_cc[VTIME] = 0;//设置等待时间为0
    uart.c_cc[VMIN] = 1;//设置最小接受字符为1
    tcflush(fd,TCIFLUSH);//清空输入缓冲区

    return 0;
}

int safe_read(int fd, char *vptr, size_t len)
{
    size_t left;//剩下的空间大小
    left = len;
    size_t nread;//读取的空间大小
    char *ptr;
    ptr = vptr;
    while(left > 0)
    {
        if ((nread = read(fd, ptr, left)) <= 0)
        {
                break;
        }
        left -= nread;//read成功后，剩余要读取的字节自减
        ptr += nread;//指针向后移，避免后读到的字符覆盖先读到的字符
    }
    return (len - left);
}

int uart_read(int fd, char *r_buf, size_t lenth)
{
    int ret;
    fd_set rfds;
    struct timeval time;
    size_t cnt = 0;

    /* 将读文件描述符加入描述符集合 */
    FD_ZERO(&rfds);
    FD_SET(fd,&rfds);

    /* 设置超时为15s */
    time.tv_sec = 15;
    time.tv_usec = 0;

    /* 实现多路IO */
    ret = select(fd+1, &rfds ,NULL, NULL, &time);
    switch (ret)
    {
        case -1:
                    fprintf(stderr,"select error!");
                    break;
        case 0:
                    fprintf(stderr, "time over!");
                    break;
        default:
                    cnt = safe_read(fd, r_buf, lenth);
                    if(cnt == -1)
                    {
                        fprintf(stderr, "safe read failed!");
                        return -1;
                    }
                    return cnt;
    }
}

int uart_close(int fd)
{
    assert(fd);//assert先检查文件描述符是否存在
    close(fd);
    return 0;
}
