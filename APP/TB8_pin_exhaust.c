//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : 	                                                    **
//**  PRODUCT  : AS350	                                                    **
//**                                                                        **
//**  FILE     : TB8_pin_exhaust.c					    **
//**  MODULE   : TB8_pin_exhaust_test()	           		            **
//**  VERSION  : V1.00                                                      **
//**  DATE     : 2024/09/10                                                 **
//**  EDITOR   : James Hsieh                                                **
//**                                                                        **
//**  Copyright(C) 2024 SymLink Corporation. All rights reserved.      	    **
//**                                                                        **
//****************************************************************************
//============================================================================
//
//----------------------------------------------------------------------------
#include <string.h>
#include "POSAPI.H"
#include "OS_LIB.h"


// ---------------------------------------------------------------------------
// FUNCTION: TB8.5 -- exhaustive PIN entry test.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
// ---------------------------------------------------------------------------
void	TB8_pin_exhaust_test()
{
UCHAR	result;
UCHAR	amt[]	= {0x04, '1', '.', '0', '0' };


	UI_ClearScreen();
	result = PP_GetPIN( 30, amt );
	if( result == apiOK )
	  UI_PutMsg( 3, COL_MIDWAY, FONT0, sizeof("PIN OK")-1, (UCHAR *)"PIN OK" );
	else
	  UI_PutMsg( 3, COL_MIDWAY, FONT0, sizeof("FAILED")-1, (UCHAR *)"FAILED" );
	UI_WaitKey();
}

