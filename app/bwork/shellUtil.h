//
//  memoryUtil.h
//  onvifclient
//
//  Created by 彭闽 on 15/7/1.
//  Copyright (c) 2015年 Carvedge. All rights reserved.
//

#ifndef __onvifclient__shellUtil__
#define __onvifclient__shellUtil__

#include <stdlib.h>
#include <sys/types.h>
#include <stdbool.h>
#include <sys/vfs.h>
void _mkdir(char* cmd);
bool _isMounted();
struct statfs _getMountState();
#endif