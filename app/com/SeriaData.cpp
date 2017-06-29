#include "SeriaData.h"


// -----------------------------------------------------------------------------
// DESCRIPTION: RTU CRC D￡?éμ???×??ú±í
// -----------------------------------------------------------------------------
static const unsigned char auchCRCHi[] = {
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,
    0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,
    0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,
    0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
    0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,
    0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
    0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,
    0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
    0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,
    0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,
    0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
    0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
    0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,
    0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
    0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,
    0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
    0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,
    0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,
    0x00,0xC1,0x81,0x40
};

// -----------------------------------------------------------------------------
// DESCRIPTION: RTU CRC D￡?éμ?μí×??ú±í
// -----------------------------------------------------------------------------
static const unsigned char auchCRCLo[] = {
    0x00,0xC0,0xC1,0x01,0xC3,0x03,0x02,0xC2,0xC6,0x06,0x07,0xC7,0x05,0xC5,
    0xC4,0x04,0xCC,0x0C,0x0D,0xCD,0x0F,0xCF,0xCE,0x0E,0x0A,0xCA,0xCB,0x0B,
    0xC9,0x09,0x08,0xC8,0xD8,0x18,0x19,0xD9,0x1B,0xDB,0xDA,0x1A,0x1E,0xDE,
    0xDF,0x1F,0xDD,0x1D,0x1C,0xDC,0x14,0xD4,0xD5,0x15,0xD7,0x17,0x16,0xD6,
    0xD2,0x12,0x13,0xD3,0x11,0xD1,0xD0,0x10,0xF0,0x30,0x31,0xF1,0x33,0xF3,
    0xF2,0x32,0x36,0xF6,0xF7,0x37,0xF5,0x35,0x34,0xF4,0x3C,0xFC,0xFD,0x3D,
    0xFF,0x3F,0x3E,0xFE,0xFA,0x3A,0x3B,0xFB,0x39,0xF9,0xF8,0x38,0x28,0xE8,
    0xE9,0x29,0xEB,0x2B,0x2A,0xEA,0xEE,0x2E,0x2F,0xEF,0x2D,0xED,0xEC,0x2C,
    0xE4,0x24,0x25,0xE5,0x27,0xE7,0xE6,0x26,0x22,0xE2,0xE3,0x23,0xE1,0x21,
    0x20,0xE0,0xA0,0x60,0x61,0xA1,0x63,0xA3,0xA2,0x62,0x66,0xA6,0xA7,0x67,
    0xA5,0x65,0x64,0xA4,0x6C,0xAC,0xAD,0x6D,0xAF,0x6F,0x6E,0xAE,0xAA,0x6A,
    0x6B,0xAB,0x69,0xA9,0xA8,0x68,0x78,0xB8,0xB9,0x79,0xBB,0x7B,0x7A,0xBA,
    0xBE,0x7E,0x7F,0xBF,0x7D,0xBD,0xBC,0x7C,0xB4,0x74,0x75,0xB5,0x77,0xB7,
    0xB6,0x76,0x72,0xB2,0xB3,0x73,0xB1,0x71,0x70,0xB0,0x50,0x90,0x91,0x51,
    0x93,0x53,0x52,0x92,0x96,0x56,0x57,0x97,0x55,0x95,0x94,0x54,0x9C,0x5C,
    0x5D,0x9D,0x5F,0x9F,0x9E,0x5E,0x5A,0x9A,0x9B,0x5B,0x99,0x59,0x58,0x98,
    0x88,0x48,0x49,0x89,0x4B,0x8B,0x8A,0x4A,0x4E,0x8E,0x8F,0x4F,0x8D,0x4D,
    0x4C,0x8C,0x44,0x84,0x85,0x45,0x87,0x47,0x46,0x86,0x82,0x42,0x43,0x83,
    0x41,0x81,0x80,0x40
};

/*************************************************************
 ?èê?: CRC16D￡?é2é±í3ìDò￡??ù?è?ì
 è??ú2?êy:???òêy×é????￡?D￡?é×??ú??êy
 3??ú2?êy: 16??CRCD￡?é??
 **************************************************************/
unsigned short CRC16_Table(unsigned char *puchMsg, unsigned short usDataLen)
{
    unsigned char uchCRCHi;// high byte of CRC initialized
    unsigned char uchCRCLo;// low byte of CRC initialized
    unsigned uIndex;// will index into CRC lookup table
    uchCRCHi = 0xFF;
    uchCRCLo = 0xFF;
    
    while (usDataLen--) {
        // calculate the CRC
        uIndex = uchCRCHi ^ (unsigned char)( *puchMsg++ );
        uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
        uchCRCLo = auchCRCLo[uIndex];
    }
    
    return ( uchCRCHi << 8 | uchCRCLo );
}


//串口设置
static int set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio;
    
    if (tcgetattr(fd, &newtio)  !=  0) {
        perror("SetupSerial 1");
        return -1;
    }
    
    newtio.c_oflag &= ~OPOST;//lrx add
    newtio.c_cflag &= ~CSIZE;//屏蔽其他标志位
    newtio.c_cflag &= ~CSIZE;//屏蔽其他标志位
    //lrx  newtio.c_cflag &= ~CRTSCTS;//不使用流控
    
    //printf("CVPLOUTINV = %ld .....\n", CVPLOUTINV);
    
    switch (nBits) {
        case 7:
            newtio.c_cflag |= CS7;
            break;
        case 8:
            newtio.c_cflag |= CS8;
            break;
            
        default:
            break;
    }
    
    //设置校验位
    switch (nEvent) {
        case 'O':                     //奇校验
            newtio.c_cflag |= PARENB;
            newtio.c_cflag |= PARODD;
            newtio.c_iflag |= (INPCK | ISTRIP);
            break;
        case 'E':                     //偶校验
            newtio.c_iflag |= (INPCK | ISTRIP);
            newtio.c_cflag |= PARENB;
            newtio.c_cflag &= ~PARODD;
            break;
        case 'N':                    //无校验
            newtio.c_cflag &= ~PARENB;
            newtio.c_iflag &= ~INPCK;
            //newtio.c_iflag |= IGNPAR;
            break;
        default:
            break;
    }
    
    //设置波特率
    switch (nSpeed) {
        case 2400:
            cfsetispeed(&newtio, B2400);
            cfsetospeed(&newtio, B2400);
            break;
        case 4800:
            cfsetispeed(&newtio, B4800);
            cfsetospeed(&newtio, B4800);
            break;
        case 9600:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
        case 115200:
            cfsetispeed(&newtio, B115200);
            cfsetospeed(&newtio, B115200);
            break;
            
        default:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
    }
    
    // 设置停止位
    if (nStop == 1) {
        newtio.c_cflag &=  ~CSTOPB;
    } else if (nStop == 2) {
        newtio.c_cflag |=  CSTOPB;
    }
    
    //修改输出模式，原始数据输出
    //newtio.c_oflag &= ~OPOST;
    newtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    
    newtio.c_oflag &= ~OPOST;
    newtio.c_oflag &= ~(ONLCR | OCRNL);    //添加的
    
    newtio.c_iflag &= ~(ICRNL | INLCR);
    newtio.c_iflag &= ~(IXON | IXOFF | IXANY);    //添加的
    
    newtio.c_lflag = 0; //lrx add 2016
    newtio.c_iflag = 0;//lrx add 2016
    newtio.c_oflag = 0;//lrx add 2016
    
    tcflush(fd, TCIFLUSH);//如果发生数据溢出，接收数据，但是不再读取
    
    //激活配置 (将修改后的termios数据设置到串口中）
    if ((tcsetattr(fd, TCSANOW, &newtio)) != 0) {
        perror("com set error");
        return -1;
    }
    
    printf("set done!\n");
    
    return 0;
}


//串口打开
static int open_port(int fd, int comport)
{
    char *dev[]={"/dev/ttyS0", "/dev/ttyS1", "/dev/ttyS2"};
    long  vdisable;
    
    if (comport == 1) {
        fd = open( "/dev/ttyS0", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == fd) {
            perror("Can't Open Serial Port");
            return (-1);
        } else {
            printf("open ttyS0 .....\n");
        }
    } else if (comport == 2) {
        fd = open("/dev/ttyS1", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == fd) {
            perror("Can't Open Serial Port");
            return (-1);
        } else {
            printf("open ttyS1 .....\n");
        }
    } else if (comport == 3) {
        fd = open( "/dev/ttyS2", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == fd) {
            perror("Can't Open Serial Port");
            return (-1);
        } else {
            printf("open ttyS2 .....\n");
        }
    }
    
    /*
     //设置串口为阻塞模式
     if (fcntl(fd, F_SETFL, 0) < 0) {
     printf("fcntl failed!\n");
     
        return -1;
     }
     
     
     if(fcntl(fd, F_SETFL, FNDELAY) < 0)//非阻塞，覆盖前面open的属性
     {
     printf("fcntl failed\n");
     }
     */
    
    return fd;
}

//串口初始化配置
static int UART_init(Device *pWireless)
{
    struct epoll_event epoll;
    
    pWireless->device_fd = open_port(0, pWireless->uart_index);//open ttyS0
    if (pWireless->device_fd == -1) {
        return -1;
    }
    
    //配置串口，波特率115200 数据位8位，无校验位，1位停止位
    if (set_opt(pWireless->device_fd, BAUDRATE, 8, 'N', 1) < 0) {
        perror("set_opt error");
        return -1;
    }
    
    pWireless->epoll_fd = epoll_create(LISTEN_OBJECT);//设置epoll
    if (pWireless->epoll_fd < 0) {
        perror("epoll_create error \n");
        return -1;
    }
    
    epoll.events = EPOLLIN;
    epoll.data.fd = pWireless->device_fd;
    
    if (epoll_ctl(pWireless->epoll_fd, EPOLL_CTL_ADD, pWireless->device_fd, &epoll) < 0) {
        fprintf(stderr, "epoll set insertion error: fd=%d \n", pWireless->device_fd);
        return -1;
    }
    
    return 0;
}

static int UART_deinit(Device *pWireless)
{
    if (pWireless->epoll_fd != -1)
        close(pWireless->epoll_fd);
    
    if (pWireless->device_fd != -1) {
        close(pWireless->device_fd);
    }
    
    return 0;
}

/*************************
 描    述： 读串口数据
 入口参数： pWireless ，   pBuff：读取的数据将存储在该缓冲区， count：期望读取数据的大小 getsize：实际读到的数据大小  timeout：超时(单位ms)
 出口参数： 无
 返 回 值:  成功为0
 非0失败
 ***************************/
int UARTGetData(Device *pWireless, unsigned char *pBuff, int count, int *getsize, int timeout)
{
    int i;
    int flg = 0;
    int id;
    int cnt = 0;
    unsigned char *p = NULL;
    int size = 0;
    
    p = pBuff;
    
    *getsize = 0;
    
    struct epoll_event events[LISTEN_OBJECT + 1];
    
    if (pBuff == NULL) {
        printf("recv buf is empty\r\n");
        return -1;
    }
    
    if (pWireless->device_fd == -1) {
        printf("device fd error\r\n");
        return -1;
    }
    printf("0 get buffer size is %d\n",*getsize);
    for (cnt = 0; cnt < 10; cnt ++) {
        id = epoll_wait(pWireless->epoll_fd, events, LISTEN_OBJECT, timeout);
        if (id < 0) {
            printf("error. epoll_wait...\n");
            return -1;
        } else if (id == 0) {
            if (flg) {
                break;
            } else {
                //printf("time out...\r\n");
                return -1;
            }
        } else {
            for (i = 0; i < id; i++) {
                if (events[i].events & EPOLLIN) {
                    //printf("start read.\r\n");
                    size = read(events[i].data.fd, p, count);
                    printf("1 get buffer size is %d\n",*getsize);
                    if (size < 0) {
                        printf("Do not find data in uart...\n");
                        return -1;
                    }
                    
                    p += size;
                    *getsize += size;
                    
                    flg = 1;
                }
            }
        }
    }
    return 0;
}

int UARTSendNByte(Device *pWireless, unsigned char *pBuff, int len)
{
    int cnt = 0;
    char sendbuf[36];//实测36个字节CPU占用最少
    
    if (pWireless->device_fd == -1) {
        return -1;
    }
    
    memset(sendbuf, 0x0, sizeof(sendbuf));
    
    memcpy(sendbuf, pBuff, len > sizeof(sendbuf) ? sizeof(sendbuf) : len);
    
#if 0
    if ((cnt = write(pWireless->device_fd, sendbuf, sizeof(sendbuf))) < 0) {
        return -1;
    }
#else
    if ((cnt = write(pWireless->device_fd, sendbuf, len)) < 0) {
        return -1;
    }
#endif
    
    //printf("uart send data length %d \n", cnt);
    
    return 0;
}

/*************************
 描    述： 初始化
 入口参数： pWireless
 出口参数： 无
 返 回 值:  成功为0
 非0失败
 ***************************/
int Hal_UART_init(Device *pWireless)
{
    if (UART_init(pWireless) < 0)
    {
        UART_deinit(pWireless);
        return INIT_ERROR;
    }
    
    return INIT_SUCCESS;
}

/*************************
 描    述： 去初始化
 入口参数： pWireless
 出口参数： 无
 返 回 值:  无
 ***************************/
void Hal_UART_deinit(Device *pWireless)
{
    UART_deinit(pWireless);
}

Device device;
/**
 *解析具体数据
 */
void parseDetail(unsigned char*data,size_t length){
   
   unsigned char Frame_Type[1];  
   memcpy(Frame_Type, data, 1);
   printf("Frame_Type is %02x\n",Frame_Type[0]);
   if (Frame_Type[0] == 0x02)
   {
    //
     if (length != 41)
     {
         return;
     }
     unsigned char Time[6]; 
     unsigned char Power_Data[19];
     unsigned char Temp_Data[2]; 
     unsigned char Gps_Data[12];
     unsigned char GPRS_Status[1];  
     
     memcpy(Time, data+1, 6);
     memcpy(Power_Data, data+7, 19);
     memcpy(Temp_Data, data+26, 2);
     memcpy(Gps_Data, data+28, 12);
     memcpy(GPRS_Status, data+40, 1);
     printf("Temp_Data data is %02x%02x\n",Temp_Data[0],Temp_Data[1]);
     printf("gps data is %02x%02x%02x%02x%02x\n",Gps_Data[0],Gps_Data[1],Gps_Data[2],Gps_Data[3],Gps_Data[4]);
       printf("GPRS_Status is %02x\n",GPRS_Status[0]);
     //
   }
   
}
void parse(unsigned char*data,size_t length){
    printf("parse data ,receive length is %d\n", length);
    if (0xA5 == data[0] && 0x5A == data[1])
    {
          //printf("parse length is %d\n",length);
          unsigned short crc = CRC16_Table(data, length-2);
          printf("calculate crc  is %02x%02x\n",crc&0xFF,(crc>>8)& 0xFF);
          printf("receiveCrc  is %02x%02x\n",data[length-2],data[length-1] );
          parseDetail(data+4,length-6);
          
    }
}
int UARTSendData(unsigned char Frame_Type){

    if (Frame_Type != 0x05)
    {
        EngineMsg_t  msg;
        msg.head = MSG_HEAD;
        msg.frameType = Frame_Type;
        msg.packetLength = sizeof(EngineMsg_t);
        msg.CRC16 = CRC16_Table((unsigned char *)&msg, sizeof(EngineMsg_t)-2);
        
        if (UARTSendNByte(&device, (unsigned char *)&msg, sizeof(msg)) < 0) {
            printf("uart send data error \r\n");
            return SEND_ERROR;
        }
        else{
            printf("uart send data success \r\n");
            //(Device *pWireless, unsigned char *pBuff, int count, int *getsize, int timeout)
            int ret = 0;
            usleep(200000);
            unsigned char * receiveMsg = new unsigned char[1024];
            UARTGetData(&device,receiveMsg,1024,&ret,1);
            if (ret > 0)
            {
                parse(receiveMsg,ret);
                printf("-get size is %d\n",ret); 
            }
            free(receiveMsg);

        }
    }
    else{
        EngineMsg_config  msg;
        msg.head = MSG_HEAD;
        msg.frameType = Frame_Type;
        memset(msg.Bid,0x31,17);
        msg.Server_IP[0] = 47;
        msg.Server_IP[1] = 89;
        msg.Server_IP[2] = 51;
        msg.Server_IP[3] = 239;
        msg.packetLength = sizeof(EngineMsg_config);
        msg.CRC16 = CRC16_Table((unsigned char *)&msg, sizeof(EngineMsg_config)-2);
        
        if (UARTSendNByte(&device, (unsigned char *)&msg, sizeof(msg)) < 0) {
            printf("uart send data error \r\n");
            return SEND_ERROR;
        }
        else{
            printf("uart send data success \r\n");
            //(Device *pWireless, unsigned char *pBuff, int count, int *getsize, int timeout)
            int ret = 0;
            usleep(200000);
            unsigned char * receiveMsg = new unsigned char[1024];
            UARTGetData(&device,receiveMsg,1024,&ret,1);
            if (ret > 0)
            {
                parse(receiveMsg,ret);
                printf("-get size is %d\n",ret); 
            }
            free(receiveMsg);
        }
    }
    return 0;
    
}
