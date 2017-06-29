#ifndef _FUN_H
#define _FUN_H
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
unsigned short RTU_CRC( unsigned char * puchMsg, unsigned short usDataLen );

int  ucha2int(unsigned char *buff);
void int2uchar(int value,unsigned char *ch);
void X2Str(unsigned char* s_src, int n_len_src, char* s_dst);
int setSystemTime(time_t timep);
//使用gps（单片机）时间给系统对时
void setSystemTimeWithGps(unsigned char*time);


#endif