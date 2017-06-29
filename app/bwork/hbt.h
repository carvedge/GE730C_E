#ifndef HBT_H
#define HBT_H
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/function.hpp>
#include <boost/thread/thread.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/format.hpp>


using boost::asio::ip::tcp;
class Hbt
{
public:
    Hbt(boost::asio::io_service& io_service);
    void start(const std::string& server,const std::string& port);
    void httpReq();
    /**
    *通知服务器关闭了机芯
    */
    void httpNotifyClose();
    void httpXPower(char*powerData,char*tempData,char*gpsData);
    void UARTConfig();
private:
    void handle_timeout(const boost::system::error_code& err);
    boost::asio::io_service & io_service_;  
    boost::asio::deadline_timer timer_;
    std::string server_;
    std::string path_;
    std::string port_;
    tcp::resolver::iterator endpoint_iterator_mip; 

    //sip
    tcp::resolver::iterator endpoint_iterator_sip;
    int  resloved_sip;
private:
    
    long lastInterval;
    int resloved_mip;//是否已经解析主机

    bool isConnected;//是否已经连上网络

    bool _UARTConfig;//是否已经配置过单片机
private:
    int heartBeatCount;//如果不看视频，并且heartBeatCount>5 则不发送心跳
    
private:
    void noNetWorkSaveImages();
};
#endif
