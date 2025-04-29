//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : 							    **
//**  PRODUCT  : AS330							    **
//**                                                                        **
//**  FILE     : API_PED.C                                                  **
//**  MODULE   : api_ped_xxx()				                    **
//**									    **
//**  FUNCTION : API::PED (Pinpad Entry Device)			    	    **
//**  VERSION  : V1.00                                                      **
//**  DATE     : 2018/07/04                                                 **
//**  EDITOR   : James Hsieh                                                **
//**                                                                        **
//**  Copyright(C) 2021 SymLink Corporation. All rights reserved.           **
//**                                                                        **
//****************************************************************************
//============================================================================
//
//----------------------------------------------------------------------------
#include <string.h>

#include "POSAPI.h"
#include "DEV_PED.h"
#include "PEDAPI.h"
#include "LCDTFTAPI.h"
#include "OS_PROCS.h"
#include "IPC_client.h"
// #include "MPU.h"


// ---------------------------------------------------------------------------
// FUNCTION: To request PIN entry from PIN pad device.
// INPUT   : none.
// OUTPUT  : amt   - amount to be confirmed on display. (external)
//                   format: LEN(1) + ASCII(n)
//                           LEN = 0: no confirmation.
//	         tout  - PIN entry timeout in seconds.
// RETURN  : apiOK           (key-in digit available)
//           apiFailed       (timeout or invalid request)
//	         apiNotReady     (bypass PIN)
//           apiOutOfService (aborted by cancel key)
// ---------------------------------------------------------------------------
UCHAR	api_ped_GetPin( UINT tout, UCHAR *amt )
{
#if 0
UCHAR	result;


	MPU_SwitchToSuperMode( SVCID );
	
	result = PED_GetPin( tout, amt );
	
	MPU_SwitchToUserMode();

	return( result );
#else
    UCHAR   retval;
    UCHAR   args[2 + 20];


    args[0] = tout;
    memmove(&args[1], amt, amt[0] + 1);
    IPC_clientHandler(psDEV_PED, 2, 2, 22, args, &retval);

    return retval;
#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: To retrieve the current key operation mode.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : key mode. (PED_KEY_MODE_xxx)
// ---------------------------------------------------------------------------
UCHAR	api_ped_GetKeyMode( void )
{
#if 0
UCHAR	result;


	MPU_SwitchToSuperMode( SVCID );

	result = PED_ReadKeyMode();
	
	MPU_SwitchToUserMode();

	return( result );
#else
    UCHAR   retval;
    UCHAR   *args;


	IPC_clientHandler(psDEV_PED, 1, 0, 0, args, &retval);

    return retval;
#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: To read the CAPK header by key slot index.
// INPUT   : index - key slot index. (00..nn)
// OUTPUT  : pkh   - CA public key header data as follows. (fixed 29 bytes)
//                   RID		5  bytes
//		     KEY INDEX		1  byte
//		     EXP LENGTH		1  byte
//		     MOD LENGTH		2  bytes
//		     HASH		20 bytes
// RETURN  : apkOK
//           apkFailed
// ---------------------------------------------------------------------------
UCHAR	api_ped_GetKeyHeader_CAPK( UCHAR index, UCHAR *pkh )
{
#if 0
UCHAR	result;


	MPU_SwitchToSuperMode( SVCID );

	result = PED_CAPK_GetKeyHeader( index, pkh );

	MPU_SwitchToUserMode();
	
	return( result );
#else
    UCHAR   retval;
    UCHAR   args[1 + 63];


    args[0] = index;
	IPC_clientHandler(psDEV_PED, 8, 2, 1, args, &retval);
    memmove(pkh, &args[1], 63);

    return retval;
#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: To select CAPK by the specified public key INDEX & RID.
// INPUT   : pki   - CA public key index.
//           rid   - registered application provider id. (fixed 5 bytes)
// OUTPUT  : pkh   - CA public key header data as follows. (fixed 29 bytes)
//                   RID		5  bytes
//		     KEY INDEX		1  byte
//		     EXP LENGTH		1  byte
//		     MOD LENGTH		2  bytes
//		     HASH		20 bytes
//	     index - the selected key slot index.
// RETURN  : apkOK
//           apkFailed
// ---------------------------------------------------------------------------
UCHAR	api_ped_SelectKey_CAPK( UCHAR pki, UCHAR *rid, UCHAR *pkh, UCHAR *index )
{
#if 0
UCHAR	result;


	MPU_SwitchToSuperMode( SVCID );

	result = PED_CAPK_SelectKey( pki, rid, pkh, index );

	MPU_SwitchToUserMode();

	return( result );
#else
    UCHAR   retval;
    UCHAR   args[1 + 5 + 29 + 4];


    args[0] = pki;
    memmove(&args[1], rid, 5);
	IPC_clientHandler(psDEV_PED, 9, 4, 6, args, &retval);
    memmove(pkh, &args[1 + 5], 29);
    memmove(index, &args[1 + 5 + 29], 4);

    return retval;
#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: To generate MAC by using Master/Session key algorithm.
// INPUT   : sbuf   - refer to API_GENMAC structure.
//		mode   - algorithm and padding method.
//		      MAC_ALGx + MAC_PADx
//		      Padding Method 1: postfix right-padded with '0' bits.
//		      Padding Method 2: postfix right-padded with a single '1' bit and
//					postfix right-padded with '0' bits.
//		      Padding Method 3: prefix left-padded with a block with binary length data and
//					postfix right-padded with '0' bits.
// 	     	index  - session key slot index.
//	     	icv    - initial chain vector.
//	     	length - length of data.
//	     data   - data to be processed.
// OUTPUT  : mac    - MAC (8 bytes).
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
/*
UCHAR	api_ped_GenMAC_MSKEY( UCHAR mode, UCHAR index, UCHAR *icv, UINT length, UCHAR *data, UCHAR *mac )
{
#if 0	
UCHAR	result;


	MPU_SwitchToSuperMode( SVCID );

	result = PED_MSKEY_GenMAC( mode, index, icv, length, data, mac );

	MPU_SwitchToUserMode();

	return( result );
#else
    UCHAR   retval;
    UCHAR   args[1 + 1 + 8 + 2 + length + 8];


    args[0] = mode;
    args[1] = index;
    memmove(&args[2], icv, 8);
    memmove(&args[10], &length, 2);
    memmove(&args[12], data, length);
    IPC_clientHandler(psDEV_PED, 6, 6, 12 + length, args, &retval);
    memmove(mac, &args[12 + length], 8);

    return retval;
#endif
}
*/

// ---------------------------------------------------------------------------
// FUNCTION: To generate MAC by using Master/Session key algorithm.
// INPUT   : sbuf   - refer to API_GENMAC structure.
//		     mode   - bit0~7: algorithm and padding method.
//		                      MAC_ALGx + MAC_PADx
//		              Padding Method 1: postfix right-padded with '0' bits.
//		              Padding Method 2: postfix right-padded with a single '1' bit and
//					                    postfix right-padded with '0' bits.
//		              Padding Method 3: prefix left-padded with a block with binary length data and
//					                    postfix right-padded with '0' bits.
//		              bit8~15: working key type
//			                   0: _2TDEA_
//			                   1: _3TDEA_
//			                   2: _AES128_
//			                   3: _AES192_
//			                   4: _AES256_
// 	     	index  - session key slot index.
//	     	icv    - initial chain vector.
//	     	length - length of data.
//	        data   - data to be processed.
// OUTPUT  : mac    - MAC (8/16 bytes).
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_ped_GenMAC_MSKEY( UINT mode, UCHAR index, UCHAR *icv, UINT length, UCHAR *data, UCHAR *mac )
{
#if 0	
UCHAR	result;


	MPU_SwitchToSuperMode( SVCID );

	result = PED_MSKEY_GenMAC( mode, index, icv, length, data, mac );

	MPU_SwitchToUserMode();

	return( result );
#else
    UCHAR   retval;
    UCHAR   args[2 + 1 + 8 + 2 + length + 16];


    memmove(args, &mode, 2);
    args[2] = index;
    memmove(&args[3], icv, 8);
    memmove(&args[11], &length, 2);
    memmove(&args[13], data, length);
    IPC_clientHandler(psDEV_PED, 6, 6, 13 + length, args, &retval);
    memmove(mac, &args[13 + length], 16);

    return retval;
#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: To generate MAC by using Fixed Key algorithm.
// INPUT   : sbuf   - refer to API_GENMAC structure.
//		mode   - algorithm and padding method.
//			 MAC_ALGx + MAC_PADx
// 	     	index  - fixed key slot index.
//	     	icv    - initial chain vector.
//	     	length - length of data.
//	     data   - data to be processed.
// OUTPUT  : mac    - MAC (8 bytes).
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
#if	0
UCHAR	api_ped_GenMAC_FXKEY( UCHAR mode, UCHAR index, UCHAR *icv, UINT length, UCHAR *data, UCHAR *mac )
{	
UCHAR	result;


	MPU_SwitchToSuperMode( SVCID );

	result = PED_FXKEY_GenMAC( mode, index, icv, length, data, mac );

	MPU_SwitchToUserMode();

	return( result );
}
#endif

// ---------------------------------------------------------------------------
// FUNCTION: To generate MAC by using DUKPT algorithm.
// INPUT   : sbuf   - refer to API_GENMAC structure.
//		mode   - algorithm and padding method.
//		         MAC_ALGx + MAC_PADx
//	     	icv    - initial chain vector.
//	     	length - length of data.
//	     data   - data to be processed.
// OUTPUT  : mac    - MAC (8 bytes).
//	     ksn    - key serial number. (10 bytes)
// RETURN  : apiOK
//           apiFailed
//           apiDeviceError
// ---------------------------------------------------------------------------
UCHAR	api_ped_GenMAC_DUKPT( UCHAR mode, UCHAR *icv, UINT length, UCHAR *data, UCHAR *mac, UCHAR *ksn )
{
#if 0	
UCHAR	result;


	MPU_SwitchToSuperMode( SVCID );

	result = PED_DUKPT_GenMAC( mode, icv, length, data, mac, ksn );

	MPU_SwitchToUserMode();

	return( result );
#endif
    UCHAR   retval;
    UCHAR   args[1 + 8 + 2 + length + 8 + 10];


    args[0] = mode;
    memmove(&args[1], icv, 8);
    memmove(&args[9], &length, 2);
    memmove(&args[11], data, length);
    IPC_clientHandler(psDEV_PED, 7, 6, 11 + length, args, &retval);
    memmove(mac, &args[11 + length], 8);
    memmove(ksn, &args[11 + length + 8], 10);

    return retval;
}

// ---------------------------------------------------------------------------
// FUNCTION: To generate MAC by using AES DUKPT algorithm.
// INPUT   : sbuf   - refer to API_GENMAC structure.
//		     mode   - bit0~7: algorithm and padding method.
//		                      MAC_ALGx + MAC_PADx
//		              bit8~15: working key type
//			                   0: _2TDEA_
//			                   1: _3TDEA_
//			                   2: _AES128_
//			                   3: _AES192_
//			                   4: _AES256_
//	     	 icv    - initial chain vector.
//	     	 length - length of data.
//	         data   - data to be processed.
// OUTPUT  : mac    - MAC (8/16 bytes).
//	         ksn    - key serial number. (12 bytes)
// RETURN  : apiOK
//           apiFailed
//           apiDeviceError
// ---------------------------------------------------------------------------
UCHAR	api_ped_GenMAC_AES_DUKPT( UINT mode, UCHAR *icv, UINT length, UCHAR *data, UCHAR *mac, UCHAR *ksn )
{
#if 0	
UCHAR	result;


	MPU_SwitchToSuperMode( SVCID );

	result = PED_AES_DUKPT_GenMAC( mode, icv, length, data, mac, ksn );

	MPU_SwitchToUserMode();

	return( result );
#endif
    UCHAR   retval;
    UCHAR   args[2 + 8 + 2 + length + 16 + 12];


    memmove(args, &mode, 2);
    memmove(&args[2], icv, 8);
    memmove(&args[10], &length, 2);
    memmove(&args[12], data, length);
    IPC_clientHandler(psDEV_PED, 11, 6, 12 + length, args, &retval);
    memmove(mac, &args[12 + length], 16);
    memmove(ksn, &args[12 + length + 8], 12);

    return retval;
}

// ---------------------------------------------------------------------------
// FUNCTION: To generate plaintext PIN block by using ISO 9564 format2.
// INPUT   : none.
// OUTPUT  : ppb     - plaintext pin block (fixed 8 bytes).
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
// UCHAR	api_ped_GenPinBlock( UCHAR *ppb )
// {
// #if 0
// 	return( PED_GenPinBlock( ppb ) );
// #else
//     UCHAR   retval;

    
//     IPC_clientHandler(psDEV_PED, 12, 1, 0, ppb, &retval);
//     return retval;
// #endif
// }

// ---------------------------------------------------------------------------
// FUNCTION: To generate encrypted PIN block by using Master/Session key algorithm.
// INPUT   : mode   - algorithm of PIN block.
//		      0: ISO 9564 Format 0 (ANSI X9.8)
//		      1: ISO 9564 Format 1
//		      2: ISO 9564 Format 2
//		      3: ISO 9564 Format 3
// 	     index  - session key index.
//	     pan    - full PAN digits or transaction field for ISO3. (format: L-V)
// OUTPUT  : epb    - encrypted pin block (fixed 8 bytes).
//	     sn     - randomly selected sequence number used only for ISO3. (1 byte) - RFU
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_ped_GenPinBlock_MSKEY( UCHAR mode, UCHAR index, UCHAR *pan, UCHAR *epb )
{
#if 0
UCHAR	result;


	MPU_SwitchToSuperMode( SVCID );

	result = PED_MSKEY_GenPinBlock( mode, index, pan, epb );

	MPU_SwitchToUserMode();

	return( result );
#else
    UCHAR   retval;
    UCHAR   args[1 + 1 + 32 + 8];


    args[0] = mode;
    args[1] = index;
    memmove(&args[2], pan, 32);
    IPC_clientHandler(psDEV_PED, 3, 4, 34, args, &retval);
    memmove(epb, &args[34], 8);

    return retval;
#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: To generate encrypted PIN block by using Fixed key algorithm.
// INPUT   : mode   - algorithm of PIN block.
// 	     index  - fixed key index.
//	     pan    - PAN. (format: L-V)
// OUTPUT  : epb    - encrypted pin block (8 bytes).
//	     sn     - randomly selected sequence number used only for ISO3. (1 byte) - RFU
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
#if	0
UCHAR	api_ped_GenPinBlock_FXKEY( UCHAR mode, UCHAR index, UCHAR *pan, UCHAR *epb )
{
UCHAR	result;


	MPU_SwitchToSuperMode( SVCID );

	result = PED_FXKEY_GenPinBlock( mode, index, pan, epb );

	MPU_SwitchToUserMode();

	return( result );
}
#endif

// ---------------------------------------------------------------------------
// FUNCTION: To generate encrypted PIN block by using DUKPT algorithm.
// INPUT   : mode   - algorithm of PIN block. (DES or TDES)
//		      default: TDES
//	     pan    - PAN data. (L-V)
// OUTPUT  : epb    - encrypted pin block (fixed 8 bytes).
//	     ksn    - key serial number.  (fixed 10 bytes).
// RETURN  : apiOK
//           apiFailed
//           apiDeviceError
// ---------------------------------------------------------------------------
UCHAR	api_ped_GenPinBlock_DUKPT( UCHAR mode, UCHAR *pan, UCHAR *epb, UCHAR *ksn )
{
#if 0
UCHAR	result;


	MPU_SwitchToSuperMode( SVCID );

	result = PED_DUKPT_GenPinBlock( mode, pan, epb, ksn );

	MPU_SwitchToUserMode();

	return( result );
#else
    UCHAR   retval;
    UCHAR   args[1 + 32 + 8 + 10];
    

    args[0] = mode;
    memmove(&args[1], pan, 32);
    IPC_clientHandler(psDEV_PED, 4, 4, 33, args, &retval);
    memmove(epb, &args[33], 8);
    memmove(ksn, &args[41], 10);

    return retval;
#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: To generate encrypted PIN block by using AES DUKPT algorithm.
// INPUT   : mode   - algorithm of PIN block.
//		              bit1~4:
//		      	      0: ISO 9564 Format 0 (ANSI X9.8)
//		      	      1: ISO 9564 Format 1
//		      	      2: ISO 9564 Format 2 (NA)
//		      	      3: ISO 9564 Format 3
//		              bit5~8:
//			          0: _2TDEA_
//			          1: _3TDEA_
//			          2: _AES128_
//			          3: _AES192_
//			          4: _AES256_
//	         pan    - PAN data. (L-V)
// OUTPUT  : epb    - encrypted pin block (8/16 bytes).
//	         ksn    - key serial number.  (fixed 12 bytes).
// RETURN  : apiOK
//           apiFailed
//           apiDeviceError
// ---------------------------------------------------------------------------
UCHAR	api_ped_GenPinBlock_AES_DUKPT( UCHAR mode, UCHAR *pan, UCHAR *epb, UCHAR *ksn )
//UCHAR	api_ped_GenPinBlock_AES_DUKPT( UINT mode, UCHAR *pan, UCHAR *epb, UCHAR *ksn )
{
#if 0
UCHAR	result;


	MPU_SwitchToSuperMode( SVCID );

    result = PED_AES_DUKPT_GenPinBlock(mode, pan, epb, ksn);

	MPU_SwitchToUserMode();

	return( result );
#else
    UCHAR   retval;
    // UCHAR   args[2 + 32 + 16 + 12];
    UCHAR   args[1 + 32 + 16 + 12];
    
    
    // memmove(args, &mode, 2);
    // memmove(&args[2], pan, 32);
    // IPC_clientHandler(psDEV_PED, 10, 4, 34, args, &retval);
    // memmove(epb, &args[34], 16);
    // memmove(ksn, &args[50], 12);

    args[0] = mode;
    memmove(&args[1], pan, 32);
    IPC_clientHandler(psDEV_PED, 10, 4, 33, args, &retval);
    memmove(epb, &args[33], 16);
    memmove(ksn, &args[49], 12);

    return retval;
#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: To generate encrypted PIN block by using AES key algorithm.
// INPUT   : mode   - algorithm of PIN block.
//		      fixed 0x04, ISO 9564 Format 4.
// 	     index  - fixed key index (0x00).
//	     pan    - PAN. (format: L-V)
// OUTPUT  : epb    - encrypted pin block (16 bytes).
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_ped_GenPinBlock_AESKEY( UCHAR mode, UCHAR index, UCHAR *pan, UCHAR *epb )
{
#if 0
UCHAR	result;


	MPU_SwitchToSuperMode( SVCID );

	result = PED_AESKEY_GenPinBlock( mode, index, pan, epb );

	MPU_SwitchToUserMode();

	return( result );
#else
    UCHAR   retval;
    UCHAR   args[1 + 1 + 32 + 16];


    args[0] = mode;
    args[1] = index;
    memmove(&args[2], pan, 32);
    IPC_clientHandler(psDEV_PED, 5, 4, 34, args, &retval);
    memmove(epb, &args[34], 16);

    return retval;
#endif
}

// -------------------------------------------------------------------------------------------------
// FUNC  : Setup parameters for AS350 internal PIN entry function.
// INPUT : row	   - row number of display for PIN entry.
//	       col	   - beginning column number of display for PIN entry.
//	       palette - fixed 3 bytes palette values of RGB.
// OUTPUT: none.
// RETURN: none.
// NOTE  : This function is to be implemented from AP level.
// -------------------------------------------------------------------------------------------------
void	api_ped_SetupPinPad( UCHAR *sbuf )
{
#if 0
	memmove( &os_ped_EMVSignPadPara, sbuf, sizeof(API_LCDTFT_PARA) );
#else
    UCHAR   retval;
    UCHAR   args[sizeof(API_LCDTFT_PARA)];


    memmove(args, sbuf, sizeof(API_LCDTFT_PARA));
    IPC_clientHandler(psDEV_PED, 13, 1, sizeof(API_LCDTFT_PARA), args, &retval);
#endif
}

// ---------------------------------------------------------------------------
// FUNC  : set COM port for external pin pad device.
// INPUT : port
// OUTPUT: 
// RETURN: emvOK
//         emvFailed (invalid port number)
// NOTE  : This function should be called before api_emvk_CardholderVerification().
// ---------------------------------------------------------------------------
UCHAR	api_ped_SetPinPadPort( UCHAR port )
{
#if 0
UCHAR	result = apiOK;


	switch( port )	// 2016-10-11
	      {
	      case COM0:
	      	
	      	   g_dhn_ped = 0x88;
	      	   break;
	      	   
	      case COM1:
	      	
	      	   g_dhn_ped = 0x89;
	      	   break;
	      	   
	      case COM2:
	      	
	      	   g_dhn_ped = 0x8A;
	      	   break;
	      	   
	      default:	// COM0
	      	
	      	   result = apiFailed;
	      	   break;
	      }
	      
	return( result );
#else
    UCHAR   retval;


    IPC_clientHandler(psDEV_PED, 14, 1, 1, (UCHAR *)&port, &retval);
    return retval;
#endif
}
