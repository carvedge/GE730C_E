#include "hbt.h"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/join.hpp>
#include <string>
#include "Log.h"
#include "SeriaData.h"
#include "fxn.h"

string pm1_id;
int zoom_cmd[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
bool zoom_cmd_true = false;

extern "C"{
    #include <sys/vfs.h>
    #include "timeUtil.h"
    #include "datetimeUtil.h"
    #include "shellUtil.h"
}
using namespace boost::asio;
using boost::lexical_cast;  
using boost::bad_lexical_cast; 
Hbt::Hbt(boost::asio::io_service& io_service)
         :io_service_( io_service ),
          timer_(io_service),
		  y_timer_(io_service),
		  d_timer_(io_service)
{
    lastInterval = 0;
    _UARTConfig = false;
    resloved_sip = 0;
    resloved_mip = 0;
    vidState = 0;
    heartBeatCount = 0;	
      //LOG("start 0");
    UARTSendData(1);//获得时间
}

void Hbt::y_handle_timeout(const boost::system::error_code& err)
{
	if (err)  
    {  
        std::cout << "time error: " << err.message() << "\n";
    } 
	else
	{
		if(zoom_is_work == 0)
		{
			boost::function0<void>z=boost::bind(&Hbt::httpReq_y,this);
            boost::thread thrd(z);
		}
		else
		{
			y_timer_.expires_from_now(boost::posix_time::milliseconds(1000));
			y_timer_.async_wait(boost::bind(&Hbt::y_handle_timeout, this,boost::asio::placeholders::error));
		}
	}
}

void Hbt::d_handle_timeout(const boost::system::error_code& err)
{
	if (err)  
    {  
        std::cout << "time error: " << err.message() << "\n";
    } 
	else
	{
		if(y_zoom_is_work == 0)
		{
			boost::function0<void>z=boost::bind(&Hbt::capture_d,this);
            boost::thread thrd(z);
		}
		else
		{
			d_timer_.expires_from_now(boost::posix_time::milliseconds(1000));
			d_timer_.async_wait(boost::bind(&Hbt::d_handle_timeout, this,boost::asio::placeholders::error));
		}
	}
}

void Hbt::start(const std::string& server, const std::string& port)
{

    
    server_ = server;
    port_ = port;
    timer_.expires_from_now(boost::posix_time::milliseconds(3000));
    timer_.async_wait(boost::bind(&Hbt::handle_timeout, this,boost::asio::placeholders::error));
    tcp::resolver resolver(io_service_);
    tcp::resolver::query query(server_, port_);

    resloved_mip = 0;
    try { 
         //LOG("start 1");
        endpoint_iterator_mip = resolver.resolve(query);
    }
    catch (boost::system::system_error& e) {
         //LOG("start 2");
         std::cout <<e.what()<<"\n"; 
         return;
    }
     //LOG("start 3");
    resloved_mip = 1;
}
void Hbt::UARTConfig(){
    //UARTConfigData(unsigned char*ip,unsigned char*bid)
    if (!_UARTConfig)
    {
        

        vector<string> vStr;
        boost::split( vStr, _sip, boost::is_any_of( "." ), boost::token_compress_on);
        if (vStr.size() == 4)
        {
            unsigned char   Bid[17];
            unsigned char   Server_IP[4];
            sprintf((char*)Bid,_bid.c_str());
            Server_IP[0] = atoi(vStr[0].c_str());
            Server_IP[1] = atoi(vStr[1].c_str());
            Server_IP[2] = atoi(vStr[2].c_str());
            Server_IP[3] = atoi(vStr[3].c_str());
            UARTSendData(0x07);
            printf("deviceip is %02x.%02x.%02x.%02x,DeviceBid is %s\n",Server_IP[0],DeviceSip[1],DeviceSip[2],DeviceSip[3],(char*)DeviceBid);
            
            bool compareIP = (Server_IP[0] == DeviceSip[0] && Server_IP[1] == DeviceSip[1] && Server_IP[2] == DeviceSip[2] && Server_IP[3] == DeviceSip[3]);
            int compareBid = strcmp((char*)DeviceBid,_bid.c_str());
            

            if (!compareIP || compareBid != 0)
            {
                 UARTConfigData(Server_IP,Bid);
            }           
             _UARTConfig = true;
        }
        
    }
    

}
void Hbt::noNetWorkSaveImages(){
    if (!_isMounted())
    {
        return;
    }

    char * filename = new char[100];
    sprintf(filename,"/tmp/DataDisk/app/image/%s/%s_0.jpg",get_Year_Month_Day(),get_TimeDetail());
    //取图片
    char * cmd = (char*)malloc(200);
    sprintf(cmd,"CmdSnapShot 1920 1080 80");
    writeToFile(cmd,strlen(cmd),2);
    sleep(3);
    memset(cmd,0,200);
    sprintf(cmd,"cp -f /tmp/snapshot.jpg %s",filename);
    printf("mv cmd is %s\n",cmd);
    writeToFile(cmd,strlen(cmd),2);
    sleep(3);
    free(cmd);
    free(filename);
}
void Hbt::httpReq()
{
    //LOG("httpReq 0");
    /* ******************************************************** */
    printf("-------------------------------hbt 0\n");

    if(resloved_mip == 0){
        try {
            tcp::resolver resolver(io_service_);
            tcp::resolver::query query(server_, port_);
            endpoint_iterator_mip = resolver.resolve(query);
        }
        catch (boost::system::system_error& e) {
            std::cout <<e.what()<<"\n"; 
             
            return;
        }
    }
    resloved_mip = 1;
    tcp::socket socket(io_service_);

    boost::system::error_code ec = boost::asio::error::host_not_found;
    tcp::resolver::iterator iterator = endpoint_iterator_mip;
    tcp::resolver::iterator end;
    while(ec && iterator != end) {
        printf("connect iterator ,ec is %s\n", ec.message().c_str());
        socket.close();
        socket.connect(*iterator++, ec);
    }
    //如果没有链接上，再次拨号 最多拨号 3次
    static int dialog_4g = 6;
    if(ec){
        printf("connect iterator failed,ec is %s\n", ec.message().c_str());
        //have not network connect,then per hour save preset images
        resloved_mip =0;
        isConnected = false;
        --dialog_4g;
        printf("dialog_4g is %d\n", dialog_4g);
        if ( dialog_4g%2 == 0 && dialog_4g > 0)
        {
            printf("reconnect 4g\n");
            //LOG("no connect and connect 4g up ");
            char * cmd = (char*)malloc(100);
            sprintf(cmd,"/etc/connect4G.sh down");
            writeToFile(cmd,strlen(cmd),2);
            sleep(3);
            memset(cmd,0,100);
            sprintf(cmd,"/etc/connect4G.sh up &");
            writeToFile(cmd,strlen(cmd),2);
            free(cmd);
            heartBeatCount = 0;
        }
        else if(dialog_4g == 0){
            char * mkdirname = new char[100];
            sprintf(mkdirname,"mkdir /tmp/DataDisk/app/image/%s",get_Year_Month_Day());
            writeToFile(mkdirname,strlen(mkdirname),2);
            sleep(2);
            free(mkdirname);
        }
        else if(dialog_4g == -1){
            //保存当前图片
            heartBeatCount = 2;
            printf("heartBeatCount is %d,and noNetWorkSaveImages\n",heartBeatCount);
            noNetWorkSaveImages();
        }
		if(dialog_4g < 0)
			vidState = 0;
         
       // //LOG("execXBeat connect failed,bid is %s,ec is %s\n",bid_,ec.message().c_str());
        return;
    }
    
    printf("-------------------------------hbt 1\n");
    std::ostringstream stringStream;
    //http://box.carvedge.com/svr/box.php?act=hbt&bid=1436196841
   // http://".$mip."/svr/box.php?act=hbt&bid=".$bid."&vsn=".$_vsn
    
   
    stringStream <<"/svr/box.php?act=h&bid="<<_bid<<"&v0="<<_vsn;

    
    path_ = stringStream.str();
   
    cout<<path_<<endl;
    
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream << "GET " << path_ << " HTTP/1.0\r\n";
    request_stream << "Host: " << server_ << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    boost::asio::write(socket, request,ec);
    if( ec ){
        cout<<222<<endl;
        return;
    }

    boost::asio::streambuf response;
    boost::asio::read_until(socket, response, "\r\n",ec);
     if( ec ){
         return;
    }
    std::istream response_stream(&response);
    std::string http_version;
    response_stream >> http_version;
    unsigned int status_code;
    response_stream >> status_code;
    std::string status_message;
    std::getline(response_stream, status_message);
    if (!response_stream || http_version.substr(0, 5) != "HTTP/")
    {
        std::cout << "Invalid response\n";
        return ;
    }
    if (status_code != 200)
    {
        std::cout << "Response returned with status code " << status_code << "\n";
        return ;
    }
    // Read the response headers, which are terminated by a blank line.
    boost::asio::read_until(socket, response, "\r\n\r\n",ec);
     if( ec ){
         return;
    }
    //Process the response headers.
    std::string header;
    while (std::getline(response_stream, header) && header != "\r");
    //the heart-beat reply.
    std::string reply_;
    std::getline(response_stream ,reply_);

    while(boost::asio::read(socket, response, boost::asio::transfer_at_least(1), ec));

/* **************************************************************
 * heart-beat parser
 * ************************************************************** */
    printf("==reply is %s \n",reply_.c_str());
    isConnected = true;
    //LOG("httpReq response is %s\n",reply_.c_str());
    lastInterval =  getTImeInterVal ();
    vector<string> vStr;
    boost::split( vStr, reply_, boost::is_any_of( "," ), boost::token_compress_on);
    //sip,视频开关，截图，电源数据，版本，时间戳
    if (vStr.size()<7)
    {
        return;
    }
    _sip = vStr[0];
    if (!_client->_inited)
    {
        _client->init(_sip,"1940",_bid);
    }
    if (heartBeatCount == 1 && _isMounted())
    {
        //上传
        char * cmd = (char*)malloc(150);
        sprintf(cmd,"/tmp/DataDisk/app/uploadunconnectimage.sh %s %s &",_sip.c_str(),_bid.c_str());
        printf("cmd is %s\n", cmd);
        writeToFile(cmd,strlen(cmd),2);
        free(cmd);
        sleep(2);
    }
    UARTConfig();
   // writeToFile((char*)_sip.c_str(),strlen(_sip.c_str()),"/opt/app/ram/sip.txt");
    //处理视频  
    int ooo=boost::lexical_cast<int>(vStr[1]);
    vidState = ooo;
    if(1 == ooo){
        printf("------------------------- should open video\n");
        std::stringstream scmd;
    
        scmd << "/tmp/DataDisk/app/1.mod";
        scmd<<" ' '  'rtmp://"<<_sip<<":1935/live/"<<_bid<<"' &";

        printf("cmd is :%s\n", scmd.str().c_str());
        writeToFile((char*)scmd.str().c_str(),strlen(scmd.str().c_str()),1);
       
    }
    else if(0 == ooo){
        char * cmd = new char[100];
        sprintf(cmd,"killall 1.mod");
        writeToFile(cmd,strlen(cmd),1);
        free(cmd);
       
    }
    //处理截图
    if (vStr[2].length() != 0)
    {
        vector<string> vPtz;
        boost::split( vPtz, vStr[2], boost::is_any_of( "|" ), boost::token_compress_on);
		int i = 0;
        for (vector<string>::iterator iter=vPtz.begin();iter!=vPtz.end();++iter)
        {
            int zoom  = boost::lexical_cast<int>(*iter);	
			if(zoom == 0)
				continue;
			if((zoom != 0) && (i < 20))			
			{
				zoom_cmd[i] = zoom;
				zoom_cmd_true = true;
				i++;
			}			
        }
		i = 0;
		if(zoom_cmd_true == true)
		{
			zoom_cmd_true = false;
			d_timer_.expires_from_now(boost::posix_time::milliseconds(1000));
			d_timer_.async_wait(boost::bind(&Hbt::d_handle_timeout, this,boost::asio::placeholders::error));			
		}
    }
    
    /*int status=0;
    try  
    {  
        status=boost::lexical_cast<int>(vStr[2]); // 将字符串转化为整数  
    }  
    catch(bad_lexical_cast&)    // 转换失败会抛出一个异常  
    {  
        status=0;  
    }  
    if (status == 1)
    {
        char * cmd = new char[200];
        memset(cmd,0,200);
        sprintf(cmd,"CmdSnapShot 1920 1080 80");
        writeToFile(cmd,strlen(cmd),2);
        sleep(3);
        memset(cmd,0,200);
        //http://slv:1936/svr/box.php?act=g&bid=123456001&pre=1 pre:预置点序号0~19
        //curl -F 'dat=@0.jpg' 'http://box.carvedge.com/svr/box.php?act=gpm&bid=123456001&pre=1'
        sprintf(cmd, "/tmp/DataDisk/app/curl -F 'dat=@/tmp/snapshot.jpg' 'http://%s:1936/svr/box.php?act=g&bid=%s&pre=0'", _sip.c_str(),_bid.c_str());
        printf("cmd is %s\n",cmd);
        writeToFile(cmd,strlen(cmd),2);
        free(cmd);
        sleep(1);
    }    */
    

    //处理电源数据
    if (boost::lexical_cast<int>(vStr[3]) == 1)
    {
        UARTSendData(1);
    }
    //更新应用版本
    int vsn=boost::lexical_cast<int>(vStr[4]);
    if (vsn > _vsn)
    {
        printf("require update vsn\n");
        char*cmd =(char*)malloc( 200 );
        if (cmd)
        {
            sprintf(cmd,"/tmp/DataDisk/app/vsn.sh http://%s/vsn/%d.tar.gz",_mip.c_str(),vsn);
            printf("update cmd is %s\n",cmd);
            writeToFile(cmd,strlen(cmd),2);
            free(cmd);
        }
    }
    {
        string ctime = vStr[vStr.size()-1];
        unsigned int cctime = atoi(ctime.c_str());
        time_t t = time(NULL);
        if(t-cctime > 100 || cctime-t>100){
            //setSystemTime(cctime);
        }
    }
    //处理一键巡视
     if (vStr[6].length() != 0)
    {
        vector<string> vPtz;
        printf("zoom_is_work is%d.\n",zoom_is_work);
        boost::split( vPtz, vStr[6], boost::is_any_of( "|" ), boost::token_compress_on);
        for (vector<string>::iterator iter=vPtz.begin();iter!=vPtz.end();++iter)
        {
            int cmd_y  = boost::lexical_cast<int>(*iter);
            printf("cmd_y is %d\n", cmd_y);
            if((cmd_y == 0) || (capture_work == 1))
            {
                break;
            }
            if(cmd_y == 8)
            {
				pm1_id = vPtz[1];
                y_timer_.expires_from_now(boost::posix_time::milliseconds(1000));
				y_timer_.async_wait(boost::bind(&Hbt::y_handle_timeout, this,boost::asio::placeholders::error));
            }
        }
    }
    //LOG("httpReq end response is %s\n",reply_.c_str());
    printf("httpReq_heartbeat close.\n");
    socket.close();   
}

void Hbt::capture_d()//定时巡视
{
	int resetToDefaultZoom = 0;
	for(int i = 0; i < 20; i++)
	{
		if(capture_work == 1)
			break;
		if(zoom_cmd[i] != 0)
		{
            printf("zoom is %d\n", zoom_cmd[i]);
            zoom_is_work = 1;
            resetToDefaultZoom = 1;
            heartBeatCount = 1;
            char * cmd = new char[200];
            memset(cmd,0,200);
        //    if (zoom != 1)
            {
                sprintf(cmd,"AutoFocus -Z %d",zoom_cmd[i]);
                printf("--------------------------------------------test test ++++++++++++++++++++++++++++++++++++++++++++++++cmd is %s\n", cmd);
                writeToFile(cmd,strlen(cmd),2);
                sleep(8);
            }
            memset(cmd,0,200);
            sprintf(cmd,"CmdSnapShot 1920 1080 80");
            writeToFile(cmd,strlen(cmd),2);
            sleep(2);
            memset(cmd,0,200);
            //判断截图是否成功
            int snapshot_exist = 1;           
            snapshot_exist = access("/tmp/snapshot.jpg",F_OK);
            //printf("access is %d\n", snapshot_exist);
            if(snapshot_exist == -1)
            {
                sprintf(cmd,"CmdSnapShot 1920 1080 80");
                writeToFile(cmd,strlen(cmd),2);
                sleep(2);               
                memset(cmd,0,200);            
            }
            //http://slv:1936/svr/box.php?act=g&bid=123456001&pre=1 pre:预置点序号0~19
            //curl -F 'dat=@0.jpg' 'http://box.carvedge.com/svr/box.php?act=gpm&bid=123456001&pre=1'
            sprintf(cmd, "/tmp/DataDisk/app/curl -F 'dat=@/tmp/snapshot.jpg' 'http://%s:1936/svr/box.php?act=g&bid=%s&pre=%d&flg=0'", _sip.c_str(),_bid.c_str(),zoom_cmd[i]-1);
            printf("cmd is %s\n",cmd);
            writeToFile(cmd,strlen(cmd),2);
            free(cmd);
            sleep(5);
             snapshot_exist = access("/tmp/snapshot.jpg",F_OK);
            printf("access is %d\n", snapshot_exist);
            //int fremove = remove("/tmp/snapshot.jpg");
            int remove_count = 0;
            while((access("/tmp/snapshot.jpg",F_OK) == 0)&&(remove_count <3))
            {
                remove("/tmp/snapshot.jpg");
                snapshot_exist = access("/tmp/snapshot.jpg",F_OK);
                printf("access is %d\n", snapshot_exist);
                sleep(1);
                remove_count++;
                if(access("/tmp/snapshot.jpg",F_OK) != 0)
                    break;         
			}
            //printf("fremove is %d\n", fremove);
		}
		else if(zoom_cmd[i] == 0)
		{
			break;
		}
	}
	for(int j = 0; j < 20; j++)
	{
		zoom_cmd[j] = 0;
	}
	zoom_is_work = 0;
	if (1 == resetToDefaultZoom)
	{
		char * cmd = new char[200];
		sprintf(cmd,"AutoFocus -Z 1");
		writeToFile(cmd,strlen(cmd),2);
		free(cmd);
	}
}

void Hbt::httpReq_y()
{	
	printf("httpReq_y start.\n");	
    if(resloved_mip == 0){
        try {
            tcp::resolver resolver(io_service_);
            tcp::resolver::query query(server_, port_);
            endpoint_iterator_mip = resolver.resolve(query);
        }
        catch (boost::system::system_error& e) {
            std::cout <<e.what()<<"\n"; 
             
            return;
        }
    }
    resloved_mip = 1;
    tcp::socket socket(io_service_);

    boost::system::error_code ec = boost::asio::error::host_not_found;
    tcp::resolver::iterator iterator = endpoint_iterator_mip;
    tcp::resolver::iterator end;
    while(ec && iterator != end) {
        printf("connect iterator ,ec is %s\n", ec.message().c_str());
        socket.close();
        socket.connect(*iterator++, ec);
    }
    if(ec){
        return;
    }
    printf("httpReq_y start.\n");
    std::ostringstream stringStream;
    //http://ivs2.carvedge.com/svr/box.php?act=y&bid=01230021704100009
   // http://".$mip."/svr/box.php?act=hbt&bid=".$bid."&vsn=".$_vsn
    
   
    stringStream <<"/svr/box.php?act=y&bid="<<_bid;

    
    path_ = stringStream.str();
   
    cout<<path_<<endl;
    
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream << "GET " << path_ << " HTTP/1.0\r\n";
    request_stream << "Host: " << server_ << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    boost::asio::write(socket, request,ec);
    if( ec ){
        cout<<222<<endl;
        return;
    }

    boost::asio::streambuf response;
    boost::asio::read_until(socket, response, "\r\n",ec);
     if( ec ){
         return;
    }
    std::istream response_stream(&response);
    std::string http_version;
    response_stream >> http_version;
    unsigned int status_code;
    response_stream >> status_code;
    std::string status_message;
    std::getline(response_stream, status_message);
    if (!response_stream || http_version.substr(0, 5) != "HTTP/")
    {
        std::cout << "Invalid response\n";
        return ;
    }
    if (status_code != 200)
    {
        std::cout << "Response returned with status code " << status_code << "\n";
        return ;
    }
    // Read the response headers, which are terminated by a blank line.
    boost::asio::read_until(socket, response, "\r\n\r\n",ec);
     if( ec ){
         return;
    }
    //Process the response headers.
    std::string header;
    while (std::getline(response_stream, header) && header != "\r");
    //the heart-beat reply.
    std::string reply_;
    std::getline(response_stream ,reply_);

    while(boost::asio::read(socket, response, boost::asio::transfer_at_least(1), ec));

/* **************************************************************
 * heart-beat parser
 * ************************************************************** */
    printf("==httpReq_y_reply is %s \n",reply_.c_str());
    vector<string> vPtz;
    boost::split( vPtz, reply_, boost::is_any_of( "," ), boost::token_compress_on);
    int resetToDefaultZoom = 0;
    for (vector<string>::iterator iter=vPtz.begin();iter!=vPtz.end();++iter)
    {
        if(capture_work == 1)
                break;
		y_zoom_is_work = 1;
        int zoom_y  = boost::lexical_cast<int>(*iter);
        printf("zoom_y is %d\n", zoom_y);
        resetToDefaultZoom = 1;
        heartBeatCount = 1;
        char * cmd = new char[200];
        memset(cmd,0,200);
    //    if (zoom != 1)
        {
            sprintf(cmd,"AutoFocus -Z %d",zoom_y);
            printf("--------------------------------------------test test ++++++++++++++++++++++++++++++++++++++++++++++++cmd is %s\n", cmd);
            writeToFile(cmd,strlen(cmd),2);
            sleep(8);
        }
        memset(cmd,0,200);
        sprintf(cmd,"CmdSnapShot 1920 1080 80");
        writeToFile(cmd,strlen(cmd),2);
        sleep(2);
        memset(cmd,0,200);
        //判断截图是否成功
        int snapshot_exist = 1;           
        snapshot_exist = access("/tmp/snapshot.jpg",F_OK);
        //printf("access is %d\n", snapshot_exist);
        if(snapshot_exist == -1)
        {
            sprintf(cmd,"CmdSnapShot 1920 1080 80");
            writeToFile(cmd,strlen(cmd),2);
            sleep(2);               
            memset(cmd,0,200);            
        }
        //http://slv:1936/svr/box.php?act=g&bid=123456001&pre=1 pre:预置点序号0~19
        //curl -F 'dat=@0.jpg' 'http://box.carvedge.com/svr/box.php?act=gpm&bid=123456001&pre=1'
        sprintf(cmd, "/tmp/DataDisk/app/curl -F 'dat=@/tmp/snapshot.jpg' 'http://%s:1936/svr/box.php?act=g&pm1=%s&bid=%s&pre=%d&flg=1'", _sip.c_str(), pm1_id.c_str(),_bid.c_str(),zoom_y-1);
        printf("cmd is %s\n",cmd);
        writeToFile(cmd,strlen(cmd),2);
        free(cmd);
        sleep(5);
        snapshot_exist = access("/tmp/snapshot.jpg",F_OK);
        printf("access is %d\n", snapshot_exist);
        //int fremove = remove("/tmp/snapshot.jpg");
        while(access("/tmp/snapshot.jpg",F_OK) == 0)
        {
            remove("/tmp/snapshot.jpg");
            snapshot_exist = access("/tmp/snapshot.jpg",F_OK);
            printf("access is %d\n", snapshot_exist);
            sleep(1);
            if(access("/tmp/snapshot.jpg",F_OK) != 0)
                break;         
        }
        //printf("fremove is %d\n", fremove);
    }
	y_zoom_is_work = 0;
    if (1 == resetToDefaultZoom)     
    {
        char * cmd = new char[200];
        sprintf(cmd,"AutoFocus -Z 1");
        writeToFile(cmd,strlen(cmd),2);
        free(cmd);
    }
    socket.close();
}

void Hbt::httpNotifyClose(){
    //LOG("httpReq 0");
    /* ******************************************************** */
    printf("-------------------------------hbt 0\n");

    if(resloved_mip == 0){
        try {
            tcp::resolver resolver(io_service_);
            tcp::resolver::query query(server_, port_);
            endpoint_iterator_mip = resolver.resolve(query);
        }
        catch (boost::system::system_error& e) {
            std::cout <<e.what()<<"\n"; 
             
            return;
        }
    }
    resloved_mip = 1;
    tcp::socket socket(io_service_);

    boost::system::error_code ec = boost::asio::error::host_not_found;
    tcp::resolver::iterator iterator = endpoint_iterator_mip;
    tcp::resolver::iterator end;
    while(ec && iterator != end) {
        printf("connect iterator ,ec is %s\n", ec.message().c_str());
        socket.close();
        socket.connect(*iterator++, ec);
    }
    
    
    if(ec){
        //have not network connect,then per hour save preset images
        resloved_mip =0;
        return;
    }
    
    printf("-------------------------------hbt 1\n");
    std::ostringstream stringStream;
    //http://box.carvedge.com/svr/box.php?act=hbt&bid=1436196841
   // http://".$mip."/svr/box.php?act=hbt&bid=".$bid."&vsn=".$_vsn
    
   
    stringStream <<"/svr/box.php?act=q&bid="<<_bid;

    
    path_ = stringStream.str();
   
    cout<<path_<<endl;
    
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream << "GET " << path_ << " HTTP/1.0\r\n";
    request_stream << "Host: " << server_ << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    boost::asio::write(socket, request,ec);
    if( ec ){
        cout<<222<<endl;
        return;
    }

    boost::asio::streambuf response;
    boost::asio::read_until(socket, response, "\r\n",ec);
     if( ec ){
         return;
    }
    std::istream response_stream(&response);
    std::string http_version;
    response_stream >> http_version;
    unsigned int status_code;
    response_stream >> status_code;
    std::string status_message;
    std::getline(response_stream, status_message);
    if (!response_stream || http_version.substr(0, 5) != "HTTP/")
    {
        std::cout << "Invalid response\n";
        return ;
    }
    if (status_code != 200)
    {
        std::cout << "Response returned with status code " << status_code << "\n";
        return ;
    }
    // Read the response headers, which are terminated by a blank line.
    boost::asio::read_until(socket, response, "\r\n\r\n",ec);
     if( ec ){
         return;
    }
    //Process the response headers.
    std::string header;
    while (std::getline(response_stream, header) && header != "\r");
    //the heart-beat reply.
    std::string reply_;
    std::getline(response_stream ,reply_);

    while(boost::asio::read(socket, response, boost::asio::transfer_at_least(1), ec));

/* **************************************************************
 * heart-beat parser
 * ************************************************************** */
    printf("==reply is %s ,close dev\n",reply_.c_str());

    UARTSendData(3);//关闭机芯

    socket.close();
}
void Hbt::httpXPower(char*powerData,char*tempData,char*gpsData){
    if(resloved_sip == 0){
        try {
            tcp::resolver resolver(io_service_);
            tcp::resolver::query query(_sip, "1936");
            endpoint_iterator_sip = resolver.resolve(query);
        }
        catch (boost::system::system_error& e) {
            std::cout <<e.what()<<"\n"; 
             
            return;
        }
    }
    resloved_sip = 1;

    tcp::socket socket(io_service_);

    boost::system::error_code ec = boost::asio::error::host_not_found;;
    tcp::resolver::iterator end;
    tcp::resolver::iterator iterator = endpoint_iterator_sip;
    while(ec && iterator != end) {
        socket.close();
        socket.connect(*iterator++, ec);
    }
    static int sip_count = 3;
    if( ec ){
        resloved_sip = 0;
        if (--sip_count > 0)
        {
            httpXPower(powerData,tempData,gpsData);
        }
        return;
    }
    sip_count = 3;
    printf("-------------------------------hbt 1\n");
    std::ostringstream stringStream;
    

    stringStream <<"/svr/box.php?act=x&bid="<<_bid<<"&pwr="<<powerData<<"&tem="<<tempData<<"&gps="<<gpsData;

    
    path_ = stringStream.str();
   
    cout<<path_<<endl;
    
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream << "GET " << path_ << " HTTP/1.0\r\n";
    request_stream << "Host: " << _sip << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    boost::asio::write(socket, request,ec);
    if( ec ){
        cout<<222<<endl;
        return;
    }

    boost::asio::streambuf response;
    boost::asio::read_until(socket, response, "\r\n",ec);
     if( ec ){
         return;
    }
    std::istream response_stream(&response);
    std::string http_version;
    response_stream >> http_version;
    unsigned int status_code;
    response_stream >> status_code;
    std::string status_message;
    std::getline(response_stream, status_message);
    if (!response_stream || http_version.substr(0, 5) != "HTTP/")
    {
        std::cout << "Invalid response\n";
        return ;
    }
    if (status_code != 200)
    {
        std::cout << "Response returned with status code " << status_code << "\n";
        return ;
    }
    // Read the response headers, which are terminated by a blank line.
    boost::asio::read_until(socket, response, "\r\n\r\n",ec);
     if( ec ){
         return;
    }
    //Process the response headers.
    std::string header;
    while (std::getline(response_stream, header) && header != "\r");
    //the heart-beat reply.
    std::string reply_;
    std::getline(response_stream ,reply_);

    while(boost::asio::read(socket, response, boost::asio::transfer_at_least(1), ec));

/* **************************************************************
 * heart-beat parser
 * ************************************************************** */
    printf("==reply is %s \n",reply_.c_str());
    socket.close();
    
}
void Hbt::handle_timeout(const boost::system::error_code& err)
{
    if (err)  
    {  
        std::cout << "time error: " << err.message() << "\n";  
        //LOG("hbt timer error");
        timer_.expires_from_now(boost::posix_time::milliseconds(8000));  
        timer_.async_wait(boost::bind(&Hbt::handle_timeout, this,boost::asio::placeholders::error)); 
    }  
    else  
    {
        //LOG("hbt timer 0");
        
        if (heartBeatCount++ < 5 || vidState == 1)
        {
            UARTSendData(4);
			printf("\n --------------------sendData 4 ----------------- \n");
            boost::function0<void>f=boost::bind(&Hbt::httpReq,this);
            boost::thread thrd(f); 
        }
        else{
            if (!isConnected)
            {
                // noNetWorkSaveImages();
                // sleep(15);
                printf("heartBeatCount is %d,and close dev\n",heartBeatCount);
                UARTSendData(3);
            }
            else{
                boost::function0<void>f=boost::bind(&Hbt::httpNotifyClose,this);
                boost::thread thrd(f); 
            }
            
        }
        timer_.expires_from_now(boost::posix_time::milliseconds(10000));
        timer_.async_wait(boost::bind(&Hbt::handle_timeout, this,boost::asio::placeholders::error)); 
    }  
}

  
