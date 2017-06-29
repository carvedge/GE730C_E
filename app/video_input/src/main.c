#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#include "DH_lib.h"
#include "RtmpSender.h"
int g_running = 1;
int hasAVCInit = 0;
uint8_t *sps = NULL, *pps = NULL;
int spsSize, ppsSize;
double fps = 25;
int frame =0;
void SendH264(uint8_t* data, int size) {
    if (!(data[0] || data[1] || data[2]) && data[3]) {
        switch (data[4] & 0x1f) {
            case 7: // SPS
            	printf("Get a SPS NALU\n");
                spsSize = size - 4;
                sps = (uint8_t*)malloc(sizeof(uint8_t) * spsSize);
                memcpy(sps, data + 4, spsSize);
                if (sps && pps && hasAVCInit == 0) {
                	printf("hasAVCInited\n");
				    RtmpSendHeader(sps, spsSize, pps, ppsSize); 
                    sps = pps = NULL;
                    hasAVCInit = 1;
                }
                break;
            case 8: // PPS
            	printf("Get a PPS NALU\n");
                ppsSize = size - 4;
                pps = (uint8_t*)malloc(sizeof(uint8_t) * ppsSize);
                memcpy(pps, data + 4, ppsSize);
                if (sps && pps && hasAVCInit == 0) {
                	printf("hasAVCInited,pps sps size is %d,pps size is %d\n",spsSize,ppsSize);
				    RtmpSendHeader(sps, spsSize, pps, ppsSize); 
                    sps = pps = NULL;
                    hasAVCInit = 1;
                }
                break;
                
            case 5:
               //idr
            case 1:
            	if (hasAVCInit)
            	{
            		printf("send frame,frame is %d\n",frame++);
            		RtmpSendH264Frame(data+4, size-4, frame * 1000 / fps);
            	}
                break;
                
            default:
                break;
        }
    }
}



void sig_handler(int arg)
{
	printf("Receive signal(%d) to terminate\n", arg);
	g_running = 0;
}
int video_callback_net(char *buff, int size, int fps, int frametype, int width, int height, 
                   	   int streamtype, struct timeval ntimestamp, int fec_app, int fec_mode,
                       long reserv)
{

	printf("[VIDEO_NET][ %x %x %x %x %x ] %d x %d [%d] [%d] [ %d | %d ] \n", buff[0], buff[1], buff[2], buff[3], 
		                                                          		 	 buff[4], width, height, fps, size, 
		                                                          		 	 fec_app, fec_mode);
	if (frametype == 1)
	{
		int last = -1;
		int i = 3;
		for (; i < size; ++i) {
            if (!(buff[i - 3] || buff[i - 2] || buff[i - 1]) && buff[i] == 0x01) {
                if (last >= 0) {
                    SendH264(buff + last, i - last - 3);
                }
                last = i - 3;
            }
        }
        if (last >= 0) {
            SendH264(buff + last, size - last);
        }
	}
	else if(frametype == 3){
		SendH264(buff, size);
	}
	
	return 0;
}

int video_callback_local(char *buff, int size, int fps, int frametype, int width, int height, 
                   int streamtype, struct timeval ntimestamp, int fec_app, int fec_mode,
                   long reserv)
{
	printf("[VIDEO_LOCAL][ %x %x %x %x %x ] %d x %d [%d] [%d] [ %d | %d ] \n", buff[0], buff[1], buff[2], buff[3], 
		                                                          		 	   buff[4], width, height, fps, size, 
		                                                          		       fec_app, fec_mode);
	if (frametype == 1)
	{
		int last = -1;
		int i = 3;
		for (; i < size; ++i) {
            if (!(buff[i - 3] || buff[i - 2] || buff[i - 1]) && buff[i] == 0x01) {
                if (last >= 0) {
                    SendH264(buff + last, i - last - 3);
                }
                last = i - 3;
            }
        }
        if (last >= 0) {
            SendH264(buff + last, size - last);
        }
	}
	else if(frametype == 3){
		SendH264(buff, size);
	}
	
	return 0;
}
// int video_main_callback(uint8_t *buff, int size, int fps, int frametype, int width, int height, 
//                         int streamtype, struct timeval ntimestamp, long reserv)
// {
// 	printf("-- 1 receive buf len is %d,streamtype is %d,frametype is %d\n", size,streamtype,frametype);
// 	if (frametype == 1)
// 	{
// 		int last = -1;
// 		int i = 3;
// 		for (; i < size; ++i) {
//             if (!(buff[i - 3] || buff[i - 2] || buff[i - 1]) && buff[i] == 0x01) {
//                 if (last >= 0) {
//                     SendH264(buff + last, i - last - 3);
//                 }
//                 last = i - 3;
//             }
//         }
//         if (last >= 0) {
//             SendH264(buff + last, size - last);
//         }
// 	}
// 	else if(frametype == 3){
// 		SendH264(buff, size);
// 	}
// 	return 0;
// }

int h264_start(char * addr,char *url)
{
	int ret;
	int audio_flag = 0;  //
	int talk_flag = 0;   //
	int audio_rate = 0;  //
	

	ret = XMHAL_Init(audio_flag, talk_flag, audio_rate);
	if(ret != XM_SUCCESS)
	{
		goto end;
	}

	// ret = XMHAL_ConnectAVSource(addr, 3);
	// if(ret != XM_SUCCESS)
	// {
	// 	goto end;
	// }

	// ret = XMHAL_RegistsVideo(video_callback_net, STREAM_VIDEO_TYPE, 0);
	// if(ret != XM_SUCCESS)
	// {
	// 	printf("regist err\n");
	// 	goto end;
	// }

	//init rtmp
	RtmpInit(url,5);
	ret = XMHAL_RegistsVideo1(video_callback_local, STREAM_VIDEO_TYPE, 0);
	if(ret != XM_SUCCESS)
	{
		printf("regist video callback(local) err\n");
		XMHAL_Deinit();
		return -1;
	}
	ret = XMHAL_VideoStart(STREAM_VIDEO_TYPE);
	if(ret != XM_SUCCESS)
	{
		printf("video start err\n");
		goto end;
	}

	ret = XMHAL_InsertIFrame(STREAM_VIDEO_TYPE);
	if(ret != XM_SUCCESS)
	{
		printf("inset i frame err\n");
	}


	while(g_running)
	{
		sleep(1);
	}

	printf("h264_start stop\n");
	
end:
	
	
	//XMHAL_VideoStop(STREAM_VIDEO_TYPE);
	XMHAL_Deinit();
	return 0;
}

int main(int argc, char* argv[])
{	
	signal(SIGINT, sig_handler); 
	signal(SIGQUIT, sig_handler);
	signal(SIGTERM, sig_handler);

	if(argc < 3)
	{
		printf("please input ip rtmpurl\n");
		return -1;
	}
	h264_start(argv[1],argv[2]);

	return 0;
}
