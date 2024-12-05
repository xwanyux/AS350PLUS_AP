//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : 	                                                    **
//**  PRODUCT  : AS350	                                                    **
//**                                                                        **
//**  FILE     : TD2_buf_overflow.c					    **
//**  MODULE   : DTR_B_buf_overflow_test()	                            **
//**  VERSION  : V1.00                                                      **
//**  DATE     : 2024/09/03                                                 **
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


UCHAR	const	msg_BUF_OVERFLOW_TEST[]		= {"BUF OVERFLOW TEST"};
UCHAR	const	msg_1_RUN[]			= {"1-RUN"};
UCHAR	const	msg_X_RETURN[]			= {"X-RETURN"};


// ---------------------------------------------------------------------------
// FUNCTION: TD2.9 -- buffer overflow test.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
// ---------------------------------------------------------------------------
void	TD2_buf_overflow_test( void )
{
UCHAR	destination[5];
UCHAR	const *source = "NEEDS TO BE MUCH LARGER THAN BEFORE";	// sizeof "source" is larger than "destination"
UCHAR	keypress;


START:
	UI_ClearScreen();
	UI_PutStr( 0, 0, FONT0, strlen(msg_BUF_OVERFLOW_TEST), msg_BUF_OVERFLOW_TEST );
	
	UI_PutStr( 2, 0, FONT0, strlen(msg_1_RUN), msg_1_RUN );		// 1
	UI_PutStr( 3, 0, FONT0, strlen(msg_X_RETURN), msg_X_RETURN );	// CANCEL
	
	do
	 {
	 keypress = UI_WaitKey();
	 } while( (keypress != '1') && (keypress != 'x') );
	 
	if( keypress == 'x' )
	  return;	// to trigger STACK OVERFLOW!
	  
	strcpy( destination, source );	// buffer overflow happens!
	
	// just want to see what source is pointed compared with before and after the 1st buffer overflow
	TL_DumpHexData( 1, 2, strlen(destination), destination );
	
	goto START;
}
