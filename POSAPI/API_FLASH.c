//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : IMX6UL							    **
//**  PRODUCT  : AS350	                                                    **
//**                                                                        **
//**  FILE     : API_FLASH.C 	(NAND FLASH API)			    **
//**  MODULE   : 							    **
//**									    **
//**  FUNCTION : API::LCDTFT (TFT Liquid Crystal Display Module)	    **
//**  VERSION  : V1.00                                                      **
//**  DATE     : 2023/08/23                                                 **
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
#include "FLASHAPI.h"


// ---------------------------------------------------------------------------
// FUNCTION: To select FLASH type for access.
// INPUT   : Type	- flash type. (FLASH_TYPE_NOR or FLASH_TYPE_NAND)
// OUTPUT  : none.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
ULONG	api_flash_TypeSelect( ULONG Type )
{	
UCHAR	retval;
UCHAR	args[4];


	args[0] =  Type & 0x000000ff;
	args[1] = (Type & 0x0000ff00) >> 8;
	args[2] = (Type & 0x00ff0000) >> 16;
	args[3] = (Type & 0xff000000) >> 24;
	IPC_clientHandler( psDEV_FLS, 4, 1, 4, args, &retval );
	
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To request the accessible range of the given page number.
// INPUT   : Page		// the specified page number
// OUTPUT  : pAddr
//	     ->StAddr		// start address of the specified page
//	     ->EndAddr		// end   address of the specified page
// RETURN  : apiOK
//           apiFailed
// NOTE    : compatible to 8/16 bit application.
// ---------------------------------------------------------------------------
ULONG	api_flash_PageSelect( UINT Page, API_FLASH_ADDR *pAddr )
{
UCHAR	retval;
UCHAR	args[2+sizeof(API_FLASH_ADDR)];


	args[0] = Page & 0x00ff;
	args[2] = (Page & 0xff00) >> 8;
	
	IPC_clientHandler( psDEV_FLS, 5, 2, 2, args, &retval);
	memmove( pAddr, &args[2], sizeof(API_FLASH_ADDR) );
	
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To link the contiguous pages of FLASH into a linear memory location
//	     or free the allocated memory.
// INPUT   : pFls->StPage
//	     The beginning page number of the linked area.
//	     pFls->EndPage
//	     The end page number of the linked area.
//
//	     Action
//	     0 = to allocate the page link.
//	     1 = to free the page link.
// OUTPUT  : none.
// RETURN  : apiOK
//           apiFailed
// NOTE    : compatible to 8/16 bit application.
// ---------------------------------------------------------------------------
ULONG	api_flash_PageLink( API_FLASH pFls, UCHAR Action )
{
UCHAR	retval;
UCHAR	args[1+sizeof(API_FLASH)];

	
	memmove( args, &pFls, sizeof(API_FLASH) );
    	args[sizeof(API_FLASH)] = Action;
	IPC_clientHandler( psDEV_FLS, 6, 2, 1+sizeof(API_FLASH), args, &retval );
	
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To read the contents of NOR FLASH from the specified address.
// INPUT   : pFls
//	     ->StPage	// page number to read.
//	     ->StAddr	// logical beginning address to read.
//	     ->Len      // length of data to be read.
//
// OUTPUT  : pData
//	     Buffer to store the data read.
// RETURN  : apiOK
//           apiFailed
// NOTE    : compatible to 8/16 bit application.
// ---------------------------------------------------------------------------
ULONG	api_flash_PageRead( API_FLASH pFls, UCHAR *pData )
{
UCHAR	retval;
UCHAR	args[sizeof(API_FLASH)+pFls.Len];

	
	memmove( args, &pFls, sizeof(API_FLASH));
	IPC_clientHandler( psDEV_FLS, 7, 2, sizeof(API_FLASH), args, &retval );
	memmove( pData, args+sizeof(API_FLASH), pFls.Len );
	
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To write data into the NOR FLASH memory according to the given address.
// INPUT   : pFls
//	     ->StPage	// page number to read.
//	     ->StAddr	// logical beginning address to write.
//	     ->Len      // length of data to be written.
//
//	     pData
//	     Buffer to store the data to be written.
// OUTPUT  : none.
// RETURN  : apiOK
//           apiFailed
// NOTE    : compatible to 8/16 bit application.
// ---------------------------------------------------------------------------
ULONG	api_flash_PageWrite( API_FLASH pFls, UCHAR *pData )
{	
UCHAR	retval;
UCHAR	args[sizeof(API_FLASH)+pFls.Len];

	
	memmove( args, &pFls, sizeof(API_FLASH) );
	memmove( &args[sizeof(API_FLASH)], pData, pFls.Len );
	IPC_clientHandler( psDEV_FLS, 8, 2, sizeof(API_FLASH)+pFls.Len ,args, &retval );
	
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To clear the NOR FLASH memory according to the given address and pattern.
// INPUT   : pFls
//	     ->StPage	// page number to clear.
//	     ->StAddr	// logical beginning address.
//	     ->Len	// length of data to clear. If length=0, will clear the whole page.
//
//	     Pattern
//	     Pattern used to fill the specified area.
// OUTPUT  : none.
// RETURN  : apiOK
//           apiFailed
// NOTE    : compatible to 8/16 bit application.
// ---------------------------------------------------------------------------
ULONG	api_flash_PageClear( API_FLASH pFls, UCHAR Pattern )
{
UCHAR	retval;
UCHAR	args[sizeof(API_FLASH)+1];
	
	
	memmove( args, &pFls, sizeof(API_FLASH) );
	args[sizeof(API_FLASH)] = Pattern;
	IPC_clientHandler( psDEV_FLS, 9, 2, sizeof(API_FLASH)+1, args, &retval );
	
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To read the initial BBT info of NAND FLASH.
// INPUT   : none.
// OUTPUT  : info
//		bad block table, fixed length of 1024 bytes.
// RETURN  : none.
// ---------------------------------------------------------------------------
#if	0
void	api_flash_BadBlockInfo( UCHAR *info )
{
	OS_FLASH_BadBlockInfo( info );
}
#endif



// ---------------------------------------------------------------------------
// FUNCTION: To read the contents of FLASH from the specified address. (CTLS)
// INPUT   : id   - 0 = CAPK
//		    1 = PARAMETERS
//		    2 = IMEK
//		    3 = IAEK
//	     addr - offset address of the backup flash area.
//	     len  - length of data to be read.
// OUTPUT  : buf  - buffer to store the data to be read.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
ULONG	api_fls_read( UCHAR id, ULONG addr, ULONG len, UCHAR *buf )
{
	// add new psDEV_FLS entry here
UCHAR	retval;
UCHAR	*args;


	args = malloc( 1+4+4+len );
	
	args[0] = id;
	
	args[1] = addr & 0x000000ff;
	args[2] = (addr & 0x0000ff00) >> 8;
	args[3] = (addr & 0x00ff0000) >> 16;
	args[4] = (addr & 0xff000000) >> 24;

	args[5] = len & 0x000000ff;
	args[6] = (len & 0x0000ff00) >> 8;
	args[7] = (len & 0x00ff0000) >> 16;
	args[8] = (len & 0xff000000) >> 24;
	
	IPC_clientHandler( psDEV_FLS, 10, 3, (1+4+4), args, &retval );
	
	memmove( buf, args+1+4+4, len );
	
	free( args );
	
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To read the contents of FLASH from the specified address. (CTLS)
// INPUT   : id   - 0 = CAPK
//		    1 = PARAMETERS
//		    2 = IMEK
//		    3 = IAEK
//	     addr - offset address of the backup flash area.
//	     len  - length of data to be read.
// 	     buf  - buffer to store the data to be written.
//	     
// OUTPUT  : none.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
ULONG	api_fls_write( UCHAR id, ULONG addr, ULONG len, UCHAR *buf )
{
	// add new psDEV_FLS entry here
UCHAR	retval;
UCHAR	*args;


	args = malloc( 1+4+4+len );
	
	args[0] = id;
	
	args[1] = addr & 0x000000ff;
	args[2] = (addr & 0x0000ff00) >> 8;
	args[3] = (addr & 0x00ff0000) >> 16;
	args[4] = (addr & 0xff000000) >> 24;

	args[5] = len & 0x000000ff;
	args[6] = (len & 0x0000ff00) >> 8;
	args[7] = (len & 0x00ff0000) >> 16;
	args[8] = (len & 0xff000000) >> 24;

    memmove(&args[9], buf, len);
	
	IPC_clientHandler( psDEV_FLS, 11, 4, (1+4+4+len), args, &retval );
	
	free( args );
	
	return( retval );
}
