//
//  RtmpSender.h
//  RtmpSender
//
//  Created by 彭闽 on 16/9/26.
//  Copyright © 2016年 Carvedge. All rights reserved.
//

#ifndef RtmpSender_h
#define RtmpSender_h

#include <librtmp/rtmp_sys.h>
#include <librtmp/log.h>

extern RTMP_LogLevel LogLevel;

/**
 *  Rtmp client initial.
 *
 *  @param url     rtmp server address.
 *  @param timeout rtmp send timeout setting.
 */
void RtmpInit(const char* url, int timeout);

/**
 *  Send binary data to rtmp server.
 *
 *  @param buffer    binary data array.
 *  @param size      binary data array length.
 *  @param type      rtmp data type.
 *  @param timeStamp rtmp timestamp.
 */
void RtmpSend(const uint8_t* buffer, uint32_t size, int type, uint32_t timeStamp);

/**
 *  Send avc header to rtmp server.
 *
 *  @param sps     Sequence parameter set array. (without start code!)
 *  @param spsSize Sequence parameter set length.
 *  @param pps     Picture parameter set array. (without start code!)
 *  @param ppsSize Picture parameter set length.
 */
void RtmpSendHeader(uint8_t* sps, uint32_t spsSize, uint8_t* pps, uint32_t ppsSize);

/**
 *  Send h264 frame to rtmp server.
 *
 *  @param buffer    h264 frame data array. (without start code!)
 *  @param size      h264 frame length.
 *  @param timeStamp rtmp timestamp.
 */
void RtmpSendH264Frame(const uint8_t* buffer, uint32_t size, uint32_t timeStamp);

/**
 *  Send aac audio data to rtmp server. (not implement!)
 *
 *  @param buffer    aac data array.
 *  @param size      aac data length.
 *  @param timeStamp rtmp timestamp.
 *  @param specInfo  specInfo.
 *  @param infoSize  specInfo size.
 */
void RtmpSendAACFrame(const uint8_t* buffer, uint32_t size, uint32_t timeStamp, const uint8_t* specInfo, uint32_t infoSize);

/**
 *  Release and clean rtmp resources.
 */
void RtmpClose();

#endif /* RtmpSender_h */
