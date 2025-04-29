//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : EMV L2                                                     **
//**  PRODUCT  : AS330	                                                    **
//**                                                                        **
//**  FILE     : EMVDC_06.C                                                 **
//**  MODULE   : EMVDC_processing_restrictions()                            **
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


// ---------------------------------------------------------------------------
// FUNCTION: to determine the degree of compatibility of the application in
//           the terminal with the application in the ICC and to make any
//           necessary adjustments, including possible rejection of the
//           transaction.
//           (1) AVN: Application Version Number
//           (2) AUC: Application Usage Control
//           (3) AED: Application Effective/Expiration Dates Checking
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : emvOK
//           emvFailed
// ---------------------------------------------------------------------------
UCHAR EMVDC_processing_restrictions( void )
{

      return( api_emv_ProcessingRestrictions() );
}

