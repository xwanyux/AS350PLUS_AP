//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : EMV L2                                                     **
//**  PRODUCT  : AS330	                                                    **
//**                                                                        **
//**  FILE     : EMVDC_08.C                                                 **
//**  MODULE   : EMVDC_terminal_risk_management()                           **
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


// ---------------------------------------------------------------------------
// FUNCTION: the TRM is that portion of risk management performed by the
//           terminal to protect the acquirer, issuer, and system from fraud.
//           (1) Floor limit checking.
//           (2) Random transaction selection.
//           (3) Velocity checking.
// INPUT   : none.
// OUTPUT  : none.
// REF     : g_term_tx_log_cnt
//           g_term_tx_amt
// RETURN  : emvOK
//           emvFailed
//
//           emvOutOfService (shall terminate the transaction)
// ---------------------------------------------------------------------------
UCHAR EMVDC_terminal_risk_management( void )
{
UINT  i;
UINT  iLen;
UCHAR pan[10];
UCHAR amt[6];
UCHAR log[TX_LOG_LEN];

      // Find the most recent entry the same PAN
      // << This part shall be modified according to the real application. >>
      memset( amt, 0x00, 6 ); // clear amt value

      if( g_term_tx_log_cnt != 0 ) // tx log available?
        {
        // get current PAN ("pan" is left-justified & padded with trailing 'F'.
        memset( pan, 0xFF, 10 );
        api_emv_GetDataElement( DE_ICC, ADDR_ICC_AP_PAN, 2, (UCHAR *)&iLen );
	api_emv_GetDataElement_SRED( DE_ICC, ADDR_ICC_AP_PAN+2, iLen, pan );
	
        // scan all logs "backwardly" to find the same PAN
        i = g_term_tx_log_cnt;
        do{

          TL_GetTransLog( --i, log );

          if( TL_memcmp( pan, &log[LOG_AP_PAN], iLen ) == 0 ) // same PAN?
            {
            memmove( amt, &log[LOG_TX_AMT], 6 ); // get amount value

// UI_ClearScreen();
// TL_DispHexByte(0,0, 0x11);
// UI_WaitKey();
// TL_DumpHexData(0,0,6, amt);

            break;
            }

          } while( i != 0 );
        }

// UI_ClearScreen();
// TL_DispHexByte(0,0, 0x22);
// UI_WaitKey();
// TL_DumpHexData(0,0,6, amt);

      return( api_emv_TerminalRiskManagement( g_term_tx_amt, g_term_tx_amt, amt ) );

      // 1. TRM -- Floor Limit Checking
//    api_emv_FloorLimitChecking( g_term_tx_amt, amt );

      // 2. TRM -- Random Transaction Selection
//    api_emv_RandomTransactionSelection( g_term_tx_amt, g_term_tx_amt );

      // 3. TRM -- Velocity Checking
//    api_emv_VelocityChecking();

}

