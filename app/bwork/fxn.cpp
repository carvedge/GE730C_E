#include "fxn.h"
#include "hbt.h"
string _mip;
string _bid;
string _sip;
int _vsn;
int vidState;//0:关闭 1:开启
bs * _client = NULL;
Hbt* _hbt    = NULL;
 


int writeToFile(char *str,int length,int type){
     FILE *fp = NULL; 
     if (1 == type)
     {
        fp = fopen("/tmp/DataDisk/app/ram/1.txt","w");
     }
     else{
        fp = fopen("/tmp/DataDisk/app/ram/2.txt","w");
     }
     if(fp != NULL)
     {
        fwrite(str, sizeof(char), length, fp);
        fclose(fp);
        return 0;
    } 
    return -1;

}

int writeToFile(char *str,int length,char* fileName){
     FILE *fp = NULL; 
     fp = fopen(fileName,"w");
     
     if(fp != NULL)
     {
        char* buff = new char[200];

        fread(buff, sizeof(char), sizeof(buff), fp);
        if (strcmp(str,buff) != 0 )
        {
            fwrite(str, sizeof(char), length, fp);
        }
        free(buff);
        fclose(fp);
        return 0;
    } 
    return -1;

}
int  getZoomValue(){
    FILE *fp = NULL; 
    fp = fopen("/tmp/DataDisk/app/zoom.txt","r");
    if(fp != NULL)
    {
        char* buff = new char[50];
        fread(buff, sizeof(char), sizeof(buff), fp);
        printf("read buff is %s\n",buff);
        fclose(fp);
        int value = atoi(buff);
        free(buff);
        return value;
    } 
    else{
        return 1;
    }
}
int  setZoomVaule(int value){
    if (value < 1)
    {
        value = 1;
    }
    if (value > 20)
    {
        value = 20;
    }
    FILE *fp = NULL; 
    printf("set zoom value , value is %d\n",value);
    fp = fopen("/tmp/DataDisk/app/zoom.txt","w");
    if(fp != NULL)
    {
        char* buff = new char[50];
        sprintf(buff,"%d",value);
        fwrite(buff, sizeof(char), strlen(buff), fp);
        free(buff);
        fclose(fp);
        return 0;
    } 
    return -1;
}


