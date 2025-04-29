//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : 							                                **
//**  PRODUCT  : AS350 PLUS						                            **
//**                                                                        **
//**  FILE     : API_XPED.C                                                 **
//**  MODULE   : api_xped_xxx()				                                **
//**									                                    **
//**  FUNCTION : API::XPED (External Pinpad Entry Device)		            **
//**  VERSION  : V1.00                                                      **
//**  DATE     : 2025/01/02                                                 **
//**  EDITOR   : James Hsieh                                                **
//**                                                                        **
//**  Copyright(C) 2025 SymLink Corporation. All rights reserved.	        **
//**                                                                        **
//****************************************************************************
//============================================================================
//
//----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>

#include "POSAPI.h"
#include "OS_PROCS.h"
#include "IPC_client.h"

//typedef	struct API_GENEPB_S
//{
//	UCHAR			Mode;		// algorithm and padding method
//	UCHAR			Index;		// key slot index (not used for DUKPT)
//	UCHAR			Algo;		// algorithm
//} __attribute__((packed)) API_GENEPB;


// ---------------------------------------------------------------------------
// FUNCTION: To request PIN entry from external PIN pad device.
// INPUT   : none.
// OUTPUT  : amt   - amount to be confirmed on display. (external)
//                   format: LEN(1) + ASCII(n)
//                           LEN = 0: no confirmation.
//	     tout  - PIN entry timeout in seconds.
// RETURN  : apiOK         (key-in digit available)
//           apiFailed     (timeout or invalid request)
//	     apiNotReady   (bypass PIN)
// ---------------------------------------------------------------------------
UCHAR	api_xped_GetPin( UINT tout, UCHAR *amt )
{
#if	0
	asm("	STMFD	SP!, {R0-R12}		");	// PUSH all
	API_InitVectorTable();
	asm("	LDMFD	SP!, {R0-R12}		");	// POP all

	asm("	STMFD	SP!, {R0}		");	// PUSH R0
	asm("	LDR	R0, =dev_XPED*0x100+0	");	// dev_id(H8) + sub_id(L8)
	
	asm("	B	XPED_GETPIN		");
	asm("	.ltorg				");
	asm("XPED_GETPIN:			");
	
	asm("	ADD	LR, PC, #0		");	// set return address of this function
	asm("	MOVS	PC, #8			");	// goto OS API entry
#else

UCHAR	retval;
UINT	iptlen=(*amt)+1;
UCHAR	sendbuff[2+iptlen];

	
	sendbuff[0]=tout & 0x00ff;
	sendbuff[1]=(tout & 0xff00) >> 8;
	memmove(&sendbuff[2],amt,iptlen);
	IPC_clientHandler( psDEV_PED, 15, 2, iptlen+2, sendbuff, &retval );
	return( retval );
#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: To request PIN entry from external PIN pad device.
// INPUT   : none.
// OUTPUT  : amt   - amount to be confirmed on display. (external)
//                   format: LEN(1) + ASCII(n)
//                           LEN = 0: no confirmation.
//	     tout  - PIN entry timeout in seconds.
// RETURN  : apiOK         (key-in digit available)
//           apiFailed     (timeout or invalid request)
//	     apiNotReady   (bypass PIN)
// ---------------------------------------------------------------------------
UCHAR	api_xped_GetPin2( UINT tout, UCHAR *amt )
{
#if	0
	asm("	STMFD	SP!, {R0-R12}		");	// PUSH all
	API_InitVectorTable();
	asm("	LDMFD	SP!, {R0-R12}		");	// POP all

	asm("	STMFD	SP!, {R0}		");	// PUSH R0
	asm("	LDR	R0, =dev_XPED*0x100+4	");	// dev_id(H8) + sub_id(L8)
	
	asm("	B	XPED_GETPIN2		");
	asm("	.ltorg				");
	asm("XPED_GETPIN2:			");
	
	asm("	ADD	LR, PC, #0		");	// set return address of this function
	asm("	MOVS	PC, #8			");	// goto OS API entry
#else

UCHAR	retval;
UINT	iptlen=(*amt)+1;
UCHAR	sendbuff[2+iptlen];

	
	sendbuff[0]=tout & 0x00ff;
	sendbuff[1]=(tout & 0xff00) >> 8;
	memmove(&sendbuff[2],amt,iptlen);
	IPC_clientHandler( psDEV_PED, 16, 2, iptlen+2, sendbuff, &retval );
	return( retval );
#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: To generate online encrypted PIN block from external PIN pad device.
// INPUT   : algo   - key algorithm.
// 	     mode   - pin block format.
// 	     index  - key index.
//	     pan    - PAN. (format: L-V)
// OUTPUT  : epb    - encrypted pin block (fixed  8 bytes).
//	     ksn    - key sequence number (fixed 10 bytes).
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_xped_GenEncryptedPinBlock( UCHAR algo, UCHAR mode, UCHAR index, UCHAR *pan, UCHAR *epb, UCHAR *ksn )
{
#if	0
API_GENEPB sbuf;


	sbuf.Mode	= mode;
	sbuf.Index	= index;
	sbuf.Algo	= algo;
	
	return( HAL_xped_GenEncryptedPinBlock( (UCHAR *)&sbuf, pan, epb, ksn ) );
#else

UCHAR	retval;
UINT	iptlen=3+1+(*pan);
UCHAR	sendbuff[iptlen+18];

	
	sendbuff[0]=algo;
	sendbuff[1]=mode;
	sendbuff[2]=index;
	memmove(&sendbuff[3],pan,iptlen-3);
	IPC_clientHandler( psDEV_PED, 17, 4, iptlen, sendbuff, &retval );
	return( retval );
#endif
}

// ------------------------------------------------------------
// FUNCTION: To generate PIN block by using Master/Session.
//	     This function is used to simulate non-PCI spec to generate EMV online PIN block
//	     according to NCCC/CTCB spec.
//
// NOTE    : non-PCI application  : the PIN data is encrypted by IKEK and saved in SRAM key slot. (NCCC/CTCB)
//	     PCI & EMV application: the PIN data is saved in SECM key slot by plaintext format.
//
// INPUT   : mode  - algorithm of PIN block. (default 0)
//           index - session key index used to encrypt the PIN block.
//                   (default 0, using TPK)
//           pan   - full PAN.digits. (format: LEN(1) + ASCII(n))
// OUTPUT  : epb   - encrypted pin block (fixed 8 bytes).
// RETURN  : apiOK
//           apiFailed
// ------------------------------------------------------------
UCHAR	api_xped_MSKEY_GenPinBlock( UCHAR mode, UCHAR index, UCHAR *pan, UCHAR *epb )
{
#if	0
	asm("	STMFD	SP!, {R0-R12}		");	// PUSH all
	API_InitVectorTable();
	asm("	LDMFD	SP!, {R0-R12}		");	// POP all

	asm("	STMFD	SP!, {R0}		");	// PUSH R0
	asm("	LDR	R0, =dev_XPED*0x100+2	");	// dev_id(H8) + sub_id(L8)
	
	asm("	B	XPED_GENPINBLOCK_MSKEY	");
	asm("	.ltorg				");
	asm("XPED_GENPINBLOCK_MSKEY:		");
	
	asm("	ADD	LR, PC, #0		");	// set return address of this function
	asm("	MOVS	PC, #8			");	// goto OS API entry
#else

UCHAR	retval;
UINT	iptlen=2+1+(*pan);
UCHAR	sendbuff[iptlen+8];

	
	sendbuff[0]=mode;
	sendbuff[1]=index;
	memmove(&sendbuff[2],pan,iptlen-2);
	IPC_clientHandler( psDEV_PED, 18, 3, iptlen, sendbuff, &retval );
	return( retval );
#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: To show PIN entry message with ID number. (English & Chinese)
// INPUT   : id. (0..n)
//		0 = "LAST PIN TRY"
//		1 = "PIN ERROR" (INCORRECT PIN)
//		2 = "PIN_TRY_LIMIT_EXCEEDED"
// OUTPUT  : none.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_xped_show_MsgID( UCHAR id )
{
#if	0
	asm("	STMFD	SP!, {R0-R12}		");	// PUSH all
	API_InitVectorTable();
	asm("	LDMFD	SP!, {R0-R12}		");	// POP all

	asm("	STMFD	SP!, {R0}		");	// PUSH R0
	asm("	LDR	R0, =dev_XPED*0x100+3	");	// dev_id(H8) + sub_id(L8)
	
	asm("	B	XPED_SHOW_MSGID		");
	asm("	.ltorg				");
	asm("XPED_SHOW_MSGID:			");
	
	asm("	ADD	LR, PC, #0		");	// set return address of this function
	asm("	MOVS	PC, #8			");	// goto OS API entry
#else

UCHAR	retval;


	IPC_clientHandler( psDEV_PED, 19, 1, 1, (UCHAR *)&id, &retval );
	return( retval );
#endif
}
