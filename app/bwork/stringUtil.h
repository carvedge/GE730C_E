//
//  stringUtil.h
//  onvifclient
//
//  Created by 彭闽 on 15/7/1.
//  Copyright (c) 2015年 Carvedge. All rights reserved.
//

#ifndef __onvifclient__stringUtil__
#define __onvifclient__stringUtil__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *  从字符串数组中查找指定的字符串
 *
 *  @param array 字符串数组，需以NULL结尾，否则请使用findnstr
 *  @param str   欲查找的字符串
 *
 *  @return 如果找到完全匹配的字符串，返回字符串指针的地址，否则返回NULL
 */
const char** findstr(const char** array, const char* str);

/**
 *  从字符串数组中查找指定的字符串
 *
 *  @param array 字符串数组
 *  @param size  数组长度
 *  @param str   欲查找的字符串
 *
 *  @return 如果找到完全匹配的字符串，返回字符串指针的地址，否则返回NULL
 */
const char** findnstr(const char** array, int size, const char* str);

/**
 *  分割一个字符串
 *
 *  @param str     字符串
 *  @param c       用于分割的字符
 *  @param splited 分割后字符串数组
 *
 *  @return 分割后的元素个数
 */
int strsplit(const char* str, const char c, char*** splited);

#endif