#include "POSAPI.h"
#include "OS_PROCS.h"
#include "IPC_client.h"
#include "DEV_MEM.h"
/**
 *  this function is use to initilaize the memory system region.
 *  @param[in] info         necessary information of memory system
 *  @return error code
 *  @retval apiOK           create success
 *  @retval apiFailed       create failed
 */ 
//args order: info->size(4B) system_name length(1B) info->system_name(system_name_length B) info->base_address(4B)
UCHAR mem_init(struct dev_mem_info *info){
UCHAR retval;
UCHAR sysnameLen=strlen(info->system_name);
ULONG sendlength=4+1+sysnameLen+4;
UCHAR args[sendlength];
    memmove(args,&info->size,4);
    args[4]=sysnameLen;
    memmove(&args[5],info->system_name,sysnameLen);
    memmove(&args[5+sysnameLen],&info->base_address,4);

	IPC_clientHandler(psDEV_MEM,1,1,sendlength,args,&retval);
	return( retval );
}


/**
 *  this function is used to read or write to memory system by address
 *  @param[in] info         necessary information of memory system
 *  @param[in] mem_add      start read/write address 
 *  @param[in] length       read/write data length
 *  @param[in/out] data     flag = WRITE, as input data
 *                          flag = READ, as output data
 *  @param[in] flag         macro
 *                          @li WRITE
 *                          @li READ
 *  @return error code
 *  @retval apiOK           read/write memory success
 *  @retval apiFailed       read/write memory failed
 */ 
//args order: info->size(4B) system_name length(1B) info->system_name(system_name_length B) info->base_address(4B) mem_add(4B) flag(1B) length(4B) *data(length B)
UCHAR mem_read_or_write(struct dev_mem_info *info, ULONG mem_add, ULONG length, UCHAR *data, UCHAR flag){
UCHAR retval;
UCHAR sysnameLen=strlen(info->system_name);
ULONG sendlength=4+1+sysnameLen+4+4+1+4+length;
UCHAR args[sendlength];
    memmove(args,&info->size,4);
    args[4]=sysnameLen;
    memmove(&args[5],info->system_name,sysnameLen);
    memmove(&args[5+sysnameLen],&info->base_address,4);
    memmove(&args[5+sysnameLen+4],&mem_add,4);
    args[5+sysnameLen+4+4]=flag;
    memmove(&args[5+sysnameLen+4+4+1],&length,4);
    if(flag>0)
        memmove(&args[5+sysnameLen+4+4+1+4],data,length);  
    else
        sendlength-= length;     
	IPC_clientHandler(psDEV_MEM,2,5,sendlength,args,&retval);
    if(flag==READ)
        memmove(data,&args[sendlength],length);
	return( retval );
}

/**
 *  this function is used to clear the memory system by address
 *  @param[in] info         necessary information of memory system
 *  @param[in] mem_add      start read/write address 
 *  @param[in] length       read/write data length
 *  @param[in] Pattern      the clear value
 *  @return error code
 *  @retval apiOK           clear success
 *  @retval apiFailed       clear failed
 */ 
//args order: info->size(4B) system_name length(1B) info->system_name(system_name_length B) info->base_address(4B) mem_add(4B) length(4B) pattern(1B)
UCHAR mem_clear(struct dev_mem_info *info, ULONG mem_add, ULONG length, UCHAR pattern){
UCHAR retval;
UCHAR sysnameLen=strlen(info->system_name);
ULONG sendlength=4+1+sysnameLen+4+4+4+1;
UCHAR args[sendlength];
    memmove(args,&info->size,4);
    args[4]=sysnameLen;
    memmove(&args[5],info->system_name,sysnameLen);
    memmove(&args[5+sysnameLen],&info->base_address,4);
    memmove(&args[5+sysnameLen+4],&mem_add,4);
    memmove(&args[5+sysnameLen+4+4],&length,4);
    args[5+sysnameLen+4+4+4]=pattern;
	IPC_clientHandler(psDEV_MEM,3,4,sendlength,args,&retval);
	return( retval );
}

