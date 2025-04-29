//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : EMV LEVEL 2 Debit/Credit                                   **
//**  PRODUCT  : AS330	                                                    **
//**                                                                        **
//**  FILE     : L2CONFIG.C                                                 **
//**  MODULE   : Configuration settings for EMV L2 V4.1a.                   **
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

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 0. (terminal default settings)
//
// ---------------------------------------------------------------------------
void SETUP_config_00( void )
{
UINT  i;
UCHAR buf[64];

      // --- Terminal Type ---
      buf[0] = 0x22; // POS terminal
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TYPE, 1, buf );

      // --- Terminal Capabilities ---
      buf[0] = 0xE0;
      buf[1] = 0xF8; // EMV L2
//    buf[1] = 0xE9; // PBOC2.0 ONLY, not support Offline Enciphered PIN but support Cardholder License Check
      buf[2] = 0xC8;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_CAP, 3, buf );

      // --- Additional Terminal Capabilities ---
      buf[0] = 0x60;
      buf[1] = 0x00;
      buf[2] = 0xF0;
      buf[3] = 0xA0;
      buf[4] = 0x01;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_ADD_CAP, 5, buf );

      // IFD serial number
      buf[0] = 'I';
      buf[1] = 'F';
      buf[2] = 'D';
      buf[3] = '0';
      buf[4] = '0';
      buf[5] = '0';
      buf[6] = '0';
      buf[7] = '1';
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_IFD_SN, 8, buf );

      // ----------------------------
      // Application Independent Data
      // ----------------------------

      // --- Terminal Country Code (n3) ---
      buf[0] = 0x01;
      buf[1] = 0x58;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_CNTR_CODE, 2, buf );

      // -----------------------------
      // Application Dependent Data
      // (specified by payment system)
      // -----------------------------

      // --- Application Identifier (AID) ---

      // clear all TERM_AID's
      memset( buf, 0x00, TERM_AID_LEN );
      for(i=0; i<MAX_AID_CNT; i++)
         api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_01+i*TERM_AID_LEN, TERM_AID_LEN, buf );

      // --- clear all Application Version Number (b2) ---
      memset( buf, 0x00, AVN_LEN );
      for(i=0; i<MAX_AVN_CNT; i++)
         api_emv_PutDataElement( DE_TERM, ADDR_AVN_01+i*AVN_LEN, AVN_LEN, buf );

      // --- clear Default DDOL ---
      buf[0] = 0;
      buf[1] = 0;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_DDOL, 2, buf );

      // --- clear Default TDOL ---
      buf[0] = 0;
      buf[1] = 0;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TDOL, 2, buf );

      // --- Terminal Identification (an8) ---
      buf[0] = '3';
      buf[1] = '1';
      buf[2] = '0';
      buf[3] = '0';
      buf[4] = '0';
      buf[5] = '0';
      buf[6] = '0';
      buf[7] = '0';
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TID, 8, buf );

      // --- Transaction Currency Code (n3) ---
      buf[0] = 0x09; // iso 4217, TWD=901
      buf[1] = 0x01; //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_CC, 2, buf );

      // --- Transaction Currency Exponent (n1, decimal point from the right of amt) ---
      buf[0] = 0x00; // iso 4217, no decimal point
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_CE, 1, buf );
      g_term_tx_exp = 0;
      g_term_decimal_point = 0;

      // --- Transaction Type (iso 8583:Processing Code) ---
      g_term_tx_type = TT_GOODS; // 00=GOODS, 01=SERVICE

      // --- default "Language Preference" ---
      buf[0] = 4;
      buf[1] = 0;
      buf[2] = 'z'; // "zh" = Chinese
      buf[3] = 'h';
      buf[4] = 'e'; // "en" = English
      buf[5] = 'n';
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_LANG_PREFER, 6, buf );

      // --- PBOC2.0, acquirer id ---
      buf[0] = 0x08;
      buf[1] = 0x22;
      buf[2] = 0x00;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x01;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_ACQ_ID, 6, buf );

      // --- PBOC2.0, merchant category code ---
      buf[0] = 0x00;
      buf[1] = 0x01;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_MCC, 2, buf );

      // --- PBOC2.0, merchant id ---
      memset( buf, 0x30, 15 );
      buf[0] = '1';
      buf[1] = '2';
      buf[2] = '3';
      buf[3] = '4';
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_MID, 15, buf );

      // --- PBOC2.0, transaction reference currency code ---
      buf[0] = 0x08; // iso 4217, USD=0840
      buf[1] = 0x40; //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_RCC, 2, buf );

      // --- PBOC2.0, transaction reference currency exponent  ---
      buf[0] = 0x02; // iso 4217, BEF=0056 (Belgium Franc)
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_RCE, 1, buf );

      g_dhn_pinpad = 0; // pinpad works
}

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 1. (EMV L2)
//
// List of AID's in the following sequence order:
// 1 Non EMV   : B0 12 34 56 78 12 34
//               (Application Selection Indicator indicates that multiples
//               occurences of the Application within the card are not allowed)
// 2 MasterCard: A0 00 00 00 04 10 10
//               (Application Selection Indicator indicates that multiples
//               occurences of the Application within the card are not allowed)
// 3 Partial   : A0 00 00 00 10
//               (Application Selection Indicator indicates that multiples
//               occurences of the Application within the card are allowed)
//
// Terminal Application Version Number: 00 02
//               (if different values are supported, all values set to 00 02)
//
// All TAC's set to 0
//
// Default TDOL if used: ICC Application Version Number ( 9F 08)
//
// Default DDOL: 9F 37 04 9F 47 01 8F 01 9F 32 01
//
// Terminal Country Code: 00 56
//
// Transaction Currency Code: 09 78 (EURO)
//
// Transaction Currency Exponent: 02
//
// Terminal Floor Limit: 100 (=100.0 or 100.00 or 100.000 according to the number of decimal values)
//
// If Random Transaction Selection process supported:
//    Target and Maximum Target percentage for random selection set to 0 to avoid random behaviour.
//
// If Random Transaction Selection process supported:
//    Threshold value for biased random selection shall be set to 5 (or 5.0 or 5.00 or 5.000 according to the number of decimal values)
//
// If supported, the revocation list should include the following data:
//   RID   A0 00 00 00 04
//   CA PK Index   F8
//   Certificate Serial Number   00 10 00
//
// "If supported, the exception list should include the following data:"
//     54 13 33 90 00 00 15 96
// ---------------------------------------------------------------------------
void SETUP_config_01( void )
{
UINT  i;
UCHAR buf[64];


      // --- Terminal Capabilities ---
      // for ADVT Test Card #29: Offline Enciphered PIN
      buf[0] = 0xE0;
      buf[1] = 0xF8; // offline plaintext PIN + online enciphered PIN + signature + offline enciphered PIN + no CVM required
//    buf[1] = 0x10; // offline enciphered PIN
      buf[2] = 0xC8;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_CAP, 3, buf );

      // ------ list of AID's ------
#if 0
      // 1. VISA ADVT -- for DEBUG ONLY (OEP test)
      buf[0] = 0x08;
      buf[1] = 0x00; // partial matching not allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x03;
      buf[7] = 0x10;
      buf[8] = 0x10;
#else
      // 1. Visa ADVT
      buf[0] = 0x08;
      buf[1] = 0x01;	// for VISA ADVT, partial matching allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x03;	// for VISA ADVT AID
      buf[7] = 0x10;
      buf[8] = 0x10;
#endif
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_01, 0x09, buf );

      // 2. MasterCard / Visa ADVT
      buf[0] = 0x08;
      buf[1] = 0x01;	// partial matching allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x04;	// formal EMV AID
      buf[7] = 0x10;
      buf[8] = 0x10;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_02, 0x09, buf );

#if	0
      // 3. Partial
      buf[0] = 0x06;
      buf[1] = 0x01; // partial matching allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x10;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_03, 0x07, buf );
#else
      // 1. CUP -- for DEBUG ONLY
      buf[0] = 0x06;
      buf[1] = 0x01; // partial matching allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x03;
      buf[6] = 0x33;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_03, 0x07, buf );
#endif

      // end of AID list
      buf[0] = 0x00;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_04, 0x01, buf );

      // --- Application Version Number (b2) ---
      buf[0] = 0x00;
      buf[1] = 0x02;
//    api_emv_PutDataElement( DE_TERM, ADDR_AVN_01, AVN_LEN, buf );
//    api_emv_PutDataElement( DE_TERM, ADDR_AVN_02, AVN_LEN, buf );
//    api_emv_PutDataElement( DE_TERM, ADDR_AVN_03, AVN_LEN, buf );
      for( i=0; i<MAX_AVN_CNT; i++ )
         api_emv_PutDataElement( DE_TERM, ADDR_AVN_START+(i*AVN_LEN), AVN_LEN, buf );

      // --- TAC's ---
      memset( buf, 0x00, 5 );
      
#if	0
      // VSDC OEP test
      // default: DC 40 00 A8 00
      buf[0] = 0xDC;
      buf[1] = 0x40;
      buf[2] = 0x00;
      buf[3] = 0xA8;
      buf[4] = 0x00;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
      
      // denial: 00 10 00 00 00
      buf[0] = 0x00;
      buf[1] = 0x10;
      buf[2] = 0x00;
      buf[3] = 0x00;
      buf[4] = 0x00;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
      
      // online: DC 40 04 F8 00
      buf[0] = 0xDC;
      buf[1] = 0x40;
      buf[2] = 0x04;
      buf[3] = 0xF8;
      buf[4] = 0x00;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online
      
      memset( buf, 0x00, 5 );
#else
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online
#endif
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online

      // --- Default TDOL --- (9F 08: AVN)
      buf[0] = 3;
      buf[1] = 0;
      buf[2] = 0x9F;
      buf[3] = 0x08;
      buf[4] = 0x02;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TDOL, 5, buf );

      // --- Default DDOL ---
      buf[0] = 0x0B; // DDOL length
      buf[1] = 0x00; //

      buf[2] = 0x9F; // the unpredictable number
      buf[3] = 0x37; //
      buf[4] = 0x04; //

      buf[5] = 0x9F; // icc public key exponent
      buf[6] = 0x47; //
      buf[7] = 0x01; //

      buf[8] = 0x8F; // CA public key index
      buf[9] = 0x01; //

      buf[10] = 0x9F; // issuer public key exponent
      buf[11] = 0x32; //
      buf[12] = 0x01; //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_DDOL, 13, buf );

      // --- Terminal Country Code (n3) ---
      buf[0] = 0x00;  // BELGIUM
      buf[1] = 0x56;  //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_CNTR_CODE, 2, buf );

      // --- Transaction Currency Code (n3) ---
      buf[0] = 0x09; // iso 4217, BEF=0978 (EURO)
      buf[1] = 0x78; //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_CC, 2, buf );

      // --- Transaction Currency Exponent (n1, imply decimal point from the right of amt) ---
      // the formal rule is to access the exponent according to Currency Code (iso 4217)
      buf[0] = 0x02; // iso 4217, BEF=0056 (Belgium Franc)
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_CE, 1, buf );
      g_term_tx_exp = 2;
      g_term_decimal_point = 2;
      
//      api_sram_putvar( "g_term_tx_exp", sizeof(UCHAR), (UCHAR *)&g_term_tx_exp ); // save it to SRAM
//      api_sram_putvar( "g_term_decimal_point", sizeof(UCHAR), (UCHAR *)&g_term_decimal_point );

      // --- Terminal Floor Limit ---
      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
#if	1
      buf[3] = 0x01; // integer part=100
#else
      buf[3] = 0x00;	// for OEP test
#endif
      buf[4] = 0x00; //
      buf[5] = 0x00; // decimal part=.00

      buf[6] = 0x01; // TFL flag is set (TFL present)

      // update all TFLs to the same value (from AID_01 to AID_16)
      for( i=0; i<MAX_TFL_CNT; i++ )
         {
         api_emv_PutDataElement( DE_TERM, ADDR_TFL_01+TFL_LEN*i, TFL_LEN, buf ); // value
         api_emv_PutDataElement( DE_TERM, ADDR_TFL_FLAG_01+i, 1, &buf[6] ); // flag
         }

//    buf[0] = 0x00;
//    buf[1] = 0x00;
//    buf[2] = 0x00;
//    buf[3] = 0x64;
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_FL, 4, buf );

      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
#if	1
      buf[3] = 0x01; // integer part=100
#else
      buf[3] = 0x00;	// for OEP test
#endif
      buf[4] = 0x00; //
      buf[5] = 0x00; // decimal part=.00
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_01, TFL_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_02, TFL_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_03, TFL_LEN, buf );

      // --- Target Percentage for Random Selection ---
      buf[0] = 0;
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01+RS_TP_LEN*1, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01+RS_TP_LEN*2, 1, buf );

      // --- Max. Target Percentage for Biased Random Selection ---
      buf[0] = 0;
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01+BRS_MTP_LEN*1, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01+BRS_MTP_LEN*2, 1, buf );

      // --- Threshold Value for Biased Random Selection ---
      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x00;
      buf[4] = 0x05; // integer=5
      buf[5] = 0x00; // decimal=.00
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD, BRS_THRESHOLD_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01, BRS_THRESHOLD_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01+BRS_THRESHOLD_LEN*1, BRS_THRESHOLD_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01+BRS_THRESHOLD_LEN*2, BRS_THRESHOLD_LEN, buf );

      // --- CAPK Revocation List ---
      buf[0] = 0xA0; // RID
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x00;
      buf[4] = 0x04;

      buf[5] = 0xF8; // CAPKI

      buf[6] = 0x00; // certificate serial number
      buf[7] = 0x10;
      buf[8] = 0x00;
      api_emv_PutDataElement( DE_KEY, ADDR_CAPK_REVOCATION_LIST_01, CAPK_REVOC_LEN, buf );

      memset( buf, 0x00, 9 ); // end of list
      api_emv_PutDataElement( DE_KEY, ADDR_CAPK_REVOCATION_LIST_02, CAPK_REVOC_LEN, buf );

      g_dhn_pinpad = 0; // pinpad works
}

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 2. (EMV L2)
//
// TAC denial  = 00 00 00 00 00
// TAC online  = F8 F8 FC F8 F0
// TAC default = 00 00 00 00 00
//
// Default DDOL not present
//
// If Random Transaction Selection process supported:
//    Target and Maximum Target percentage for random selection set to 99 to avoid random behaviour.
//
// If Random Transaction Selection process supported:
//    Threshold value for biased random selection shall be set to 5
//
// PIN Pad is malfunctioning
//
// (unless otherwise specified, other parameters are the same as in terminal configuration 1)
// ---------------------------------------------------------------------------
void SETUP_config_02( void )
{
UINT  i;
UCHAR buf[64];


      SETUP_config_01();

      // --- Terminal Capabilities ---
      // for ADVT Test Card #29: Offline Plaintext PIN
      buf[0] = 0xE0;
      buf[1] = 0xE8; // offline plaintext PIN + online enciphered PIN + signature + no CVM required
      buf[2] = 0xC8;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_CAP, 3, buf );
      
      return;
      

      // --- TAC's ---
      memset( buf, 0x00, 5 );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial

      buf[0] = 0xF8;
      buf[1] = 0xF8;
      buf[2] = 0xFC;
      buf[3] = 0xF8;
      buf[4] = 0xF0;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online

      // --- Default DDOL ---
      buf[0] = 0x00; // DDOL not present
      buf[1] = 0x00; //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_DDOL, 2, buf );

      // --- Target Percentage for Random Selection ---
      buf[0] = 0x99;
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01+RS_TP_LEN*1, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01+RS_TP_LEN*2, 1, buf );

      // --- Max. Target Percentage for Biased Random Selection ---
      buf[0] = 0x99;
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01+BRS_MTP_LEN*1, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01+BRS_MTP_LEN*2, 1, buf );

      // --- PIN PAD ---
      g_dhn_pinpad = apiDeviceError; // malfunctioning
}

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 3. (EMV L2)
//
// TAC denial  = 40 00 00 00 00 (if SDA supported) or
//               00 20 00 00 00 (if SDA not supported)
// TAC online  = 00 00 00 00 00
// TAC default = 00 00 00 00 00
//
// default DDOL = 9F 47 01 8F 01 9F 32 01 (no unpredictable number)
//
// Terminal floor limit not present
//
// (unless otherwise specified, other parameters are the same as in terminal configuration 1)
// ---------------------------------------------------------------------------
void SETUP_config_03( void )
{
UINT  i;
UCHAR buf[64];


      SETUP_config_01();

      // --- Terminal Capabilities ---
      // for ADVT Test Card #29: Online Enciphered PIN
      buf[0] = 0xE0;
      buf[1] = 0x48; // online enciphered PIN + no CVM required
      buf[2] = 0xC8;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_CAP, 3, buf );
      
      return;
      

      // --- TAC's ---
      buf[0] = 0x40;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x00;
      buf[4] = 0x00;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial

      memset( buf, 0x00, 5 );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf ); // TAC-Default

      // --- Default DDOL ---
      buf[0] = 0x08; // DDOL length
      buf[1] = 0x00; //

      buf[2] = 0x9F; // icc public key exponent
      buf[3] = 0x47; //
      buf[4] = 0x01; //

      buf[5] = 0x8F; // CA public key index
      buf[6] = 0x01; //

      buf[7] = 0x9F; // issuer public key exponent
      buf[8] = 0x32; //
      buf[9] = 0x01; //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_DDOL, 10, buf );

      // --- Terminal Floor Limit ---
      memset( buf, 0x00, TFL_LEN );
      buf[6] = 0x00; // TFL flag is cleared (TFL not present)

      // update all TFLs to the same value (from AID_01 to AID_16)
      for( i=0; i<MAX_TFL_CNT; i++ )
         {
         api_emv_PutDataElement( DE_TERM, ADDR_TFL_01+TFL_LEN*i, TFL_LEN, buf ); // value
         api_emv_PutDataElement( DE_TERM, ADDR_TFL_FLAG_01+i, 1, &buf[6] ); // flag
         }

//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_FL, 4, buf );

      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x00; // integer part= 0
      buf[4] = 0x00; //
      buf[5] = 0x00; // decimal part=.00
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_01, TFL_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_02, TFL_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_03, TFL_LEN, buf );
}

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 4. (for PCI Online Enciphered PIN test using Collis Tool)
//		VISA, T=1
//
// ---------------------------------------------------------------------------
void SETUP_config_04( void )
{
UINT  i;
UCHAR buf[64];


      // --- Terminal Capabilities ---
      // for ADVT Test Card #29: Offline Enciphered PIN
      buf[0] = 0xE0;
      buf[1] = 0xF8; // offline plaintext PIN + online enciphered PIN + signature + offline enciphered PIN + no CVM required
//    buf[1] = 0x10; // offline enciphered PIN
      buf[2] = 0xC8;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_CAP, 3, buf );

      // ------ list of AID's ------
#if 0
      // 1. VISA ADVT -- for DEBUG ONLY (OEP test)
      buf[0] = 0x08;
      buf[1] = 0x00; // partial matching not allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x03;
      buf[7] = 0x10;
      buf[8] = 0x10;
#else
      // 1. Visa ADVT
      buf[0] = 0x08;
      buf[1] = 0x01;	// for VISA ADVT, partial matching allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x03;	// for VISA ADVT AID
      buf[7] = 0x10;
      buf[8] = 0x10;
#endif
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_01, 0x09, buf );

      // 2. MasterCard / Visa ADVT
      buf[0] = 0x08;
      buf[1] = 0x01;	// partial matching allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x04;	// formal EMV AID
      buf[7] = 0x10;
      buf[8] = 0x10;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_02, 0x09, buf );

#if	0
      // 3. Partial
      buf[0] = 0x06;
      buf[1] = 0x01; // partial matching allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x10;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_03, 0x07, buf );
#else
      // 1. CUP -- for DEBUG ONLY
      buf[0] = 0x06;
      buf[1] = 0x01; // partial matching allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x03;
      buf[6] = 0x33;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_03, 0x07, buf );
#endif

      // end of AID list
      buf[0] = 0x00;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_04, 0x01, buf );

      // --- Application Version Number (b2) ---
      buf[0] = 0x00;
      buf[1] = 0x02;
//    api_emv_PutDataElement( DE_TERM, ADDR_AVN_01, AVN_LEN, buf );
//    api_emv_PutDataElement( DE_TERM, ADDR_AVN_02, AVN_LEN, buf );
//    api_emv_PutDataElement( DE_TERM, ADDR_AVN_03, AVN_LEN, buf );
      for( i=0; i<MAX_AVN_CNT; i++ )
         api_emv_PutDataElement( DE_TERM, ADDR_AVN_START+(i*AVN_LEN), AVN_LEN, buf );

      // --- TAC's ---
      memset( buf, 0x00, 5 );
      
#if	1
      // VSDC OEP test
      // default: DC 40 00 A8 00
      buf[0] = 0xDC;
      buf[1] = 0x40;
      buf[2] = 0x00;
      buf[3] = 0xA8;
      buf[4] = 0x00;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
      
      // denial: 00 10 00 00 00
      buf[0] = 0x00;
      buf[1] = 0x10;
      buf[2] = 0x00;
      buf[3] = 0x00;
      buf[4] = 0x00;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
      
      // online: DC 40 04 F8 00
      buf[0] = 0xDC;
      buf[1] = 0x40;
      buf[2] = 0x04;
      buf[3] = 0xF8;
      buf[4] = 0x00;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online
      
      memset( buf, 0x00, 5 );
#else
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online
#endif
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online

      // --- Default TDOL --- (9F 08: AVN)
      buf[0] = 3;
      buf[1] = 0;
      buf[2] = 0x9F;
      buf[3] = 0x08;
      buf[4] = 0x02;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TDOL, 5, buf );

      // --- Default DDOL ---
      buf[0] = 0x0B; // DDOL length
      buf[1] = 0x00; //

      buf[2] = 0x9F; // the unpredictable number
      buf[3] = 0x37; //
      buf[4] = 0x04; //

      buf[5] = 0x9F; // icc public key exponent
      buf[6] = 0x47; //
      buf[7] = 0x01; //

      buf[8] = 0x8F; // CA public key index
      buf[9] = 0x01; //

      buf[10] = 0x9F; // issuer public key exponent
      buf[11] = 0x32; //
      buf[12] = 0x01; //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_DDOL, 13, buf );

      // --- Terminal Country Code (n3) ---
      buf[0] = 0x00;  // BELGIUM
      buf[1] = 0x56;  //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_CNTR_CODE, 2, buf );

      // --- Transaction Currency Code (n3) ---
      buf[0] = 0x09; // iso 4217, BEF=0978 (EURO)
      buf[1] = 0x78; //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_CC, 2, buf );

      // --- Transaction Currency Exponent (n1, imply decimal point from the right of amt) ---
      // the formal rule is to access the exponent according to Currency Code (iso 4217)
      buf[0] = 0x02; // iso 4217, BEF=0056 (Belgium Franc)
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_CE, 1, buf );
      g_term_tx_exp = 2;
      g_term_decimal_point = 2;
      
//      api_sram_putvar( "g_term_tx_exp", sizeof(UCHAR), (UCHAR *)&g_term_tx_exp ); // save it to SRAM
//      api_sram_putvar( "g_term_decimal_point", sizeof(UCHAR), (UCHAR *)&g_term_decimal_point );

      // --- Terminal Floor Limit ---
      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
#if	0
      buf[3] = 0x01; // integer part=100
#else
      buf[3] = 0x00;	// for OEP test
#endif
      buf[4] = 0x00; //
      buf[5] = 0x00; // decimal part=.00

      buf[6] = 0x01; // TFL flag is set (TFL present)

      // update all TFLs to the same value (from AID_01 to AID_16)
      for( i=0; i<MAX_TFL_CNT; i++ )
         {
         api_emv_PutDataElement( DE_TERM, ADDR_TFL_01+TFL_LEN*i, TFL_LEN, buf ); // value
         api_emv_PutDataElement( DE_TERM, ADDR_TFL_FLAG_01+i, 1, &buf[6] ); // flag
         }

//    buf[0] = 0x00;
//    buf[1] = 0x00;
//    buf[2] = 0x00;
//    buf[3] = 0x64;
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_FL, 4, buf );

      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
#if	0
      buf[3] = 0x01; // integer part=100
#else
      buf[3] = 0x00;	// for OEP test
#endif
      buf[4] = 0x00; //
      buf[5] = 0x00; // decimal part=.00
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_01, TFL_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_02, TFL_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_03, TFL_LEN, buf );

      // --- Target Percentage for Random Selection ---
      buf[0] = 0;
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01+RS_TP_LEN*1, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01+RS_TP_LEN*2, 1, buf );

      // --- Max. Target Percentage for Biased Random Selection ---
      buf[0] = 0;
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01+BRS_MTP_LEN*1, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01+BRS_MTP_LEN*2, 1, buf );

      // --- Threshold Value for Biased Random Selection ---
      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x00;
      buf[4] = 0x05; // integer=5
      buf[5] = 0x00; // decimal=.00
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD, BRS_THRESHOLD_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01, BRS_THRESHOLD_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01+BRS_THRESHOLD_LEN*1, BRS_THRESHOLD_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01+BRS_THRESHOLD_LEN*2, BRS_THRESHOLD_LEN, buf );

      // --- CAPK Revocation List ---
      buf[0] = 0xA0; // RID
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x00;
      buf[4] = 0x04;

      buf[5] = 0xF8; // CAPKI

      buf[6] = 0x00; // certificate serial number
      buf[7] = 0x10;
      buf[8] = 0x00;
      api_emv_PutDataElement( DE_KEY, ADDR_CAPK_REVOCATION_LIST_01, CAPK_REVOC_LEN, buf );

      memset( buf, 0x00, 9 ); // end of list
      api_emv_PutDataElement( DE_KEY, ADDR_CAPK_REVOCATION_LIST_02, CAPK_REVOC_LEN, buf );

      g_dhn_pinpad = 0; // pinpad works
}

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 4. (EMV L2)
//
// TAC denial  = 00 00 00 00 00
// TAC online  = 40 00 00 00 00 (if SDA supported) or
//               00 20 00 00 00 (if SDA not supported)
// TAC default = 00 00 00 00 00
//
// Terminal floor limit not present
//
// (unless otherwise specified, other parameters are the same as in terminal configuration 1)
// ---------------------------------------------------------------------------
#if	0
void SETUP_config_04( void )
{
UINT  i;
UCHAR buf[64];

//    TL_DispHexByte(0,18,0x04);
//    UI_WaitKey();

      SETUP_config_01();

      // --- TAC's ---
      buf[0] = 0x40;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x00;
      buf[4] = 0x00;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online

      memset( buf, 0x00, 5 );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf ); // TAC-Default

      // --- Terminal Floor Limit ---
      memset( buf, 0x00, TFL_LEN );
      buf[6] = 0x00; // TFL flag is cleared (TFL not present)

      // update all TFLs to the same value (from AID_01 to AID_16)
      for( i=0; i<MAX_TFL_CNT; i++ )
         {
         api_emv_PutDataElement( DE_TERM, ADDR_TFL_01+TFL_LEN*i, TFL_LEN, buf ); // value
         api_emv_PutDataElement( DE_TERM, ADDR_TFL_FLAG_01+i, 1, &buf[6] ); // flag
         }

//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_FL, 4, buf );

      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x00; // integer part= 0
      buf[4] = 0x00; //
      buf[5] = 0x00; // decimal part=.00
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_01, TFL_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_02, TFL_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_03, TFL_LEN, buf );
}
#endif

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 5. (EMV L2)
//
// TAC denial  = 00 00 00 00 00
// TAC online  = F0 E0 64 F8 00
// TAC default = 40 00 00 00 00 (if SDA supported) or
//               00 20 00 00 00 (if SDA not supported)
//
// Default TDOL is not present
//
// (unless otherwise specified, other parameters are the same as in terminal configuration 1)
// ---------------------------------------------------------------------------
void SETUP_config_05( void )
{
UINT  i;
UCHAR buf[64];

//    TL_DispHexByte(0,18,0x05);
//    UI_WaitKey();

      SETUP_config_01();

      // --- TAC's ---
      buf[0] = 0x40;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x00;
      buf[4] = 0x00;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf ); // TAC-Default

      buf[0] = 0xF0;
      buf[1] = 0xE0;
      buf[2] = 0x64;
      buf[3] = 0xF8;
      buf[4] = 0x00;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf ); // TAC-Online
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf ); // TAC-Online
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf ); // TAC-Online

      memset( buf, 0x00, 5 );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial

      // --- Default TDOL --- (not used)
      buf[0] = 0;
      buf[1] = 0;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TDOL, 2, buf );
}

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 6. (EMV L2)
//
// TAC denial  = 00 00 00 00 00
// TAC online  = F8 F8 FC F8 F0
// TAC default = 40 00 00 00 00 (if SDA supported) or
//               00 20 00 00 00 (if SDA not supported)
// (unless otherwise specified, other parameters are the same as in terminal configuration 1)
// ---------------------------------------------------------------------------
void SETUP_config_06( void )
{
UINT  i;
UCHAR buf[64];

//    TL_DispHexByte(0,18,0x06);
//    UI_WaitKey();

      SETUP_config_01();

      // --- TAC's ---
      memset( buf, 0x00, 5 );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial

      buf[0] = 0xF8;
      buf[1] = 0xF8;
      buf[2] = 0xFC;
      buf[3] = 0xF8;
      buf[4] = 0xF0;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online

      buf[0] = 0x40;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x00;
      buf[4] = 0x00;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf ); // TAC-Default
}

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 7. (EMV L2)
//
// TAC denial  = 00 00 00 00 00
// TAC online  = 00 00 00 00 00
// TAC default = B8 F8 FC F8 F0 (if SDA supported) or
//               78 D8 FC F8 F0 (if SDA not supported)
//
// (unless otherwise specified, other parameters are the same as in terminal configuration 1)
// ---------------------------------------------------------------------------
void SETUP_config_07( void )
{
UINT  i;
UCHAR buf[64];

//    TL_DispHexByte(0,18,0x07);
//    UI_WaitKey();

      SETUP_config_01();

      // --- TAC's ---
      memset( buf, 0x00, 5 );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online

      buf[0] = 0xB8;
      buf[1] = 0xF8;
      buf[2] = 0xFC;
      buf[3] = 0xF8;
      buf[4] = 0xF0;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf ); // TAC-Default
}

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 8. (EMV L2)
//
// TAC denial  = 07 07 03 07 0F
// TAC online  = 00 00 00 00 00
// TAC default = 00 00 00 00 00
//
// (unless otherwise specified, other parameters are the same as in terminal configuration 1)
// ---------------------------------------------------------------------------
void SETUP_config_08( void )
{
UINT  i;
UCHAR buf[64];

//    TL_DispHexByte(0,18,0x08);
//    UI_WaitKey();

      SETUP_config_01();

      // --- TAC's ---
      buf[0] = 0x07;
      buf[1] = 0x07;
      buf[2] = 0x03;
      buf[3] = 0x07;
      buf[4] = 0x0F;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial

      memset( buf, 0x00, 5 );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf ); // TAC-Default
}

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 9. (EMV L2)
//
// TAC denial  = B8 F8 FC F8 F0 (if SDA supported) or
//               78 D8 FC F8 F0 (if SDA not supported)
// TAC online  = 00 00 00 00 00
// TAC default = 00 00 00 00 00
//
// ---------------------------------------------------------------------------
void SETUP_config_09( void )
{
UINT  i;
UCHAR buf[64];

//    TL_DispHexByte(0,18,0x09);
//    UI_WaitKey();

      SETUP_config_01();

      // --- TAC's ---
      buf[0] = 0xB8;
      buf[1] = 0xF8;
      buf[2] = 0xFC;
      buf[3] = 0xF8;
      buf[4] = 0xF0;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial

      memset( buf, 0x00, 5 );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf ); // TAC-Default
}

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 10. (EMV L2)
//
// TAC denial  = 00 00 00 00 00
// TAC online  = B8 F8 FC F8 F0 (if SDA supported) or
//               78 D8 FC F8 F0 (if SDA not supported)
// TAC default = 00 00 00 00 00
//
// (unless otherwise specified, other parameters are the same as in terminal configuration 1)
// ---------------------------------------------------------------------------
void SETUP_config_10( void )
{
UINT  i;
UCHAR buf[64];

//    TL_DispHexByte(0,18,0x10);
//    UI_WaitKey();

      SETUP_config_01();

      // --- TAC's ---
      buf[0] = 0xB8;
      buf[1] = 0xF8;
      buf[2] = 0xFC;
      buf[3] = 0xF8;
      buf[4] = 0xF0;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online

      memset( buf, 0x00, 5 );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf ); // TAC-Default
}

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 11. (EMV L2)
//
// TAC denial  = 00 00 00 00 00
// TAC online  = F8 F8 FC F8 F0
// TAC default = B8 F8 FC F8 F0 (if SDA supported) or
//               78 D8 FC F8 F0 (if SDA not supported)
//
// (unless otherwise specified, other parameters are the same as in terminal configuration 1)
// ---------------------------------------------------------------------------
void SETUP_config_11( void )
{
UINT  i;
UCHAR buf[64];

//    TL_DispHexByte(0,18,0x11);
//    UI_WaitKey();

      SETUP_config_01();

      // --- TAC's ---
      memset( buf, 0x00, 5 );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial

      buf[0] = 0xF8;
      buf[1] = 0xF8;
      buf[2] = 0xFC;
      buf[3] = 0xF8;
      buf[4] = 0xF0;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online

      buf[0] = 0xB8;
      buf[1] = 0xF8;
      buf[2] = 0xFC;
      buf[3] = 0xF8;
      buf[4] = 0xF0;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf ); // TAC-Default
}

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 12. (EMV L2)
//
// TAC denial  = (Not Present)
// TAC online  = 00 00 00 00 00
// TAC default = 00 00 00 00 00
//
// (terminal is unable to go online, no response received from issuer)
// ---------------------------------------------------------------------------
void SETUP_config_12( void )
{
UINT  i;
UCHAR buf[64];

//    TL_DispHexByte(0,18,0x12);
//    UI_WaitKey();

      SETUP_config_01();

      // --- TAC's ---
      memset( buf, 0x00, 5 );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf );  // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf );  // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf );  // TAC-Default

      // TCA Denial -- Not Present !!!
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial
}

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 13. (EMV L2)
//
// TAC denial  = 00 00 00 00 00
// TAC online  = 00 00 00 00 00
// TAC default = (Not Present)
//
// (terminal is unable to go online, no response received from issuer)
// ---------------------------------------------------------------------------
void SETUP_config_13( void )
{
UINT  i;
UCHAR buf[64];

//    TL_DispHexByte(0,18,0x13);
//    UI_WaitKey();

      SETUP_config_01();

      // --- TAC's ---
      memset( buf, 0x00, 5 );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial

      // TCA Default -- Not Present !!!
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf );  // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf );  // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf );  // TAC-Default
}

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 14. (EMV L2)
//
// TAC denial  = 00 00 00 00 00
// TAC online  = (Not Present)
// TAC default = 00 00 00 00 00
//
// (terminal is unable to go online, no response received from issuer)
// ---------------------------------------------------------------------------
void SETUP_config_14( void )
{
UINT  i;
UCHAR buf[64];

//    TL_DispHexByte(0,18,0x14);
//    UI_WaitKey();

      SETUP_config_01();

      // --- TAC's ---
      memset( buf, 0x00, 5 );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf );  // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf );  // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf );  // TAC-Default

      // TCA Online -- Not Present !!!
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online
}

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 15. (EMV L2)
//
// List of AID's in the following sequence order: All AIDs are partial.
// 1 Non EMV   : B0 12 34 56 78 12
//               (Application Selection Indicator indicates that multiples
//               occurences of the Application within the card are not allowed)
// 2 MasterCard: A0 00 00 00 04 10 10
//               (Application Selection Indicator indicates that multiples
//               occurences of the Application within the card are not allowed)
// 3 Partial   : A0 00 00 00 10
//               (Application Selection Indicator indicates that multiples
//               occurences of the Application within the card are allowed)
//
// Terminal Application Version Number: 00 02
//               (if different values are supported, all values set to 00 02)
//
// All TAC's set to 0
//
// Default TDOL if used: ICC Application Version Number ( 9F 08)
//
// Default DDOL: 9F 37 04 9F 47 01 8F 01 9F 32 01
//
// Terminal Country Code: 00 56
//
// Transaction Currency Code: 09 78 (EURO)
//
// Transaction Currency Exponent: 02
//
// Terminal Floor Limit: 100 (=100.0 or 100.00 or 100.000 according to the number of decimal values)
//
// ---------------------------------------------------------------------------
void SETUP_config_15( void )
{
UINT  i;
UCHAR buf[64];

//    TL_DispHexByte(0,18,0x15);
//    UI_WaitKey();

      SETUP_config_01();

      // ------ list of AID's ------
      // 1. Non EMV
      buf[0] = 0x07;
      buf[1] = 0x00; // partial matching not allowed
      buf[2] = 0xB0;
      buf[3] = 0x12;
      buf[4] = 0x34;
      buf[5] = 0x56;
      buf[6] = 0x78;
      buf[7] = 0x12;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_01, 0x08, buf );

      // 2. MasterCard
      buf[0] = 0x08;
      buf[1] = 0x00; // partial matching not allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x04;
      buf[7] = 0x10;
      buf[8] = 0x10;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_02, 0x09, buf );

      // 3. Partial
      buf[0] = 0x06;
      buf[1] = 0x01; // partial matching allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x10;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_03, 0x07, buf );

      // end of AID list
      buf[0] = 0x00;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_04, 0x01, buf );

}

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 16. (EMV L2)
//
// List of AID's in the following sequence order: NEPI has a partial AID
// 1 Non EMV   : B0 12 34 56 78 12
//               (Application Selection Indicator indicates that multiples
//               occurences of the Application within the card are not allowed)
// 2 MasterCard: A0 00 00 00 04 10 10
//               (Application Selection Indicator indicates that multiples
//               occurences of the Application within the card are not allowed)
// 3 Partial   : A0 00 00 00 10
//               (Application Selection Indicator indicates that multiples
//               occurences of the Application within the card are allowed)
//
// Terminal Application Version Number: 00 02
//               (if different values are supported, all values set to 00 02)
//
// All TAC's set to 0
//
// Default TDOL if used: ICC Application Version Number ( 9F 08)
//
// Default DDOL: 9F 37 04 9F 47 01 8F 01 9F 32 01
//
// Terminal Country Code: 00 56
//
// Transaction Currency Code: 09 78 (EURO)
//
// Transaction Currency Exponent: 02
//
// Terminal Floor Limit: 100 (=100.0 or 100.00 or 100.000 according to the number of decimal values)
//
// ---------------------------------------------------------------------------
void SETUP_config_16( void )
{
UINT  i;
UCHAR buf[64];

//    TL_DispHexByte(0,18,0x16);
//    UI_WaitKey();

      SETUP_config_01();

      // ------ list of AID's ------
      // 1. Non EMV
      buf[0] = 0x07;
      buf[1] = 0x00; // partial matching not allowed
      buf[2] = 0xB0;
      buf[3] = 0x12;
      buf[4] = 0x34;
      buf[5] = 0x56;
      buf[6] = 0x78;
      buf[7] = 0x12;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_01, 0x08, buf );

      // 2. MasterCard
      buf[0] = 0x08;
      buf[1] = 0x00; // partial matching not allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x04;
      buf[7] = 0x10;
      buf[8] = 0x10;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_02, 0x09, buf );

      // 3. Partial
      buf[0] = 0x06;
      buf[1] = 0x01; // partial matching allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x10;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_03, 0x07, buf );

      // end of AID list
      buf[0] = 0x00;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_04, 0x01, buf );

}

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 17. (EMV L2)
//
// List of AID's in the following sequence order: NEPI has a partial AID
// 1 Non EMV   : B0 12 34 56 78
//               (Application Selection Indicator indicates that multiples
//               occurences of the Application within the card are allowed)
// 2 MasterCard: A0 00 00 00 04
//               (Application Selection Indicator indicates that multiples
//               occurences of the Application within the card are allowed)
// 3 Partial   : A0 00 00 00 10
//               (Application Selection Indicator indicates that multiples
//               occurences of the Application within the card are allowed)
//
// Terminal Application Version Number: 00 02
//               (if different values are supported, all values set to 00 02)
//
// All TAC's set to 0
//
// Default TDOL if used: ICC Application Version Number ( 9F 08)
//
// Default DDOL: 9F 37 04 9F 47 01 8F 01 9F 32 01
//
// Terminal Country Code: 00 56
//
// Transaction Currency Code: 09 78 (EURO)
//
// Transaction Currency Exponent: 02
//
// Terminal Floor Limit: 100 (=100.0 or 100.00 or 100.000 according to the number of decimal values)
//
// ---------------------------------------------------------------------------
void SETUP_config_17( void )
{
UINT  i;
UCHAR buf[64];

//    TL_DispHexByte(0,18,0x16);
//    UI_WaitKey();

      SETUP_config_01();

      // ------ list of AID's ------
      // 1. Non EMV
      buf[0] = 0x06;
      buf[1] = 0x01; // partial matching allowed
      buf[2] = 0xB0;
      buf[3] = 0x12;
      buf[4] = 0x34;
      buf[5] = 0x56;
      buf[6] = 0x78;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_01, 0x07, buf );

      // 2. MasterCard
      buf[0] = 0x06;
      buf[1] = 0x01; // partial matching allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x04;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_02, 0x07, buf );

      // 3. Partial
      buf[0] = 0x06;
      buf[1] = 0x01; // partial matching allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x10;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_03, 0x07, buf );

      // end of AID list
      buf[0] = 0x00;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_04, 0x01, buf );

}

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 18. (EMV L2)
//
// List of AID's in the following sequence order:
// 1. A0 00 00 00 04 10 10
// 2. A0 00 00 00 10
// 3. A0 00 00 00 04 10 14
// 4. A0 00 00 00 04 10 12
// 5. B0 12 34 56 78 12 34
//
// Terminal Application Version Number: 00 02
//
// All TAC's set to 0
//
// Default TDOL if used: ICC Application Version Number ( 9F 08)
//
// Default DDOL: 9F 37 04 9F 47 01 8F 01 9F 32 01
//
// Terminal Country Code: 00 56
//
// Transaction Currency Code: 09 78 (EURO)
//
// Transaction Currency Exponent: 02
//
// Terminal Floor Limit: 100 (=100.0 or 100.00 or 100.000 according to the number of decimal values)
//
// If Random Transaction Selection process supported:
//    Target and Maximum Target percentage for random selection set to 0 to avoid random behaviour.
//
// ---------------------------------------------------------------------------
void SETUP_config_18( void )
{
UINT  i;
UCHAR buf[64];

//    TL_DispHexByte(0,18,0x18);
//    UI_WaitKey();

      // ------ list of AID's ------
      // 1. A0 00 00 00 04 10 10
      buf[0] = 0x08;
      buf[1] = 0x00; // partial matching not allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x04;
      buf[7] = 0x10;
      buf[8] = 0x10;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_01, 0x09, buf );

      // 2. A0 00 00 00 10
      buf[0] = 0x06;
      buf[1] = 0x01; // partial matching allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x10;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_02, 0x07, buf );

      // 3. A0 00 00 00 04 10 14
      buf[0] = 0x08;
      buf[1] = 0x00; // partial matching not allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x04;
      buf[7] = 0x10;
      buf[8] = 0x14;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_03, 0x09, buf );

      // 4. A0 00 00 00 04 10 12
      buf[0] = 0x08;
      buf[1] = 0x00; // partial matching not allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x04;
      buf[7] = 0x10;
      buf[8] = 0x12;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_04, 0x09, buf );

      // 5. B0 12 34 56 78 12 34
      buf[0] = 0x08;
      buf[1] = 0x00; // partial matching not allowed
      buf[2] = 0xB0;
      buf[3] = 0x12;
      buf[4] = 0x34;
      buf[5] = 0x56;
      buf[6] = 0x78;
      buf[7] = 0x12;
      buf[8] = 0x34;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_05, 0x09, buf );

      // end of AID list
      buf[0] = 0x00;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_06, 0x01, buf );

      // --- Application Version Number (b2) ---
      buf[0] = 0x00;
      buf[1] = 0x02;
//    api_emv_PutDataElement( DE_TERM, ADDR_AVN_01, AVN_LEN, buf );
//    api_emv_PutDataElement( DE_TERM, ADDR_AVN_02, AVN_LEN, buf );
//    api_emv_PutDataElement( DE_TERM, ADDR_AVN_03, AVN_LEN, buf );
      for( i=0; i<MAX_AVN_CNT; i++ )
         api_emv_PutDataElement( DE_TERM, ADDR_AVN_START+(i*AVN_LEN), AVN_LEN, buf );

      // --- TAC's ---
      memset( buf, 0x00, 5 );

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_04, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_04, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_04, 5, buf );  // TAC-Online

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_05, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_05, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_05, 5, buf );  // TAC-Online

      // --- Default TDOL --- (9F 08: AVN)
      buf[0] = 3;
      buf[1] = 0;
      buf[2] = 0x9F;
      buf[3] = 0x08;
      buf[4] = 0x02;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TDOL, 5, buf );

      // --- Default DDOL ---
      buf[0] = 0x0B; // DDOL length
      buf[1] = 0x00; //

      buf[2] = 0x9F; // the unpredictable number
      buf[3] = 0x37; //
      buf[4] = 0x04; //

      buf[5] = 0x9F; // icc public key exponent
      buf[6] = 0x47; //
      buf[7] = 0x01; //

      buf[8] = 0x8F; // CA public key index
      buf[9] = 0x01; //

      buf[10] = 0x9F; // issuer public key exponent
      buf[11] = 0x32; //
      buf[12] = 0x01; //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_DDOL, 13, buf );

      // --- Terminal Country Code (n3) ---
      buf[0] = 0x00;  // BELGIUM
      buf[1] = 0x56;  //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_CNTR_CODE, 2, buf );

      // --- Transaction Currency Code (n3) ---
      buf[0] = 0x09; // iso 4217, BEF=0978 (EURO)
      buf[1] = 0x78; //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_CC, 2, buf );

      // --- Transaction Currency Exponent (n1, imply decimal point from the right of amt) ---
      // the formal rule is to access the exponent according to Currency Code (iso 4217)
      buf[0] = 0x02; // iso 4217, BEF=0056 (Belgium Franc)
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_CE, 1, buf );
      g_term_tx_exp = 2;
      g_term_decimal_point = 2;

      // --- Terminal Floor Limit ---
      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x01; // integer part=100
      buf[4] = 0x00; //
      buf[5] = 0x00; // decimal part=.00

      buf[6] = 0x01; // TFL flag is set (TFL present)

      // update all TFLs to the same value (from AID_01 to AID_16)
      for( i=0; i<MAX_TFL_CNT; i++ )
         {
         api_emv_PutDataElement( DE_TERM, ADDR_TFL_01+TFL_LEN*i, TFL_LEN, buf ); // value
         api_emv_PutDataElement( DE_TERM, ADDR_TFL_FLAG_01+i, 1, &buf[6] ); // flag
         }

//    buf[0] = 0x00;
//    buf[1] = 0x00;
//    buf[2] = 0x00;
//    buf[3] = 0x64;
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_FL, 4, buf );

      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x01; // integer part=100
      buf[4] = 0x00; //
      buf[5] = 0x00; // decimal part=.00
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_01, TFL_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_02, TFL_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_03, TFL_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_04, TFL_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_05, TFL_LEN, buf );

      // --- Target Percentage for Random Selection ---
      buf[0] = 0;
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01+RS_TP_LEN*1, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01+RS_TP_LEN*2, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01+RS_TP_LEN*3, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01+RS_TP_LEN*4, 1, buf );

      // --- Max. Target Percentage for Biased Random Selection ---
      buf[0] = 0;
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01+BRS_MTP_LEN*1, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01+BRS_MTP_LEN*2, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01+BRS_MTP_LEN*3, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01+BRS_MTP_LEN*4, 1, buf );

      // --- Threshold Value for Biased Random Selection ---
      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x00;
      buf[4] = 0x05; // integer=5
      buf[5] = 0x00; // decimal=.00
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD, BRS_THRESHOLD_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01, BRS_THRESHOLD_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01+BRS_THRESHOLD_LEN*1, BRS_THRESHOLD_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01+BRS_THRESHOLD_LEN*2, BRS_THRESHOLD_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01+BRS_THRESHOLD_LEN*3, BRS_THRESHOLD_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01+BRS_THRESHOLD_LEN*4, BRS_THRESHOLD_LEN, buf );

      g_dhn_pinpad = 0; // pinpad works
}

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 19. (EMV L2)
//
// List of AID's in the following sequence order:
// 1 Non EMV   : B0 12 34 56 78 12 34
//               (Application Selection Indicator indicates that multiples
//               occurences of the Application within the card are not allowed)
// 2 MasterCard: A0 00 00 00 04 10 10
//               (Application Selection Indicator indicates that multiples
//               occurences of the Application within the card are not allowed)
// 3 Partial   : A0 00 00 00 10
//               (Application Selection Indicator indicates that multiples
//               occurences of the Application within the card are allowed)
//
// Terminal Application Version Number: 00 02
//
// All TAC's set to 0
//
// Default TDOL if used: ICC Application Version Number ( 9F 08)
//
// Default TDOL: not present
//
// Default DDOL: 9F 37 04
//
// Terminal Country Code: 00 56
//
// Transaction Currency Code: 09 78 (EURO)
//
// Transaction Currency Exponent: 02
//
// Terminal Floor Limit: 100 (=100.0 or 100.00 or 100.000 according to the number of decimal values)
//
// If Random Transaction Selection process supported:
//    Target and Maximum Target percentage for random selection set to 0 to avoid random behaviour.
//
// ---------------------------------------------------------------------------
void SETUP_config_19( void )
{
UINT  i;
UCHAR buf[64];

//    TL_DispHexByte(0,18,0x19);
//    UI_WaitKey();

      // ------ list of AID's ------
      // 1. Non EMV
      buf[0] = 0x08;
      buf[1] = 0x00; // partial matching not allowed
      buf[2] = 0xB0;
      buf[3] = 0x12;
      buf[4] = 0x34;
      buf[5] = 0x56;
      buf[6] = 0x78;
      buf[7] = 0x12;
      buf[8] = 0x34;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_01, 0x09, buf );

      // 2. A0 00 00 00 04 10 10
      buf[0] = 0x08;
      buf[1] = 0x00; // partial matching not allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x04;
      buf[7] = 0x10;
      buf[8] = 0x10;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_02, 0x09, buf );

      // 3. A0 00 00 00 10
      buf[0] = 0x06;
      buf[1] = 0x01; // partial matching allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x10;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_03, 0x07, buf );

      // end of AID list
      buf[0] = 0x00;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_04, 0x01, buf );

      // --- Application Version Number (b2) ---
      buf[0] = 0x00;
      buf[1] = 0x02;
//    api_emv_PutDataElement( DE_TERM, ADDR_AVN_01, AVN_LEN, buf );
//    api_emv_PutDataElement( DE_TERM, ADDR_AVN_02, AVN_LEN, buf );
//    api_emv_PutDataElement( DE_TERM, ADDR_AVN_03, AVN_LEN, buf );
      for( i=0; i<MAX_AVN_CNT; i++ )
         api_emv_PutDataElement( DE_TERM, ADDR_AVN_START+(i*AVN_LEN), AVN_LEN, buf );

      // --- TAC's ---
      memset( buf, 0x00, 5 );

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online

      // --- Default TDOL --- (not present)
      buf[0] = 0;
      buf[1] = 0;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TDOL, 2, buf );

      // --- Default DDOL ---
      buf[0] = 0x03; // DDOL length
      buf[1] = 0x00; //

      buf[2] = 0x9F; // the unpredictable number
      buf[3] = 0x37; //
      buf[4] = 0x04; //

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_DDOL, 5, buf );

      // --- Terminal Country Code (n3) ---
      buf[0] = 0x00;  // BELGIUM
      buf[1] = 0x56;  //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_CNTR_CODE, 2, buf );

      // --- Transaction Currency Code (n3) ---
      buf[0] = 0x09; // iso 4217, BEF=0978 (EURO)
      buf[1] = 0x78; //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_CC, 2, buf );

      // --- Transaction Currency Exponent (n1, imply decimal point from the right of amt) ---
      // the formal rule is to access the exponent according to Currency Code (iso 4217)
      buf[0] = 0x02; // iso 4217, BEF=0056 (Belgium Franc)
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_CE, 1, buf );
      g_term_tx_exp = 2;
      g_term_decimal_point = 2;

      // --- Terminal Floor Limit ---
      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x01; // integer part=100
      buf[4] = 0x00; //
      buf[5] = 0x00; // decimal part=.00

      buf[6] = 0x01; // TFL flag is set (TFL present)

      // update all TFLs to the same value (from AID_01 to AID_16)
      for( i=0; i<MAX_TFL_CNT; i++ )
         {
         api_emv_PutDataElement( DE_TERM, ADDR_TFL_01+TFL_LEN*i, TFL_LEN, buf ); // value
         api_emv_PutDataElement( DE_TERM, ADDR_TFL_FLAG_01+i, 1, &buf[6] ); // flag
         }

//    buf[0] = 0x00;
//    buf[1] = 0x00;
//    buf[2] = 0x00;
//    buf[3] = 0x64;
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_FL, 4, buf );

      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x01; // integer part=100
      buf[4] = 0x00; //
      buf[5] = 0x00; // decimal part=.00
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_01, TFL_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_02, TFL_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_03, TFL_LEN, buf );

      // --- Target Percentage for Random Selection ---
      buf[0] = 0;
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01+RS_TP_LEN*1, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01+RS_TP_LEN*2, 1, buf );

      // --- Max. Target Percentage for Biased Random Selection ---
      buf[0] = 0;
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01+BRS_MTP_LEN*1, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01+BRS_MTP_LEN*2, 1, buf );

      // --- Threshold Value for Biased Random Selection ---
      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x00;
      buf[4] = 0x05; // integer=5
      buf[5] = 0x00; // decimal=.00
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD, BRS_THRESHOLD_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01, BRS_THRESHOLD_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01+BRS_THRESHOLD_LEN*1, BRS_THRESHOLD_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01+BRS_THRESHOLD_LEN*2, BRS_THRESHOLD_LEN, buf );

      g_dhn_pinpad = 0; // pinpad works
}

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 20. (EMV L2)
//
// List of AID's in the following sequence order:
// 1 Non EMV   : B0 12 34 56 78 12 34
//               (Application Selection Indicator indicates that multiples
//               occurences of the Application within the card are not allowed)
// 2 JCB       : A0 00 00 00 65 10 10
//               (Application Selection Indicator indicates that multiples
//               occurences of the Application within the card are not allowed)
// 3 Partial   : A0 00 00 00 10
//               (Application Selection Indicator indicates that multiples
//               occurences of the Application within the card are allowed)
//
// Terminal Application Version Number: 02 00
//
// All TAC's set to 0
//
// Default TDOL if used: ICC Application Version Number ( 9F 08)
//
// Default TDOL: not present
//
// Default DDOL: 9F 37 04 9F 1C 08 9F 1A 02 9A 03
//
// Terminal Country Code: 00 56
//
// Transaction Currency Code: 09 78 (EURO)
//
// Transaction Currency Exponent: 02
//
// ---------------------------------------------------------------------------
void SETUP_config_20( void )
{
UINT  i;
UCHAR buf[64];

//    TL_DispHexByte(0,18,0x20);
//    UI_WaitKey();

      // ------ list of AID's ------
      // 1. B0 12 34 56 78 12 34
      buf[0] = 0x08;
      buf[1] = 0x00; // partial matching not allowed
      buf[2] = 0xB0;
      buf[3] = 0x12;
      buf[4] = 0x34;
      buf[5] = 0x56;
      buf[6] = 0x78;
      buf[7] = 0x12;
      buf[8] = 0x34;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_01, 0x09, buf );

      // 2. A0 00 00 00 65 10 10
      buf[0] = 0x08;
      buf[1] = 0x00; // partial matching not allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x65;
      buf[7] = 0x10;
      buf[8] = 0x10;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_02, 0x09, buf );

      // 3. A0 00 00 00 10
      buf[0] = 0x06;
      buf[1] = 0x01; // partial matching allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x10;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_03, 0x07, buf );

      // end of AID list
      buf[0] = 0x00;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_04, 0x01, buf );

      // --- Application Version Number (b2) ---
      buf[0] = 0x02;
      buf[1] = 0x00;
//    api_emv_PutDataElement( DE_TERM, ADDR_AVN_01, AVN_LEN, buf );
//    api_emv_PutDataElement( DE_TERM, ADDR_AVN_02, AVN_LEN, buf );
//    api_emv_PutDataElement( DE_TERM, ADDR_AVN_03, AVN_LEN, buf );
      for( i=0; i<MAX_AVN_CNT; i++ )
         api_emv_PutDataElement( DE_TERM, ADDR_AVN_START+(i*AVN_LEN), AVN_LEN, buf );

      // --- TAC's ---
      memset( buf, 0x00, 5 );

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online

      // --- Default TDOL --- (not present)
      buf[0] = 0;
      buf[1] = 0;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TDOL, 2, buf );

      // --- Default DDOL ---
      buf[0] = 0x0B; // DDOL length
      buf[1] = 0x00; //

      buf[2]  = 0x9F; // the unpredictable number
      buf[3]  = 0x37; //
      buf[4]  = 0x04; //
      buf[5]  = 0x9F; // terminal id
      buf[6]  = 0x1C; //
      buf[7]  = 0x08; //
      buf[8]  = 0x9F; // terminal country coude
      buf[9]  = 0x1A; //
      buf[10] = 0x02; //
      buf[11] = 0x9A; // transaction date
      buf[12] = 0x03; //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_DDOL, 13, buf );

      // --- Terminal Country Code (n3) ---
      buf[0] = 0x00;  // BELGIUM
      buf[1] = 0x56;  //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_CNTR_CODE, 2, buf );

      // --- Transaction Currency Code (n3) ---
      buf[0] = 0x09; // iso 4217, BEF=0978 (EURO)
      buf[1] = 0x78; //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_CC, 2, buf );

      // --- Transaction Currency Exponent (n1, imply decimal point from the right of amt) ---
      // the formal rule is to access the exponent according to Currency Code (iso 4217)
      buf[0] = 0x02; // iso 4217, BEF=0056 (Belgium Franc)
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_CE, 1, buf );
      g_term_tx_exp = 2;
      g_term_decimal_point = 2;

      // --- Terminal Floor Limit ---
      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x01; // integer part=100
      buf[4] = 0x00; //
      buf[5] = 0x00; // decimal part=.00

      buf[6] = 0x01; // TFL flag is set (TFL present)

      // update all TFLs to the same value (from AID_01 to AID_16)
      for( i=0; i<MAX_TFL_CNT; i++ )
         {
         api_emv_PutDataElement( DE_TERM, ADDR_TFL_01+TFL_LEN*i, TFL_LEN, buf ); // value
         api_emv_PutDataElement( DE_TERM, ADDR_TFL_FLAG_01+i, 1, &buf[6] ); // flag
         }

//    buf[0] = 0x00;
//    buf[1] = 0x00;
//    buf[2] = 0x00;
//    buf[3] = 0x64;
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_FL, 4, buf );

      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x01; // integer part=100
      buf[4] = 0x00; //
      buf[5] = 0x00; // decimal part=.00
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_01, TFL_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_02, TFL_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_03, TFL_LEN, buf );

      // --- Target Percentage for Random Selection ---
      buf[0] = 0;
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01+RS_TP_LEN*1, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01+RS_TP_LEN*2, 1, buf );

      // --- Max. Target Percentage for Biased Random Selection ---
      buf[0] = 0;
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01+BRS_MTP_LEN*1, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01+BRS_MTP_LEN*2, 1, buf );

      // --- Threshold Value for Biased Random Selection ---
      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x00;
      buf[4] = 0x05; // integer=5
      buf[5] = 0x00; // decimal=.00
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD, BRS_THRESHOLD_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01, BRS_THRESHOLD_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01+BRS_THRESHOLD_LEN*1, BRS_THRESHOLD_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01+BRS_THRESHOLD_LEN*2, BRS_THRESHOLD_LEN, buf );

      g_dhn_pinpad = 0; // pinpad works
}

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 21. (EMV L2)
//
// List of AID's in the following sequence order:
// 1 Non EMV   : B0 12 34 56 78 12 34
//               (Application Selection Indicator indicates that multiples
//               occurences of the Application within the card are not allowed)
// 2 VISA      : A0 00 00 00 03 10 10
//               (Application Selection Indicator indicates that multiples
//               occurences of the Application within the card are not allowed)
// 3 Partial   : A0 00 00 00 10
//               (Application Selection Indicator indicates that multiples
//               occurences of the Application within the card are allowed)
//
// Terminal Application Version Number: 00 8C
//
// All TAC's set to 0
//
// Default TDOL if used: ICC Application Version Number ( 9F 08)
//
// Default TDOL: not present
//
// Default DDOL: 9F 37 04
//
// Terminal Country Code: 00 56
//
// Transaction Currency Code: 09 78 (EURO)
//
// Transaction Currency Exponent: 02
//
// ---------------------------------------------------------------------------
void SETUP_config_21( void )
{
UINT  i;
UCHAR buf[64];

//    TL_DispHexByte(0,18,0x21);
//    UI_WaitKey();

      // ------ list of AID's ------
      // 1. B0 12 34 56 78 12 34
      buf[0] = 0x08;
      buf[1] = 0x00; // partial matching not allowed
      buf[2] = 0xB0;
      buf[3] = 0x12;
      buf[4] = 0x34;
      buf[5] = 0x56;
      buf[6] = 0x78;
      buf[7] = 0x12;
      buf[8] = 0x34;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_01, 0x09, buf );

      // 2. A0 00 00 00 03 10 10
      buf[0] = 0x08;
      buf[1] = 0x00; // partial matching not allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x03;
      buf[7] = 0x10;
      buf[8] = 0x10;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_02, 0x09, buf );

      // 3. A0 00 00 00 10
      buf[0] = 0x06;
      buf[1] = 0x01; // partial matching allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x10;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_03, 0x07, buf );

      // end of AID list
      buf[0] = 0x00;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_04, 0x01, buf );

      // --- Application Version Number (b2) ---
      buf[0] = 0x00;
      buf[1] = 0x8C;
//    api_emv_PutDataElement( DE_TERM, ADDR_AVN_01, AVN_LEN, buf );
//    api_emv_PutDataElement( DE_TERM, ADDR_AVN_02, AVN_LEN, buf );
//    api_emv_PutDataElement( DE_TERM, ADDR_AVN_03, AVN_LEN, buf );
      for( i=0; i<MAX_AVN_CNT; i++ )
         api_emv_PutDataElement( DE_TERM, ADDR_AVN_START+(i*AVN_LEN), AVN_LEN, buf );

      // --- TAC's ---
      memset( buf, 0x00, 5 );

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online

      // --- Default TDOL --- (not present)
      buf[0] = 0;
      buf[1] = 0;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TDOL, 2, buf );

      // --- Default DDOL ---
      buf[0] = 0x03; // DDOL length
      buf[1] = 0x00; //

      buf[2]  = 0x9F; // the unpredictable number
      buf[3]  = 0x37; //
      buf[4]  = 0x04; //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_DDOL, 5, buf );

      // --- Terminal Country Code (n3) ---
      buf[0] = 0x00;  // BELGIUM
      buf[1] = 0x56;  //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_CNTR_CODE, 2, buf );

      // --- Transaction Currency Code (n3) ---
      buf[0] = 0x09; // iso 4217, BEF=0978 (EURO)
      buf[1] = 0x78; //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_CC, 2, buf );

      // --- Transaction Currency Exponent (n1, imply decimal point from the right of amt) ---
      // the formal rule is to access the exponent according to Currency Code (iso 4217)
      buf[0] = 0x02; // iso 4217, BEF=0056 (Belgium Franc)
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_CE, 1, buf );
      g_term_tx_exp = 2;
      g_term_decimal_point = 2;

      // --- Terminal Floor Limit ---
      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x01; // integer part=100
      buf[4] = 0x00; //
      buf[5] = 0x00; // decimal part=.00

      buf[6] = 0x01; // TFL flag is set (TFL present)

      // update all TFLs to the same value (from AID_01 to AID_16)
      for( i=0; i<MAX_TFL_CNT; i++ )
         {
         api_emv_PutDataElement( DE_TERM, ADDR_TFL_01+TFL_LEN*i, TFL_LEN, buf ); // value
         api_emv_PutDataElement( DE_TERM, ADDR_TFL_FLAG_01+i, 1, &buf[6] ); // flag
         }

//    buf[0] = 0x00;
//    buf[1] = 0x00;
//    buf[2] = 0x00;
//    buf[3] = 0x64;
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_FL, 4, buf );

      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x01; // integer part=100
      buf[4] = 0x00; //
      buf[5] = 0x00; // decimal part=.00
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_01, TFL_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_02, TFL_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_03, TFL_LEN, buf );

      // --- Target Percentage for Random Selection ---
      buf[0] = 0;
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01+RS_TP_LEN*1, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01+RS_TP_LEN*2, 1, buf );

      // --- Max. Target Percentage for Biased Random Selection ---
      buf[0] = 0;
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01+BRS_MTP_LEN*1, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01+BRS_MTP_LEN*2, 1, buf );

      // --- Threshold Value for Biased Random Selection ---
      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x00;
      buf[4] = 0x05; // integer=5
      buf[5] = 0x00; // decimal=.00
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD, BRS_THRESHOLD_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01, BRS_THRESHOLD_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01+BRS_THRESHOLD_LEN*1, BRS_THRESHOLD_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01+BRS_THRESHOLD_LEN*2, BRS_THRESHOLD_LEN, buf );

      g_dhn_pinpad = 0; // pinpad works
}

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 22. (EMV L2)
//
// List of AID's in the following sequence order:
// 1 A0 00 00 00 04 10 10
//               (Application Selection Indicator indicates that multiples
//               occurences of the Application within the card are not allowed)
// 2 A0 00 00 00 10
//               (Application Selection Indicator indicates that multiples
//               occurences of the Application within the card are allowed)
//
// Terminal Application Version Number: 00 02
//
// All TAC's set to 0
//
// Default TDOL if used: ICC Application Version Number ( 9F 08)
//
// Default DDOL: 9F 37 04 9F 47 01 8F 01 9F 32 01
//
// Terminal Country Code: 00 56
//
// Transaction Currency Code: 09 78 (EURO)
//
// Transaction Currency Exponent: 02
//
// ---------------------------------------------------------------------------
void SETUP_config_22( void )
{
UINT  i;
UCHAR buf[64];

//    TL_DispHexByte(0,18,0x22);
//    UI_WaitKey();

      // ------ list of AID's ------
      // 1. B0 12 34 56 78 12 34
      buf[0] = 0x08;
      buf[1] = 0x00; // partial matching not allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x04;
      buf[7] = 0x10;
      buf[8] = 0x10;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_01, 0x09, buf );

      // 2. A0 00 00 00 10
      buf[0] = 0x06;
      buf[1] = 0x01; // partial matching allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x10;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_02, 0x07, buf );

      // end of AID list
      buf[0] = 0x00;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_03, 0x01, buf );

      // --- Application Version Number (b2) ---
      buf[0] = 0x00;
      buf[1] = 0x02;
//    api_emv_PutDataElement( DE_TERM, ADDR_AVN_01, AVN_LEN, buf );
//    api_emv_PutDataElement( DE_TERM, ADDR_AVN_02, AVN_LEN, buf );
//    api_emv_PutDataElement( DE_TERM, ADDR_AVN_03, AVN_LEN, buf );
      for( i=0; i<MAX_AVN_CNT; i++ )
         api_emv_PutDataElement( DE_TERM, ADDR_AVN_START+(i*AVN_LEN), AVN_LEN, buf );

      // --- TAC's ---
      memset( buf, 0x00, 5 );

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online

      // --- Default TDOL --- (9F 08: AVN)
      buf[0] = 3;
      buf[1] = 0;
      buf[2] = 0x9F;
      buf[3] = 0x08;
      buf[4] = 0x02;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TDOL, 5, buf );

      // --- Default DDOL ---
      buf[0] = 0x0B; // DDOL length
      buf[1] = 0x00; //

      buf[2] = 0x9F; // the unpredictable number
      buf[3] = 0x37; //
      buf[4] = 0x04; //

      buf[5] = 0x9F; // icc public key exponent
      buf[6] = 0x47; //
      buf[7] = 0x01; //

      buf[8] = 0x8F; // CA public key index
      buf[9] = 0x01; //

      buf[10] = 0x9F; // issuer public key exponent
      buf[11] = 0x32; //
      buf[12] = 0x01; //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_DDOL, 13, buf );

      // --- Terminal Country Code (n3) ---
      buf[0] = 0x00;  // BELGIUM
      buf[1] = 0x56;  //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_CNTR_CODE, 2, buf );

      // --- Transaction Currency Code (n3) ---
      buf[0] = 0x09; // iso 4217, BEF=0978 (EURO)
      buf[1] = 0x78; //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_CC, 2, buf );

      // --- Transaction Currency Exponent (n1, imply decimal point from the right of amt) ---
      // the formal rule is to access the exponent according to Currency Code (iso 4217)
      buf[0] = 0x02; // iso 4217, BEF=0056 (Belgium Franc)
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_CE, 1, buf );
      g_term_tx_exp = 2;
      g_term_decimal_point = 2;

      // --- Terminal Floor Limit ---
      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x01; // integer part=100
      buf[4] = 0x00; //
      buf[5] = 0x00; // decimal part=.00

      buf[6] = 0x01; // TFL flag is set (TFL present)

      // update all TFLs to the same value (from AID_01 to AID_16)
      for( i=0; i<MAX_TFL_CNT; i++ )
         {
         api_emv_PutDataElement( DE_TERM, ADDR_TFL_01+TFL_LEN*i, TFL_LEN, buf ); // value
         api_emv_PutDataElement( DE_TERM, ADDR_TFL_FLAG_01+i, 1, &buf[6] ); // flag
         }

//    buf[0] = 0x00;
//    buf[1] = 0x00;
//    buf[2] = 0x00;
//    buf[3] = 0x64;
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_FL, 4, buf );

      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x01; // integer part=100
      buf[4] = 0x00; //
      buf[5] = 0x00; // decimal part=.00
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_01, TFL_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_02, TFL_LEN, buf );

      // --- Target Percentage for Random Selection ---
      buf[0] = 0;
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01+RS_TP_LEN*1, 1, buf );

      // --- Max. Target Percentage for Biased Random Selection ---
      buf[0] = 0;
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01+BRS_MTP_LEN*1, 1, buf );

      // --- Threshold Value for Biased Random Selection ---
      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x00;
      buf[4] = 0x05; // integer=5
      buf[5] = 0x00; // decimal=.00
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD, BRS_THRESHOLD_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01, BRS_THRESHOLD_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01+BRS_THRESHOLD_LEN*1, BRS_THRESHOLD_LEN, buf );

      g_dhn_pinpad = 0; // pinpad works
}

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 23. (EMV L2)
//
// List of AID's in the following sequence order:
// 1. B0 12 34 56 78 12 34
// 2. A0 00 00 00 04 10 10
// 3. A0 00 00 00 04 10 12
// 4. A0 00 00 00 04 10 14
// 5. A0 00 00 00 10
//
// Terminal Application Version Number: 00 02
//
// All TAC's set to 0
//
// Default TDOL if used: ICC Application Version Number ( 9F 08)
//
// Default DDOL: 9F 37 04 9F 47 01 8F 01 9F 32 01
//
// Terminal Country Code: 00 56
//
// Transaction Currency Code: 09 78 (EURO)
//
// Transaction Currency Exponent: 02
//
// Terminal Floor Limit: 100 (=100.0 or 100.00 or 100.000 according to the number of decimal values)
//
// If Random Transaction Selection process supported:
//    Target and Maximum Target percentage for random selection set to 0 to avoid random behaviour.
//
// ---------------------------------------------------------------------------
void SETUP_config_23( void )
{
UINT  i;
UCHAR buf[64];

//    TL_DispHexByte(0,18,0x23);
//    UI_WaitKey();

      // ------ list of AID's ------
      // 1. B0 12 34 56 78 12 34
      buf[0] = 0x08;
      buf[1] = 0x00; // partial matching not allowed
      buf[2] = 0xB0;
      buf[3] = 0x12;
      buf[4] = 0x34;
      buf[5] = 0x56;
      buf[6] = 0x78;
      buf[7] = 0x12;
      buf[8] = 0x34;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_01, 0x09, buf );

      // 2. A0 00 00 00 04 10 10
      buf[0] = 0x08;
      buf[1] = 0x00; // partial matching not allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x04;
      buf[7] = 0x10;
      buf[8] = 0x10;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_02, 0x09, buf );

      // 3. A0 00 00 00 04 10 12
      buf[0] = 0x08;
      buf[1] = 0x00; // partial matching not allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x04;
      buf[7] = 0x10;
      buf[8] = 0x12;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_03, 0x09, buf );

      // 4. A0 00 00 00 04 10 14
      buf[0] = 0x08;
      buf[1] = 0x00; // partial matching not allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x04;
      buf[7] = 0x10;
      buf[8] = 0x14;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_04, 0x09, buf );

      // 5. A0 00 00 00 10
      buf[0] = 0x06;
      buf[1] = 0x01; // partial matching allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x10;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_05, 0x07, buf );

      // end of AID list
      buf[0] = 0x00;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_06, 0x01, buf );

      // --- Application Version Number (b2) ---
      buf[0] = 0x00;
      buf[1] = 0x02;
//    api_emv_PutDataElement( DE_TERM, ADDR_AVN_01, AVN_LEN, buf );
//    api_emv_PutDataElement( DE_TERM, ADDR_AVN_02, AVN_LEN, buf );
//    api_emv_PutDataElement( DE_TERM, ADDR_AVN_03, AVN_LEN, buf );
      for( i=0; i<MAX_AVN_CNT; i++ )
         api_emv_PutDataElement( DE_TERM, ADDR_AVN_START+(i*AVN_LEN), AVN_LEN, buf );

      // --- TAC's ---
      memset( buf, 0x00, 5 );

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_04, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_04, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_04, 5, buf );  // TAC-Online

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_05, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_05, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_05, 5, buf );  // TAC-Online

      // --- Default TDOL --- (9F 08: AVN)
      buf[0] = 3;
      buf[1] = 0;
      buf[2] = 0x9F;
      buf[3] = 0x08;
      buf[4] = 0x02;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TDOL, 5, buf );

      // --- Default DDOL ---
      buf[0] = 0x0B; // DDOL length
      buf[1] = 0x00; //

      buf[2] = 0x9F; // the unpredictable number
      buf[3] = 0x37; //
      buf[4] = 0x04; //

      buf[5] = 0x9F; // icc public key exponent
      buf[6] = 0x47; //
      buf[7] = 0x01; //

      buf[8] = 0x8F; // CA public key index
      buf[9] = 0x01; //

      buf[10] = 0x9F; // issuer public key exponent
      buf[11] = 0x32; //
      buf[12] = 0x01; //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_DDOL, 13, buf );

      // --- Terminal Country Code (n3) ---
      buf[0] = 0x00;  // BELGIUM
      buf[1] = 0x56;  //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_CNTR_CODE, 2, buf );

      // --- Transaction Currency Code (n3) ---
      buf[0] = 0x09; // iso 4217, BEF=0978 (EURO)
      buf[1] = 0x78; //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_CC, 2, buf );

      // --- Transaction Currency Exponent (n1, imply decimal point from the right of amt) ---
      // the formal rule is to access the exponent according to Currency Code (iso 4217)
      buf[0] = 0x02; // iso 4217, BEF=0056 (Belgium Franc)
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_CE, 1, buf );
      g_term_tx_exp = 2;
      g_term_decimal_point = 2;

      // --- Terminal Floor Limit ---
      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x01; // integer part=100
      buf[4] = 0x00; //
      buf[5] = 0x00; // decimal part=.00

      buf[6] = 0x01; // TFL flag is set (TFL present)

      // update all TFLs to the same value (from AID_01 to AID_16)
      for( i=0; i<MAX_TFL_CNT; i++ )
         {
         api_emv_PutDataElement( DE_TERM, ADDR_TFL_01+TFL_LEN*i, TFL_LEN, buf ); // value
         api_emv_PutDataElement( DE_TERM, ADDR_TFL_FLAG_01+i, 1, &buf[6] ); // flag
         }

//    buf[0] = 0x00;
//    buf[1] = 0x00;
//    buf[2] = 0x00;
//    buf[3] = 0x64;
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_FL, 4, buf );

      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x01; // integer part=100
      buf[4] = 0x00; //
      buf[5] = 0x00; // decimal part=.00
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_01, TFL_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_02, TFL_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_03, TFL_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_04, TFL_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_05, TFL_LEN, buf );

      // --- Target Percentage for Random Selection ---
      buf[0] = 0;
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01+RS_TP_LEN*1, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01+RS_TP_LEN*2, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01+RS_TP_LEN*3, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01+RS_TP_LEN*4, 1, buf );

      // --- Max. Target Percentage for Biased Random Selection ---
      buf[0] = 0;
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01+BRS_MTP_LEN*1, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01+BRS_MTP_LEN*2, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01+BRS_MTP_LEN*3, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01+BRS_MTP_LEN*4, 1, buf );

      // --- Threshold Value for Biased Random Selection ---
      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x00;
      buf[4] = 0x05; // integer=5
      buf[5] = 0x00; // decimal=.00
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD, BRS_THRESHOLD_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01, BRS_THRESHOLD_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01+BRS_THRESHOLD_LEN*1, BRS_THRESHOLD_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01+BRS_THRESHOLD_LEN*2, BRS_THRESHOLD_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01+BRS_THRESHOLD_LEN*3, BRS_THRESHOLD_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01+BRS_THRESHOLD_LEN*4, BRS_THRESHOLD_LEN, buf );

      g_dhn_pinpad = 0; // pinpad works
}

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 24. (EMV L2)
//
// List of AID's in the following sequence order:
// A0 00 00 00 09 08 07 06 05 04
//
// Terminal Application Version Number: 00 02
//
// All TAC's set to 0
//
// Default TDOL if used: ICC Application Version Number ( 9F 08)
//
// Default DDOL: 9F 37 04 9F 47 01 8F 01 9F 32 01
//
// Terminal Country Code: 00 56
//
// Transaction Currency Code: 09 78 (EURO)
//
// Transaction Currency Exponent: 02
//
// Terminal Floor Limit: 100 (=100.0 or 100.00 or 100.000 according to the number of decimal values)
//
// If Random Transaction Selection process supported:
//    Target and Maximum Target percentage for random selection set to 0 to avoid random behaviour.
//
// If Random Transaction Selection process supported:
//    Threshold value for biased random selection shall be set to 5 (or 5.0 or 5.00 or 5.000 according to the number of decimal values)
// ---------------------------------------------------------------------------
void SETUP_config_24( void )
{
UINT  i;
UCHAR buf[64];

//    TL_DispHexByte(0,18,0x24);
//    UI_WaitKey();

      // ------ list of AID's ------
      // 1. A0 00 00 00 09 08 07 06 05 04
      buf[0] = 0x0B;
      buf[1] = 0x00; // partial matching not allowed
      buf[2] = 0xA0;
      buf[3] = 0x00;
      buf[4] = 0x00;
      buf[5] = 0x00;
      buf[6] = 0x09;
      buf[7] = 0x08;
      buf[8] = 0x07;
      buf[9] = 0x06;
      buf[10] = 0x05;
      buf[11] = 0x04;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_01, 0x0C, buf );

      // end of AID list
      buf[0] = 0x00;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_02, 0x01, buf );

      // --- Application Version Number (b2) ---
      buf[0] = 0x00;
      buf[1] = 0x02;
//    api_emv_PutDataElement( DE_TERM, ADDR_AVN_01, AVN_LEN, buf );
//    api_emv_PutDataElement( DE_TERM, ADDR_AVN_02, AVN_LEN, buf );
//    api_emv_PutDataElement( DE_TERM, ADDR_AVN_03, AVN_LEN, buf );
      for( i=0; i<MAX_AVN_CNT; i++ )
         api_emv_PutDataElement( DE_TERM, ADDR_AVN_START+(i*AVN_LEN), AVN_LEN, buf );

      // --- TAC's ---
      memset( buf, 0x00, 5 );

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online

      // --- Default TDOL --- (9F 08: AVN)
      buf[0] = 3;
      buf[1] = 0;
      buf[2] = 0x9F;
      buf[3] = 0x08;
      buf[4] = 0x02;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TDOL, 5, buf );

      // --- Default DDOL ---
      buf[0] = 0x0B; // DDOL length
      buf[1] = 0x00; //

      buf[2] = 0x9F; // the unpredictable number
      buf[3] = 0x37; //
      buf[4] = 0x04; //

      buf[5] = 0x9F; // icc public key exponent
      buf[6] = 0x47; //
      buf[7] = 0x01; //

      buf[8] = 0x8F; // CA public key index
      buf[9] = 0x01; //

      buf[10] = 0x9F; // issuer public key exponent
      buf[11] = 0x32; //
      buf[12] = 0x01; //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_DDOL, 13, buf );

      // --- Terminal Country Code (n3) ---
      buf[0] = 0x00;  // BELGIUM
      buf[1] = 0x56;  //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_CNTR_CODE, 2, buf );

      // --- Transaction Currency Code (n3) ---
      buf[0] = 0x09; // iso 4217, BEF=0978 (EURO)
      buf[1] = 0x78; //
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_CC, 2, buf );

      // --- Transaction Currency Exponent (n1, imply decimal point from the right of amt) ---
      // the formal rule is to access the exponent according to Currency Code (iso 4217)
      buf[0] = 0x02; // iso 4217, BEF=0056 (Belgium Franc)
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_CE, 1, buf );
      g_term_tx_exp = 2;
      g_term_decimal_point = 2;

      // --- Terminal Floor Limit ---
      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x01; // integer part=100
      buf[4] = 0x00; //
      buf[5] = 0x00; // decimal part=.00

      buf[6] = 0x01; // TFL flag is set (TFL present)

      // update all TFLs to the same value (from AID_01 to AID_16)
      for( i=0; i<MAX_TFL_CNT; i++ )
         {
         api_emv_PutDataElement( DE_TERM, ADDR_TFL_01+TFL_LEN*i, TFL_LEN, buf ); // value
         api_emv_PutDataElement( DE_TERM, ADDR_TFL_FLAG_01+i, 1, &buf[6] ); // flag
         }

//    buf[0] = 0x00;
//    buf[1] = 0x00;
//    buf[2] = 0x00;
//    buf[3] = 0x64;
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_FL, 4, buf );

      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x01; // integer part=100
      buf[4] = 0x00; //
      buf[5] = 0x00; // decimal part=.00
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_01, TFL_LEN, buf );

      // --- Target Percentage for Random Selection ---
      buf[0] = 0;
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01, 1, buf );

      // --- Max. Target Percentage for Biased Random Selection ---
      buf[0] = 0;
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP, 1, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01, 1, buf );

      // --- Threshold Value for Biased Random Selection ---
      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x00;
      buf[4] = 0x05; // integer=5
      buf[5] = 0x00; // decimal=.00
//    api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD, BRS_THRESHOLD_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01, BRS_THRESHOLD_LEN, buf );

      g_dhn_pinpad = 0; // pinpad works
}

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 25. (EMV L2)
//
// TAC denial  = 00 00 00 00 00
// TAC online  = 00 00 00 00 00
// TAC default = FF FF FF FF FF
//
// (unless otherwise specified, other parameters are the same as in terminal configuration 1)
// ---------------------------------------------------------------------------
void SETUP_config_25( void )
{
UINT  i;
UCHAR buf[64];

//    TL_DispHexByte(0,18,0x25);
//    UI_WaitKey();

      SETUP_config_01();

      // --- TAC's ---
      memset( buf, 0x00, 5 );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online

      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online

      buf[0] = 0xFF;
      buf[1] = 0xFF;
      buf[2] = 0xFF;
      buf[3] = 0xFF;
      buf[4] = 0xFF;
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf );  // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf );  // TAC-Default
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf );  // TAC-Default
}

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 26. (EMV L2)
//
// Terminal floor limit = 0
// (unless otherwise specified, other parameters are the same as in terminal configuration 1)
// ---------------------------------------------------------------------------
void SETUP_config_26( void )
{
UINT  i;
UCHAR buf[64];

//    TL_DispHexByte(0,18,0x26);
//    UI_WaitKey();

      SETUP_config_01();

      // --- Terminal Floor Limit ---
      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x00; // integer part=0
      buf[4] = 0x00; //
      buf[5] = 0x00; // decimal part=.00

      buf[6] = 0x01; // TFL flag is set (TFL present)

      // update all TFLs to the same value (from AID_01 to AID_16)
      for( i=0; i<MAX_TFL_CNT; i++ )
         {
         api_emv_PutDataElement( DE_TERM, ADDR_TFL_01+TFL_LEN*i, TFL_LEN, buf ); // value
         api_emv_PutDataElement( DE_TERM, ADDR_TFL_FLAG_01+i, 1, &buf[6] ); // flag
         }

      buf[0] = 0x00;
      buf[1] = 0x00;
      buf[2] = 0x00;
      buf[3] = 0x00; // integer part=0
      buf[4] = 0x00; //
      buf[5] = 0x00; // decimal part=.00
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_01, TFL_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_02, TFL_LEN, buf );
      api_emv_PutDataElement( DE_TERM, ADDR_TFL_03, TFL_LEN, buf );
}

// ---------------------------------------------------------------------------
