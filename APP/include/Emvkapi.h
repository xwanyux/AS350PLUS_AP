//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : EMV L2 KERNEL API                                          **
//**  PRODUCT  : AS-300EX                                                   **
//**                                                                        **
//**  FILE     : EMVKAPI.H                                                  **
//**  MODULE   : Declaration of related EMV API functions.                  **
//**  VERSION  : V1.00                                                      **
//**  DATE     : 2003/06/05                                                 **
//**  EDITOR   : James Hsieh                                                **
//**                                                                        **
//**  Copyright(C) 2003 SymLink Corporation. All rights reserved.           **
//**                                                                        **
//****************************************************************************
//============================================================================
//
//----------------------------------------------------------------------------

//extern  UCHAR EMV_CB_OnlineProcessing( UCHAR *arc, UCHAR *rmsg );
extern  UCHAR EMV_CB_OnlineProcessing( UCHAR *arc, UCHAR *air, UCHAR *rmsg );
extern  UCHAR EMV_CB_FindMostRecentPAN( UCHAR *pan, UCHAR *amt );

extern  UCHAR api_emvk_GetDataElement( UCHAR tag1, UCHAR tag2, UCHAR *data );
extern  UCHAR api_emvk_InitEMVKernel( void );
extern  UCHAR api_emvk_OpenSessionICC( void );
extern  UCHAR api_emvk_CloseSessionICC( void );
extern  UCHAR api_emvk_DetectICC( void );
extern  UCHAR api_emvk_EnableICC( void );
extern  UCHAR api_emvk_DisableICC( void );
extern  UCHAR api_emvk_CreateCandidateList( void );
extern  UCHAR api_emvk_GetCandidateList( UCHAR *list_cnt, UCHAR *list );
extern  UCHAR api_emvk_SelectApplication( UCHAR list_cnt, UCHAR list_no, UCHAR *list_item );
extern  UCHAR api_emvk_SetTransPara( UCHAR *amt, UCHAR *sc );	// ==== Echo 2013/4/29 增加return ====
extern  UCHAR api_emvk_InitiateApplication( UCHAR tx_type );
extern  UCHAR api_emvk_ReadApplicationData( void );
extern  UCHAR api_emvk_OfflineDataAuthen( void );
extern  void  api_emvk_ProcessingRestrictions( void );
//extern  UCHAR api_emvk_CardholderVerification( UCHAR *epin );
extern  void  api_emvk_TerminalRiskManagement( void );
//extern  UCHAR api_emvk_TerminalActionAnalysis( void );
extern  UCHAR api_emvk_TerminalActionAnalysis( UCHAR GoOnline);
extern  void  api_emvk_Completion( UCHAR *arc );

//rex 2010/03/19 因 OffLine Pin 新增以下 
extern	UCHAR api_emvk_CardholderVerification( UCHAR ped, UCHAR algo, UCHAR mode, UCHAR index, UCHAR *epb, UCHAR *ksn, UINT tout );	// ==== Echo 2014/5/20 增加PIN輸入Time Out時間設定 ====
//rex 2010/03/19 因 OffLine Pin 新增以下 
extern	void	EMV_CB_ShowMsg_ENTER_PIN( UCHAR *amt );
extern	void	EMV_CB_ShowMsg_ENTER_PIN_BY_EXT_PINPAD( void );
extern	void	EMV_CB_ShowMsg_PIN_OK( void );
extern	void	EMV_CB_ShowMsg_INCORRECT_PIN( void );
extern	void	EMV_CB_ShowMsg_LAST_PIN_TRY( void );
extern	void	EMV_CB_ShowMsg_PLEASE_WAIT( void );

extern	void	api_emvk_GetPublicKeyInfo( UCHAR *info );	//	Black	2011/07/19
extern	UCHAR	api_emvk_PutTmsPARA(UCHAR *);	//	Black	2011/07/19
extern	UCHAR	api_emvk_PutTmsCAPK(UCHAR *);	//	Black	2011/07/19
extern	UCHAR	api_emvk_GetTmsDataElement(UCHAR,UCHAR,UCHAR,UCHAR *);	//	Black	2011/07/19
extern	void	api_emvk_PutWaveIEK(UCHAR *,UCHAR *);	//	Black	2011/07/19
extern	void	api_emvk_GetStatusBytes( UCHAR *sw1, UCHAR *sw2 );	// ==== Echo 2014/5/20 取回執行api_emvk_SelectApplication()失敗後的原因代碼 ====

//----------------------------------------------------------------------------
#define emvOK                   0x00
#define emvFailed               0x01
#define emvReady                0x00
#define emvNotReady             0x02
#define emvAborted              0x03
#define emvAutoSelected         0x04
#define emvOutOfService         0xff

