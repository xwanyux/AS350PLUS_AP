//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : EMV L2                                                     **
//**  PRODUCT  : AS330	                                                    **
//**                                                                        **
//**  FILE     : EMVDCDSP.C                                                 **
//**  MODULE   : Display Standard Messages.                                 **
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
#include "EMVDCMSG.h"
#include "UI.h"


// ---------------------------------------------------------------------------
// FUNCTION: display "[GOODS/SERVICES]
//
// ---------------------------------------------------------------------------
void DISP_trans_type()
{

      if( g_term_tx_type == TT_GOODS )
        UI_PutMsg( 3, COL_LEFTMOST, FONT0+attrCLEARWRITE, 7, (UCHAR *)msg_goods );
      else
        UI_PutMsg( 3, COL_LEFTMOST, FONT0+attrCLEARWRITE, 10, (UCHAR *)msg_services );
}

// ---------------------------------------------------------------------------
// FUNCTION: display main menu.
//
//             01234567890123456789
//           0 EMV LEVEL2 TEST V1.0
//           1
//           2
//           3 READY           [OK]
// ---------------------------------------------------------------------------
void DISP_main_menu()
{
UCHAR	buf[1];
UCHAR	msg_1[] =	{"[1] UPLOAD BDC"};
// UCHAR	msg_2[] =	{"[2] CTLS READER"};
// UCHAR	msg_2[] =	{"[2] B17 TEST"};
UCHAR	msg_2[] =	{"[2] BUF OVERFLOW TEST"};
UCHAR	msg_3[] =	{"[3] MORE FUNCTIONS"};
UCHAR	msg_4[] =	{"[4] PIN EXHAUST TEST"};


      UI_ClearRow( 0, 1, FONT0+attrREVERSE );

      UI_PutMsg( 0, COL_MIDWAY, FONT0+attrREVERSE, 13, "AS350 PCI PTS" );

      // 2014-10-27, APP version
//    UI_PutMsg( 4, COL_LEFTMOST, FONT0, 21, "VER:AS-PED-02-AP-V1.0" );

      UI_PutMsg( 2, COL_LEFTMOST, FONT0, 7, (UCHAR *)msg_config );
      api_emv_GetDataElement( DE_TERM, ADDR_TERM_CONFIG_ID, 1, buf );
      TL_DispHexByte( 2, 7, buf[0] );

      UI_PutMsg( 7, COL_LEFTMOST, FONT0, 16, (UCHAR *)msg_start );
      
      UI_PutMsg( 13-1, COL_LEFTMOST, FONT0, strlen(msg_1), (UCHAR *)msg_1 );
      UI_PutMsg( 14-1, COL_LEFTMOST, FONT0, strlen(msg_2), (UCHAR *)msg_2 );
      UI_PutMsg( 15-1, COL_LEFTMOST, FONT0, strlen(msg_3), (UCHAR *)msg_3 );
      UI_PutMsg( 16-1, COL_LEFTMOST, FONT0, strlen(msg_4), (UCHAR *)msg_4 );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "INSERT CARD"
//
//           __INSERT CARD__
// ---------------------------------------------------------------------------
void DISP_STD_insert_card()
{
//    UI_PutMsg( 0, COL_MIDWAY, FONT2+attrCLEARWRITE, 10, (UCHAR *)stdmsg_C_insert_card );
      UI_PutMsg( 1, COL_MIDWAY, FONT0+attrCLEARWRITE, 12, (UCHAR *)stdmsg_insert_card );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "USE CHIP READER"
//
//           USE CHIP READER
// ---------------------------------------------------------------------------
void DISP_STD_use_chip_reader()
{
//    UI_PutMsg( 0, COL_MIDWAY, FONT2+attrCLEARWRITE, 10, (UCHAR *)stdmsg_C_use_chip_reader );
      UI_PutMsg( 1, COL_MIDWAY, FONT0+attrCLEARWRITE, 15, (UCHAR *)stdmsg_use_chip_reader );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "PLEASE WAIT"
//
//           __PLEASE WAIT__
// ---------------------------------------------------------------------------
void DISP_STD_please_wait()
{
      TL_WaitTime(30);
//    UI_PutMsg( 0, COL_MIDWAY, FONT2+attrCLEARWRITE, 6, (UCHAR *)stdmsg_C_please_wait );
      UI_PutMsg( 1, COL_MIDWAY, FONT0+attrCLEARWRITE, 11, (UCHAR *)stdmsg_please_wait );
      TL_WaitTime(30);
}

// ---------------------------------------------------------------------------
// FUNCTION: display "PROCESSING ERR"
//
// ---------------------------------------------------------------------------
void DISP_STD_processing_error()
{
//    UI_PutMsg( 0, COL_MIDWAY, FONT2+attrCLEARWRITE, 8, (UCHAR *)stdmsg_C_processing_error );
      UI_PutMsg( 1, COL_MIDWAY, FONT0+attrCLEARWRITE, 14, (UCHAR *)stdmsg_processing_error );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "CARD ERROR"
//
//           __CARD ERROR___
// ---------------------------------------------------------------------------
void DISP_STD_card_error()
{
//    UI_PutMsg( 0, COL_MIDWAY, FONT2+attrCLEARWRITE, 8, (UCHAR *)stdmsg_C_card_error );
      UI_PutMsg( 1, COL_MIDWAY, FONT0+attrCLEARWRITE, 10, (UCHAR *)stdmsg_card_error );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "REMOVE CARD"
//
//           __REMOVE CARD__
// ---------------------------------------------------------------------------
void DISP_STD_remove_card()
{
//    UI_PutMsg( 0, COL_MIDWAY, FONT2+attrCLEARWRITE, 10, (UCHAR *)stdmsg_C_remove_card );
      UI_PutMsg( 1, COL_MIDWAY, FONT0+attrCLEARWRITE, 11, (UCHAR *)stdmsg_remove_card );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "TRY AGAIN"
//
//           ---TRY AGAIN---
// ---------------------------------------------------------------------------
void DISP_STD_try_again()
{
//    UI_PutMsg( 0, COL_MIDWAY, FONT2+attrCLEARWRITE, 6, (UCHAR *)stdmsg_C_try_again );
      UI_PutMsg( 1, COL_MIDWAY, FONT0+attrCLEARWRITE, 9, (UCHAR *)stdmsg_try_again );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "NOT ACCEPTED"
//
//           --NOT ACCEPTED-
// ---------------------------------------------------------------------------
void DISP_STD_not_accepted()
{
//    UI_PutMsg( 0, COL_MIDWAY, FONT2+attrCLEARWRITE, 6, (UCHAR *)stdmsg_C_not_accepted );
      UI_PutMsg( 1, COL_MIDWAY, FONT0+attrCLEARWRITE, 12, (UCHAR *)stdmsg_not_accepted );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "ENTER AMOUNT"
//
//           --ENTER AMOUNT-
// ---------------------------------------------------------------------------
void DISP_STD_enter_amount()
{
//    UI_PutMsg( 0, COL_LEFTMOST, FONT1+attrCLEARWRITE, 12, (UCHAR *)stdmsg_enter_amount );

      UI_PutMsg( 0, COL_LEFTMOST, FONT0+attrCLEARWRITE, 6, (UCHAR *)stdmsg_amount );
//    UI_PutMsg( 0, COL_RIGHTMOST, FONT2, 4, stdmsg_C_amount );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "ENTER PIN"
//
//           -- ENTER PIN --
// ---------------------------------------------------------------------------
//void DISP_STD_enter_pin()
//{
//      UI_PutMsg( 0, COL_MIDWAY, FONT1, 9, stdmsg_enter_pin );
//}

// ---------------------------------------------------------------------------
// FUNCTION: display "(AMOUTN) OK?"
//
//           (AMOUNT) OK?---
// ---------------------------------------------------------------------------
//void DISP_STD_amount_ok()
//{
//      UI_PutMsg( 3, COL_LEFTMOST, FONT0, 12, stdmsg_amount_ok );
//}

// ---------------------------------------------------------------------------
// FUNCTION: display "CALL YOUR BANK"
//
//           -CALL YOUR BANK
// ---------------------------------------------------------------------------
void DISP_STD_call_your_bank()
{
//    UI_PutMsg( 0, COL_MIDWAY, FONT2+attrCLEARWRITE, 8, (UCHAR *)stdmsg_C_call_your_bank );
      UI_PutMsg( 1, COL_MIDWAY, FONT0, 14, (UCHAR *)stdmsg_call_your_bank );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "APPROVED"
//
//           -- APPROVED ---
// ---------------------------------------------------------------------------
void DISP_STD_approved()
{
//    UI_PutMsg( 0, COL_MIDWAY, FONT2+attrCLEARWRITE, 8, (UCHAR *)stdmsg_C_approved );
      UI_PutMsg( 1, COL_MIDWAY, FONT0+attrCLEARWRITE, 8, (UCHAR *)stdmsg_approved );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "DECLINED"
//
//           -- DECLINED ---
// ---------------------------------------------------------------------------
void DISP_STD_declined()
{
//    UI_PutMsg( 0, COL_MIDWAY, FONT2+attrCLEARWRITE, 8, (UCHAR *)stdmsg_C_declined );
      UI_PutMsg( 1, COL_MIDWAY, FONT0+attrCLEARWRITE, 8, (UCHAR *)stdmsg_declined );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "SELECT"
//
//           SELECT_________
// ---------------------------------------------------------------------------
void DISP_select_app()
{
//    UI_PutStr( 0, 0, FONT1+attrCLEARWRITE, 7, msg_select );
//    UI_PutStr( 0, 7, FONT1, 5, msg_appl );

//    UI_PutMsg( 0, COL_RIGHTMOST, FONT2, 4, (UCHAR *)msg_C_select );
      UI_PutMsg( 0, COL_LEFTMOST, FONT0, 6, (UCHAR *)msg_select );
//    UI_PutStr( 0, 7, FONT0, 11, msg_application );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "SELECT"
//
//           SELECT_________
// ---------------------------------------------------------------------------
void DISP_select()
{
      UI_PutStr( 0, 0, FONT0, 6, (UCHAR *)msg_select );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "PAN:"
//
//           PAN:
// ---------------------------------------------------------------------------
void DISP_pan( UCHAR row )
{
//    UI_PutMsg( row, COL_LEFTMOST, FONT0, 4, msg_pan );

//    UI_PutMsg( row, COL_RIGHTMOST, FONT2, 4, (UCHAR *)msg_C_pan );
      UI_PutMsg( row, COL_LEFTMOST, FONT0, 3, (UCHAR *)msg_pan );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "1-APPROVE  2-DECLINE"
//
//           1-APPROVE  2-DECLINE
// ---------------------------------------------------------------------------
void DISP_1approve_2decline( UCHAR row )
{
//    UI_PutMsg( row, COL_LEFTMOST, FONT0, 20, msg_1approve_2decline );

//    UI_PutStr( 0, 0, FONT2+attrCLEARWRITE, 14, (UCHAR *)msg_C_1approve );
      UI_PutStr( 1, 0, FONT2+attrCLEARWRITE, 14, (UCHAR *)msg_C_2decline );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "1-CLEAR    2-DETAILS"
//
// ---------------------------------------------------------------------------
void DISP_1clear_2details( void )
{
      UI_PutStr( 3, 0, FONT0, 20, (UCHAR *)msg_1clear_2details );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "PRINT LOG DETAILS"
//
// ---------------------------------------------------------------------------
void DISP_print_log_details( void )
{
      UI_PutStr( 0, 0, FONT0, 17, (UCHAR *)msg_print_log_details );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "ENTER TX SEQ COUNTER"
//
// ---------------------------------------------------------------------------
void DISP_enter_tx_sc( void )
{
      UI_PutStr( 2, 0, FONT0, 20, (UCHAR *)msg_enter_tx_sc );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "RECORD NOT FOUND"
//
// ---------------------------------------------------------------------------
void DISP_record_not_found( void )
{
      UI_PutMsg( 2, COL_MIDWAY, FONT0+attrCLEARWRITE, 16, (UCHAR *)msg_record_not_found );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "APPROVAL CODE"
//
// ---------------------------------------------------------------------------
//void DISP_approval_code()
//{
//      UI_PutMsg( 0, COL_MIDWAY, FONT1, 13, msg_approval_code );
//}

// ---------------------------------------------------------------------------
// FUNCTION: display "APPROVE"
//
// ---------------------------------------------------------------------------
void DISP_approve( UCHAR row, UCHAR col, UCHAR font )
{
      UI_PutStr( row, col, font, 7, (UCHAR *)msg_approve );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "DECLINE"
//
// ---------------------------------------------------------------------------
void DISP_decline( UCHAR row, UCHAR col, UCHAR font )
{
      UI_PutStr( row, col, font, 7, (UCHAR *)msg_decline );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "ENTER AUTH CODE"
//
// ---------------------------------------------------------------------------
//void DISP_enter_auth_code( UCHAR row, UCHAR col, UCHAR font )
//{
//      UI_PutStr( row, col, font, 15, msg_enter_auth_code );
//}

// ---------------------------------------------------------------------------
// FUNCTION: display "AUTH CODEcccccc"
//
// ---------------------------------------------------------------------------
void DISP_enter_authcode( void )
{
      UI_PutMsg( 0, COL_LEFTMOST, FONT0+attrCLEARWRITE, 9, (UCHAR *)msg_auth_code );
//    UI_PutMsg( 0, COL_RIGHTMOST, FONT2, 6, (UCHAR *)msg_C_auth_code );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "1-ONLINE"
//
// ---------------------------------------------------------------------------
void DISP_1online( UCHAR row, UCHAR col, UCHAR font )
{
      UI_PutStr( row, col, font, 8, (UCHAR *)msg_1online );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "2-DECLINE"
//
// ---------------------------------------------------------------------------
void DISP_2decline( UCHAR row, UCHAR col, UCHAR font )
{
      UI_PutStr( row, col, font, 9, (UCHAR *)msg_2decline );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "2-DECLINE   3-ACCEPT"
//
// ---------------------------------------------------------------------------
void DISP_2decline_3accept( UCHAR row, UCHAR col, UCHAR font )
{
      UI_PutStr( row, col, font, 20, (UCHAR *)msg_2decline_3accept );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "PROCESSING"
//
// ---------------------------------------------------------------------------
void DISP_processing()
{
//    UI_PutMsg( 0, COL_MIDWAY, FONT2+attrCLEARWRITE, 6, (UCHAR *)msg_C_processing );
      UI_PutMsg( 1, COL_MIDWAY, FONT0+attrCLEARWRITE, 10, (UCHAR *)msg_processing );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "REVERSAL"
//
// ---------------------------------------------------------------------------
void DISP_reversal()
{
//    UI_PutMsg( 0, COL_MIDWAY, FONT2+attrCLEARWRITE, 4, (UCHAR *)msg_C_reversal );
      UI_PutMsg( 1, COL_MIDWAY, FONT0+attrCLEARWRITE, 8, (UCHAR *)msg_reversal );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "ONLINE     cccc"
//                   "  PROCESSING   "
//
// ---------------------------------------------------------------------------
void DISP_online_processing()
{
      UI_PutMsg( 0, COL_LEFTMOST, FONT0+attrCLEARWRITE, 6, (UCHAR *)msg_online );
//    UI_PutMsg( 0, COL_RIGHTMOST, FONT2, 4, (UCHAR *)msg_C_online );

      UI_PutMsg( 1, COL_MIDWAY, FONT0+attrCLEARWRITE, 10, (UCHAR *)msg_processing );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "ONLINE     cccc"
//
// ---------------------------------------------------------------------------
void DISP_online()
{
      UI_PutMsg( 0, COL_LEFTMOST, FONT0+attrCLEARWRITE, 6, (UCHAR *)msg_online );
//    UI_PutMsg( 0, COL_RIGHTMOST, FONT2, 4, (UCHAR *)msg_C_online );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "REVERSAL   cccc"
//                   "  PROCESSING   "
//
// ---------------------------------------------------------------------------
void DISP_reversal_processing()
{
      UI_PutMsg( 0, COL_LEFTMOST, FONT0+attrCLEARWRITE, 8, (UCHAR *)msg_reversal );
//    UI_PutMsg( 0, COL_RIGHTMOST, FONT2, 4, (UCHAR *)msg_C_reversal );

      UI_PutMsg( 1, COL_MIDWAY, FONT0+attrCLEARWRITE, 10, (UCHAR *)msg_processing );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "TRANS LOG FULL"
//
// ---------------------------------------------------------------------------
void DISP_trans_log_full()
{
      UI_PutMsg( 0, COL_MIDWAY, FONT0+attrCLEARWRITE, 14, (UCHAR *)msg_trans_log_full );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "SALE          "
//
// ---------------------------------------------------------------------------
void DISP_sale()
{
      UI_PutMsg( 0, COL_LEFTMOST, FONT0+attrCLEARWRITE, 4, (UCHAR *)msg_sale );
//    UI_PutMsg( 0, COL_RIGHTMOST, FONT2, 4, (UCHAR *)msg_C_sale );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "AUTH          "
//
// ---------------------------------------------------------------------------
void DISP_auth()
{
      UI_PutMsg( 0, COL_LEFTMOST, FONT0+attrCLEARWRITE, 4, (UCHAR *)msg_auth );
//    UI_PutMsg( 0, COL_RIGHTMOST, FONT2, 4, (UCHAR *)msg_C_auth );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "OUT OF SERVICE"
//
// ---------------------------------------------------------------------------
void DISP_out_of_service()
{
      UI_ClearScreen();
//    UI_PutMsg( 0, COL_MIDWAY, FONT2+attrCLEARWRITE, 8, (UCHAR *)msg_C_out_of_service );
      UI_PutMsg( 1, COL_MIDWAY, FONT0, 14, (UCHAR *)msg_out_of_service );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "PRINTING"
//
// ---------------------------------------------------------------------------
void DISP_printing()
{
//    UI_PutMsg( 0, COL_MIDWAY, FONT2+attrCLEARWRITE, 6, (UCHAR *)msg_C_printing );
      UI_PutMsg( 1, COL_MIDWAY, FONT0+attrCLEARWRITE, 8, (UCHAR *)msg_printing );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "SWIPE CARD"
//
//           __SWIPE CARD__
// ---------------------------------------------------------------------------
void DISP_swipe_card()
{
//    UI_PutMsg( 0, COL_MIDWAY, FONT2+attrCLEARWRITE, 8, (UCHAR *)msg_C_swipe_card );
      UI_PutMsg( 1, COL_MIDWAY, FONT0+attrCLEARWRITE, 10, (UCHAR *)msg_swipe_card );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "LOAD KEY OK"
//
//           __LOAD KEY OK_
// ---------------------------------------------------------------------------
void DISP_load_key_ok()
{
      UI_PutMsg( 1, COL_MIDWAY, FONT0+attrCLEARWRITE, 11, (UCHAR *)msg_load_key_ok );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "INVALID KEY"
//
//           __INVALID KEY__
// ---------------------------------------------------------------------------
void DISP_load_key_failed()
{
      UI_PutMsg( 1, COL_MIDWAY, FONT0+attrCLEARWRITE, 15, (UCHAR *)msg_load_key_failed );
}

// ---------------------------------------------------------------------------
//void DISP_verify_ch_license()
//{
//      UI_PutMsg( 0, COL_LEFTMOST, FONT2+attrCLEARWRITE, 16, (UCHAR *)msg_C_verify_ch_license );
//}

// ---------------------------------------------------------------------------
//void DISP_license_00()
//{
//      UI_PutMsg( 1, COL_LEFTMOST, FONT2+attrCLEARWRITE, 6, (UCHAR *)msg_C_license_00 );
//}

// ---------------------------------------------------------------------------
//void DISP_license_01()
//{
//      UI_PutMsg( 1, COL_LEFTMOST, FONT2+attrCLEARWRITE, 6, (UCHAR *)msg_C_license_01 );
//}

// ---------------------------------------------------------------------------
//void DISP_license_02()
//{
//      UI_PutMsg( 1, COL_LEFTMOST, FONT2+attrCLEARWRITE, 4, (UCHAR *)msg_C_license_02 );
//}

// ---------------------------------------------------------------------------
//void DISP_license_03()
//{
//      UI_PutMsg( 1, COL_LEFTMOST, FONT2+attrCLEARWRITE, 6, (UCHAR *)msg_C_license_03 );
//}

// ---------------------------------------------------------------------------
//void DISP_license_04()
//{
//      UI_PutMsg( 1, COL_LEFTMOST, FONT2+attrCLEARWRITE, 10, (UCHAR *)msg_C_license_04 );
//}

// ---------------------------------------------------------------------------
//void DISP_license_05()
//{
//      UI_PutMsg( 1, COL_LEFTMOST, FONT2+attrCLEARWRITE, 4, (UCHAR *)msg_C_license_05 );
//}

// ---------------------------------------------------------------------------
//void DISP_correct_or_not()
//{
//      UI_PutMsg( 3, COL_LEFTMOST, FONT2+attrCLEARWRITE, 14, (UCHAR *)msg_C_correct_or_not );
//}

// ---------------------------------------------------------------------------
// FUNCTION: display special 'E'
//
// ---------------------------------------------------------------------------
//void DISP_graphic_E( UCHAR x, UCHAR y )
//{
//UCHAR data_e[] = { 0xF0, 0xF0, 0x10, 0x10, 0x10, 0x10, 0x00,
//                   0x1F, 0x1F, 0x11, 0x11, 0x11, 0x10, 0x00 };
//UCHAR sbuf[16];
//UCHAR dbuf[16];
//
//      sbuf[0] = x;
//      sbuf[1] = y;
//      sbuf[2] = 7;
//      sbuf[3] = 16;
//      sbuf[4] = BMP_OW;
//
//      memmove( dbuf, data_e, 14 );
//      api_lcd_putgraphics( 0, sbuf, dbuf);
//}

// ---------------------------------------------------------------------------
// FUNCTION: display graphic "ENTER AMOUNT"  CCCC
//
//           --ENTER AMOUNT-
// ---------------------------------------------------------------------------
void DISP_GRP_enter_amount()
{
      UI_PutMsg( 0, COL_LEFTMOST, FONT0+attrCLEARWRITE, 12, (UCHAR *)stdmsg_enter_amount );

  //  UI_PutMsg( 0, COL_LEFTMOST, FONT1+attrCLEARWRITE, 6, stdmsg_amount );
  //  UI_PutMsg( 0, COL_RIGHTMOST, FONT2, 4, stdmsg_C_amount );

//      UI_ClearRow( 0, 1, FONT1 );
//      DISP_graphic_E( 0, 0 );                         // E
//      UI_PutChar( 0, 7, FONT1+attrPIXCOLUMN, 0x4E );  // N
//      UI_PutChar( 0, 14, FONT1+attrPIXCOLUMN, 0x54 ); // T
//      DISP_graphic_E( 22, 0 );                        // E
//      UI_PutChar( 0, 29, FONT1+attrPIXCOLUMN, 0x52 ); // R

//      UI_PutChar( 0, 41, FONT1+attrPIXCOLUMN, 0x41 ); // A
//      UI_PutChar( 0, 49, FONT1+attrPIXCOLUMN, 0x4D ); // M
//      UI_PutChar( 0, 57, FONT1+attrPIXCOLUMN, 0x4F ); // O
//      UI_PutChar( 0, 65, FONT1+attrPIXCOLUMN, 0x55 ); // U
//      UI_PutChar( 0, 73, FONT1+attrPIXCOLUMN, 0x4E ); // N
//      UI_PutChar( 0, 80, FONT1+attrPIXCOLUMN, 0x54 ); // T

//    UI_PutMsg( 0, COL_RIGHTMOST, FONT2, 4, (UCHAR *)stdmsg_C_amount );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "PLEASE SELECT ITEM"
//
// ---------------------------------------------------------------------------
void DISP_pls_select_item()
{
      UI_PutMsg( 7, COL_LEFTMOST, FONT0+attrCLEARWRITE, 18, (UCHAR *)msg_pls_select_item );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "RID:"
//
// ---------------------------------------------------------------------------
void DISP_rid()
{
      UI_PutMsg( 2, COL_LEFTMOST, FONT0+attrCLEARWRITE, 4, (UCHAR *)msg_rid );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "[OK]"
//
// ---------------------------------------------------------------------------
void DISP_ok()
{
      UI_PutMsg( 7, COL_RIGHTMOST, FONT0, 4, (UCHAR *)msg_b_ok );
}

// ---------------------------------------------------------------------------
// FUNCTION: display "PLEASE WAIT"
//
//           __PLEASE WAIT__
// ---------------------------------------------------------------------------
void DISP_please_wait()
{
//    UI_PutMsg( 0, COL_MIDWAY, FONT2+attrCLEARWRITE, 6, (UCHAR *)stdmsg_C_please_wait );
      UI_PutMsg( 1, COL_MIDWAY, FONT0+attrCLEARWRITE, 11, (UCHAR *)stdmsg_please_wait );
}

// ---------------------------------------------------------------------------
