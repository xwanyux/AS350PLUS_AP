//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : IMX6UL                                                     **
//**  PRODUCT  : AS350                                                      **
//**                                                                        **
//**  FILE     : API_DSS2.C                                                 **
//**  MODULE   : api_dss2_init()					                        **
//**		     api_dss2_file()			                                **
//**		     api_dss2_burn()					                        **
//**		     api_dss2_apid()					                        **
//**		     api_dss2_run()						                        **
//**									                                    **
//**  FUNCTION : API::DSS (Download Support System)			                **
//**  VERSION  : V1.00                                                      **
//**  DATE     : 2025/01/13                                                 **
//**  EDITOR   : James Hsieh                                                **
//**                                                                        **
//**  Copyright(C) 2025 SymLink Corporation. All rights reserved.           **
//**                                                                        **
//****************************************************************************
//============================================================================
//
//----------------------------------------------------------------------------
#include "POSAPI.h"


// ---------------------------------------------------------------------------
// FUNCTION: To initialize DSS.
// INPUT   : mode - 0: single APP platform. (default)
//		            1: double APP platform.
// OUTPUT  : none.
// RETURN  : apiOK
//	         apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_dss2_init( UCHAR mode )
{
    return apiOK;
}

// ---------------------------------------------------------------------------
// FUNCTION: To store the code data to the DSS working space.
// INPUT   : offset - the beginning offset address to the program file for the code data.
//	         length - size of the current code data in bytes.
//	         data   - code data of the program file.
// OUTPUT  : none.
// RETURN  : apiOK
//	         apiFailed
// NOTE    : for ZA9L0 platform, the program file shall be: "FileName_sig.bin".
// ---------------------------------------------------------------------------
UCHAR	api_dss2_file( ULONG offset, ULONG length, UCHAR *data )
{
    return apiOK;
}

// ---------------------------------------------------------------------------
// FUNCTION: To get application ID of the current active program.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : program ID, 0 = the 1'st application. (default)
//			 1 = the 2'nd application. (only effective for the double APP)
// ---------------------------------------------------------------------------
UCHAR	api_dss2_apid( void )
{
    return 0;
}

// ---------------------------------------------------------------------------
// FUNCTION: To burn the specified program code from SDRAM to FLASH memory.
// INPUT   : apid - applicaion id (0 or 1).
// OUTPUT  : none.
// RETURN  : apiOK
//	         apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_dss2_burn( UCHAR apid )
{
    return apiOK;
}

// ---------------------------------------------------------------------------
// FUNCTION: To execute the specified application program, it will reboot after the function called.
// INPUT   : apid - application id (0 or 1).
// OUTPUT  : none.
// RETURN  : apiFailed - if condition is not satisfied.
//	     otherwise the specified AP will be activated automatically.
// ---------------------------------------------------------------------------
UCHAR	api_dss2_run( UCHAR apid )
{
    return apiOK;
}
