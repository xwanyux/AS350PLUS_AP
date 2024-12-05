//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : EMV L2                                                     **
//**  PRODUCT  : AS330	                                                    **
//**                                                                        **
//**  FILE     : EMVDCFUN.C                                                 **
//**  MODULE   : Specific functions invoked by pressing FUNC key.           **
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

#include "APPEMVconfig.h"

#include "POSAPI.h"
#include "EMVDC.h"
#include "GDATAEX.h"
#include "EMVAPI.h"
#include "EMVDCMSG.h"
#include "UI.h"
#include "TOOLS.h"

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 99. (internal test only)
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
// ---------------------------------------------------------------------------
//void SETUP_config_99( void )
//{
//UINT  i;
//UCHAR buf[64];
//
//      // --- Terminal Type ---
//      buf[0] = 0x22; // POS terminal
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TYPE, 1, buf );
//
//      // --- Terminal Capabilities ---
//      buf[0] = 0xE0;
//      buf[1] = 0xB8;
//      buf[2] = 0xC0;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_CAP, 3, buf );
//
//      // --- Additional Terminal Capabilities ---
//      buf[0] = 0x40;
//      buf[1] = 0x00;
//      buf[2] = 0xF0;
//      buf[3] = 0xB0;
//      buf[4] = 0x01;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_ADD_CAP, 5, buf );
//
//      // IFD serial number
//      buf[0] = 'I';
//      buf[1] = 'F';
//      buf[2] = 'D';
//      buf[3] = '0';
//      buf[4] = '0';
//      buf[5] = '0';
//      buf[6] = '0';
//      buf[7] = '1';
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_IFD_SN, 8, buf );
//
//      // ----------------------------
//      // Application Independent Data
//      // ----------------------------
//
//      // --- Terminal Country Code (n3) ---
//      buf[0] = 0x01;
//      buf[1] = 0x58;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_CNTR_CODE, 2, buf );
//
//      // -----------------------------
//      // Application Dependent Data
//      // (specified by payment system)
//      // -----------------------------
//
//      // --- Application Identifier (AID) ---
//
//      // clear all TERM_AID
//      memset( buf, 0x00, TERM_AID_LEN );
//      for(i=0; i<MAX_AID_CNT; i++)
//         api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_01+i*TERM_AID_LEN, TERM_AID_LEN, buf );
//
//      // init AID - "VISACREDIT"
//      buf[0] = 0x08;
//      buf[1] = 0x01; // partial matching allowed
//      buf[2] = 0xa0;
//      buf[3] = 0x00;
//      buf[4] = 0x00;
//      buf[5] = 0x00;
//      buf[6] = 0x03;
//      buf[7] = 0x10;
//      buf[8] = 0x10;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_01, 0x09, buf );
//
//      // init AID - "ELECTRON"
//      buf[0] = 0x08;
//      buf[1] = 0x01; // partial matching allowed
//      buf[2] = 0xa0;
//      buf[3] = 0x00;
//      buf[4] = 0x00;
//      buf[5] = 0x00;
//      buf[6] = 0x03;
//      buf[7] = 0x20;
//      buf[8] = 0x10;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_02, 0x09, buf );
//
//      // init AID - "INTERLINK"
//      buf[0] = 0x08;
//      buf[1] = 0x01; // partial matching allowed
//      buf[2] = 0xa0;
//      buf[3] = 0x00;
//      buf[4] = 0x00;
//      buf[5] = 0x00;
//      buf[6] = 0x03;
//      buf[7] = 0x30;
//      buf[8] = 0x10;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_03, 0x09, buf );
//
//      // init LEN = 0x00 - end of AID
//      buf[0] = 0x00;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_04, 0x01, buf );
//
//      // --- Application Version Number (b2) ---
//      buf[0] = 0x00;                                     // the 1'st AVN
//      buf[1] = 0x84;                                     //
//      api_emv_PutDataElement( DE_TERM, ADDR_AVN_01, AVN_LEN, buf );
//
//      // --- CA Public Key Modulus ---
//
//      // --- CA Public Key Exponent ---
//
//      // --- Default DDOL ---
//      buf[0] = 0x03; // DDOL length
//      buf[1] = 0x00; //
//
//      buf[2] = 0x9F; // the unpredictable number
//      buf[3] = 0x37; //
//      buf[4] = 0x04; //
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_DDOL, 5, buf );
//
//      // --- Default TDOL ---
//
//      // --- Max. Target Percentage for Biased Random Selection ---
//
//      // --- Target Percentage for Random Selection ---
//
//      // --- Threshold Value for Biased Random Selection ---
//
//      // --- Terminal Action Code (Default, Denial, Online) ---
//
//      // --- Terminal Floor Limit ---
//
//      // --- Terminal Identification (an8) ---
//      buf[0] = '3';
//      buf[1] = '1';
//      buf[2] = '0';
//      buf[3] = '0';
//      buf[4] = '0';
//      buf[5] = '0';
//      buf[6] = '0';
//      buf[7] = '0';
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TID, 8, buf );
//
//      // --- Terminal Risk Management Data ---
//
//      // --- Merchant Category Code ---
//
//      // --- Merchant Identifier ---
//
//      // --- Merchant Name and Location ---
//
//      // --- Transaction Currency Code (n3) ---
//      buf[0] = 0x09; // iso 4217, TWD=901
//      buf[1] = 0x01; //
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_CC, 2, buf );
//
//      // --- Transaction Currency Exponent (n1, decimal point from the right of amt) ---
//      buf[0] = 0x02; // iso 4217, 2 decimal points
////    buf[0] = 0x00; // iso 4217, no decimal point
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_CE, 1, buf );
//      g_term_tx_exp = 2;
//      g_term_decimal_point = 0;
//
//      // --- Transaction Reference Currency Code ---
//
//      // --- Transaction Reference Currency Conversion ---
//
//      // --- Transaction Reference Currency Exponent ---
//
//      // --- Transaction Type (iso 8583:Processing Code) ---
//      g_term_tx_type = 0x00; // goods & services
//}

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration 0. (terminal default settings)
//
// ---------------------------------------------------------------------------
//void SETUP_config_00( void )
//{
//UINT  i;
//UCHAR buf[64];
//
////    TL_DispHexByte(0,18,0x00);
////    UI_WaitKey();
//
//      // --- Terminal Type ---
//      buf[0] = 0x22; // POS terminal
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TYPE, 1, buf );
//
//      // --- Terminal Capabilities ---
//      buf[0] = 0xE0;
////    buf[1] = 0xF8;
//      buf[1] = 0xE9; // PBOC2.0 ONLY, not support Offline Enciphered PIN but support Cardholder License Check
//      buf[2] = 0xC8;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_CAP, 3, buf );
//
//      // --- Additional Terminal Capabilities ---
//      buf[0] = 0x60;
//      buf[1] = 0x00;
//      buf[2] = 0xF0;
//      buf[3] = 0xA0;
//      buf[4] = 0x01;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_ADD_CAP, 5, buf );
//
//      // IFD serial number
//      buf[0] = 'I';
//      buf[1] = 'F';
//      buf[2] = 'D';
//      buf[3] = '0';
//      buf[4] = '0';
//      buf[5] = '0';
//      buf[6] = '0';
//      buf[7] = '1';
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_IFD_SN, 8, buf );
//
//      // ----------------------------
//      // Application Independent Data
//      // ----------------------------
//
//      // --- Terminal Country Code (n3) ---
//      buf[0] = 0x01;
//      buf[1] = 0x58;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_CNTR_CODE, 2, buf );
//
//      // -----------------------------
//      // Application Dependent Data
//      // (specified by payment system)
//      // -----------------------------
//
//      // --- Application Identifier (AID) ---
//
//      // clear all TERM_AID's
//      memset( buf, 0x00, TERM_AID_LEN );
//      for(i=0; i<MAX_AID_CNT; i++)
//         api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_01+i*TERM_AID_LEN, TERM_AID_LEN, buf );
//
//      // --- clear all Application Version Number (b2) ---
//      memset( buf, 0x00, AVN_LEN );
//      for(i=0; i<MAX_AVN_CNT; i++)
//         api_emv_PutDataElement( DE_TERM, ADDR_AVN_01+i*AVN_LEN, AVN_LEN, buf );
//
//      // --- clear Default DDOL ---
//      buf[0] = 0;
//      buf[1] = 0;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_DDOL, 2, buf );
//
//      // --- clear Default TDOL ---
//      buf[0] = 0;
//      buf[1] = 0;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TDOL, 2, buf );
//
//      // --- Terminal Identification (an8) ---
//      buf[0] = '3';
//      buf[1] = '1';
//      buf[2] = '0';
//      buf[3] = '0';
//      buf[4] = '0';
//      buf[5] = '0';
//      buf[6] = '0';
//      buf[7] = '0';
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TID, 8, buf );
//
//      // --- Transaction Currency Code (n3) ---
//      buf[0] = 0x09; // iso 4217, TWD=901
//      buf[1] = 0x01; //
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_CC, 2, buf );
//
//      // --- Transaction Currency Exponent (n1, decimal point from the right of amt) ---
//      buf[0] = 0x00; // iso 4217, no decimal point
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_CE, 1, buf );
//      g_term_tx_exp = 0;
//      g_term_decimal_point = 0;
//
//      // --- Transaction Type (iso 8583:Processing Code) ---
//      g_term_tx_type = TT_GOODS; // 00=GOODS, 01=SERVICE
//
//      // --- default "Language Preference" ---
//      buf[0] = 4;
//      buf[1] = 0;
//      buf[2] = 'z'; // "zh" = Chinese
//      buf[3] = 'h';
//      buf[4] = 'e'; // "en" = English
//      buf[5] = 'n';
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_LANG_PREFER, 6, buf );
//
//      // --- PBOC2.0, acquirer id ---
//      buf[0] = 0x08;
//      buf[1] = 0x22;
//      buf[2] = 0x00;
//      buf[3] = 0x00;
//      buf[4] = 0x00;
//      buf[5] = 0x01;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_ACQ_ID, 6, buf );
//
//      // --- PBOC2.0, merchant category code ---
//      buf[0] = 0x00;
//      buf[1] = 0x01;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_MCC, 2, buf );
//
//      // --- PBOC2.0, merchant id ---
//      memset( buf, 0x30, 15 );
//      buf[0] = '1';
//      buf[1] = '2';
//      buf[2] = '3';
//      buf[3] = '4';
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_MID, 15, buf );
//
//      // --- PBOC2.0, transaction reference currency code ---
//      buf[0] = 0x08; // iso 4217, USD=0840
//      buf[1] = 0x40; //
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_RCC, 2, buf );
//
//      // --- PBOC2.0, transaction reference currency exponent  ---
//      buf[0] = 0x02; // iso 4217, BEF=0056 (Belgium Franc)
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_RCE, 1, buf );
//
//      g_dhn_pinpad = 0; // pinpad works
//}

//// ---------------------------------------------------------------------------
//// FUNCTION: setup configuration 1. (PBOC L2)
////
//// List of AID's in the following sequence order:
//// 1 VSDC      : A0 00 00 00 03 10 10
////               (Application Selection Indicator indicates that multiples
////               occurences of the Application within the card are not allowed)
//// 2 Non EMV   : A0 00 00 00 99 90 90
////               (Application Selection Indicator indicates that multiples
////               occurences of the Application within the card are not allowed)
////
//// Terminal Application Version Number: 00 84
////               (if different values are supported, all values set to 00 84)
////
//// All TAC's set to 0
////
//// Default TDOL if used: ICC Application Version Number ( 9F 08)
////
//// Default DDOL: 9F 37 04 9F 47 01 8F 01 9F 32 01
////
//// Terminal Country Code: 00 56
////
//// Transaction Currency Code: 09 78 (EURO)
////
//// Terminal Floor Limit: 100 (=100.0 or 100.00 or 100.000 according to the number of decimal values)
////
//// If Random Transaction Selection process supported:
////    Target and Maximum Target percentage for random selection set to 0 to avoid random behaviour.
////
//// If Random Transaction Selection process supported:
////    Threshold value for biased random selection shall be set to 5 (or 5.0 or 5.00 or 5.000 according to the number of decimal values)
////
//// If supported, the revocation list should include the following data:
////   RID   A0 00 00 00 04
////   CA PK Index   F8
////   Certificate Serial Number   00 10 00
////
//// "If supported, the exception list should include the following data:"
////     54 13 33 90 00 00 15 96
//// ---------------------------------------------------------------------------
//void SETUP_config_01( void )
//{
//UINT  i;
//UCHAR buf[64];
//
//
//      // ------ list of AID's ------
//      // 1. VSDC
//      buf[0] = 0x08;
//      buf[1] = 0x00; // partial matching not allowed
//      buf[2] = 0xA0;
//      buf[3] = 0x00;
//      buf[4] = 0x00;
//      buf[5] = 0x00;
//      buf[6] = 0x03;
//      buf[7] = 0x10;
//      buf[8] = 0x10;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_01, 0x09, buf );
//
//      // 2. Non EMV
//      buf[0] = 0x08;
//      buf[1] = 0x00; // partial matching not allowed
//      buf[2] = 0xA0;
//      buf[3] = 0x00;
//      buf[4] = 0x00;
//      buf[5] = 0x00;
//      buf[6] = 0x99;
//      buf[7] = 0x90;
//      buf[8] = 0x90;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_02, 0x09, buf );
//
//      // end of AID list
//      buf[0] = 0x00;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_03, 0x01, buf );
//
//      // --- Application Version Number (b2) ---
//      buf[0] = 0x00;
//      buf[1] = 0x8C;
//      for( i=0; i<MAX_AVN_CNT; i++ )
//         api_emv_PutDataElement( DE_TERM, ADDR_AVN_START+(i*AVN_LEN), AVN_LEN, buf );
//
//      // --- TAC's ---
//      memset( buf, 0x00, 5 );
//
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online
//
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online
//
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf ); // TAC-Default
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online
//
//      // --- Default TDOL --- (9F 08: AVN)
//      buf[0] = 3;
//      buf[1] = 0;
//      buf[2] = 0x9F;
//      buf[3] = 0x08;
//      buf[4] = 0x02;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TDOL, 5, buf );
//
//      // --- Default DDOL ---
//      buf[0] = 0x0B; // DDOL length
//      buf[1] = 0x00; //
//
//      buf[2] = 0x9F; // the unpredictable number
//      buf[3] = 0x37; //
//      buf[4] = 0x04; //
//
//      buf[5] = 0x9F; // icc public key exponent
//      buf[6] = 0x47; //
//      buf[7] = 0x01; //
//
//      buf[8] = 0x8F; // CA public key index
//      buf[9] = 0x01; //
//
//      buf[10] = 0x9F; // issuer public key exponent
//      buf[11] = 0x32; //
//      buf[12] = 0x01; //
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_DDOL, 13, buf );
//
//      // --- Terminal Country Code (n3) ---
//      buf[0] = 0x08;  // USA
//      buf[1] = 0x40;  //
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_CNTR_CODE, 2, buf );
//
//      // --- Transaction Currency Code (n3) ---
//      buf[0] = 0x08; // iso 4217, USD=0840
//      buf[1] = 0x40; //
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_CC, 2, buf );
//
//      // --- Transaction Currency Exponent (n1, imply decimal point from the right of amt) ---
//      // the formal rule is to access the exponent according to Currency Code (iso 4217)
//      buf[0] = 0x02; // iso 4217, BEF=0056 (Belgium Franc)
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_CE, 1, buf );
//      g_term_tx_exp = 2;
//      g_term_decimal_point = 2;
//
//      // --- Terminal Floor Limit ---
//      buf[0] = 0x00;
//      buf[1] = 0x00;
//      buf[2] = 0x00;
//      buf[3] = 0x00; // integer part=100 -> 0
//      buf[4] = 0x00; //
//      buf[5] = 0x00; // decimal part=.00
//
//      buf[6] = 0x01; // TFL flag is set (TFL present)
//
//      // update all TFLs to the same value (from AID_01 to AID_16)
//      for( i=0; i<MAX_TFL_CNT; i++ )
//         {
//         api_emv_PutDataElement( DE_TERM, ADDR_TFL_01+TFL_LEN*i, TFL_LEN, buf ); // value
//         api_emv_PutDataElement( DE_TERM, ADDR_TFL_FLAG_01+i, 1, &buf[6] ); // flag
//         }
//
////    buf[0] = 0x00;
////    buf[1] = 0x00;
////    buf[2] = 0x00;
////    buf[3] = 0x00; // 0x64 -> 0
////    api_emv_PutDataElement( DE_TERM, ADDR_TERM_FL, 4, buf );
//
//      buf[0] = 0x00;
//      buf[1] = 0x00;
//      buf[2] = 0x00;
//      buf[3] = 0x01; // integer part=100
//      buf[4] = 0x00; //
//      buf[5] = 0x00; // decimal part=.00
//      api_emv_PutDataElement( DE_TERM, ADDR_TFL_01, TFL_LEN, buf );
//      api_emv_PutDataElement( DE_TERM, ADDR_TFL_02, TFL_LEN, buf );
//
//      // --- Target Percentage for Random Selection ---
//      buf[0] = 0;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01, 1, buf );
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01+RS_TP_LEN, 1, buf );
//
//      // --- Max. Target Percentage for Biased Random Selection ---
//      buf[0] = 0;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01, 1, buf );
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01+BRS_MTP_LEN, 1, buf );
//
//      // --- Threshold Value for Biased Random Selection ---
//      buf[0] = 0x00;
//      buf[1] = 0x00;
//      buf[2] = 0x00;
//      buf[3] = 0x00;
//      buf[4] = 0x05; // integer=5
//      buf[5] = 0x00; // decimal=.00
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01, BRS_THRESHOLD_LEN, buf );
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01+BRS_THRESHOLD_LEN, BRS_THRESHOLD_LEN, buf );
//
//      // --- CAPK Revocation List ---
//      buf[0] = 0xA0; // RID
//      buf[1] = 0x00;
//      buf[2] = 0x00;
//      buf[3] = 0x00;
//      buf[4] = 0x04;
//
//      buf[5] = 0xF8; // CAPKI
//
//      buf[6] = 0x00; // certificate serial number
//      buf[7] = 0x10;
//      buf[8] = 0x00;
//      api_emv_PutDataElement( DE_KEY, ADDR_CAPK_REVOCATION_LIST_01, CAPK_REVOC_LEN, buf );
//
//      memset( buf, 0x00, 9 ); // end of list
//      api_emv_PutDataElement( DE_KEY, ADDR_CAPK_REVOCATION_LIST_02, CAPK_REVOC_LEN, buf );
//
//      g_dhn_pinpad = 0; // pinpad works
//}
//
//// ---------------------------------------------------------------------------
//// FUNCTION: setup configuration 1. (EMV L2)
////
//// List of AID's in the following sequence order:
//// 1 Non EMV   : B0 12 34 56 78 12 34
////               (Application Selection Indicator indicates that multiples
////               occurences of the Application within the card are not allowed)
//// 2 MasterCard: A0 00 00 00 04 10 10
////               (Application Selection Indicator indicates that multiples
////               occurences of the Application within the card are not allowed)
//// 3 Partial   : A0 00 00 00 10
////               (Application Selection Indicator indicates that multiples
////               occurences of the Application within the card are allowed)
////
//// Terminal Application Version Number: 00 02
////               (if different values are supported, all values set to 00 02)
////
//// All TAC's set to 0
////
//// Default TDOL if used: ICC Application Version Number ( 9F 08)
////
//// Default DDOL: 9F 37 04 9F 47 01 8F 01 9F 32 01
////
//// Terminal Country Code: 00 56
////
//// Transaction Currency Code: 09 78 (EURO)
////
//// Terminal Floor Limit: 100 (=100.0 or 100.00 or 100.000 according to the number of decimal values)
////
//// If Random Transaction Selection process supported:
////    Target and Maximum Target percentage for random selection set to 0 to avoid random behaviour.
////
//// If Random Transaction Selection process supported:
////    Threshold value for biased random selection shall be set to 5 (or 5.0 or 5.00 or 5.000 according to the number of decimal values)
////
//// If supported, the revocation list should include the following data:
////   RID   A0 00 00 00 04
////   CA PK Index   F8
////   Certificate Serial Number   00 10 00
////
//// "If supported, the exception list should include the following data:"
////     54 13 33 90 00 00 15 96
//// ---------------------------------------------------------------------------
////void SETUP_config_01( void )
////{
////UINT  i;
////UCHAR buf[64];
////
//////    TL_DispHexByte(0,18,0x01);
//////    UI_WaitKey();
////
////      // ------ list of AID's ------
////      // 1. Non EMV
////      buf[0] = 0x08;
////      buf[1] = 0x00; // partial matching not allowed
////      buf[2] = 0xB0;
////      buf[3] = 0x12;
////      buf[4] = 0x34;
////      buf[5] = 0x56;
////      buf[6] = 0x78;
////      buf[7] = 0x12;
////      buf[8] = 0x34;
////      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_01, 0x09, buf );
////
////      // 2. MasterCard
////      buf[0] = 0x08;
////      buf[1] = 0x00; // partial matching not allowed
////      buf[2] = 0xA0;
////      buf[3] = 0x00;
////      buf[4] = 0x00;
////      buf[5] = 0x00;
////      buf[6] = 0x04;
////      buf[7] = 0x10;
////      buf[8] = 0x10;
////      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_02, 0x09, buf );
////
////      // 3. Partial
////      buf[0] = 0x06;
////      buf[1] = 0x01; // partial matching allowed
////      buf[2] = 0xA0;
////      buf[3] = 0x00;
////      buf[4] = 0x00;
////      buf[5] = 0x00;
////      buf[6] = 0x10;
////      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_03, 0x07, buf );
////
////      // end of AID list
////      buf[0] = 0x00;
////      api_emv_PutDataElement( DE_TERM, ADDR_TERM_AID_04, 0x01, buf );
////
////      // --- Application Version Number (b2) ---
////      buf[0] = 0x00;
////      buf[1] = 0x02;
////      api_emv_PutDataElement( DE_TERM, ADDR_AVN_01, AVN_LEN, buf );
////      api_emv_PutDataElement( DE_TERM, ADDR_AVN_02, AVN_LEN, buf );
////      api_emv_PutDataElement( DE_TERM, ADDR_AVN_03, AVN_LEN, buf );
////
////      // --- TAC's ---
////      memset( buf, 0x00, 5 );
////
////      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
////      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
////      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online
////
////      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default
////      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
////      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online
////
////      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf ); // TAC-Default
////      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial
////      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online
////
////      // --- Default TDOL --- (9F 08: AVN)
////      buf[0] = 3;
////      buf[1] = 0;
////      buf[2] = 0x9F;
////      buf[3] = 0x08;
////      buf[4] = 0x02;
////      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TDOL, 5, buf );
////
////      // --- Default DDOL ---
////      buf[0] = 0x0B; // DDOL length
////      buf[1] = 0x00; //
////
////      buf[2] = 0x9F; // the unpredictable number
////      buf[3] = 0x37; //
////      buf[4] = 0x04; //
////
////      buf[5] = 0x9F; // icc public key exponent
////      buf[6] = 0x47; //
////      buf[7] = 0x01; //
////
////      buf[8] = 0x8F; // CA public key index
////      buf[9] = 0x01; //
////
////      buf[10] = 0x9F; // issuer public key exponent
////      buf[11] = 0x32; //
////      buf[12] = 0x01; //
////      api_emv_PutDataElement( DE_TERM, ADDR_TERM_DDOL, 13, buf );
////
////      // --- Terminal Country Code (n3) ---
////      buf[0] = 0x00;  // BELGIUM
////      buf[1] = 0x56;  //
////      api_emv_PutDataElement( DE_TERM, ADDR_TERM_CNTR_CODE, 2, buf );
////
////      // --- Transaction Currency Code (n3) ---
////      buf[0] = 0x09; // iso 4217, BEF=0978 (EURO)
////      buf[1] = 0x78; //
////      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_CC, 2, buf );
////
////      // --- Transaction Currency Exponent (n1, imply decimal point from the right of amt) ---
////      // the formal rule is to access the exponent according to Currency Code (iso 4217)
////      buf[0] = 0x02; // iso 4217, BEF=0056 (Belgium Franc)
////      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_CE, 1, buf );
////      g_term_tx_exp = 2;
////      g_term_decimal_point = 2;
////
////      // --- Terminal Floor Limit ---
////      buf[0] = 0x00;
////      buf[1] = 0x00;
////      buf[2] = 0x00;
////      buf[3] = 0x01; // integer part=100
////      buf[4] = 0x00; //
////      buf[5] = 0x00; // decimal part=.00
////
////      buf[6] = 0x01; // TFL flag is set (TFL present)
////
////      // update all TFLs to the same value (from AID_01 to AID_16)
////      for( i=0; i<MAX_TFL_CNT; i++ )
////         {
////         api_emv_PutDataElement( DE_TERM, ADDR_TFL_01+TFL_LEN*i, TFL_LEN, buf ); // value
////         api_emv_PutDataElement( DE_TERM, ADDR_TFL_FLAG_01+i, 1, &buf[6] ); // flag
////         }
////
////      buf[0] = 0x00;
////      buf[1] = 0x00;
////      buf[2] = 0x00;
////      buf[3] = 0x64;
////      api_emv_PutDataElement( DE_TERM, ADDR_TERM_FL, 4, buf );
////
////      // --- Target Percentage for Random Selection ---
////      buf[0] = 0;
////      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP, 1, buf );
////
////      // --- Max. Target Percentage for Biased Random Selection ---
////      buf[0] = 0;
////      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP, 1, buf );
////
////      // --- Threshold Value for Biased Random Selection ---
////      buf[0] = 0x00;
////      buf[1] = 0x00;
////      buf[2] = 0x00;
////      buf[3] = 0x00;
////      buf[4] = 0x05; // integer=5
////      buf[5] = 0x00; // decimal=.00
////      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD, BRS_THRESHOLD_LEN, buf );
////
////      // --- CAPK Revocation List ---
////      buf[0] = 0xA0; // RID
////      buf[1] = 0x00;
////      buf[2] = 0x00;
////      buf[3] = 0x00;
////      buf[4] = 0x04;
////
////      buf[5] = 0xF8; // CAPKI
////
////      buf[6] = 0x00; // certificate serial number
////      buf[7] = 0x10;
////      buf[8] = 0x00;
////      api_emv_PutDataElement( DE_KEY, ADDR_CAPK_REVOCATION_LIST_01, CAPK_REVOC_LEN, buf );
////
////      memset( buf, 0x00, 9 ); // end of list
////      api_emv_PutDataElement( DE_KEY, ADDR_CAPK_REVOCATION_LIST_02, CAPK_REVOC_LEN, buf );
////
////      g_dhn_pinpad = 0; // pinpad works
////}
//
//// ---------------------------------------------------------------------------
//// FUNCTION: setup configuration 2. (EMV L2)
////
//// TAC denial  = 00 00 00 00 00
//// TAC online  = F8 F8 FC F8 F0
//// TAC default = 00 00 00 00 00
////
//// Default DDOL not present
////
//// If Random Transaction Selection process supported:
////    Target and Maximum Target percentage for random selection set to 99 to avoid random behaviour.
////
//// If Random Transaction Selection process supported:
////    Threshold value for biased random selection shall be set to 5
////
//// PIN Pad is malfunctioning
////
//// (unless otherwise specified, other parameters are the same as in terminal configuration 1)
//// ---------------------------------------------------------------------------
//void SETUP_config_02( void )
//{
//UINT  i;
//UCHAR buf[64];
//
////    TL_DispHexByte(0,18,0x02);
////    UI_WaitKey();
//
//      SETUP_config_01();
//
//      // --- TAC's ---
//      memset( buf, 0x00, 5 );
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
//
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
//
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf ); // TAC-Default
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial
//
//      buf[0] = 0xF8;
//      buf[1] = 0xF8;
//      buf[2] = 0xFC;
//      buf[3] = 0xF8;
//      buf[4] = 0xF0;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online
//
//      // --- Default DDOL ---
//      buf[0] = 0x00; // DDOL not present
//      buf[1] = 0x00; //
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_DDOL, 2, buf );
//
//      // --- Target Percentage for Random Selection ---
//      buf[0] = 0x99;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP, 1, buf );
//
//      // --- Max. Target Percentage for Biased Random Selection ---
//      buf[0] = 0x99;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP, 1, buf );
//
//      // --- PIN PAD ---
//      g_dhn_pinpad = apiDeviceError; // malfunctioning
//}
//
//// ---------------------------------------------------------------------------
//// FUNCTION: setup configuration 3. (EMV L2)
////
//// TAC denial  = 40 00 00 00 00 (if SDA supported) or
////               00 20 00 00 00 (if SDA not supported)
//// TAC online  = 00 00 00 00 00
//// TAC default = 00 00 00 00 00
////
//// default DDOL = 9F 47 01 8F 01 9F 32 01 (no unpredictable number)
////
//// Terminal floor limit not present
////
//// (unless otherwise specified, other parameters are the same as in terminal configuration 1)
//// ---------------------------------------------------------------------------
//void SETUP_config_03( void )
//{
//UINT  i;
//UCHAR buf[64];
//
////    TL_DispHexByte(0,18,0x03);
////    UI_WaitKey();
//
//      SETUP_config_01();
//
//      // --- TAC's ---
//      buf[0] = 0x40;
//      buf[1] = 0x00;
//      buf[2] = 0x00;
//      buf[3] = 0x00;
//      buf[4] = 0x00;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial
//
//      memset( buf, 0x00, 5 );
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
//
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default
//
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf ); // TAC-Default
//
//      // --- Default DDOL ---
//      buf[0] = 0x08; // DDOL length
//      buf[1] = 0x00; //
//
//      buf[2] = 0x9F; // icc public key exponent
//      buf[3] = 0x47; //
//      buf[4] = 0x01; //
//
//      buf[5] = 0x8F; // CA public key index
//      buf[6] = 0x01; //
//
//      buf[7] = 0x9F; // issuer public key exponent
//      buf[8] = 0x32; //
//      buf[9] = 0x01; //
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_DDOL, 10, buf );
//
//      // --- Terminal Floor Limit ---
//      memset( buf, 0x00, TFL_LEN );
//      buf[6] = 0x00; // TFL flag is cleared (TFL not present)
//
//      // update all TFLs to the same value (from AID_01 to AID_16)
//      for( i=0; i<MAX_TFL_CNT; i++ )
//         {
//         api_emv_PutDataElement( DE_TERM, ADDR_TFL_01+TFL_LEN*i, TFL_LEN, buf ); // value
//         api_emv_PutDataElement( DE_TERM, ADDR_TFL_FLAG_01+i, 1, &buf[6] ); // flag
//         }
//
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_FL, 4, buf );
//}
//
//// ---------------------------------------------------------------------------
//// FUNCTION: setup configuration 4. (EMV L2)
////
//// TAC denial  = 00 00 00 00 00
//// TAC online  = 40 00 00 00 00 (if SDA supported) or
////               00 20 00 00 00 (if SDA not supported)
//// TAC default = 00 00 00 00 00
////
//// Terminal floor limit not present
////
//// (unless otherwise specified, other parameters are the same as in terminal configuration 1)
//// ---------------------------------------------------------------------------
//void SETUP_config_04( void )
//{
//UINT  i;
//UCHAR buf[64];
//
////    TL_DispHexByte(0,18,0x04);
////    UI_WaitKey();
//
//      SETUP_config_01();
//
//      // --- TAC's ---
//      buf[0] = 0x40;
//      buf[1] = 0x00;
//      buf[2] = 0x00;
//      buf[3] = 0x00;
//      buf[4] = 0x00;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online
//
//      memset( buf, 0x00, 5 );
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
//
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default
//
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf ); // TAC-Default
//
//      // --- Terminal Floor Limit ---
//      memset( buf, 0x00, TFL_LEN );
//      buf[6] = 0x00; // TFL flag is cleared (TFL not present)
//
//      // update all TFLs to the same value (from AID_01 to AID_16)
//      for( i=0; i<MAX_TFL_CNT; i++ )
//         {
//         api_emv_PutDataElement( DE_TERM, ADDR_TFL_01+TFL_LEN*i, TFL_LEN, buf ); // value
//         api_emv_PutDataElement( DE_TERM, ADDR_TFL_FLAG_01+i, 1, &buf[6] ); // flag
//         }
//
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_FL, 4, buf );
//}
//
//// ---------------------------------------------------------------------------
//// FUNCTION: setup configuration 5. (EMV L2)
////
//// TAC denial  = 00 00 00 00 00
//// TAC online  = 00 00 00 00 00
//// TAC default = 40 00 00 00 00 (if SDA supported) or
////               00 20 00 00 00 (if SDA not supported)
////
//// Default TDOL is not present
////
//// (unless otherwise specified, other parameters are the same as in terminal configuration 1)
//// ---------------------------------------------------------------------------
//void SETUP_config_05( void )
//{
//UINT  i;
//UCHAR buf[64];
//
////    TL_DispHexByte(0,18,0x05);
////    UI_WaitKey();
//
//      SETUP_config_01();
//
//      // --- TAC's ---
//      buf[0] = 0x40;
//      buf[1] = 0x00;
//      buf[2] = 0x00;
//      buf[3] = 0x00;
//      buf[4] = 0x00;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf ); // TAC-Default
//
//      memset( buf, 0x00, 5 );
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online
//
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online
//
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online
//
//      // --- Default TDOL --- (not used)
//      buf[0] = 0;
//      buf[1] = 0;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TDOL, 2, buf );
//}
//
//// ---------------------------------------------------------------------------
//// FUNCTION: setup configuration 6. (EMV L2)
////
//// TAC denial  = 00 00 00 00 00
//// TAC online  = F8 F8 FC F8 F0
//// TAC default = 40 00 00 00 00 (if SDA supported) or
////               00 20 00 00 00 (if SDA not supported)
//// (unless otherwise specified, other parameters are the same as in terminal configuration 1)
//// ---------------------------------------------------------------------------
//void SETUP_config_06( void )
//{
//UINT  i;
//UCHAR buf[64];
//
////    TL_DispHexByte(0,18,0x06);
////    UI_WaitKey();
//
//      SETUP_config_01();
//
//      // --- TAC's ---
//      memset( buf, 0x00, 5 );
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial
//
//      buf[0] = 0xF8;
//      buf[1] = 0xF8;
//      buf[2] = 0xFC;
//      buf[3] = 0xF8;
//      buf[4] = 0xF0;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online
//
//      buf[0] = 0x40;
//      buf[1] = 0x00;
//      buf[2] = 0x00;
//      buf[3] = 0x00;
//      buf[4] = 0x00;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf ); // TAC-Default
//}
//
//// ---------------------------------------------------------------------------
//// FUNCTION: setup configuration 7. (EMV L2)
////
//// TAC denial  = 00 00 00 00 00
//// TAC online  = 00 00 00 00 00
//// TAC default = B8 F8 FC F8 F0 (if SDA supported) or
////               78 D8 FC F8 F0 (if SDA not supported)
////
//// (unless otherwise specified, other parameters are the same as in terminal configuration 1)
//// ---------------------------------------------------------------------------
//void SETUP_config_07( void )
//{
//UINT  i;
//UCHAR buf[64];
//
////    TL_DispHexByte(0,18,0x07);
////    UI_WaitKey();
//
//      SETUP_config_01();
//
//      // --- TAC's ---
//      memset( buf, 0x00, 5 );
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial
//
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online
//
//      buf[0] = 0xB8;
//      buf[1] = 0xF8;
//      buf[2] = 0xFC;
//      buf[3] = 0xF8;
//      buf[4] = 0xF0;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf ); // TAC-Default
//}
//
//// ---------------------------------------------------------------------------
//// FUNCTION: setup configuration 8. (EMV L2)
////
//// TAC denial  = 07 07 03 07 0F
//// TAC online  = 00 00 00 00 00
//// TAC default = 00 00 00 00 00
////
//// (unless otherwise specified, other parameters are the same as in terminal configuration 1)
//// ---------------------------------------------------------------------------
//void SETUP_config_08( void )
//{
//UINT  i;
//UCHAR buf[64];
//
////    TL_DispHexByte(0,18,0x08);
////    UI_WaitKey();
//
//      SETUP_config_01();
//
//      // --- TAC's ---
//      buf[0] = 0x07;
//      buf[1] = 0x07;
//      buf[2] = 0x03;
//      buf[3] = 0x07;
//      buf[4] = 0x0F;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial
//
//      memset( buf, 0x00, 5 );
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online
//
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf ); // TAC-Default
//}
//
//// ---------------------------------------------------------------------------
//// FUNCTION: setup configuration 9. (EMV L2)
////
//// TAC denial  = B8 F8 FC F8 F0 (if SDA supported) or
////               78 D8 FC F8 F0 (if SDA not supported)
//// TAC online  = 00 00 00 00 00
//// TAC default = 00 00 00 00 00
////
//// ---------------------------------------------------------------------------
//void SETUP_config_09( void )
//{
//UINT  i;
//UCHAR buf[64];
//
////    TL_DispHexByte(0,18,0x09);
////    UI_WaitKey();
//
//      SETUP_config_01();
//
//      // --- TAC's ---
//      buf[0] = 0xB8;
//      buf[1] = 0xF8;
//      buf[2] = 0xFC;
//      buf[3] = 0xF8;
//      buf[4] = 0xF0;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial
//
//      memset( buf, 0x00, 5 );
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online
//
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf ); // TAC-Default
//}
//
//// ---------------------------------------------------------------------------
//// FUNCTION: setup configuration 10. (EMV L2)
////
//// TAC denial  = 00 00 00 00 00
//// TAC online  = B8 F8 FC F8 F0 (if SDA supported) or
////               78 D8 FC F8 F0 (if SDA not supported)
//// TAC default = 00 00 00 00 00
////
//// (unless otherwise specified, other parameters are the same as in terminal configuration 1)
//// ---------------------------------------------------------------------------
//void SETUP_config_10( void )
//{
//UINT  i;
//UCHAR buf[64];
//
////    TL_DispHexByte(0,18,0x10);
////    UI_WaitKey();
//
//      SETUP_config_01();
//
//      // --- TAC's ---
//      buf[0] = 0xB8;
//      buf[1] = 0xF8;
//      buf[2] = 0xFC;
//      buf[3] = 0xF8;
//      buf[4] = 0xF0;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online
//
//      memset( buf, 0x00, 5 );
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial
//
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf ); // TAC-Default
//}
//
//// ---------------------------------------------------------------------------
//// FUNCTION: setup configuration 11. (EMV L2)
////
//// TAC denial  = 00 00 00 00 00
//// TAC online  = F8 F8 FC F8 F0
//// TAC default = B8 F8 FC F8 F0 (if SDA supported) or
////               78 D8 FC F8 F0 (if SDA not supported)
////
//// (unless otherwise specified, other parameters are the same as in terminal configuration 1)
//// ---------------------------------------------------------------------------
//void SETUP_config_11( void )
//{
//UINT  i;
//UCHAR buf[64];
//
////    TL_DispHexByte(0,18,0x11);
////    UI_WaitKey();
//
//      SETUP_config_01();
//
//      // --- TAC's ---
//      memset( buf, 0x00, 5 );
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01, 5, buf );  // TAC-Denial
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_02, 5, buf );  // TAC-Denial
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_03, 5, buf );  // TAC-Denial
//
//      buf[0] = 0xF8;
//      buf[1] = 0xF8;
//      buf[2] = 0xFC;
//      buf[3] = 0xF8;
//      buf[4] = 0xF0;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01, 5, buf );  // TAC-Online
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_02, 5, buf );  // TAC-Online
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_03, 5, buf );  // TAC-Online
//
//      buf[0] = 0xB8;
//      buf[1] = 0xF8;
//      buf[2] = 0xFC;
//      buf[3] = 0xF8;
//      buf[4] = 0xF0;
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01, 5, buf ); // TAC-Default
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_02, 5, buf ); // TAC-Default
//      api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_03, 5, buf ); // TAC-Default
//}

// ---------------------------------------------------------------------------
// FUNCTION: setup configuration for testing.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
//
//             01234567890123456789
//           0 SETUP CONFIG
//           1 NN
//           2
//           3 UPDATE?
// ---------------------------------------------------------------------------
void FUNC_setup_config( void )
{
UCHAR ibuf[16];
UCHAR obuf[16];

      UI_ClearScreen();
      UI_PutMsg( 0, COL_LEFTMOST, FONT0, EMVDC_ITEM_LEN, (UCHAR *)&EMVDC_Func_Table[EMVDC_FUNC_LEN*FN_SETUP_CONFIG+1] );

      // read current config setting
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_CONFIG_ID, 1, ibuf );
      TL_bcd2asc( 2, ibuf, obuf ); // convert to ascii(L-V)

      // show it
      UI_PutMsg( 1, COL_LEFTMOST, FONT0, 2, &obuf[1] );

      // update?
      if( TL_UpdateReq( 0, 3, COL_LEFTMOST, FONT0 ) == FALSE )
        return;

      UI_PutMsg( 1, COL_LEFTMOST, FONT0, 6, (UCHAR *)msg_confid_range );
      UI_ClearRow( 3, 1, FONT0 );

      // keyin
      if( TL_GetNumKey( 0, 0, '_', FONT0, 3, 2, ibuf ) == FALSE )
        return;

      // update
      TL_asc2bcd( 1, obuf, ibuf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_CONFIG_ID, 1, obuf );

      // setup new testing config.
      switch( obuf[0] )
            {
            case 0x00: // terminal default (clear settings)
                 SETUP_config_00();
                 break;

            case 0x01:
                 SETUP_config_01();
                 break;

            case 0x02:
                 SETUP_config_02();
                 break;

            case 0x03:
                 SETUP_config_03();
                 break;

            case 0x04:
                 SETUP_config_04();
                 break;

            case 0x05:
                 SETUP_config_05();
                 break;

            case 0x06:
                 SETUP_config_06();
                 break;

            case 0x07:
                 SETUP_config_07();
                 break;

            case 0x08:
                 SETUP_config_08();
                 break;

            case 0x09:
                 SETUP_config_09();
                 break;

            case 0x10:
                 SETUP_config_10();
                 break;

            case 0x11:
                 SETUP_config_11();
                 break;

            // --- new config for EMV 4.1a ---
            case 0x12:
                 SETUP_config_12();
                 break;

            case 0x13:
                 SETUP_config_13();
                 break;

            case 0x14:
                 SETUP_config_14();
                 break;

            case 0x15:
                 SETUP_config_15();
                 break;

            case 0x16:
                 SETUP_config_16();
                 break;

            case 0x17:
                 SETUP_config_17();
                 break;

            case 0x18:
                 SETUP_config_18();
                 break;

            case 0x19:
                 SETUP_config_19();
                 break;

            case 0x20:
                 SETUP_config_20();
                 break;

            case 0x21:
                 SETUP_config_21();
                 break;

            case 0x22:
                 SETUP_config_22();
                 break;

            case 0x23:
                 SETUP_config_23();
                 break;

            case 0x24:
                 SETUP_config_24();
                 break;

            case 0x25:
                 SETUP_config_25();
                 break;

            case 0x26:
                 SETUP_config_26();
                 break;

        //  case 0x99:
        //       SETUP_config_99(); // internal test only
        //       break;
            }
}

// ---------------------------------------------------------------------------
// FUNCTION: setup date & time.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
//
//             01234567890123456789
//           0 SET DATE TIME
//           1 YY/MM/DD hh:mm:ss
//           2
//           3 UPDATE?
// ---------------------------------------------------------------------------
void FUNC_set_date_time( void )
{
UCHAR buffer[13];

      UI_ClearScreen();
      UI_PutMsg( 0, COL_LEFTMOST, FONT0, EMVDC_ITEM_LEN, (UCHAR *)&EMVDC_Func_Table[EMVDC_FUNC_LEN*FN_SET_DATE_TIME+1] );

      // read current date & time
      TL_GetDateTime( buffer );

      // show it
      TL_ShowDateTime( 1, 0, FONT0, buffer );

      // update?
      if( TL_UpdateReq( 0, 3, COL_LEFTMOST, FONT0 ) == FALSE )
        return;

      UI_PutMsg( 1, COL_LEFTMOST, FONT0, 17, (UCHAR *)msg_null_date );
      UI_ClearRow( 3, 1, FONT0 );

      // keyin
      do{
        if( TL_GetNumKey( 0, NUM_TYPE_LEADING_ZERO, '_', FONT0, 3, 12, buffer ) == FALSE )
          return;
        } while( TL_CheckDateTime( buffer ) != TRUE );

      // update
      TL_SetDateTime( &buffer[1] );

      // show new date time
      UI_ClearRow( 3, 1, FONT0 );
      UI_PutMsg( 3, COL_RIGHTMOST, FONT0, 4, (UCHAR *)msg_b_ok );

      do{
        TL_GetDateTime( buffer );
        TL_ShowDateTime( 1, 0, FONT0, buffer );
        } while( UI_GetKeyStatus() == apiNotReady );
      UI_WaitKey();
}

// ---------------------------------------------------------------------------
// FUNCTION: setup AES DUKPT derived working key for PIN block and MAC generation.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
// ---------------------------------------------------------------------------
void	FUNC_set_AES_DUKPT_working_key_type( void )
{
    UCHAR	msg_2TDES[] = {"0-2TDEA"};
    UCHAR	msg_3TDEA[] = {"1-3TDEA"};
    UCHAR	msg_AES128[] = {"2-AES128"};
    UCHAR	msg_AES192[] = {"3-AES192"};
    UCHAR   msg_AES256[] = {"4-AES256"};


	UI_ClearScreen();
	UI_PutMsg(0, COL_LEFTMOST, FONT0, EMVDC_ITEM_LEN, (UCHAR *)&EMVDC_Func_Table[EMVDC_FUNC_LEN*FN_SET_DUKPT_KEY_TYPE+1]);
	
	TL_DispHexByte(1, 0, g_aes_dukpt_working_key_type);
	
	UI_PutMsg(2, COL_LEFTMOST, FONT0, strlen(msg_2TDES), (UCHAR *)msg_2TDES);
	UI_PutMsg(3, COL_LEFTMOST, FONT0, strlen(msg_3TDEA), (UCHAR *)msg_3TDEA);
	UI_PutMsg(4, COL_LEFTMOST, FONT0, strlen(msg_AES128), (UCHAR *)msg_AES128);
	UI_PutMsg(5, COL_LEFTMOST, FONT0, strlen(msg_AES192), (UCHAR *)msg_AES192);
    UI_PutMsg(6, COL_LEFTMOST, FONT0, strlen(msg_AES256), (UCHAR *)msg_AES256);
	
	switch(UI_WaitKey())
	{
        case 'x':
            break;
            
        case '0':
            g_aes_dukpt_working_key_type = 0;
            break;
            
        case '1':
            g_aes_dukpt_working_key_type = 1;
            break;
        
        case '2':
            g_aes_dukpt_working_key_type = 2;
            break;
        
        case '3':
            g_aes_dukpt_working_key_type = 3;
            break;
            
        case '4':
            g_aes_dukpt_working_key_type = 4;
            break;
            
        default:
            break;
	}
	      
	TL_DispHexByte(1, 0, g_aes_dukpt_working_key_type);
	
	UI_WaitKey();
}

// ---------------------------------------------------------------------------
// FUNCTION: setup ISO format for PIN block.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
// ---------------------------------------------------------------------------
void	FUNC_set_iso_format( void )
{
UCHAR	msg_ISO0[] =	{"0-ISO0"};
UCHAR	msg_ISO1[] =	{"1-ISO1"};
UCHAR	msg_ISO3[] =	{"3-ISO3"};
UCHAR	msg_ISO4[] =	{"4-ISO4"};



	UI_ClearScreen();
	UI_PutMsg( 0, COL_LEFTMOST, FONT0, EMVDC_ITEM_LEN, (UCHAR *)&EMVDC_Func_Table[EMVDC_FUNC_LEN*FN_SET_ISO_FORMAT+1] );
	
	TL_DispHexByte( 1, 0, g_iso_format );
	
	UI_PutMsg( 2, COL_LEFTMOST, FONT0, strlen(msg_ISO0), (UCHAR *)msg_ISO0 );
	UI_PutMsg( 3, COL_LEFTMOST, FONT0, strlen(msg_ISO1), (UCHAR *)msg_ISO1 );
	UI_PutMsg( 4, COL_LEFTMOST, FONT0, strlen(msg_ISO3), (UCHAR *)msg_ISO3 );
	UI_PutMsg( 5, COL_LEFTMOST, FONT0, strlen(msg_ISO4), (UCHAR *)msg_ISO4 );
	
	switch( UI_WaitKey() )
	      {
	      case 'x':
	      	   break;
	      	   
	      case '0':
	      	   g_iso_format = 0;
	      	   break;
	      	   
	      case '1':
	      	   g_iso_format = 1;
	      	   break;
	      	   
	      case '3':
	      	   g_iso_format = 3;
	      	   break;
	      	   
	      case '4':
	      	   g_iso_format = 4;
	      	   break;
	      	   
	      default:
	      	   break;
	      }
	      
	TL_DispHexByte( 1, 0, g_iso_format );
	
	UI_WaitKey();
}

// ---------------------------------------------------------------------------
// FUNCTION: setup key index (key slot#) for PIN block.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
// ---------------------------------------------------------------------------
void	FUNC_set_key_index( void )
{
UCHAR	buf[16];
UCHAR	index;


	UI_ClearScreen();
	UI_PutMsg( 0, COL_LEFTMOST, FONT0, EMVDC_ITEM_LEN, (UCHAR *)&EMVDC_Func_Table[EMVDC_FUNC_LEN*FN_SET_KEY_INDEX+1] );
	
	TL_DispHexByte( 1, 0, g_key_index );
	
	memset( buf, 0x00, sizeof(buf) );
	if( TL_GetNumKey( 0, 0, '_', FONT0, 3, 2, buf ) == FALSE )
	  return;
	
	index = atoi( &buf[1] );
	if( index > 10 )
	  index = 0;
	  
	g_key_index = index;
	
	TL_DispHexByte( 1, 0, g_key_index );
	
	UI_WaitKey();
}

// ---------------------------------------------------------------------------
// FUNCTION: setup key index (key slot#) for MAC.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
// ---------------------------------------------------------------------------
void FUNC_set_mac_key_index(void)
{
    UCHAR buf[16];
    UCHAR index;

    UI_ClearScreen();
    UI_PutMsg(0, COL_LEFTMOST, FONT0, EMVDC_ITEM_LEN, (UCHAR *)&EMVDC_Func_Table[EMVDC_FUNC_LEN * FN_SET_MAC_KEY_INDEX + 1]);

    TL_DispHexByte(1, 0, g_mac_key_index);

    memset(buf, 0x00, sizeof(buf));
    if (TL_GetNumKey(0, 0, '_', FONT0, 3, 2, buf) == FALSE)
      return;

    index = atoi(&buf[1]);
    if (index > 10)
      index = 0;

    g_mac_key_index = index;

    TL_DispHexByte(1, 0, g_mac_key_index);

    UI_WaitKey();
}

// ---------------------------------------------------------------------------
// FUNCTION: setup AID
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
//
//             01234567890123456789
//           0 SET AID
//           1 > NNNNNNNN
//           2   NNNNNNNN
//           3   NNNNNNNN  <- AID = LEN[1] ASI[1] AID[16]
// ---------------------------------------------------------------------------
void FUNC_set_AID( void )
{
     PBOC_SetAID();
}

// ---------------------------------------------------------------------------
// FUNCTION: setup Default DDOL for terminal.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
//
//             01234567890123456789
//           0 SET DDOL
//           1 NN NN NN NN NN NN NN
//           2 NN................NN
//           3 NN................NN
//           4 NN................NN
//           7 UPDATE(Y/N)?
// ---------------------------------------------------------------------------
void FUNC_set_DDOL( void )
{
      UI_ClearScreen();
      UI_PutMsg( 0, COL_LEFTMOST, FONT0, EMVDC_ITEM_LEN, (UCHAR *)&EMVDC_Func_Table[EMVDC_FUNC_LEN*FN_SET_DDOL+1] );

     //  PBOC_SetDDOL();
}

// ---------------------------------------------------------------------------
// FUNCTION: setup Default TDOL for terminal.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
//
//             01234567890123456789
//           0 SET TDOL
//           1 NN NN NN NN NN NN NN
//           2 NN................NN
//           3 NN................NN
//           4 NN................NN
//           7 UPDATE(Y/N)?
// ---------------------------------------------------------------------------
void FUNC_set_TDOL( void )
{
      UI_ClearScreen();
      UI_PutMsg( 0, COL_LEFTMOST, FONT0, EMVDC_ITEM_LEN, (UCHAR *)&EMVDC_Func_Table[EMVDC_FUNC_LEN*FN_SET_TDOL+1] );

     //  PBOC_SetTDOL();
}

// ---------------------------------------------------------------------------
// FUNCTION: get index data of CAPK of the related payment systemt.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : apiOK
//           apiFailed
//
//           CAPK STATUS
//
//           RID:NNNNNNNNNN
//
//           XX XX XX XX XX XX XX
//           XX XX XX XX XX XX XX
//                             [OK]
// ---------------------------------------------------------------------------
UCHAR FUNC_get_CAPK_index( void )
{
UCHAR i, j;
UCHAR buf[257];

      UI_ClearScreen();
      UI_PutMsg( 0, COL_LEFTMOST, FONT0, EMVDC_ITEM_LEN, (UCHAR *)&EMVDC_Func_Table[EMVDC_FUNC_LEN*FN_GET_CAPK_INDEX+1] );

      DISP_please_wait();

#ifdef	USE_RSA_SAM
      api_emv_CloseSession( g_dhn_sam );
#endif

      EMVDC_RetrievePublicKeyCA();

//    for( i=0; i<50; i++ )
//       {
//       apk_ReadRamDataKEY( ADDR_CA_PK_01+CA_PK_LEN*i, CA_PK_HEADER_LEN, buf );
//       TL_DumpHexData(0,0,CA_PK_HEADER_LEN, buf);
//       }
//    for(;;);

      api_emvk_GetPublicKeyInfo( buf );

#ifdef	USE_RSA_SAM
      g_dhn_sam = api_emv_OpenSession( RSA_SAM ); // RSA SAM
#endif

      UI_ClearRow( 1, 1, FONT1 );

      if( buf[0] == 0xff )
        {
//      DISP_empty();
        UI_WaitKey();
        return( apiOK );
        }

      for( i=0; i<MAX_AID_CNT; i++ )
         {
         if( buf[i*16+0] != 0xFF )
           {
           DISP_ok();
           DISP_rid();

           // RID
           for( j=0; j<5; j++ )
              TL_DispHexByte( 2, 2*j+4, buf[i*16+j] );

           TL_DumpHexData2( 0, 4, buf[i*16+5], &buf[i*16+6] );
           }
         }

      return( apiOK );
}

// ---------------------------------------------------------------------------
// FUNCTION: setup transaction type.            
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
//
//             01234567890123456789
//           0 SET TRANS TYPE
//           1 GOODS (or SERVICE)
//           2                       
//           3 
//           4                     
//           7 1-GOODS    2-SERVICE
// ---------------------------------------------------------------------------
//void FUNC_set_trans_type( void )
//{
//      UI_ClearScreen();
//      UI_PutMsg( 0, COL_LEFTMOST, FONT0, EMVDC_ITEM_LEN, (UCHAR *)&EMVDC_Func_Table[EMVDC_FUNC_LEN*FN_SET_TRANS_TYPE+1] );
//
//      PBOC_SetTransType();
//}                                                                                                                         

// ---------------------------------------------------------------------------
// FUNCTION: setup terminal country code.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
//
//             01234567890123456789
//           0 SET COUNTRY CODE
//           1 158                  <- n3 (2 bytes)
//           2
//           3 UPDATE?
// ---------------------------------------------------------------------------
void FUNC_set_term_country_code( void )
{
UCHAR ibuf[16];
UCHAR obuf[16];

      UI_ClearScreen();
      UI_PutMsg( 0, COL_LEFTMOST, FONT0, EMVDC_ITEM_LEN, (UCHAR *)&EMVDC_Func_Table[EMVDC_FUNC_LEN*FN_SET_CONTRY_CODE+1] );

      // read current terminal country code
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_CNTR_CODE, 2, ibuf );
      TL_bcd2asc( 2, ibuf, obuf ); // convert to ascii(L-V)

      // show it
      UI_PutMsg( 1, COL_LEFTMOST, FONT0, 3, &obuf[2] ); // ignore leading 0

      // update?
      if( TL_UpdateReq( 0, 3, COL_LEFTMOST, FONT0 ) == FALSE )
        return;

      UI_ClearRow( 3, 1, FONT0 );

      // keyin
      if( TL_GetNumKey( 0, NUM_TYPE_LEADING_ZERO, '_', FONT0, 3, 3, ibuf ) == FALSE )
        return;

      // update
      TL_asc2bcd( 2, obuf, ibuf );
      api_emv_PutDataElement( DE_TERM, ADDR_TERM_CNTR_CODE, 2, obuf );
}

// ---------------------------------------------------------------------------
// FUNCTION: setup terminal floor limit.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
//
//             01234567890123456789
//           0 SET FLOOR LIMIT
//           1 nnnnn                <- n10+(n2)
//           2
//           3 UPDATE?
// ---------------------------------------------------------------------------
void FUNC_set_term_floor_limit( void )
{
UCHAR ibuf[16];
UCHAR obuf[16];
UCHAR i;
UCHAR aidno;
UCHAR start;

      UI_ClearScreen();
      UI_PutMsg( 0, COL_LEFTMOST, FONT0, EMVDC_ITEM_LEN, (UCHAR *)&EMVDC_Func_Table[EMVDC_FUNC_LEN*FN_SET_FLOOR_LIMIT+1] );

      start = 0;
      while(1)
           {
           aidno = PBOC_SelectAID( start );
           if( aidno == 0xff ) // abort?
             return;

           // read current Terminal Floor Limit (TFL)
           api_emv_GetDataElement( DE_TERM, ADDR_TFL_01+aidno*TFL_LEN, TFL_LEN, ibuf );
           TL_bcd2asc( TFL_LEN, ibuf, obuf ); // convert to ascii(L-V)
           TL_insert_decimal_point( 0x04, obuf );
           TL_trim_decimal( g_term_tx_exp, g_term_decimal_point, obuf ); // NNNNN

           // show it
           TL_trim_asc( 0, obuf, ibuf ); // ignore the leading zeros
           UI_PutMsg( 1, COL_LEFTMOST, FONT0, ibuf[0], &ibuf[1] );

           // update?
           if( TL_UpdateReq( 0, 3, COL_LEFTMOST, FONT0 ) == FALSE )
             {
             start = aidno;
             return;
             }

           UI_ClearRow( 3, 1, FONT0 );

           // keyin
           if( TL_GetNumKey( 0, NUM_TYPE_DEC, '_', FONT0, 3, 12, ibuf ) == FALSE )
             {
             start = aidno;
             continue;
             }

           // update all TFLs to the same value (from AID_01 to AID_16)
           TL_asc2bcd( TFL_LEN, obuf, ibuf );
           obuf[6] = 0x01; // TFL flag is set (TFL present)

//         for( i=0; i<MAX_TFL_CNT; i++ )
//            {
//            api_emv_PutDataElement( DE_TERM, ADDR_TFL_01+TFL_LEN*i, TFL_LEN, obuf ); // value
//            api_emv_PutDataElement( DE_TERM, ADDR_TFL_FLAG_01+i, 1, &obuf[6] ); // flag
//            }

           // update the target FLOOR LIMIT
           api_emv_PutDataElement( DE_TERM, ADDR_TFL_01+aidno*TFL_LEN, TFL_LEN, obuf ); // value
           api_emv_PutDataElement( DE_TERM, ADDR_TFL_FLAG_01+aidno, 1, &obuf[6] ); // flag

           TL_bcd2hex( TFL_LEN, obuf, ibuf );
           api_emv_PutDataElement( DE_TERM, ADDR_TERM_FL, 4, ibuf );

           start = aidno;
           }
}

// ---------------------------------------------------------------------------
// FUNCTION: setup Threshold value for Biased Random Selection.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
//
//             01234567890123456789
//           0 SET BRS-SHRESHOLD
//           1 nnnnn                <- n10+(n2)
//           2
//           3 UPDATE?
// ---------------------------------------------------------------------------
void FUNC_set_BRS_shreshold( void )
{
UCHAR ibuf[16];
UCHAR obuf[16];
UCHAR aidno;
UCHAR start;

      UI_ClearScreen();
      UI_PutMsg( 0, COL_LEFTMOST, FONT0, EMVDC_ITEM_LEN, (UCHAR *)&EMVDC_Func_Table[EMVDC_FUNC_LEN*FN_SET_BRS_SHRESHOLD+1] );

      start = 0;
      while(1)
           {
          //  aidno = PBOC_SelectAID( start );
           if( aidno == 0xff ) // abort?
             return;

           // read "Threshold Value for Biased Random Selection"
           api_emv_GetDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01+aidno*BRS_THRESHOLD_LEN, BRS_THRESHOLD_LEN, ibuf );
           TL_bcd2asc( BRS_THRESHOLD_LEN, ibuf, obuf ); // convert to ascii(L-V)
           TL_insert_decimal_point( 0x04, obuf );

           // show it
           TL_trim_asc( 0, obuf, ibuf ); // ignore the leading zeros
           UI_PutMsg( 1, COL_LEFTMOST, FONT0, ibuf[0], &ibuf[1] );

           // update?
           if( TL_UpdateReq( 0, 3, COL_LEFTMOST, FONT0 ) == FALSE )
             {
             start = aidno;
             continue;
             }

           UI_ClearRow( 3, 1, FONT0 );

           // keyin
           if( TL_GetNumKey( 0, NUM_TYPE_DEC, '_', FONT0, 3, 12, ibuf ) == FALSE )
             {
             start = aidno;
             continue;
             }

           // update
           TL_asc2bcd( BRS_THRESHOLD_LEN, obuf, ibuf );
           api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_THRESHOLD_01+aidno*BRS_THRESHOLD_LEN, BRS_THRESHOLD_LEN, obuf );

           start = aidno;
           }
}

// ---------------------------------------------------------------------------
// FUNCTION: setup Max target percentage for Biased Random Selection. (0..99)
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
//
//             01234567890123456789
//           0 SET BRS-MTP (%)
//           1 nn                   <- n2
//           2
//           3 UPDATE?
// ---------------------------------------------------------------------------
void FUNC_set_BRS_max_target_percentage( void )
{
UCHAR ibuf[16];
UCHAR obuf[16];
UCHAR aidno;
UCHAR start;

      UI_ClearScreen();
      UI_PutMsg( 0, COL_LEFTMOST, FONT0, EMVDC_ITEM_LEN, (UCHAR *)&EMVDC_Func_Table[EMVDC_FUNC_LEN*FN_SET_BRS_MTP+1] );

      start = 0;
      while(1)
           {
          //  aidno = PBOC_SelectAID( start );
           if( aidno == 0xff ) // abort?
             return;

           // read "Max Target Percentage for Biased Random Selection"
           api_emv_GetDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01+aidno*BRS_MTP_LEN, BRS_MTP_LEN, ibuf );
           TL_bcd2asc( 1, ibuf, obuf ); // convert to ascii(L-V)

           // show it
           TL_trim_asc( 0, obuf, ibuf ); // ignore the leading zeros
           UI_PutMsg( 1, COL_LEFTMOST, FONT0, ibuf[0], &ibuf[1] );

           // update?
           if( TL_UpdateReq( 0, 3, COL_LEFTMOST, FONT0 ) == FALSE )
             {
             start = aidno;
             continue;
             }

           UI_ClearRow( 3, 1, FONT0 );

           // keyin
           if( TL_GetNumKey( 0, 0, '_', FONT0, 3, 2, ibuf ) == FALSE )
             {
             start = aidno;
             continue;
             }

           // update
           TL_asc2bcd( 1, obuf, ibuf );
           api_emv_PutDataElement( DE_TERM, ADDR_TERM_BRS_MTP_01+aidno*BRS_MTP_LEN, BRS_MTP_LEN, obuf );

           start = aidno;
           }
}

// ---------------------------------------------------------------------------
// FUNCTION: setup Target percentage for Random Selection. (0..99)
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
//
//             01234567890123456789
//           0 SET RS-TP (%)
//           1 nn                   <- n2
//           2
//           3 UPDATE?
// ---------------------------------------------------------------------------
void FUNC_set_RS_target_percentage( void )
{
UCHAR ibuf[16];
UCHAR obuf[16];
UCHAR aidno;
UCHAR start;

      UI_ClearScreen();
      UI_PutMsg( 0, COL_LEFTMOST, FONT0, EMVDC_ITEM_LEN, (UCHAR *)&EMVDC_Func_Table[EMVDC_FUNC_LEN*FN_SET_RS_TP+1] );

      start = 0;
      while(1)
           {
          //  aidno = PBOC_SelectAID( start );
           if( aidno == 0xff ) // abort?
             return;

           // read "Target Percentage for Random Selection"
           api_emv_GetDataElement( DE_TERM, ADDR_TERM_RS_TP_01+aidno*RS_TP_LEN, RS_TP_LEN, ibuf );
           TL_bcd2asc( 1, ibuf, obuf ); // convert to ascii(L-V)

           // show it
           TL_trim_asc( 0, obuf, ibuf ); // ignore the leading zeros
           UI_PutMsg( 1, COL_LEFTMOST, FONT0, ibuf[0], &ibuf[1] );

           // update?
           if( TL_UpdateReq( 0, 3, COL_LEFTMOST, FONT0 ) == FALSE )
             {
             start = aidno;
             continue;
             }

           UI_ClearRow( 3, 1, FONT0 );

           // keyin
           if( TL_GetNumKey( 0, 0, '_', FONT0, 3, 2, ibuf ) == FALSE )
             {
             start = aidno;
             continue;
             }

           // update
           TL_asc2bcd( 1, obuf, ibuf );
           api_emv_PutDataElement( DE_TERM, ADDR_TERM_RS_TP_01+aidno*RS_TP_LEN, RS_TP_LEN, obuf );

           start = aidno;
           }
}

// ---------------------------------------------------------------------------
// FUNCTION: setup TAC codes.
// INPUT   : ibuf - the old values.
// OUTPUT  : obuf - the new values.
// RETURN  : TRUE  - settings confirmed.
//           FALSE - aborted.
// ---------------------------------------------------------------------------
UCHAR SET_TAC_codes( UCHAR *ibuf, UCHAR *obuf )
{
UCHAR i;
UCHAR result;
UCHAR col;
UCHAR cnt;

      // show it
      for( i=0; i<5; i++ )
         TL_DispHexByte( 1, i*3, ibuf[i] );

      // update?
      if( TL_UpdateReq( 0, 3, COL_LEFTMOST, FONT0 ) == FALSE )
        return( FALSE );

      UI_ClearRow( 3, 1, FONT0 );

TAC_FILLER:
      // show null filler
      UI_PutMsg( 3, COL_LEFTMOST, FONT0, 20, (UCHAR *)msg_null_atc );

      col = 0;
      i = 0;
      cnt = 0;

      while(1)
           {
           result = TL_GetAlphaNumKey( 0, 3, col, FONT0, 2, obuf );
           if( result == FALSE )
             {
             if( obuf[0] == KEY_CLEAR )
               goto TAC_FILLER;
             else
               return;
             }

           ibuf[i++] = obuf[0];
           ibuf[i++] = obuf[1];

           col += 3;

           UI_PutStr( 3, col, FONT0, 2, (UCHAR *)msg_null_atc );

           if( ++cnt == 5 )
             break;
           }

      // update
      for( i=0; i<5; i++ )
         obuf[i]= TL_ascw2hexb( &ibuf[i*2] ); // convert to hex format

      return( TRUE );
}

// ---------------------------------------------------------------------------
// FUNCTION: setup TAC-DENIAL.
// INPUT   : index - the index number to the selected AID.
// OUTPUT  : none.
// RETURN  : none.
//
//             01234567890123456789
//           0 TAC-DENIAL
//           1 nn nn nn nn nn       <- b5
//           2
//           3 UPDATE?
// ---------------------------------------------------------------------------
void SET_TAC_denial( UCHAR index )
{
UCHAR i;
UCHAR ibuf[16];
UCHAR obuf[16];
UCHAR result;
UCHAR col;
UCHAR cnt;

      UI_ClearScreen();

      UI_PutMsg( 0, COL_LEFTMOST, FONT0, TAC_ITEM_LEN, (UCHAR *)&TAC_Func_Table[TAC_FUNC_LEN*FN_TAC_DENIAL+1] );

      // read TAC-DENIAL
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01+index*TAC_LEN, TAC_LEN, ibuf );

      // update
      if( SET_TAC_codes( ibuf, obuf ) == TRUE )
        api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DENIAL_01+index*TAC_LEN, TAC_LEN, obuf );
}

// ---------------------------------------------------------------------------
// FUNCTION: setup TAC-ONLINE.
// INPUT   : index - the index number to the selected AID.
// OUTPUT  : none.
// RETURN  : none.
//
//             01234567890123456789
//           0 TAC-DENIAL
//           1 nn nn nn nn nn       <- b5
//           2
//           3 UPDATE?
// ---------------------------------------------------------------------------
void SET_TAC_online( UCHAR index )
{
UCHAR i;
UCHAR ibuf[16];
UCHAR obuf[16];
UCHAR result;
UCHAR col;
UCHAR cnt;

      UI_ClearScreen();

      UI_PutMsg( 0, COL_LEFTMOST, FONT0, TAC_ITEM_LEN, (UCHAR *)&TAC_Func_Table[TAC_FUNC_LEN*FN_TAC_ONLINE+1] );

      // read TAC-ONLINE
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01+index*TAC_LEN, TAC_LEN, ibuf );

      // update
      if( SET_TAC_codes( ibuf, obuf ) == TRUE )
        api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_ONLINE_01+index*TAC_LEN, TAC_LEN, obuf );
}

// ---------------------------------------------------------------------------
// FUNCTION: setup TAC-DEFAULT.
// INPUT   : index - the index number to the selected AID.
// OUTPUT  : none.
// RETURN  : none.
//
//             01234567890123456789
//           0 TAC-DENIAL
//           1 nn nn nn nn nn       <- b5
//           2
//           3 UPDATE?
// ---------------------------------------------------------------------------
void SET_TAC_default( UCHAR index )
{
UCHAR i;
UCHAR ibuf[16];
UCHAR obuf[16];
UCHAR result;
UCHAR col;
UCHAR cnt;

      UI_ClearScreen();

      UI_PutMsg( 0, COL_LEFTMOST, FONT0, TAC_ITEM_LEN, (UCHAR *)&TAC_Func_Table[TAC_FUNC_LEN*FN_TAC_DEFAULT+1] );

      // read TAC-DEFAULT
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01+index*TAC_LEN, TAC_LEN, ibuf );

      // update
      if( SET_TAC_codes( ibuf, obuf ) == TRUE )
        api_emv_PutDataElement( DE_TERM, ADDR_TERM_TAC_DEFAULT_01+index*TAC_LEN, TAC_LEN, obuf );
}

// ---------------------------------------------------------------------------
// FUNCTION: setup Terminal Action Codes.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
//
//             01234567890123456789
//           0 SET TAC
//           1 > TAC-DENIAL
//           2   TAC-ONLINE
//           3   TAC-DEFAULT      v
//
//             01234567890123456789
//           0 TAC-XXXXX
//           1 nn nn nn nn nn       <- b5
//           2
//           3 UPDATE?
// ---------------------------------------------------------------------------
void FUNC_set_TAC( void )
{
UCHAR buffer[6];
UCHAR list[MAX_TAC_FUNC_CNT*TAC_FUNC_LEN];
UCHAR result;
UCHAR aidno;
UCHAR start;
UCHAR index;
UCHAR flag;

      UI_ClearScreen();
      UI_PutMsg( 0, COL_LEFTMOST, FONT0, EMVDC_ITEM_LEN, (UCHAR *)&EMVDC_Func_Table[EMVDC_FUNC_LEN*FN_SET_TAC+1] );

      start = 0;
      index = 0;

      while(1)
           {
           aidno = PBOC_SelectAID( index );
           if( aidno == 0xff ) // abort?
             return;

           index = aidno;

           flag = TRUE;
           while(flag)
                {
                buffer[0] = 1; // starting row number
                buffer[1] = MAX_DSP_ROW_CNT;
                buffer[2] = MAX_TAC_FUNC_CNT;   // defined in "EMVDC.h"
                buffer[3] = TAC_FUNC_LEN;       // item length
                buffer[4] = 0; // offset of LEN field in item
                buffer[5] = FONT0;

                TL_LoadFuncList( 1, list );

                result = TL_ListBox( start, buffer, &list[0], 60 ); // timeout = 60sec

                switch( result )
                      {
                      case 0xff: // aborted
                           result = 0;
                           flag = FALSE;
                           break;

                      case 0x00: // TAC-DENIAL
                           SET_TAC_denial( aidno );
                           break;

                      case 0x01: // TAC-ONLINE
                           SET_TAC_online( aidno );
                           break;

                      case 0x02: // TAC-DEFAULT
                           SET_TAC_default( aidno );
                           break;
                      }

                start = result;
                }
           } // while(1)
}

// ---------------------------------------------------------------------------
// FUNCTION: view Transaction Log.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
//
//             01234567890123456789
//           0 TRANSACTION LOG
//           1 TOTAL REC: nnnn
//           2
//           3 1-CLEAR    2-DETAILS
// ---------------------------------------------------------------------------
void FUNC_trans_log( void )
{
UCHAR buf[8];

      UI_ClearScreen();
      UI_PutMsg( 0, COL_LEFTMOST, FONT0, EMVDC_ITEM_LEN, (UCHAR *)&EMVDC_Func_Table[EMVDC_FUNC_LEN*FN_TRANS_LOG+1] );

XLOG_1000:

      // PBOC2.0 -- Read ICC Logs
      UI_PutMsg( 3, COL_LEFTMOST, FONT0, 15, (UCHAR *)msg_1term_2icc );
      buf[0] =  UI_WaitKey();

      if( buf[0] == 'x' )
        return;

      if( buf[0] == '2' ) // icc
        {
        UI_PutMsg( 0, COL_RIGHTMOST, FONT0, 4, (UCHAR *)&msg_1term_2icc[11] );
        PBOC_ReadIccLog();

        api_emv_Deactivate( g_dhn_icc ); // disable all ICC contacts
        api_emv_Deactivate( g_dhn_sam ); // disable all SAM contacts

        UI_CloseKeyAll();

        DISP_STD_remove_card();          // instruct to remove card
        while( api_emv_CardPresent( g_dhn_icc ) == emvReady );

        UI_OpenKeyAll();
        }
      else // terminal
        {
        if( buf[0] != '1' )
          goto XLOG_1000;

        UI_PutMsg( 0, COL_RIGHTMOST, FONT0, 5, (UCHAR *)&msg_1term_2icc[1] );

        UI_PutMsg( 1, COL_LEFTMOST, FONT0, 10, (UCHAR *)msg_total_rec );
        buf[0] = TL_itoa( g_term_tx_log_cnt, &buf[1] );
        UI_PutStr( 1, 11, FONT0, buf[0], &buf[1] );

        while(1)
             {
             DISP_1clear_2details();

             switch( UI_WaitKey() )
                   {
                   case KEY_1: // clear all logs

                        UI_ClearRow( 3, 1, FONT0 );
                        if( TL_UpdateReq( 2, 3, COL_LEFTMOST, FONT0 ) == TRUE )
                          {
                          TL_ClearTransLog();
                          return;
                          }
                        else
                          continue;

                   case KEY_2: // print the specified log details

                        EMVDC_PrintLogDetails();
                        return;

                   case KEY_CANCEL:
                        return;

                   default:
                        continue;
                   }
             }
        }
}

// ---------------------------------------------------------------------------
// FUNCTION: reconcile the captured transactions with host simulator.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
//
//             01234567890123456789
//           0 SETTLE
//           1 TOTAL REC: nnnn
//           2
//           3 (Y/N) ?
// ---------------------------------------------------------------------------
void FUNC_settle( void )
{
UINT  i;
UINT  iMsgType;
UCHAR buf[TX_LOG_LEN];
UCHAR amt_n[6];
ULONG amt_b;
UCHAR amt_total[6];
UCHAR btno[8];
UCHAR fWait;
UCHAR result;

      UI_ClearScreen();
      UI_PutMsg( 0, COL_LEFTMOST, FONT0, EMVDC_ITEM_LEN, (UCHAR *)&EMVDC_Func_Table[EMVDC_FUNC_LEN*FN_SETTLE+1] );

      UI_PutMsg( 1, COL_LEFTMOST, FONT0, 10, (UCHAR *)msg_total_rec );
      buf[0] = TL_itoa( g_term_tx_log_cnt, &buf[1] );
      UI_PutStr( 1, 11, FONT0, buf[0], &buf[1] );

      memset( amt_total, 0x00, 6 ); // reset total net reconciliation amount

      if( TL_UpdateReq( 1, 3, COL_LEFTMOST, FONT0 ) == TRUE )
        {
        if( g_term_tx_log_cnt == 0 )
          return;

        UI_ClearRow( 3, 1, FONT0 );
        if( TL_OpenAUX() == FALSE )
          {
          DISP_STD_processing_error();
          return;
          }

//      fWait = TRUE;
        fWait = FALSE;
        for( i=0; i<g_term_tx_log_cnt; i++ )
           {
           TL_GetTransLog( i, buf );

           // accumulate transaction amount
           memmove( amt_n, &buf[LOG_TX_AMT], 6 );
           TL_bcd_add_bcd( 6, amt_total, amt_n );

//  TL_DumpHexData(0,0,8, &buf[LOG_MSG_TYPE]);

           // check "Message Type" for BDC (Batch Data Captured)
           iMsgType = buf[LOG_MSG_TYPE+0]*256 + buf[LOG_MSG_TYPE+1];
//         if( (iMsgType == ARC_OFFLINE_APPROVED) ||            // PATCH: PBOC2.0, 2006-02-15, all tx logs are uploaded
//             (iMsgType == ARC_APPROVED_UNABLE_ONLINE) )       //
//           {
             UI_PutMsg( 7, COL_LEFTMOST, FONT0+attrCLEARWRITE, 6, (UCHAR *)msg_batchno );
             btno[0] = TL_itoa( i+1, &btno[1] );
             UI_PutStr( 7, 7, FONT0, btno[0], &btno[1] );

             if( fWait == TRUE )
               {
               result = UI_WaitKey();
               if( result == 'x' )
                 {
                 TL_CloseAUX();
                 return;
                 }
               if( result == 'n' )
                 fWait == FALSE;
               }

             if( EMVDC_OnlineReconciliation( MSGID_FINANCIAL_ADVICE, buf ) != ONL_Completed )
               {
               DISP_STD_processing_error();
               TL_WaitTime( 100 );
               TL_CloseAUX();

               return;
               }
//           }
           }

        // BDC UPLOADED: NN
        // PRESS [OK] TO SETTLE
        UI_PutMsg( 6, COL_LEFTMOST, FONT0+attrCLEARWRITE, 13, (UCHAR *)msg_bdc_uploaded );
        btno[0] = TL_itoa( i, &btno[1] );
        UI_PutStr( 6, 13, FONT0, btno[0], &btno[1] );
        UI_PutMsg( 7, COL_LEFTMOST, FONT0+attrCLEARWRITE, 20, (UCHAR *)msg_press_ok_to_finish );
        UI_WaitKey(); // stop at the latest BDC record for HOST checking

        // Settlement

//      if( TL_UpdateReq( 2, 7, COL_LEFTMOST, FONT0+attrCLEARWRITE ) == TRUE )
//        {
//        if( EMVDC_OnlineReconciliation( MSGID_SETTLE_REQ, amt_total ) == ONL_Completed )
//          {
//          TL_ClearTransLog();
//          }
//        }
//      TL_CloseAUX();

        if( EMVDC_OnlineReconciliation( MSGID_SETTLE_REQ, amt_total ) == ONL_Completed )
          TL_ClearTransLog();
  
        TL_CloseAUX();

        }
}

// ---------------------------------------------------------------------------
// FUNCTION: load CA public keys from HOST Simulator.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
//
//             01234567890123456789
//           0 LOAD CA PUBLIC KEYS
//           1
//           2
//           3 (Y/N) ?
// ---------------------------------------------------------------------------
void FUNC_load_ca_public_keys( void )
{

      UI_ClearScreen();
      UI_PutMsg( 0, COL_LEFTMOST, FONT0, EMVDC_ITEM_LEN, (UCHAR *)&EMVDC_Func_Table[EMVDC_FUNC_LEN*FN_LOAD_CAPK+1] );

      if( TL_UpdateReq( 1, 3, COL_LEFTMOST, FONT0 ) == TRUE )
        {
        EMVDC_LoadCaPublicKey();
        }
}

// ---------------------------------------------------------------------------
// FUNCTION: load PED initial keys from HOST Simulator.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
//
//             01234567890123456789
//           0 LOAD PED INIT-KEY
//           1
//           2
//           3 (Y/N) ?
// ---------------------------------------------------------------------------
void FUNC_load_ped_init_keys( void )
{
UCHAR ipek[16]; // initial PIN encryption key
UCHAR ksn[10];  // key serial number
UCHAR Shift_Reg[3];

      UI_ClearScreen();
      UI_PutMsg( 0, COL_LEFTMOST, FONT0, EMVDC_ITEM_LEN, (UCHAR *)&EMVDC_Func_Table[EMVDC_FUNC_LEN*FN_LOAD_PED_IKEY+1] );

      if( TL_UpdateReq( 1, 3, COL_LEFTMOST, FONT0 ) == TRUE )
        {
        UI_ClearScreen();
        DISP_processing();

        ipek[0] = 0x6a;
        ipek[1] = 0xc2;
        ipek[2] = 0x92;
        ipek[3] = 0xfa;
        ipek[4] = 0xa1;
        ipek[5] = 0x31;
        ipek[6] = 0x5b;
        ipek[7] = 0x4d;
        ipek[8] = 0x85;
        ipek[9] = 0x8a;
        ipek[10] = 0xb3;
        ipek[11] = 0xa3;
        ipek[12] = 0xd7;
        ipek[13] = 0xd5;
        ipek[14] = 0x93;
        ipek[15] = 0x3a;

        ksn[0] = 0xFF;
        ksn[1] = 0xFF;
        ksn[2] = 0x98;
        ksn[3] = 0x76;
        ksn[4] = 0x54;
        ksn[5] = 0x32;
        ksn[6] = 0x10;
        ksn[7] = 0xE0;
        ksn[8] = 0x00;
        ksn[9] = 0x00;

//      PED_LoadInitialKey( 0, 0, Shift_Reg, ipek, ksn );
        }
}

// ---------------------------------------------------------------------------
// FUNCTION: calculate kernel checksum and show on the display.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
//
//             01234567890123456789
//           0 KERNEL CHECKSUM
//           1
//           2
//           3 XXXX
// ---------------------------------------------------------------------------
void FUNC_kernel_checksum( void )
{
UINT  iSum;

      UI_ClearScreen();
      UI_PutMsg( 0, COL_LEFTMOST, FONT0, EMVDC_ITEM_LEN, (UCHAR *)&EMVDC_Func_Table[EMVDC_FUNC_LEN*FN_KERNEL_CHECKSUM+1] );

//    iSum = api_emv_KernelCheckSum();
      iSum = 0xBDC1;
      TL_DispHexWord( 3, 0, iSum );
      iSum = 0x9F3E;
      TL_DispHexWord( 3, 4, iSum );
      UI_WaitKey();
}

// ---------------------------------------------------------------------------
// FUNCTION: function main entry.
//           > AAAAA
//             BBBBB
//             CCCCC
//             DDDDD
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
// ---------------------------------------------------------------------------
void FUNC_entry( void )
{
UCHAR i;
UCHAR buffer[8];
UCHAR list_cnt;
//UCHAR list[MAX_EMVDC_FUNC_CNT*EMVDC_FUNC_LEN];
UCHAR result;
UCHAR start;

      start = 0;
      while(1)
           {
           UI_ClearScreen();
           UI_OpenKeyAll();

//         DISP_pls_select_item();

           buffer[0] = 0; // starting row number
           buffer[1] = MAX_DSP_ROW_CNT;
           buffer[2] = MAX_EMVDC_FUNC_CNT; // defined in "EMVDC.h"
           buffer[3] = EMVDC_FUNC_LEN;     // item length
           buffer[4] = 0; // offset of LEN field in item
           buffer[5] = FONT0;
//         TL_LoadFuncList( 0, list );
           //TL_LoadFuncList( 0, g_temp );

//         result = TL_ListBox( start, &buffer[0], &list[0], 60 ); // timeout = 60sec
           //result = TL_ListBox( start, &buffer[0], &g_temp[0], 60 ); // timeout = 60sec
           result = TL_ListBox( start, &buffer[0], &EMVDC_Func_Table[0], 60 ); // timeout = 60sec

           switch( result )
                 {
                 case 0xff: // aborted
                      return;

                      // ------------------------
                      //  Testing Configurations
                      // ------------------------
                 case 0x00: // Config.
                      FUNC_setup_config();
                      break;

                      // ------------------------------
                      //  Application Independent Data
                      // ------------------------------
                 case 0x01: // Date & Time
                      FUNC_set_date_time();
                      break;

                 case 0x02:
                      FUNC_set_AES_DUKPT_working_key_type();
                      break;

                 case 0x03: // SET ISO FORMAT
                      FUNC_set_iso_format();
                      break;
                
                 case 0x04: // SET KEY INDEX
                      FUNC_set_key_index();
                      break;

                 case 0x05: // SET MAC KEY INDEX
                      FUNC_set_mac_key_index();
                      break;
		      
                 case 0x06: // AID
                      FUNC_set_AID();
                      break;

                 case 0x07: // Terminal Country Code
                      FUNC_set_term_country_code();
                      break;

                      // ------------------------------
                      //  Application Dependent Data
                      // ------------------------------
                 case 0x08: // Terminal Floor Limit
                      FUNC_set_term_floor_limit();
                      break;

                 case 0x09: // Threshold value for Biased Random Selection
                      FUNC_set_BRS_shreshold();
                      break;

                 case 0x0A: // Max target percentage for Biased Random Selection
                      FUNC_set_BRS_max_target_percentage();
                      break;

                 case 0x0B: // Target percentage for Random Selection
                      FUNC_set_RS_target_percentage();
                      break;

                 case 0x0C: // Terminal Action Codes
                      FUNC_set_TAC();
                      break;

                 case 0x0D: // Transaction Log
                      FUNC_trans_log();
                      break;

                 case 0x0E: // Settle
                      FUNC_settle();
                      break;

                 case 0x0F: // Load CA Public Keys
//                    FUNC_load_ca_public_keys();
                      break;

                 case 0x10: // Load PED Initial Keys
//                    FUNC_load_ped_init_keys();
                      break;

                 case 0x11: // Show Kernel Checksum
                      FUNC_kernel_checksum();
                      break;

                 case 0x12: // Default DDOL         
                      FUNC_set_DDOL();
                      break;

                 case 0x13: // Default TDOL         
                      FUNC_set_TDOL();
                      break;

                 case 0x14: // Get CAPK Index
                      FUNC_get_CAPK_index();
                      break;
                 }

           start = result;
           } // while(1)
}

// ---------------------------------------------------------------------------
void FUNC_UploadLastBDC( void )
{

     EMVDC_UploadLastBDC();

}

// ---------------------------------------------------------------------------
