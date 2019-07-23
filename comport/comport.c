/*********************************************************************************
 *      Copyright:  (C) 2019 LingYun<lingyun@email.com>
 *                  All rights reserved.
 *
 *       Filename:  comport.c
 i    Description:  串口编程
 *                 
 *        Version:  1.0.0(16/05/19)
 *         Author:  LingYun <lingyun@email.com>
 *      ChangeLog:  1, Release initial version on "16/05/19 18:58:42"
 *                 
 *********************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <strings.h>
#include <string.h>
#include "comport.h"

int comport_init(const char *dev_name, int baudrate, const char *settings)
 {
    st_comport *comport = NULL; //comport是指向串口结构体的指针
    
    memset(comport, 0, sizeof(st_comport)); //清空结构体
    strncpy(comport->dev_name, dev_name, DEVNAME_LEN); //串口路径写入结构体
    comport->baudrate = baudrate; //串口波特率写入结构体
    comport->parity = settings[0]; //串口奇偶校验位写入结构体
    comport->databit = settings[1]; //串口数据位写入结构体
    comport->stopbit = settings[2]; //串口停止位写入结构体
 }

int comport_open()
 {
    int open_fd = -1;
    st_comport *comport; //comport是指向串口结构体的指针 
    if((open_fd = open(comport->dev_name,O_RDWR|O_NOCTTY|O_NDELAY)) < 0)
    {
        printf("Open dev_name failure : %s\n",strerror(errno));
        return -1;
    }
    if(isatty(open_fd) == 0)
    {
        printf("open fd is not a terminal device\n");
        return -1;
    }
    
    comport->com_fd = open_fd; //串口打开的文件写入结构体
    
    return 0;    
}

int comport_set()
{
    st_comport *comport; //comport是指向串口结构体的指针
    struct termios *term; //term是指向termios结构体的指针

    switch(comport->baudrate) //获取波特率
    {
        case 4800:
                    cfsetispeed(term,B4800); //设置输入波特率
                    cfsetospeed(term,B4800); //设置输出波特率
                    break;
        
        case 9600:
                    cfsetispeed(term,B9600);
                    cfsetospeed(term,B9600);
                    break;
        
        case 19200:
                    cfsetispeed(term,B19200);
                    cfsetospeed(term,B19200);
                    break;

        case 38400:
                    cfsetispeed(term,B38400);
                    cfsetospeed(term,B38400);
                    break;

        default:
                    printf("Unknown baudrate : %s\n",strerror(errno));
                    return -1;
        
    }

    switch(comport->parity) //获取奇偶校验位
    {
        case 'n':
        case 'N':
                    term->c_cflag &= ~PARENB; //PARENB：产生奇偶校验
                    term->c_cflag &= ~INPCK; //INPCK：使奇偶校验起作用
                    break;

        case 's':
        case 'S':
                    term->c_cflag &= ~PARENB;
                    term->c_cflag &= ~CSTOPB; //使用两位停止位
                    break;

        case 'o':
        case 'O':
                    term->c_cflag |= PARENB;
                    term->c_cflag |= PARODD; //使用奇校验
                    term->c_cflag |= INPCK;
                    term->c_cflag |= ISTRIP; //使字符串剥离第八个字符，即校验位
                    break;

        case 'e':
        case 'E':
                    term->c_cflag |= PARENB;
                    term->c_cflag &= ~PARODD; //非奇校验，即偶校验
                    term->c_cflag |= INPCK;
                    term->c_cflag |= ISTRIP;
                    break;

        default:
                    printf("Unknown parity : %s\n",strerror(errno));
                    return -1;

    }

    switch(comport->databit) //获取数据位
    {
        case 5:
                    term->c_cflag &= ~CSIZE; //屏蔽其他标志位
                    term->c_cflag |= CS5; //数据位为5位
                    break;

        case 6:
                    term->c_cflag &= ~CSIZE;
                    term->c_cflag |= CS6;
                    break;

        case 7:
                    term->c_cflag &= ~CSIZE;
                    term->c_cflag |= CS7;
                    break;

        case 8:
                    term->c_cflag &= ~CSIZE;
                    term->c_cflag |= CS8;
                    break;

        default:
                    printf("Unknown bits : %s\n",strerror(errno));
                    return -1;

    }

    switch(comport->stopbit) //获取停止位
    {
        case 1:
                    term->c_cflag &= ~CSTOPB; //CSTOPB：使用两位停止位
                    break;

        case 2:
                    term->c_cflag |= CSTOPB;
                    break;

        default:
                    printf("Unknown stop : %s\n",strerror(errno));
                    return -1;

    }

    term->c_oflag &= ~OPOST; //OPOST:表示数据经过处理后输出
    term->c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG ); //使串口工作在原始模式下
    term->c_cc[VTIME] = 0; //设置等待时间为0
    term->c_cc[VMIN] = 1; //设置最小接受字符为1

    return 0;
}
 
int comport_read(int fd, char *buf, int bufsize)
{
    int rv = -1;
    
    if(NULL == buf)
    {
        printf("Cannot pull in null pointer!\n");
        return -1;
    }

    memset(buf,0,bufsize);
    if((rv = read(fd,buf,bufsize)) < 0)
    {
        printf("Read failure: %s\n",strerror(errno));
        return -1;
    }

    return rv;
}

int comport_write(int fd, char *buf, int bufsize)
{
    int rv = -1;

    if(NULL == buf)
    {
        printf("Cannot pull in null pointer!\n");
        return -1;
    }
    
    if((rv = write(fd,buf,bufsize)) < 0)
    {
        printf("Write failure : %s\n",strerror(errno));
        return -1;
    }

    printf("Write [%d] data to serialport :\n%s\n",fd,buf);
    
}

int comport_close(int fd)
{
    struct termios *term;
    tcflush(fd,TCIOFLUSH);
    if((tcsetattr(fd,TCSANOW,term)) != 0)
    {
        printf("Set to the original property failure:%s\n",strerror(errno));
    }

    close(fd);

    return 0;
}

