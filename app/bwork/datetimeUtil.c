//
//  datetimeUtil.c
//  onvifclient
//
//  Created by 彭闽 on 15/7/1.
//  Copyright (c) 2015年 Carvedge. All rights reserved.
//

#include "datetimeUtil.h"

char* timeStringShortNow() {
    time_t now = time(NULL);
    return timeStringShort(localtime(&now));
}

char* timeStringShort(struct tm* time) {
    char *result = (char*)malloc(16);
    sprintf(result, "%04d%02d%02d_%02d%02d%02d", time->tm_year+1900, time->tm_mon+1, time->tm_mday, time->tm_hour, time->tm_min, time->tm_sec);
    return result;
}

char* timeStringNow() {
    time_t now = time(NULL);
    return timeString(localtime(&now));
}

char* timeString(struct tm* time) {
    char *result = (char*)malloc(20);
    sprintf(result, "%04d-%02d-%02d %02d:%02d:%02d", time->tm_year+1900, time->tm_mon+1, time->tm_mday, time->tm_hour, time->tm_min, time->tm_sec);
    return result;
}

char* timeStringTZNow() {
    time_t now = time(NULL);
    return timeStringTZ(gmtime(&now));
}

char* timeStringTZ(struct tm* time) {
    char *result = (char*)malloc(21);
    sprintf(result, "%04d-%02d-%02dT%02d:%02d:%02dZ", time->tm_year+1900, time->tm_mon+1, time->tm_mday, time->tm_hour + 1, time->tm_min, time->tm_sec);
    return result;
}

long long current_timestamp() {
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // caculate milliseconds
   // printf("milliseconds: %lld\n", milliseconds);
    return milliseconds;
}