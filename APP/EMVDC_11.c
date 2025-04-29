//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : EMV L2                                                     **
//**  PRODUCT  : AS330	                                                    **
//**                                                                        **
//**  FILE     : EMVDC_11.C                                                 **
//**  MODULE   : EMVDC_contactless()		                            **
//**  VERSION  : V1.00                                                      **
//**  DATE     : 2019/04/25                                                 **
//**  EDITOR   : James Hsieh                                                **
//**                                                                        **
//**  Copyright(C) 2019 SymLink Corporation. All rights reserved.	    **
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

#include "UTILS.h"
#include "Define.h"
#include "Function.h"
#include "DTE_Function.h"



extern	void	_CLS( void );				// clear screen and reset cursor
extern	void	_PRINTF( const char *fmt, ... );	// output message onto display as "printf()"

extern	void	DEMO_Application(void);
extern	UCHAR	DTE_Open_USB(void);

//Test
extern UCHAR	mpp_flgDBG;
extern UCHAR	mpp_dbg_dhnAUX;
extern UCHAR	dbg_flgEnable;
extern UCHAR	dbg_dhnAUX;
extern UCHAR	dte_flgTA;
extern UCHAR	dte_dhnAUX;

#define	CTLS_FAILED				0xFF
#define	CTLS_OFFLINE_APPROVED			0
#define	CTLS_ONLINE_APPROVED			1
#define	CTLS_ONLINE_PIN_REQUIRED		2
#define	CTLS_SIGNATURE_REQUIRED			3

UCHAR	CTLS_amt[8];


// ---------------------------------------------------------------------------
// FUNCTION: Seek for the beginning offset of the transaction outcome.
// INPUT   : length
//	     idata
// OUTPUT  : d1, d2, d3, d4
// RETURN  : none.
// ---------------------------------------------------------------------------
void	CTLS_SeekDataSegment( UINT length, UCHAR *idata, UINT *d1, UINT *d2, UINT *d3, UINT *d4 )
{
UCHAR	data;
UINT	iLen;
UINT	d11, d22, d33, d44;
UINT	offset = 9;

	
	*d1 = 0xFFFF;
	*d2 = 0xFFFF;
	*d3 = 0xFFFF;
	*d4 = 0xFFFF;
	
	// find D1
	if( idata[offset] == 0xD1 )
	  {
	  *d1 = offset++;
	  
	  data = idata[offset];
	  if( data & 0x80 )
	    data = idata[++offset];
	    
	  iLen = data;
	  offset += (iLen + 1);	// ptr to D2
	    
	  if( idata[offset] == 0xD2 )
	    {
	    *d2 = offset++;
	    
	    data = idata[offset];
	    if( data & 0x80 )
	      data = idata[++offset];
	      
	    iLen = data;
	    offset += (iLen + 1);	// ptr to D3
	    
	    if( idata[offset] == 0xD3 )
	      {
	      *d3 = offset++;
	      
	      data = idata[offset];
	      if( data & 0x80 )
	      	data = idata[++offset];
	      
	      iLen = data;
	      offset += (iLen + 1);	// ptr to D4
	      
	      if( idata[offset] == 0xD4 )
	      	*d4 = offset;
	      }
	    }
	  }
}

// ---------------------------------------------------------------------------
// FUNCTION: Initialize contactless reader & parameters.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
// ---------------------------------------------------------------------------
void	CTLS_InitReader( void )
{
UCHAR	bufSend[128];
UCHAR	bufRecv[128];
	
UCHAR	tstPar[]=
	{
		0x08,
//		0x9F,0x1A,0x02,0x01,0x58,
//		0x5F,0x2A,0x02,0x09,0x01,
		0x9F,0x1A,0x02,0x01,0x56,
		0x5F,0x2A,0x02,0x01,0x56,

		0x9C,0x01,0x00,
		0xDF,0x00,0x06,0x99,0x99,0x99,0x99,0x99,0x99,
		0xDF,0x01,0x06,0x00,0x00,0x00,0x10,0x00,0x00,
		0xDF,0x02,0x06,0x00,0x00,0x00,0x30,0x00,0x00,
		0xDF,0x61,0x06,0x00,0x00,0x00,0x10,0x00,0x00,
		0xDF,0x62,0x06,0x00,0x00,0x00,0x30,0x00,0x00
	};



	memset(bufSend, 0, 8);
	api_pcd_vap_sys_OpenContactlessInterface(&bufSend[0], &bufSend[2], &bufRecv[0], &bufRecv[2]);
	api_pcd_vap_sys_InitializeContactlessParameter(&bufSend[0], &bufSend[2], &bufRecv[0], &bufRecv[2]);


	bufSend[1]=1;
	bufSend[2]=0;
	api_pcd_vap_adm_SwitchToAdminMode(&bufSend[0], &bufSend[2], &bufRecv[0], &bufRecv[2]);


	bufSend[1]=69;
	memcpy(&bufSend[2], tstPar, 69);
	api_pcd_vap_adm_SetEMVTagsValues(&bufSend[0], &bufSend[2], &bufRecv[0], &bufRecv[2]);


	bufSend[1]=1;
	bufSend[2]=1;
	api_pcd_vap_adm_SwitchToAdminMode(&bufSend[0], &bufSend[2], &bufRecv[0], &bufRecv[2]);
}

// ---------------------------------------------------------------------------
// FUNCTION: Contactless Transaction.
// INPUT   : amt	V(6), in BCD.
// OUTPUT  : result	0 = offline approved
//			1 = online approved
//			2 = online PIN required
//			3 = signature required
//	     opLen
// RETURN  : pointer to CTLS response msg.
//	     NULL if error
// ---------------------------------------------------------------------------
UCHAR	bufRecv[768];
UCHAR	*EMVDC_contactless( UCHAR *amt, UCHAR *result, UINT *opLen )
{	
	UCHAR bufSend[128];
//	UCHAR bufRecv[768];
//	UCHAR *bufRecv;
	UCHAR tlv[261];
	UINT  iLen;
	UCHAR rc;
	UINT  D1, D2, D3, D4;
	UCHAR *ptr;
	UCHAR *ptrtmp;
	UCHAR reclen[2];
	UCHAR *output = 0;


//	DUMY_Open_MSR();
//	DUMY_Open_IFM();

//	bufRecv = malloc( 768 );
//	if( !bufRecv )
//	  return( 0 );

	*result = 0xFF;
	memset( bufRecv, 0x00, sizeof(bufRecv) );

//	while (1)
//	{
		memset(bufSend, 0, 8);
		bufSend[1]=6;
	//	bufSend[6]=8;
		memmove( &bufSend[2], amt, 6 );
		api_pcd_vap_tra_ReadyForSale(&bufSend[0], &bufSend[2], &bufRecv[0], &bufRecv[2]);
		
	//	LIB_WaitKey();
		
		// LEN(2) -- H-L
		// RC (1)
		// SCHEME ID (1)
		//		17
		// DATE_TIME (7)
		//		20 04 01 03 13 26 04
		// D1 (TLV)	track 1 data (if available)
		//		D1 28 42 34 39 33 38
		//		31 37 30 31 30 30 30 30
		//		31 33 31 32 5E 20 2F 5E
		//		31 39 31 32 32 30 31 36
		//		35 35 31 32 34 31 33 30
		//		30 30 30
		//		
		// D2 (TLV)	track 2 data (if available) BCD
		//		D2 10 49 38 17
		//		01 00 00 13 12 D1 91 22
		//		01 12 45 51 36
		//
		// D3 (TLV)	chip data
		//		D3 81 8F 
		//		5A 08		(PAN)
		//			49 38 17 01 00 00 13 12 
		//		5F 24 03 	(APP Expiration Date)
		//			19 12 31
		//		5F 2A 02 	(Trans Currency Code)
		//			09 01 
		//		5F 34 01	(PAN Sequnece Number)
		//			00 
		//		82 02 		(AIP)
		//			20 00 
		//		95 05 		(TVR)
		//			00 00 00 00 00 
		//		9A 03 		(Trans Date)
		//			04 01 03 
		//		9C 01 		(Trans Type)
		//			00 
		//		9F 10 07 	(Issuer App Data)
		//			06 01 11 03 90 00 00 
		//		9F 1A 02 	(Term Country Code)
		//			01 58 
		//		9F 26 08 	(AC)
		//			44 EE C3 6A B0 DE 20 8F 
		//		9F 36 02 	(ATC)
		//			05 A1
		//		9F 37 04 	(Unpredictable Number)
		//			19 5F 5B 70 
		//		9F 27 01 	(CID)
		//			40 
		//		57 10		(Track2 Equivalent Data)
		//			49 38 17 01 00 00 13 12
		//			D1 91 22 01 12 45 51 36
		//		9F 66 04 	(???)
		//			A6 00 C0 00 
		//		9F 02 06 	(Amount, Auth)
		//			00 00 00 00 08 00
		//		9F 03 06 	(Amount, Other)
		//			00 00 00 00 00 00 
		//		9F 39 01 	(PEM)
		//			07 
		//		DF 0E 01	(???)
		//			05 
		//		9F 21 03 	(Trans Time)
		//			13 36 47
		//
		// D4 (TLV)	additional data
		//		D4 0C 
		//		DF 1F 01 00 
		//		9F 39 01 07 
		//		DF 0E 01 05
		//
		// �P�_Offline approve���覡�G
		// D4�]�tTLV DF 1F 01 00
		//
		// �P�_Online approve���覡�G
		// D4�]�tTLV DF 0F 01 01
		//
		// �P�_Online PIN���覡�G
		// D4�]�tTLV 99 01 00
		//
		// �P�_Obtain signature���覡�G
		// D4�]�tTLV 55 01 00

		
		iLen = bufRecv[0]*256 + bufRecv[1];
		rc   = bufRecv[2];
//		TL_DumpHexData( 0, 0, iLen, &bufRecv[2] );

		*opLen = iLen;
		output = &bufRecv[2];
//		TL_DumpHexData( 0, 0, iLen, output );

		if( rc != 0x01 )
		  {
		  TL_DispHexByte( 0, 1, rc );
		  UI_WaitKey();
		  }

		if( rc == 0x01 )	// success?
		  {
		  CTLS_SeekDataSegment( iLen, &bufRecv[2], &D1, &D2, &D3, &D4 );
//		  LIB_DispHexWord( 0, 0, D1 );
//		  LIB_DispHexWord( 1, 0, D2 );
//		  LIB_DispHexWord( 2, 0, D3 );
//		  LIB_DispHexWord( 3, 0, D4 );

//		  LIB_DumpHexData( 0, 0, 16, &bufRecv[2+D4] );

		  iLen = bufRecv[2+D4+1];
		  reclen[0] = iLen & 0x00ff;
          	  reclen[1] = (iLen & 0xff00) >> 8;
          	  ptrtmp = &bufRecv[2+D4+2];

		  *result = 0xFF;
		  while(1)
		       {
		       ptr = apk_GetBERTLV( reclen, ptrtmp, tlv );	// tlv=T-L1-L2-V
		       if( ptr == (UCHAR *)0 )
		       	 break;
		       	 
	//	       TL_DumpHexData( 0, 0, 5, tlv );
	//	       ptrtmp = ptr;
	//	       continue;
		       	 
		       if( ptr != (UCHAR *)-1 )
		         {
		  //     LIB_DumpHexData( 0, 0, 16, tlv );
		         
		         if( (*(tlv+0) == 0xDF) && (*(tlv+1) == 0x1F) && (*(tlv+2) == 0x01) && (*(tlv+3) == 0x00) && (*(tlv+4) == 0x00) )
		           {
		           *result = CTLS_OFFLINE_APPROVED;
		           break;
		           }
		         
		         if( (*(tlv+0) == 0xDF) && (*(tlv+1) == 0x0F) && (*(tlv+2) == 0x01) && (*(tlv+3) == 0x00) && (*(tlv+4) == 0x01) )
		           {
		           *result = CTLS_ONLINE_APPROVED;
		           break;
		           }
		           
		         if( (*(tlv+0) == 0x00) && (*(tlv+1) == 0x99) && (*(tlv+2) == 0x01) && (*(tlv+3) == 0x00) && (*(tlv+4) == 0x00) )
		           {
		           *result = CTLS_ONLINE_PIN_REQUIRED;
		           break;
		           }
		           
		         if( (*(tlv+0) == 0x00) && (*(tlv+1) == 0x55) && (*(tlv+2) == 0x01) && (*(tlv+3) == 0x00) && (*(tlv+4) == 0x00) )
		           {
		           *result = CTLS_SIGNATURE_REQUIRED;
		           break;
		           }
		           
		         ptrtmp = ptr;
		         }
		       else
		         break;
		       } // while(1)
		  }
		
//	}

//	TL_DumpHexData( 0, 0, *opLen, output );
	
//	free( bufRecv );
	
	return( output );
}

// ---------------------------------------------------------------------------
// FUNCTION: set the related data elements for CTLS transaction.
// INPUT   : cLen	length of cData.
//	     cData	icc data elements. (TLV)
// RETURN  : none.
// ---------------------------------------------------------------------------
void	EMVDC_CTLS_SetupDataElements( UINT cLen, UCHAR *cData )
{
UINT	i;
UINT	D1, D2, D3, D4;
UINT	iLen;
UINT	iPanLen;
UCHAR	cnt;
UCHAR	*ptrobj;
UCHAR	*ptrtmp;
UCHAR	reclen[2];
UCHAR	pan[10];
	

//	TL_DumpHexData( 0, 0, cLen, cData );
	
	CTLS_SeekDataSegment( cLen, cData, &D1, &D2, &D3, &D4 );
	
//	apk_GetBERLEN( &cData[D3+1], &cnt )
//	ptrtmp = &cData[D3+1+cnt];	// ptr to 1'st DE

	// get PAN from D2 LN PAN.................... till Dx or xD
	//		D2 10 49 38 17 01 00 00 13 12 D1 91 22 01 12 45 51 36
	iLen = cData[D2+1];
	iPanLen = 0;
	memset( pan, 0xff, sizeof(pan) );
	for( i=0; i<iLen; i++ )
	   {
	   if( (cData[D2+2+i] & 0xD0) == 0xD0 )
	     break;
	     
	   if( (cData[D2+2+i] & 0x0D) == 0x0D )
	     {
	     pan[i] = (cData[D2+2+i] & 0xF0) | 0x0F;
	     iPanLen++;
	     break;
	     }
	     
	   pan[i] = cData[D2+2+i];
	   iPanLen++;
	   }

	api_emv_PutDataElement( DE_ICC, ADDR_ICC_AP_PAN, 2, (UCHAR *)&iPanLen );
	api_emv_PutDataElement_SRED( DE_ICC, ADDR_ICC_AP_PAN+2, iPanLen, pan );

	// find other EMV data elements
	cData[D3] = 0x70;	// 70-LL-[TLV...]
//	TL_DumpHexData( 0, 0, 32, &cData[D3] );
#if	0	
	ptrobj = apk_FindTag( 0x5A, 0x00, &cData[D3] );	// PAN
	if( ptrobj )	// L-V
	  {
	  iLen = *ptrobj;
//	  TL_DumpHexData( 0, 0, iLen+1, ptrobj );
	  api_emv_PutDataElement( DE_ICC, ADDR_ICC_AP_PAN, 2, (UCHAR *)&iLen );
	  api_emv_PutDataElement_SRED( DE_ICC, ADDR_ICC_AP_PAN+2, iLen, ptrobj+1 );
	  }
#endif

	ptrobj = apk_FindTag( 0x5F, 0x24, &cData[D3] );	// APP Expiration Date
	if( ptrobj )	// L-V
	  {
	  iLen = *ptrobj;
	  api_emv_PutDataElement( DE_ICC, ADDR_ICC_AP_EXPIRE_DATE, 2, (UCHAR *)&iLen );
	  api_emv_PutDataElement( DE_ICC, ADDR_ICC_AP_EXPIRE_DATE+2, iLen, ptrobj+1 );
	  }
	  
	ptrobj = apk_FindTag( 0x5F, 0x2A, &cData[D3] );	// Trans Currency Code
	if( ptrobj )	// L-V
	  {
	  iLen = *ptrobj;
	  api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_CC, iLen, ptrobj+1 );
	  }
	  
	ptrobj = apk_FindTag( 0x5F, 0x34, &cData[D3] );	// PAN Sequence Number
	if( ptrobj )	// L-V
	  {
	  iLen = *ptrobj;
	  api_emv_PutDataElement( DE_ICC, ADDR_ICC_AP_PAN_SN, 2, (UCHAR *)&iLen );
	  api_emv_PutDataElement( DE_ICC, ADDR_ICC_AP_PAN_SN+2, iLen, ptrobj+1 );
	  }

	ptrobj = apk_FindTag( 0x82, 0x00, &cData[D3] );	// AIP
	if( ptrobj )	// L-V
	  {
	  iLen = *ptrobj;
	  api_emv_PutDataElement( DE_ICC, ADDR_ICC_AIP2, 2, (UCHAR *)&iLen );
	  api_emv_PutDataElement( DE_ICC, ADDR_ICC_AIP2+2, iLen, ptrobj+1 );
	  
	  api_emv_PutDataElement( DE_ICC, ADDR_ICC_AIP, iLen, ptrobj+1 );
	  }
	  
	ptrobj = apk_FindTag( 0x95, 0x00, &cData[D3] );	// TVR
	if( ptrobj )	// L-V
	  {
	  iLen = *ptrobj;
	  api_emv_PutDataElement( DE_TERM, ADDR_TERM_TVR, iLen, ptrobj+1 );
	  }
	  
	ptrobj = apk_FindTag( 0x9C, 0x00, &cData[D3] );	// Trans Type
	if( ptrobj )	// L-V
	  {
	  iLen = *ptrobj;
	  api_emv_PutDataElement( DE_TERM, ADDR_TERM_TX_TYPE, iLen, ptrobj+1 );
	  }
	  
	ptrobj = apk_FindTag( 0x9F, 0x10, &cData[D3] );	// Issuer App Data
	if( ptrobj )	// L-V
	  {
	  iLen = *ptrobj;
	  api_emv_PutDataElement( DE_ICC, ADDR_ICC_ISU_AP_DATA, 2, (UCHAR *)&iLen );
	  api_emv_PutDataElement( DE_ICC, ADDR_ICC_ISU_AP_DATA+2, iLen, ptrobj+1 );
	  }
	  
	ptrobj = apk_FindTag( 0x9F, 0x1A, &cData[D3] );	// Term Country Code
	if( ptrobj )	// L-V
	  {
	  iLen = *ptrobj;
	  api_emv_PutDataElement( DE_TERM, ADDR_TERM_CNTR_CODE, iLen, ptrobj+1 );
	  }
	  
	ptrobj = apk_FindTag( 0x9F, 0x26, &cData[D3] );	// AC
	if( ptrobj )	// L-V
	  {
	  iLen = *ptrobj;
//	  TL_DumpHexData( 0, 0, iLen+1, ptrobj );
	  api_emv_PutDataElement( DE_ICC, ADDR_ICC_AC, 2, (UCHAR *)&iLen );
	  api_emv_PutDataElement( DE_ICC, ADDR_ICC_AC+2, iLen, ptrobj+1 );
	  }
	  
	ptrobj = apk_FindTag( 0x9F, 0x36, &cData[D3] );	// ATC
	if( ptrobj )	// L-V
	  {
	  iLen = *ptrobj;
	  api_emv_PutDataElement( DE_ICC, ADDR_ICC_ATC, 2, (UCHAR *)&iLen );
	  api_emv_PutDataElement( DE_ICC, ADDR_ICC_ATC+2, iLen, ptrobj+1 );
	  }
	  
	ptrobj = apk_FindTag( 0x9F, 0x37, &cData[D3] );	// Unpredictable Number
	if( ptrobj )	// L-V
	  {
	  iLen = *ptrobj;
	  api_emv_PutDataElement( DE_TERM, ADDR_TERM_UPD_NBR, iLen, ptrobj+1 );
	  }
	  
	ptrobj = apk_FindTag( 0x9F, 0x27, &cData[D3] );	// CID
	if( ptrobj )	// L-V
	  {
	  iLen = *ptrobj;
	  api_emv_PutDataElement( DE_ICC, ADDR_ICC_CID, 2, (UCHAR *)&iLen );
	  api_emv_PutDataElement( DE_ICC, ADDR_ICC_CID+2, iLen, ptrobj+1 );
	  }
	  
	ptrobj = apk_FindTag( 0x57, 0x00, &cData[D3] );	// Track2 Equivalent Data
	if( ptrobj )	// L-V
	  {
	  iLen = *ptrobj;
	  api_emv_PutDataElement( DE_ICC, ADDR_ICC_TRK2_EQUD, 2, (UCHAR *)&iLen );
	  api_emv_PutDataElement( DE_ICC, ADDR_ICC_TRK2_EQUD+2, iLen, ptrobj+1 );
	  }
	  
	ptrobj = apk_FindTag( 0x57, 0x00, &cData[D3] );	// Track2 Equivalent Data
	if( ptrobj )	// L-V
	  {
	  iLen = *ptrobj;
	  api_emv_PutDataElement( DE_ICC, ADDR_ICC_TRK2_EQUD, 2, (UCHAR *)&iLen );
	  api_emv_PutDataElement( DE_ICC, ADDR_ICC_TRK2_EQUD+2, iLen, ptrobj+1 );
	  }
}

// ---------------------------------------------------------------------------
// FUNCTION: CTLS online transaction for PCI test.
// INPUT   : PINflag =	0	not required
//			1	required
// OUTPUT  : none.
// RETURN  : emvOK
//           emvFailed
// ---------------------------------------------------------------------------
UCHAR	PCI_CtlsOnlineTransaction( UCHAR PINflag )
{
UCHAR	buf[32];
UCHAR	buf2[32];
UCHAR	arc[2];
UCHAR	iad[34];
UCHAR	epb[18];	// LL-V(8+8)
UCHAR	ksn[12];	// LL-V(10)
UINT	ist;
UINT	iLen;
UCHAR	result;
ULONG	cnt;
UCHAR	data;


	if( !PINflag )
	  goto SETUP_DT;
	  
	memmove( buf, CTLS_amt, CTLS_amt[0]+1 );
	TL_insert_decimal_point( NUM_TYPE_DEC, buf ); // NNNNN.NN

	// --- Request PIN Entry ---
	buf2[0] = buf[0] + 1;
	buf2[1] = '$';
	memmove( &buf2[2], &buf[1], buf[0] );
	
	result = PP_GetPIN( 30, buf2 );
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
	      // ----------------------------------------------------------------------------------
	      // Here shows the Encryped PIN Block (EPB) on display for PCI online testing purpose
	      // ----------------------------------------------------------------------------------
	      UI_ClearScreen();
	      
	      UI_PutStr( 0, 0, FONT0, 4, "EPB:" );
	      TL_DumpHexData( 0, 1, iLen, epb+2 );
	      }
	    
	    if( (ksn[0]+ksn[1]*256) != 0 )
	      {
	      // ----------------------------------------------------------------------------------
	      // Here shows the Key Serial Number (KSN) on display for PCI online testing purpose
	      // ----------------------------------------------------------------------------------
	      UI_PutStr( 4, 0, FONT0, 4, "KSN:" );
	      TL_DumpHexData( 0, 5, 10, ksn+2 );
	      }
	    }
	  else	// fail to generate EPB
	    {          
            UI_ClearScreen();
            DISP_STD_declined();
	    UI_WaitKey();
	    return( emvFailed );
	    }

SETUP_DT:
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

	    g_term_ARC[0] = arc[0];
	    g_term_ARC[0] = arc[1];
	    api_emv_PutDataElement( DE_TERM, ADDR_TERM_ARC, 2, g_term_ARC );
	    }
	  else // unable to go online
	    {	    
	    UI_ClearScreen();
	    DISP_STD_declined();
	    UI_WaitKey();
	    return( emvFailed );
	    }
          }
        else // fail to get PIN (timeout or invalid request)
          {          
          UI_ClearScreen();
          DISP_STD_declined();
	  UI_WaitKey();
	  return( emvFailed );
          }
          
	return( emvOK ); // done
}

// ---------------------------------------------------------------------------
// FUNCTION: Contactless Transaction.
// INPUT   : none.
// RETURN  : none.
// ---------------------------------------------------------------------------
void	PCI_CTLS_Transaction( void )
{
UCHAR	rc;		// result code
UCHAR	amt[6];
UCHAR	buf[32];
UCHAR	buf2[32];
UCHAR	*pData;
UCHAR	arc[2];
UCHAR	iad[34];
UCHAR	epb[18];	// LL-V(8+8)
UCHAR	ksn[12];	// LL-V(10)
UINT	ist;
UINT	iLen;
UCHAR	result;
ULONG	cnt;
UCHAR	data;


	// clear all ICC-Related data elements to 0
	iLen = ADDR_ICC_DE_END;
	iLen -= ADDR_ICC_DE_START;
	api_emv_ClrDataElement( DE_ICC, ADDR_ICC_DE_START, iLen, 0 );
	
	memset( CTLS_amt, 0x00, sizeof(CTLS_amt) );
      
	// set POS Entry Mode
	EMVDC_SetupPosEntryMode( 0x07 );

	if( FB_GetAmount(buf) == FALSE )
	  return;
	  
	memmove( CTLS_amt, buf, buf[0]+1 );
	  
	UI_ClearScreen();

	TL_insert_decimal_point( NUM_TYPE_DEC, buf ); // NNNNN.NN

	apk_ReadRamDataTERM( ADDR_TERM_AMT_AUTH_N, 6, amt );
	
	// setup transaction Date & Time
	TL_GetDateTime( &buf[1] );

	buf[0]=6;
	TL_asc2bcd( 3, buf2, buf );     // convert to BCD, buf2: YYMMDD
	apk_WriteRamDataTERM( ADDR_TERM_TX_DATE, 3, buf2 );
	
	buf[6]=6;
	TL_asc2bcd( 3, buf2, &buf[6] ); // convert to BCD, buf2: HHMMSS
	apk_WriteRamDataTERM( ADDR_TERM_TX_TIME, 3, buf2 );

	pData = EMVDC_contactless( amt, &rc, &iLen );	// start CTLS transaction...
//	TL_DumpHexData( 0, 0, iLen, pData );
	UI_ClearScreen();
	switch( rc )
	      {
	      case CTLS_FAILED:
	      	
	      	   DISP_STD_not_accepted();
//	      	   UI_WaitKey();
	      	   
	      	   break;
	      	   
	      case CTLS_OFFLINE_APPROVED:
	      case CTLS_SIGNATURE_REQUIRED:
	      	   
	      	   EMVDC_CTLS_SetupDataElements( iLen, pData );
	      	   DISP_STD_approved();
//	      	   UI_WaitKey();
	      	   
	      	   break;

	      case CTLS_ONLINE_APPROVED:
	      	   
	      	   EMVDC_CTLS_SetupDataElements( iLen, pData );
	      	   PCI_CtlsOnlineTransaction(0);
	      	   
	      	   break;

	      case CTLS_ONLINE_PIN_REQUIRED:
	      	
	      	   EMVDC_CTLS_SetupDataElements( iLen, pData );
	      	   PCI_CtlsOnlineTransaction(1);
	      	   
	      	   break;
	      
	      default:

	      	   DISP_STD_not_accepted();
//	      	   UI_WaitKey();
	      	   
	      	   break;
	      }
	
	UI_WaitKey();
	
	UT_LED_Switch( 6, 0 );
}
