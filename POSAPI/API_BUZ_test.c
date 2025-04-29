#include<stdint.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<linux/input.h>
#include<stdlib.h>
#include <string.h>
#include "POSAPI.h"
#include "IPC_client.h"
#include "OS_PROCS.h"
UCHAR api_buz_open( UCHAR *sbuf )
{
UCHAR buffer[6];
UCHAR ret=1;
//printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
  memmove(buffer,sbuf,6);
//printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
  IPC_clientHandler(psDEV_BUZ,1,1,6,buffer,&ret);
//printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
  return ret;
}
UCHAR api_buz_close(UCHAR dhn)
{
UCHAR ret=1;
  IPC_clientHandler(psDEV_BUZ,2,1,1,(UCHAR*)&dhn,&ret);
    	return ret;
}
UCHAR api_buz_sound( UCHAR dhn )
{
UCHAR ret=1;
  IPC_clientHandler(psDEV_BUZ,3,1,1,(UCHAR*)&dhn,&ret);
  return ret;
}

