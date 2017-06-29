#include <stdio.h>
#include <string.h>
#include <time.h>
unsigned short RTU_CRC( unsigned char * puchMsg, unsigned short usDataLen );

int  ucha2int(unsigned char *buff);
void int2uchar(int value,unsigned char *ch);

int writeToFile(char *str,int length);