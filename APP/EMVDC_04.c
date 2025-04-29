//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : EMV L2                                                     **
//**  PRODUCT  : AS330	                                                    **
//**                                                                        **
//**  FILE     : EMVDC_04.C                                                 **
//**  MODULE   : EMVDC_read_application_data()                              **
//**  VERSION  : V1.00                                                      **
//**  DATE     : 2018/08/08                                                 **
//**  EDITOR   : James Hsieh                                                **
//**                                                                        **
//**  Copyright(C) 2018 SymLink Corporation. All rights reserved.	    **
//**                                                                        **
//****************************************************************************
//============================================================================
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
#include "POSAPI.h"

#include "UI.h"


// ---------------------------------------------------------------------------
// FUNCTION: Terminal shall read the files and records indicated in the AFL.
// INPUT   : none.
// OUTPUT  : none.
// REF     : g_ibuf
// RETURN  : emvOK
//           emvFailed
// ---------------------------------------------------------------------------
UCHAR EMVDC_read_application_data( void )
{

      // show "PLEASE WAIT"
      UI_ClearScreen();
      DISP_STD_please_wait();

      return( api_emv_ReadApplicationData() );
}

