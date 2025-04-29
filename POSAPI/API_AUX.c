//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : ZA9L                                                       **
//**  PRODUCT  : AS330/QS200                                                **
//**                                                                        **
//**  FILE     : API_AUX.C 	                                            **
//**  MODULE   : api_aux_open()				                    **
//**		 api_aux_close()					    **
//**		 api_aux_rxready()					    **
//**		 api_aux_txready()					    **
//**		 api_aux_rxstring()					    **
//**		 api_aux_txstring()					    **
//**		 api_aux_rxsetting()					    **
//**		 api_aux_SetModemControl()				    **
//**									    **
//**  FUNCTION : API::AUX (Auxiliary UART Module)			    **
//**  VERSION  : V1.00                                                      **
//**  DATE     : 2007/09/17                                                 **
//**  EDITOR   : James Hsieh                                                **
//**                                                                        **
//**  Copyright(C) 2007 SymLink Corporation. All rights reserved.           **
//**                                                                        **
//****************************************************************************
//============================================================================
//
//----------------------------------------------------------------------------
#include "OS_PROCS.h"
#include "POSAPI.h"
#include "DEV_AUX.h"
#include "IPC_client.h"
#include <stdio.h>
#include <stdlib.h>
UINT		AUX_RxreadyLength = 0;
// ---------------------------------------------------------------------------
// FUNCTION: To enable the auxiliary port.
// INPUT   : port -- UART port number.
// 	     pAux
//	     ->Mode		// data link protocol
//	     ->Baud		// baud rate
//	     ->Tob		// inter-byte timeout for receiving character string
//	     ->Tor		// timeout of waiting for response
//	     ->Resend		// re-transmit limit
//	     ->Acks		// no. of repetitive acknowledgement to the received message
//				// the followings are new elements
//	     ->BufferSize	// size of the transmit and receive driver buffer (in bytes)
//	     ->FlowCtrl		// flow control
//	     ->IoConfig		// io control for RTC,CTS,DTR,DSR...
//	     sbuf
//		same structure as API_AUX.
// OUTPUT  : none.
// RETURN  : DeviceHandleNumber
//           apiOutOfService
// ---------------------------------------------------------------------------
UCHAR	api_aux_open( UCHAR port, API_AUX pAux )
{
UCHAR retval;
UCHAR iptlen=sizeof(API_AUX)+1;
UCHAR sendbuff[iptlen];
	sendbuff[0]=port;
	memmove(sendbuff+1,&pAux,iptlen-1);
	IPC_clientHandler(psDEV_AUX,1,2,iptlen,sendbuff,&retval);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To disable AUX port.
// INPUT   : dhn
//	     The specified device handle number.
//	     0x00 = to close all opened tasks. (NOT SUPPORTED)
// OUTPUT  : none.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR 	api_aux_close( UCHAR dhn )
{	
UCHAR retval;
	IPC_clientHandler(psDEV_AUX,2,1,1,(UCHAR *)&dhn,&retval);
	return( retval );
	
}

// ---------------------------------------------------------------------------
// FUNCTION: To determine whether data is ready for input from the AUX device.
// INPUT   : dhn
//           The specified device handle number.
// OUTPUT  : dbuf
//           UINT  length ;     // length of the received data string.
//				if BYPASS mode is used, the caller can assign
//				the expected length of data to be received to
//				accelerate the performance.
//				if the value of "length" is 0, the incomming data
//				can only be captured after reaching TOB in BYPASS
//				mode.
// RETURN  : apiReady
//           apiNotReady
//	     apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_aux_rxready( UCHAR dhn, UCHAR *dbuf )
{
UCHAR retval;
UCHAR sendbuff[3];

	sendbuff[0]=dhn;
	IPC_clientHandler(psDEV_AUX,3,1,1,sendbuff,&retval);
	if(retval==apiReady)
	{
		memmove(dbuf,sendbuff+1,2);
		AUX_RxreadyLength=(*dbuf)|((*dbuf+1)<<8);
	}
	
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To read the incoming data string from AUX receiving stream buffer.
// INPUT   : dhn
//           The specified device handle number.
// OUTPUT  : dbuf
//           UINT  length ;     	// length of the received data string.
//	     UCHAR data[length] ;	// data string.	
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_aux_rxstring( UCHAR dhn, UCHAR *dbuf )
{
UCHAR retval;
UCHAR *sendbuff;

	sendbuff=malloc(AUX_RxreadyLength+2+1);//length of message + (UINT 2B)length + (1B)dhn
	sendbuff[0]=dhn;
	memmove(sendbuff+1,dbuf,AUX_RxreadyLength+2);
	IPC_clientHandler(psDEV_AUX,4,1,1,sendbuff,&retval);
	memmove(dbuf,sendbuff+1,AUX_RxreadyLength+2);
	free(sendbuff);
	AUX_RxreadyLength=0;
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To determine whether AUX port is ready to transmit the next data string.
// INPUT   : dhn
//           The specified device handle number.
// OUTPUT  : none.
// RETURN  : apiReady
//           apiNotReady
//	     apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_aux_txready( UCHAR dhn )
{
UCHAR retval;
	IPC_clientHandler(psDEV_AUX,5,1,1,&dhn,&retval);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To write the outgoing data string to AUX transmitting stream buffer.
// INPUT   : dhn
//           The specified device handle number.
//           sbuf
//           UINT  length ;     	// length of data string to be transmitted.
//	     UCHAR data[length] ;	// data string.	
// OUTPUT  : none.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_aux_txstring( UCHAR dhn, UCHAR *sbuf )
{
UCHAR retval;
UINT  iptlen=(*sbuf)+(*(sbuf+1)*256)+2;
UCHAR sendbuff[iptlen+1];

	
	sendbuff[0]=dhn;
	memmove(&sendbuff[1],sbuf,iptlen);
	IPC_clientHandler(psDEV_AUX,6,2,iptlen+1,sendbuff,&retval);
	return( retval );
}


/*
// ---------------------------------------------------------------------------
// FUNCTION: To alter the state of modem control signals. (RTS & DTR)
// INPUT   : dhn
//           The specified device handle number.
//           
//	     ctrl
//           The control state.
//	     	0 = de-assert both RTS and DTR
//		1 = assert DTR and de-assert RTS
//		2 = de-assert DTR and assert RTS
//		3 = assert both RTS and DTR
// OUTPUT  : none.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_aux_SetModemControl( UCHAR dhn, UCHAR ctrl  )
{
BSP_UART *pUart;

        pUart = AUX_CheckDHN( dhn );
	if( pUart == NULLPTR )
	  return( apiFailed );
	
	if( OS_AUX_SetModemControl( pUart, ctrl ) == TRUE )
	  return( apiOK );
	else
	  return( apiFailed );		
}

// ---------------------------------------------------------------------------
// FUNCTION: To get the state of modem control signals.
//		DCD, RI, DSR, CTS, DDCD, TERI, DDSR, DCTS
// INPUT   : dhn
//           The specified device handle number.
//           
//	     stat
//           The state of modem control signals.
//		bit0: DCTS	delta status change of CTS
//		   1: DDSR	delta status change of DSR
//		   2: TERI	trailing edge change on RI
//		   3: DDCD	delta status change of DCD
//		   4: CTS	inverted state of /CTS	(currently support)
//		   5: DSR	inverted state of /DSR
//		   6: RI	inverted state of /RI
//		   7: DCD	inverted state of /DCD
// OUTPUT  : none.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_aux_GetModemStatus( UCHAR dhn, UCHAR *stat )
{
BSP_UART *pUart;

        pUart = AUX_CheckDHN( dhn );
	if( pUart == NULLPTR )
	  return( apiFailed );
	
	if( OS_AUX_GetModemStatus( pUart, stat ) == TRUE )
	  return( apiOK );
	else
	  return( apiFailed );		
}
*/


// ---------------------------------------------------------------------------
// FUNCTION: To setup long length type for SOH protocol.
// INPUT   : dhn
//	     The specified device handle number.
//	     flag
//	     0 = 2 bytes length (default)
//	     1 = 4 bytes length
// OUTPUT  : none.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR 	api_aux_SetLongLen( UINT dhn, UCHAR flag )
{
UCHAR retval;
UCHAR sbuf[2];
	sbuf[0]=dhn;
	sbuf[1]=flag;
	IPC_clientHandler(psDEV_AUX,7,2,2,sbuf,&retval);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To setup ACK response behavior, real-time or after-service mode.
// INPUT   : dhn
//	     The specified device handle number.
//	     mode
//	     0 = real-time mode (default).
//	     1 = after-service mode, ACK only after APP has processed the
//		 incoming message, especially when using DLL protocol.
// OUTPUT  : none.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR 	api_aux_SetAckMode( UINT dhn, UCHAR mode )
{
UCHAR retval;
UCHAR sbuf[2];
	sbuf[0]=dhn;
	sbuf[1]=mode;
	IPC_clientHandler(psDEV_AUX,8,2,2,sbuf,&retval);
	  
	return( retval );
}
