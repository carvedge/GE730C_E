#ifndef _FXN_H
#define _FXN_H

#include <iostream>
#include <cstdlib> 
#include <boost/lexical_cast.hpp>
#include "ini.h"

#include "bs.h"
#include "hbt.h"
#include "Log.h"
using namespace std;

extern int _vsn;
extern string _mip;
extern string _sip;

extern string _bid;

extern bs * _client;
extern Hbt* _hbt;
extern int vidState;//0:关闭 1:开启

extern int capture_work;//1 正在进行截图，如果在巡视，退出巡视；0 没有在截图；
extern int zoom_is_work;//1 正在进行定时巡视，如果有一键巡视命令，等待定时巡视结束后再进行一键巡视
extern int y_zoom_is_work;//1 正在进行一键巡视，如果有定时巡视命令，等待一键巡视结束后再进行定时巡视


/**
*type : 1 /opt/app/ram/1.txt
*type : 2 /opt/app/ram/2.txt
*
*/
int writeToFile(char *str,int length,int type);

int writeToFile(char *str,int length,char* fileName);

int  getZoomValue();
int  setZoomVaule(int value);
#endif