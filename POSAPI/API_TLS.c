//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : i.MX6UL				                                    **
//**  PRODUCT  : AS350 PLUS					                                **
//**                                                                        **
//**  FILE     : API_TLS.C                                                  **
//**  MODULE   :                                                            **
//**									                                    **
//**  FUNCTION : API::TLS (SSL/TLS Module)			                        **
//**  VERSION  : V1.00                                                      **
//**  DATE     : 2023/11/14                                                 **
//**  EDITOR   : Tammy Tsai                                                 **
//**                                                                        **
//**  Copyright(C) 2023 SymLink Corporation. All rights reserved.           **
//**                                                                        **
//****************************************************************************
//============================================================================
//
//----------------------------------------------------------------------------
#include <stdio.h>
#include "POSAPI.h"
#include "OS_PROCS.h"
#include "IPC_client.h"


// ---------------------------------------------------------------------------
// FUNCTION: To copy certificate and key from secure memory to FLASH memory.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : apiOK
//	         apiFailed
// ---------------------------------------------------------------------------
UCHAR   api_tls_cpkey(void)
{
    UCHAR   retval;
    UCHAR   sbuf;


	IPC_clientHandler(psDEV_TLS, 1, 0, 0, &sbuf, &retval);
	return retval;
}

// ---------------------------------------------------------------------------
// FUNCTION: To remove certificate and key.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : apiOK
//	         apiFailed
// ---------------------------------------------------------------------------
UCHAR   api_tls_rmkey(void)
{
    UCHAR   retval;
    UCHAR   sbuf;


	IPC_clientHandler(psDEV_TLS, 2, 0, 0, &sbuf, &retval);
	return retval;
}
