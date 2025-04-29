//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : EMV L2                                                     **
//**  PRODUCT  : AS330	                                                    **
//**                                                                        **
//**  FILE     : PINPAD.C                                                   **
//**  MODULE   : PP_GePIN()                                                 **
//**             PP_VerifyPIN()                                             **
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
#include <string.h>

#include "POSAPI.h"
#include "EMVDC.h"
#include "GDATAEX.h"
#include "EMVAPI.h"
//#include "EMVAPK.H"
#include "EMVDCMSG.h"
//#include "APDU.H"
#include "TOOLS.h"
#include "UI.h"
#include "EMVCB.H"

#include "PEDAPI.h"
#include "LCDTFTAPI.h"

extern	void	EMV_CB_SetupPinPad( API_LCDTFT_PARA *para );

// ---------------------------------------------------------------------------
// FUNCTION: display 
//			0123456789012345
//		0	TOTAL	    NNNN
//		1
//		2	ENTER PIN:
//
// INPUT   : amt   - total amount to be confirmed on display. (external)
//                   format: LEN(1) + ASCII(n)
// ---------------------------------------------------------------------------
void	PP_show_enter_pin( UCHAR *amt )
{
const	UCHAR msg_TOTAL[] =	{"TOTAL"};
const	UCHAR msg_ENTER_PIN[] =	{"ENTER PIN:"};


	// UI_ClearScreen();
//	UI_ClearRow_EX( 1, 3, FONT1 );
	
#if	0
	if( amt[0] != 0 )
	  {
	  UI_PutMsg( 0, COL_LEFTMOST, FONT1, sizeof(msg_TOTAL)-1, (UCHAR *)msg_TOTAL );
	  UI_PutMsg( 0, COL_RIGHTMOST, FONT1, amt[0], &amt[1] );
	  }
	
	LIB_LCD_PutMsg( 2, COL_LEFTMOST, FONT1, sizeof(msg_ENTER_PIN)-1, (UCHAR *)msg_ENTER_PIN );
#endif

//#ifdef	_USER_DEFINE_

	EMV_CB_ShowMsg_ENTER_PIN( amt );
//#endif

}

// ---------------------------------------------------------------------------
// FUNCTION: display 
//			0123456789012345
//		0	
//		1	
//		2	 
//		3	 PLS ENTER PIN
//
// ---------------------------------------------------------------------------
void	PP_show_enter_pin_by_ext_pinpad( void )
{
const	UCHAR msg_PLS_ENTER_PIN[] = {"PLS ENTER PIN"};


	// UI_ClearScreen();
	UI_ClearRow_EX( 1, 3, FONT1 );
#if	0
	UI_PutMsg( 3, COL_MIDWAY, FONT1, sizeof(msg_PLS_ENTER_PIN)-1, (UCHAR *)msg_PLS_ENTER_PIN );
#endif

//#ifdef	_USER_DEFINE_

	EMV_CB_ShowMsg_ENTER_PIN_BY_EXT_PINPAD();
//#endif

}

// ---------------------------------------------------------------------------
// FUNCTION: display "INCORRECT PIN"
// ---------------------------------------------------------------------------
void	PP_show_incorrect_pin( void )
{
#if 0
	UI_ClearScreen();
	// UI_PutMsg( 0, COL_MIDWAY, FONT2, 8, (UCHAR *)stdmsg_C_incorrect_pin );
	UI_PutMsg( 1, COL_MIDWAY, FONT1, 13, (UCHAR *)stdmsg_incorrect_pin );
#else
    const	UCHAR msg_INCORRECT_PIN[] = {"INCORRECT PIN"};


	// UI_ClearScreen();
	UI_ClearRow_EX( 1, 3, FONT1 );
#if	0
//	UI_PutMsg( 0, COL_MIDWAY, FONT2, 8, (UCHAR *)stdmsg_C_incorrect_pin );
//	UI_PutMsg( 1, COL_MIDWAY, FONT1, 13, (UCHAR *)stdmsg_incorrect_pin );

//#ifndef	_USER_DEFINE_

	UI_PutMsg( 3, COL_MIDWAY, FONT1, sizeof(msg_INCORRECT_PIN)-1, (UCHAR *)msg_INCORRECT_PIN );
//#endif
#endif

//#ifdef	_USER_DEFINE_

	EMV_CB_ShowMsg_INCORRECT_PIN();
	
	if( g_emv_ped_src == 1 )
	  api_xped_show_MsgID(1);
	  
	TL_WaitTime(200);		// 2013-12-11, wait 2 sec
	
	if( g_emv_ped_src == 1 )
	  {
	  api_xped_show_MsgID(255);	// return to main menu
	  TL_WaitTime(10);
	  }
//#endif
#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: display "LAST PIN TRY"
// ---------------------------------------------------------------------------
void	PP_show_last_pin_try( void )
{
#if 0
	UI_ClearScreen();
	// UI_PutMsg( 0, COL_MIDWAY, FONT2, 12, (UCHAR *)stdmsg_C_last_pin_try );
	UI_PutMsg( 1, COL_MIDWAY, FONT1, 12, (UCHAR *)stdmsg_last_pin_try );
#else
    const	UCHAR msg_LAST_PIN_TRY[] = {"LAST PIN TRY"};


	// UI_ClearScreen();
	UI_ClearRow_EX( 1, 3, FONT1 );
#if	0
//	UI_PutMsg( 0, COL_MIDWAY, FONT2, 12, (UCHAR *)stdmsg_C_last_pin_try );
//	UI_PutMsg( 1, COL_MIDWAY, FONT1, 12, (UCHAR *)stdmsg_last_pin_try );

//#ifndef	_USER_DEFINE_

	UI_PutMsg( 3, COL_MIDWAY, FONT1, sizeof(msg_LAST_PIN_TRY)-1, (UCHAR *)msg_LAST_PIN_TRY );
//#endif
#endif

//#ifdef	_USER_DEFINE_

	EMV_CB_ShowMsg_LAST_PIN_TRY();
	
	if( g_emv_ped_src == 1 )
	  api_xped_show_MsgID(0);

	TL_WaitTime(200);		// 2013-12-11, wait 2 sec
	
	if( g_emv_ped_src == 1 )
	  {
	  api_xped_show_MsgID(255);	// return to main menu
	  TL_WaitTime(10);
	  }
//#endif
#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: display "PIN OK"
// ---------------------------------------------------------------------------
void	PP_show_pin_ok( void )
{
#if 0
	UI_ClearScreen();
	// UI_PutMsg( 0, COL_MIDWAY, FONT2, 8, (UCHAR *)stdmsg_C_pin_ok );
	UI_PutMsg( 1, COL_MIDWAY, FONT1, 6, (UCHAR *)stdmsg_pin_ok );
#else
    const UCHAR msg_PIN_OK[] = {"PIN OK"};


	// UI_ClearScreen();
	UI_ClearRow_EX( 1, 3, FONT1 );
#if	0
//	UI_PutMsg( 0, COL_MIDWAY, FONT2, 8, (UCHAR *)stdmsg_C_pin_ok );
//	UI_PutMsg( 1, COL_MIDWAY, FONT1, 6, (UCHAR *)stdmsg_pin_ok );

//#ifndef	_USER_DEFINE_

	UI_PutMsg( 3, COL_MIDWAY, FONT1, sizeof(msg_PIN_OK)-1, (UCHAR *)msg_PIN_OK );
//#endif
#endif

//#ifdef	_USER_DEFINE_

	EMV_CB_ShowMsg_PIN_OK();

//	if( g_emv_ped_src == 1 )
//	  api_xped_show_MsgID(3);
//	  
//	TL_WaitTime(200);		// 2013-12-11, wait 2 sec
//	
//	if( g_emv_ped_src == 1 )
//	  {
//	  api_xped_show_MsgID(255);	// return to main menu
//	  TL_WaitTime(10);
//	  }
//#endif
#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: display "PLEASE WAIT"
// ---------------------------------------------------------------------------
void	PP_show_please_wait( void )
{
#if 0
	UI_ClearScreen();
	// UI_PutMsg( 0, COL_MIDWAY, FONT2+attrCLEARWRITE, 6, (UCHAR *)stdmsg_C_please_wait );
	UI_PutMsg( 1, COL_MIDWAY, FONT1+attrCLEARWRITE, 11, (UCHAR *)stdmsg_please_wait );
#else
    const	UCHAR msg_PLEASE_WAIT[] = {"PLEASE WAIT"};


	// UI_ClearScreen();
	UI_ClearRow_EX( 1, 3, FONT1 );
#if	0
//	UI_PutMsg( 0, COL_MIDWAY, FONT2+attrCLEARWRITE, 6, (UCHAR *)stdmsg_C_please_wait );
//	UI_PutMsg( 1, COL_MIDWAY, FONT1+attrCLEARWRITE, 11, (UCHAR *)stdmsg_please_wait );

//#ifndef	_USER_DEFINE_

	UI_PutMsg( 3, COL_MIDWAY, FONT1, sizeof(msg_PLEASE_WAIT)-1, (UCHAR *)msg_PLEASE_WAIT );
//#endif
#endif

//#ifdef	_USER_DEFINE_

	EMV_CB_ShowMsg_PLEASE_WAIT();
//#endif
#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: request PIN (4-12 digits) from cardholder.
// INPUT   : amt   - total amount to be confirmed on display. (external)
//                   format: LEN(1) + ASCII(n)
//			     e.g., 0x07, "$100.00"
//	     tout  - time out for PIN entry in seconds.
// OUTPUT  : none.
// REF	   : g_dhn_pinpad.
//	         g_emv_ped_src.
// RETURN  : apiOK           - confirmed. (by entering OK)
//           apiFailed       - aborted.   (by entering CANCEL)
//           apiDeviceError  - PIN pad is malfunctioning.
//           apiOutOfService - timeout. (-1)
// ---------------------------------------------------------------------------
UCHAR	PP_GetPIN( UINT tout, UCHAR *amt )
{
UCHAR	status;
API_LCDTFT_PARA para;


	// special case for EMV test requirement
	if( g_dhn_pinpad == apiDeviceError )
	  return( apiDeviceError );
	  
	if( (g_emv_ped_src != 0) && (g_emv_ped_src != 1) )
	  return( apiDeviceError );		// 2010-03-22

	if( g_emv_ped_src == 0 )		// 2010-03-16
	  {
	  UI_ClearScreen();		// 2014-12-25
	  PP_show_enter_pin(amt);	//
	  
	  // 2015-05-21
	  memset( (UCHAR *)&para, 0x00, sizeof(API_LCDTFT_PARA) );
	  EMV_CB_SetupPinPad( &para );	// get PIN pad settings for PIN entry function from APP layer
	  api_ped_SetupPinPad( (UCHAR *)&para );	// convey parameters to kernel
	  
	  status = api_ped_GetPin( tout, amt );		// internal pinpad, 2013-12-07, add "tout"
	  }
	else
	  {
	  PP_show_enter_pin_by_ext_pinpad();
	  status = api_xped_GetPin( tout, amt );	// external pinpad, 2013-12-07, add "tout"
	  }
	  
	switch( status )
	      {
	      case apiOK:
	      	   break;
	      	   
	      case apiNotReady:	// bypass
	      	   status = apiFailed;
	      	   break;
	      	   
	      case apiFailed:	// timeout
	      	   status = apiOutOfService;
	      	   break;
	      }

	return( status );
}

// ---------------------------------------------------------------------------
// FUNCTION: construct a plaintext PIN block. (ISO 9564 format 2)
// INPUT   : none.
// OUTPUT  : pinblock - the plaintext pin block.
// RETURN  : none.
// ---------------------------------------------------------------------------
//void	PP_GenPinBlock( UCHAR *pinblock )	// 2014-10-29, removed.
//{
//	api_ped_GenPinBlock( pinblock );
//}

// ---------------------------------------------------------------------------
// FUNCTION: construct an encrypted PIN block. (by using fixed key)
// INPUT   : pan    - full PAN digits or transaction field for ISO3.
//		              format: L-V(n)
//	         mod      ISO format	(new)
//	         idx      key slot index	(new)
// OUTPUT  : epb    - the encrypted pin block.
//		              format: LL-V(8/16)
//	         ksn    - key serial number for DUKPT if available.
//		              format: LL-V(12)
// RETURN  : apiOK
//	         apiFailed
// ---------------------------------------------------------------------------
UCHAR	PP_GenEncrypedPinBlock( UCHAR *pan, UCHAR *epb, UCHAR *ksn )
//UCHAR	PP_GenEncrypedPinBlock( UCHAR *pan, UCHAR *epb, UCHAR *ksn, UCHAR mod, UCHAR idx )
{
UCHAR	result;
UCHAR	mode;
UCHAR	index;
UCHAR	temp[12];
UCHAR	epb2[16];
UCHAR	ksn2[12];
UINT    mode2;


	api_emv_ClrDataElement( DE_TERM, ADDR_TERM_EPIN_DATA, 18, 0x00 );
	api_emv_ClrDataElement( DE_TERM, ADDR_TERM_KSN, 14, 0x00 );

	memset( ksn, 0x00, 14 );
	memset( epb, 0x00, 18 );
	
	result = apiFailed;
	
	// The following two settings shall be assigned according to the actual key management requirement
	// mode = mod	;	//EPB_ISO0;	// algorithm for PIN block - ISO9564 Format 0
	// index = idx;		//0;		// key slot index for PIN usage - key slot #0

    mode	= g_emv_ped_mode;	// 2010-03-16
	index	= g_emv_ped_index;	//
		
	// switch( api_ped_GetKeyMode() )
    switch( g_emv_ped_algo )	// 2010-03-16
	      {
          case PED_KEY_MODE_NULL:	// NCCC & CTCB (non-PCI key bundle format)
		   	   
		   result = api_ped_GenPinBlock_MSKEY( mode, index, pan, epb2 );
		   if( result == apiOK )
		     {
		     epb[0] = 8;
		     epb[1] = 0;
		     memmove( &epb[2], epb2, 8 );
		     api_emv_PutDataElement( DE_TERM, ADDR_TERM_EPIN_DATA, 10, epb );	// save EPB for online transmission
		     }
		   break;
#if 0
	      case PED_KEY_MODE_FIXED:
      	    
		   result = api_ped_GenPinBlock_FXKEY( mode, index, pan, epb2 );
		   if( result == apiOK )
		     {
		     epb[0] = 8;
		     epb[1] = 0;
		     memmove( &epb[2], epb2, 8 );
		     api_emv_PutDataElement( DE_TERM, ADDR_TERM_EPIN_DATA, 10, epb );	// save EPB for online transmission
		     }
		   break;
#endif
      	    	 
	      case PED_KEY_MODE_MS:
      	    
		   result = api_ped_GenPinBlock_MSKEY( mode, index, pan, epb2 );
		   if( result == apiOK )
		     {
		     epb[0] = 8;
		     epb[1] = 0;
		     memmove( &epb[2], epb2, 8 );
		     api_emv_PutDataElement( DE_TERM, ADDR_TERM_EPIN_DATA, 10, epb );	// save EPB for online transmission
             }
		   break;
      	    	 
	      case PED_KEY_MODE_DUKPT:

		//    result = api_ped_GenPinBlock_DUKPT( mode, pan, epb2, ksn2 );
		//    if( result == apiOK )
		//      {
		//      epb[0] = 8;
		//      epb[1] = 0;
		//      memmove( &epb[2], epb2, 8 );
		//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_EPIN_DATA, 10, epb );	// save EPB for online transmission
		     
		//      ksn[0] = 10;
	  	//      ksn[1] = 0;
	  	//      memmove( &ksn[2], ksn2, 10 );
	  	//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_KSN, 12, ksn );		// save KSN for online transmission
		//      }
        
        //    mode2 = (g_aes_dukpt_working_key_type << 8) | mode;
        //    result = api_ped_GenPinBlock_AES_DUKPT( mode2, pan, epb2, ksn2 );
           result = api_ped_GenPinBlock_AES_DUKPT( mode, pan, epb2, ksn2 );
           if(result == apiOK)
           {
                if(g_aes_dukpt_working_key_type > 1)    // AES-128, AES-192, AES-256
                {
                    epb[0] = 16;
                    epb[1] = 0;
                    memmove(&epb[2], epb2, 16);
                    api_emv_PutDataElement( DE_TERM, ADDR_TERM_EPIN_DATA, 18, epb );	// save EPB for online transmission
                }
                else    // TDES-128, TDES-192
                {
                    epb[0] = 8;
                    epb[1] = 0;
                    memmove(&epb[2], epb2, 8);
                    api_emv_PutDataElement( DE_TERM, ADDR_TERM_EPIN_DATA, 10, epb );	// save EPB for online transmission
                }
                
                ksn[0] = 12;
                ksn[1] = 0;
                memmove(&ksn[2], ksn2, 12);
                api_emv_PutDataElement( DE_TERM, ADDR_TERM_KSN, 14, ksn );		// save KSN for online transmission
           }
		   break;
		   
	      case PED_KEY_MODE_ISO4:	// 2019-03-26
      	    
      	   result = api_ped_GenPinBlock_AESKEY( mode, index, pan, epb2 );	// must be mode=4, index=0
		   if( result == apiOK )
		     {
		     epb[0] = 16;
		     epb[1] = 0;
		     memmove( &epb[2], epb2, 16 );
		     api_emv_PutDataElement( DE_TERM, ADDR_TERM_EPIN_DATA, 18, epb );	// save EPB for online transmission
		     }
		   break;
      	    }
	
	return( result );
}
