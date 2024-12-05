//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : EMV L2                                                     **
//**  PRODUCT  : AS330	                                                    **
//**                                                                        **
//**  FILE     : EMVDC_03.C                                                 **
//**  MODULE   : EMVDC_initiate_application()                               **
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
#include "UI.h"
#include "TOOLS.h"

// ---------------------------------------------------------------------------
// FUNCTION: (1) Inform ICC a new transaction.
//           (2) Provide to ICC the terminal info about transaction.
//           (3) Obtain from ICC the AIP and a list of files that contain the
//               ICC data to be used in processing the transaction.
//           (4) Determine whether the transaction is allowed.
// INPUT   : fci - the FCI of the selected file. (RFU)
// OUTPUT  : none.
// RETURN  : emvOK
//           emvNotReady (the application cannot be applied) --> try next one.
//           emvFailed   (init failed) --> terminate transaction.
//           emvAborted  (transaction aborted by attendant) --> terminate transaction.
// ---------------------------------------------------------------------------
UCHAR EMVDC_initiate_application( void )
{
UCHAR i;
UCHAR *ptrobj;
UCHAR result1, result2;
UCHAR buffer[16];
UCHAR digits;
UCHAR len;


      // clear TVR & TSI to 0
//    for( i=0; i<5; i++ )
//       {
//       g_term_TVR[i] = 0;
//       api_emv_PutDataElement( DE_TERM, ADDR_TERM_TVR+i, 1, g_term_TVR );
//       }
//
//    for( i=0; i<2; i++ )
//       {
//       g_term_TSI[i] = 0;
//       api_emv_PutDataElement( DE_TERM, ADDR_TERM_TSI+i, 1, g_term_TSI );
//       }

      // check "Amount" element in PDOL of FCI ("Amount, Other" is not supported)
//    result1 = api_emv_CheckPDOL( 0x9f, 0x02 ); // Amount, Authorized (n12)?
//    result2 = api_emv_CheckPDOL( 0x00, 0x81 ); // Amount, Authorized (b)?

      UI_ClearScreen(); // GW12864

      // always need to enter amount
      result1 = emvOK;
      result2 = emvOK;

      if( (result1 == emvOK) || (result2 == emvOK) )
        {
        // display "ENTER AMOUNT" request
GET_AMT:
    //  DISP_STD_enter_amount();
        DISP_GRP_enter_amount();

        if( g_term_decimal_point == 0 )
          digits = 10 - g_term_tx_exp;
        else
          digits = 10;

        if( TL_GetNumKey( 0, NUM_TYPE_DEC, '0', FONT0, 3, digits, buffer ) == FALSE )
          return( emvAborted ); // aborted
        if( (buffer[0] == 1) && (buffer[1] == '0') ) // amt = 0?
          goto GET_AMT;

        // compensation for implicit decimal point (eg. exp=2 but decimal=0)
        if( (g_term_decimal_point == 0) && (g_term_tx_exp != 0) )
          {
          len = buffer[0];
          for( i=0; i<g_term_tx_exp; i++ )
             buffer[len+1+i] = '0';
          buffer[0] += g_term_tx_exp;
          }

        // backup current tx amount in 'n12' format (including integer & decimal)
        TL_asc2bcd( AMT_INT_SIZE+AMT_DEC_SIZE, g_term_tx_amt, buffer ); // integer+decimal part

        // range check, max. amount = "42 94 96 72 95" (0xFF FF FF FF)

//      buffer[0] = 0x42;
//      buffer[1] = 0x94;
//      buffer[2] = 0x96;
//      buffer[3] = 0x72;
//      buffer[4] = 0x95;
//      if( TL_memcmp( g_term_tx_amt, buffer, AMT_INT_SIZE ) > 0 )
        if( TL_VerifyTransAmount( g_term_tx_amt ) == FALSE )
          {
          // display "NOT ACCEPTED"
          printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
          DISP_STD_not_accepted();
          TL_WaitTime(100);
          goto GET_AMT;
          }
        }
      else
        {
        for( i=0; i<(AMT_INT_SIZE+AMT_DEC_SIZE); i++ )
           g_term_tx_amt[i] = 0x00; // clear tx amt (n10+n2)
        }
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_AMT_AUTH_N, 6, g_term_tx_amt );

      // issue "GET PROCESSING OPTIONS" command
      result1 =  api_emv_InitApplication( TT_GOODS_AND_SERVICE, g_term_tx_amt );
      if( result1 ==  emvOK )
        return( emvOK );
      else
        {
        if( result1 == emvNotReady )
          {
          // eliminate the current App from consideration, try another.
          api_emv_RemoveApplication( g_candidate_name_index );
          return( emvNotReady );
          }
        else
          return( emvFailed );
        }
}

