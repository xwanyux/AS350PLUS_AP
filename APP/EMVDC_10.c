//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : EMV L2                                                     **
//**  PRODUCT  : AS330	                                                    **
//**                                                                        **
//**  FILE     : EMVDC_10.C                                                 **
//**  MODULE   :                                                            **
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
#include "EMVAPK.h"
#include "EMVDCMSG.h"
#include "TOOLS.h"
#include "UI.h"
#include "PINPAD.h"

extern	UCHAR FB_GetAmount( UCHAR *amt );

// ---------------------------------------------------------------------------
// FUNCTION: MSR online transaction for PCI test.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : emvOK
//           emvFailed
// ---------------------------------------------------------------------------
UCHAR	PCI_MsrOnlineTransaction( void )
{
UCHAR	buf[32];
UCHAR	buf2[32];
UCHAR	arc[2];
UCHAR	iad[34];
UCHAR	epb[18];	// LL-V(8+8)
UCHAR	ksn[14];	// LL-V(12)
UINT	ist;
UINT	iLen;
UCHAR	result;
ULONG	cnt;
UCHAR	data;


	// set POS Entry Mode
	EMVDC_SetupPosEntryMode( PEM_FALLBACK );
	if( FB_GetAmount(buf) == FALSE )
	  {
	  api_msr_close( g_dhn_msr );
	  return( emvFailed );
	  }
	TL_insert_decimal_point( NUM_TYPE_DEC, buf ); // NNNNN.NN

	// --- Request PIN Entry ---
	buf2[0] = buf[0] + 1;
	buf2[1] = '$';
	memmove( &buf2[2], &buf[1], buf[0] );
	printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
	result = PP_GetPIN( 30, buf2 );
  printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
	if( (result == apiOK) || (result == apiNotReady) ) // PIN ready or bypassed
	  {
	  // PAN
	  api_emv_GetDataElement( DE_ICC, ADDR_ICC_AP_PAN, 2, (UCHAR *)&iLen );
	  api_emv_GetDataElement( DE_ICC, ADDR_ICC_AP_PAN+2, iLen, buf2 );
//	  api_emv_GetDataElement_SRED( DE_ICC, ADDR_ICC_AP_PAN+2, iLen, buf2 );
	  TL_bcd2asc( iLen, buf2, buf );  // buf = pan, L-V(ANS)
	  
	  // --- Generate Encrypted PIN Block ---
	  
	  if( PP_GenEncrypedPinBlock( buf, epb, ksn ) == apiOK )
    //   if( PP_GenEncrypedPinBlock( buf, epb, ksn, g_iso_format, g_key_index ) == apiOK )
	    {
	    iLen = epb[0]+epb[1]*256;
	    if( iLen != 0 )
	      {
            printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
	      // ----------------------------------------------------------------------------------
	      // Here shows the Encryped PIN Block (EPB) on display for PCI online testing purpose
	      // ----------------------------------------------------------------------------------
	    //   UI_ClearScreen();
	      
	    //   UI_PutStr( 0, 0, FONT0, 4, "EPB:" );
	    //   TL_DumpHexData( 0, 1, iLen, epb+2 );
	      }
	    
	    if( (ksn[0]+ksn[1]*256) != 0 )
	      {
	      // ----------------------------------------------------------------------------------
	      // Here shows the Key Serial Number (KSN) on display for PCI online testing purpose
	      // ----------------------------------------------------------------------------------
	    //   UI_PutStr( 4, 0, FONT0, 4, "KSN:" );
	    //   TL_DumpHexData( 0, 5, 12, ksn+2 );
	      }
	    }
	  else	// fail to generate EPB
	    {
	    api_msr_close( g_dhn_msr );
          
            UI_ClearScreen();
            DISP_STD_declined();
	    UI_WaitKey();
	    return( emvFailed );
	    }
	  
	  // setup transaction Date & Time
	  TL_GetDateTime( &buf[1] );

	  buf[0]=6;
	  TL_asc2bcd( 3, buf2, buf );     // convert to BCD, buf2: YYMMDD
	  api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_DATE, 3, buf2 );

	  buf[6]=6;
	  TL_asc2bcd( 3, buf2, &buf[6] ); // convert to BCD, buf2: HHMMSS
	  api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_TIME, 3, buf2 );

	  // setup transaction Sequence Counter
	  api_emv_IncTransSequenceCounter();

	  // must go online
	  if( EMVDC_OnlineProcessing( MSGID_FINANCIAL_REQ, 0x00, arc, iad, (UINT *)&ist ) == ONL_Completed )
	    {
	    DISP_STD_approved();
	    UI_WaitKey();

	    buf[0] = 0x1E;
	    buf[1] = 0x00;
	    buf[2] = 0x00;
	    api_emv_PutDataElement( DE_TERM, ADDR_TERM_CVMR, 3, buf );

	    g_term_ARC[0] = arc[0];
	    g_term_ARC[0] = arc[1];
	    api_emv_PutDataElement( DE_TERM, ADDR_TERM_ARC, 2, g_term_ARC );
	    }
	  else // unable to go online
	    {
	    api_msr_close( g_dhn_msr );
	    
	    UI_ClearScreen();
	    DISP_STD_declined();
	    UI_WaitKey();
	    return( emvFailed );
	    }
          }
        else // fail to get PIN (timeout or invalid request)
          {
          api_msr_close( g_dhn_msr );
       printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);   
          UI_ClearScreen();
          DISP_STD_declined();
          printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
	  UI_WaitKey();
	  return( emvFailed );
          }
          printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
	api_msr_close( g_dhn_msr );
	return( emvOK ); // done

}

// ---------------------------------------------------------------------------
// FUNCTION: Left shift the string N bits.
// INPUT   : old - the original string.
//           len - length of string.
//           cnt - number of bits to be shifted.
// OUTPUT  : new - the result string.
// REF     : none.
// RETURN  : none.
// ---------------------------------------------------------------------------
void  EMVDC_left_shift_str( UCHAR *new, UCHAR *old, UCHAR len, UCHAR cnt )
{
UCHAR i, j;

      memmove( new, old, len );

      for( i=0; i<cnt; i++ )
         {
         for( j=0; j<len; j++ )
            {
            new[j] <<= 1;  // leftshift N'th byte one bit

            if( (j+1) < len )
              {
              if( new[j+1] & 0x80 )  // shfit the (N-1)'th carry to the N'th lsb
                new[j] |= 0x01;
              }
            else
              break;
            }
         }
}

// ---------------------------------------------------------------------------
// FUNCTION: MSR process. (msr data have been captured)
// INPUT   : status - track status
//           mode   - POS entry mode. (PEM_xxx)
// OUTPUT  : none.
// RETURN  : TRUE  - MSR only
//	     FALSE - ICC Fallback
// ---------------------------------------------------------------------------
UCHAR EMVDC_MsrProcessing( UCHAR mode, UCHAR *status )
{
UCHAR i, j;
UCHAR ch_name[26];
UCHAR exp_date[4]; // YYMM
UCHAR pan[19];
UCHAR service_code[3];
UINT  iLen;
UCHAR pan_len;
UCHAR buf[32];
UCHAR buf2[32];
UCHAR fMSR;

      // Card Holder Name
      //
      // PAN
      // Expiry Date (YY/MM)

      // Track 1 Data
      g_ibuf[0] = 1;
      g_ibuf[1] = isoTrack1;
      api_msr_getstring( g_dhn_msr, g_ibuf, g_obuf );

      memset( ch_name, 0x20, sizeof(ch_name) );
      memset( exp_date, 0x20, sizeof(exp_date) );
      memset( service_code, 0x20, sizeof(service_code) );

      if( status[1] == msrDataOK )
        {
        pan_len = 0;
        memset( pan, 0x20, sizeof(pan) );

        // Track 1 PAN
        for( i=0; i<19; i++ )
           {
           if( g_obuf[i+3] != '^' )
             {
             pan[i] = g_obuf[i+3];
             pan_len++;
             }
           else
             break;
           }

        // cardholder name ^XXXX..X^
        for( i=1; i<g_obuf[0]; i++ )
           {
           if( g_obuf[i] == '^' )
             {
             for( j=0; j<26; j++ )
                {
                if( g_obuf[i+j+1] == '?' ) // end sentinel
                  break;

                if( g_obuf[i+j+1] != '^' )
                  ch_name[j] = g_obuf[i+j+1];
                else
                  {
                  if( (g_obuf[0] - i - 2) < 7 )
                    break;

                  memmove( exp_date, &g_obuf[i+j+2], sizeof(exp_date) );
                  memmove( service_code, &g_obuf[i+j+6], sizeof(service_code) );
                  break;
                  }
                }

             if( j == 26 )
               {
               if( (g_obuf[0] - i - 2) < 7 )
                 break;

               if( ((g_obuf[i+j+2] >= 0x30) && (g_obuf[i+j+2] <= 0x39)) &&
                   ((g_obuf[i+j+3] >= 0x30) && (g_obuf[i+j+3] <= 0x39)) &&
                   ((g_obuf[i+j+4] >= 0x30) && (g_obuf[i+j+4] <= 0x39)) &&
                   ((g_obuf[i+j+5] >= 0x30) && (g_obuf[i+j+5] <= 0x39)) ) 
                   memmove( exp_date, &g_obuf[i+j+2], 4 );

               if( ((g_obuf[i+j+6] >= 0x30) && (g_obuf[i+j+6] <= 0x39)) &&
                   ((g_obuf[i+j+7] >= 0x30) && (g_obuf[i+j+7] <= 0x39)) &&
                   ((g_obuf[i+j+8] >= 0x30) && (g_obuf[i+j+8] <= 0x39)) )
                   memmove( service_code, &g_obuf[i+j+6], 3 );
               }

             break;
             }
           }
        }


      // Track 2 Data
      memset( g_obuf, 0x00, 250 );
      g_ibuf[0] = 0;
      g_ibuf[1] = isoTrack2;
      api_msr_getstring( g_dhn_msr, g_ibuf, g_obuf );

      if( g_obuf[0] < 16 )
        goto MSR_1000; // using Track 1 data


	// DEBUG for 19 PAN digits
	// ;4386160000080307123=22011778450100000?
//	g_obuf[18]='1';
//	g_obuf[19]='2';
//	g_obuf[20]='3';
//	g_obuf[21]='=';

      // PAN
      pan_len = 0;
      memset( pan, 0x20, sizeof(pan) );
      for( i=2; i<g_obuf[0]; i++ )
         {
         if( g_obuf[i] != '=' )
           {
           pan[i-2] = g_obuf[i];
           pan_len++;
           if( pan_len > 19 )
             break;
           }
         else
           {
           if( (g_obuf[0] - i - 1) < 7 )
             break;

           // expiry date
           if( ((g_obuf[i+1] >= 0x30) && (g_obuf[i+1] <= 0x39)) &&
               ((g_obuf[i+2] >= 0x30) && (g_obuf[i+2] <= 0x39)) &&
               ((g_obuf[i+3] >= 0x30) && (g_obuf[i+3] <= 0x39)) &&
               ((g_obuf[i+4] >= 0x30) && (g_obuf[i+4] <= 0x39)) )
               memmove( exp_date, &g_obuf[i+1], 4 );

           // service code
           if( ((g_obuf[i+5] >= 0x30) && (g_obuf[i+5] <= 0x39)) &&
               ((g_obuf[i+5] >= 0x30) && (g_obuf[i+6] <= 0x39)) &&
               ((g_obuf[i+7] >= 0x30) && (g_obuf[i+7] <= 0x39)) )
               memmove( service_code, &g_obuf[i+5], 3 );

           break;
           }
         }

MSR_1000:
      // save cardholder name
      iLen = 26;
      api_emv_PutDataElement( DE_ICC, ADDR_ICC_CH_NAME, 2, (UCHAR *)&iLen );
      api_emv_PutDataElement( DE_ICC, ADDR_ICC_CH_NAME+2, 26, ch_name );

      // save PAN
      memmove( &buf2[1], pan, pan_len );
      buf2[0] = pan_len;
      TL_asc2bcd( 10, buf, buf2 );
      
      for( i=0; i<10; i++ )
         {
         if( buf[i] != 0 )
           break;
         }
      iLen = 10 - i;
      
      if( i == 0 )	// trim the leading zero if available
      	{
          
        EMVDC_left_shift_str( buf, buf, 10, 4 );
        
        buf[9] = (buf[9] & 0xf0) | 0x0f;
//      UTIL_DumpHexData( 0, 0, 10, buf );
        }
      
        printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
      api_emv_PutDataElement( DE_ICC, ADDR_ICC_AP_PAN, 2, (UCHAR *)&iLen );
      api_emv_PutDataElement_SRED( DE_ICC, ADDR_ICC_AP_PAN+2, iLen, &buf[i] );
printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
      // save Exp Date
      buf2[0] = 4;
      memmove( &buf2[1], exp_date, 4 );
      TL_asc2bcd( 2, buf, buf2 );
      i = buf[0];
      buf[0] = buf[1];
      buf[1] = i;
      iLen = 2;
      api_emv_PutDataElement( DE_ICC, ADDR_ICC_AP_EXPIRE_DATE, 1, (UCHAR *)&iLen );
      api_emv_PutDataElement( DE_ICC, ADDR_ICC_AP_EXPIRE_DATE+2, 2, buf );

    //   UI_ClearScreen();

      // show Cardholder Name
    //   UI_PutStr( 0, 0, FONT0, 20+1, ch_name );
    //   UI_PutStr( 1, 0, FONT0, 6-1, &ch_name[20+1] );

      // show PAN
    //   UI_PutStr( 2, 0, FONT0, 19, pan );

      // show Expiry Date
    //   g_ibuf[0] = exp_date[2];
    //   g_ibuf[1] = exp_date[3];
    //   g_ibuf[2] = '/';
    //   g_ibuf[3] = exp_date[0];
    //   g_ibuf[4] = exp_date[1];
    //   UI_PutStr( 3, 0, FONT0, 5, g_ibuf );

      // show Service Code
    //   UI_PutStr( 3, 7, FONT0, 3, service_code );

    //   UI_WaitKey();

      fMSR = FALSE;
#if	0
      if( (service_code[0] == '2') || (service_code[0] == '6') )
        {
        if( mode == PEM_MSR )
          {
          DISP_STD_use_chip_reader();
          TL_WaitTimeAndKey( 150 );
          }
        else
          return( fMSR );
        }
      else
        {
        // --- for EMV L2 Test Only ---
//	DISP_out_of_service();

	// --- for PCI Test Only ---
	fMSR = TRUE;
        }
#endif
	fMSR = TRUE;	// debug only
	
      api_msr_close( g_dhn_msr );
//    TL_WaitTimeAndKey( 150 );      // wait 1.5 sec
//    UI_WaitKey();

      return( fMSR );
}

// ---------------------------------------------------------------------------
// FUNCTION: get amount.
// INPUT   : none.
// OUTPUT  : amt   - amount entered. (L-V[ANS])
// RETURN  : TRUE  - OK.
//           FALSE - aborted.
// ---------------------------------------------------------------------------
UCHAR FB_GetAmount( UCHAR *amt )
{
UCHAR i;
UCHAR buffer[16];
UCHAR digits;
UCHAR len;

        UI_ClearScreen();
GET_AMT:
     // DISP_STD_enter_amount();
        DISP_GRP_enter_amount();

        if( g_term_decimal_point == 0 )
          digits = 10 - g_term_tx_exp;
        else
          digits = 10;

        if( TL_GetNumKey( 0, NUM_TYPE_DEC, '0', FONT0, 3, digits, buffer ) == FALSE )
          return( FALSE ); // aborted
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

	// output amount string
	memmove( amt, buffer, buffer[0]+1 ); // L-V[ANS]

        // backup current tx amount in 'n12' format (including integer & decimal)
        TL_asc2bcd( AMT_INT_SIZE+AMT_DEC_SIZE, g_term_tx_amt, buffer ); // integer+decimal part

        // range check, max. amount = "42 94 96 72 95" (0xFF FF FF FF)
        if( TL_VerifyTransAmount( g_term_tx_amt ) == FALSE )
          {
          // display "NOT ACCEPTED"
          printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
          DISP_STD_not_accepted();
          TL_WaitTime(100);
          goto GET_AMT;
          }

        // save transaction amount
        apk_WriteRamDataTERM( ADDR_TERM_AMT_AUTH_N, 6, g_term_tx_amt );
        apk_WriteRamDataTERM( ADDR_TERM_TX_AMT, 6, g_term_tx_amt );

        TL_bcd2hex( 5, &g_term_tx_amt[1], buffer ); // convert to binary format
        apk_WriteRamDataTERM( ADDR_TERM_AMT_AUTH_B, 4, buffer );

        return( TRUE );
}

// ---------------------------------------------------------------------------
// FUNCTION: fallback transaction (switch transaction from icc to msr).
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : emvOK
//           emvFailed
// ---------------------------------------------------------------------------
UCHAR EMVDC_FallBackTransaction( void )
{
UCHAR buf[16];
UCHAR buf2[16];
UCHAR arc[2];
UCHAR iad[34];
UINT  ist;
UINT  iLen;

      // clear all ICC-Related data elements to 0
      iLen = ADDR_ICC_DE_END;
      iLen -= ADDR_ICC_DE_START;
      api_emv_ClrDataElement( DE_ICC, ADDR_ICC_DE_START, iLen, 0 );

      // set PEM
      EMVDC_SetupPosEntryMode( PEM_FALLBACK );

      g_dhn_msr = api_msr_open( isoTrack1 + isoTrack2 ); // MSR track 1 & 2

      DISP_swipe_card();

      while(1)
           {
           // check MSR
           api_msr_status( g_dhn_msr, 0, buf );
           if( (buf[0] == msrSwiped) && ((buf[1] == msrDataOK) || (buf[2] == msrDataOK)) )
             {
             EMVDC_MsrProcessing( PEM_FALLBACK, buf );
             if( FB_GetAmount(buf2) == FALSE )
               {
               api_msr_close( g_dhn_msr );
               return( emvFailed );
               }

             // setup transaction Date & Time
             TL_GetDateTime( &buf[1] );

             buf[0]=6;
             TL_asc2bcd( 3, buf2, buf );     // convert to BCD, buf2: YYMMDD
             apk_WriteRamDataTERM( ADDR_TERM_TX_DATE, 3, buf2 );

             buf[6]=6;
             TL_asc2bcd( 3, buf2, &buf[6] ); // convert to BCD, buf2: HHMMSS
             apk_WriteRamDataTERM( ADDR_TERM_TX_TIME, 3, buf2 );

             // setup transaction Sequence Counter
             api_emv_IncTransSequenceCounter();

             // must go online
             if( EMVDC_OnlineProcessing( MSGID_FINANCIAL_REQ, 0x80, arc, iad, (UINT *)&ist ) == ONL_Completed )
               {
               DISP_STD_approved();
               UI_WaitKey();

               buf[0] = 0x1E;
               buf[1] = 0x00;
               buf[2] = 0x00;
               api_emv_PutDataElement( DE_TERM, ADDR_TERM_CVMR, 3, buf );

               g_term_ARC[0] = arc[0];
               g_term_ARC[0] = arc[1];
               api_emv_PutDataElement( DE_TERM, ADDR_TERM_ARC, 2, g_term_ARC );
               }
             else // unable to go online
               {
               DISP_STD_declined();
               UI_WaitKey();
               }

             api_msr_close( g_dhn_msr );
             return( emvOK ); // done
             }

           // check KEY
           if( api_kbd_status(g_dhn_kbd, buf) == apiReady ) // key pressed?
             {
             if( UI_WaitKey() == 'x' )   // cancel?
               {
               api_msr_close( g_dhn_msr );
               return( emvFailed );
               }
             }

           } // while(1)
}

// ---------------------------------------------------------------------------
// FUNCTION: send "Financial Transaction Request" to HOST Simulator.
// INPUT   : ct  - the crytogram type. (eg. ARQC)
//           tt  - transaction type. (sale or reversal)
// OUTPUT  : none.
// RETURN  : TRUE
//           FALSE
// ---------------------------------------------------------------------------
UCHAR OP_SendFinancialTxRequest( UCHAR tt, UCHAR ct )
{
UINT  iLen;
UINT  iDataLen;
UINT  iIsrLen;
UCHAR len;
UCHAR cnt;
UCHAR isr[2+5*16]; // 2L-V

      iDataLen = 0;

      // Terminal Identifier
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TID, 8, &g_ibuf[TX_TID] );
      iDataLen += 8;

      // Transaction Date
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TX_DATE, 3, &g_ibuf[TX_DATE] );
      iDataLen += 3;

      // Transaction Time
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TX_TIME, 3, &g_ibuf[TX_TIME] );
      iDataLen += 3;

      // ARQC
      g_ibuf[TX_ARQC] = ct;
      iDataLen += 1;

      // Transaction Type
      if( tt == TT_GOODS_AND_SERVICE )
        {
        if( g_pre_auth == FALSE )
          api_emv_GetDataElement( DE_TERM, ADDR_TERM_TX_TYPE, 1, &g_ibuf[TX_TYPE] );
        else
          g_ibuf[TX_TYPE] = TT_PRE_AUTH; // TA only
        }
      else
        g_ibuf[TX_TYPE] = tt;
      iDataLen += 1;

      // Amount, Authorized
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_AMT_AUTH_N, 6, &g_ibuf[TX_AMT_AUTH] );
      iDataLen += 6;

      // Amount, Other
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_AMT_OTHER_N, 6, &g_ibuf[TX_AMT_OTHER] );
      iDataLen += 6;

      // Transaction Amount
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TX_AMT, 6, &g_ibuf[TX_AMT] );
      iDataLen += 6;

      // Terminal Currency Code
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TX_CC, 2, &g_ibuf[TX_CC] );
      iDataLen += 2;

      // Application PAN
      memset( &g_ibuf[2+TX_PAN], 0xff, 10 );
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_AP_PAN, 2, (UCHAR *)&iLen );
//    api_emv_GetDataElement( DE_ICC, ADDR_ICC_AP_PAN+2, iLen, &g_ibuf[TX_PAN] );
      api_emv_GetDataElement_SRED( DE_ICC, ADDR_ICC_AP_PAN+2, iLen, &g_ibuf[TX_PAN] );
      iDataLen += 10;

      // Application PAN Sequence Number
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_AP_PAN_SN+2, 1, &g_ibuf[TX_PAN_SN] );
      iDataLen += 1;

      // Application Interchange Profile (b2)
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_AIP, 2, &g_ibuf[TX_AIP] );
      iDataLen += 2;

      // Application Transaction Counter (b2) - ATC
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_ATC+2, 2, &g_ibuf[TX_ATC] );
      iDataLen += 2;

      // Terminal Verification Results
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TVR, 5, &g_ibuf[TX_TVR] );
      iDataLen += 5;

      // Terminal Country Code
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_CNTR_CODE, 2, &g_ibuf[TX_CNTR_CODE] );
      iDataLen += 2;

      // Unpredictable Number
      memset( &g_ibuf[TX_UPD_NBR], 0x00, 6 );
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_UPD_NBR_LEN, 1, (UCHAR *)&len );
      if( len != 0 )
        {
        g_ibuf[TX_UPD_NBR+0] = 4;
        g_ibuf[TX_UPD_NBR+1] = 0;
        api_emv_GetDataElement( DE_TERM, ADDR_TERM_UPD_NBR, 4, &g_ibuf[TX_UPD_NBR+2] );
        }
      iDataLen += 6;

      // Issuer Application Data - IAD
      memset( &g_ibuf[TX_ISU_AP_DATA], 0x00, 34 );
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_ISU_AP_DATA, 2, (UCHAR *)&iLen );
      if( iLen != 0 )
        {
        g_ibuf[TX_ISU_AP_DATA+0] = iLen & 0x00ff;
        g_ibuf[TX_ISU_AP_DATA+1] = (iLen & 0xff00) >> 8;
        api_emv_GetDataElement( DE_ICC, ADDR_ICC_ISU_AP_DATA+2, iLen, &g_ibuf[TX_ISU_AP_DATA+2] );
        }
      iDataLen += 34;

      // Cryptogram Information Data (b1) - CID
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_CID+2, 1, &g_ibuf[TX_CID] );
      iDataLen += 1;

      // Application Cryptogram (b8)
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_AC+2, 8, &g_ibuf[TX_AC] );
      iDataLen += 8;

      // POS Entry Mode (n2)
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_PEM, 1, &g_ibuf[TX_PEM] );
      iDataLen += 1;

      // Issuer Script Results for Reversal if present (2L-V[5*n])
      g_ibuf[TX_REV_ISR+0] = 0;
      g_ibuf[TX_REV_ISR+1] = 0;
      iIsrLen = 0;
      if( tt == TT_FINANCIAL_REVERSAL )
        {
        apk_ReadRamDataTERM( ADDR_TERM_ISR, 1, (UCHAR *)&cnt );
        if( cnt != 0 )
          {
          iLen = cnt*5;
          apk_ReadRamDataTERM( ADDR_TERM_ISR+1, iLen, &isr[2] );
          isr[0] = iLen & 0x00ff;
          isr[1] = (iLen & 0xff00) >> 8;
          iDataLen += iLen;
          iIsrLen = iLen;
          memmove( &g_ibuf[TX_REV_ISR], isr, iLen+2 );
          }
        }
      iDataLen += 2;
      iIsrLen += 2;
printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
      // Enciphered PIN Data (RFU)
      g_ibuf[TX_REV_ISR+iIsrLen+0] = 0;
      g_ibuf[TX_REV_ISR+iIsrLen+1] = 0;
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_EPIN_DATA, 2, (UCHAR *)&iLen );
      printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
      if( iLen != 0 )
        {
//      api_emv_GetDataElement( DE_TERM, ADDR_TERM_EPIN_DATA, 10, &g_ibuf[TX_REV_ISR+iIsrLen] );
printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
        api_emv_GetDataElement( DE_TERM, ADDR_TERM_EPIN_DATA, iLen+2, &g_ibuf[TX_REV_ISR+iIsrLen] );	// 2019-04-26
        printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
        iDataLen += iLen;
        iIsrLen += iLen;
        }
      iDataLen += 2;
      iIsrLen += 2;
      printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
      // TSI (2 bytes) PATCH: PBOC2.0, 2006-02-09
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TSI, 2, &g_ibuf[TX_REV_ISR+iIsrLen] );
      iDataLen += 2;
printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
      // KSN (if available)
      g_ibuf[TX_REV_ISR+iIsrLen+2] = 0;
      g_ibuf[TX_REV_ISR+iIsrLen+3] = 0;
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_KSN, 2, (UCHAR *)&iLen );
      if( iLen != 0 )
        {
//        g_ibuf[TX_REV_ISR+iIsrLen+4] = 10;
//        g_ibuf[TX_REV_ISR+iIsrLen+5] = 0;
//  
//        api_emv_GetDataElement( DE_TERM, ADDR_TERM_KSN+2, iLen, &g_ibuf[TX_REV_ISR+iIsrLen+4] );
printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
	api_emv_GetDataElement( DE_TERM, ADDR_TERM_KSN, iLen+2, &g_ibuf[TX_REV_ISR+iIsrLen+2] );
  printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
        iDataLen += iLen;
        }
      iDataLen += 2;
      printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
      	iDataLen += 1;	// dummy

//    if( iLen == 0 )
//      g_ibuf[2] = 0; // set final block flag
//    else
//      g_ibuf[2] = 1; // set chained block flag


      // ------------------------------------------------
      g_ibuf[2] = 0;   // set final block flag
      iDataLen += 1;

      g_ibuf[0] = iDataLen & 0x00ff;
      g_ibuf[1] = (iDataLen & 0xff00) >> 8;
printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
      if( TL_TransmitAUX( g_ibuf ) == FALSE )
        return( FALSE );
      else
        {
        if( g_ibuf[2] == 0 ) // check PF chained flag
          {
//        if( tt == TT_FINANCIAL_REVERSAL )
//          apk_ClearRamDataTERM( ADDR_TERM_ISR, 1, 0x00 ); // 2006-10-11, removed for Last BDC Upload function

          return( TRUE );
          }
        else
          {
          // Send Enciphered PIN Data
          // --- TO BE IMPLEMENTED ---
          return( TRUE );
          }
        }
}

// ---------------------------------------------------------------------------
// FUNCTION: send "Load CA Public Key" to HOST Simulator.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : TRUE
//           FALSE
// ---------------------------------------------------------------------------
UCHAR OP_SendLoadPublicKeyRequest( void )
{
UINT  iLen;
UINT  iDataLen;
UCHAR len;

      iDataLen = 0;

      // Terminal Identifier
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TID, 8, &g_ibuf[TX_TID] );
      iDataLen += 8;

      // Transaction Date
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TX_DATE, 3, &g_ibuf[TX_DATE] );
      iDataLen += 3;

      // Transaction Time
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TX_TIME, 3, &g_ibuf[TX_TIME] );
      iDataLen += 3;

      // ARQC
      g_ibuf[TX_ARQC] = 0x00;
      iDataLen += 1;

      // Transaction Type
      g_ibuf[TX_TYPE] = TT_LOAD_CAPK;
      iDataLen += 1;

      // send reqeust
      g_ibuf[2] = 0; // set final block flag

      iDataLen += 1;
      g_ibuf[0] = iDataLen & 0x00ff;
      g_ibuf[1] = (iDataLen & 0xff00) >> 8;

      if( TL_TransmitAUX( g_ibuf ) == FALSE )
        return( FALSE );
      else
        return( TRUE );
}

// ---------------------------------------------------------------------------
// FUNCTION: send "Financial Confirmation" to HOST Simulator.
// INPUT   : ct  - the crytogram type. (eg. ARQC)
//           isr - 2L-V, the issuer script results.
// OUTPUT  : none.
// RETURN  : TRUE
//           FALSE
// ---------------------------------------------------------------------------
UCHAR OP_SendFinancialConfirm( UCHAR ct, UCHAR *isr )
{
UINT  iLen;
UINT  iDataLen;
UCHAR len;

      iDataLen = 0;

      // Terminal Identifier
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TID, 8, &g_ibuf[TX_TID] );
      iDataLen += 8;

      // Transaction Date
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TX_DATE, 3, &g_ibuf[TX_DATE] );
      iDataLen += 3;

      // Transaction Time
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TX_TIME, 3, &g_ibuf[TX_TIME] );
      iDataLen += 3;

      // ARQC
      g_ibuf[TX_ARQC] = ct;
      iDataLen += 1;

      // Transaction Type
      g_ibuf[TX_TYPE] = TT_FINANCIAL_CONFIRM;
      iDataLen += 1;

      // Issuer Script Results (2L-V[5*n])
      g_ibuf[TX_ISR+0] = isr[0];
      g_ibuf[TX_ISR+1] = isr[1];
      iDataLen += (2 + isr[0]);
      memmove( &g_ibuf[TX_ISR], isr, isr[0]+2 );

      // PATCH: PBOC2.0, 2006-02-11, EPIN
      iLen = isr[0] + isr[1]*256;
      g_ibuf[TX_ISR+iLen+2] = 0;
      g_ibuf[TX_ISR+iLen+3] = 0;
      iDataLen += 2;

      // PATCH: PBOC2.0, 2006-02-11, TSI
      g_ibuf[TX_ISR+iLen+4] = g_term_TSI[0];
      g_ibuf[TX_ISR+iLen+5] = g_term_TSI[1];
      iDataLen += 2;

      g_ibuf[2] = 0; // set final block flag

      iDataLen += 1;
      g_ibuf[0] = iDataLen & 0x00ff;
      g_ibuf[1] = (iDataLen & 0xff00) >> 8;

      if( TL_TransmitAUX( g_ibuf ) == FALSE )
        return( FALSE );
      else
        return( TRUE );
}

// ---------------------------------------------------------------------------
// FUNCTION: send "Financial Advice" to HOST Simulator.
// INPUT   : log - the offline-approved transaction log.
// OUTPUT  : none.
// RETURN  : TRUE
//           FALSE
// ---------------------------------------------------------------------------
UCHAR OP_SendFinancialUpload( UCHAR *log )
{
UINT  iLen;
UINT  iDataLen;
UCHAR len;

      iDataLen = 0;

      // Terminal Identifier
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TID, 8, &g_ibuf[TX_TID] );
      iDataLen += 8;

      // Transaction Date
      memmove( &g_ibuf[TX_DATE], &log[LOG_TX_DATE], 3 );
      iDataLen += 3;

      // Transaction Time
      memmove( &g_ibuf[TX_TIME], &log[LOG_TX_TIME], 3 );
      iDataLen += 3;

      // ARQC
      g_ibuf[TX_ARQC] = 0x40;
      iDataLen += 1;

      // Transaction Type
      g_ibuf[TX_TYPE] = TT_FINANCIAL_ADVICE;
      iDataLen += 1;

      // Amount, Authorized
      memmove( &g_ibuf[TX_AMT_AUTH], &log[LOG_TX_AMT], 6 );
      iDataLen += 6;

      // Amount, Other
      memset( &g_ibuf[TX_AMT_OTHER], 0x00, 6 );
      iDataLen += 6;

      // Transaction Amount
      memmove( &g_ibuf[TX_AMT], &log[LOG_TX_AMT], 6 );
      iDataLen += 6;

      // Terminal Currency Code
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TX_CC, 2, &g_ibuf[TX_CC] );
      iDataLen += 2;

      // Application PAN
      memmove( &g_ibuf[TX_PAN], &log[LOG_AP_PAN], 10 );
      iDataLen += 10;

      // Application PAN Sequence Number
      memmove( &g_ibuf[TX_PAN_SN], &log[LOG_AP_PAN_SN], 1 );
      iDataLen += 1;

      g_ibuf[2] = 0; // set final block flag

      iDataLen += 1;
      g_ibuf[0] = iDataLen & 0x00ff;
      g_ibuf[1] = (iDataLen & 0xff00) >> 8;

      if( TL_TransmitAUX( g_ibuf ) == FALSE )
        return( FALSE );
      else
        return( TRUE );
}

// ---------------------------------------------------------------------------
// FUNCTION: send "Financial Advice" to HOST Simulator.
// INPUT   : log - the offline-approved transaction log.
// OUTPUT  : none.
// RETURN  : TRUE
//           FALSE
// ---------------------------------------------------------------------------
UCHAR OP_SendFinancialUpload2( UCHAR *log )
{
UINT  iLen;
UINT  iDataLen;
UINT  iIsrLen;
UCHAR len;
UCHAR cnt;
UCHAR isr[2+5*16]; // 2L-V

      iDataLen = 0;

      // Terminal Identifier
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TID, 8, &g_ibuf[TX_TID] );
      iDataLen += 8;

      // Transaction Date
      memmove( &g_ibuf[TX_DATE], &log[LOG_TX_DATE], 3 );
      iDataLen += 3;

      // Transaction Time
      memmove( &g_ibuf[TX_TIME], &log[LOG_TX_TIME], 3 );
      iDataLen += 3;

      // ARQC
      g_ibuf[TX_ARQC] = log[LOG_ARQC];
      iDataLen += 1;

      // Transaction Type
      g_ibuf[TX_TYPE] = TT_BDC;
//    g_ibuf[TX_TYPE] = TT_GOODS_AND_SERVICE;
      iDataLen += 1;

      // Amount, Authorized
      memmove( &g_ibuf[TX_AMT_AUTH], &log[LOG_TX_AMT], 6 );
      iDataLen += 6;

      // Amount, Other
      memset( &g_ibuf[TX_AMT_OTHER], 0x00, 6 );
      iDataLen += 6;

      // Transaction Amount
      memmove( &g_ibuf[TX_AMT], &log[LOG_TX_AMT], 6 );
      iDataLen += 6;

      // Terminal Currency Code
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TX_CC, 2, &g_ibuf[TX_CC] );
      iDataLen += 2;

      // Application PAN
      memmove( &g_ibuf[TX_PAN], &log[LOG_AP_PAN], 10 );
      iDataLen += 10;

      // Application PAN Sequence Number
      memmove( &g_ibuf[TX_PAN_SN], &log[LOG_AP_PAN_SN], 1 );
      iDataLen += 1;

      // Application Interchange Profile (b2)
      memmove( &g_ibuf[TX_AIP], &log[LOG_AIP], 2 );
      iDataLen += 2;

      // Application Transaction Counter (b2) - ATC
      memmove( &g_ibuf[TX_ATC], &log[LOG_ATC], 2 );
      iDataLen += 2;

      // Terminal Verification Results
      memmove( &g_ibuf[TX_TVR], &log[LOG_TVR], 5 );
      iDataLen += 5;

      // Terminal Country Code
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_CNTR_CODE, 2, &g_ibuf[TX_CNTR_CODE] );
      iDataLen += 2;

      // Unpredictable Number
      g_ibuf[TX_UPD_NBR+0] = 4;
      g_ibuf[TX_UPD_NBR+1] = 0;
      memmove( &g_ibuf[TX_UPD_NBR+2], &log[LOG_UPD_NBR], 4 );
      iDataLen += 6;

      // Issuer Application Data - IAD
      memmove( &g_ibuf[TX_ISU_AP_DATA], &log[LOG_ISU_AP_DATA], 34 );
      iDataLen += 34;

      // Cryptogram Information Data (b1) - CID
      memmove( &g_ibuf[TX_CID], &log[LOG_CID], 1 );
      iDataLen += 1;

      // Application Cryptogram (b8)
//    memset( &g_ibuf[TX_AC], 0x00, 8 );
      memmove( &g_ibuf[TX_AC], &log[LOG_AC], 8 );
      iDataLen += 8;

      // POS Entry Mode (n2)
      memmove( &g_ibuf[TX_PEM], &log[LOG_PEM], 1 );
      iDataLen += 1;

      // Issuer Script Results for Reversal if present (2L-V[5*n])

//  api_emv_GetDataElement( DE_TERM, ADDR_TERM_ISR, 21, g_obuf );
//  TL_DumpHexData(0,0,21, g_obuf);
//  TL_DumpHexData(0,0,21, &log[LOG_ISR]);

      g_ibuf[TX_REV_ISR+0] = 0;
      g_ibuf[TX_REV_ISR+1] = 0;
      iIsrLen = 0;

      cnt = log[LOG_ISR];
      if( cnt != 0 )
        {
        iLen = cnt*5;
        memmove( &isr[2], &log[LOG_ISR+1], iLen );
        isr[0] = iLen & 0x00ff;
        isr[1] = (iLen & 0xff00) >> 8;
        iDataLen += iLen;
        iIsrLen = iLen;
        memmove( &g_ibuf[TX_REV_ISR], isr, iLen+2 );
        }
      iDataLen += 2;
      iIsrLen += 2;

      // Enciphered PIN Data (RFU)
      g_ibuf[TX_REV_ISR+iIsrLen+0] = 0;
      g_ibuf[TX_REV_ISR+iIsrLen+1] = 0;
      iDataLen += 2;
      iIsrLen += 2;

      // TSI (2 bytes) PATCH: PBOC2.0, 2006-02-09
//    g_ibuf[TX_REV_ISR+iIsrLen+0] = 0;
//    g_ibuf[TX_REV_ISR+iIsrLen+1] = 0;

      memmove( &g_ibuf[TX_REV_ISR+iIsrLen], &log[LOG_TSI], 2 );
      iDataLen += 2;

      // ------------------------------------------------
      g_ibuf[2] = 0;   // set final block flag
      iDataLen += 1;

      g_ibuf[0] = iDataLen & 0x00ff;
      g_ibuf[1] = (iDataLen & 0xff00) >> 8;


//  TL_DumpHexData(0,0,g_ibuf[0]+g_ibuf[1]*256+2, g_ibuf);

      if( TL_TransmitAUX( g_ibuf ) == FALSE )
        return( FALSE );
      else
        return( TRUE );
}

// ---------------------------------------------------------------------------
// FUNCTION: send "Financial Advice" to HOST Simulator.
// INPUT   : log - the settlement data. (currently only Total Net Amount)
// OUTPUT  : none.
// RETURN  : TRUE
//           FALSE
// ---------------------------------------------------------------------------
UCHAR OP_SendFinancialSettle( UCHAR *log )
{
UINT  iLen;
UINT  iDataLen;
UCHAR len;
UCHAR buf1[32];
UCHAR buf2[32];

      iDataLen = 0;

      // Terminal Identifier
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TID, 8, &g_ibuf[TX_TID] );
      iDataLen += 8;

      // Transaction Date
      TL_GetDateTime( &buf1[1] );
      buf1[0]=6;
      TL_asc2bcd( 3, buf2, buf1 );     // convert to BCD, buf2: YYMMDD
      memmove( &g_ibuf[TX_DATE], buf2, 3 );
      iDataLen += 3;

      // Transaction Time
      buf1[6]=6;
      TL_asc2bcd( 3, buf2, &buf1[6] ); // convert to BCD, buf2: HHMMSS
      memmove( &g_ibuf[TX_TIME], buf2, 3 );
      iDataLen += 3;

      // ARQC
      g_ibuf[TX_ARQC] = 0x80;
      iDataLen += 1;

      // Transaction Type
      g_ibuf[TX_TYPE] = TT_SETTLEMENT;
      iDataLen += 1;

      // Amount, Authorized
      memmove( &g_ibuf[TX_AMT_AUTH], log, 6 );
      iDataLen += 6;

      // Amount, Other
      memset( &g_ibuf[TX_AMT_OTHER], 0x00, 6 );
      iDataLen += 6;

      // Transaction Amount
      memmove( &g_ibuf[TX_AMT], log, 6 );
      iDataLen += 6;

      g_ibuf[2] = 0; // set final block flag

      iDataLen += 1;
      g_ibuf[0] = iDataLen & 0x00ff;
      g_ibuf[1] = (iDataLen & 0xff00) >> 8;

      if( TL_TransmitAUX( g_ibuf ) == FALSE )
        return( FALSE );
      else
        return( TRUE );
}

// ---------------------------------------------------------------------------
// FUNCTION: to signal HOST that terminal is ready to receive next data block.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : TRUE  - OK.
//           FALSE - device error.
// ---------------------------------------------------------------------------
UCHAR OP_SendReadyBlock( void )
{
UCHAR buf[8];

      buf[0] = 0x02;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x30;
      return( TL_TransmitAUX( buf ) );
}

// ---------------------------------------------------------------------------
// FUNCTION: receive "Financial Transaction Response" from HOST Simulator.
// INPUT   : none.
// OUTPUT  : data - 2L-V, the response data received.
// RETURN  : TRUE  - data recevied OK.
//           FALSE - no data received or device error.
// ---------------------------------------------------------------------------
UCHAR OP_ReceiveFinancialTxResponse( UCHAR *data )
{
      return( TL_ReceiveAUX( data ) );
}

// ---------------------------------------------------------------------------
// FUNCTION: receive "Financial Confirmation Response" from HOST Simulator.
// INPUT   : none.
// OUTPUT  : data - 2L-V, the response data received.
// RETURN  : TRUE  - data recevied OK.
//           FALSE - no data received or device error.
// ---------------------------------------------------------------------------
UCHAR OP_ReceiveFinancialConfirmResponse( UCHAR *data )
{
      return( TL_ReceiveAUX( data ) );
}

// ---------------------------------------------------------------------------
// FUNCTION: receive "Issuer-to-Card Scripts" from HOST simulator.
// INPUT   : none.
// OUTPUT  : data  - 2L-[T-L-V], issuer scripts template.
// RETURN  : TRUE  - data recevied OK.
//           FALSE - no data received or device error.
// NOTE    : the script template may be more than one block.
// ---------------------------------------------------------------------------
//UCHAR OP_ReceiveIssuerScripts( UCHAR *data )
//{
//UCHAR buf[255];
//UINT  iLen;
//UINT  iDataLen;
//
//      UI_WaitKey();
//
//      iDataLen = 0;
//      do{
//        if( OP_SendReadyBlock() == FALSE )
//          return( FALSE );
//        else
//          {
//          if( OP_ReceiveFinancialTxResponse( buf ) == FALSE ) // 2L-PF-DATA
//            return( FALSE );
//
//          iLen = buf[1]*256 + buf[0] - 1;
//          memmove( &data[2+iDataLen], &buf[3], iLen ); // concatenation of scripts
//          iDataLen += iLen;
//          }
//        } while( buf[2] == 1 ); // continue if chained blocks
//
//      data[0] = iDataLen & 0x00ff;
//      data[1] = (iDataLen & 0xff00) >> 8;
//
////    TL_DumpHexData(0,0, iDataLen+2, data);
////    for(;;);
//
//      return( TRUE );
//}

// ---------------------------------------------------------------------------
// FUNCTION: receive "Issuer-to-Card Scripts" from HOST simulator.
//           store all scripts at "ADDR_ISU_SCRIPT_TEMP" pool.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : TRUE  - data recevied OK.
//           FALSE - no data received or device error.
// NOTE    : the script template may be more than one block.
// ---------------------------------------------------------------------------
UCHAR OP_ReceiveIssuerScripts2( void )
{
UCHAR buf[255];
UINT  iLen;
UINT  iDataLen;
UINT  iAddr;

//    UI_WaitKey();

      iAddr = ADDR_ISU_SCRIPT_TEMP + 2;
      iDataLen = 0;
      do{
        if( OP_SendReadyBlock() == FALSE )
          return( FALSE );
        else
          {
          if( OP_ReceiveFinancialTxResponse( buf ) == FALSE ) // 2L-PF-DATA
            return( FALSE );

          // concatenate the scripts
          iLen = buf[1]*256 + buf[0] - 1;
          api_emv_PutDataElement( DE_TERM, iAddr, iLen, &buf[3] );
          iDataLen += iLen;
          iAddr += iLen;
          }
        } while( buf[2] == 1 ); // continue if chained blocks

      // update the final length of the script template
      api_emv_PutDataElement( DE_TERM, ADDR_ISU_SCRIPT_TEMP, 2, (UCHAR *)&iDataLen );

//    api_emv_GetDataElement( DE_TERM, ADDR_ISU_SCRIPT_TEMP, iDataLen+2, buf );
//    TL_DumpHexData(0,0, iDataLen+2, buf);
//    for(;;);

      return( TRUE );
}

// ---------------------------------------------------------------------------
// FUNCTION: load CA public key from host simulator.
// INPUT   : msgid - message id.
// OUTPUT  : none.
// RETURN  : ONL_Completed
//           ONL_Failed
// ---------------------------------------------------------------------------
UCHAR EMVDC_LoadCaPublicKey( void )
{
UCHAR i;
UCHAR buf[250];
UCHAR buf2[32];
UINT  iLen;
UCHAR modlen;
UCHAR result;

      UI_ClearScreen();

      if( TL_OpenAUX() == FALSE )
        {
        DISP_STD_processing_error();
        return( ONL_Failed ); // device error
        }

      // clear all ICC-Related data elements to 0
      iLen = ADDR_ICC_DE_END;
      iLen -= ADDR_ICC_DE_START;
      api_emv_ClrDataElement( DE_ICC, ADDR_ICC_DE_START, iLen, 0 );

      // setup transaction Date & Time
      TL_GetDateTime( &buf[1] );

      buf[0]=6;
      TL_asc2bcd( 3, buf2, buf );     // convert to BCD, buf2: YYMMDD
      apk_WriteRamDataTERM( ADDR_TERM_TX_DATE, 3, buf2 );

      buf[6]=6;
      TL_asc2bcd( 3, buf2, &buf[6] ); // convert to BCD, buf2: HHMMSS
      apk_WriteRamDataTERM( ADDR_TERM_TX_TIME, 3, buf2 );

      DISP_processing();

      // --------------------------------------------------------------------
      // send request to HOST Simulator (2L-PF-DATA[])
      // --------------------------------------------------------------------
      result = FALSE;

      if( OP_SendLoadPublicKeyRequest() == TRUE )
        {

        // ------------------------------------------------------------
        // OLD program
        // ------------------------------------------------------------
//      if( OP_ReceiveFinancialConfirmResponse( g_ibuf ) == TRUE )
//        {
//        TL_WaitTime(150);
//        UI_ClearScreen();
//
//        if( (g_ibuf[RX_ARC+0] == '0') && (g_ibuf[RX_ARC+1] == '0') )
//          DISP_load_key_ok();
//        else
//          DISP_load_key_failed();
//        }
//      else
//        return( ONL_Failed );

        // ------------------------------------------------------------
        // NEW program
        // ------------------------------------------------------------
        if( OP_ReceiveFinancialConfirmResponse( g_ibuf ) == TRUE )
          {
          memmove( buf2, &g_ibuf[RX_IAD], 22 ); // checksum 2L-V(20)

          if( g_ibuf[RX_PF] == 1 )
            {
            if( OP_ReceiveIssuerScripts2() == FALSE )
              {
              TL_CloseAUX();
              return( ONL_Failed );
              }

            // RID(5) INDEX(1) MOD_LEN(1) MOD(n) EXP_LEN(1) EXP(n)
            // Checksum = SHA1( RID+INDEX+MOD+EXP )
            api_emv_GetDataElement( DE_TERM, ADDR_ISU_SCRIPT_TEMP, 250, buf );

            memmove( &g_ibuf[0], &buf[2], 6 ); // RID(5)+INDEX(1)
            iLen = 6;
            modlen = buf[2+6];
            memmove( &g_ibuf[6], &buf[2+6+1], modlen ); // MOD(n)
            iLen += modlen;
            memmove( &g_ibuf[6+modlen], &buf[2+7+modlen+1], buf[2+7+modlen] ); // EXP(n)
            iLen += buf[2+7+modlen];

//          g_dhn_sam = api_emv_OpenSession( RSA_SAM ); // RSA SAM
            if( api_emv_ATR( g_dhn_sam, buf ) == emvOK )
              {
//              if( apk_HASH( SHA1, iLen, g_ibuf, buf ) == apkOK )
                {
                for( i=0; i<20; i++ )
                   {
                   if( buf2[i+2] != buf[i] )
                     goto LCAPK_EXIT;
                   }
                result = TRUE;
                }
              }
            }
          }
        }

LCAPK_EXIT:

#ifdef	USE_RSA_SAM
      api_emv_CleanSAM();              // garbage collection
#endif
      api_emv_Deactivate( g_dhn_sam ); // disable all SAM contacts
//    api_emv_CloseSession( g_dhn_sam );

      TL_CloseAUX();

      UI_ClearScreen();
      if( result == TRUE )
        {
        DISP_load_key_ok();
        result = ONL_Completed;
        }
      else
        {
        DISP_load_key_failed();
        result = ONL_Failed;
        }

      UI_WaitKey();

      return( result );
}

// ---------------------------------------------------------------------------
// FUNCTION: Reconciliation process.
// INPUT   : msgid - iso 8583 message type id.
//           log   - transaction log to be uploaded. (fixed length)
//                   BDC or settlement data.
// OUTPUT  : none.
// RETURN  : ONL_Completed
//           ONL_Failed
// ---------------------------------------------------------------------------
UCHAR EMVDC_OnlineReconciliation( UINT msgid, UCHAR *log )
{
UCHAR result;

      UI_ClearScreen();

//    if( TL_OpenAUX() == FALSE )
//      {
//      DISP_STD_processing_error();
//      return( ONL_Failed ); // device error
//      }

      // "PROCESSING..."
      DISP_processing();

      if( msgid == MSGID_FINANCIAL_ADVICE )
        result =  OP_SendFinancialUpload2( log );
      else
        result =  OP_SendFinancialSettle( log );

      if( result == TRUE )
        return( ONL_Completed );
      else
        return( ONL_Failed );
}

// ---------------------------------------------------------------------------
// FUNCTION: Online processing.
//           it is a CALL-BACK function used by EMV Kernel API.
// INPUT   : ct  - the crytogram type. (eg. ARQC)
// OUTPUT  : arc - an2,   authorization response code.
//           iad - 2L-V,  issuer authentication data.
//                 mandatory: 8 bytes containing the cryptogram.
//                 optional : 1~8 bytes are proprietary.
//           ist - 2L-[T-L-V], issuer scripts template address.
//                 T-L-(9F18-04-ScriptID[4])-[Commands], T=71 or 72
//                 Commands: [86-L1-CMD1][86-L2-CMD2][86-L3-CMD3]...
//                 The size of "ist" is variable.
//                 or
//                 "ist" is the address of scripts stored in SRAM page.
//                 Because the length of "Issuer Script Template" is variable,
//                 this function will put the template to "ADDR_ISU_SCRIPT_TEMP",
//                 to which the kernel API shall refer.
//           msgid - iso 8583 message type id.
//                   eg. 0200 = financial request, 0400 = reversal request.
// REF     : g_ibuf
// RETURN  : ONL_Completed
//           ONL_Failed
// ---------------------------------------------------------------------------
UCHAR EMVDC_OnlineProcessing( UINT msgid, UCHAR ct, UCHAR *arc, UCHAR *iad, UINT *ist )
{
UINT  iLen;
UINT  iDataLen;
UCHAR tt;

      *ist = ADDR_ISU_SCRIPT_TEMP; // to signal kernel where the scripts are stored

      if( msgid != MSGID_FINANCIAL_CONFIRM )
        {
        arc[0] = 0;
        arc[1] = 0;
        }

      UI_ClearScreen();

      if( TL_OpenAUX() == FALSE )
        {
        DISP_STD_processing_error();
        return( ONL_Failed ); // device error
        }

      if( msgid == MSGID_REVERSAL_REQ )
        // "REVERSAL..."
        DISP_reversal();
      else
        // "PROCESSING..."
        DISP_processing();

      // --------------------------------------------------------------------
      // send request to HOST Simulator (2L-PF-DATA[])
      // --------------------------------------------------------------------
      if( msgid == MSGID_FINANCIAL_CONFIRM )
        {
        OP_SendFinancialConfirm( ct, iad );
//      OP_ReceiveFinancialConfirmResponse( g_ibuf );

        TL_CloseAUX();
        return( ONL_Completed );
        }

      if( msgid == MSGID_FINANCIAL_REQ )
        tt = TT_GOODS_AND_SERVICE;
      else
        tt = TT_FINANCIAL_REVERSAL;
	printf("@@@@@@@@@@@@%s %d tt=%d ct=%d\n",__func__,__LINE__,tt,ct);
      if( OP_SendFinancialTxRequest( tt, ct ) == FALSE )
        {printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
        TL_CloseAUX();
        return( ONL_Failed );
        }
printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
      // --------------------------------------------------------------------
      // receive response from HOST Simulator (2L-PF-DATA[])
      // --------------------------------------------------------------------
      if( OP_ReceiveFinancialTxResponse( g_ibuf ) == FALSE )
        {printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
        TL_CloseAUX();
        return( ONL_Failed );
        }
printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
      if( g_ibuf[RX_PF] == 1 )
        {printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
        if( OP_ReceiveIssuerScripts2() == FALSE )
          {printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
          TL_CloseAUX();
          return( ONL_Failed );
          }
        }
printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
      // process the received data elements
      iDataLen = g_ibuf[1]*256 + g_ibuf[0];
      if( iDataLen < 43 )
        {printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
        TL_CloseAUX();	// PATCH: 2007-11-05
        return( ONL_Failed );
	}
printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
      // Transaction Date
      // --- set Terminal DATE ---
      // Transaction Time
      // --- set Terminal TIME ---

      // Authorization Response Code
      arc[0] = g_ibuf[RX_ARC+0];
      arc[1] = g_ibuf[RX_ARC+1];

      // Authorization Code
      iLen = g_ibuf[RX_AUTH_CODE+1]*256 + g_ibuf[RX_AUTH_CODE];
      if( iLen != 0 )
        api_emv_PutDataElement( DE_TERM, ADDR_ISU_AUTH_CODE, 6, &g_ibuf[RX_AUTH_CODE+2] );
printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
      // Issuer Authentication Data
      iLen = g_ibuf[RX_IAD+1]*256 + g_ibuf[RX_IAD];
      if( iLen != 0 )
        memmove( iad, &g_ibuf[RX_IAD], iLen+2); // 2L-V
      else
        {
        iad[0] = 0;
        iad[1] = 0;
        }
printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
      TL_CloseAUX();
      return( ONL_Completed );
}

// ---------------------------------------------------------------------------
// FUNCTION: Voice Referral Processing - to APPROVE or DECLINE.
// INPUT   : action - 1=  to approve, 2= to decline.
// OUTPUT  : authcode - authorization code from bank. (an6)
//           or         authorization response code.  (an2)
// RETURN  : TRUE  - ARC entered to approve or decline.
//           FALSE - aborted by attendant.
// ---------------------------------------------------------------------------
UCHAR RP_EnterApprovalCode( UCHAR action, UCHAR *authcode )
{
UCHAR i;
UCHAR arc[7];
UCHAR result;

      // show "APPROVE" or "DECLINE"
      // show "ENTER AUTH CODE"
      UI_ClearScreen();
//    if( action == 1 )
//      DISP_approve( 0, 0, FONT0 );
//    else
//      DISP_decline( 0, 0, FONT0 );
//    DISP_enter_auth_code( 1, 0, FONT0 );

      DISP_enter_authcode();

      // wait for attendant entering ARC
      if( TL_GetAlphaNumDigits( 3, 0, FONT1, 2, arc ) == FALSE )
        {
        UI_ClearScreen(); // PATCH: PBOC2.0, 2006-02-14
        return( FALSE ); // aborted
        }

      result = 2 - arc[0];
      if( result != 0 )
        {
        for( i=0; i<result; i++ )
           arc[3-result+i] = 0x20; // left-adjustfied with padding spaces
        }

      memmove( authcode, &arc[1], 2 );

//    // case 1: the approval code = "Authorization Code (Tag=89)" -- an6
//    api_emv_PutDataElement( DE_TERM, ADDR_ISU_AUTH_CODE, 6, &arc[1] );

//    // case 2: the approval code = "Authorization Response Code (Tag=8A)" -- an2
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_ARC, 2, &arc[1] );

      UI_ClearScreen(); // PATCH: PBOC2.0, 2006-02-14
      return( TRUE ); // done
}

// ---------------------------------------------------------------------------
// FUNCTION: Voice Referral Processing.
//           it is a CALL-BACK function used by EMV Kernel API.
// INPUT   : none.
// OUTPUT  : auth_code - authorization code from bank. (an6)
//           or          authorization response code.  (an2)
// RETURN  : REF_Approved
//           REF_Declined
//           REF_ForcedOnline
//           REF_ForcedAccept
//           REF_ForcedDecline
// ---------------------------------------------------------------------------
UCHAR EMVDC_ReferralProcessing( UCHAR *authcode )
{
UCHAR buf[30];
UCHAR buf2[30];
UCHAR list[3*EMVDC_FUNC_LEN];
UCHAR result;
UINT  iLen;

RF_START:

      // show "CALL YOUR BANK"
      UI_ClearScreen();
      DISP_STD_call_your_bank();

      if( TL_UpdateReq( 99, 0, 0, 0 ) == TRUE )
        {
        // ---------------------
        // get & show card info
        // ---------------------

        UI_ClearScreen();

        //  1. Label Name
        api_emv_GetDataElement( DE_ICC, ADDR_ICC_AP_LABEL, 2, (UCHAR *)&iLen );
        if( iLen != 0 )
          {
          api_emv_GetDataElement( DE_ICC, ADDR_ICC_AP_LABEL, iLen+2, buf );
          UI_PutStr( 0, 0, FONT0, buf[0], &buf[2] );
          }

        //  2. Cardholder Name

        //  3. PAN
        api_emv_GetDataElement_SRED( DE_ICC, ADDR_ICC_AP_PAN, 12, buf );
        TL_ShowBCD( 2, 0, FONT0, buf[0], &buf[2] );

        //  4. Effect & Expiry Date (YY/MM/DD - YY/MM/DD)

        api_emv_GetDataElement( DE_ICC, ADDR_ICC_AP_EFFECT_DATE, 2, (UCHAR *)&iLen );
        if( iLen != 0 )
          {
          api_emv_GetDataElement( DE_ICC, ADDR_ICC_AP_EFFECT_DATE+2, iLen, buf );
          TL_bcd2asc( (UCHAR)iLen, buf, buf2 );
          TL_FormatDate( &buf2[1], buf );
          UI_PutStr( 3, 0, FONT0, 8, buf );
          }

        UI_PutChar( 3, 9, FONT0, 0x2d ); // '-'

        api_emv_GetDataElement( DE_ICC, ADDR_ICC_AP_EXPIRE_DATE+2, 3, buf );
        TL_bcd2asc( 3, buf, buf2 );
        TL_FormatDate( &buf2[1], buf );
        UI_PutStr( 3, 11, FONT0, 8, buf );

        UI_WaitKey();

        // -------------------------------------
        // show "1-APPROVE  2-DECLINE  3-ONLINE"
        // -------------------------------------
//      DISP_1approve_2decline( 3 );

        // wait for attendant's selection
        buf[0] = 0;
        buf[1] = 3;
        buf[2] = 2; // org=3, PATCH: PBOC2.0, 2006-02-14, remove the ONLINE item
        buf[3] = EMVDC_FUNC_LEN;
        buf[4] = 0;
        buf[5] = FONT2;
        TL_LoadFuncList( 2, list );
        result = TL_ListBox( 0, &buf[0], list, 00 ); // always wait

        switch( result )
              {
              case 0:   // approve

                   if( RP_EnterApprovalCode( 1, authcode ) == TRUE )
                     return( REF_Approved );
                   else
                     goto RF_START;

              case 1:   // decline

              //   if( RP_EnterApprovalCode( 1, authcode ) == TRUE ) REMOVED: EMV2000
                     return( REF_Declined );
                   goto RF_START;

              case 2:   // go online (RFU)

                   return( REF_ForcedOnline );

              default:  // abort

                   goto RF_START;
              }

//      // wait for selection
//      while(1)
//           {
//           switch( UI_WaitKey() )
//                 {
//                 case KEY_1: // to approve
//
//                      // reqeust "approval code"
//                      if( RP_EnterApprovalCode( 1, authcode ) == TRUE )
//                        return( REF_Approved );
//
//                      goto RF_START;
//
//                 case KEY_2: // to decline
//
//              //      if( RP_EnterApprovalCode( 2, authcode ) == TRUE )
//              //        return( REF_Declined );
//              //      goto RF_START;
//
//                      authcode[0] = 0x30; // "01"=refer to card issuer
//                      authcode[1] = 0x31; //
//                      return( REF_Declined );
//
//                 case KEY_CANCEL: // other cases
//
//                      goto RF_ABORT;
//                 }
//           } // while(1)

        } // if( update request )

RF_ABORT:

       return( REF_ForcedAbort );

      // --------------------------------------------------------------------
      //  SELECT (RFU)
      //
      //  1-ONLINE
      //  2-DECLINE   3-ACCEPT
      // --------------------------------------------------------------------
      UI_ClearScreen();
      DISP_select_app();
      DISP_1online( 2, 0, FONT0 );
      DISP_2decline( 3, 0, FONT0 );
//    DISP_2decline_3accept( 3, 0, FONT0 );

      while(1)
           {
           result = UI_WaitKey();
           switch( result )
                 {
                 case KEY_1:
                      return( REF_ForcedOnline );

                 case KEY_2:
                      return( REF_ForcedDecline );

        //       case KEY_3:
        //            return( REF_ForcedAccept );

                 case KEY_CANCEL:
                      return( REF_ForcedAbort );
                 }
           }

}

// ---------------------------------------------------------------------------
// FUNCTION: print receipt.
// INPUT   : condition - 0 = do not print the signature line. (declined)
//                       1 = print the signature line. (approved)
//           arc       - the authorization response code. (an2)
// OUTPUT  : none.
// RETURN  : TRUE
//           FALSE
//
// RECEIPT : 01234567890123456789012345678901
//
//           ********************************
//                    SYMLINK  AS300
//             EMV APPLICATION TEST RECEIPT
//           ********************************
//
//           LABEL: [                       ]
//           AID  : RRRRRRRRRRPPPPPPPPPPPPPP  (AID 12 bytes)
//                  PPPPPPPP                  (AID 4  bytes)
//
//           [CARDHOLDER NAME]
//           PAN  : [NNNN NNNN NNNN NNNN NNN]
//           [YY/MM/DD] - [YY/MM/DD]
//
//           TX SC: NNNNNNNN
//           YY/MM/DD  HH:MM
//
//           SALE           $N,NNN,NNN,NNN.NN
//                             $NNNNNNNNNN.NN
//
//           X_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
//
//           ********************************
//           [ VERDICT + ARC                ]
//           ********************************
//
//           AAC      XX XX XX XX XX XX XX XX
//           CVMR     XX XX XX
//           TVR      XX XX XX XX XX
//           IAC-DEN  XX XX XX XX XX
//           IAC-ONL  XX XX XX XX XX
//           IAC-DEF  XX XX XX XX XX
//           TSI      XX XX
//           DAC      XX XX
//           IDN      XX XX XX XX XX XX XX XX
//
// ---------------------------------------------------------------------------
UCHAR EMVDC_PrintReceipt( UCHAR condition, UCHAR *arc )
{
#ifdef	_PRINTER_ENABLED_
UCHAR msg_approved[]={"APPROVED"};
UCHAR msg_declined[]={"DECLINED"};
UCHAR buf[40];
UCHAR buf2[40];
UCHAR data[40];
UINT  iLen;
UCHAR i, j;
UINT  iARC;
UCHAR arc_code[2];

//                          01234567890123456789012345678901
UCHAR msg_star[]=         {"********************************"};
UCHAR msg_title1[]=       {"         SYMLINK  AS320"};
//UCHAR msg_title2[]=       {"  EMV APPLICATION TEST RECEIPT"};
UCHAR msg_title2[]=       {"  PCI APPLICATION TEST RECEIPT"};
UCHAR msg_label[]=        {"LABEL: "};
UCHAR msg_aid[]=          {"AID  : "};
UCHAR msg_pan[]=          {"PAN  : "};
UCHAR msg_txsc[]=         {"TX SC: "};
UCHAR msg_sale[]=         {"SALE           "};
UCHAR msg_sign_line[]=    {"X_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _"};
UCHAR msg_aac[]=          {"AC       "}; // AAC, TC or ARQC
UCHAR msg_cvmr[]=         {"CVMR     "};
UCHAR msg_tvr[]=          {"TVR      "};
UCHAR msg_iac_den[]=      {"IAC-DEN  "};
UCHAR msg_iac_onl[]=      {"IAC-ONL  "};
UCHAR msg_iac_def[]=      {"IAC-DEF  "};
UCHAR msg_tsi[]=          {"TSI      "};
UCHAR msg_dac[]=          {"DAC      "}; // data authentication code
UCHAR msg_idn[]=          {"IDN      "}; // ICC dynamic number

      UI_PrintClear();

      memmove( buf, msg_star, 32 );                     // ********************************
      buf[32] = 0x0a;
      UI_PrintPutStr( 33, buf );

      memmove( buf, msg_title1, 23 );                   //          SYMLINK  AS300
      buf[23] = 0x0a;
      UI_PrintPutStr( 24, buf );

      memmove( buf, msg_title2, 30 );                   //   EMV APPLICATION TEST RECEIPT
      buf[30] = 0x0a;
      UI_PrintPutStr( 31, buf );

      memmove( buf, msg_star, 32 );                     // ********************************
      buf[32] = 0x0a;
      buf[33] = 0x0a;
      UI_PrintPutStr( 34, buf );

      // --------------------------------------------------------------------
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_PEM, 1, buf );
      if( buf[0] == PEM_FALLBACK ) // MSR transaction
        goto PRINT_CHN;

      // --------------------------------------------------------------------
      memset( buf, 0x20, 32 );
      memmove( buf, msg_label, 7 );                     // LABEL:
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_AP_LABEL, 2, (UCHAR *)&iLen );
      if( iLen != 0 )
        api_emv_GetDataElement( DE_ICC, ADDR_ICC_AP_LABEL+2, iLen, &buf[7] );
      buf[7+iLen] = 0x0a;
      UI_PrintPutStr( 8+iLen, buf );

      api_emv_GetDataElement( DE_ICC, ADDR_ICC_AID, 2, (UCHAR *)&iLen );
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_AID+2, iLen, data );
      TL_hex2asc( (UCHAR)iLen, data, buf );

//    j=0;
//    for(i=0; i<19; i++)
//       {
//       buf[15+i] = 0x30 + j++;
//       if( j > 9 )
//         j=0;
//       }
//    buf[0] = 32;
//    iLen = 16;

      // --------------------------------------------------------------------
      memmove( &buf[7], &buf[1], buf[0] );
      memmove( buf, msg_aid, 7 );                       // AID  :
      if( iLen <= 12 )
        {
        buf[7+iLen*2] = 0x0a;
        buf[8+iLen*2] = 0x0a;
        UI_PrintPutStr( 9+iLen*2, buf );
        }
      else
        {
        memmove( data, &buf[0], 7+24 );
        data[7+24] = 0x0a;
        UI_PrintPutStr( 8+24, data );

        memset( data, 0x20, 7 );
        memmove( &data[7], &buf[7+24], (iLen-12)*2 );
        data[7+(iLen-12)*2] = 0x0a;
        data[8+(iLen-12)*2] = 0x0a;
        UI_PrintPutStr( 9+(iLen-12)*2, data );
        }

      // --------------------------------------------------------------------
PRINT_CHN:
                                                        //  Cardholder Name
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_CH_NAME, 2, (UCHAR *)&iLen );
      if( iLen != 0 )
        {
        api_emv_GetDataElement( DE_ICC, ADDR_ICC_CH_NAME+2, iLen, buf );
        buf[iLen] = 0x0a;
        UI_PrintPutStr( 1+iLen, buf );
        }

      // --------------------------------------------------------------------
      memmove( buf, msg_pan, 7 );                       // PAN  :
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_AP_PAN, 2, (UCHAR *)&iLen );
      api_emv_GetDataElement_SRED( DE_ICC, ADDR_ICC_AP_PAN+2, iLen, &buf[7] );
      TL_bcd2asc( (UCHAR)iLen, &buf[7], data );
      memmove( &buf[7], &data[1], data[0] );
      buf[7+iLen*2] = 0x0a;
      UI_PrintPutStr( 8+iLen*2, buf );

      // --------------------------------------------------------------------
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_PEM, 1, buf );
      if( buf[0] == PEM_FALLBACK ) // MSR transaction
        {
        api_emv_GetDataElement( DE_ICC, ADDR_ICC_AP_EXPIRE_DATE+2, 3, data );
        TL_bcd2asc( 3, data, buf );
        TL_FormatDate( &buf[1], data );
        data[5] = 0x0a;
        data[6] = 0x0a;
        UI_PrintPutStr( 7, data );

        goto PRINT_SC;
        }

                                                        // YY/MM/DD - YY/MM/DD
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_AP_EFFECT_DATE, 2, (UCHAR *)&iLen );
      if( iLen != 0 )
        {
        api_emv_GetDataElement( DE_ICC, ADDR_ICC_AP_EFFECT_DATE+2, iLen, data );
        TL_bcd2asc( (UCHAR)iLen, data, buf );
        TL_FormatDate( &buf[1], data );
        UI_PrintPutStr( 8, data );
        }

      buf[0] = ' ';
      buf[1] = '-';
      buf[2] = ' ';
      UI_PrintPutStr( 3, buf );

      api_emv_GetDataElement( DE_ICC, ADDR_ICC_AP_EXPIRE_DATE+2, 3, data );
      TL_bcd2asc( 3, data, buf );
      TL_FormatDate( &buf[1], data );
      data[8] = 0x0a;
      data[9] = 0x0a;
      UI_PrintPutStr( 10, data );

      // --------------------------------------------------------------------
PRINT_SC:

      memmove( buf, msg_txsc, 7 );                      // TX SC:
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TX_SC, 4, &buf[7] );
      TL_bcd2asc( 4, &buf[7], data );
      memmove( &buf[7], &data[1], data[0] );
      buf[7+8] = 0x0a;
      UI_PrintPutStr( 16, buf );

      // --------------------------------------------------------------------
                                                        // YY/MM/DD HH:MM
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TX_DATE, 3, data );
      if( data[0] == 0xff )
        TL_GetDateTime( &buf[1] );
      else
        TL_bcd2asc( 3, data, buf );

      TL_FormatDate( &buf[1], data );
      data[8] = 0x20;
      data[9] = 0x20;
      UI_PrintPutStr( 10, data );

      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TX_TIME, 3, data );
      if( data[0] == 0xff )
        {
        TL_GetDateTime( buf );
        TL_FormatTime( &buf[6], data );
        }
      else
        {
        TL_bcd2asc( 3, data, buf );
        TL_FormatTime( &buf[1], data );
        }
      data[8] = 0x0a;
      data[9] = 0x0a;
      UI_PrintPutStr( 10, data );

      // --------------------------------------------------------------------
      memset( buf, 0x20, 32 );
      memmove( buf, msg_sale, 15 );                     // SALE           $N,NNN,NNN,NNN.NN
      if( g_pre_auth == TRUE )
        {
        buf[0] = 'A';
        buf[1] = 'U';
        buf[2] = 'T';
        buf[3] = 'H';
        }

      api_emv_GetDataElement( DE_TERM, ADDR_TERM_AMT_AUTH_N, 6, data );

      // convert AMT bcd to ascii ($NNN)
      TL_bcd2asc( AMT_INT_SIZE+AMT_DEC_SIZE, data, buf2 );
      TL_trim_asc( 0, buf2, data );
      i = TL_insert_decimal_point( NUM_TYPE_DEC, data ); // NNNNN.NN
//    TL_insert_thousand_comma( data, i );       // NN,NNN.NN
      TL_trim_decimal( g_term_tx_exp, g_term_decimal_point, data ); // NNNNN

      buf2[0] = '$';
      for( i=0; i<data[0]; i++ )
         buf2[i+1] = data[i+1];

      i = 14 - (data[0]+1);
      memmove( &buf[18+i], buf2, data[0]+1 );
      buf[32] = 0x0a;
      buf[33] = 0x0a;
      UI_PrintPutStr( 34, buf );

      // --------------------------------------------------------------------
      if( condition == 1 )                              // X_________________
        {
        buf[0] = 0x0a;
        memmove( &buf[1], msg_sign_line, 32 );
//      buf[1] = 0xa2; // bigger signature char
//      buf[2] = 0xae; //
	buf[1] = 'X';
        buf[33] = 0x0a;
        UI_PrintPutStr( 34, buf );
        }

      // --------------------------------------------------------------------
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_PEM, 1, buf );
      if( buf[0] == PEM_FALLBACK ) // MSR transaction
        goto PRINT_FF;

      // --------------------------------------------------------------------
      memmove( buf, msg_star, 32 );                     // ********************************
      buf[32] = 0x0a;
      UI_PrintPutStr( 33, buf );

      memset( buf, 0x20, 34 );
      iARC = arc[0]*256 + arc[1];

      if( (iARC == ARC_OFFLINE_APPROVED) || (iARC == ARC_ONLINE_APPROVED) ||
          (iARC == ARC_APPROVED_UNABLE_ONLINE) || (iARC == ISU_ARC_APPROVED) ||
          (iARC == 0x5934) )
        {
        memmove( &buf[10], msg_approved, 8 );        // "APPROVED"
        i = 0;
        }
      else
        {
        memmove( &buf[10], msg_declined, 8 );        // "DECLINED"
        i = 1;
        }

      api_emv_GetDataElement( DE_TERM, ADDR_TERM_ARC, 2, arc_code ); // get ARC
      if( TL_CheckDataType( DEF_AN, arc_code, 2 ) == TRUE )
        {
        memmove( &buf[19], arc_code, 2 );            // ARC
        if( (i == 1) && (arc_code[0] == 'Y') && (arc_code[1] == '1') )
          memset( &buf[19], 0x2D, 2 );               // undefined ARC '--'
        }
      else
        memset( &buf[19], 0x2D, 2 );                  // unknonw ARC '--'

      buf[21] = 0x0a;
      UI_PrintPutStr( 22, buf );

      memmove( buf, msg_star, 32 );                     // ********************************
      buf[32] = 0x0a;
      buf[33] = 0x0a;
      UI_PrintPutStr( 34, buf );

      // --------------------------------------------------------------------
      memmove( buf, msg_aac, 9 );                       // AAC      XX XX XX

      api_emv_GetDataElement( DE_ICC, ADDR_ICC_AC+2, 8, data );
      TL_PrintHexData( 8, data, &buf[9] );
      buf[32] = 0x0a;
      UI_PrintPutStr( 33, buf );

      // --------------------------------------------------------------------
      memmove( buf, msg_cvmr, 9 );                      // CVMR     XX XX XX

      api_emv_GetDataElement( DE_TERM, ADDR_TERM_CVMR, 3, data );
      TL_PrintHexData( 3, data, &buf[9] );
      buf[17] = 0x0a;
      UI_PrintPutStr( 18, buf );

      // --------------------------------------------------------------------
      memmove( buf, msg_tvr, 9 );                       // TVR      XX XX XX

      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TVR, 5, data );
      TL_PrintHexData( 5, data, &buf[9] );
      buf[23] = 0x0a;
      UI_PrintPutStr( 24, buf );

      // --------------------------------------------------------------------
      memmove( buf, msg_iac_den, 9 );                   // IAC-DEN  XX XX XX

      api_emv_GetDataElement( DE_ICC, ADDR_ICC_IAC_DENIAL+2, 5, data );
      TL_PrintHexData( 5, data, &buf[9] );
      buf[23] = 0x0a;
      UI_PrintPutStr( 24, buf );

      // --------------------------------------------------------------------
      memmove( buf, msg_iac_onl, 9 );                   // IAC-ONL  XX XX XX

      api_emv_GetDataElement( DE_ICC, ADDR_ICC_IAC_ONLINE+2, 5, data );
      TL_PrintHexData( 5, data, &buf[9] );
      buf[23] = 0x0a;
      UI_PrintPutStr( 24, buf );

      // --------------------------------------------------------------------
      memmove( buf, msg_iac_def, 9 );                   // IAC-DEF  XX XX XX

      api_emv_GetDataElement( DE_ICC, ADDR_ICC_IAC_DEFAULT+2, 5, data );
      TL_PrintHexData( 5, data, &buf[9] );
      buf[23] = 0x0a;
      UI_PrintPutStr( 24, buf );

      // --------------------------------------------------------------------
      memmove( buf, msg_tsi, 9 );                       // TSI      XX XX

      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TSI, 2, data );
      TL_PrintHexData( 2, data, &buf[9] );
      buf[14] = 0x0a;
      UI_PrintPutStr( 15, buf );

      // --------------------------------------------------------------------
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_DAC, 4, data );

      if( (data[1]*256 + data[0]) != 0 )
        {
        memmove( buf, msg_dac, 9 );                       // DAC      XX XX
        TL_PrintHexData( 2, &data[2], &buf[9] );
        buf[14] = 0x0a;
        UI_PrintPutStr( 15, buf );
        }

      // --------------------------------------------------------------------
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_DYNAMIC_NBR, 10, data );

      iLen = data[1]*256 + data[0];
      if( iLen != 0 )
        {
        memset( buf, 0x20, 33 );
        memmove( buf, msg_idn, 9 );                       // IDN      XX XX XX XX XX XX XX XX
        TL_PrintHexData( iLen, &data[2], &buf[9] );
        buf[32] = 0x0a;
        UI_PrintPutStr( 33, buf );
        }

      // --------------------------------------------------------------------
PRINT_FF:

      buf[0] = 0x0a;
      buf[1] = 0x0a;
      buf[2] = 0x0a;
      buf[3] = 0x0a;
      buf[4] = 0x0a;
      buf[5] = 0x0a;
      buf[6] = 0x0c; // form feed
      UI_PrintPutStr( 7, buf );
      UI_PrintOut();

//    TL_DispHexByte(0,0, UI_PrintOut());
//    for(;;);
#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: print the specified log details.
// INPUT   : log - the fixed length transaction record.
// OUTPUT  : none.
// RETURN  : none.
//
//           01234567890123456789012345678901
//           TX SC: NNNNNN
//           - - - - - - - - - - - - - - - -
//           AIP  : XX XX
//           ATC  : XX XX
//           AUC  : XX XX
//           ARQC : XX
//           CID  : XX
//           CVML : XX XX XX XX   YY YY YY YY
//                  MM CC
//                  MM CC
//                  MM CC
//                  MM CC
//                  MM CC
//                  MM CC
//           CVMR : XX XX XX
//           IFD SN : CCCCCCCC
//           IAC-DEN: XX XX XX XX XX
//           IAC-ONL: XX XX XX XX XX
//           IAC-DEF: XX XX XX XX XX
//           ISSUER APP DATA:
//                    XX XX XX XX XX XX XX XX
//                    XX XX XX XX XX XX XX XX
//                    XX XX XX XX XX XX XX XX
//                    XX XX XX XX XX XX XX XX
//           TERM CAP : XX XX XX
//           TERM TYPE: XX
//           TVR      : XX XX XX XX XX
//           UPD NBR  : XX XX XX XX
//           ACQ ID   : NN NN NN NN NN NN
//           AMT AUTH : NN NN NN NN NN NN
//           AMT OTHER: NN NN NN NN NN NN
//           EFT DATE : YY MM DD
//           EXP DATE : YY MM DD
//           PAN      :
//           NN NN NN NN NN NN NN NN NN NN
//           PAN SN : NN
//           MID    : CCCCCCCCCCCCCCC
//           ISSUER COUNTRY CODE: NN NN
//           TERM COUNTRY CODE  : NN NN
//           MERCHANT CAT CODE  : NN NN
//           POS ENTRY MODE     : NN
//           TX CURRENCY CODE   : NN NN
//           TID    : NNNNNNNN
//           TX AMT : NN NN NN NN NN NN
//           TX DATE: YY MM DD
//           TX TIME: HH MM SS
//           TX TYPE: NN
//
// ---------------------------------------------------------------------------
void FN_PrintLogDetails( UCHAR *log )
{
#ifdef	_PRINTER_ENABLED_
UCHAR i, j;
UCHAR cnt;
UCHAR buf1[40];
UCHAR buf2[40];
UCHAR msg_txsc[] =        {"TX SC: "};
UCHAR msg_line[] =        {"- - - - - - - - - - - - - - - - "};
UCHAR msg_aip[] =         {"AIP  : "};
UCHAR msg_atc[] =         {"ATC  : "};
UCHAR msg_auc[] =         {"AUC  : "};
UCHAR msg_arqc[] =        {"ARQC : "};
UCHAR msg_cid[] =         {"CID  : "};
UCHAR msg_cvml[] =        {"CVML : "};
UCHAR msg_cvmr[] =        {"CVMR : "};
UCHAR msg_ifd_sn[] =      {"IFD SN : "};
UCHAR msg_iac_den[] =     {"IAC-DEN: "};
UCHAR msg_iac_onl[] =     {"IAC-ONL: "};
UCHAR msg_iac_def[] =     {"IAC-DEF: "};
UCHAR msg_isu_ap_data[] = {"ISSUER APP DATA:"};
UCHAR msg_term_cap[] =    {"TERM CAP : "};
UCHAR msg_term_type[] =   {"TERM TYPE: "};
UCHAR msg_tvr[] =         {"TVR      : "};
UCHAR msg_upd_nbr[] =     {"UPD NBR  : "};
UCHAR msg_acq_id[] =      {"ACQ ID   : "};
UCHAR msg_amt_auth[] =    {"AMT AUTH : "};
UCHAR msg_amt_other[] =   {"AMT OTHER: "};
UCHAR msg_eft_date[] =    {"EFT DATE : "};
UCHAR msg_exp_date[] =    {"EXP DATE : "};
UCHAR msg_pan[] =         {"PAN    : "};
UCHAR msg_pan_sn[] =      {"PAN SN : "};
UCHAR msg_mid[] =         {"MID    : "};
UCHAR msg_isu_cntr_code[]= {"ISSUER COUNTRY CODE: "};
UCHAR msg_term_cntr_code[]={"TERM COUNTRY CODE  : "};
UCHAR msg_mer_cat_code[] = {"MERCHANT CAT CODE  : "};
UCHAR msg_pem[] =          {"POS ENTRY MODE     : "};
UCHAR msg_tx_cc[] =        {"TX CURRENCY CODE   : "};
UCHAR msg_tid[] =          {"TID    : "};
UCHAR msg_tx_amt[] =       {"TX AMT : "};
UCHAR msg_tx_date[] =      {"TX DATE: "};
UCHAR msg_tx_time[] =      {"TX TIME: "};
UCHAR msg_tx_type[] =      {"TX TYPE: "};


      UI_PutMsg( 2, COL_MIDWAY, FONT0+attrCLEARWRITE, 8, (UCHAR *)msg_printing );

      UI_PrintClear();

      // TX SC: NNNNNNNN
      memmove( buf1, msg_txsc, 7 );
      TL_bcd2asc( 4, &log[LOG_TX_SC], buf2 );
      memmove( &buf1[7], &buf2[1], buf2[0] );
      buf1[7+8] = 0x0a;
      UI_PrintPutStr( 16, buf1 );

      // - - - - - - - - - - - - - - - -
      memmove( buf1, msg_line, 32 );
      buf1[32] = 0x0a;
      UI_PrintPutStr( 33, buf1 );

      // AIP  : XX XX
      memmove( buf1, msg_aip, 7 );
      TL_PrintHexData( 2, &log[LOG_AIP], &buf1[7] );
      buf1[12] = 0x0a;
      UI_PrintPutStr( 13, buf1 );

      // ATC  : XX XX
      memmove( buf1, msg_atc, 7 );
      TL_PrintHexData( 2, &log[LOG_ATC], &buf1[7] );
      buf1[12] = 0x0a;
      UI_PrintPutStr( 13, buf1 );

      // AUC  : XX XX
      memmove( buf1, msg_auc, 7 );
      TL_PrintHexData( 2, &log[LOG_AUC], &buf1[7] );
      buf1[12] = 0x0a;
      UI_PrintPutStr( 13, buf1 );

      // ARQC : XX
      memmove( buf1, msg_arqc, 7 );
      TL_PrintHexData( 2, &log[LOG_ARQC], &buf1[7] );
      buf1[9] = 0x0a;
      UI_PrintPutStr( 10, buf1 );

      // CID  : XX
      memmove( buf1, msg_cid, 7 );
      TL_PrintHexData( 2, &log[LOG_CID], &buf1[7] );
      buf1[9] = 0x0a;
      UI_PrintPutStr( 10, buf1 );

      // CVML : XX XX XX XX   YY YY YY YY
      //        MM CC
      memmove( buf1, msg_cvml, 7 );
      TL_PrintHexData( 4, &log[LOG_CVML+2], &buf1[7] );
      buf1[18] = 0x20;
      buf1[19] = 0x20;
      buf1[20] = 0x20;
      TL_PrintHexData( 4, &log[LOG_CVML+6], &buf1[21] );
      buf1[32] = 0x0a;
      UI_PrintPutStr( 33, buf1 );

      i = log[LOG_CVML+1]*256 + log[LOG_CVML+0];
      if( i >= 8 )
        {
        cnt = ( i - 8) / 2; // no. of MM CC
        if( cnt != 0 )
          {
          memset( buf1, 0x20, 7 );
          for( i=0; i<cnt; i++ )
             {
             TL_PrintHexData( 1, &log[LOG_CVML+10+i*2], &buf1[7] );
             buf1[9] = 0x20;
             TL_PrintHexData( 1, &log[LOG_CVML+10+i*2+1], &buf1[10] );
             buf1[12] = 0x0a;
             UI_PrintPutStr( 13, buf1 );
             }
          }
        }

      // CVMR : XX XX XX
      memmove( buf1, msg_cvmr, 7 );
      TL_PrintHexData( 3, &log[LOG_CVMR], &buf1[7] );
      buf1[15] = 0x0a;
      UI_PrintPutStr( 16, buf1 );

      // IFD SN : CCCCCCCC
      memmove( buf1, msg_ifd_sn, 9 );
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_IFD_SN, 8, &buf1[9] );
      buf1[17] = 0x0a;
      UI_PrintPutStr( 18, buf1 );

      // IAC-DEN: XX XX XX XX XX
      memmove( buf1, msg_iac_den, 9 );
      TL_PrintHexData( 5, &log[LOG_IAC_DEN], &buf1[9] );
      buf1[23] = 0x0a;
      UI_PrintPutStr( 24, buf1 );

      // IAC-ONL: XX XX XX XX XX
      memmove( buf1, msg_iac_onl, 9 );
      TL_PrintHexData( 5, &log[LOG_IAC_ONL], &buf1[9] );
      buf1[23] = 0x0a;
      UI_PrintPutStr( 24, buf1 );

      // IAC-DEF: XX XX XX XX XX
      memmove( buf1, msg_iac_def, 9 );
      TL_PrintHexData( 5, &log[LOG_IAC_DEF], &buf1[9] );
      buf1[23] = 0x0a;
      UI_PrintPutStr( 24, buf1 );

      // ISSUER APP DATA:
      //          XX XX XX XX XX XX XX XX
      memmove( buf1, msg_isu_ap_data, 16 );
      buf1[16] = 0x0a;
      UI_PrintPutStr( 17, buf1 );

      i = log[LOG_ISU_AP_DATA+1]*256 + log[LOG_ISU_AP_DATA+0];
      if( i != 0 )
        {
        cnt = i / 8;
        j = i % 8;

        memset( buf1, 0x20, 9 );
        for( i=0; i<cnt; i++ )
           {
           TL_PrintHexData( 8, &log[LOG_ISU_AP_DATA+2+i*8], &buf1[9] );
           buf1[32] = 0x0a;
           UI_PrintPutStr( 33, buf1 );
           }

        memset( buf1, 0x20, 32 );
        if( j != 0 )
          {
          TL_PrintHexData( j, &log[LOG_ISU_AP_DATA+2+i*8], &buf1[9] );
          buf1[32] = 0x0a;
          UI_PrintPutStr( 33, buf1 );
          }
        }

      // TERM CAP : XX XX XX
      memmove( buf1, msg_term_cap, 11 );
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_CAP, 3, buf2 );
      TL_PrintHexData( 3, buf2, &buf1[11] );
      buf1[19] = 0x0a;
      UI_PrintPutStr( 20, buf1 );

      // TERM TYPE: XX
      memmove( buf1, msg_term_type, 11 );
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TYPE, 1, buf2 );
      TL_PrintHexData( 1, buf2, &buf1[11] );
      buf1[14] = 0x0a;
      UI_PrintPutStr( 15, buf1 );

      // TVR      : XX XX XX XX XX
      memmove( buf1, msg_tvr, 11 );
      TL_PrintHexData( 5, &log[LOG_TVR], &buf1[11] );
      buf1[25] = 0x0a;
      UI_PrintPutStr( 26, buf1 );

      // UPD NBR  : XX XX XX XX
      memmove( buf1, msg_upd_nbr, 11 );
      TL_PrintHexData( 4, &log[LOG_UPD_NBR], &buf1[11] );
      buf1[22] = 0x0a;
      UI_PrintPutStr( 23, buf1 );

      // ACQ ID   : NN NN NN NN NN NN
      memmove( buf1, msg_acq_id, 11 );
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_ACQ_ID, 6, buf2 );
      TL_PrintHexData( 6, buf2, &buf1[11] );
      buf1[28] = 0x0a;
      UI_PrintPutStr( 29, buf1 );

      // AMT AUTH : NN NN NN NN NN NN
      memmove( buf1, msg_amt_auth, 11 );
      TL_PrintHexData( 6, &log[LOG_AMT_AUTH], &buf1[11] );
      buf1[28] = 0x0a;
      UI_PrintPutStr( 29, buf1 );

      // AMT OTHER: NN NN NN NN NN NN
      memmove( buf1, msg_amt_other, 11 );
      TL_PrintHexData( 6, &log[LOG_AMT_OTHER], &buf1[11] );
      buf1[28] = 0x0a;
      UI_PrintPutStr( 29, buf1 );

      // EFT DATE : YY MM DD
      memmove( buf1, msg_eft_date, 11 );
      TL_PrintHexData( 6, &log[LOG_AP_EFFECT_DATE], &buf1[11] );
      buf1[19] = 0x0a;
      UI_PrintPutStr( 20, buf1 );

      // EXP DATE : YY MM DD
      memmove( buf1, msg_exp_date, 11 );
      TL_PrintHexData( 6, &log[LOG_AP_EXPIRE_DATE], &buf1[11] );
      buf1[19] = 0x0a;
      UI_PrintPutStr( 20, buf1 );

      // PAN    :
      // NN NN NN NN NN NN NN NN NN NN
      memmove( buf1, msg_pan, 9 );
      buf1[9] = 0x0a;
      UI_PrintPutStr( 10, buf1 );
      TL_PrintHexData( 10, &log[LOG_AP_PAN], buf1 );
      buf1[29] = 0x0a;
      UI_PrintPutStr( 30, buf1 );

      // PAN SN : NN
      memmove( buf1, msg_pan_sn, 9 );
      TL_PrintHexData( 1, &log[LOG_AP_PAN_SN], &buf1[9] );
      buf1[11] = 0x0a;
      UI_PrintPutStr( 12, buf1 );

      // MID    : CCCCCCCCCCCCCCC
      memmove( buf1, msg_mid, 9 );
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_MID, 15, &buf1[9] );
      if( buf1[9] == 0 )
        memset( &buf1[9], 0x20, 15 );
      buf1[24] = 0x0a;
      UI_PrintPutStr( 25, buf1 );

      // ISSUER COUNTRY CODE: NN NN
      memmove( buf1, msg_isu_cntr_code, 21 );
      TL_PrintHexData( 2, &log[LOG_ISU_CNTR_CODE], &buf1[21] );
      buf1[26] = 0x0a;
      UI_PrintPutStr( 27, buf1 );

      // TERM COUNTRY CODE  : NN NN
      memmove( buf1, msg_term_cntr_code, 21 );
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_CNTR_CODE, 2, buf2 );
      TL_PrintHexData( 2, buf2, &buf1[21] );
      buf1[26] = 0x0a;
      UI_PrintPutStr( 27, buf1 );

      // MERCHANT CAT CODE  : NN NN
      memmove( buf1, msg_mer_cat_code, 21 );
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_MCC, 2, buf2 );
      TL_PrintHexData( 2, buf2, &buf1[21] );
      buf1[26] = 0x0a;
      UI_PrintPutStr( 27, buf1 );

      // POS ENTRY MODE     : NN
      memmove( buf1, msg_pem, 21 );
      TL_PrintHexData( 1, &log[LOG_PEM], &buf1[21] );
      buf1[23] = 0x0a;
      UI_PrintPutStr( 24, buf1 );

      // TX CURRENCY CODE   : NN NN
      memmove( buf1, msg_tx_cc, 21 );
      TL_PrintHexData( 2, &log[LOG_TX_CC], &buf1[21] );
      buf1[26] = 0x0a;
      UI_PrintPutStr( 27, buf1 );

      // TID    : NNNNNNNN
      memmove( buf1, msg_tid, 9 );
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TID, 8, &buf1[9] );
      buf1[17] = 0x0a;
      UI_PrintPutStr( 18, buf1 );

      // TX AMT : NN NN NN NN NN NN
      memmove( buf1, msg_tx_amt, 9 );
      TL_PrintHexData( 6, &log[LOG_TX_AMT], &buf1[9] );
      buf1[26] = 0x0a;
      UI_PrintPutStr( 27, buf1 );

      // TX DATE: YY MM DD
      memmove( buf1, msg_tx_date, 9 );
      TL_PrintHexData( 6, &log[LOG_TX_DATE], &buf1[9] );
      buf1[17] = 0x0a;
      UI_PrintPutStr( 18, buf1 );

      // TX TIME: HH MM SS
      memmove( buf1, msg_tx_time, 9 );
      TL_PrintHexData( 6, &log[LOG_TX_TIME], &buf1[9] );
      buf1[17] = 0x0a;
      UI_PrintPutStr( 18, buf1 );

      // TX TYPE: NN
      memmove( buf1, msg_tx_type, 9 );
      TL_PrintHexData( 1, &log[LOG_TX_TYPE], &buf1[9] );
      buf1[11] = 0x0a;
      UI_PrintPutStr( 12, buf1 );

      buf1[0] = 0x0a;
      buf1[1] = 0x0a;
      buf1[2] = 0x0a;
      buf1[3] = 0x0a;
      buf1[4] = 0x0a;
      buf1[5] = 0x0a;
      buf1[6] = 0x0c; // form feed
      UI_PrintPutStr( 7, buf1 );
      UI_PrintOut();
#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: print the specified log details.
//
//             01234567890123456789
//           0 PRINT LOG DETAILS
//           1 ENTER TX SEQ COUNTER
//           2
//           3               NNNNNN
//
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
// ---------------------------------------------------------------------------
void EMVDC_PrintLogDetails( void )
{
#ifdef	_PRINTER_ENABLED_
UINT  i;
UCHAR buf1[64];
UCHAR buf2[64];
UCHAR log[TX_LOG_LEN];

PRT_ST:

      UI_ClearScreen();
      DISP_print_log_details();
      DISP_enter_tx_sc();

ASK_TXSC:
      // ask for "Transaction Sequence Counter"
      if( TL_GetNumKey( 0, 0, '_', FONT0, 3, 8, buf1 ) == FALSE )
        return;
      if( (buf1[0] == 1) && (buf1[1] == '0') )
        {
//      goto ASK_TXSC; // reject 0

	// get the lastest one by "Transaction Sequence Counter"
	buf2[0] = 0;
//	api_emvk_GetDataElement( 0x9F, 0x41, buf1 );
//	memmove( &buf2[1], &buf1[2], 3 );
	api_emv_GetDataElement( DE_TERM, ADDR_TERM_TX_SC, 4, buf1 );
	memmove( &buf2[1], &buf1[1], 3 );

	goto SEARCH_TSC;
	}

      TL_asc2bcd( 4, buf2, buf1 ); // convert to BCD

// memset( log, 0x00, TX_LOG_LEN );
// FN_PrintLogDetails( log );
// return;

// TL_DispHexWord(0,0,g_term_tx_log_cnt);
// UI_WaitKey();
// TL_DumpHexData(0,0,8, buf2);
SEARCH_TSC:
      // search the specified record (backward)
      if( g_term_tx_log_cnt != 0 ) // tx log available?
        {
        i = g_term_tx_log_cnt;
        do{

          TL_GetTransLog( --i, log ); // read record

// TL_DumpHexData(0,0,255, log);

          if( TL_memcmp( buf2, &log[LOG_TX_SC], 4 ) == 0 ) // same TX_SC?
            {
            FN_PrintLogDetails( log );
            return; // done
            }

          } while( i != 0 );

        }

      // record not found
      UI_ClearRow( 3, 1, FONT0 );
      DISP_record_not_found();
      TL_WaitTime(100);

      goto PRT_ST;
#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: complete the transaction.
//           (1) save transaction log.
//           (2) close icc session.
//           (3) print receipt.
// INPUT   : none.
// OUTPUT  : none.
// REF     : APPLICATION LABEL
//           AID
//           CARDHOLDER NAME
//           PAN
//           PAN SEQUENCE NBR
//           EFFECTIVE DATE
//           EXPIRY DATE
//           TRANSACTION TYPE
//           TRANSACTION AMOUNT
//           VERDICT + ARC
//           TRANSACTION SEQUENCE COUNTER
//           TRANSACTION DATE / TIME
//           AAC
//           CVMR
//           TVR
//           IAC-DEFAULT
//           IAC-DENIAL
//           IAC-ONLINE
//           TSI
// RETURN  :
//
// ---------------------------------------------------------------------------
UCHAR EMVDC_close_transaction( void )
{
UCHAR buf[8];
UCHAR signline;

      UI_ClearScreen();

      return( TRUE );	// skip printing receipt

      // print out or not?
      if( TL_UpdateReq( 3, 3, COL_LEFTMOST, FONT0 ) == FALSE )
        return;

      UI_ClearScreen();

      // print receipt ...
      DISP_printing();

//    api_emv_GetDataElement( DE_TERM, ADDR_TERM_ARC, 2, arc );
//    EMVDC_PrintReceipt( 1, arc );

      api_emv_GetDataElement( DE_TERM, ADDR_TERM_CVMR, 3, buf );
      buf[0] &= 0x3F; // effective only bit0-5
      if( (buf[0] == CVR_PLAINTEXT_PIN_AND_SIGN)  ||
          (buf[0] == CVR_ENCIPHERED_PIN_AND_SIGN) ||
          (buf[0] == CVR_SIGN_PAPER) )
        signline = 1;
      else
        signline = 0;

      EMVDC_PrintReceipt( signline, g_term_ARC );
}

// ---------------------------------------------------------------------------
// FUNCTION: Upload the last BDC record to HOST.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : TRUE
//           FALSE
// ---------------------------------------------------------------------------
UCHAR OP_UploadLastBDC( void )
{
UINT  iLen;
UINT  iDataLen;
UINT  iIsrLen;
UCHAR len;
UCHAR cnt;
UCHAR isr[2+5*16]; // 2L-V

UCHAR ct, tt;


      iDataLen = 0;

      // Terminal Identifier
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TID, 8, &g_ibuf[TX_TID] );
      iDataLen += 8;

      // Transaction Date
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TX_DATE, 3, &g_ibuf[TX_DATE] );
      iDataLen += 3;

      // Transaction Time
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TX_TIME, 3, &g_ibuf[TX_TIME] );
      iDataLen += 3;

      ct = AC_ARQC;
      // ARQC
      g_ibuf[TX_ARQC] = ct;
      iDataLen += 1;

      tt = TT_GOODS_AND_SERVICE;
      g_pre_auth = FALSE;

      // Transaction Type
      if( tt == TT_GOODS_AND_SERVICE )
        {
        if( g_pre_auth == FALSE )
          api_emv_GetDataElement( DE_TERM, ADDR_TERM_TX_TYPE, 1, &g_ibuf[TX_TYPE] );
        else
          g_ibuf[TX_TYPE] = TT_PRE_AUTH; // TA only
        }
      else
        g_ibuf[TX_TYPE] = tt;
      iDataLen += 1;

      // Amount, Authorized
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_AMT_AUTH_N, 6, &g_ibuf[TX_AMT_AUTH] );
      iDataLen += 6;

      // Amount, Other
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_AMT_OTHER_N, 6, &g_ibuf[TX_AMT_OTHER] );
      iDataLen += 6;

      // Transaction Amount
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TX_AMT, 6, &g_ibuf[TX_AMT] );
      iDataLen += 6;

      // Terminal Currency Code
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TX_CC, 2, &g_ibuf[TX_CC] );
      iDataLen += 2;

      // Application PAN
      memset( &g_ibuf[2+TX_PAN], 0xff, 10 );
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_AP_PAN, 2, (UCHAR *)&iLen );
      api_emv_GetDataElement_SRED( DE_ICC, ADDR_ICC_AP_PAN+2, iLen, &g_ibuf[TX_PAN] );
      iDataLen += 10;

      // Application PAN Sequence Number
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_AP_PAN_SN+2, 1, &g_ibuf[TX_PAN_SN] );
      iDataLen += 1;

      // Application Interchange Profile (b2)
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_AIP, 2, &g_ibuf[TX_AIP] );
      iDataLen += 2;

      // Application Transaction Counter (b2) - ATC
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_ATC+2, 2, &g_ibuf[TX_ATC] );
      iDataLen += 2;

      // Terminal Verification Results
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TVR, 5, &g_ibuf[TX_TVR] );
      iDataLen += 5;

      // Terminal Country Code
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_CNTR_CODE, 2, &g_ibuf[TX_CNTR_CODE] );
      iDataLen += 2;

      // Unpredictable Number
      memset( &g_ibuf[TX_UPD_NBR], 0x00, 6 );
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_UPD_NBR_LEN, 1, (UCHAR *)&len );
      if( len != 0 )
        {
        g_ibuf[TX_UPD_NBR+0] = 4;
        g_ibuf[TX_UPD_NBR+1] = 0;
        api_emv_GetDataElement( DE_TERM, ADDR_TERM_UPD_NBR, 4, &g_ibuf[TX_UPD_NBR+2] );
        }
      iDataLen += 6;

      // Issuer Application Data - IAD
      memset( &g_ibuf[TX_ISU_AP_DATA], 0x00, 34 );
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_ISU_AP_DATA, 2, (UCHAR *)&iLen );
      if( iLen != 0 )
        {
        g_ibuf[TX_ISU_AP_DATA+0] = iLen & 0x00ff;
        g_ibuf[TX_ISU_AP_DATA+1] = (iLen & 0xff00) >> 8;
        api_emv_GetDataElement( DE_ICC, ADDR_ICC_ISU_AP_DATA+2, iLen, &g_ibuf[TX_ISU_AP_DATA+2] );
        }
      iDataLen += 34;

      // Cryptogram Information Data (b1) - CID
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_CID+2, 1, &g_ibuf[TX_CID] );
      iDataLen += 1;

      // Application Cryptogram (b8)
      api_emv_GetDataElement( DE_ICC, ADDR_ICC_AC+2, 8, &g_ibuf[TX_AC] );
      iDataLen += 8;

      // POS Entry Mode (n2)
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_PEM, 1, &g_ibuf[TX_PEM] );
      iDataLen += 1;

      // Issuer Script Results for Reversal if present (2L-V[5*n])
      g_ibuf[TX_REV_ISR+0] = 0;
      g_ibuf[TX_REV_ISR+1] = 0;
      iIsrLen = 0;
//    if( tt == TT_FINANCIAL_REVERSAL )
//      {
        apk_ReadRamDataTERM( ADDR_TERM_ISR, 1, (UCHAR *)&cnt );
        if( cnt != 0 )
          {
          iLen = cnt*5;
          apk_ReadRamDataTERM( ADDR_TERM_ISR+1, iLen, &isr[2] );
          isr[0] = iLen & 0x00ff;
          isr[1] = (iLen & 0xff00) >> 8;
          iDataLen += iLen;
          iIsrLen = iLen;
          memmove( &g_ibuf[TX_REV_ISR], isr, iLen+2 );
          }
//      }
      iDataLen += 2;
      iIsrLen += 2;

      // Enciphered PIN Data (RFU)
      g_ibuf[TX_REV_ISR+iIsrLen+0] = 0;
      g_ibuf[TX_REV_ISR+iIsrLen+1] = 0;
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_EPIN_DATA, 2, (UCHAR *)&iLen );
      if( iLen != 0 )
        {
//      api_emv_GetDataElement( DE_TERM, ADDR_TERM_EPIN_DATA, 10, &g_ibuf[TX_REV_ISR+iIsrLen] );
        api_emv_GetDataElement( DE_TERM, ADDR_TERM_EPIN_DATA, iLen+2, &g_ibuf[TX_REV_ISR+iIsrLen] );	// 2019-04-26
        iDataLen += iLen;
        iIsrLen += iLen;
        }
      iDataLen += 2;
      iIsrLen += 2;

      // KSN (RFU)
//    g_ibuf[TX_REV_ISR+iIsrLen+2+0] = 0;
//    api_emv_GetDataElement( DE_KEY, ADDR_DUKPT_KSN, 1, (UCHAR *)&iLen );
//    if( iLen != 0 )
//      {
//      g_ibuf[TX_REV_ISR+iIsrLen] = 10;
//      g_ibuf[TX_REV_ISR+iIsrLen+1] = 0;
//
//      api_emv_GetDataElement( DE_KEY, ADDR_DUKPT_KSN+1, 10, &g_ibuf[TX_REV_ISR+iIsrLen+2] );
//      iDataLen += iLen;
//      }
//    iDataLen += 1;

//    if( iLen == 0 )
//      g_ibuf[2] = 0; // set final block flag
//    else
//      g_ibuf[2] = 1; // set chained block flag

      // TSI (2 bytes) PATCH: PBOC2.0, 2006-02-09
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TSI, 2, &g_ibuf[TX_REV_ISR+iIsrLen] );
      iDataLen += 2;

      // ------------------------------------------------
      g_ibuf[2] = 0;   // set final block flag
      iDataLen += 1;

      g_ibuf[0] = iDataLen & 0x00ff;
      g_ibuf[1] = (iDataLen & 0xff00) >> 8;

      if( TL_TransmitAUX( g_ibuf ) == FALSE )
        return( FALSE );
      else
        {
        if( g_ibuf[2] == 0 ) // check PF chained flag
          {
          return( TRUE );
          }
        else
          {
          // Send Enciphered PIN Data
          // --- TO BE IMPLEMENTED ---
          return( TRUE );
          }
        }
}

// ---------------------------------------------------------------------------
UCHAR EMVDC_UploadLastBDC( void )
{

      UI_ClearScreen();

      if( TL_OpenAUX() == FALSE )
        {
        DISP_STD_processing_error();
        return( ONL_Failed ); // device error
        }

      DISP_processing();

      // --------------------------------------------------------------------
      OP_UploadLastBDC();

      // --------------------------------------------------------------------
      // receive response from HOST Simulator (2L-PF-DATA[])
      // --------------------------------------------------------------------
      if( OP_ReceiveFinancialTxResponse( g_ibuf ) == FALSE )
        {
        TL_CloseAUX();
        return( ONL_Failed );
        }

      if( g_ibuf[RX_PF] == 1 )
        {
        if( OP_ReceiveIssuerScripts2() == FALSE )
          {
          TL_CloseAUX();
          return( ONL_Failed );
          }
        }

      TL_CloseAUX();
      return( ONL_Completed );
}

