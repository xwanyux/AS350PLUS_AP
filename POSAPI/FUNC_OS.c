//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : MAX32550 (32-bit Platform)				    **
//**  PRODUCT  : AS330_mPOS						    **
//**                                                                        **
//**  FILE     : FUNC_OS.C						    **
//**  MODULE   :							    **
//**									    **
//**  FUNCTION : OS::xxx (OS function call)				    **
//**  VERSION  : V1.00                                                      **
//**  DATE     : 2019/01/15                                                 **
//**  EDITOR   : James Hsieh                                                **
//**                                                                        **
//**  Copyright(C) 2019 SymLink Corporation. All rights reserved.           **
//**                                                                        **
//****************************************************************************
//============================================================================
//
//----------------------------------------------------------------------------
#include <stdlib.h> /*for system function*/
#include <string.h>

#include "bsp_types.h"
//#include "bsp_wdt.h"
//#include "za9_pmu.h"

#include "POSAPI.h"
#include "OS_PROCS.h"
#include "IPC_client.h"


extern	volatile	UINT32	os_SysTimerFreeCnt;	// DEV_TIM.c

extern	volatile	UINT32	os_ScEventFlag;		// BSP_SC.c	(to be implemented)

extern	volatile	UINT32	os_MsrEventFlag;	// BSP_MCR.c	(to be implemented)

extern	volatile	UINT32	os_KbdEventFlag;	// DEV_KBD.c

//extern	BSP_WDT		BspWdt;				// bsp_wdt.c
//extern	BSP_SEM		WdtAvailable;			//


// ---------------------------------------------------------------------------
// FUNCTION: Get the value of system global variable "os_SysTimerFreeCnt".
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : value.
// ---------------------------------------------------------------------------
#if	1
ULONG	OS_GET_SysTimerFreeCnt( void )
{
UCHAR	retval;
UCHAR	sbuf[16];
ULONG	cnt;


	// void IPC_clientHandler(UINT8 PID, UINT8 APInum, UINT8 ArgsNum, UINT32 IptSize, UINT8 *Arg, UINT8 *retval)
	IPC_clientHandler( psDEV_OS, 1, 0, 0, sbuf, &retval );
	
	cnt = sbuf[0] + sbuf[1]*0x100 + sbuf[2]*0x10000 + sbuf[3]*0x1000000;
	return( cnt );
}
#endif

// ---------------------------------------------------------------------------
// FUNCTION: Set the value of system global variable "os_SysTimerFreeCnt".
// INPUT   : value to be set.
// OUTPUT  : none.
// RETURN  : none.
// ---------------------------------------------------------------------------
#if	1
void	OS_SET_SysTimerFreeCnt( ULONG value )
{
#if	0
	os_SysTimerFreeCnt = value;
#else

UCHAR	retval;
UCHAR	sbuf[16];


	// void IPC_clientHandler(UINT8 PID, UINT8 APInum, UINT8 ArgsNum, UINT32 IptSize, UINT8 *Arg, UINT8 *retval)
	sbuf[0] =  value & 0x000000ff;
	sbuf[1] = (value & 0x0000ff00) >> 8;
	sbuf[2] = (value & 0x00ff0000) >> 16;
	sbuf[3] = (value & 0xff000000) >> 24;
	IPC_clientHandler( psDEV_OS, 2, 1, 4, sbuf, &retval );

#endif
}
#endif

// ---------------------------------------------------------------------------
// FUNCTION: Get the value of system global variable "os_KbdEventFlag".
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : value.
// ---------------------------------------------------------------------------
ULONG	OS_GET_KbdEventFlag( void )
{
	UCHAR dhn;
	UCHAR sbuf[5];
	UCHAR dbuf;
    sbuf[0] = 0xff; // 7, 4, 1
    sbuf[1] = 0xff; // 0, 8, 5, 2
    sbuf[2] = 0xff; // 9, 6, 3
    sbuf[3] = 0xff; // ENTER, CLEAR, CANCEL
	dhn = api_kbd_open(0, sbuf);

	if(api_kbd_status(dhn, &dbuf) == apiReady)
		return TRUE;
	return FALSE;

#ifdef	_KBD_ENABLED_
	return( os_KbdEventFlag );
#else
	return( 0 );
#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: Set the value of system global variable "os_KbdEventFlag".
// INPUT   : value to be set.
// OUTPUT  : none.
// RETURN  : none.
// ---------------------------------------------------------------------------
void	OS_SET_KbdEventFlag( ULONG value )
{
#ifdef	_KBD_ENABLED_
	os_KbdEventFlag = value;
#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: Get the value of system global variable "os_ScEventFlag".
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : value.
// ---------------------------------------------------------------------------
#if	0
ULONG	OS_GET_ScEventFlag( void )
{
	return( os_ScEventFlag );
}
#endif

// ---------------------------------------------------------------------------
// FUNCTION: Set the value of system global variable "os_ScEventFlag".
// INPUT   : value to be set.
// OUTPUT  : none.
// RETURN  : none.
// ---------------------------------------------------------------------------
#if	0
void	OS_SET_ScEventFlag( ULONG value )
{
	os_ScEventFlag = value;
}
#endif

// ---------------------------------------------------------------------------
// FUNCTION: Get the value of system global variable "os_MsrEventFlag".
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : value.
// ---------------------------------------------------------------------------
#if	0
ULONG	OS_GET_MsrEventFlag( void )
{
	return( os_MsrEventFlag );
}
#endif

// ---------------------------------------------------------------------------
// FUNCTION: Set the value of system global variable "os_MsrEventFlag".
// INPUT   : value to be set.
// OUTPUT  : none.
// RETURN  : none.
// ---------------------------------------------------------------------------
#if	0
void	OS_SET_MsrEventFlag( ULONG value )
{
	os_MsrEventFlag = value;
}
#endif

// ---------------------------------------------------------------------------
// FUNCTION: Write data to the specified address.
// INPUT   : address - start address to write. (0..x)
//	     length  - length of data to write.
//	     data    - data to be written.
//	     id     - group id (0..3)
// OUTPUT  : none.
// RETURN  : TRUE  -- OK
//           FALSE -- Failed
// ---------------------------------------------------------------------------
ULONG	OS_SECM_PutData_EFC( UCHAR id, ULONG address, ULONG length, UCHAR *data )
{
UCHAR	retval;
UCHAR	*sbuf;
ULONG	iptlen;


	iptlen = 1 + 4 + 4 + length;
	sbuf = malloc( iptlen );
	if( sbuf == NULL )
	  return( FALSE );
	  
	sbuf[0] = id;
	
	sbuf[1] =  address & 0x000000ff;
	sbuf[2] = (address & 0x0000ff00) >> 8;
	sbuf[3] = (address & 0x00ff0000) >> 16;
	sbuf[4] = (address & 0xff000000) >> 24;
	
	sbuf[5] =  length & 0x000000ff;
	sbuf[6] = (length & 0x0000ff00) >> 8;
	sbuf[7] = (length & 0x00ff0000) >> 16;
	sbuf[8] = (length & 0xff000000) >> 24;
	
	memmove( &sbuf[9], data, length );

	// void IPC_clientHandler(UINT8 PID, UINT8 APInum, UINT8 ArgsNum, UINT32 IptSize, UINT8 *Arg, UINT8 *retval)
	IPC_clientHandler( psDEV_OS, 3, 4, iptlen, sbuf, &retval );
	
	free(sbuf);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: Read data from the specified address.
// INPUT   : address - start address to read. (0..x)
//	     length  - length of data to read.
//	     id     - group id (0..3)
// OUTPUT  : data    - storage of data read.
// RETURN  : TRUE  -- OK
//           FALSE -- Failed
// ---------------------------------------------------------------------------
ULONG	OS_SECM_GetData_EFC( UCHAR id, ULONG address, ULONG length, UCHAR *data )
{
UCHAR	retval;
UCHAR	*sbuf;
ULONG	iptlen;


	iptlen = 1 + 4 + 4 + length;
	sbuf = malloc( iptlen );
	if( sbuf == NULL )
	  return( FALSE );
	  
	sbuf[0] = id;
	
	sbuf[1] =  address & 0x000000ff;
	sbuf[2] = (address & 0x0000ff00) >> 8;
	sbuf[3] = (address & 0x00ff0000) >> 16;
	sbuf[4] = (address & 0xff000000) >> 24;
	
	sbuf[5] =  length & 0x000000ff;
	sbuf[6] = (length & 0x0000ff00) >> 8;
	sbuf[7] = (length & 0x00ff0000) >> 16;
	sbuf[8] = (length & 0xff000000) >> 24;

	// void IPC_clientHandler(UINT8 PID, UINT8 APInum, UINT8 ArgsNum, UINT32 IptSize, UINT8 *Arg, UINT8 *retval)
//	IPC_clientHandler( psDEV_OS, 4, 4, iptlen, sbuf, &retval );
	IPC_clientHandler( psDEV_OS, 4, 3, 1+4+4, sbuf, &retval );	// 2023-10-17
	
	if( retval )
	  memmove( data, &sbuf[9], length );
	
	free(sbuf);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: Read MCU serial number.
// INPUT   : bpSNLen	- max length of MCU serail number read.
// OUTPUT  : bpMcuSN	- MCU serial number.
//	     bpSNLen	- actual length of MCU serial number.
// RETURN  : TRUE
//	     FALSE
// ---------------------------------------------------------------------------
ULONG	OS_MCU_ReadSN( UCHAR *bpMcuSN, UCHAR *bpSNLen )
{
UCHAR	retval;
UCHAR	sbuf[16];
ULONG	iptlen;


	// void IPC_clientHandler(UINT8 PID, UINT8 APInum, UINT8 ArgsNum, UINT32 IptSize, UINT8 *Arg, UINT8 *retval)
	sbuf[0] = *bpSNLen;
	IPC_clientHandler( psDEV_OS, 5, 1, 1, sbuf, &retval );
	
	if( retval )
	  {
	  *bpSNLen = 4;	// sbuf[0];
	  memmove( bpMcuSN, &sbuf[1], 4 );
	  }
	
	return( retval );
}






#if	0
// ---------------------------------------------------------------------------
// FUNCTION: To setup WDT resources,
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : NULLPTR		-- failed
//	     WDT pointer	-- OK
// ---------------------------------------------------------------------------
BSP_WDT	*OS_WDT_Acquire( void )
{
BSP_WDT	*pWdt;


	pWdt = &BspWdt;

	/*
	 * The timer has been acquired.
	 */
	pWdt->pIsrFunc = NULLPTR;
	pWdt->pInt = BSP_INT_Acquire( INTNUM_WDT, NULLPTR );
	if( pWdt->pInt == NULLPTR )
	  {
	  BSP_WDT_Release( pWdt );
	  pWdt = NULLPTR;
	  }

	return( pWdt );
}

// ---------------------------------------------------------------------------
// FUNCTION: To enable system WDT (Watch Dog Timer).
//	     Default Settings (in BSP_WDT_Init())
//		Interrupt Period = 0.745 seconds
//		Reset     Period = 1.49  seconds
//
// INPUT   : pWdt - reference to BSP_WDT structure.
// OUTPUT  : none.
// RETURN  : TRUE
//	     FALSE
// ---------------------------------------------------------------------------
UINT32	OS_WDT_Start( BSP_WDT *pWdt )
{
UINT32	Div;


	Div = 5;	// RST_PERIOD = 2^26 / 90M = 0.75s 
			// INT_PERIOD = 2^25 / 90M = 0.37s
	pWdt->Control  = WDT_RST_EN
		       | WDT_INT_EN
		       | WDT_EN
		       | (Div << 4)
		       | (Div + 1);

	WdtAvailable = FALSE;
	if( BSP_WDT_Start( pWdt ) == BSP_SUCCESS )
	  return( TRUE );
	else
	  return( FALSE );
}

// ---------------------------------------------------------------------------
// FUNCTION: To clear WDT interrupt.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
// ---------------------------------------------------------------------------
void	OS_WDT_ClearInterrupt( void )
{
	/*
	 * Clear the WDT interrupt
	 */
	BSP_WR32( WDT_RR_REG, WDT_RR_FIRST );
	BSP_WR32( WDT_RR_REG, WDT_RR_SECOND );
}

// ---------------------------------------------------------------------------
// FUNCTION: Read MCU serial number.
// INPUT   : bpSNLen	- max length of MCU serail number read.
// OUTPUT  : bpMcuSN	- MCU serial number.
//	     bpSNLen	- actual length of MCU serial number.
// RETURN  : TRUE
//	     FALSE
// ---------------------------------------------------------------------------
UINT32	OS_MCU_ReadSN( UINT8 *bpMcuSN, UINT8 *bpSNLen )
{
UINT32	result = FALSE;
UINT32	data;


	if( *bpSNLen >= 4 )
	  {
	  data = BSP_RD32(PMU_ID0_REG);
	  *(bpMcuSN+0) =  data & 0x000000FF;
	  *(bpMcuSN+1) = (data & 0x0000FF00) >> 8;
	  *(bpMcuSN+2) = (data & 0x00FF0000) >> 16;
	  *(bpMcuSN+3) = (data & 0xFF000000) >> 24;
	  
	  *bpSNLen = 4;
	  result = TRUE;
	  }
	  
	return( result );
}
#endif
