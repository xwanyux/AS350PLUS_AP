#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "POSAPI.h"
#include "OS_PROCS.h"
#include "IPC_client.h"
UCHAR api_kbd_open( UCHAR deviceid, UCHAR *sbuf )
{
UCHAR buffer[6];
UCHAR ret=1;
  buffer[0]=deviceid;
  memmove(&buffer[1],sbuf,5);
  IPC_clientHandler(psDEV_KBD,1,2,6,buffer,&ret);
  return ret;

}

UCHAR api_kbd_close( UINT16 dhn )
{	
UCHAR ret=1;
  IPC_clientHandler(psDEV_KBD,2,1,1,(UCHAR*)&dhn,&ret);
  return ret;
}
UCHAR api_kbd_status( UINT16 dhn, UCHAR *dbuf )
{
UCHAR buffer[2];
UCHAR ret=1;
  buffer[0]=dhn;
  buffer[1]=*dbuf;
  IPC_clientHandler(psDEV_KBD,3,2,1,buffer,&ret);
  *dbuf=buffer[1];
  return ret;

}

UCHAR api_kbd_getchar( UINT16 dhn, UCHAR *dbuf )
{
UCHAR buffer[3];
UCHAR ret=1;
  buffer[0]=dhn;
  buffer[1]=*dbuf;
  IPC_clientHandler(psDEV_KBD,4,2,1,buffer,&ret);
  *dbuf=buffer[1];
  printf("getchar ret=%d *dbuf=0x%x\n",ret,*dbuf);
  
  return ret;  
}


