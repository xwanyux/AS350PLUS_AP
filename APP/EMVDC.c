//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : EMV L2                                                     **
//**  PRODUCT  : AS330	                                                    **
//**                                                                        **
//**  FILE     : EMVDC.C                                                    **
//**  MODULE   : main()                                                     **
//**  VERSION  : V1.00                                                      **
//**  DATE     : 2018/08/08                                                 **
//**  EDITOR   : James Hsieh                                                **
//**                                                                        **
//**  Copyright(C) 2002-2018 SymLink Corporation. All rights reserved.      **
//**                                                                        **
//****************************************************************************
//============================================================================
//
//----------------------------------------------------------------------------
#include <string.h>

#include "APPEMVconfig.h"

#include "POSAPI.h"
#include "EMVDC.h"
// #include "GDATA.h"
#include "GDATAEX.h"//20220406 modified by west.Replace GDATA.h with GDATAEX.h, prevent from duplicate definition with L2API_xx in EMVK. 
#include "EMVAPI.h"
#include "UI.h"
#include "PEDAPI.h" // ==== [Debug] ====

extern	void	CTLS_InitReader( void );
extern	void	PCI_CTLS_Transaction( void );
extern	void	DTR_B17_test( void );


//----------------------------------------------------------------------------
void TEST_init_term_para()
{
UCHAR buf[16];


      g_test_read_app_data = 0;

      g_beep_on = 1; // 0=default turn off beep (will be moved to FUNC)

//      api_emv_GetDataElement( DE_TERM, ADDR_TERM_CONFIG_ID, 1, buf );
//      if( buf[0] == 0 )
//        {
        // setup default config = 1
        SETUP_config_00();
        SETUP_config_01();
        buf[0] = 1;
        api_emv_PutDataElement( DE_TERM, ADDR_TERM_CONFIG_ID, 1, buf );
//        }
}



// ---------------------------------------------------------------------------
// FUNCTION: To generate MAC by using Master/Session key or DUKPT algorithm. (used for test)
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : apiOK
//	         apiFailed
// ---------------------------------------------------------------------------
UINT8    TEST_GenMAC(void)
{
    UINT8   result = apiFailed;
    UINT8   buf[3];
    UINT8   num;
    UINT8   select = 0;
    UINT8   index = 0;
    UINT8	macAlgo = 0;
    UINT8	padding = 0;
    UINT8	ap;	//MAC_ALGx + MAC_PADx
    UINT16  mode = 0;
    UINT8	iv[8] = {0};
    UINT8   testData[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
    UINT8	mac[16];
    UINT8	ksn[12];
    UINT8	idx;    // ==== [Debug] ====


    ap = MAC_ISO16609 | MAC_PAD1;
    printf("MAC_ALGx + MAC_PADx = 0x%02X\n", ap);
    index = 1;

    memset(mac, 0x00, sizeof(mac));
    result = api_ped_GenMAC_MSKEY(ap, index, iv, sizeof(testData), testData, mac);

    if(result == apiOK)
    {
        printf("Generate MAC by using Master/Session key algorithm : ");
        for(idx = 0 ; idx < sizeof(mac) ; idx++)
            printf("0x%02X ", mac[idx]);

        printf("\n");
    }

    ////////////////////////////////////////////////////////////////////////////////////

    ap = MAC_ISO16609 | MAC_PAD2;
    printf("MAC_ALGx + MAC_PADx = 0x%02X\n", ap);
    index = 1;

    memset(mac, 0x00, sizeof(mac));
    result = api_ped_GenMAC_MSKEY(ap, index, iv, sizeof(testData), testData, mac);

    if(result == apiOK)
    {
        printf("Generate MAC by using Master/Session key algorithm : ");
        for(idx = 0 ; idx < sizeof(mac) ; idx++)
            printf("0x%02X ", mac[idx]);

        printf("\n");
    }

    ////////////////////////////////////////////////////////////////////////////////////

    ap = MAC_ISO16609 | MAC_PAD3;
    printf("MAC_ALGx + MAC_PADx = 0x%02X\n", ap);
    index = 1;

    memset(mac, 0x00, sizeof(mac));
    result = api_ped_GenMAC_MSKEY(ap, index, iv, sizeof(testData), testData, mac);

    if(result == apiOK)
    {
        printf("Generate MAC by using Master/Session key algorithm : ");
        for(idx = 0 ; idx < sizeof(mac) ; idx++)
            printf("0x%02X ", mac[idx]);

        printf("\n");
    }

    ////////////////////////////////////////////////////////////////////////////////////

    ap = MAC_ALG1 | MAC_PAD1;
    printf("MAC_ALGx + MAC_PADx = 0x%02X\n", ap);
    index = 1;

    memset(mac, 0x00, sizeof(mac));
    result = api_ped_GenMAC_MSKEY(ap, index, iv, sizeof(testData), testData, mac);

    if(result == apiOK)
    {
        printf("Generate MAC by using Master/Session key algorithm : ");
        for(idx = 0 ; idx < sizeof(mac) ; idx++)
            printf("0x%02X ", mac[idx]);

        printf("\n");
    }

    ////////////////////////////////////////////////////////////////////////////////////

    ap = MAC_ALG1 | MAC_PAD2;
    printf("MAC_ALGx + MAC_PADx = 0x%02X\n", ap);
    index = 1;

    memset(mac, 0x00, sizeof(mac));
    result = api_ped_GenMAC_MSKEY(ap, index, iv, sizeof(testData), testData, mac);

    if(result == apiOK)
    {
        printf("Generate MAC by using Master/Session key algorithm : ");
        for(idx = 0 ; idx < sizeof(mac) ; idx++)
            printf("0x%02X ", mac[idx]);

        printf("\n");
    }

    ////////////////////////////////////////////////////////////////////////////////////

    ap = MAC_ALG1 | MAC_PAD3;
    printf("MAC_ALGx + MAC_PADx = 0x%02X\n", ap);
    index = 1;

    memset(mac, 0x00, sizeof(mac));
    result = api_ped_GenMAC_MSKEY(ap, index, iv, sizeof(testData), testData, mac);

    if(result == apiOK)
    {
        printf("Generate MAC by using Master/Session key algorithm : ");
        for(idx = 0 ; idx < sizeof(mac) ; idx++)
            printf("0x%02X ", mac[idx]);

        printf("\n");
    }

    ////////////////////////////////////////////////////////////////////////////////////

    ap = MAC_ALG3 | MAC_PAD1;
    printf("MAC_ALGx + MAC_PADx = 0x%02X\n", ap);
    index = 1;

    memset(mac, 0x00, sizeof(mac));
    result = api_ped_GenMAC_MSKEY(ap, index, iv, sizeof(testData), testData, mac);

    if(result == apiOK)
    {
        printf("Generate MAC by using Master/Session key algorithm : ");
        for(idx = 0 ; idx < sizeof(mac) ; idx++)
            printf("0x%02X ", mac[idx]);

        printf("\n");
    }

    ////////////////////////////////////////////////////////////////////////////////////

    ap = MAC_ALG3 | MAC_PAD2;
    printf("MAC_ALGx + MAC_PADx = 0x%02X\n", ap);
    index = 1;

    memset(mac, 0x00, sizeof(mac));
    result = api_ped_GenMAC_MSKEY(ap, index, iv, sizeof(testData), testData, mac);

    if(result == apiOK)
    {
        printf("Generate MAC by using Master/Session key algorithm : ");
        for(idx = 0 ; idx < sizeof(mac) ; idx++)
            printf("0x%02X ", mac[idx]);

        printf("\n");
    }

    ////////////////////////////////////////////////////////////////////////////////////

    ap = MAC_ALG3 | MAC_PAD3;
    printf("MAC_ALGx + MAC_PADx = 0x%02X\n", ap);
    index = 1;

    memset(mac, 0x00, sizeof(mac));
    result = api_ped_GenMAC_MSKEY(ap, index, iv, sizeof(testData), testData, mac);

    if(result == apiOK)
    {
        printf("Generate MAC by using Master/Session key algorithm : ");
        for(idx = 0 ; idx < sizeof(mac) ; idx++)
            printf("0x%02X ", mac[idx]);

        printf("\n");
    }

    ////////////////////////////////////////////////////////////////////////////////////

    ap = MAC_ALG5 | MAC_PAD4;
    printf("MAC_ALGx + MAC_PADx = 0x%02X\n", ap);
    index = 1;

    memset(mac, 0x00, sizeof(mac));
    result = api_ped_GenMAC_MSKEY(ap, index, iv, sizeof(testData), testData, mac);

    if(result == apiOK)
    {
        printf("Generate MAC by using Master/Session key algorithm : ");
        for(idx = 0 ; idx < sizeof(mac) ; idx++)
            printf("0x%02X ", mac[idx]);

        printf("\n");
    }

    printf("----------------------------------------------------------------------------------\n");

    ap = MAC_ISO16609 | MAC_PAD1;
    printf("MAC_ALGx + MAC_PADx = 0x%02X\n", ap);
    index = 1;

    memset(mac, 0x00, sizeof(mac));
    mode = (g_aes_dukpt_working_key_type << 8) | ap;
    result = api_ped_GenMAC_AES_DUKPT(mode, iv, sizeof(testData), testData, mac, ksn);
    // result = api_ped_GenMAC_DUKPT(ap, iv, sizeof(testData), testData, mac, ksn);

    if(result == apiOK)
    {
        printf("Generate MAC by using DUKPT algorithm : ");
        if(g_aes_dukpt_working_key_type > 1)    // AES key
        {
            for(idx = 0 ; idx < 16 ; idx++)
                printf("0x%02X ", mac[idx]);
        }
        else
        {
            for(idx = 0 ; idx < 8 ; idx++)
                printf("0x%02X ", mac[idx]);
        }

        printf("\n");
    }

    ////////////////////////////////////////////////////////////////////////////////////

    ap = MAC_ISO16609 | MAC_PAD2;
    printf("MAC_ALGx + MAC_PADx = 0x%02X\n", ap);
    index = 1;

    memset(mac, 0x00, sizeof(mac));
    mode = (g_aes_dukpt_working_key_type << 8) | ap;
    result = api_ped_GenMAC_AES_DUKPT(mode, iv, sizeof(testData), testData, mac, ksn);
    // result = api_ped_GenMAC_DUKPT(ap, iv, sizeof(testData), testData, mac, ksn);

    if(result == apiOK)
    {
        printf("Generate MAC by using DUKPT algorithm : ");
        if(g_aes_dukpt_working_key_type > 1)    // AES key
        {
            for(idx = 0 ; idx < 16 ; idx++)
                printf("0x%02X ", mac[idx]);
        }
        else
        {
            for(idx = 0 ; idx < 8 ; idx++)
                printf("0x%02X ", mac[idx]);
        }

        printf("\n");
    }

    ////////////////////////////////////////////////////////////////////////////////////

    ap = MAC_ISO16609 | MAC_PAD3;
    printf("MAC_ALGx + MAC_PADx = 0x%02X\n", ap);
    index = 1;

    memset(mac, 0x00, sizeof(mac));
    mode = (g_aes_dukpt_working_key_type << 8) | ap;
    result = api_ped_GenMAC_AES_DUKPT(mode, iv, sizeof(testData), testData, mac, ksn);
    // result = api_ped_GenMAC_DUKPT(ap, iv, sizeof(testData), testData, mac, ksn);

    if(result == apiOK)
    {
        printf("Generate MAC by using DUKPT algorithm : ");
        if(g_aes_dukpt_working_key_type > 1)    // AES key
        {
            for(idx = 0 ; idx < 16 ; idx++)
                printf("0x%02X ", mac[idx]);
        }
        else
        {
            for(idx = 0 ; idx < 8 ; idx++)
                printf("0x%02X ", mac[idx]);
        }

        printf("\n");
    }

    ////////////////////////////////////////////////////////////////////////////////////

    ap = MAC_ALG1 | MAC_PAD1;
    printf("MAC_ALGx + MAC_PADx = 0x%02X\n", ap);
    index = 1;

    memset(mac, 0x00, sizeof(mac));
    mode = (g_aes_dukpt_working_key_type << 8) | ap;
    result = api_ped_GenMAC_AES_DUKPT(mode, iv, sizeof(testData), testData, mac, ksn);
    // result = api_ped_GenMAC_DUKPT(ap, iv, sizeof(testData), testData, mac, ksn);

    if(result == apiOK)
    {
        printf("Generate MAC by using DUKPT algorithm : ");
        if(g_aes_dukpt_working_key_type > 1)    // AES key
        {
            for(idx = 0 ; idx < 16 ; idx++)
                printf("0x%02X ", mac[idx]);
        }
        else
        {
            for(idx = 0 ; idx < 8 ; idx++)
                printf("0x%02X ", mac[idx]);
        }

        printf("\n");
    }

    ////////////////////////////////////////////////////////////////////////////////////

    ap = MAC_ALG1 | MAC_PAD2;
    printf("MAC_ALGx + MAC_PADx = 0x%02X\n", ap);
    index = 1;

    memset(mac, 0x00, sizeof(mac));
    mode = (g_aes_dukpt_working_key_type << 8) | ap;
    result = api_ped_GenMAC_AES_DUKPT(mode, iv, sizeof(testData), testData, mac, ksn);
    // result = api_ped_GenMAC_DUKPT(ap, iv, sizeof(testData), testData, mac, ksn);

    if(result == apiOK)
    {
        printf("Generate MAC by using DUKPT algorithm : ");
        if(g_aes_dukpt_working_key_type > 1)    // AES key
        {
            for(idx = 0 ; idx < 16 ; idx++)
                printf("0x%02X ", mac[idx]);
        }
        else
        {
            for(idx = 0 ; idx < 8 ; idx++)
                printf("0x%02X ", mac[idx]);
        }

        printf("\n");
    }

    ////////////////////////////////////////////////////////////////////////////////////

    ap = MAC_ALG1 | MAC_PAD3;
    printf("MAC_ALGx + MAC_PADx = 0x%02X\n", ap);
    index = 1;

    memset(mac, 0x00, sizeof(mac));
    mode = (g_aes_dukpt_working_key_type << 8) | ap;
    result = api_ped_GenMAC_AES_DUKPT(mode, iv, sizeof(testData), testData, mac, ksn);
    // result = api_ped_GenMAC_DUKPT(ap, iv, sizeof(testData), testData, mac, ksn);

    if(result == apiOK)
    {
        printf("Generate MAC by using DUKPT algorithm : ");
        if(g_aes_dukpt_working_key_type > 1)    // AES key
        {
            for(idx = 0 ; idx < 16 ; idx++)
                printf("0x%02X ", mac[idx]);
        }
        else
        {
            for(idx = 0 ; idx < 8 ; idx++)
                printf("0x%02X ", mac[idx]);
        }

        printf("\n");
    }

    ////////////////////////////////////////////////////////////////////////////////////

    ap = MAC_ALG3 | MAC_PAD1;
    printf("MAC_ALGx + MAC_PADx = 0x%02X\n", ap);
    index = 1;

    memset(mac, 0x00, sizeof(mac));
    mode = (g_aes_dukpt_working_key_type << 8) | ap;
    result = api_ped_GenMAC_AES_DUKPT(mode, iv, sizeof(testData), testData, mac, ksn);
    // result = api_ped_GenMAC_DUKPT(ap, iv, sizeof(testData), testData, mac, ksn);

    if(result == apiOK)
    {
        printf("Generate MAC by using DUKPT algorithm : ");
        if(g_aes_dukpt_working_key_type > 1)    // AES key
        {
            for(idx = 0 ; idx < 16 ; idx++)
                printf("0x%02X ", mac[idx]);
        }
        else
        {
            for(idx = 0 ; idx < 8 ; idx++)
                printf("0x%02X ", mac[idx]);
        }

        printf("\n");
    }

    ////////////////////////////////////////////////////////////////////////////////////

    ap = MAC_ALG3 | MAC_PAD2;
    printf("MAC_ALGx + MAC_PADx = 0x%02X\n", ap);
    index = 1;

    memset(mac, 0x00, sizeof(mac));
    mode = (g_aes_dukpt_working_key_type << 8) | ap;
    result = api_ped_GenMAC_AES_DUKPT(mode, iv, sizeof(testData), testData, mac, ksn);
    // result = api_ped_GenMAC_DUKPT(ap, iv, sizeof(testData), testData, mac, ksn);

    if(result == apiOK)
    {
        printf("Generate MAC by using DUKPT algorithm : ");
        if(g_aes_dukpt_working_key_type > 1)    // AES key
        {
            for(idx = 0 ; idx < 16 ; idx++)
                printf("0x%02X ", mac[idx]);
        }
        else
        {
            for(idx = 0 ; idx < 8 ; idx++)
                printf("0x%02X ", mac[idx]);
        }

        printf("\n");
    }

    ////////////////////////////////////////////////////////////////////////////////////

    ap = MAC_ALG3 | MAC_PAD3;
    printf("MAC_ALGx + MAC_PADx = 0x%02X\n", ap);
    index = 1;

    memset(mac, 0x00, sizeof(mac));
    mode = (g_aes_dukpt_working_key_type << 8) | ap;
    result = api_ped_GenMAC_AES_DUKPT(mode, iv, sizeof(testData), testData, mac, ksn);
    // result = api_ped_GenMAC_DUKPT(ap, iv, sizeof(testData), testData, mac, ksn);

    if(result == apiOK)
    {
        printf("Generate MAC by using DUKPT algorithm : ");
        if(g_aes_dukpt_working_key_type > 1)    // AES key
        {
            for(idx = 0 ; idx < 16 ; idx++)
                printf("0x%02X ", mac[idx]);
        }
        else
        {
            for(idx = 0 ; idx < 8 ; idx++)
                printf("0x%02X ", mac[idx]);
        }

        printf("\n");
    }

    ////////////////////////////////////////////////////////////////////////////////////

    ap = MAC_ALG5 | MAC_PAD4;
    printf("MAC_ALGx + MAC_PADx = 0x%02X\n", ap);
    index = 1;

    memset(mac, 0x00, sizeof(mac));
    mode = (g_aes_dukpt_working_key_type << 8) | ap;
    result = api_ped_GenMAC_AES_DUKPT(mode, iv, sizeof(testData), testData, mac, ksn);

    if(result == apiOK)
    {
        printf("Generate MAC by using DUKPT algorithm : ");
        if(g_aes_dukpt_working_key_type > 1)    // AES key
        {
            for(idx = 0 ; idx < 16 ; idx++)
                printf("0x%02X ", mac[idx]);
        }
        else
        {
            for(idx = 0 ; idx < 8 ; idx++)
                printf("0x%02X ", mac[idx]);
        }

        printf("\n");
    }
}

UCHAR    TEST_SRAM(void)
{
    UCHAR   testData[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    UCHAR   buf[32];
    UCHAR   iLen = 8;
    UCHAR   oLen = 0;


    apk_WriteRamDataICC(ADDR_ICC_AP_PAN, 2, (UCHAR *)&iLen);
    apk_WriteRamDataICC(ADDR_ICC_AP_PAN + 2, iLen, testData);

    apk_ReadRamDataICC(ADDR_ICC_AP_PAN, 2, (UCHAR *)&oLen);
    apk_ReadRamDataICC(ADDR_ICC_AP_PAN + 2, iLen, buf);

    printf("length = %d\n", oLen);
    for(int i = 0 ; i < oLen ; i++)
        printf("%d ", buf[i]);
    printf("\n");
}

// --------------------------------------------------------------------------
//     Main Program
// --------------------------------------------------------------------------
void	APP_main( void )
{
UCHAR	result;
UCHAR	occurrence;


//    api_sram_PageInit();		// format SRAM to page structure at first time system boot, some condition shall be checked 
// OS_EnableTimer1();
      g_test_flag = 0;

      g_dhn_kbd = 0;
      g_dhn_buz_1s = 0;
      g_dhn_buz_1l = 0;
      g_dhn_icc = 0;
      g_dhn_sam = 0;
      g_dhn_aux = 0;
      g_dhn_pinpad = 0;
      g_dhn_msr = 0;
printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
      g_dhn_buz_1s = UI_OpenBuzzer1S();
      printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
      g_dhn_buz_1l = UI_OpenBuzzer1L();
      api_lcdtft_open( 0 );
      printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
      TEST_init_term_para();
      printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
      // TL_OpenAUX();
      // OP_SendFinancialTxRequest( 0, 128 );
      // TL_CloseAUX();
      // ---------------------------
      //     EMV D/C Application
      // ---------------------------      
      UI_ClearScreen();
      printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
      EMVDC_RetrievePublicKeyCA();
      printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
      CTLS_InitReader();
printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);

#if 0
      //SSL test for PCI open protocol
      UTIL_OpenBuzzer_1S();
      UTIL_OpenKey_ALL();
      TEST_SSL();
    //   TEST_TCP();
#endif

      while(1)
           {
           g_dhn_icc = api_emv_OpenSession( ICC1 ); // target ICC  
#ifdef	USE_RSA_SAM
           g_dhn_sam = api_emv_OpenSession( RSA_SAM ); // RSA SAM
#endif
             
           do{
             UI_OpenKeyAll();

             UI_ClearScreen();
             printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
             DISP_main_menu();
            printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
             result = UI_WaitKey();
             switch( result )
                   {
                   case '1':	// Upload Last BDC?
                   	FUNC_UploadLastBDC();
                   	break;

#if 1                   	
                   case '2':	// Contactless Reader?
                  	// PCI_CTLS_Transaction();
			// DTR_B17_test();
                    TD2_buf_overflow_test();
                   	break;
#endif
                   	
                   case '3':	// FUNC key pressed?
                   	FUNC_entry();
                   	break;
                   	
		   case '4':	// exhaustive PIN test
               	        TB8_pin_exhaust_test();
               	        break;
               
#if 0   //used for test
                   case '4':
                    TEST_GenMAC();
                    break;

                   case '5':
                    TEST_SRAM();
                    break;
#endif
                   }
 
             } while( result != KEY_OK ); // OK key pressed?
           
           
           // --- ICC Level 1 Startup ---
#ifndef L2_SW_DEBUG

           result = EMVDC_startup( g_temp );
ICC_STARTUP:

	   if( result == apiNotReady )	// for PCI MSR Online Transaction Only
	     {	     
         printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
	     EMVDC_close_session(254);
       printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
	     PCI_MsrOnlineTransaction();
       printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
	     EMVDC_close_transaction();
	     
	     continue;
	     }

           if( result == emvOutOfService )
             {
             DISP_out_of_service();
             EMVDC_close_session(0);
             continue;
             }
           if( result == emvFailed )
             {
             DISP_STD_card_error();
             EMVDC_close_session(0);

             if( EMVDC_FallBackTransaction() == emvOK )
               EMVDC_close_transaction();
             continue;
             }
             
           if( result == emvAborted )
             {
             EMVDC_close_session(255);
             continue;
             }

#endif
           occurrence = 0;

           // --- Select Application ---
SELECT_APP:
           UI_OpenKeyAll();
printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
           result = EMVDC_select_application( occurrence, &g_candidate_list_index );
printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
           if( result == emvNotSupported ) // PATCH: PBOC2.0, 2006-02-13, 2CM.019, fallback after application selection
             {
             result = emvFailed;
             goto ICC_STARTUP;
             }

           if( result == emvNotReady )
             {
             // show "TRY AGAIN" for 1.5 second
             UI_BeepLong();
             UI_ClearScreen();
             DISP_STD_try_again();
             TL_WaitTimeAndKey( 150 );

             occurrence = 1;
             goto SELECT_APP; // select another application
             }

           if( result == emvFailed )
             {printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
             DISP_STD_not_accepted();
             EMVDC_close_session(0);
             continue;
             }
           else // application selected OK
             {
             // g_candidate_list_index: the selected index to ADDR_CANDIDATE
printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
             // --- Initiate Application ---
	     result = EMVDC_initiate_application();
	     printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
INIT_APP_RESULT:
             switch( result )
                   {
                   case emvFailed:  // init failed
                   case emvAborted: // aborted by attendant

                        UI_ClearScreen();
                        DISP_STD_not_accepted();
                        EMVDC_close_session(0);
                        continue;

                   case emvNotReady: // not applied

                        // show "TRY AGAIN" for 1.5 second
                        UI_BeepLong();
                        UI_ClearScreen();

                        DISP_STD_not_accepted();
                        TL_WaitTimeAndKey( 150 );

                        occurrence = 1;
                        goto SELECT_APP; // select another application
                   }
             }
printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
           // --- Read Application Data ---
           if( EMVDC_read_application_data() != emvOK )
             {
             DISP_STD_not_accepted();
             EMVDC_close_session(0);
             continue;
             }
printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
           // --- Data Authentication ---
           if( EMVDC_offline_data_authen() == emvOutOfService )
             {
             DISP_STD_not_accepted();
             EMVDC_close_session(0);
             continue;
             }
printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
           // --- Processing Restriction ---
           EMVDC_processing_restrictions();
printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
           // --- Cardholder Verification ---
           if( EMVDC_cardholder_verification() == emvOutOfService )
             {
             UI_ClearScreen();
             DISP_STD_not_accepted();
             EMVDC_close_session(0);
             continue;
             }
#if 1
           else
            {
                UI_ClearScreen();
                EMVDC_close_session(255);
            }
#else
printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
           // --- Terminal Risk Management ---
           if( EMVDC_terminal_risk_management() == emvOutOfService ) // PATCH: 2006-10-02
             {
             DISP_STD_not_accepted();
             EMVDC_close_session(0);
             }
printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
           // --- Terminal Action Analysis ---
           if( EMVDC_terminal_action_analysis() != emvOK )
             {
             DISP_STD_not_accepted();
             EMVDC_close_session(0);
             }
           else
             EMVDC_close_session(1);
#endif
printf("@@@@@@@@@@@@%s %d\n",__func__,__LINE__);
           // --- Complete & Close Transaction ---
           EMVDC_close_transaction();

           } // main while(1)

}
