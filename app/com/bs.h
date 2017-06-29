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
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include<boost/function.hpp>
#include<boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
using namespace std;
using boost::asio::ip::tcp;
using namespace boost::asio;  
using namespace boost::posix_time;
class bs
{
public:
  bs(boost::asio::io_service& io_service,string ip,string port);
  tcp::socket& socket(){ return socket_; }
  void start();
  void handle_read(const boost::system::error_code& error,size_t bytes_transferred);
  void handle_write(const boost::system::error_code& error);
  void snd_hbt();
  void handle_connect(boost::system::error_code ec,boost::optional<boost::system::error_code>* out_ec);
  void handle_timeout(const boost::system::error_code& error);
  void handle_recon_timeout(const boost::system::error_code& error);
  void handle_hbt_timeout(const boost::system::error_code& error);
  
private:
  tcp::socket socket_;
  tcp::resolver::iterator endpoint_iterator;
  enum { max_length = 1024 };
  unsigned char data_[max_length];
private:
  io_service&  io_service_;  
  deadline_timer timer_;
  deadline_timer recon_timer_;
  deadline_timer hbt_timer_;
  bool wrt_end;
  int lfe_;
  unsigned char Bid[17];
  void sendData(unsigned char Frame_Type);
  void parse(unsigned char*data,size_t length);
  void parseDetail(unsigned char*data,size_t length);
  string ip_;
  string port_;
};
