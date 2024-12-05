//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : EMV L2                                                     **
//**  PRODUCT  : AS330	                                                    **
//**                                                                        **
//**  FILE     : EMVDC_05.C                                                 **
//**  MODULE   : EMVDC_offline_data_authen()                                **
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

#include "EMVAPI.h"


// ---------------------------------------------------------------------------
// FUNCTION: If both the following are true, the terminal shall perform DDA.
//           (1) The AIP indicates that the card supports offline DDA.
//           (2) The terminal supports offline DDA.
//           If all of the following are true, the terminal shall perform SDA.
//           (1) The AIP indicates that the card supports offline SDA.
//           (2) The terminal supports offline SDA.
//           (3) Either the card or the terminal (or both) does not support DDA.
//           If both DDA & SDA are supported, the terminal performs only DDA.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : emvOK
//           emvFailed
//           emvOutOfService - terminate the transaction.
// ---------------------------------------------------------------------------
UCHAR EMVDC_offline_data_authen( void )
{
      return( api_emv_OfflineDataAuthen() );
}

