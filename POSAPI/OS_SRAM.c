#include "POSAPI.h"
#include "OS_PROCS.h"
#include "IPC_client.h"

/**
 *  this function is used to write sram by address
 *  @param[in] address          start address
 *  @param[in] length           length of data
 *  @param[in] data             data you want to write
 *  @return error code
 *  @retval apiFailed           write failed
 *  @retval apiOK               write OK
 */ 
UCHAR api_sram_write(UCHAR* address, ULONG length, UCHAR* data ){
UCHAR retval;
ULONG addr=(ULONG)address;
UCHAR args[sizeof(ULONG)+sizeof(ULONG)+length];
	
	memmove(args,&addr,sizeof(ULONG));
	memmove(&args[sizeof(ULONG)],&length,sizeof(ULONG));
	memmove(&args[sizeof(ULONG)+sizeof(ULONG)],data,length);
	IPC_clientHandler(psDEV_OSSRAM,1,3,sizeof(ULONG)+sizeof(ULONG)+length,args,&retval);
	return( retval );
}

/**
 *  this function is used to write sram by address
 *  @param[in] address          start address
 *  @param[in] length           length of data
 *  @param[out] data            data read form sram
 *  @return error code
 *  @retval apiFailed           read failed
 *  @retval apiOK               read OK
 */ 
UCHAR api_sram_read(UCHAR* address, ULONG length, UCHAR* data){
UCHAR retval;
ULONG addr=(ULONG)address;
UCHAR args[sizeof(ULONG)+sizeof(ULONG)+length];
	// printf("addr=0x%04lx\n",addr);
	memmove(args,&addr,sizeof(ULONG));
	memmove(&args[sizeof(ULONG)],&length,sizeof(ULONG));
	IPC_clientHandler(psDEV_OSSRAM,2,3,sizeof(ULONG)+sizeof(ULONG),args,&retval);
	memmove(data,&args[sizeof(ULONG)+sizeof(ULONG)],length);
	return( retval );
}

/**
 *  this function is used to clear sram by address
 *  @param[in] address          start address
 *  @param[in] length           length you want to clear
 *  @param[in] pattern          value you want to set
 *  @return error code
 *  @retval apiFailed           clear failed
 *  @retval apiOK               clear OK
 */
UCHAR api_sram_clear(UCHAR* address, ULONG length, UCHAR pattern ){
UCHAR retval;
ULONG addr=(ULONG)address;
UCHAR args[sizeof(ULONG)+sizeof(ULONG)+1];
	
	memmove(args,&addr,sizeof(ULONG));
	memmove(&args[sizeof(ULONG)],&length,sizeof(ULONG));
    args[sizeof(ULONG)+sizeof(ULONG)]=pattern;
	IPC_clientHandler(psDEV_OSSRAM,3,3,sizeof(ULONG)+sizeof(ULONG)+1,args,&retval);
	return( retval );
}



