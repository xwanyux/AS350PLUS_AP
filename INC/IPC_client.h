#ifndef _IPC_CLIENT_H_
#define _IPC_CLIENT_H_
#include "bsp_types.h"
//graphic size(320*240*3)+reserve size( 100 )
#define MMAP_DATA_SIZE 2342500
#define CLEIENT_ARGS_SET 0x99
extern void IPC_clientSet(UINT8 PID,UINT8 APInum,UINT8 ArgsNum,UINT32 ArgsSize,UINT8 *Arg);
extern void IPC_clientRead(UINT8 *ReturnValue,UINT8 *ReturnArgs,UINT32 IptSize,UINT32 OptSize);
extern void IPC_clientHandler(UINT8 PID,UINT8 APInum,UINT8 ArgsNum,UINT32 IptSize,UINT8 *Arg,UINT8 *retval);
extern UINT8 IPC_Reciever(UINT8 *ret,UINT32 reclen);
extern UINT8 IPC_Reciever2(UINT8 *ret,UINT32 reclen);
extern UINT8 IPC_Sender(UINT8 PID,UINT8 APInum,UINT8 ArgsNum,UINT32 ArgsSize,UINT8 *Arg);
extern UINT8 IPC_Handler(UINT8 PID,UINT8 APInum,UINT8 ArgsNum,UINT32 ArgsSize,UINT8 *Arg,UINT8 *retval);
extern void IPC_client_Sys_init(void);
#endif