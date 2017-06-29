
//
// blocking_tcp_echo_client.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include <pthread.h>
#include "SeriaData.h"
#include "fxn.h"


void initValues(){
    string ini_file = "./ini/0.ini";
    Ini *ini = new Ini();
    ini->setFileName(ini_file);
    _mip = ini->get( "cfg.mip" );
    _bid = ini->get( "cfg.bid" );
    cout<<_mip<<endl;

    string ini_file1 = "./ini/7.ini";
    Ini ini1;
    ini1.setFileName(ini_file1);
    _vsn = boost::lexical_cast<int>(ini1.get( "cfg.vsn" ));

    Ini* ini2 = Ini::Instance(); //预置位
    ini2->setFileName("./ini/1.ini");
   
}
boost::asio::io_service io_service_;


int main(int argc, char* argv[])
{
	initValues();
   // LOG("0.mod start\n");

    device.uart_index = 1;
    Hal_UART_init(&device);

    io_service_.reset();
    
    _hbt = new Hbt(io_service_);
    SetPowerXCallbackFunc(boost::bind(&Hbt::httpXPower,_hbt,_1,_2,_3 ));
    _hbt->start(_mip,"80");
    _client = new bs(io_service_);
    io_service_.run(); 
    return 0;
}
