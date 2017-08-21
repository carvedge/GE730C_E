#include "bs.h"
#include "fun.h"
#include "fxn.h"
bs::bs(boost::asio::io_service& io_service):
io_service_( io_service),
socket_(io_service),
timer_(io_service),
hbt_timer_(io_service),
recon_timer_( io_service )
{
    wrt_end = true;
    _inited = false;
}
void bs::init(string ip,string port,string bid){
    _inited = true;
    sprintf((char*)Bid,bid.c_str());
    //memset(Bid,0x31,17);
    ip_= ip;
    port_ = port;
    tcp::resolver resolver(io_service_); 
    tcp::resolver::query query(ip_, port_);
    endpoint_iterator = resolver.resolve(query);  
    boost::optional<boost::system::error_code> result;  
    socket_.async_connect(*endpoint_iterator,boost::bind(&bs::handle_connect,this, _1,&result));
    timer_.expires_from_now(boost::posix_time::seconds(5));  
    timer_.async_wait(boost::bind(&bs::handle_timeout, this,boost::asio::placeholders::error)); 
}
//  timer Mechanism 
void bs::handle_connect(boost::system::error_code ec,boost::optional<boost::system::error_code>* out_ec)
{

    if (ec)  
    {  
        std::cout << "Connect error: " << ec.message() << "\n";
        //timer_.cancel();
		
    }  
    else  
    {  
            /*if connect success ,start( actually wait to read)*/
        assert ( socket_.is_open() );
    	  lfe_=5;
        socket_.non_blocking(true);
        timer_.cancel();
        this->snd_hbt();
        hbt_timer_.expires_from_now(boost::posix_time::seconds(20));  
        hbt_timer_.async_wait(boost::bind(&bs::handle_hbt_timeout, this,boost::asio::placeholders::error)); 
        start();  
            
    }  
      
}

   
void bs::handle_timeout(const boost::system::error_code& err) {
   
    if (err)  
    {  
        std::cout << "time error: " << err.message() << "\n";
    }  
    else  
    {  
         
   		if(socket_.is_open())
        socket_.close();
        // boost::system::error_code    ec;
    	//this->socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both,ec);
         
        boost::optional<boost::system::error_code> result;  
        socket_.async_connect(*endpoint_iterator,boost::bind(&bs::handle_connect,this, _1,&result));
        timer_.expires_from_now(boost::posix_time::seconds(5));  
        timer_.async_wait(boost::bind(&bs::handle_timeout, this,boost::asio::placeholders::error)); 
    }  
}

void bs::handle_hbt_timeout(const boost::system::error_code& err) {
  
    if (err)  
    {  
      
    }  
    else  
    {  
        std::cout << "hbt time out ,lfe :"<<lfe_<<'\n';
        if(--lfe_<0)
        {
        	if(socket_.is_open()){
        		socket_.close();
        		return;
        	}
        }
        this->snd_hbt();
        hbt_timer_.expires_from_now(boost::posix_time::seconds(20));  
        hbt_timer_.async_wait(boost::bind(&bs::handle_hbt_timeout, this,boost::asio::placeholders::error)); 
    
    }  
}
void bs::handle_recon_timeout(const boost::system::error_code& err) {
  
    if (err)  
    {  
     
    }  
    else  
    {  
        std::cout << "recon time out"<<'\n';
        if(socket_.is_open())
        socket_.close();  
       //  boost::system::error_code    ec;
    	//this->socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both,ec);
        boost::optional<boost::system::error_code> result;  
        socket_.async_connect(*endpoint_iterator,boost::bind(&bs::handle_connect,this, _1,&result));
        timer_.expires_from_now(boost::posix_time::seconds(5));  
        timer_.async_wait(boost::bind(&bs::handle_timeout, this,boost::asio::placeholders::error)); 
       
    }  
}

void bs::start()
{
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
                            boost::bind(&bs::handle_read, this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
    
}
void bs::snd_hbt()
{
	assert ( socket_.is_open() );
    if( !wrt_end ) return;
    sendData(0x11);
	
}

void bs::handle_write(const boost::system::error_code& error)
{
    wrt_end = true;
    
    if (!error)
    {
        std::cout<<"socket write success"<<"\n";
     
            // hbt_timer start
    }
    else
    {
       
        std::cout << "socket write error: " << error.message() << "\n";  
    }
}
    

void bs::handle_read(const boost::system::error_code& error,size_t bytes_transferred)
{
    if(!error)
    {
       lfe_=5;
       parse(data_,bytes_transferred);
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
                                boost::bind(&bs::handle_read, this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));
    }
    else
    {
        
        std::cout << "socket read error: " << error.message() << "\n";
        wrt_end = true;
        hbt_timer_.cancel();
        recon_timer_.expires_from_now(boost::posix_time::seconds(10));  
        recon_timer_.async_wait(boost::bind(&bs::handle_recon_timeout, this,boost::asio::placeholders::error)); 
       
    }
}
void bs::parse(unsigned char*data,size_t length){
    printf("parse data ,receive length is %d\n", length);
    if (0xA5 == data[0] && 0x5A == data[1])
    {
          int length = ucha2int(data+2);
          printf("leng ori   is %02x%02x\n",data[2],data[3]);
          printf("parse length is %d\n",length);
          unsigned short crc = RTU_CRC( data, length-2 );
          int receiveCrc =  ucha2int(data+length-2);
          printf("calculate crc  is %02x%02x\n",crc&0xFF,(crc>>8)& 0xFF);
          printf("receiveCrc  is %02x%02x\n",data[length-2],data[length-1] );
          parseDetail(data+4,length-6);
          
    }

}
/**
 *解析具体数据
 */
void bs::parseDetail(unsigned char*data,size_t length){
   
   unsigned char Frame_Type[1];  
   //memcpy(Bid, data, 17); 
   //printf("bid is %s\n",Bid );
   memcpy(Frame_Type, data+17, 1);
   printf("Frame_Type is %02x\n",Frame_Type[0]);
   if (Frame_Type[0] == 0x12)
   {
    //机芯发送给服务器的心跳之后服务器的回复
     unsigned char Current_Time[6]; 
     memcpy(Current_Time, data+18, 6);
    
   }
   else if (Frame_Type[0] == 0x13){
    //服务器发送给机芯的视频请求命令：
     unsigned char Video_Status[1]; 
     memcpy(Video_Status, data+18, 1);
     if (Video_Status[0] == 0xFF)
     {
        vidState = 1;
        std::stringstream scmd;
        scmd << "/tmp/DataDisk/app/1.mod";
        scmd<<" ' '  'rtmp://"<<_sip<<":1935/live/"<<_bid<<"' &";
        printf("cmd is :%s\n", scmd.str().c_str());
        writeToFile((char*)scmd.str().c_str(),strlen(scmd.str().c_str()),1);
     }
     else if(Video_Status[0] == 0x00){
        char * cmd = new char[100];
        sprintf(cmd,"killall 1.mod");
        writeToFile(cmd,strlen(cmd),1);
        free(cmd);
     }
     sendData(0x14);
   }
   else if(Frame_Type[0] == 0x17){
	   if((capture_work == 0) && (zoom_is_work == 0) && (y_zoom_is_work == 0))
	   {
		  unsigned char Control_Type[1]; 
		  memcpy(Control_Type, data+18, 1);
		  if (Control_Type[0] == 0x05)
		  {
			  //放大
			
			int zoom = getZoomValue();
			printf("zoom is %d\n",zoom);
			char * cmd = new char[100];


			if (++zoom > 20)
			{
			  zoom = 20;
			}

			sprintf(cmd,"AutoFocus -Z %d",zoom);

			writeToFile(cmd,strlen(cmd),2);
			free(cmd);
			printf("zoom+,zoom is %d\n",zoom);
			setZoomVaule(zoom);
		  }
		  else if(Control_Type[0] == 0x06){
			 //缩小
			int zoom = getZoomValue();
			char * cmd = new char[100];
			--zoom;
			if (zoom < 1)
			{
			  zoom = 1;
			}
			sprintf(cmd,"AutoFocus -Z %d",zoom);
			writeToFile(cmd,strlen(cmd),2);
			free(cmd);
			printf("zoom-,zoom is %d\n",zoom);
			setZoomVaule(zoom);
		  }
		}
   }

   else if(Frame_Type[0] == 0x19){
      //设预置位
      unsigned char Control_Type[1]; 
      memcpy(Control_Type, data+18, 1);
      int pre = Control_Type[0];
      char* key = (char*)malloc(10);
      sprintf(key,"cfg.n%02d",pre);
      printf("set pre key is %s\n",key);
      setPresets(key);
      free(key);
      
   }
   else if(Frame_Type[0] == 0x21){
	   if((capture_work == 0) && (zoom_is_work == 0) && (y_zoom_is_work == 0))
	   {
		  //置预置位
		  unsigned char Control_Type[1]; 
		  memcpy(Control_Type, data+18, 1);
		  int pre = Control_Type[0]+1;
		  printf("set preset is %d\n",pre);
		  //char* key = (char*)malloc(10);
		 // sprintf(key,"cfg.n%02d",pre);
		  toPresets(pre);
		 // free(key);
	   }
   }
   else if(Frame_Type[0] == 0x23){
      printf("capture\n");
      capture_work = 1;//正在进行截图
      char * cmd = new char[200];
      int snapshot_exist = 1;           
      sprintf(cmd,"CmdSnapShot 1920 1080 80");
      writeToFile(cmd,strlen(cmd),2);
      sleep(2);               
      memset(cmd,0,200);            
      sprintf(cmd, "/tmp/DataDisk/app/curl -F 'dat=@/tmp/snapshot.jpg' 'http://%s:1936/svr/box.php?act=c&bid=%s'", _sip.c_str(),_bid.c_str());
      printf("cmd is %s\n",cmd);
      writeToFile(cmd,strlen(cmd),2);
      free(cmd);
      sleep(5);
      snapshot_exist = access("/tmp/snapshot.jpg",F_OK);
      printf("access is %d\n", snapshot_exist);
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
      capture_work = 0;//截图结束
   }

}
void bs::sendData(unsigned char Frame_Type){
    unsigned char ret_dat[1500];
    unsigned char Head[2];
    unsigned char packet_length[2];
    ret_dat[0] = 0xA5;
    ret_dat[1] = 0x5A;
    if(Frame_Type == 0x11 || Frame_Type == 0x14){
        int length = 2+2+17+1+2;
        ret_dat[2] = length& 0xFF;
        ret_dat[3] = (length>>8)& 0xFF;
        memcpy(ret_dat+4, Bid, 17);
        ret_dat[21] = Frame_Type;
        unsigned short crc = RTU_CRC( ret_dat, length-2 );     
        ret_dat[22] = crc & 0xFF;
        ret_dat[23] = crc>>8 & 0xFF;
        socket_.async_write_some(boost::asio::buffer(ret_dat, length),
                                 boost::bind(&bs::handle_write, this,
                                             boost::asio::placeholders::error()));

    }
}
void bs::setPresets(string key){
  //思路 p为预置点 只需要把当前的放大倍数给到这个预置点就可以
    // int zoom = getZoomValue();
    // stringstream szoom;  
    // szoom<<zoom; 
    // Ini* ini = Ini::Instance(); //alias
    // ini->save(key,szoom.str());
}
void bs::toPresets(int pre){
  //把key对应的放大倍数取出来，然后控制云台，然后再写到zoom.txt文件里
    // Ini* ini = Ini::Instance(); //alias
    // int zoom = atoi(ini->get(key).c_str());
    char * cmd = new char[100];
    sprintf(cmd,"AutoFocus -Z %d",pre);
    writeToFile(cmd,strlen(cmd),2);
    free(cmd);
    setZoomVaule(pre);
}


