#include "OS_PROCS.h"
#include "IPC_client.h"
#include "POSAPI.h"
#include "ECL_LV1_Define.h"
#include "ECL_LV1_Util.h"
#include "NXP_Define.h"
#include "SPI_Function.h"
#include "OS_Function.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define serial 0


 

//		Register Parameter
//		TxAmp(0x29)
UCHAR 	nxp_TxAmp_A=0;	//Type A
UCHAR 	nxp_TxAmp_B=0;	//Type B
UCHAR 	nxp_TxAmp_C=0;	//TxAmp for Carrier On

//		RxAna(0x39)
UCHAR 	nxp_RxAna_A=0;	//Type A
UCHAR 	nxp_RxAna_B=0;	//Type B

ULONG	nxp_tmrValue=0;	//Timer for Long INT
ULONG	nxp_tmrSELECT=0;//Timer for SELECT Command
ULONG	nxp_tmrException=0;

UCHAR	nxp_noiParameter[NXP_NOISE_PARAMETER_BUFFER]={0};	//Noise Parameter
UCHAR	nxp_flgNoise=FALSE;			//Noise Flag

UCHAR	nxp_dbg_flgNoise=FALSE;
UCHAR	nxp_dbg_flgSecond=FALSE;
UCHAR	nxp_dbg_flgCase=FALSE;

//Test Load Modulation
extern UCHAR	emv_flgLodModulation;





void NXP_Load_RC663_Parameter_AS350(void)
{
UCHAR retval;
UCHAR args[2];
	IPC_clientHandler(psDEV_NXP,1,0,0,args,&retval);
}


void NXP_Load_RC663_Parameter_TA(void)
{
UCHAR retval;
UCHAR args[2];
	IPC_clientHandler(psDEV_NXP,2,0,0,args,&retval);
}

UCHAR NXP_Read_Register_Serial(UCHAR *iptAddress, UINT iptLen, UCHAR *optData)
{//argument order:iptLen(2B) *iptAddress(iptLen B) *optData(iptLen B)
UCHAR retval;
UCHAR args[2+iptLen+iptLen];
struct timeval nowtime;
	
	memmove(args,&iptLen,2);
	memmove(&args[2],iptAddress,iptLen);
	
	IPC_clientHandler(psDEV_NXP,3,3,2+iptLen,args,&retval);
	memmove(optData,&args[2+iptLen],iptLen);
	return( retval ); 
}


void NXP_Read_Register(UCHAR regAddress, UCHAR *regData)
{
	NXP_Read_Register_Serial(&regAddress, 1, regData);
}


UCHAR NXP_Read_FIFO(UINT iptLen, UCHAR *optData)
{//argument order:iptLen(2B) *optData(iptLen B)
UCHAR retval;
UCHAR args[2+iptLen];
	memmove(args,&iptLen,2);
	IPC_clientHandler(psDEV_NXP,4,2,2,args,&retval);
	memmove(optData,&args[2],iptLen);
	return( retval );  
}


UCHAR NXP_Write_Register_Serial(UCHAR iptAddress, UINT iptLen, UCHAR *iptData)
{//argument order:iptLen(2B) iptAddress(1B) *iptData(iptLen B)	
UCHAR retval;
UCHAR args[2+1+iptLen];
	memmove(args,&iptLen,2);
	args[2]=iptAddress;
	memmove(&args[3],iptData,iptLen);
	IPC_clientHandler(psDEV_NXP,5,3,2+1+iptLen,args,&retval);
	return( retval );   
}


void NXP_Write_Register(UCHAR regAddress, UCHAR regData)
{
	NXP_Write_Register_Serial(regAddress, 1, &regData);
}


void NXP_Check_Noise_Parameters(void)
{
UCHAR retval;
UCHAR args[2];
	IPC_clientHandler(psDEV_NXP,6,0,0,args,&retval); 
}

void NXP_Switch_Receiving_Serial(void)
{
UCHAR retval;
UCHAR args[2];
	IPC_clientHandler(psDEV_NXP,7,0,0,args,&retval);  
}

void NXP_Switch_Receiving_Serial2(void)
{
UCHAR retval;
UCHAR args[2];
	IPC_clientHandler(psDEV_NXP,8,0,0,args,&retval);  
}

UCHAR NXP_Check_Register(UCHAR regAddress, UCHAR regBit)
{
	UCHAR regData=0xFF;

	NXP_Read_Register(regAddress, &regData);

	if (regData & regBit)
		return ECL_LV1_SUCCESS;

	return ECL_LV1_FAIL;
}


UCHAR NXP_Check_SPI_RC663(void)
{
	UCHAR cntRetry=0;
	UCHAR regData=0;
	
	do
	{
		NXP_Read_Register(0x3B, &regData);	//Serial Speed

	
		if (regData == 0x7A) return ECL_LV1_SUCCESS;

		ECL_LV1_UTI_WaitTime(50);
	} while (cntRetry++ < 3);

	return ECL_LV1_FAIL;
}


void NXP_Clear_IRQFlag(void)
{

	// NXP_Write_Register(0x06, 0x7F);
	// NXP_Write_Register(0x07, 0x7F);



	UCHAR cmdIRQ[2]={0x7F,0x7F};

	NXP_Write_Register_Serial(0x06, 2, cmdIRQ);

}


UCHAR NXP_Check_ACK(UCHAR iptData)
{
	UCHAR regData=0;

	NXP_Read_Register(0x0C, &regData);	//RxBitCtrl

	if (((regData & 0x07) == 0x04) && (iptData == 0x0A))
	{
		return ECL_LV1_SUCCESS;
	}
	
	return ECL_LV1_FAIL;
}


void NXP_Get_FIFO_Length(UINT *datLen)
{
/*	UCHAR	regData=0;
	UINT	regLen=0;
	
	NXP_Read_Register(0x02, &regData);	//FIFO Control(FIFO Length bit9~8)
	regLen=(regData & 0x03) << 8;
	
	NXP_Read_Register(0x04, &regData);	//FIFO Length(FIFO Length bit7~0)
	regLen+=regData;
*/
	UCHAR	addRegister[2]={0x02,0x04};
	UCHAR	regData[2];

	NXP_Read_Register_Serial(addRegister, 2, regData);

	datLen[0]=(regData[0] & 0x03)*256+regData[1];
}


void NXP_Get_FIFO_Data(UINT datLen, UCHAR *datBuffer)
{
/*	UINT	cntIdx=0;
	UCHAR	regData=0;

	for (cntIdx=0; cntIdx < datLen; cntIdx++)
	{
		NXP_Read_Register(0x05, &regData);
		datBuffer[cntIdx]=regData;
	}
*/
	NXP_Read_FIFO(datLen, datBuffer);
}


void NXP_Get_FIFO(UINT *datLen, UCHAR *datBuffer)
{
	NXP_Get_FIFO_Length(datLen);

	NXP_Get_FIFO_Data(datLen[0], datBuffer);
}


void NXP_Set_Timer(ULONG tmrValue)
{
UCHAR retval;
UCHAR args[4];
	IPC_clientHandler(psDEV_NXP,10,1,4,(UCHAR*)&tmrValue,&retval);
}


void NXP_Set_IRQ(UCHAR irq0, UCHAR irq1)
{

	// NXP_Write_Register(0x08, irq0);
	// NXP_Write_Register(0x09, irq1);

	UCHAR irqData[2];

	irqData[0]=irq0;
	irqData[1]=irq1;
	NXP_Write_Register_Serial(0x08, 2, irqData); 

}


UCHAR NXP_Wait_GlobelIRQ(void)
{
	UCHAR rspCode=0;
	ULONG tmrStart;
	ULONG tmrTick;
	  		 
	tmrStart=OS_GET_SysTimerFreeCnt();
	
	do
	{
		rspCode=NXP_Check_Register(0x07, NXP_IRQ1_GLOBAL);
		if (rspCode == ECL_LV1_SUCCESS)
			return ECL_LV1_SUCCESS;

		tmrTick=OS_GET_SysTimerFreeCnt();
	} while ((tmrTick-tmrStart) < NXP_TIMER_GLOBAL_IRQ);

	return ECL_LV1_FAIL;
}


UCHAR NXP_Get_CLChipSerialNumber(UCHAR * optData)
{
UCHAR retval;
UCHAR args[11];
	IPC_clientHandler(psDEV_NXP,9,1,0,args,&retval);
	return( retval );    
}


void NXP_Switch_FieldOn(void)
{
	NXP_Write_Register(0x28, 0x89);	//DrvMode: Tx2Inv, TxEn
}


void NXP_Switch_FieldOff(void)
{
	NXP_Write_Register(0x28, 0x80);	//DrvMode: Tx2Inv
}


void NXP_Switch_Receiving(void)
{
UCHAR retval;
UCHAR args[2];
	IPC_clientHandler(psDEV_NXP,11,0,0,args,&retval);  
}


void NXP_Reset_NoiseParameter(void)
{
UCHAR retval;
UCHAR args[2];
	IPC_clientHandler(psDEV_NXP,12,0,0,args,&retval); 
}

UCHAR NXP_Receive_ExceptionProcessing(void)
{
UCHAR retval;
UCHAR args[2];
	IPC_clientHandler(psDEV_NXP,13,0,0,args,&retval); 
	return retval;
}

 

UCHAR NXP_Receive_ExceptionProcessing2(void)
{
UCHAR retval;
UCHAR args[2];
	IPC_clientHandler(psDEV_NXP,14,0,0,args,&retval); 
	return retval; 
}

UCHAR NXP_Receive(void)
{
UCHAR retval;
UCHAR args[2];
	IPC_clientHandler(psDEV_NXP,15,0,0,args,&retval); 
	return retval; 
}


void NXP_Initialize_Reader(void)
{	
UCHAR retval;
UCHAR args[2];
	IPC_clientHandler(psDEV_NXP,16,0,0,args,&retval); 
}


void NXP_Load_Protocol_A(void)
{
UCHAR retval;
UCHAR args[2];
	IPC_clientHandler(psDEV_NXP,17,0,0,args,&retval);  
}


void NXP_Load_Protocol_B(void)
{
UCHAR retval;
UCHAR args[2];
	IPC_clientHandler(psDEV_NXP,18,0,0,args,&retval);  
}


void NXP_REQA_Send(ULONG rcvTimeout)
{
UCHAR retval;
UCHAR args[2];
	IPC_clientHandler(psDEV_NXP,19,1,4,(UCHAR*)&rcvTimeout,&retval);   
}


UCHAR NXP_REQA_Receive(UINT *rcvLen, UCHAR *rcvATQA)
{
UCHAR retval;
UCHAR args[2+500];//not sure output size, 500 byte should be enough.
	IPC_clientHandler(psDEV_NXP,20,2,0,args,&retval); 
	memmove(rcvLen,args,2);
	memmove(rcvATQA,&args[2],*rcvLen);
	return retval;
}


void NXP_WUPA_Send(ULONG rcvTimeout)
{
UCHAR retval;
UCHAR args[2];
	IPC_clientHandler(psDEV_NXP,21,1,4,(UCHAR*)&rcvTimeout,&retval);    
}


UCHAR NXP_WUPA_Receive(UINT *rcvLen, UCHAR *rcvATQA)
{
UCHAR retval;
UCHAR args[2+500];//not sure output size, 500 byte should be enough.
	IPC_clientHandler(psDEV_NXP,22,2,0,args,&retval); 
	memmove(rcvLen,args,2);
	memmove(rcvATQA,&args[2],*rcvLen); 
	return retval;
}


void NXP_ANTICOLLISION_Send(UCHAR selCL, ULONG rcvTimeout)
{
UCHAR retval;
UCHAR args[1+4];
	args[0]=selCL;
	memmove(&args[1],(UCHAR*)&rcvTimeout,4);
	IPC_clientHandler(psDEV_NXP,23,2,5,args,&retval); 
	
}


UCHAR NXP_ANTICOLLISION_Receive(UINT *rcvLen, UCHAR *rcvCLn)
{
UCHAR retval;
UCHAR args[2+500];//not sure output size, 500 byte should be enough.
	IPC_clientHandler(psDEV_NXP,24,2,0,args,&retval); 
	memmove(rcvLen,args,2);
	memmove(rcvCLn,&args[2],*rcvLen);  
	return retval;
}


void NXP_SELECT_Send(UCHAR selCL, UCHAR *selUID, UCHAR uidBCC, ULONG rcvTimeout)
{
UCHAR retval;
UCHAR args[1+4+1+4];
	args[0]=selCL;
	memmove(&args[1],selUID,4);
	args[1+4]=uidBCC;
	memmove(&args[1+4+1],(UCHAR*)&rcvTimeout,4);
	IPC_clientHandler(psDEV_NXP,25,4,10,args,&retval); 
}


UCHAR NXP_SELECT_Receive(UINT *rcvLen, UCHAR *rcvSAK)
{
UCHAR retval;
UCHAR args[2+500];//not sure output size, 500 byte should be enough.
	IPC_clientHandler(psDEV_NXP,26,2,0,args,&retval); 
	memmove(rcvLen,args,2);
	memmove(rcvSAK,&args[2],*rcvLen);  
	return retval; 
}


void NXP_RATS_Send(UCHAR ratPARAM, ULONG rcvTimeout)
{
UCHAR retval;
UCHAR args[1+4];
	args[0]=ratPARAM;
	memmove(&args[1],(UCHAR*)&rcvTimeout,4);
	IPC_clientHandler(psDEV_NXP,27,2,5,args,&retval);  
}


UCHAR NXP_RATS_Receive(UINT *rcvLen, UCHAR *rcvATS)
{
UCHAR retval;
UCHAR args[2+500];//not sure output size, 500 byte should be enough.
	IPC_clientHandler(psDEV_NXP,28,2,0,args,&retval); 
	memmove(rcvLen,args,2);
	memmove(rcvATS,&args[2],*rcvLen);  
	return retval;  
}


void NXP_PPS_Send(ULONG rcvTimeout)
{
UCHAR retval;
UCHAR args[4];
	IPC_clientHandler(psDEV_NXP,29,1,4,(UCHAR*)&rcvTimeout,&retval);
}


UCHAR NXP_PPS_Receive(void)
{
UCHAR retval;
UCHAR args[2];
	IPC_clientHandler(psDEV_NXP,30,0,0,args,&retval); 
	return retval;  
}


void NXP_HLTA_Send(ULONG rcvTimeout)
{
UCHAR retval;
UCHAR args[4];
	IPC_clientHandler(psDEV_NXP,31,1,4,(UCHAR*)&rcvTimeout,&retval); 
}


void NXP_REQB_Send(ULONG rcvTimeout)
{
UCHAR retval;
UCHAR args[4];
	IPC_clientHandler(psDEV_NXP,32,1,4,(UCHAR*)&rcvTimeout,&retval); 
}


UCHAR NXP_REQB_Receive(UINT *rcvLen, UCHAR *rcvATQB)
{
UCHAR retval;
UCHAR args[2+500];//not sure output size, 500 byte should be enough.
	IPC_clientHandler(psDEV_NXP,33,2,0,args,&retval); 
	memmove(rcvLen,args,2);
	memmove(rcvATQB,&args[2],*rcvLen);  
	return retval;   
}


void NXP_WUPB_Send(ULONG rcvTimeout)
{
UCHAR retval;
UCHAR args[4];
	IPC_clientHandler(psDEV_NXP,34,1,4,(UCHAR*)&rcvTimeout,&retval);  
}


UCHAR NXP_WUPB_Receive(UINT *rcvLen, UCHAR *rcvATQB)
{
UCHAR retval;
UCHAR args[2+500];//not sure output size, 500 byte should be enough.
	IPC_clientHandler(psDEV_NXP,35,2,0,args,&retval); 
	memmove(rcvLen,args,2);
	memmove(rcvATQB,&args[2],*rcvLen);  
	return retval;    
}


void NXP_ATTRIB_Send(UCHAR *cmdATTRIB, ULONG rcvTimeout)
{//argument order:rcvTimeout(4B) *cmdATTRIB(11B)
UCHAR retval;
UCHAR args[11+4];
	memmove(args,(UCHAR*)&rcvTimeout,4);
	IPC_clientHandler(psDEV_NXP,36,2,4,args,&retval); 
	
	memmove(cmdATTRIB,args+4,11);
}


UCHAR NXP_ATTRIB_Receive(UINT *rcvLen, UCHAR *rcvATA)
{
UCHAR retval;
UCHAR args[2+500];//not sure output size, 500 byte should be enough.
	IPC_clientHandler(psDEV_NXP,37,2,0,args,&retval); 
	memmove(rcvLen,args,2);
	memmove(rcvATA,&args[2],*rcvLen);  
	return retval;     
}


void NXP_HLTB_Send(UCHAR * iptPUPI, ULONG rcvTimeout)
{//argument order:rcvTimeout(4B) *iptPUPI(4B)
UCHAR retval;
UCHAR args[4+4];
	memmove(args,(UCHAR*)&rcvTimeout,4);
	memmove(&args[4],iptPUPI,4);
	IPC_clientHandler(psDEV_NXP,38,2,8,args,&retval); 
	         
}


UCHAR NXP_HLTB_Receive(UINT *rcvLen, UCHAR *rcvData)
{
UCHAR retval;
UCHAR args[2+500];//not sure output size, 500 byte should be enough.
	IPC_clientHandler(psDEV_NXP,39,2,0,args,&retval); 
	memmove(rcvLen,args,2);
	memmove(rcvData,&args[2],*rcvLen);  
	return retval;   
}


void NXP_DEP_Send(UCHAR crdType, UINT datLen, UCHAR *datBuffer, ULONG rcvTimeout)
{//argument order:crdType(1B) datLen(2B) rcvTimeout(4B) *datBuffer(datLen B)
UCHAR retval;
UINT sendLen=1+2+4+datLen;
UCHAR args[sendLen];
	args[0]=crdType;
	memmove(args+1,(UCHAR*)&datLen,2);
	memmove(args+3,(UCHAR*)&rcvTimeout,4);
	memmove(args+7,datBuffer,datLen);
	IPC_clientHandler(psDEV_NXP,40,4,sendLen,args,&retval);
   
}


UCHAR NXP_DEP_Receive(UINT *rcvLen, UCHAR *rcvData)
{
UCHAR retval;
UCHAR args[2+500];//not sure output size, 500 byte should be enough.
	IPC_clientHandler(psDEV_NXP,41,2,0,args,&retval); 
	memmove(rcvLen,args,2);
	// printf("!!!!!!!!!!rcvLen=%d\n",*rcvLen);
	memmove(rcvData,&args[2],*rcvLen);  
	return retval;  
}


UCHAR NXP_LOADKEY(UCHAR *iptKey)
{
UCHAR retval;
UCHAR args[2];
	IPC_clientHandler(psDEV_NXP,42,1,6,iptKey,&retval);
	return retval;   
}


UCHAR NXP_AUTHENTICATION(UCHAR iptAutType, UCHAR iptAddress, UCHAR *iptUID)
{
UCHAR retval;
UCHAR args[1+1+4];
	args[0]=iptAutType;
	args[1]=iptAddress;
	memmove(&args[2],iptUID,4);
	IPC_clientHandler(psDEV_NXP,43,3,6,args,&retval);
	return retval;    
}


UCHAR NXP_READ(UCHAR iptAddress, UCHAR *optData)
{
UCHAR retval;
UCHAR args[1+16];
	args[0]=iptAddress;
	IPC_clientHandler(psDEV_NXP,44,2,1,args,&retval);
	memmove(optData,&args[1],16);
	return retval;     
}


UCHAR NXP_WRITE(UCHAR iptAddress, UCHAR *iptData)
{
UCHAR retval;
UCHAR args[1+16];
	args[0]=iptAddress;
	memmove(&args[1],iptData,16);
	IPC_clientHandler(psDEV_NXP,45,2,17,args,&retval);
	return retval;  
}


UCHAR NXP_DECREMENT(UCHAR iptAddress, UCHAR *iptValue)
{
UCHAR retval;
UCHAR iptlen=4;
UCHAR args[1+iptlen];
	args[0]=iptAddress;
	memmove(&args[1],iptValue,4);
	IPC_clientHandler(psDEV_NXP,46,2,sizeof(args),args,&retval);
	return retval;   
}


UCHAR NXP_INCREMENT(UCHAR iptAddress, UCHAR *iptValue)
{
UCHAR retval;
UCHAR iptlen=4;
UCHAR args[1+iptlen];
	args[0]=iptAddress;
	memmove(&args[1],iptValue,4);
	IPC_clientHandler(psDEV_NXP,47,2,sizeof(args),args,&retval);
	return retval; 
}


UCHAR NXP_RESTORE(UCHAR iptAddress)
{
UCHAR retval;
UCHAR args[1];
	IPC_clientHandler(psDEV_NXP,48,1,1,&iptAddress,&retval);
	return retval; 
}


UCHAR NXP_TRANSFER(UCHAR iptAddress)
{
UCHAR retval;
UCHAR args[1];
	IPC_clientHandler(psDEV_NXP,49,1,1,&iptAddress,&retval);
	return retval;  
}


UCHAR NXP_AV2_AUTHENTICATION_1ST(UCHAR iptAutType, UCHAR iptAddress, UCHAR *optData)
{
UCHAR retval;
UCHAR args[1+1+4];
	args[0]=iptAutType;
	args[1]=iptAddress;
	IPC_clientHandler(psDEV_NXP,50,3,sizeof(args),args,&retval);
	memmove(optData,&args[2],4); 
}


UCHAR NXP_AV2_AUTHENTICATION_2ND(UCHAR *iptData, UCHAR *optData)
{//argument order:iptData(9B) *optData(5B)
UCHAR retval;
UCHAR args[9+5];
	memmove(args,iptData,9);
	IPC_clientHandler(psDEV_NXP,51,2,9,args,&retval);
	memmove(optData,&args[9],5);  
}


UCHAR NXP_AV2_TRANSCEIVE(UINT iptLen, UCHAR *iptData, UINT *optLen, UCHAR *optData)
{//argument order:iptLen(2B) *iptData(iptLen B) *optLen() *optData(optLen B)
UCHAR retval;
UCHAR args[2+iptLen+200];//not sure output size, 200 byte should be enough.
	memmove(args,&iptLen,2);
	memmove(&args[2],iptData,iptLen);
	IPC_clientHandler(psDEV_NXP,52,4,2+iptLen,args,&retval); 
	memmove(optLen,&args[2+iptLen],2);
	memmove(optData,&args[2+iptLen+2],*optLen);  
	return retval;  
}
