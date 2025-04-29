//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : EMV L2                                                     **
//**  PRODUCT  : AS330	                                                    **
//**                                                                        **
//**  FILE     : EMVDC_02.C                                                 **
//**  MODULE   : EMVDC_select_application()                                 **
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

#include "GDATAEX.h"
#include "EMVAPI.h"
#include "EMVDCMSG.h"
#include "TOOLS.h"

// ---------------------------------------------------------------------------
// FUNCTION: (1) Create candidate list.
//           (2) Selection the application to be run from the candidate list.
//               (Final selection)
// INPUT   : occurrence - 0x00 = the 1'st time to select.
//                        0x01 = the following time to select.
// OUTPUT  : candid - the candidate id, index to ADDR_CANDIDATE table.
// OUTPUT  : fci    - 2L-V, the file control information.
// RETURN  : emvOK
//           emvNotReady (the application cannot be applied) --> try next one.
//           emvFailed
// ---------------------------------------------------------------------------
UCHAR EMVDC_select_application( UCHAR occurrence, UCHAR *candid )
{
UCHAR result;
UCHAR result2;
UCHAR list_cnt;
UCHAR list[MAX_CANDIDATE_NAME_CNT][CANDIDATE_NAME_LEN];
UCHAR buffer[6];

      // if 1'st time, to create the candidate list
      if( occurrence == 0x00 )
        {
        // create the candidate list
        result = api_emv_CreateCandidateList(); 
        if( result == emvFailed )
          return( result );
        if( result == emvNotReady ) // no mutually supported applications
          {
          if( g_fallback == TRUE )     // PATCH: PBOC2.0, 2006-02-21, modify 2006-02-13 fallback condition
            return( emvNotSupported ); //
          else
            return( emvFailed );
          }

        if( result == emvNotSupported ) // PATCH: PBOC2.0, 2006-02-13, to be falled back to msr process
          return( result );
        }

      // get the candidate list
      //
      // format: LINK[1] LEN1[1] NAME1[16]
      //         LINK[1] LEN2[1] NAME2[16]...
      //         LINK[1] LEN16[1] NAME16[16]

      while(1)
           {
           list_cnt = api_emv_GetCandidateList( &list[0][0] );
           if( list_cnt == 0 )
             return( emvFailed ); // no any application supported

           // special check for auto-selecting application
           if( api_emv_AutoSelectApplication( occurrence, list_cnt, &result ) == TRUE )
             goto SEL_APP;

//         TL_DispHexByte(0,0,list_cnt);
//         UI_WaitKey();
//         TL_DumpHexData(0,0,18, &list[0][0]);
//         TL_DumpHexData(0,0,18, &list[1][0]);
//         for(;;);

           // show the candidate list to attendant
           UI_ClearScreen();
           DISP_select_app();

           // wait for attendant's selection for 30 seconds
           buffer[0] = 2;
           buffer[1] = 4;
           buffer[2] = list_cnt;
           buffer[3] = CANDIDATE_NAME_LEN;
           buffer[4] = 1;
           buffer[5] = FONT0;
           result = TL_ListBox( 0, &buffer[0], &list[0][0], 00 ); // always wait

SEL_APP:
           g_candidate_name_index = result; // backup the selected item# for later use

           if( result == 255 )
             return( emvFailed ); // timeout or aborted

           // final selection
           result2 = api_emv_FinalSelection( &list[result][0] );

           if( result2 == emvOutOfService )
             return( emvFailed );

           if( result2 == emvFailed )
             {
             if( list_cnt <= 1 )
               return( emvFailed );
             else
               {
               // show "TRY AGAIN" for 1 second
        //     UI_BeepLong();
        //     UI_ClearScreen();
        //     DISP_STD_try_again();
        //     TL_WaitTimeAndKey( 100 );
        //
        //     continue;

               return( emvNotReady );
               }
             }
           else // target application is confirmed OK
             {
             candid = (UCHAR *)&list[result][0];

             PBOC_SetupCardParameters(); // setup card related parameters

             return( emvOK );
             }
           } // while(1)

}
