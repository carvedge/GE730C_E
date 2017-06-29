#ifndef _MSG_H_
#define _MSG_H_


#define MSG_SUCCESS               (0)
#define CRC_ERROR 				  (-1)
#define LOST_DATA				  (-2)
#define UNKNOW_CMD				  (-3)
#define INIT_SUCCESS              (0)
#define INQUIRY_SUCCESS           (0)
#define SET_PARAM_SUCCESS         (0)
#define STUDY_SUCCESS             (0)
#define STOP_STUDY_SUCCESS        (0)
#define INIT_ERROR                (-12)
#define OPEN_FILE_ERROR           (-13)
#define NO_INIT                   (-14)
#define INQUIRY_ERROR             (-4)
#define SET_PARAM_ERROR           (-5)
#define SEND_ERROR                (-6)
#define GET_ERROR                 (-7)
#define GET_PARAM_ERROR           (-8)
#define PARAM_ERROR               (-9)
#define HEADER_ERROR              (-10)
#define GET_STATUS_ERRORR         (-11)
#define UPDATE_SEND_DATA_ERROR    (-31)
#define UPDATE_SEND_STATUS_AP     (-32)
#define UPDATE_SEND_STATUS_LD     (-33)
#define UPDATE_FAILD              (-33)
#define RUN_TO_AP_ERROR           (-41)
#define STUDY_ERROR               (-51)
#define STOP_STUDY_ERROR          (-52)
#define STUDY_TIME_OUT            (-53)
#define UNKNOWN_COMMAND           (-254)
#define UNKNOWN_ERROR             (-255)


enum __Error {
    CMD_SUCCESS = 0x00,
    CMD_FAILED,
    CMD_CRC_ERROR,
    CMD_UNKNOW,
    CMD_HEAD_ERROR,
};

enum __ItemType {
    SOC_INTERRUPT = 0x1,
    SOC_EVENT,
    SOC_ALARM,
    SOC_HEART,
    SOC_HAVE4GDATA,
    SOC_MSG,
};


typedef struct REPLY_ARGS {
    unsigned char  ack;
    unsigned char  reserve1;
    unsigned short reserve2;
} cmd_args;

typedef struct CONF_CMD_ARGS {
    unsigned char cmd;
    unsigned char cmd1;
    unsigned short index;
} conf_cmd_args;

typedef struct Packet {
    int packetHeard;
    int crc;
    int packetCnt;
    int packetCmd;
} strPacket;

typedef struct __Alarm433 {
    strPacket packet;
    int keyValue;
    int address;
} Alarm433;



/*???*/
struct __Msg {
    int packet_head;
    int crc;
    int packet_lenght;
    int packet_comand;
    int packet_reply;
};

typedef struct __DateTime {
    unsigned short year;
    unsigned short mon;
    unsigned short day;
    unsigned short hour;
    unsigned short min;
    unsigned short sec;
} DateTime;



#define PACKET_SIZE	            64
#define FILE_BUFFER	            128
#define APROM_MODE	            1
#define LDROM_MODE	            2



#define HIGH16(iTemp) ((iTemp >> 16) & 0xFFFF)
#define LOW16(iTemp)  (iTemp & 0xFFFF)

#define HIGH8(iTemp)  ((iTemp >> 8) & 0xFF)
#define LOW8(iTemp)   (iTemp & 0xFF)

#define PACKET_HEADER                     (0xAAAAAAAA)
#define PACKET_GETHEADER                  (0xBBBBBBBB)

#define TIMEOUT (1000)//1000ms
#define LISTEN_OBJECT 1//监听对象的个数
#define TEST_CNT	5
#define BAUDRATE	9600
#define CVPLOUTINV   000100000000	/*8进制 vpl: for inverse of RTS */
#define ALARM_PORT_NUM  8

/***************************************/

typedef void (*__pfunc)(int);
typedef void *(*__pthreadfunc)(void *);

typedef struct __Device {
    int device_fd;//设备描述符
    int epoll_fd;//poll句柄
    int uart_index;//串口设备号
    pthread_t thread_id;//针对串口查询线程的
    __pthreadfunc thread_func;//串口的线程函数指针
} Device;

enum __MsgCmd {
    SEND_UID = 0x10,
    CTRL_IO,
};

//?óáD
typedef struct __queue {
    int 	front; //?óí· 
    int 	rear;  //?ó?2
    int 	size;  //?óáD′óD?
} Queue; 

#define ALARM_QUEUE_SIZE 20


#pragma pack(1)
typedef struct __EngineMsg {
    unsigned short  head;
    unsigned short  packetLength;
    unsigned char   frameType;
    unsigned short  CRC16;
} EngineMsg_t;
typedef struct __EngineMsgConfig {
    unsigned short  head;
    unsigned short  packetLength;
    unsigned char   frameType;
    unsigned char   Bid[17];
    unsigned char   Server_IP[4];
    unsigned short  CRC16;
} EngineMsg_config;
#pragma pack()

#define MSG_HEAD    0x5AA5


#endif
