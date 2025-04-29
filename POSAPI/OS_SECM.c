#include "POSAPI.h"
#include "OS_PROCS.h"
#include "IPC_client.h"

// ---------------------------------------------------------------------------
// FUNCTION: Verify the boot status by checking "SYS_WARMBOOT_FLAG".
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : TRUE  -- warm boot (flag ok  )
//           FALSE -- cold boot (flag lost)
// ---------------------------------------------------------------------------
UINT32	OS_SECM_VerifyBootStatus( void )
{
UCHAR retval;
UCHAR args;
	IPC_clientHandler(psDEV_SECM,1,0,0,args,&retval);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: Set the boot status to "SYS_WARMBOOT_FLAG".
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : TRUE  -- OK
//           FALSE -- Failed
// ---------------------------------------------------------------------------
UINT32	OS_SECM_SetWarmBootStatus( void )
{
UCHAR retval;
UCHAR args;
	IPC_clientHandler(psDEV_SECM,2,0,0,args,&retval);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: Clear the boot status to NULL. (initial state)
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : TRUE  -- OK
//           FALSE -- Failed
// ---------------------------------------------------------------------------
UINT32	OS_SECM_ResetBootStatus( void )
{
UCHAR retval;
UCHAR args;
	IPC_clientHandler(psDEV_SECM,3,0,0,args,&retval);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: Verify the application status by checking "APP_READY_FLAG".
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : TRUE  -- ready.
//           FALSE -- not ready.
// ---------------------------------------------------------------------------
UINT32	OS_SECM_VerifyAppStatus( void )
{
UCHAR retval;
UCHAR args;
	IPC_clientHandler(psDEV_SECM,4,0,0,args,&retval);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: Set the application status to "APP_READY_FLAG".
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : TRUE  -- OK
//           FALSE -- Failed
// ---------------------------------------------------------------------------
UINT32	OS_SECM_SetAppStatus( void )
{
UCHAR retval;
UCHAR args;
	IPC_clientHandler(psDEV_SECM,5,0,0,args,&retval);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: Clear the application status to NULL. (initial state)
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : TRUE  -- OK
//           FALSE -- Failed
// ---------------------------------------------------------------------------
UINT32	OS_SECM_ResetAppStatus( void )
{
UCHAR retval;
UCHAR args;
	IPC_clientHandler(psDEV_SECM,6,0,0,args,&retval);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: Write data to the specified address.
// INPUT   : address - start address to write.
//	     length  - length of data to write.
//	     data    - data to be written.
// OUTPUT  : none.
// RETURN  : none.
// ---------------------------------------------------------------------------
void	OS_SECM_PutData( UINT32 address, UINT32 length, UINT8 *data )
{
UCHAR retval;
UCHAR args[4+4+length];
	memmove(args,&address,4);
	memmove(&args[4],&length,4);
	memmove(&args[4+4],data,length);
	IPC_clientHandler(psDEV_SECM,7,3,4+4+length,args,&retval);
}

// ---------------------------------------------------------------------------
// FUNCTION: Read data from the specified address.
// INPUT   : address - start address to read.
//	     length  - length of data to read.
// OUTPUT  : data    - storage of data read.
// RETURN  : none.
// ---------------------------------------------------------------------------
void	OS_SECM_GetData( UINT32	address, UINT32 length, UINT8 *data )
{
UCHAR retval;
UCHAR args[4+4+length];
	memmove(args,&address,4);
	memmove(&args[4],&length,4);
	IPC_clientHandler(psDEV_SECM,8,3,4+4,args,&retval);
	memmove(data,&args[4+4],length);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: Clear data to the specified pattern.
// INPUT   : address - start address to write.
//	     length  - length of data to write.
//	     pattern - data to be written.
// OUTPUT  : none.
// RETURN  : none.
// ---------------------------------------------------------------------------
void	OS_SECM_ClearData( UINT32 address, UINT32 length, UINT8 pattern )
{
UCHAR retval;
UCHAR args[4+4+1];
	memmove(args,&address,4);
	memmove(&args[4],&length,4);
	args[8]=pattern;
	IPC_clientHandler(psDEV_SECM,9,3,4+4+1,args,&retval);
}
// ---------------------------------------------------------------------------
// FUNCTION: Verify the SRED status by checking "SYS_ENABLE_FLAG".
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : TRUE  -- ready.
//           FALSE -- not ready.
// ---------------------------------------------------------------------------
UINT32	OS_SECM_VerifySredStatus(void)
{
UCHAR retval;
UCHAR args;
	IPC_clientHandler(psDEV_SECM,10,0,0,args,&retval);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: Set SRED status to enablement or disablement
// INPUT   : flag - 0 = disable SRED
//				    1 = enable SRED
// OUTPUT  : none.
// RETURN  : TRUE  -- OK
//	         FALSE -- Failed
// ---------------------------------------------------------------------------
UINT32	OS_SECM_SetSredStatus(UINT8 flag)
{
UCHAR retval;
UCHAR args;
	args=flag;
	IPC_clientHandler(psDEV_SECM,11,1,1,&args,&retval);
	return( retval );
}