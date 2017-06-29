//
//  RtmpSender.c
//  RtmpSender
//
//  Created by 彭闽 on 16/9/26.
//  Copyright © 2016年 Carvedge. All rights reserved.
//

#include "RtmpSender.h"
#include <stdlib.h>
#include <string.h>

RTMP* Rtmp = NULL;
RTMP_LogLevel LogLevel = RTMP_LOGINFO;

void RtmpInit(const char* url, int timeout) {
    RtmpClose();
    
    Rtmp = RTMP_Alloc();
    
    RTMP_LogSetLevel(LogLevel);
    RTMP_Init(Rtmp);
    Rtmp->Link.timeout = timeout;
    
    RTMP_SetupURL(Rtmp, (char*)url);
    
    RTMP_EnableWrite(Rtmp);
    
    if (!RTMP_Connect(Rtmp, NULL)) printf("Unable to connect the server.\n");
    if (!RTMP_ConnectStream(Rtmp, 0)) printf("Unable to connect the stream.\n");
}

void RtmpClose() {
    if (!Rtmp) return;
    RTMP_Close(Rtmp), RTMP_Free(Rtmp), Rtmp = NULL;
}

void RtmpSend(const uint8_t* buffer, uint32_t size, int type, uint32_t timeStamp) {
    RTMPPacket packet;
    RTMPPacket_Reset(&packet);
    RTMPPacket_Alloc(&packet, size);
    
    packet.m_packetType = type;
    packet.m_nBodySize = size;
    packet.m_nTimeStamp = timeStamp;
    packet.m_nChannel = 0x04;
    packet.m_nInfoField2 = Rtmp->m_stream_id;
    packet.m_headerType = RTMP_PACKET_SIZE_LARGE;
    
    memcpy(packet.m_body, buffer, size);
    
    RTMP_SendPacket(Rtmp, &packet, 0);
    
    RTMPPacket_Free(&packet);
}

void RtmpSendH264Frame(const uint8_t* buffer, uint32_t size, uint32_t timeStamp) {
    uint8_t* send = (uint8_t*)malloc(size + 9);
    uint8_t* p = send;
    
    int type = buffer[0] & 0x1f;
    
    if (type == 0x05 || type == 0x07 || type == 0x08)
        *p++ = 0x17;
    else
        *p++ = 0x27;
    
    // AVC NALU
    *p++ = 0x01, *p++ = 0x00, *p++ = 0x00, *p++ = 0x00;
    
    // NALU Size
    *p++ = size >> 24;
    *p++ = size >> 16;
    *p++ = size >> 8;
    *p++ = size & 0xff;
    
    memcpy(p, buffer, size);
    
    RtmpSend(send, size + 9, RTMP_PACKET_TYPE_VIDEO, timeStamp);
    
    free(send);
}

void RtmpSendAACFrame(const uint8_t* buffer, uint32_t size, uint32_t timeStamp, const uint8_t* specInfo, uint32_t infoSize) {
    
}

void RtmpSendHeader(uint8_t* sps, uint32_t spsSize, uint8_t* pps, uint32_t ppsSize)
{
    uint8_t* sendBuffer = malloc(1024);
    uint8_t* p = sendBuffer;
    
    //FrameType and CodecID
    *p++ = 0x17; //FrameType:1 key frame | CodecID:7 AVC
    
    //Video Data Begin
    *p++ = 0x00; //AVC PacketType: AVC sequence header
    *p++ = 0x00, *p++ = 0x00, *p++ = 0x00; //Composition time : 0x000000;
    
    //AVC DecoderConfiguration Record
    *p++ = 0x01; //configuration version
    *p++ = sps[1]; // sps[1] AVCProfileIndication
    *p++ = sps[2]; // sps[2] profile_compatibility
    *p++ = sps[3]; // sps[3] AVCLevelIndication
    *p++ = 0xff; //lengthSizeMinusOne always 0xff;
    
    //Numbers of SPS
    *p++ = 0xe1; //0xe1 & 0x1f == 1
    
    //SPS Data Length
    *p++ = spsSize >> 8;
    *p++ = spsSize & 0xff;
    
    //SPS Data
    int i=0;
    for (i = 0; i < spsSize; ++i)
        *p++ = sps[i];
    
    //Numbers of PPS
    *p++ = 0x01; //0x01 & 0xff == 1
    
    //PPS Data Length
    *p++ = ppsSize >> 8;
    *p++ = ppsSize & 0xff;
    
    //PPS Data
    for (i = 0; i < ppsSize; ++i)
        *p++ = pps[i];
    
    uint32_t length = (uint32_t)(p - sendBuffer);
    RtmpSend(sendBuffer, length, RTMP_PACKET_TYPE_VIDEO, 0);
    
    free(sendBuffer);
}
