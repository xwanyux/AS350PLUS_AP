//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : EMV L2                                                     **
//**  PRODUCT  : AS320-A                                                    **
//**                                                                        **
//**  FILE     : APK_SRAM.C                                                 **
//**  MODULE   : apk_WriteRamDataTERM()                                     **
//**             apk_ReadRamDataTERM()                                      **
//**             apk_ClearRamDataTERM()                                     **
//**             apk_WriteRamDataICC()                                      **
//**             apk_ClearRamDataICC()                                      **
//**             apk_ReadRamDataICC()                                       **
//**             apk_WriteRamDataKEY()                                      **
//**             apk_ReadRamDataKEY()                                       **
//**             apk_ClearRamDataKEY()                                      **
//**             apk_PushRamData()                                          **
//**             apk_PopRamData()                                           **
//**  VERSION  : V1.00                                                      **
//**  DATE     : 2002/12/08                                                 **
//**  EDITOR   : James Hsieh                                                **
//**                                                                        **
//**  Copyright(C) 2002-2007 SymLink Corporation. All rights reserved.      **
//**                                                                        **
//****************************************************************************
//============================================================================
//
//----------------------------------------------------------------------------
// PAGE 14: 32KB, data elements.
// PAGE 15: 16KB, key mamagement.
//----------------------------------------------------------------------------
#include "POSAPI.h"
//#include "EMVDC.h"
#include "GDATAEX.h"
#include "EMVAPI.h"
//#include <EMVAPK.h>
//#include <APDU.H>
//#include <TOOLS.h>
#include "OS_PROCS.h"
#include "IPC_client.h"

//API_SRAM *pSram1;

// ---------------------------------------------------------------------------
// FUNCTION: To write ICC or TERMINAL data element to SRAM page memory.
// INPUT   : address - begin address to write.
//           length  - length of the data element.
//           data    - the data element.
// OUTPUT  : none.
// RETURN  : apkOK
//           apkFailed
// ---------------------------------------------------------------------------
UCHAR apk_WriteRamDataTERM( ULONG address, ULONG length, UCHAR *data )
{
#ifdef	PLATFORM_16BIT

UCHAR sbuf[7];

      sbuf[0] = SRAM_PAGE_TERM; // all data elements in sram
      sbuf[1] = address & 0x00ff;
      sbuf[2] = (address & 0xff00) >> 8;
      sbuf[3] = 0x00;
      sbuf[4] = 0x00;
      sbuf[5] = length & 0x00ff;
      sbuf[6] = (length & 0xff00) >> 8;
      return( api_ram_write( sbuf, data ) );
      
#endif

#ifdef	_SRAM_PAGE_

API_SRAM pSram;

	pSram.StPage = SRAM_PAGE_TERM;
	pSram.StAddr = address;
	pSram.Len = length;
	return( api_sram_PageWrite( pSram, data ) );

#endif

#ifndef	_SRAM_PAGE_

#if 0
UCHAR	*pSramMemory = (UCHAR *)SRAM_BASE_EMV_TERM;

	return( api_sram_write( pSramMemory+address, length, data ) );
#else
    UCHAR   retval;
    UCHAR   args[4 + 4 + length];
    UCHAR	*pSramMemory = (UCHAR *)SRAM_BASE_EMV_TERM;
    ULONG   addr = (ULONG)(pSramMemory + address);
   

    memmove(args, &addr, 4);
    memmove(&args[4], &length, 4);
    memmove(&args[4 + 4], data, length);
    IPC_clientHandler(psDEV_OSSRAM, 1, 3, 4 + 4 + length, args, &retval);

    return retval;
#endif

#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: To write ICC or TERMINAL data element to SRAM page memory.
// INPUT   : address - begin address to write.
//           length  - length of the data element.
//           data    - the data element.
// OUTPUT  : none.
// RETURN  : apkOK
//           apkFailed
// ---------------------------------------------------------------------------
UCHAR apk_WriteRamDataICC( ULONG address, ULONG length, UCHAR *data )
{
#ifdef	PLATFORM_16BIT

UCHAR sbuf[7];

      sbuf[0] = SRAM_PAGE_ICC; // all data elements in sram
      sbuf[1] = address & 0x00ff;
      sbuf[2] = (address & 0xff00) >> 8;
      sbuf[3] = 0x00;
      sbuf[4] = 0x00;
      sbuf[5] = length & 0x00ff;
      sbuf[6] = (length & 0xff00) >> 8;
      return( api_ram_write( sbuf, data ) );
      
#endif

#ifdef	_SRAM_PAGE_

API_SRAM pSram;

	pSram.StPage = SRAM_PAGE_ICC;
	pSram.StAddr = address;
	pSram.Len = length;
	return( api_sram_PageWrite( pSram, data ) );
	
#endif

#ifndef	_SRAM_PAGE_

#if 0
UCHAR	*pSramMemory = (UCHAR *)SRAM_BASE_EMV_ICC;

	return( api_sram_write( pSramMemory+address, length, data ) );
#else
    UCHAR   retval;
    UCHAR   args[4 + 4 + length];
    UCHAR	*pSramMemory = (UCHAR *)SRAM_BASE_EMV_ICC;
    ULONG   addr = (ULONG)(pSramMemory + address);
   

    // ==== [Debug] ====
    // printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);

    // printf("address = %p\n", address);
    // printf("address = 0x%04lx\n", address);
    // printf("addr = %p\n", addr);
    // printf("addr = 0x%04lx\n", addr);

    // printf("length = %d\n", length);

    // printf("data:");
    // for(int i = 0 ; i < length ; i++)
    //     printf("%02X ", data[i]);
    // printf("\n");
    // ==== [Debug] ====

    memmove(args, &addr, 4);
    memmove(&args[4], &length, 4);
    memmove(&args[4 + 4], data, length);
    IPC_clientHandler(psDEV_OSSRAM, 1, 3, 4 + 4 + length, args, &retval);

    return retval;
#endif

#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: To write CA Public Key data element to SRAM page memory.
// INPUT   : address - begin address to write.
//           length  - length of the data element.
//           data    - the data element.
// OUTPUT  : none.
// RETURN  : apkOK
//           apkFailed
// ---------------------------------------------------------------------------
UCHAR apk_WriteRamDataKEY( ULONG address, ULONG length, UCHAR *data )
{
      printf("apk_WriteRamDataKEY\n");
#ifdef	PLATFORM_16BIT

UCHAR sbuf[7];

      sbuf[0] = SRAM_PAGE_KEY; // all data elements in sram
      sbuf[1] = address & 0x00ff;
      sbuf[2] = (address & 0xff00) >> 8;
      sbuf[3] = 0x00;
      sbuf[4] = 0x00;
      sbuf[5] = length & 0x00ff;
      sbuf[6] = (length & 0xff00) >> 8;
      return( api_ram_write( sbuf, data ) );

#endif

#ifdef	_SRAM_PAGE_

API_SRAM pSram;
memset(&pSram,0,sizeof(pSram));
	pSram.StPage = SRAM_PAGE_KEY;
	pSram.StAddr = address;
	pSram.Len = length;
      printf("StPage=%d address=%d  length=%d",SRAM_PAGE_KEY,address,length);
	return( api_sram_PageWrite( pSram, data ) );
	
#endif

#ifndef	_SRAM_PAGE_

#if 0
UCHAR	*pSramMemory = (UCHAR *)SRAM_BASE_EMV_KEY;

	return( api_sram_write( pSramMemory+address, length, data ) );
#else
    UCHAR   retval;
    UCHAR   args[4 + 4 + length];
    UCHAR	*pSramMemory = (UCHAR *)SRAM_BASE_EMV_KEY;
    ULONG   addr = (ULONG)(pSramMemory + address);
   

    memmove(args, &addr, 4);
    memmove(&args[4], &length, 4);
    memmove(&args[4 + 4], data, length);
    IPC_clientHandler(psDEV_OSSRAM, 1, 3, 4 + 4 + length, args, &retval);

    return retval;
#endif
	
#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: To clear ICC or TERMINAL data element to specified pattern.
// INPUT   : address - begin address to write.
//           length  - length of the data element.
//           data    - the data element.
// OUTPUT  : none.
// RETURN  : apkOK
//           apkFailed
// ---------------------------------------------------------------------------
UCHAR apk_ClearRamDataICC( ULONG address, ULONG length, UCHAR pattern )
{
#ifdef	PLATFORM_16BIT

UCHAR sbuf[7];

      sbuf[0] = SRAM_PAGE_ICC; // all data elements in sram
      sbuf[1] = address & 0x00ff;
      sbuf[2] = (address & 0xff00) >> 8;
      sbuf[3] = 0x00;
      sbuf[4] = 0x00;
      sbuf[5] = length & 0x00ff;
      sbuf[6] = (length & 0xff00) >> 8;
      return( api_ram_clear( sbuf, pattern ) );

#endif

#ifdef	_SRAM_PAGE_

API_SRAM pSram;

	pSram.StPage = SRAM_PAGE_ICC;
	pSram.StAddr = address;
	pSram.Len = length;
	return( api_sram_PageClear( pSram, pattern ) );
	
#endif

#ifndef	_SRAM_PAGE_

#if 0
UCHAR	*pSramMemory = (UCHAR *)SRAM_BASE_EMV_ICC;

	return( api_sram_clear( pSramMemory+address, length, pattern ) );
#else
    UCHAR   retval;
    UCHAR   args[4 + 4 + 1];
    UCHAR	*pSramMemory = (UCHAR *)SRAM_BASE_EMV_ICC;
    ULONG   addr = (ULONG)(pSramMemory + address);
   

    // ==== [Debug] ====
    // printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);

    // printf("address = %p\n", address);
    // printf("address = 0x%04lx\n", address);
    // printf("addr = %p\n", addr);
    // printf("addr = 0x%04lx\n", addr);

    // printf("length = %d\n", length);

    // printf("pattern: %02X\n", pattern);
    // ==== [Debug] ====

    memmove(args, &addr, 4);
    memmove(&args[4], &length, 4);
    args[4 + 4] = pattern;
    IPC_clientHandler(psDEV_OSSRAM, 3, 3, 4 + 4 + 1, args, &retval);
    
    return retval;
#endif

#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: To clear ICC or TERMINAL data element to specified pattern.
// INPUT   : address - begin address to write.
//           length  - length of the data element.
//           data    - the data element.
// OUTPUT  : none.
// RETURN  : apkOK
//           apkFailed
// ---------------------------------------------------------------------------
UCHAR apk_ClearRamDataTERM( ULONG address, ULONG length, UCHAR pattern )
{
#ifdef	PLATFORM_16BIT

UCHAR sbuf[7];

      sbuf[0] = SRAM_PAGE_TERM; // all data elements in sram
      sbuf[1] = address & 0x00ff;
      sbuf[2] = (address & 0xff00) >> 8;
      sbuf[3] = 0x00;
      sbuf[4] = 0x00;
      sbuf[5] = length & 0x00ff;
      sbuf[6] = (length & 0xff00) >> 8;
      return( api_ram_clear( sbuf, pattern ) );

#endif

#ifdef	_SRAM_PAGE_

API_SRAM pSram;

	pSram.StPage = SRAM_PAGE_TERM;
	pSram.StAddr = address;
	pSram.Len = length;
	return( api_sram_PageClear( pSram, pattern ) );
	
#endif

#ifndef	_SRAM_PAGE_

#if 0
UCHAR	*pSramMemory = (UCHAR *)SRAM_BASE_EMV_TERM;

	return( api_sram_clear( pSramMemory+address, length, pattern ) );
#else
    UCHAR   retval;
    UCHAR   args[4 + 4 + 1];
    UCHAR	*pSramMemory = (UCHAR *)SRAM_BASE_EMV_TERM;
    ULONG   addr = (ULONG)(pSramMemory + address);
    

    memmove(args, &addr, 4);
    memmove(&args[4], &length, 4);
    args[4 + 4] = pattern;
    IPC_clientHandler(psDEV_OSSRAM, 3, 3, 4 + 4 + 1, args, &retval);
    
    return retval;
#endif

#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: To clear CA Public Key data element to specified pattern.
// INPUT   : address - begin address to write.
//           length  - length of the data element.
//           data    - the data element.
// OUTPUT  : none.
// RETURN  : apkOK
//           apkFailed
// ---------------------------------------------------------------------------
UCHAR apk_ClearRamDataKEY( ULONG address, ULONG length, UCHAR pattern )
{
#ifdef	PLATFORM_16BIT

UCHAR sbuf[7];

      sbuf[0] = SRAM_PAGE_KEY; // all data elements in sram
      sbuf[1] = address & 0x00ff;
      sbuf[2] = (address & 0xff00) >> 8;
      sbuf[3] = 0x00;
      sbuf[4] = 0x00;
      sbuf[5] = length & 0x00ff;
      sbuf[6] = (length & 0xff00) >> 8;
      return( api_ram_clear( sbuf, pattern ) );

#endif

#ifdef _SRAM_PAGE_

API_SRAM pSram;

	pSram.StPage = SRAM_PAGE_KEY;
	pSram.StAddr = address;
	pSram.Len = length;
	return( api_sram_PageClear( pSram, pattern ) );
	
#endif

#ifndef	_SRAM_PAGE_

#if 0
UCHAR	*pSramMemory = (UCHAR *)SRAM_BASE_EMV_KEY;

	return( api_sram_clear( pSramMemory+address, length, pattern ) );
#else
    UCHAR   retval;
    UCHAR   args[4 + 4 + 1];
    UCHAR	*pSramMemory = (UCHAR *)SRAM_BASE_EMV_KEY;
    ULONG   addr = (ULONG)(pSramMemory + address);
    

    memmove(args, &addr, 4);
    memmove(&args[4], &length, 4);
    args[4 + 4] = pattern;
    IPC_clientHandler(psDEV_OSSRAM, 3, 3, 4 + 4 + 1, args, &retval);
    
    return retval;
#endif

#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: To read ICC or TERMINAL data element from SRAM page memory.
// INPUT   : address - begin address to read.
//           length  - length of the data element.
// OUTPUT  : data    - the data element read.
// RETURN  : apkOK     - matched.
//           apkFailed - not matched.
// ---------------------------------------------------------------------------
UCHAR apk_ReadRamDataTERM( ULONG address, ULONG length, UCHAR *data )
{
#ifdef	PLATFORM_16BIT

UCHAR sbuf[7];

      sbuf[0] = SRAM_PAGE_TERM; // all data elements in sram
      sbuf[1] = address & 0x00ff;
      sbuf[2] = (address & 0xff00) >> 8;
      sbuf[3] = 0x00;
      sbuf[4] = 0x00;
      sbuf[5] = length & 0x00ff;
      sbuf[6] = (length & 0xff00) >> 8;
      return( api_ram_read( sbuf, data ) );
      
#endif

#ifdef	_SRAM_PAGE_

API_SRAM pSram;

	pSram.StPage = SRAM_PAGE_TERM;
	pSram.StAddr = address;
	pSram.Len = length;
	return( api_sram_PageRead( pSram, data ) );
	
#endif

#ifndef	_SRAM_PAGE_

#if 0
UCHAR	*pSramMemory = (UCHAR *)SRAM_BASE_EMV_TERM;
// printf("pSramMemory+address=%p\n",pSramMemory+address);
	return( api_sram_read( pSramMemory+address, length, data ) );
#else
    UCHAR   retval;
    UCHAR   args[4 + 4 + length];
    UCHAR	*pSramMemory = (UCHAR *)SRAM_BASE_EMV_TERM;
    ULONG   addr = (ULONG)(pSramMemory + address);


    memmove(args, &addr, 4);
    memmove(&args[4], &length, 4);
    IPC_clientHandler(psDEV_OSSRAM, 2, 3, 4 + 4, args, &retval);
    memmove(data, &args[4 + 4], length);

    return retval;
#endif

#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: To read ICC or TERMINAL data element from SRAM page memory.
// INPUT   : address - begin address to read.
//           length  - length of the data element.
// OUTPUT  : data    - the data element read.
// RETURN  : apkOK     - matched.
//           apkFailed - not matched.
// ---------------------------------------------------------------------------
UCHAR apk_ReadRamDataICC( ULONG address, ULONG length, UCHAR *data )
{
#ifdef	PLATFORM_16BIT

UCHAR sbuf[7];

      sbuf[0] = SRAM_PAGE_ICC; // all data elements in sram
      sbuf[1] = address & 0x00ff;
      sbuf[2] = (address & 0xff00) >> 8;
      sbuf[3] = 0x00;
      sbuf[4] = 0x00;
      sbuf[5] = length & 0x00ff;
      sbuf[6] = (length & 0xff00) >> 8;
      return( api_ram_read( sbuf, data ) );

#endif

#ifdef	_SRAM_PAGE_

API_SRAM pSram;

	pSram.StPage = SRAM_PAGE_ICC;
	pSram.StAddr = address;
	pSram.Len = length;
	return( api_sram_PageRead( pSram, data ) );
	
#endif

#ifndef	_SRAM_PAGE_

#if 0
UCHAR	*pSramMemory = (UCHAR *)SRAM_BASE_EMV_ICC;
// printf("pSramMemory+address=0x%04x  length=%d\n",pSramMemory+address,length);
	return( api_sram_read( pSramMemory+address, length, data ) );
#else
    UCHAR   retval;
    UCHAR   args[4 + 4 + length];
    UCHAR	*pSramMemory = (UCHAR *)SRAM_BASE_EMV_ICC;
    ULONG   addr = (ULONG)(pSramMemory + address);


    // ==== [Debug] ====
    // printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);

    // printf("address = %p\n", address);
    // printf("address = 0x%04lx\n", address);
    // printf("addr = %p\n", addr);
    // printf("addr = 0x%04lx\n", addr);

    // printf("length = %d\n", length);
    // ==== [Debug] ====

    memmove(args, &addr, 4);
    memmove(&args[4], &length, 4);
    IPC_clientHandler(psDEV_OSSRAM, 2, 3, 4 + 4, args, &retval);
    memmove(data, &args[4 + 4], length);

    // ==== [Debug] ====
    // printf("[AP]Get data:");
    // for(int i = 0 ; i < length ; i++)
    //     printf("%02X ", data[i]);
    // printf("\n");
    // ==== [Debug] ====

    return retval;
#endif    

#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: To read CA Public Key data element from SRAM page memory.
// INPUT   : address - begin address to read.
//           length  - length of the data element.
// OUTPUT  : data    - the data element read.
// RETURN  : apkOK     - matched.
//           apkFailed - not matched.
// ---------------------------------------------------------------------------
UCHAR apk_ReadRamDataKEY( ULONG address, ULONG length, UCHAR *data )
{
#ifdef	PLATFORM_16BIT

UCHAR sbuf[7];

      sbuf[0] = SRAM_PAGE_KEY; // all data elements in sram
      sbuf[1] = address & 0x00ff;
      sbuf[2] = (address & 0xff00) >> 8;
      sbuf[3] = 0x00;
      sbuf[4] = 0x00;
      sbuf[5] = length & 0x00ff;
      sbuf[6] = (length & 0xff00) >> 8;
      return( api_ram_read( sbuf, data ) );

#endif

#ifdef	_SRAM_PAGE_

API_SRAM pSram;

	pSram.StPage = SRAM_PAGE_KEY;
	pSram.StAddr = address;
	pSram.Len = length;
      printf("StPage=%d address=%d  length=%d",SRAM_PAGE_KEY,address,length);
	return( api_sram_PageRead( pSram, data ) );
	
#endif

#ifndef	_SRAM_PAGE_

#if 0
UCHAR	*pSramMemory = (UCHAR *)SRAM_BASE_EMV_KEY;
      
	return( api_sram_read( pSramMemory+address, length, data ) );
#else
    UCHAR   retval;
    UCHAR   args[4 + 4 + length];
    UCHAR	*pSramMemory = (UCHAR *)SRAM_BASE_EMV_KEY;
    ULONG   addr = (ULONG)(pSramMemory + address);


    memmove(args, &addr, 4);
    memmove(&args[4], &length, 4);
    IPC_clientHandler(psDEV_OSSRAM, 2, 3, 4 + 4, args, &retval);
    memmove(data, &args[4 + 4], length);

    return retval;
#endif

#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: PUSH an ITEM onto a stack array. (simulated in SRAM PAGE)
// INPUT   : stk    - the linear stack array. (NOT USED)
//           top    - pointer to the available position.
//           maxstk - max number of elements can be held in stack.
//           length - size of data elements.
//           item   - the elements to be pushed.
// OUTPUT  : none.
// RETURN  : TURE  = OK
//           FLASE = stack overflow
// ---------------------------------------------------------------------------
UCHAR apk_PushRamData( UCHAR *top, UCHAR maxstk, UINT length, UCHAR *item )
{
#ifdef	PLATFORM_16BIT

UCHAR sbuf[7];
UCHAR i;
UCHAR cnt;
UINT  address;

      if( *top == maxstk )
        return( FALSE );

      cnt = *top;
      address = 0;
      address += cnt*length;

      sbuf[0] = SRAM_PAGE_WORK;
      sbuf[1] = address & 0x00ff;
      sbuf[2] = (address & 0xff00) >> 8;
      sbuf[3] = 0x00;
      sbuf[4] = 0x00;
      sbuf[5] = length & 0x00ff;
      sbuf[6] = (length & 0xff00) >> 8;
      api_ram_write( sbuf, item );

      *top = cnt + 1;

      return( TRUE );
      
#endif

#ifdef	_SRAM_PAGE_

API_SRAM pSram;
UCHAR	cnt;
UINT	address;

	if( *top == maxstk )
        return( FALSE );

	cnt = *top;
	address = 0;
	address += cnt*length;

	pSram.StPage = SRAM_PAGE_WORK;
	pSram.StAddr = address;
	pSram.Len = length;
	api_sram_PageWrite( pSram, item );
	
	*top = cnt + 1;

	return( TRUE );
	
#endif

#ifndef	_SRAM_PAGE_

#if 0
UCHAR	*pSramMemory = (UCHAR *)SRAM_BASE_EMV_WORK;
UCHAR	cnt;
UINT	address;

	if( *top == maxstk )
        return( FALSE );

	cnt = *top;
	address = 0;
	address += cnt*length;

	api_sram_write( pSramMemory+address, length, item );

	*top = cnt + 1;

	return( TRUE );
#else
    UCHAR   retval;
    UCHAR   args[2 + 2 + length];
    UCHAR	*pSramMemory = (UCHAR *)SRAM_BASE_EMV_WORK;
    UCHAR	cnt;
    UINT	address;
    UINT    addr;
   

    if(*top == maxstk)
        return FALSE;

	cnt = *top;
	address = 0;
	address += cnt * length;

    addr = (UINT)(pSramMemory + address);
    memmove(args, &addr, 2);
    memmove(&args[2], &length, 2);
    memmove(&args[2 + 2], item, length);
    IPC_clientHandler(psDEV_OSSRAM, 1, 3, 2 + 2 + length, args, &retval);

    *top = cnt + 1;

    return TRUE;
#endif

#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: Delete the top element of stack and assign it to a item.
// INPUT   : stk    - the linear stack array. (NOT USED)
//           top    - pointer to the available position.
//           maxstk - max number of elements can be held in stack.
//           len    - size of data elements.
// OUTPUT  : item   - the elements to be pushed.
// RETURN  : TURE  = OK
//           FLASE = stack underflow
// ---------------------------------------------------------------------------
UCHAR apk_PopRamData( UCHAR *top, UINT length, UCHAR *item )
{
#ifdef	PLATFORM_16BIT

UCHAR sbuf[7];
UCHAR i;
UCHAR cnt;
UINT  address;

      if( *top == 0 )
        return( FALSE );

      cnt = *top;
      address = 0;
      address += (cnt-1)*length;

      sbuf[0] = SRAM_PAGE_WORK;
      sbuf[1] = address & 0x00ff;
      sbuf[2] = (address & 0xff00) >> 8;
      sbuf[3] = 0x00;
      sbuf[4] = 0x00;
      sbuf[5] = length & 0x00ff;
      sbuf[6] = (length & 0xff00) >> 8;
      api_ram_read( sbuf, item );

      *top = cnt - 1;

      return( TRUE );
      
#endif

#ifdef	_SRAM_PAGE_

API_SRAM pSram;
UCHAR	cnt;
UINT	address;

	if( *top == 0 )
        return( FALSE );

	cnt = *top;
	address = 0;
	address += (cnt-1)*length;

	pSram.StPage = SRAM_PAGE_WORK;
	pSram.StAddr = address;
	pSram.Len = length;
	api_sram_PageRead( pSram, item );
	
	*top = cnt - 1;

	return( TRUE );
	
#endif

#ifndef	_SRAM_PAGE_

#if 0
UCHAR	*pSramMemory = (UCHAR *)SRAM_BASE_EMV_WORK;
UCHAR	cnt;
UINT	address;

	if( *top == 0 )
        return( FALSE );

	cnt = *top;
	address = 0;
	address += (cnt-1)*length;

	api_sram_read( pSramMemory+address, length, item );

	*top = cnt - 1;

	return( TRUE );
#else
    UCHAR   retval;
    UCHAR   args[2 + 2 + length];
    UCHAR	*pSramMemory = (UCHAR *)SRAM_BASE_EMV_WORK;
    UCHAR	cnt;
    UINT	address;
    UINT    addr;


    if(*top == 0)
        return FALSE;

	cnt = *top;
	address = 0;
	address += (cnt - 1) * length;

    addr = (UINT)(pSramMemory + address);
    memmove(args, &addr, 2);
    memmove(&args[2], &length, 2);
    IPC_clientHandler(psDEV_OSSRAM, 2, 3, 2 + 2, args, &retval);
    memmove(item, &args[2 + 2], length);

    return retval;
#endif

#endif

}
