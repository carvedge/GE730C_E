#ifndef _SERIADATA_H_
#define _SERIADATA_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <linux/input.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <getopt.h>
#include <sys/time.h>
#include <pthread.h>
#include <sys/epoll.h>//使用epoll

#include <termios.h>
#include "msg.h"
#include<boost/function.hpp>
#include "fun.h"

extern Device device;
extern unsigned char *DeviceBid;
extern unsigned char *DeviceSip;
unsigned short CRC16_Table(unsigned char *puchMsg, unsigned short usDataLen);
int Hal_UART_init(Device *pWireless);
int UARTSendNByte(Device *pWireless, unsigned char *pBuff, int len);
int UARTGetData(Device *pWireless, unsigned char *pBuff, int count, int *getsize, int timeout);
void Hal_UART_deinit(Device *pWireless);
void parseDetail(unsigned char*data,size_t length);
void parse(unsigned char*data,size_t length);
int UARTSendData(unsigned char Frame_Type);
int UARTConfigData(unsigned char*ip,unsigned char*bid);
void SetPowerXCallbackFunc(boost::function<void(char*,char*,char*) >func );
 
#endif