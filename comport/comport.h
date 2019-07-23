/********************************************************************************
 *      Copyright:  (C) 2019 LingYun<lingyun@email.com>
 *                  All rights reserved.
 *
 *       Filename:  comport.h
 *    Description:  This head file 
 *
 *        Version:  1.0.0(29/05/19)
 *         Author:  LingYun <lingyun@email.com>
 *      ChangeLog:  1, Release initial version on "29/05/19 20:34:16"
 *                 
 ********************************************************************************/
#include <termios.h>

#define DEVNAME_LEN 32
typedef struct _comport{
        char    parity;//奇偶检验位
        int     databit;//数据位
        int     com_fd;//fd
        int     baudrate;//波特率
        int     stopbit;//停止位
        char    dev_name[DEVNAME_LEN];//串口路径
}st_comport;

extern int comport_init(const char *dev_name, int baudrate, const char *settings);

extern int comport_open();

extern int comport_set();

extern int comport_read(int fd, char *buf, int bufsize);

extern int comport_write(int fd, char *buf, int bufsize);

extern int comport_close(int fd);
