/*********************************************************************************
 *      Copyright:  (C) 2019 LingYun<lingyun@email.com>
 *                  All rights reserved.
 *
 *       Filename:  tlv.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(02/04/19)
 *         Author:  LingYun <lingyun@email.com>
 *      ChangeLog:  1, Release initial version on "02/04/19 20:07:35"
 *                 
 ********************************************************************************/

#include<stdio.h>
#include<string.h>
#include"crc-itu-t.c"
#include"crc-itu-t.h"
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>


#define     HEAD                0xFD                    //定义一个报头
#define     BUF_SIZE            128                     //定义一个buffer的大小
#define     TLV_FIXED_SIZE      5                       //固定的TLV字节流大小，不包含value值
#define     TLV_MINI_SIZE       (TLV_FIXED_SIZE+1)      //TLV字节流的最小值，value的值为1个字节
#define     SERVER_IP           "192.168.0.16"
#define     SERVER_PORT         2000
#define     ON                  1
#define     OFF                 0

enum                                                    //使用枚举，Tag的值会自加
{
    TAG_LOGON=1,                                        //登陆Tag
    TAG_CAMERA,                                         //摄像头Tag
    TAG_LED,                                            //照明灯Tag
};


int     pack_logon(char *buf, int size, char *psw);     //声明登陆（logon）封装函数
int     pack_camera(char *buf, int size, int cmd);      //声明摄像头（camera）封装函数
int     pack_led(char *buf, int size, int cmd);         //声明照明灯（led）封装函数
void    dump_buf(char*type,char *data,int len);         //声明dump_buf


int main(int argc, char **argv)
{
        struct sockaddr_in      serv_addr;
        int                     conn_fd = -1;
        char                    buf[BUF_SIZE];
        int                     bytes;                  //一个封装函数的字节流的个数
        
        conn_fd=socket(AF_INET,SOCK_STREAM,0);
        if(conn_fd<0)
        {
            printf("creat socket failure: %s\n",strerror(errno));
            return -1;
        }

        memset(&serv_addr,0,sizeof(serv_addr));
        serv_addr.sin_family=AF_INET;
        serv_addr.sin_port=htons(SERVER_PORT);
        inet_aton(SERVER_IP,&serv_addr.sin_addr);
        
        if( connect(conn_fd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
        {
            printf("connect to server [%s:%d] failure: %s\n",SERVER_IP,SERVER_PORT,strerror(errno));
            return -1;
        }
        printf("Conneect to server [%s:%d] \n",SERVER_IP,SERVER_PORT);


        bytes = pack_logon(buf,sizeof(buf),"iot@yun");  //设置登陆（logon）函数
        dump_buf("Logon",buf,bytes);                    //设置dump_buf函数，把所有的字节流都放到dump_buf里面
        ptr = buf;

        bytes = pack_camera(buf,sizeof(buf),ON);        //设置摄像头（camera）函数
        dump_buf("Camera",buf,bytes);                            

        bytes = pack_led(buf, sizeof(buf), ON);         //设置照明灯（led）函数
        dump_buf("LED",buf,bytes);
}




int pack_logon(char *buf, int size, char *psw)          //定义登陆（logon）封装函数
//buf   缓冲区
//size  缓冲区的大小
//psw   密码
{
    unsigned short      crc=0;                          //crc校验值
    int                 pack_len=0;
    int                 data_len=0;
    int                 ofset=0;                        //buf的索引位置

    if( !buf || !psw || size<TLV_MINI_SIZE )            //判断
    {
        printf("Invalid input argument!\n");            //无效输入内容
        return 0;
    }
    
    buf[ofset]=HEAD;                                    //buf[0]设为报头（HEAD）
    ofset += 1;                                         //索引加1

    buf[ofset]=TAG_LOGON;                               //buf[1]设为类型（Tag）
    ofset += 1;

    if( strlen(psw)<size-TLV_FIXED_SIZE )               //psw是一个字节流，strlen（psw）计算它的长度，buf减去5等于value的值
        data_len = strlen(psw);                         //如果密码的长度小于value的值，就用密码的值
    else
        data_len = size-TLV_FIXED_SIZE;                 //否则就用value的最大值

    pack_len = data_len+TLV_FIXED_SIZE;                 //封装TLV的总长度

    buf[ofset]=pack_len;                                //buf[2]设为长度（Length）
    ofset += 1;

    memcpy(&buf[ofset],psw,data_len);                   //从psw拷贝到buf[3,4,5...]
    ofset += data_len;                                  //索引加data_len个字节
    
    crc = crc_itu_t(MAGIC_CRC, buf, ofset);             //调用crc函数,此函数在头文件中
    ofset += 2;                                         //索引加2

    return ofset;                                       //返回索引值
}




int pack_camera(char *buf, int size, int cmd)
//buf   缓冲区
//size  缓冲区的大小
//cmd   开关
{
    unsigned short      crc=0;
    int                 pack_len = TLV_FIXED_SIZE+1;    //开关（cmd）的值只有一个字节

    if( !buf || size<TLV_MINI_SIZE )
    {
        printf("Invalid input argument!\n");
        return 0;
    }

    buf[0] = HEAD;
    buf[1] = TAG_CAMERA;
    buf[2] = pack_len;
    buf[3] = ( ON==cmd ) ? 0x01:0x00;

    crc = crc_itu_t(MAGIC_CRC,buf,4);
    ushort_to_bytes(&buf[4],crc);

    return pack_len;
}




int pack_led(char *buf, int size, int cmd)
{
    unsigned short      crc=0;
    int                 pack_len = TLV_FIXED_SIZE+1;    //开关（cmd）的值只有一个字节
    
    if( !buf || size<TLV_MINI_SIZE )
    {
        printf("Invalid input argument!\n");
        return 0;
    }

    buf[0] = HEAD;
    buf[1] = TAG_LED;
    buf[2] = pack_len;
    buf[3] = ( ON==cmd ) ? 0x01:0x00;

    crc = crc_itu_t(MAGIC_CRC,buf,4);
    ushort_to_bytes(&buf[4],crc);

    return pack_len;
}





void dump_buf(char *type,char *data, int len)           //定义dump_buf
//data  指针，指着buf的首地址
//len   buf的长度
{
    if( type )
    {
        printf("%s:\n",type);
    }
    
    int     i;

    for(i=0; i<len; i++)
    {
        printf("0x%02X ",data[i]);
    }
    printf("\n");

}
