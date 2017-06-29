//
//  datetimeUtil.h
//  onvifclient
//
//  Created by 彭闽 on 15/7/1.
//  Copyright (c) 2015年 Carvedge. All rights reserved.
//

#ifndef __onvif__datetimeUtil__
#define __onvif__datetimeUtil__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 *  获取当前本地时间的短格式字符串
 *
 *  @return (ex: 19000101_000000)
 */
char* timeStringShortNow();
/**
 *  获取指定时间的短格式字符串
 *
 *  @param time 指定的时间
 *
 *  @return (ex: 19000101_000000)
 */
char* timeStringShort(struct tm* time);

/**
 *  获取当前本地时间的标准字符串
 *
 *  @return (ex: 1900-01-01 00:00:00)
 */
char* timeStringNow();
/**
 *  获取指定时间的标准字符串
 *
 *  @param time 指定的时间
 *
 *  @return (ex: 1900-01-01 00:00:00)
 */
char* timeString(struct tm* time);

/**
 *  获取当前UTC时间的TZ字符串
 *
 *  @return (ex: 1900-01-01T00:00:00Z)
 */
char* timeStringTZNow();
/**
 *  获取指定UTC时间的TZ字符串
 *
 *  @param time 指定的时间
 *
 *  @return (ex: 1900-01-01T00:00:00Z)
 */
char* timeStringTZ(struct tm* time);


//打印当前时间
long long current_timestamp() ;


#endif