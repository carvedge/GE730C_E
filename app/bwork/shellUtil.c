//
//  memoryUtil.c
//  onvifclient
//
//  Created by 彭闽 on 15/7/1.
//  Copyright (c) 2015年 Carvedge. All rights reserved.
//

#include "shellUtil.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/vfs.h>

void _mkdir(char* cmd){
	struct stat st = {0};
	if (stat(cmd, &st) == -1) {
    	mkdir(cmd, 0777);
    }
}
bool _isMounted(){
    struct statfs sfs;
    int i = statfs("/tmp/DataDisk/app/image", &sfs);
    // int percent = (sfs.f_blocks - sfs.f_bfree ) * 100 / (sfs.f_blocks - sfs.f_bfree + sfs.f_bavail) + 1;
    // printf("sfs bolcks %d ,percent is %d\n", sfs.f_blocks,percent);
    unsigned long long blocksize = sfs.f_bsize; 
    unsigned long long totalsize = blocksize * sfs.f_blocks; 

    printf("Total_size = %llu B = %llu KB = %llu MB = %llu GB\n",totalsize, totalsize>>10, totalsize>>20, totalsize>>30); 


    //if(sfs.f_blocks >  7000000/4 &&  sfs.f_blocks<8000000/4)
    if(totalsize >  1024*1024*1024 )
    {
    	printf("-------------------mounted\n");
    	return true;
    }
    else {
    	printf("-------------------not mounted\n");
    	return false;
    }
		
}
struct statfs _getMountState(){
    struct statfs sfs;
    int i = statfs("/tmp/DataDisk/app/image", &sfs);
    int percent = (sfs.f_blocks - sfs.f_bfree ) * 100 / (sfs.f_blocks - sfs.f_bfree + sfs.f_bavail) + 1;
    printf("sfs bolcks %d ,percent is %d\n", sfs.f_blocks,percent);
    return sfs;   
}