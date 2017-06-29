#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <linux/input.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <getopt.h>
#include <sys/time.h>
#include <pthread.h>
#include <sys/epoll.h>//使用epoll

#include <termios.h>
#include <boost/asio.hpp>
#include "SeriaData.h"
#include "bs.h"
boost::asio::io_service io_service_;


int main(int argc, char **argv)
{
    device.uart_index = 1;
    Hal_UART_init(&device);
    
    if (argc < 2)
     {
         printf("usage:com.mod 4\n");
     } 

    if (argc >= 2) {
        char * cmd = argv[1];
        while(1){
            UARTSendData(atoi(cmd));
            sleep(10);
        }
        
    }
    else{
        // io_service_.reset();
        // bs * b = new bs(io_service_,"47.89.51.239","1940");
        // io_service_.run(); 
    }
    
    return 0;
}


