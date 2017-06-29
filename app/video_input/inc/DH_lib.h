#ifndef _H_LIB_STREAM_H_
#define _H_LIB_STREAM_H_

#if defined (__cplusplus)
extern "C" {
#endif

#define    STREAM_VIDEO_TYPE                  0          //视频流

#define    STREAM_AUDIO_RATE_8000             0          //8K采样率
#define    STREAM_AUDIO_RATE_16000            1          //16K采样率
#define    STREAM_AUDIO_RATE_48000            2          //48K采样率

#define    XM_SUCCESS                         0          //成功
#define    XM_PRINT_ERR                       -100       //打印开关设置失败
#define    XM_NOINIT                          -101       //未初始化库
#define    XM_INIT_BEINIT                     -102       //重复初始化
#define    XM_INIT_ERR                        -103       //初始化错误
#define    XM_DINIT_ERR                       -104       //去初始化失败
#define    XM_CONNECT_AV_PARAM_ERR            -105       //连接音视频源参数输入错误
#define    XM_CONNECT_AV_ERR                  -106       //连接音视频源失败
#define    XM_AV_NOT_CONNECT                  -107       //未连接AV服务器                
#define    XM_REGIST_PARAM_ERR                -108       //注册回调参数错误
#define    XM_INSERT_IFREAME_ERR              -109       //强插I帧失败
#define    XM_VIDEO_START_PARAM_ERR           -110       //开启视频参数错误
#define    XM_VIDEO_START_NO_REGIST           -111       //视频回调未注册
#define    XM_AUDIO_OPEN_ERR                  -114       //音频打开失败

#define    XM_PRINT_CLOSE                     0          //打印开关 关闭
#define    XM_PRINT_OPEN                      1          //打印开关 开启

#define    XM_LIB_HEALTH_OK                   0          //库正常工作
#define    XM_LIB_HEALTH_FAILD                1          //库非正常工作

typedef int (*videocallback)(char *buff, int size, int fps, int frametype, int width, int height, 
                             int streamtype, struct timeval ntimestamp, int fec_app, int fec_mode,
                             long reserv);

typedef int (*audiocallback)(char *buff, int size, struct timeval ntimestamp, long reserv);

/******************************************************** 
	funcname:     XMHAL_getlibversion                     
	Description:  get lib version, Anywhere can be called
	intput:                                               
	output:                                               
********************************************************/
char *XMHAL_getlibversion();

/******************************************************** 
	funcname:     XMHAL_Print                             
	Description:  open lib print, Anywhere can be called  
	intput:                                               
	output:                                               
********************************************************/
int XMHAL_Print(int flag);

/******************************************************** 
	funcname:     XMHAL_Init                              
	Description:  init xm_av lib                          
	intput:                                               
	output:                                               
********************************************************/
int XMHAL_Init();

/******************************************************** 
	funcname:     XMHAL_Deinit                            
	Description:  deinit xm_av lib                        
	intput:                                               
	output:                                               
********************************************************/
int XMHAL_Deinit();

/******************************************************** 
	funcname:     XMHAL_ConnectAVSource                   
	Description:  Connect AV Source server                
	intput:       ip                                      
				  trytimes                                
	output:                                               
********************************************************/
int XMHAL_ConnectAVSource(char *ip, int trytimes);

/******************************************************** 
	funcname:     XMHAL_RegistsVideo                      
	Description:  regist video callbackfunc(network)      
	intput:       callbackfunc                            
	              streamtype                              
	              reserv                                  
	output:                                               
********************************************************/
int XMHAL_RegistsVideo(videocallback callback, int stream_type, long reserv);

/******************************************************** 
	funcname:     XMHAL_RegistsVideo                      
	Description:  regist video callbackfunc(local)        
	intput:       callbackfunc                            
	              streamtype                              
	              reserv                                  
	output:                                               
********************************************************/
int XMHAL_RegistsVideo1(videocallback callback, int stream_type, long reserv);

/******************************************************** 
	funcname:     XMHAL_RegistAudio                       
	Description:  regist audio callbackfunc               
	intput:       callbackfunc                            
	              reserv                                  
	output:                                               
********************************************************/
int XMHAL_RegistAudio(audiocallback callback, long reserv);

/******************************************************** 
	funcname:     XMHAL_InsertIFrame                      
	Description:  get a I frame                           
	intput:       stream_type                             
	output:                                               
********************************************************/
int XMHAL_InsertIFrame(int stream_type);

/******************************************************** 
	funcname:     XMHAL_OpenAudio                         
	Description:  open audio encode                       
	intput:       audio_rate                              
	output:                                               
********************************************************/
int XMHAL_OpenAudio(int audio_rate);

/******************************************************** 
	funcname:     XMHAL_CloseAudio                        
	Description:  close audio encode                      
	intput:                                               
	output:                                               
********************************************************/
int XMHAL_CloseAudio();

/******************************************************** 
	funcname:     XMHAL_OpenAudio                         
	Description:  open audio decode                       
	intput:       int audio_rate                          
	output:                                               
********************************************************/
int XMHAL_OpenTalk(int audio_rate);

/******************************************************** 
	funcname:     XMHAL_CloseAudio                        
	Description:  close audio decode                      
	intput:                                               
	output:                                               
********************************************************/
int XMHAL_CloseTalk();

/******************************************************** 
	funcname:     XMHAL_VideoStart                        
	Description:  start video stream(local                
	intput:       stream_type                             
	output:                                               
********************************************************/
int XMHAL_VideoStart(int stream_type);

#if defined (__cplusplus)
}
#endif

#endif
