/*
 * Copyright 2006, ZiLOG Inc.
 * All Rights Reserved
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of ZiLOG Inc., and might
 * contain proprietary, confidential and trade secret information of
 * ZiLOG, our partners and parties from which this code has been licensed.
 * 
 * The contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of ZiLOG Inc.
 */

#include "POSAPI.h"
#include "OS_PROCS.h"
#include "IPC_client.h"
#include <stdio.h>   /* I/O Definitions                    */
#include <stdlib.h>
#include <stdarg.h>
void	_DEBUGPRINTF( char *Msg, ... )
{
char PrintfBuffer[300];
	va_list list_ptr;
	va_start(list_ptr,Msg);
	vprintf(Msg,list_ptr);
	va_end(list_ptr);
	// printf("%s",PrintfBuffer);
}
UINT8 BSP_UART_SendAck( UINT8 dhn, UINT8 acknum)
{
UCHAR retval;
UCHAR sbuf[2];
	sbuf[0]=dhn;
	sbuf[1]=acknum;
	IPC_clientHandler(psDEV_AUX,9,2,2,sbuf,&retval);
	return( retval );	
}
