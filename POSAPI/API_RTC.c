//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : MAX32550 (32-bit Platform)				    **
//**  PRODUCT  : AS330_mPOS						    **
//**                                                                        **
//**  FILE     : API_RTC.C                                                  **
//**  MODULE   : api_rtc_open()				                    **
//**		 api_rtc_close()					    **
//**		 api_rtc_getdatetime()					    **
//**		 api_rtc_setdatetime()					    **
//**		 api_rtc_getUnixTime()					    **
//**									    **
//**  FUNCTION : API::RTC (Real Time Clock Module)			    **
//**  VERSION  : V1.00                                                      **
//**  DATE     : 2018/07/14                                                 **
//**  EDITOR   : James Hsieh                                                **
//**                                                                        **
//**  Copyright(C) 2018 SymLink Corporation. All rights reserved.           **
//**                                                                        **
//****************************************************************************
//============================================================================
//
//----------------------------------------------------------------------------
#include "POSAPI.h"
#include "OS_PROCS.h"
#include "IPC_client.h"
#include <stdio.h>   
#include <time.h>
#include <linux/rtc.h>   
#include <sys/ioctl.h>   
#include <sys/time.h>   
#include <sys/types.h>   
#include <fcntl.h>   
#include <unistd.h>   
#include <errno.h> 


// ---------------------------------------------------------------------------
// FUNCTION: To enable the service of real time clock device.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : DeviceHandleNumber
//           apiOutOfService
// ---------------------------------------------------------------------------
UCHAR api_rtc_open( void )
{
UCHAR retval;
UCHAR sbuf;
	IPC_clientHandler(psDEV_RTC,1,0,0,&sbuf,&retval);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To disable the service of real time clock device.
// INPUT   : dhn
//	     The specified device handle number.
//	     0x00 = to close all opened tasks.
// OUTPUT  : none.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR api_rtc_close( UCHAR dhn )
{
UCHAR retval;
UCHAR sbuf;
	IPC_clientHandler(psDEV_RTC,2,1,1,&dhn,&retval);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To read the current date and time from RTC device.
// INPUT   : dhn
//	     The specified device handle number. 
// OUTPUT  : dbuf
//	     UCHAR  year[2] ;		// "00", "01", "02",..."99"
//	     UCHAR  month[2] ;        	// "01", "02", "03",..."12"
//	     UCHAR  day[2] ;            // "01", "02", "03",..."31"
//	     UCHAR  hour[2] ;           // "00", "01", "02",..."23"
//	     UCHAR  minute[2] ;        	// "00", "01", "02",..."59"
//	     UCHAR  second[2] ;       	// "00", "01", "02",..."59"
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR api_rtc_getdatetime( UCHAR dhn, UCHAR *dbuf )
{
UCHAR retval;
UCHAR sbuf[1+12];
	sbuf[0]=dhn;	
	IPC_clientHandler(psDEV_RTC,3,2,1,sbuf,&retval);
	memmove(dbuf,&sbuf[1],12);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To assign the new date and time to RTC device.
// INPUT   : dhn
//	     The specified device handle number.
// 	     sbuf
//	     UCHAR  year[2] ;		// "00", "01", "02",..."99"
//	     UCHAR  month[2] ;        	// "01", "02", "03",..."12"
//	     UCHAR  day[2] ;            // "01", "02", "03",..."31"
//	     UCHAR  hour[2] ;           // "00", "01", "02",..."23"
//	     UCHAR  minute[2] ;        	// "00", "01", "02",..."59"
//	     UCHAR  second[2] ;       	// "00", "01", "02",..."59"
// OUTPUT  : none.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR api_rtc_setdatetime( UCHAR dhn, UCHAR *sbuf )
{
UCHAR retval;
UCHAR buf[1+12];
	buf[0]=dhn;
	memmove(&buf[1],sbuf,12);	
	IPC_clientHandler(psDEV_RTC,4,2,13,buf,&retval);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: Set if system time syncronize with NTP server automatically.
// INPUT   : dhn
//	     		The specified device handle number.
//			 action
//				1 for system enable syncronize with NTP server.This will change RTC time and UTC time automatically
//				0 for system disable syncronize with NTP server.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR api_rtc_sync_NTP_switch( UCHAR dhn, UCHAR action )
{
UCHAR retval;
UCHAR sbuf[2];
	sbuf[0]=dhn;
	sbuf[1]=action;
	IPC_clientHandler(psDEV_RTC,5,2,2,sbuf,&retval);
	return( retval );
}
