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
#include<unistd.h> 
#include"crc-itu-t.c"
#include<stdlib.h>

#define TLV_MAX_SIZE    16
#define TLV_MIN_SIZE    6
#define HEAD            0xfd
#define ON              1
#define OFF             0

enum
{
    TAG_LOGON=1,
    TAG_CAMERA,
    TAG_LED,
};

int unpack(char *buf,int bytes);

int main()
{
    int                 bytes;
    char                array[]={0x03,0xfd,0x03,0x06,0x00,0xb2,0x0e,0xfd,0x02,0x23,0x21,0xaa,0xa1,0xfd,0x02,0x06,0x00,0x85,0x3e};
    
    bytes = unpack(array,sizeof(array));

    printf("The array has %d bytes!\n",sizeof(array));
    
    printf("array has %d bytes now!\n",bytes);

    return 0;

}

int unpack(char *buf,int bytes)
{
    int                 i;
    char                *ptr=NULL;
    int                 len;
    unsigned short      crc,val;

    if( !buf )
    {
        printf("Invailed input!\n");
        return 0;
    }

again:

    if( bytes < TLV_MIN_SIZE )                                      //数据小于一帧
    {
		printf("TLV packet is too short!\n");
        printf("Wait for continue input...\n");
        return bytes;                                               //返回半帧的值
    }

    for( int i=0; i<bytes; i++)                                     //数据大于一帧，开始遍历
    {
        if( (unsigned char)buf[i] == HEAD )
        {

            if(bytes-i < 2)                                         //这一帧中没有（length）长度
            {
                printf("\nTLV packet is too short.it is incomplete\n");
                printf("\nWait for continue input...\n");
                memmove(buf,&buf[i],bytes-i);                       //把半帧移到buf的开端
                return bytes-i;                                     //返回半帧的值
            }

            ptr = &buf[i];
            len = ptr[2];

            if(len < TLV_MIN_SIZE || len > TLV_MAX_SIZE)            //这一帧中（length）长度错误
            {
                memmove(buf,&ptr[2],bytes-i-2);                     //把这一帧扔掉
                goto again;                                         //继续遍历
            }

            if(len > bytes-i)                                       //（length）比剩下的还要长
            {
                memmove(buf,ptr,bytes-i);                           //把半帧移到buf的开端
                printf("TLV packet is incomplete!\n");
                printf("Wait for continue input...\n");
                return bytes-i;                                     //返回半帧的值
            }

            crc = crc_itu_t(MAGIC_CRC,(unsigned char *)ptr,(len-2));
            val = bytes_to_ushort( (unsigned char *)&ptr[len-2],2 );

            if(val != crc)                                          //两次的CRC不同
            {
                printf("CRC check error\n");
                memmove(buf,&ptr[2],bytes-i-2);                     //把这一帧扔掉
                bytes = bytes-i-2;
                goto again;                                         //继续遍历
            }

            for(int i=0; i<len; i++)
            {
                switch(ptr[i+1])
                {
                    case TAG_LOGON:
                                    printf("TAG_LOGON:\n");

                    case TAG_CAMERA:
                                    printf("TAG_CAMERA:\n");

                    case TAG_LED:
                                    printf("TAG_LED:\n");
                }
                
                printf("0x%02x ",ptr[i]);
            }
            printf("\n");

            memmove(buf,&ptr[len],bytes-i-len);                     //把这一帧扔掉

            bytes = bytes-i-len;
            goto again;                                             //继续遍历
        }
    } 
}


