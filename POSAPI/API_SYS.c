




#include "POSAPI.h"
#include "OS_PROCS.h"
#include "IPC_client.h"

// ---------------------------------------------------------------------------
// FUNCTION: To generate an n-byte random number.
// INPUT   : len  -- length in bytes. (1..n)
// OUTPUT  : dbuf -- n-byte random number.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------

ULONG api_sys_random_len( UCHAR *dbuf, UINT len )
{
UCHAR retval;
UCHAR args[len+2];
	args[0]=len;
	args[1]=len>>8;
	memmove(&args[2],dbuf,4);
	IPC_clientHandler(psDEV_SYS,1,2,2,args,&retval);
	memmove(dbuf,&args[2],len);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To generate an 8-byte random number.
// INPUT   : none.
// OUTPUT  : dbuf -- 8-byte random number.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
ULONG api_sys_random( UCHAR *dbuf )
{
UCHAR retval;
    IPC_clientHandler(psDEV_SYS,2,1,0,dbuf,&retval);
    return( retval );
}

void	api_sys_reset( UCHAR target )
{
UCHAR retval;
    IPC_clientHandler(psDEV_SYS,3,1,1,target,&retval);
    return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To generate the digest (fixed 20 bytes) by using SHA-1 in one step.
// INPUT   : length -- length of data to be hashed.
//           data   -- the data to be hashed.
// OUTPUT  : digest -- the digest. (20-byte)
// RETURN  : apiOK
//           apiFailed
// NOTE    : This function is called by EMV L2 kernel only.
// ---------------------------------------------------------------------------
ULONG	api_sys_SHA1( ULONG length, UCHAR *data, UCHAR *digest )
{
UCHAR retval;
UCHAR args[length+4+20];
	memmove(args,(UCHAR *)&length,4);
	memmove(&args[4],data,length);
	IPC_clientHandler(psDEV_SYS,4,3,4+length,args,&retval);
	memmove(digest,&args[4+length],20);
	return( retval );

}

// ---------------------------------------------------------------------------
// FUNCTION: To generate the digest (fixed 16 bytes) by using MD5 in one step.
// INPUT   : length -- length of data to be hashed.
//           data   -- the data to be hashed.
// OUTPUT  : digest -- the digest. (16-byte)
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
ULONG	api_sys_MD5( ULONG length, UCHAR *data, UCHAR *digest )
{
UCHAR retval;
UCHAR args[length+4+16];
	memmove(args,(UCHAR *)&length,4);
	memmove(&args[4],data,length);
	IPC_clientHandler(psDEV_SYS,5,3,4+length,args,&retval);
	memmove(digest,&args[4+length],16);
	return( retval );

}

// ---------------------------------------------------------------------------
// FUNCTION: To generate the digest by using SHA-2 in one step.
// INPUT   : length -- length of data to be hashed.
//           data   -- the data to be hashed.
//	     mode   -- SHA2_224, SHA2_256, SHA2_384, SHA2_512.
// OUTPUT  : digest -- the digest.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
ULONG	api_sys_SHA2( UCHAR mode, ULONG length, UCHAR *data, UCHAR *digest )
{
UCHAR retval;
UCHAR args[length+4+1+64];//64 reserve for 512 bit
    args[0]=mode;
	memmove(&args[1],(UCHAR *)&length,4);
	memmove(&args[5],data,length);
	IPC_clientHandler(psDEV_SYS,6,4,1+4+length,args,&retval);
    switch( mode )
	      {
	      case HASH_SHA2_224:
	      	   
	      	   memmove(digest,&args[1+4+length],28);
	           break;
	           
	      case HASH_SHA2_256:
	      	   
	      	   memmove(digest,&args[1+4+length],32);
	      	   break;
	      	   
	      case HASH_SHA2_384:
	      	
	      	   memmove(digest,&args[1+4+length],48);
	      	   break;
	      	   
	      case HASH_SHA2_512:
	      	
	      	   memmove(digest,&args[1+4+length],64);
	      	   break;
	      }
	
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To control DEVICE backlight.
// INPUT   : device   - RFU. (LCD or keypad...)
//                     0 = LCD
//                     1 = KBD
//                     0x80 = LCD backlight level
//           duration - duration of turning on back-light of LCD in 10ms.
//                     if device = 0 or 1
//                             0x00000000 = turn off right away.
//                             0xFFFFFFFF = turn on forever.
//                     if device = 0x80
//                             duration is used for the backlight level.
//                             level: 0..7 (0=turn off, 7=highest)
// OUTPUT  : none.
// RETURN  : apiOK
//          apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_sys_backlight( UCHAR device, ULONG duration )
{
UCHAR retval;
UCHAR args[5];
	args[0]=device;
	memmove(&args[1],&duration,4);
	IPC_clientHandler(psDEV_SYS,7,2,5,args,&retval);
	return( retval );
}


// ---------------------------------------------------------------------------
// FUNCTION: To read system related information.
// INPUT   : id   - system information id.
//		    refer to SID_xxx defined in POSAPI.h.
// OUTPUT  : info - information read. [L(1)-V(n)], min. 17 bytes storage.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
ULONG	api_sys_info( UCHAR id, UCHAR *info )
{
UCHAR retval;
UCHAR args[50];
	args[0]=id;
	IPC_clientHandler(psDEV_SYS,8,2,1,args,&retval);
    memmove(info,&args[1],args[1]+1);
	return( retval );
}

void	api_sys_initialize(void)
{
	IPC_client_Sys_init();//from IPC_client.c
}