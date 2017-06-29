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