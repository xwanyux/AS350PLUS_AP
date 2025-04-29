//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : ZA9L                                                       **
//**  PRODUCT  : AS320-A                                                    **
//**                                                                        **
//**  FILE     : API_TMR.C                                                  **
//**  MODULE   : api_tim_open()				                    **
//**		 api_tim_close()					    **
//**		 api_tim_gettick()					    **
//**		 api_tim_clock()					    **
//**									    **
//**  FUNCTION : API::TIM (Timer Module)				    ** 
//**  VERSION  : V1.00                                                      **
//**  DATE     : 2007/08/03                                                 **
//**  EDITOR   : James Hsieh                                                **
//**                                                                        **
//**  Copyright(C) 2007 SymLink Corporation. All rights reserved.           **
//**                                                                        **
//****************************************************************************
//============================================================================
//
//----------------------------------------------------------------------------
#include "POSAPI.h"
#include "OS_PROCS.h"
#include <sys/timerfd.h>
#include <time.h>
#include <unistd.h>
#include <sys/unistd.h>
#include <sys/fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>        /* Definition of uint64_t */

#if	0
struct itimerspec new_value;
struct timespec now;
struct timeval timeout;
UINT 	TMR_DHN[TimerNumbers];
UCHAR 	TMR_FirstTick[TimerNumbers];
#else
#include "IPC_client.h"
#endif

// ---------------------------------------------------------------------------
// FUNCTION: To enable the timer device.
// INPUT   : unit
//           Time tick resolution unit, incremented in step of this value.
//	     One unit is 10ms, i.e., time tick is advanced by 10ms if unit=1.
// OUTPUT  : none.
// RETURN  : DeviceHandleNumber
//           apiOutOfService
// ---------------------------------------------------------------------------
UCHAR	api_tim_open( UCHAR unit )
{
#if	0
int fd;
	// printf("unit=%d\n",unit);
	long SEC = unit / 100;
	long NSEC = (unit % 100) * 10000000;
	
	if(NSEC>=1000000000)
	{
		SEC=NSEC/1000000000;
		NSEC=NSEC%1000000000;
	}

	fd = timerfd_create(CLOCK_MONOTONIC, O_NONBLOCK);					//create timer object
	if (fd == -1) {
		printf("timerfd create fail\n");
        return( apiOutOfService );
    }
	if (clock_gettime(CLOCK_MONOTONIC, &now) == -1) {
		printf("clock_gettime fail\n");
        return( apiOutOfService );
    }
	// avoid overflow
	if(now.tv_nsec + NSEC > 1000000000){
		new_value.it_value.tv_sec = now.tv_sec + SEC + 1;
		new_value.it_value.tv_nsec = now.tv_nsec + NSEC - 1000000000;
	}
	else{
		new_value.it_value.tv_sec = now.tv_sec+SEC;
		new_value.it_value.tv_nsec = now.tv_nsec + NSEC;
	}
	// new_value.it_value.tv_sec = now.tv_sec+SEC;					//timer start time(sec)
    // new_value.it_value.tv_nsec = now.tv_nsec;	//timer start time(nano sec).start counting after 1 unit
	new_value.it_interval.tv_sec = SEC;						//timer execute interval time(sec)
	new_value.it_interval.tv_nsec = NSEC;			//timer execute interval time. 10M nano sec = 10 millisec
	// printf("SEC=%d\n",SEC);
	// printf("NSEC=%d\n",NSEC);
    if (timerfd_settime(fd, TFD_TIMER_ABSTIME, &new_value, NULL) == -1) { //set timer attribute
        return( apiOutOfService );
    }
	for(UINT i=0;i<TimerNumbers;i++){
		if(TMR_DHN[i]==0)
		{
			TMR_DHN[i]=fd;
			TMR_FirstTick[i]=0;
			return( fd );//dhn
		}
	}
	return( apiOutOfService );
#else
	
UCHAR	ret=1;

	// UINT8 PID, UINT8 APInum, UINT8 ArgsNum, UINT32 IptSize, UINT8 *Arg, UINT8 *retval
	IPC_clientHandler(psDEV_TIM,1,1,1,&unit,&ret);
  
	return ret;
    
#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: To disable the system timer service.
// INPUT   : dhn
//           The specified device handle number.
//	     0x00 = to close all opened tasks.
// OUTPUT  : none.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_tim_close( UCHAR dhn )
{
#if	0
	UINT i=0;
	/*
	 *check if timer opened before
	 */
	if(dhn==0)
	{
		printf("\033[1;31;40mapi_tim dhn=0\033[0m\n");
		return( apiFailed );
	}
	for(i=0;i<TimerNumbers;i++){
		if(TMR_DHN[i]==dhn)
		{
			break;
		}
		else if(i==(TimerNumbers-1))
			return( apiFailed );			
	}
	
	new_value.it_interval.tv_sec = 0;		//timer execute interval time(sec)
	new_value.it_interval.tv_nsec = 0;		//timer execute interval=0 ==>timer not enable	
	if (timerfd_settime(dhn, TFD_TIMER_ABSTIME, &new_value, NULL) == -1) { //set timer attribute
        return( apiFailed );
    }
	if(close(dhn)==-1)
	  return( apiFailed );
  
	TMR_DHN[i]=0;
	TMR_FirstTick[i]=0;
	//if(!timer_delete(&dhn))//timer_delete return 0 = delete complete
	  return( apiOK );
	/*else
		return( apiFailed );*/ 
#else

UCHAR	ret=1;

	// UINT8 PID, UINT8 APInum, UINT8 ArgsNum, UINT32 IptSize, UINT8 *Arg, UINT8 *retval
	IPC_clientHandler(psDEV_TIM,2,1,1,&dhn,&ret);
  
	return ret;
	
#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: To get the contents of tick counter from the specified timer.
// INPUT   : dhn
//           The specified device handle number.
// OUTPUT  : dbuf
//	     UINT  counter  // current 16-bit up-counter of the specified timer.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_tim_gettick( UCHAR dhn, UCHAR *dbuf )
{
#if	0
UINT	i=0,getTime=0;
	if(dhn==0)
	{
		printf("\033[1;31;40mapi_tim dhn=0\033[0m\n");
		return( apiFailed );
	}
	for(;i<TimerNumbers;i++){
		if(TMR_DHN[i]==dhn)
		{
			break;
		}
		else if(i==(TimerNumbers-1))
			return( apiFailed );			
	}
	if(TMR_FirstTick[i]==0)
	{
		*dbuf++=0;
		*dbuf=0;
	}
	getTime=(*dbuf)+ *(dbuf+1)*256;	
	TMR_FirstTick[i]=1;
uint64_t count;	
	if(read(dhn, &count, sizeof(uint64_t))>0){
		if(count>1){
			getTime+=count;
		}
		getTime++;
		*dbuf=getTime;
		*(dbuf+1)=getTime>>8;
		return( apiOK );
	}
	return( apiFailed );
#else

UCHAR retval=1;
UCHAR sendbuff[3];


	memset( sendbuff, 0x00, sizeof(sendbuff) );
	sendbuff[0]=dhn;
	
	// UINT8 PID, UINT8 APInum, UINT8 ArgsNum, UINT32 IptSize, UINT8 *Arg, UINT8 *retval
	IPC_clientHandler(psDEV_TIM,3,1,1,sendbuff,&retval);
	if(retval==apiOK)
	{
		memmove(dbuf,sendbuff+1,2);
	}
	
	return( retval );

#endif
}


