#include "POSAPI.h"
#include "OS_PROCS.h"
#include "IPC_client.h"
UCHAR api_tsc_close(UCHAR dhn)
{	
UCHAR retval;
	IPC_clientHandler(psDEV_MEM,1,1,1,&dhn,&retval);
	return( retval );
}


