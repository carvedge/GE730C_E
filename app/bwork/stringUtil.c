//
//  stringUtil.c
//  onvifclient
//
//  Created by 彭闽 on 15/7/1.
//  Copyright (c) 2015年 Carvedge. All rights reserved.
//

#include "stringUtil.h"

const char** findstr(const char** array, const char* str) {
    for (const char** p = array; *p; p++) {
        if (!strcmp(*p, str)) {
            return p;
        }
    }
    return NULL;
}

const char** findnstr(const char** array, int size, const char* str) {
    for (int i = 0; i < size; i++) {
        if (strcmp(array[i], str) == 0) {
            return array + i;
        }
    }
    return NULL;
}

int strccount(const char* str, const char c) {
    int count = 0;
    for (const char* p = str; *p; p++) {
        if (*p == c) {
            count++;
        }
    }
    return count;
}

int strsplit(const char* str, const char c, char*** splited) {
    int count = strccount(str, c) + 1;
    if (str[strlen(str) - 1] == c) count--;
    *splited = calloc(count, sizeof(char*));
    
    const char* t = str;
    for (int i = 0; i < count; i++) {
        const char* p = t;
        t = strchr(p, c);
        (*splited)[i] = t ? strndup(p, (int)(t - p)) : strdup(p);
        t++;
    }
    
    return count;
}