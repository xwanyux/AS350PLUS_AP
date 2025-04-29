//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : EMV L2                                                     **
//**  PRODUCT  : AS330	                                                    **
//**                                                                        **
//**  FILE     : EMVDC_09.C                                                 **
//**  MODULE   : EMVDC_terminal_action_analysis()                           **
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
#include <string.h>

#include "APPEMVconfig.h"

#include "POSAPI.h"

#include "EMVDC.h"
#include "GDATAEX.h"
#include "EMVAPI.h"
#include "EMVDCMSG.h"
#include "TOOLS.h"
#include "UI.h"


// ---------------------------------------------------------------------------
// FUNCTION: setup a fixed length transaction log for saving.
//
//      Field Name                Data Format     Length
//      ------------------------  -----------     --------
//      Transaction Seq Counter   n8              4
//      App PAN                   cn ~19          10
//      App PAN Sequence Number   n2              1
//      Transaction Amount        n10+n2          5+1
//      Transaction Date          n6 (YYMMDD)     3
//      Transaction Time          n6 (HHMMSS)     3
//      Authorization Code        an6             6
//      ARC                       an2             2
//      Message Type              an2             2
//      RFU                       (null)          27
//      ------------------------  ------------    --------
//      TOTAL                                     64 bytes
//
// INPUT   : msgtype - to distinguish between offline and online.
// OUTPUT  : log     - the transaction log to be stored.
// REF     :
// RETURN  : none.
// ---------------------------------------------------------------------------
void EMVDC_SetupTransLog( UINT msgtype, UCHAR log[] )
{
UCHAR buf[64];
ULONG hex;
UINT  iLen;

      memset( log, 0x00, TX_LOG_LEN );

      // Transaction Sequence Counter
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TX_SC, 4, buf );
//    buf[4] = 0x00;                         // cnt++
//    buf[5] = 0x00;                         //
//    buf[6] = 0x00;                         //
//    buf[7] = 0x01;                         //
//    TL_bcd_add_bcd( 4, &buf[0], &buf[4] ); //
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_SC, 4, buf );
      memmove( &log[LOG_TX_SC], buf, 4 );

      // Application PAN
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_AP_PAN, 2, (UCHAR *)&iLen );
      api_emv_GetDataElement_SRED( DE_ICC, ADDR_ICC_AP_PAN+2, iLen, buf );

      memset( &log[LOG_AP_PAN], 0xFF, 10 );
      memmove( &log[LOG_AP_PAN], buf, iLen );

      // Application PAN Sequence Number
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_AP_PAN_SN+2, 1, buf );
      memmove( &log[LOG_AP_PAN_SN], buf, 1 );

      // Transaction Amount
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TX_AMT, 6, buf );
      memmove( &log[LOG_TX_AMT], buf, 6 );

      // Transaction Date
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TX_DATE, 3, buf );
      memmove( &log[LOG_TX_DATE], buf, 3 );

      // Transaction Time
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TX_TIME, 3, buf );
      memmove( &log[LOG_TX_TIME], buf, 3 );

      // Authorization Code
      api_emv_GetDataElement( DE_TERM, ADDR_ISU_AUTH_CODE, 6, buf );
      memmove( &log[LOG_AUTH_CODE], buf, 6 );

      // Authorization Responce Code (from ISSUER(online) or TERMINAL(offline))
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_ARC, 2, buf );
      if( (buf[1]*256 + buf[0]) == 0 )
        api_emv_GetDataElement( DE_TERM, ADDR_ISU_ARC, 2, buf );
      memmove( &log[LOG_ARC], buf, 2 );

      // Message Type
      log[LOG_MSG_TYPE+0] = (msgtype & 0xff00) >> 8;
      log[LOG_MSG_TYPE+1] = msgtype & 0x00ff;

      // AIP
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_AIP, 2, buf );
      log[LOG_AIP+0] = buf[0];
      log[LOG_AIP+1] = buf[1];

      // ATC
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_ATC+2, 2, buf );
      log[LOG_ATC+0] = buf[0];
      log[LOG_ATC+1] = buf[1];

      // AUC
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_AUC+2, 2, buf );
      log[LOG_AUC+0] = buf[0];
      log[LOG_AUC+1] = buf[1];

      // ARQC
      log[LOG_ARQC] = AC_ARQC;

      // CID
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_CID+2, 1, buf );
      log[LOG_CID] = buf[0];

      // CVM List
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_CVM_LIST, 22, buf );
      memmove( &log[LOG_CVML], buf, 22 );

      // CVM Results
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_CVMR, 3, buf );
      memmove( &log[LOG_CVMR], buf, 3 );

      // IFD Serial Number
//    api_emv_GetDataElement( DE_TERM, ADDR_TERM_IFD_SN, 8, buf );
//    memmove( &log[LOG_IFD_SN], buf, 8 );

      // IAC-Denial
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_IAC_DENIAL+2, 5, buf );
      memmove( &log[LOG_IAC_DEN], buf, 5 );

      // IAC-Online
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_IAC_ONLINE+2, 5, buf );
      memmove( &log[LOG_IAC_ONL], buf, 5 );

      // IAC-Default
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_IAC_DEFAULT+2, 5, buf );
      memmove( &log[LOG_IAC_DEF], buf, 5 );

      // Issuer Application Data
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_ISU_AP_DATA, 34, buf );
      memmove( &log[LOG_ISU_AP_DATA], buf, 34 );

      // Terminal Capabilities
//    api_emv_GetDataElement( DE_TERM, ADDR_TERM_CAP, 3, buf );
//    memmove( &log[LOG_TERM_CAP], buf, 3 );

      // Terminal Type
//    api_emv_GetDataElement( DE_TERM, ADDR_TERM_TYPE, 1, buf );
//    log[LOG_TERM_TYPE] = buf[0];

      // TVR
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TVR, 5, buf );
      memmove( &log[LOG_TVR], buf, 5 );

      // Unpredictable Number
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_UPD_NBR, 4, buf );
      memmove( &log[LOG_UPD_NBR], buf, 4 );

      // Acquirer ID
//    api_emv_GetDataElement( DE_TERM, ADDR_TERM_ACQ_ID, 6, buf );
//    memmove( &log[LOG_ACQ_ID], buf, 6 );

      // Amount, Authorized
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_AMT_AUTH_N, 6, buf );
      memmove( &log[LOG_AMT_AUTH], buf, 6 );

      // Amount, Other
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_AMT_OTHER_N, 6, buf );
      memmove( &log[LOG_AMT_OTHER], buf, 6 );

      // App Effective Date
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_AP_EFFECT_DATE+2, 3, buf );
      memmove( &log[LOG_AP_EFFECT_DATE], buf, 3 );

      // App Expiration Date
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_AP_EXPIRE_DATE+2, 3, buf );
      memmove( &log[LOG_AP_EXPIRE_DATE], buf, 3 );

      // Issuer Country Code
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_ISU_CNTR_CODE+2, 2, buf );
      memmove( &log[LOG_ISU_CNTR_CODE], buf, 2 );

      // Merchant Category Code
//    api_emv_GetDataElement( DE_TERM, ADDR_TERM_MCC, 2, buf );
//    memmove( &log[LOG_MCC], buf, 2 );

      // Merchant ID
//    api_emv_GetDataElement( DE_TERM, ADDR_TERM_MID, 15, buf );
//    memmove( &log[LOG_MID], buf, 15 );

      // POS Entry Mode
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_PEM, 1, buf );
      log[LOG_PEM] = buf[0];

      // Terminal Country Code
//    api_emv_GetDataElement( DE_TERM, ADDR_TERM_CNTR_CODE, 2, buf );
//    memmove( &log[LOG_TERM_CNTR_CODE, buf, 2 );

      // Terminal ID
//    api_emv_GetDataElement( DE_TERM, ADDR_TERM_TID, 8, buf );
//    memmove( &log[LOG_TID], buf, 8 );

      // Transaction Currency Code
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TX_CC, 2, buf );
      memmove( &log[LOG_TX_CC], buf, 2 );

      // Transaction Type
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TX_TYPE, 1, buf );
      log[LOG_TX_TYPE] = buf[0];

      // Issuer Script Results
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_ISR, 21, buf );
      memmove( &log[LOG_ISR], buf, 21 );

      // AC
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_AC+2, 8, buf );
      memmove( &log[LOG_AC], buf, 8 );

      // TSI
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TSI, 2, buf );
      memmove( &log[LOG_TSI], buf, 2 );

}

// ---------------------------------------------------------------------------
// FUNCTION: once terminal risk management and application functions related
//           to a normal offline transaction have been completed, the terminal
//           makes the first decision as to whether the transaction should be:
//           (1) Approved Offline   or
//           (2) Declined Offline   or
//           (3) Transmitted Online
// INPUT   : none.
// OUTPUT  : none.
// REF     : g_term_tx_log_cnt
//           g_forced_online
// RETURN  : emvOK       (TAA has finished)
//           emvFailed   (ICC logic error, terminate the transaction)
// ---------------------------------------------------------------------------
UCHAR EMVDC_terminal_action_analysis( void )
{
UCHAR result;
UCHAR arc[2];
UINT  iARC;
UCHAR log[TX_LOG_LEN];

      // --- TAA ---
      api_emv_IncTransSequenceCounter(); // tx sequence cnt++

      result = api_emv_TerminalActionAnalysis2( g_forced_online, arc );
      printf("@@@@@@@@@@@@%s %d result=%d\n",__func__,__LINE__,result);
      if( result == emvFailed )
        return( emvFailed );

      if( result == emvAborted ) // CDA failed after 2'nd issuance
        {
        DISP_STD_declined();
        return( emvOK );
        }

      // saving "Transaction Log" if approved
      iARC = arc[0]*256 + arc[1];
      printf("@@@@@@@@@@@@%s %d iARC=%d\n",__func__,__LINE__,iARC);
      if( (iARC == ARC_OFFLINE_APPROVED) || (iARC == ARC_ONLINE_APPROVED) ||
          (iARC == ARC_APPROVED_UNABLE_ONLINE) || (iARC == ISU_ARC_APPROVED) ||
          (iARC == 0x5934) )
        {
              printf("@@@@@@@@@@@@%s %d \n",__func__,__LINE__ );
        EMVDC_SetupTransLog( iARC, log ); // prepare transaction log

SAVE_TX_LOG:
        if( TL_PutTransLog( g_term_tx_log_cnt, log ) == FALSE ) // save it to memory
          {
            printf("@@@@@@@@@@@@%s %d \n",__func__,__LINE__ );
          // out of memory, reqeust to clear it or not.
          UI_ClearScreen();
          DISP_trans_log_full();
          if( TL_UpdateReq( 2, 3, COL_LEFTMOST, FONT0 ) == TRUE )
            {
            TL_ClearTransLog();
            goto SAVE_TX_LOG;
            }
          }
printf("@@@@@@@@@@@@%s %d \n",__func__,__LINE__ );
        DISP_STD_approved();
        }
      else
        DISP_STD_declined();

      return( emvOK );
}

