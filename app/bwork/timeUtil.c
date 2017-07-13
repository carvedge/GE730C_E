//
//  memoryUtil.c
//  onvifclient
//
//  Created by 彭闽 on 15/7/1.
//  Copyright (c) 2015年 Carvedge. All rights reserved.
//

#include "timeUtil.h"
#include "time.h"

char * get_Year_Month_Day(){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char* ret = (char*)malloc(100);
	sprintf(ret,"%d%02d%02d",tm.tm_year + 1900, tm.tm_mon+1,tm.tm_mday);
	return ret;
}
char * get_TimeDetail(){

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char* ret = (char*)malloc(100);
	sprintf(ret,"%d%02d%02d%02d%02d%02d",tm.tm_year + 1900, tm.tm_mon+1,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec);
	return ret;
}
long getTImeInterVal(){
	time_t t;
    return time(&t);
}
