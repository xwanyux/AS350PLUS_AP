//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : EMV L2                                                     **
//**  PRODUCT  : AS330	                                                    **
//**                                                                        **
//**  FILE     : EMVDC_07.C                                                 **
//**  MODULE   : EMVDC_cardholder_verification()                            **
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

#include "EMVDC.h"
#include "GDATAEX.h"
#include "EMVAPI.h"
#include "EMVDCMSG.h"
#include "TOOLS.h"


// ---------------------------------------------------------------------------
// FUNCTION: cardholder verification is performed to ensure that the person
//           presenting the ICC is the one to whom the applicaiton in the card
//           was issued.
//           (1) Offline Plaintext PIN.
//           (2) Offline Enciphered PIN.
//           (3) Online PIN.
//           (4) Signature.
// INPUT   : none.
// OUTPUT  : none.
// REF     : g_term_tx_amt[n6]
// RETURN  : emvOK               (CVM OK --> referece only)
//           emvFailed           (CVM Failed --> reference only)
//           emvOutOfService --> terminate current transaction.
//                               (to disagree the transaction amount or ...)
// ---------------------------------------------------------------------------
UCHAR EMVDC_cardholder_verification( void )
{
UCHAR i;
UCHAR buf1[21];
UCHAR buf2[52];
UCHAR epb[20];		// 2019-04-26
UCHAR ksn[14];
UCHAR list[3*EMVDC_FUNC_LEN];
UCHAR result;

      // wait for attendant's selection
      // 1 SALE   (general transaction)
      // 2 ONLINE (forced transaction online)
      // 3 AUTH   (authorize transaction)
      buf1[0] = 0;
      buf1[1] = 2;
      buf1[2] = 2; // PATCH: 2006-02-17, removed the last item (AUTH)
      buf1[3] = EMVDC_FUNC_LEN;
      buf1[4] = 0;
      buf1[5] = FONT0;	//FONT2;
      TL_LoadFuncList( 3, list );
      UI_ClearScreen(); //Added by Tammy
      result = TL_ListBox( 0, &buf1[0], list, 00 ); // always wait

      UI_ClearScreen();

//    if( result == 0 )
//      {
//      // SALE
//      DISP_sale();
//      g_forced_online = FALSE;
//      }
//    else
//      {
//      // ONLINE
//      DISP_online();
//      g_forced_online = TRUE;
//      }

      g_forced_online = FALSE;
      g_pre_auth = FALSE;
      switch( result )
            {
            case 0: // SALE

//               DISP_sale();
                 break;

            case 1: // ONLINE

//               DISP_online();
                 g_forced_online = TRUE;
                 break;

            case 2: // AUTHORIZE: for TA only

                 DISP_auth();
                 g_pre_auth = TRUE;
                 break;

            default:
                 return( emvOutOfService );
            }

      // ($NN,NNN) OK?
      // convert AMT bcd to ascii ($NNN)
      TL_bcd2asc( AMT_INT_SIZE+AMT_DEC_SIZE, g_term_tx_amt, buf1 );
      TL_trim_asc( 0, buf1, buf2 );
      i = TL_insert_decimal_point( 0x04, buf2 ); // NNNNN.NN
//    TL_insert_thousand_comma( buf2, i );       // NN,NNN.NN
      TL_trim_decimal( g_term_tx_exp, g_term_decimal_point, buf2 ); // NNNNN

//      buf1[0] = '(';
//      buf1[1] = '$';
//      for( i=0; i<buf2[0]; i++ )
//         buf1[i+2] = buf2[i+1];
//      buf1[i+2] = ')';
//
//      buf1[i+3] = ' ';
//      buf1[i+4] = 'O';
//      buf1[i+5] = 'K';
//      buf1[i+6] = '?';

//      UI_PutStr( 3, 0, FONT0, buf2[0]+7, buf1 );
//
//      // wait to confirm the transaction amount
//      do{
//        result = UI_WaitKey();
//        if( result == KEY_CANCEL ) // disagree amout?
//          return( emvOutOfService );
//        } while( result != KEY_OK );

//      // show "PLEASE WAIT"
//      UI_ClearScreen();
//      DISP_STD_please_wait();
//
//      // prepare message ($amount) to be shown on PIN PAD,
//      // which will be confirmed by the cardholder before entering PIN
//      i = buf2[0] + 7;                // length
//      buf2[0] = 3;                    // row
//      buf2[1] = 0;                    // col
//      buf2[2] = FONT0;                // font
//      buf2[3] = i;                    // length
//      memmove( &buf2[4], buf1, i );   // 1'st message
//
//      buf2[24+0] = 0;                 // row
//      buf2[24+1] = 0;                 // col
//      buf2[24+2] = FONT2;             // font
//      buf2[24+3] = 15;                // length
//      TL_memmove( &buf2[24+4], (UCHAR *)msg_pin_pad, 15 ); // 2'nd message
//
//      buf2[48+3] = 0;                 // no 3'rd message

      // CVM
      // total amount "$NNNN.NN" to be confirmed on PINPAD
      buf1[0] = buf2[0] + 1;
      buf1[1] = '$';
      for( i=0; i<buf2[0]; i++ )
	 buf1[i+2] = buf2[i+1];
	 
      return( api_emv_CardholderVerification( 60, buf1, epb, ksn, g_iso_format, g_key_index ) );
}
