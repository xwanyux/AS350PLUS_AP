//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : IMX6UL							    **
//**  PRODUCT  : AS350	                                                    **
//**                                                                        **
//**  FILE     : API_PRN.C 	                                            **
//**  MODULE   : api_prt_open()				                    **
//**		 api_prt_close()					    **
//**		 api_prt_status()					    **
//**		 api_prt_putstring()					    **
//**		 api_prt_putgraphics()					    **
//**		 api_prt_setlinespace()					    **
//**		 api_prt_setcharspace()					    **
//**		 api_prt_cut()						    **
//**		 api_prt_initfont()					    **
//**									    **
//**  FUNCTION : API::PRT (Printer Module)		    		    **
//**  VERSION  : V1.00                                                      **
//**  DATE     : 2023/05/25						    **
//**  EDITOR   : James Hsieh                                                **
//**                                                                        **
//**  Copyright(C) 2023 SymLink Corporation. All rights reserved.           **
//**                                                                        **
//****************************************************************************
//============================================================================
//
//----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h> /*for system function*/
#include <string.h>

#include "POSAPI.h"
#include "OS_PROCS.h"
#include "IPC_client.h"


// ---------------------------------------------------------------------------
// FUNCTION: To enable the printer device and reset the interface controller.
// INPUT   : config (RFU) - e.g., settings for darkness, printer type, etc.
// OUTPUT  : none.
// RETURN  : DeviceHandleNo
//           apiOutOfService
// ---------------------------------------------------------------------------
UCHAR	api_prt_open( UCHAR type, UCHAR mode )
{
UCHAR	retval;
UCHAR	sbuf[2];


	sbuf[0]=type;
	sbuf[1]=mode;
	IPC_clientHandler(psDEV_PRN,1,2,2,sbuf,&retval);
	  
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To disable the printer device.
// INPUT   : dhn
//	     The specified device handle number.
//	     0x00 = to close all opened tasks.
// OUTPUT  : none.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_prt_close( UCHAR dhn )
{
UCHAR	retval;


	IPC_clientHandler(psDEV_PRN,2,1,1,(UCHAR *)&dhn,&retval);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To get the printer status.
// INPUT   : dhn
//	     The specified device handle number.
// OUTPUT  : dbuf
//	     Printer status byte.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_prt_status( UCHAR dhn, UCHAR *dbuf )
{
UCHAR	retval;
UCHAR	sendbuff[3];


	sendbuff[0]=dhn;
	IPC_clientHandler(psDEV_PRN,3,1,1,sendbuff,&retval);
	if(retval==apiOK)
	{
		memmove(dbuf,sendbuff+1,1);
	}
	
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To write the given character string to printing device 
//	     by the specified font.
// INPUT   : dhn
//	     The specified device handle number.
//	     fontid - printer font id.
//	     	      APS_FONT_8x8
//		      APS_FONT_12x10
//                    APS_FONT_7x8
//		      APS_FONT_8x14
//		      SII_FONT_8x16
//		      SII_FONT_12x24
//	     sbuf
//	     UCHAR  length;        // length of the given string.
//	     UCHAR  data[length];  // character string to be printed.
//
// OUTPUT  : none.
// RETURN  : apiOK
//	     apiFailed
//	     apiNotReady (out of system memory allocation, to be re-tried)
// ---------------------------------------------------------------------------
UCHAR	api_prt_putstring( UCHAR dhn, UCHAR fontid, UCHAR *sbuf )
{	
UCHAR	retval;
UCHAR	iptlen=*sbuf;
UCHAR	sendbuff[iptlen+3];

	
	sendbuff[0]=dhn;
	sendbuff[1]=fontid;
	memmove(&sendbuff[2],sbuf,iptlen+1);
	IPC_clientHandler(psDEV_PRN,4,3,iptlen+3,sendbuff,&retval);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To write the given image data to printing device
//	     by the specified displacement.
// INPUT   : dhn
//	     The specified device handle number.
//
//	     dim
//	     The dimension of the graphics to be printed.
//	     ULONG   xmove;         // horizontal displacement in dots. (x8)
//	     ULONG   ymove ;        // vertical displacement in dots. (RFU)
//	     ULONG   width ;        // image width in dots in the horizontal direction. (x8)
//	     ULONG   height ;       // image height in dots  in the vertical direction. (x8)
//	     ULONG   mode ;         // image data mode.
//
//	     dbuf
//	     image data.
//
// OUTPUT  : none.
// RETURN  : apiOK
//	     apiFailed
//	     apiNotReady (out of system memory allocation, to be re-tried)
// ---------------------------------------------------------------------------
UCHAR	api_prt_putgraphics( UCHAR dhn, API_GRAPH dim, UCHAR *image )
{
UCHAR	retval;
ULONG	dbuflen=(dim.Width * dim.Height)/8;
ULONG	argulen=1+sizeof(API_GRAPH)+dbuflen;
//UCHAR	buffer[argulen+1];
UCHAR	*buffer;


	buffer = malloc(argulen);

	buffer[0]=dhn;
	memmove(&buffer[1],&dim,sizeof(API_GRAPH));
	memmove(&buffer[1+sizeof(API_GRAPH)],image,dbuflen);

	IPC_clientHandler(psDEV_PRN,5,3,argulen,buffer,&retval);

	free( buffer );
	
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To tune the space between two string lines.
// INPUT   : dhn
//	     The specified device handle number.
//	     space
//           The unit is in dots.
// OUTPUT  : none.
// RETURN  : apiOK
//	     apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_prt_setlinespace( UCHAR dhn, UCHAR space )
{
	return( apiFailed );
}

// ---------------------------------------------------------------------------
// FUNCTION: To get the space setting between two string lines.
// INPUT   : dhn
//	     The specified device handle number.
//	     space
//           The unit is in dots.
// OUTPUT  : none.
// RETURN  : apiOK
//	     apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_prt_getlinespace( UCHAR dhn, UCHAR *space )
{
	return( apiFailed );
}

// ---------------------------------------------------------------------------
// FUNCTION: To tune the space between two characters.
// INPUT   : dhn
//	     The specified device handle number.
//	     space
//           The unit is in dots. (default = 0)
// OUTPUT  : none.
// RETURN  : apiOK
//	     apiFailed
// ---------------------------------------------------------------------------
//UCHAR	api_prt_setcharspace( UCHAR dhn, UCHAR space )
//{
//}

// ---------------------------------------------------------------------------
// FUNCTION: To use cutter to cut the paper off.
// INPUT   : dhn
//	     The specified device handle number.
// OUTPUT  : none.
// RETURN  : apiOK
//	     apiFailed
// ---------------------------------------------------------------------------
//UCHAR	api_prt_cut( UCHAR dhn )
//{
//}

// ---------------------------------------------------------------------------
// FUNCTION: To initialize the external bitmap fonts for printer.
//	     The specified font should be downloaded to system memory prior to 
//	     initialization.
// INPUT   : ft
// 	     The "API_PRT_FONT" struture.
//	     Normal Fond ID: FONT10, FONT11, FONT12 ...
// OUTPUT  : none.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_prt_initfont( API_PRT_FONT ft  )
{
	return( apiFailed );
}

// ---------------------------------------------------------------------------
// FUNCTION: To return the bitmap address of the built-in Chinese FONT.
// INPUT   : fontid - font id.
// OUTPUT  : ft     - pointer to font structure.
// RETURN  : apiOK
//	     apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_prt_getfont( UCHAR fontid, API_PRT_FONT *ft  )
{
	return( apiFailed );
}

// ---------------------------------------------------------------------------
// FUNCTION: To tune the heater level for printing quality.
//	     currently it is only for barcode or QR code printing.
// INPUT   : level	= 0..4 (0=default)
// OUTPUT  : none.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
#if	0
UCHAR	api_prt_setheater( ULONG level )
{

}
#endif

// ---------------------------------------------------------------------------
// FUNCTION: To get the bitmap data of a specified font code.
// INPUT   : fontid - font id.
//	     code   - character code (1-byte ascii or 2-byte chinese)
// OUTPUT  : bmp    - bitmap data.
// RETURN  : length of bitmap depends on fontid (0=not found)
// ---------------------------------------------------------------------------
#if	0
ULONG	api_prt_getfontBMP( UCHAR fontid, UINT code, UCHAR *bmp  )
{

}
#endif
