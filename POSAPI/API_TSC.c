//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : IMX6UL							    **
//**  PRODUCT  : AS350	                                                    **
//**                                                                        **
//**  FILE     : API_LCDTFT.C 	                                            **
//**  MODULE   : api_tsc_open()					    	    **
//**		 api_tsc_close()					    **
//**		 api_tsc_button()					    **
//**		 api_tsc_signpad()					    **
//**		 api_tsc_getsign()					    **
//**		 api_tsc_status()					    **
//**									    **
//**  FUNCTION : API::TSC (Touch Screen Controller)			    **
//**  VERSION  : V1.00                                                      **
//**  DATE     : 2023/01/18                                                 **
//**  EDITOR   : James Hsieh                                                **
//**                                                                        **
//**  Copyright(C) 2023 SymLink Corporation. All rights reserved.           **
//**                                                                        **
//****************************************************************************
//============================================================================
//
//----------------------------------------------------------------------------
#include "POSAPI.h"
#include "OS_PROCS.h"
#include "IPC_client.h"

#include "DEV_TSC.h"


// ---------------------------------------------------------------------------
// FUNCTION: To enable the TSC device.
// INPUT   : deviceid (RFU)
//	     The device ID, there are two basic types of display are used in POS
//	     application, the operator's display and the customer's display.
//	     0x00 - default monochrome STN LCD
//	     0x01 - TFT color LCD (GF320x240)
// OUTPUT  : none.
// RETURN  : DeviceHandleNo
//	     apiOutOfService
// NOTE    : Prior to calling this function, OS_LCD_Init() shall be called first.
// ---------------------------------------------------------------------------
UCHAR	api_tsc_open( UCHAR deviceid )
{
UCHAR	retval;


	// IPC_clientHandler(UINT8 PID,UINT8 APInum,UINT8 ArgsNum,UINT32 IptSize,UINT8 *Arg,UINT8 *retval)
	IPC_clientHandler( psDEV_TSC, 1, 1, 1, &deviceid, &retval );
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To disable TSC device.
// INPUT   : dhn
//	     The specified device handle number.
//	     0x00 = to close all opened tasks.
// OUTPUT  : none.
// RETURN  : apiOK
//	     apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_tsc_close( UCHAR dhn )
{	
UCHAR	retval;


	// IPC_clientHandler(UINT8 PID,UINT8 APInum,UINT8 ArgsNum,UINT32 IptSize,UINT8 *Arg,UINT8 *retval)
	IPC_clientHandler( psDEV_TSC, 2, 1, 1, &dhn, &retval );
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To get status of TSC.
// INPUT   : dhn
//	     The specified device handle number.
//	     para	- parameters.
//
//
// OUTPUT  : status	- status byte.
//			  0: not available (no event)
//			  1: available     (event occurs)
// RETURN  : apiOK
//	     apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_tsc_status( UCHAR dhn, API_TSC_PARA para, UCHAR *status )
{
UCHAR	retval;
ULONG	argulen=1+sizeof(API_TSC_PARA);
UCHAR	buffer[argulen+1];


	memset( buffer, 0, argulen);
  	buffer[0] = dhn;
  	memmove( &buffer[1], &para, sizeof(API_TSC_PARA) );
  	
  	*status = 0;
  	
	// IPC_clientHandler(UINT8 PID,UINT8 APInum,UINT8 ArgsNum,UINT32 IptSize,UINT8 *Arg,UINT8 *retval)
  	IPC_clientHandler( psDEV_TSC, 3, 3, argulen, buffer, &retval );
  	if( retval == apiOK )
  	  *status = buffer[argulen];
	
	return retval;
}

// ---------------------------------------------------------------------------
// FUNCTION: To activate SignPad function.
// INPUT   : dhn
//	     The specified device handle number.
//	     para	- parameters.
//
//	        palette	- fixed 3 bytes for RGB. (for background RGB palette)
//
// OUTPUT  : status	- status byte.
//			  0: not available (abort or timeout)
//			  1: available	   (sign confirmed)
//			  2: change orientation and re-sign
//			
// RETURN  : apiOK
//	     apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_tsc_signpad( UCHAR dhn, API_TSC_PARA para, UCHAR *status, UCHAR *palette )
{
UCHAR	retval;
ULONG	argulen=1+sizeof(API_TSC_PARA)+3;
UCHAR	buffer[argulen];


	memset( buffer, 0, argulen );
  	buffer[0] = dhn;
  	memmove( &buffer[1], &para, sizeof(API_TSC_PARA) );
  	memmove( &buffer[1+sizeof(API_TSC_PARA)], palette, 3 );
  	
	// IPC_clientHandler(UINT8 PID,UINT8 APInum,UINT8 ArgsNum,UINT32 IptSize,UINT8 *Arg,UINT8 *retval)
  	IPC_clientHandler( psDEV_TSC, 4, 3, argulen, buffer, &retval );
  	if( retval == apiOK )
  	  *status = buffer[1];
	
	return retval;
}

// ---------------------------------------------------------------------------
// FUNCTION: To get image data of the signature.
// INPUT   : dhn
//	     The specified device handle number.
//	     para	- parameters.
//			  .RFU[0] = 0: the output format is raw data.
//			  .RFU[0] = 1: the output format is Windows BMP.
//			  .RFU[0] = 2: the output format is Windows BMP + raw data.
//			  .RFU[1] = 0: normal direction (CCW_0)
//			  .RFU[1] = 1: reverse direction (CCW_180)
//
//
// OUTPUT  : status	- status byte.
//			  0: not available (no data)
//			  1: available	   (data available)
//	     sign	- image data of the signature (fixed 320x120 bitmap)
//			  available only when "status" = 1.
//	     length	- length of the image data.
//
//
// RETURN  : apiOK
//	     apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_tsc_getsign( UCHAR dhn, API_TSC_PARA para, UCHAR *status, UCHAR *sign, ULONG *length )
{
UCHAR	retval;
ULONG	argulen=1+sizeof(API_TSC_PARA);
//UCHAR	buffer[argulen];
UCHAR	*buffer;
UINT	iLen;


	buffer = malloc( argulen + 4 + ((320*120)/8 + 0x3E) );	// (dhn+para) + length[4] + sign[]
	if( !buffer )
	  return( apiFailed );
	
	memset( buffer, 0, argulen );
  	buffer[0] = dhn;
  	memmove( &buffer[1], &para, sizeof(API_TSC_PARA) );
  	
	// IPC_clientHandler(UINT8 PID,UINT8 APInum,UINT8 ArgsNum,UINT32 IptSize,UINT8 *Arg,UINT8 *retval)
  	IPC_clientHandler( psDEV_TSC, 5, 3, argulen, buffer, &retval );
  	if( retval == apiOK )
  	  {
  	  *status = buffer[1];				// status[1]
  	  iLen = buffer[2] + buffer[3]*0x100 + buffer[4]*0x10000 + buffer[5]*0x1000000;
  	  *length = iLen;
  	  memmove( sign, &buffer[2+4], iLen );	// image[length]
  	  }
	
	free( buffer );
	
	return retval;
}

// ---------------------------------------------------------------------------
// FUNCTION: To switch TSC function. (internal use only, called by Contactless LIB.
// INPUT   : flag
//	     0x01 = enter Contactless function. (will disable TSC to avoid the interference during CL process)
//	     0x00 = exit  Contactless function. (will restore TSC if it was enabled)
// OUTPUT  : none.
// RETURN  : apiOK
//	     apiFailed
// ---------------------------------------------------------------------------
UCHAR	apk_tsc_switch( UCHAR flag )
{
	return( apiOK );
}
