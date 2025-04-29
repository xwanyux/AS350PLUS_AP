//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : EMV L2                                                     **
//**  PRODUCT  : AS330	                                                    **
//**                                                                        **
//**  FILE     : EMVDC_01.C                                                 **
//**  MODULE   : EMVDC_startup()                                            **
//**             EMVDC_close_session()                                      **
//**             EMVDC_SetupPosEntryMode()                                  **
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
#include "APPEMVconfig.h"

#include "POSAPI.h"

#include "EMVDC.h"
#include "GDATAEX.h"
#include "EMVAPI.h"


// ---------------------------------------------------------------------------
// FUNCTION: setup the value for POS entry mode (n1).
// INPUT   : mode. (PEM_xxx)
// OUTPUT  : none.
// RETURN  : none.
// ---------------------------------------------------------------------------
void EMVDC_SetupPosEntryMode( UCHAR mode )
{
     api_emv_PutDataElement( DE_TERM, ADDR_TERM_PEM, 1, (UCHAR *)&mode );
}

// ---------------------------------------------------------------------------
// FUNCTION: Waiting for inserting ICC and then process ATR function.
// INPUT   : none.
// OUTPUT  : atr - to store the contents of ATR, max 32 bytes.
// RETURN  : emvOK
//           emvFailed
//           emvAborted      (aborted by user)
//           emvOutOfService (RSA-SAM has failed or not been installed)
// ---------------------------------------------------------------------------
UCHAR EMVDC_startup( UCHAR *atr )
{
UCHAR buffer[32];
UCHAR flag;

UCHAR	sbuf[8];
UCHAR	dbuf[200];

STARTUP:

      UI_ClearScreen();
      
      flag = 0;
      g_dhn_msr = api_msr_open( isoTrack1 + isoTrack2 ); // MSR track 1 & 2

      while(1)
           {
           if( api_emv_CardPresent( g_dhn_icc ) == emvReady )
             {
               
             EMVDC_SetupPosEntryMode( PEM_ICC );

             api_msr_close( g_dhn_msr );
             break;
             }
           else
             {
             // "INSERT CARD"
             if( flag == 0 )
               {
                 
               DISP_STD_insert_card();
               
               flag = 1;
               }

             if( api_kbd_status(g_dhn_kbd, buffer) == apiReady ) // key pressed?
               {
               if( UI_WaitKey() == 'x' )   // cancel?
                 {
                 api_msr_close( g_dhn_msr );
                 return( emvAborted ); // abort
                 }
               }

             // MSR
             api_msr_status( g_dhn_msr, 0, buffer );
             if( (buffer[0] == msrSwiped) && ((buffer[1] == msrDataOK) || (buffer[2] == msrDataOK)) )
               {
                 
               if( EMVDC_MsrProcessing( PEM_MSR, buffer ) )
               {
                 
                 return( apiNotReady );	// for PCI MSR Online Transaction Only
               }
               else
               {
                 
                 goto STARTUP;		// for EMV L2 only
               }
                 
               }

             }
           }

      TL_WaitTime(40);
      UI_ClearRow( 0, 1, FONT1 );
      TL_WaitTime(40);

      // show "PLEASE WAIT"
//    DISP_STD_please_wait();

#ifdef	USE_RSA_SAM

      // reset RSA-SAM
      if( api_emv_ATR( g_dhn_sam, atr ) != emvOK )
        return( emvOutOfService );
      else
        {
        // select AID = "A0 00 00 00 00 00 02"
        if( api_emv_SelectSAM() != emvOK )
          return( emvOutOfService );

        api_emv_CleanSAM();              // garbage collection
        }
#endif

      // reset target ICC
      if( api_emv_ATR( g_dhn_icc, atr ) == emvOK )
        {
          
        TL_WaitTime( 40 );
        return( emvOK );
        }
      else
        {
        // CARD ERROR
        // REMOVE CARD

//      api_emv_Deactivate( g_dhn_icc ); // disable all ICC contacts
//
//      DISP_card_error();
//      DISP_remove_card();
//
//      while( api_emv_CardPresent( g_dhn_icc ) == emvReady );

        return( emvFailed );
        }
}

// ---------------------------------------------------------------------------
// FUNCTION: Close session and wait for removing card due to error found or
//           normal end of process.
//           The related message should be shown at the 1'st row of display
//           before calling this function.
// INPUT   : wait -- 0 = wait N seconds
//                   1 = wait unitil any key is pressed.
//                 255 = no waiting.
// OUTPUT  : none.
// RETURN  : none.
// ---------------------------------------------------------------------------
void EMVDC_close_session( UCHAR wait )
{
//    UI_BeepLong();                   // a long beep

//    api_emv_CleanSAM();              // garbage collection

      api_emv_Deactivate( g_dhn_icc ); // disable all ICC contacts
#ifdef	USE_RSA_SAM
      api_emv_Deactivate( g_dhn_sam ); // disable all SAM contacts
#endif

      switch( wait )
            {
            case 0:
                 TL_WaitTimeAndKey( 300 );      // wait 3 sec
                 break;

            case 1:
                 UI_WaitKey();                  // wait key stroke
                 break;

	    case 254:
	    	 goto EXIT;
	    	 
            case 255:
                 break;                         // no waiting
            }

//    if( wait == 0 )
//      TL_WaitTimeAndKey( 150 );      // wait 1.5 sec
//    else
//      UI_WaitKey();                  // wait key stroke

      UI_CloseKeyAll();

      DISP_STD_remove_card();          // instruct to remove card
      TL_WaitTime(40);
      while( api_emv_CardPresent( g_dhn_icc ) == emvReady );
      TL_WaitTime(40);

EXIT:
      api_emv_CloseSession( g_dhn_icc );
#ifdef	USE_RSA_SAM
      api_emv_CloseSession( g_dhn_sam );
#endif

      UI_OpenKeyAll();
}

// ---------------------------------------------------------------------------
// FUNCTION: retrieve CA Public Key Header to main memory for better performance.
//           if none of CA public key exists, do nothing.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : emvOK
//           emvFailed
// ---------------------------------------------------------------------------
UCHAR EMVDC_RetrievePublicKeyCA( void )
{
#ifdef	USE_RSA_SAM

UCHAR buf[34];
UCHAR result;

      // setup number of CA public key (temp solution for EMV L2 testing)
      // the varialbe "g_capk_cnt" will be updated according to key loading system.
//    g_capk_cnt = MAX_KEY_SLOT_CNT;

      g_dhn_sam = api_emv_OpenSession( RSA_SAM ); // RSA SAM

      // reset RSA-SAM
      result = emvFailed;

      if( api_emv_ATR( g_dhn_sam, buf ) == emvOK )
        {
        // select AID = "A0 00 00 00 00 00 02"
        if( api_emv_SelectSAM() == apiOK )
          {
          if( api_emv_RetrievePublicKeyCA() == emvOK )
            result = emvOK;
          }
        }

      api_emv_CleanSAM();              // garbage collection
      api_emv_Deactivate( g_dhn_sam ); // disable all SAM contacts

      api_emv_CloseSession( g_dhn_sam );

      return( result );
      
#endif

#ifndef	USE_RSA_SAM

	return( api_emv_RetrievePublicKeyCA() );
	
#endif
}
