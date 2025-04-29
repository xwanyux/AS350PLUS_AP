/* -----------------------------------------------
File:			EMVtrans.c
Created Date:	2003/6/5
Purpose:	
--------------------------------------------------- */

#include <string.h>
#include "posapi.h"
// #include "GlobalVar.h"
// #include "option.h"
// #include "func.h"
// #include "constxp0.h"
// #include "constxp1.h"
#include "emvkapi.h"
// #include "API_PEDS.h"
// #include "Global.h"

#include "LCDTFTAPI.H"

/* ----------------------------------------------
Name:	Check_EMV_Exit
Funct:	�ˬd�����d�O�_���ް�
Input:  None
Output: None
Global:	
------------------------------------------------*/
#if 0
void Check_EMV_Exit(void)
{
	UCHAR	msg_emv_exit[10] = {0xBD,0xD0,0xB0,0x68,0xA5,0x58,0xA5,0x64,0xA4,0xF9};	// ==== �аh�X�d�� ====
	UCHAR	status;
	UCHAR	result = 1;

	forever
	{
		status = api_emvk_DetectICC();
		if(status == emvNotReady)
			break;
		else if(status == emvFailed)
			break;

		// ==== ���ˬd��A��ܰT�� ====
		if(result == 1)
		{
			ClrDisplayDN(LINE2);
			DispText(msg_emv_exit,LINE3,3,10,FONT2);	// ==== �аh�X�d�� ====
			result = 2;
		}
	}
	FlushKey();
}
#endif

/* ----------------------------------------------
Name:	press_zero_exit
Funct:	��[0]���}
Input:  None
Output: None
Global:	
------------------------------------------------*/
#if 0
void press_zero_exit(void)
{
	UCHAR	keyset[5] = {0x00,0x20,0x00,0x00,0x00};
	
	// �ۧU������������. By Sam 2020/01/10
	if(gl_self_type)
	{
		delay(30); // ���~�T�����d3s
		return;
	}

	gl_devkbd = api_kbd_open(DEV_KBD, (UCHAR *)&keyset);
	FlushKey();
	forever
	{
		if(FlopKey() == '0')
			return;
	}
}
#endif

/* ----------------------------------------------
Name:	EMV_Message
Funct:	��ܿ��~�T��
Input:  1:���ܥi�ഫ��Fall Back
		2:����IC�d�ڵ���� �� ������ޥd	=>	�e���|�a�N��
												01 api_emvk_DectectICC
												02 api_emvk_CreateCandidateList
												03 api_emvk_GetCandidateList
												04 api_emvk_SelectApplication
												05 api_emvk_InitiateApplication
												06 api_emvk_ReadApplicationData
												07 api_emvk_OfflinedataAuthen
												08 api_emvk_CardholderVerification
												09 api_emvk_TerminalActionAnalysis
												10 api_emvk_TerminalActionAnalysis(ARPC FAIL)
		3:���ܤ��~�L�kŪ�������d(�L�ϥ�) => �אּ����3�������"�аh�X�d��"
		4:���ܤ��~������
		5:Respone Code�^���D'00'
		6:Field_55,Field_56�O���^�s������
		7:���ܥ������
		8:����IC�d��Ʋ��`(�����D) => �e���|�a�N��
		9:���ܥ������ => ��Cancel�|���"�аh�X�d��",��3�����]�|�X�{"�аh�X�d��",�ޥd��^�D�e��
		10:Get Candidate List��,application blocked(6283)�����~�e��

		[ECR]mode:
		0:���Ⱦ��s�u�n�^Error
		1:���Ⱦ��s�u���n�^Error
Output: None
Global:	
------------------------------------------------*/
#if 0
void	EMV_Message(UCHAR active,UCHAR mode)
{
	UCHAR	msg_Fall_Back_1[LEN_DISP] = {' ',' ',' ',' ',0xB4,0xB9,0xA4,0xF9,0xB7,0xB4,0xB7,0x6C,' ',' ',' ',' '};		// ==== �������l ====
	UCHAR	msg_Fall_Back_2[LEN_DISP] = {' ',' ',' ',0xBD,0xD0,0xA7,0xEF,0xA8,0xEA,0xBA,0xCF,0xB1,0xF8,' ',' ',' '};	// ==== �Ч��ϱ�====

	UCHAR	msg_read_fail_1[8] = {0xA5,0xE6,0xA9,0xF6,0xA5,0xA2,0xB1,0xD1};	// ==== ������� ====
	UCHAR	msg_read_fail_2[10] = {0xBD,0xD0,0xB0,0x68,0xA5,0x58,0xA5,0x64,0xA4,0xF9};	// ==== �аh�X�d�� ====

	UCHAR	msg_chip_fail_1[12] = {0xB4,0xB9,0xA4,0xF9,0xB5,0x4C,0xAA,0x6B,0xA8,0xCF,0xA5,0xCE};	// ==== �����L�k�ϥ� ====
	UCHAR	msg_chip_fail_2[12] = {0xBD,0xD0,0xB3,0x73,0xB5,0xB8,0xB5,0x6F,0xA5,0x64,0xA6,0xE6};	// ==== �гs���o�d�� ====

	UCHAR	msg_card_lose[8] = {0xA5,0xE6,0xA9,0xF6,0xA8,0xFA,0xAE,0xF8};	// ==== ������� ====
//	UCHAR	msg_trans_ok_1[8] = {0xA5,0xE6,0xA9,0xF6,0xA6,0xA8,0xA5,0x5C};	// ==== ������\ ====

	UCHAR	msg_sram_err_1[LEN_DISP] = {' ',0xB0,0x4F,0xBE,0xD0,0xC5,0xE9,0xC5,0xAA,0xA8,0xFA,0xA5,0xA2,0xB1,0xD1};	// ==== �O����Ū������ ====
	UCHAR	msg_press_Zero[11] = {' ',0xAB,0xF6,'\'','0','\'',0xC2,0xF7,0xB6,0x7D,' '};	// ==== ��'0'���} ====
	UCHAR	msg_data_err[8] = {0xB8,0xEA,0xAE,0xC6,0xB2,0xA7,0xB1,0x60};	// ==== ��Ʋ��` ====

	UCHAR	result, status;
	UCHAR	time_hnd = 0;
	UINT	get_time;
	UCHAR	kbd_set[5] = {0x00,0x00,0x00,0x04,0x00};
//	UCHAR	result_flag = SUCCEED;

	// �ۧU��������ܦ��T��. By Sam 2020/01/10
	if(gl_self_type)
		memset(msg_press_Zero, 0x20, sizeof(msg_press_Zero));

	//if(active == 1 || active == 2 || active == 8 || active == 3 || active == 7 || active == 9)	// ==== debug mode ====
	if(active == 8 || active == 3 || active == 7 || active == 9)
		;
	else
		ClrDisplayDN(LINE2);

	// ==== Echo 2007/11/13 [ECR]���Ⱦ��_�_�� ====
	// ==== Echo 2015/6/15 ====
	if((active == 1 || active == 2 || active == 3 || active == 4 || active == 6  || active == 8 || active == 9 || active == 10) && (gl_ecr_flag != 0))
	{
		// ==== �[�J"gl_emv_ecr_senderr_flag"����EDC�^��ECR������ ====
		if(mode == 0 && gl_emv_ecr_senderr_flag == 0)
			AUXsend(0xFF);	// ==== 2013/2/6 �^�Ш���(���~)�T�� ====
	}

	if(active == 1)
	{
		ClrDisplayDN(LINE2);
		DispText(msg_Fall_Back_1,LINE2,0,LEN_DISP,FONT2);	// ==== �������l ====
		DispText(msg_Fall_Back_2,LINE3,0,LEN_DISP,FONT2);	// ==== �Ч��ϱ� ====
		DispText(msg_press_Zero,LINE4,3,11,FONT2);			// ==== ��'0'���} ====
		gl_fall_back_flag = 1;	// ==== Fall Back flag on ====

		LongBeep();
		press_zero_exit();	// ==== ��[0]�� ====
	}
	else if(active == 2)
	{
		ClrDisplayDN(LINE2);
		DispText(msg_read_fail_1,LINE3,4,8,FONT2);	// ==== ������� ====
		DispText(msg_press_Zero,LINE4,2,11,FONT2);	// ==== ��'0'���} ====

#if CERT_MODE == CERT_ADVT || CERT_MODE == CERT_UNIONPAY
		UCHAR	EMV_result2;
		UCHAR	buf2[300];
		EMV_result2 = api_emvk_GetDataElement(0x00,0x9B,(UCHAR *)&buf2[0]);
		if(EMV_result2 == emvOK)
		{
			if(buf2[0] != 0)
			{
				debug(0x9B, &buf2[2], buf2[0], 100);
			}
		}
#endif

		if (gl_Switch[SW_DEBUG] == ON)	//�����Ҧ� 	//	Black	2011/03/29
		{
			if(gl_debug_mode_flag & 0x02)	//alex 09/09/01
				EMV_Debug_print(); //alex 2003/09/30
		}

		LongBeep();

		ResetLine();
		press_zero_exit();	// ==== ��[0]�� ====
	}
	else if(active == 3)
		;
	else if(active == 4)
		;
	else if(active == 5)
	{
		DispRspText();
		LongBeep();

		// ==== Echo 2013/2/6 [ECR]���Ⱦ��_�_�� ====
		if(gl_ecr_flag != 0)
		{
			if(mode == 0)
				AUXsend(0xFF);
		}

		// ==== ���X�{Y1,Y2,Z1,Z2��,�����n�aOffline��� ====
		if(!memcmp(gl_rsp_data.rspcode,"Y1",2) || !memcmp(gl_rsp_data.rspcode,"Y3",2) || !memcmp(gl_rsp_data.rspcode,"Z1",2) || !memcmp(gl_rsp_data.rspcode,"Z3",2))
			;
		else
		{
#if VERSION_MODE != VERSION_NCCC
			if (DinersHost() == FALSE)
				OfflineProc(ONLY_ONE);
			else
				ResetLine();
#else
			OfflineProc(ONLY_ONE);
#endif

			// ==== Echo 2014/5/1 ���`SAM Slot-1���������]���ݰ�ATR ====
			// ==== Echo 2014/5/1 �}��SAM�d�[�K ====
//			if(CubHost() == TRUE && (gl_lan_port.status == 2 || gl_lan_port.status == 3 || gl_lan_port.status == 4))	//	Black	2011/03/21
//				IFM_SAM_Close();		// ==== ����SAM2 ====
		}
	}
	else if(active == 6)
	{
		ClrDisplayDN(LINE2);
		DispText(msg_sram_err_1,LINE2,0,LEN_DISP,FONT2);	// ==== �O����Ū������ ====
		DispText(msg_read_fail_2,LINE3,3,10,FONT2);			// ==== �аh�X�d�� ====
	}
	else if(active == 7)
		;
	else if(active == 8)
	{
		DispText(msg_data_err,LINE3,4,8,FONT2);		// ==== ��Ʋ��` ====
		DispText(msg_press_Zero,LINE4,2,11,FONT2);	// ==== ��'0'���} ====

		LongBeep();

		ResetLine();
		press_zero_exit();	// ==== ��[0]�� ====
	}
	else if(active == 10)	// ==== Echo 2015/6/15 ====
	{
		ClrDisplayDN(LINE2);
		DispText(msg_chip_fail_1,LINE2,2,12,FONT2);	// ==== �����L�k�ϥ� ====
		DispText(msg_chip_fail_2,LINE3,2,12,FONT2);	// ==== �гs���o�d�� ====
		DispText(msg_press_Zero,LINE4,3,11,FONT2);	// ==== ��'0'���} ====

#if CERT_MODE == CERT_ADVT || CERT_MODE == CERT_UNIONPAY
		UCHAR	EMV_result2;
		UCHAR	buf2[300];
		EMV_result2 = api_emvk_GetDataElement(0x00,0x9B,(UCHAR *)&buf2[0]);
		if(EMV_result2 == emvOK)
		{
			if(buf2[0] != 0)
			{
				debug(0x9B, &buf2[2], buf2[0], 100);
			}
		}
#endif

		if (gl_Switch[SW_DEBUG] == ON)	//�����Ҧ� 	//	Black	2011/03/29
		{
			if(gl_debug_mode_flag & 0x02)	//alex 09/09/01
				EMV_Debug_print(); //alex 2003/09/30
		}

		LongBeep();

		ResetLine();
		press_zero_exit();	// ==== ��[0]�� ====
	}

	ResetLine();

	// ==== ���������"CANCEL"�ε�3������X�{"�аh�X�d��" ====
	// ==== �[�W�ڵ���������p							  ====
	// ==== �[�W����3����X�{"�аh�X�d��"				  ====
	if((active == 9) || (active == 5) || (active == 3))
		gl_devkbd = api_kbd_open(DEV_KBD , (UCHAR *)&kbd_set);
	else
		api_kbd_close(0);
	api_emvk_DisableICC();	// ����EMV Reader�q��

	// ==== ���������"CANCEL"�ε�3������X�{"�аh�X�d��" ====
	// ==== �[�W����3����X�{"�аh�X�d��"				  ====
	if((active == 9) || (active == 5) || (active == 3))
		time_hnd = api_tim_open(100); // 10 = 0.1 seconds , 1 unit = 0.01 seconds.

	result = 1;
	do
	{
		// ==== ���������"CANCEL"�ε�3������X�{"�аh�X�d��" ====
		// ==== �[�W�ڵ���������p							  ====
		if((active == 9) || (active == 5))
		{
			api_tim_gettick(time_hnd, (UCHAR *)&get_time);
//			if(GetKey() == KEY_CANCEL || get_time > 180)
			//	Black	2019/06/06
			if((GetKey() == KEY_CANCEL) || (get_time > gl_Time_Out))	// ==== �����I ====
			{
				ClrDisplayDN(LINE2);
				DispText(msg_read_fail_2,LINE3,3,10,FONT2);	// ==== �аh�X�d�� ====
				active = 0xFF;
				api_kbd_close(0);
				FlushKey();
			}
		}
		// ==== �[�W����5����X�{"�аh�X�d��" ====
		else if(active == 3)
		{
			api_tim_gettick(time_hnd, (UCHAR *)&get_time);
			if((GetKey() == KEY_CANCEL) || (get_time > 5))	// ==== Echo 2015/6/26 ���~�T���אּ��5�� ====
			{
				ClrDisplayDN(LINE2);
				DispText(msg_read_fail_2,LINE3,3,10,FONT2);	// ==== �аh�X�d�� ====
				active = 0xFF;
				api_kbd_close(0);
				FlushKey();
			}
		}

		status = api_emvk_DetectICC();
		if(status == emvNotReady)
			result = 0;
		else if(status == emvFailed)
			result = 0;

		// ==== ���ˬd��A��ܰT�� ====
		if(((active == 1) || (active == 2) || (active == 4) || (active == 8) || (active == 10)) && (result == 1))
		{
			ClrDisplayDN(LINE2);
			DispText(msg_read_fail_2,LINE3,3,10,FONT2);	// ==== �аh�X�d�� ====
			result = 2;
		}
	}while(result != 0);

	// ==== ���������"CANCEL"�ε�3������X�{"�аh�X�d��" ====
	if((active == 0xFF) || (active == 9) || (active == 5) || (active == 3))
		api_tim_close(time_hnd);

	if(active == 6)
	{
		ClrDisplayDN(LINE2);
		DispText(msg_card_lose,LINE3,4,8,FONT2);	// ==== ������� ====
	}

	gl_change = TRUE;
}
#endif

/* ----------------------------------------------
Name:	EMVPresent
Funct:	�����O�_��Ū����d��
Input:	None
Output: 
Global:	
------------------------------------------------*/
#if 0
UCHAR	EMVPresent(void)
{
	UCHAR	status;

	status = api_emvk_DetectICC();
	if(status == emvOK)
		return(SUCCEED);
	else
		return(FAIL);
}
#endif

/* ----------------------------------------------
Name:	EMV_status_check
Funct:	�����d���O�_�٦b
Input:	None
Output: 
Global:	
------------------------------------------------*/
#if 0
UCHAR	EMV_status_check(void)
{
	UCHAR	EMV_result;
	UCHAR	dispbuf[2] = {"00"};

	EMV_result = EMVPresent();
	if(EMV_result == FAIL)
	{
		memcpy(dispbuf,"01",2);

		ClrDisplayDN(LINE2);
		DispText(dispbuf,LINE2,0,2,FONT1);
		EMV_Message(2,0);
	}

	return EMV_result;
}
#endif

/* ----------------------------------------------
Name:	GetEmvData
Funct:	�qIC�dŪ��PAN & AID & Name & Field_55
Input:  1:Ū��AID				- 10
		2:Ū��PAN				- 20
								  21(TrackII �䤣�� 'D')
								  22(Check Digit error of PAN)
		3:Ū��Name				- 30
		4:Ū��Field_55			- 40
		5:Ū��Field_56			- 50
		6:Ū��TC value(�����X)	- 60
		7:Ū��TAG 50 - AP LABEL 
Output: SUCCEED : ���\
		FAIL : ����
Global:	
Black	2019/01/23	����Tag 57
Black	2018/09/21	[AMEX]�ק�AE�������
Black	2018/09/21	[AMEX]�s�W�x�sAP Label
Black	2018/09/21	[AMEX]POS Entry Mode
Black	2020/06/10	�s�W�x�sService Code
Black	2020/06/12	�s�W�ثn�Ȧ�
Black	2020/09/02	�ק��p���Ȧ�
Black	2020/10/19	�s�W�ɤs�Ȧ�
rex 	2020/11/26	[NCCC]
Black	2020/12/21	�ثn�Ȧ�H�Υd�n�x�sAID
Black	2020/12/28	M-TIP�s�WTag 84
rex		2021/05/11	[NCCC]
Black	2021/09/23	�ثn�Ȧ�s�WTag 84
Black	2021/10/21	�s�W�]����T
Black	2022/01/14	�ק��p���Ȧ�
Black	2022/05/12	�s�W�x�s�Ȧ�
Black	2022/05/31	�x�s�Ȧ�s�WTag 84
Black	2022/06/01	�ثn�Ȧ�B�x�s�Ȧ�H�Υd�n�x�sAID
Black	2022/12/27	�s�W����H�U
------------------------------------------------*/
#if 0
UCHAR	GetEmvData(UCHAR action)
{
	UCHAR	EMV_result = emvFailed, result;
	UCHAR	buf[200];
#if VERSION_MODE != VERSION_NCCC
//	UCHAR	emv_tag_55[58] = {	0x00,0x57,0x00,0x5A,0x5F,0x24,0x5F,0x2A,0x5F,0x34,
//								0x00,0x82,0x00,0x84,0x00,0x8A,0x00,0x95,0x00,0x9A,
//								0x00,0x9B,0x00,0x9C,0x9F,0x02,0x9F,0x03,0x9F,0x09,
//								0x9F,0x10,0x9F,0x1A,0x9F,0x1E,0x9F,0x26,0x9F,0x27,
//								0x9F,0x33,0x9F,0x34,0x9F,0x35,0x9F,0x36,0x9F,0x37,
//								0x9F,0x41,0x9F,0x5B,0x9F,0x63,0x9F,0x6E};
	//	Black	2019/01/23	����Tag 57
	UCHAR	cub_tag_55[56] = {	0x00,0x5A,0x5F,0x24,0x5F,0x2A,0x5F,0x34,0x00,0x82,
								0x00,0x84,0x00,0x8A,0x00,0x95,0x00,0x9A,0x00,0x9B,
								0x00,0x9C,0x9F,0x02,0x9F,0x03,0x9F,0x09,0x9F,0x10,
								0x9F,0x1A,0x9F,0x1E,0x9F,0x26,0x9F,0x27,0x9F,0x33,
								0x9F,0x34,0x9F,0x35,0x9F,0x36,0x9F,0x37,0x9F,0x41,
								0x9F,0x5B,0x9F,0x63,0x9F,0x6E};

	//	Black	2020/12/28	M-TIP�s�WTag 84
	//	Black	2021/09/23	�ثn�Ȧ�s�WTag 84
//#if CERT_MODE == CERT_MTIP
	UCHAR	hncb_tag_55[44] = {	0x5F,0x2A,0x5F,0x34,0x00,0x82,0x00,0x84,0x00,0x8A,
								0x00,0x95,0x00,0x9A,0x00,0x9C,0x9F,0x02,0x9F,0x03,
								0x9F,0x10,0x9F,0x1A,0x9F,0x1E,0x9F,0x26,0x9F,0x27,
								0x9F,0x33,0x9F,0x34,0x9F,0x35,0x9F,0x36,0x9F,0x37,
								0x9F,0x63,0x9F,0x6E};
//#else
//	//	Black	2020/06/12	�ثn�Ȧ�
//	UCHAR	hncb_tag_55[42] = {	0x5F,0x2A,0x5F,0x34,0x00,0x82,0x00,0x8A,0x00,0x95,
//								0x00,0x9A,0x00,0x9C,0x9F,0x02,0x9F,0x03,0x9F,0x10,
//								0x9F,0x1A,0x9F,0x1E,0x9F,0x26,0x9F,0x27,0x9F,0x33,
//								0x9F,0x34,0x9F,0x35,0x9F,0x36,0x9F,0x37,0x9F,0x63,
//								0x9F,0x6E};
//#endif

	//	Black	2022/01/14	�p���Ȧ�
	UCHAR	ubot_tag_55[48] = {	0x5F,0x2A,0x5F,0x34,0x00,0x82,0x00,0x84,0x00,0x95,
								0x00,0x9A,0x00,0x9C,0x9F,0x02,0x9F,0x03,0x9F,0x09,
								0x9F,0x10,0x9F,0x1A,0x9F,0x1E,0x9F,0x26,0x9F,0x27,
								0x9F,0x33,0x9F,0x34,0x9F,0x35,0x9F,0x36,0x9F,0x37,
								0x9F,0x53,0x9F,0x5B,0x9F,0x63,0x9F,0x6E};

	//	Black	2020/10/19	�ɤs�Ȧ�
	UCHAR	esun_tag_55[58] = {	0x00,0x57,0x5F,0x2A,0x5F,0x30,0x5F,0x34,0x00,0x82,
								0x00,0x84,0x00,0x95,0x00,0x9A,0x00,0x9B,0x00,0x9C,
								0x9F,0x02,0x9F,0x03,0x9F,0x08,0x9F,0x09,0x9F,0x10,
								0x9F,0x1A,0x9F,0x1E,0x9F,0x26,0x9F,0x27,0x9F,0x33,
								0x9F,0x34,0x9F,0x35,0x9F,0x36,0x9F,0x37,0x9F,0x41,
								0x9F,0x63,0xDF,0xED,0xDF,0xEE,0xDF,0xEF};

	//	Black	2021/10/21	�]����T�A�קאּ�]���W��12.1��EMV Tag
	UCHAR	fisc_tag_55[60] = {	0x00,0x57,0x00,0x5A,0x5F,0x24,0x5F,0x2A,0x5F,0x34,
								0x00,0x82,0x00,0x84,0x00,0x8A,0x00,0x95,0x00,0x9A,
								0x00,0x9B,0x00,0x9C,0x9F,0x02,0x9F,0x03,0x9F,0x10,
								0x9F,0x1A,0x9F,0x1E,0x9F,0x26,0x9F,0x27,0x9F,0x33,
								0x9F,0x34,0x9F,0x35,0x9F,0x36,0x9F,0x37,0x9F,0x5B,
								0x9F,0x63,0x9F,0x6E,0x9F,0x7C,0xDF,0xED,0xDF,0xEF};

	//	Black	2022/05/31	�x�s�Ȧ�s�WTag 84
	UCHAR	taishin_tag_55[58] = {	0x00,0x57,0x00,0x5A,0x5F,0x24,0x5F,0x2A,0x5F,0x34,
									0x00,0x82,0x00,0x84,0x00,0x8A,0x00,0x91,0x00,0x95,
									0x00,0x9A,0x00,0x9B,0x00,0x9C,0x9F,0x02,0x9F,0x03,
									0x9F,0x10,0x9F,0x18,0x9F,0x1A,0x9F,0x1E,0x9F,0x26,
									0x9F,0x27,0x9F,0x33,0x9F,0x34,0x9F,0x35,0x9F,0x36,
									0x9F,0x37,0x9F,0x5B,0x9F,0x63,0x9F,0x6E};

	//	Black	2022/12/27	����H�U
	UCHAR	ctbc_tag_55[40] = {	0x5F,0x2A,0x5F,0x34,0x00,0x82,0x00,0x84,0x00,0x95,
								0x00,0x9A,0x00,0x9C,0x9F,0x02,0x9F,0x03,0x9F,0x09,
								0x9F,0x10,0x9F,0x1A,0x9F,0x26,0x9F,0x27,0x9F,0x33,
								0x9F,0x34,0x9F,0x35,0x9F,0x36,0x9F,0x37,0x9F,0x63};	// ==== Echo 2014/5/3 UICC�W�[Ū��Tag 9F63 ====

	//	Black	2018/09/21	[AMEX]AMEX EMV Tag
	UCHAR	amex_tag_55[40] = {	0x9F,0x26,0x9F,0x10,0x9F,0x37,0x9F,0x36,0x00,0x95,
								0x00,0x9A,0x00,0x9C,0x9F,0x02,0x5F,0x2A,0x9F,0x1A,
								0x00,0x82,0x9F,0x03,0x5F,0x34,0x9F,0x27,0x9F,0x06,
								0x9F,0x09,0x9F,0x34,0x9F,0x0E,0x9F,0x0F,0x9F,0x0D};

	//	Black	2022/12/27	����H�U
	UCHAR	emv_tag_56[20] = {0x00,0x5A,0x5F,0x34,0x00,0x95,0x00,0x9A,0x9F,0x10,
							  0x9F,0x37,0x9F,0x41,0x9F,0x63,0xDF,0xE1,0xDF,0xE2};	// ==== Echo 2014/5/3 UICC�W�[Ū��Tag 9F63 ====

	//	Black	2021/10/21
	UCHAR	emv_tag_DFED[4] = {0xDF,0xED,0x01,0x20};
	UCHAR	emv_tag_DFEF[5] = {0xDF,0xEF,0x02,0x15,0x00};
	UCHAR	tempa[5];		//	�s���X�� �ݦs�b
#else	//[NCCC]
	const UCHAR	emv_tag_55[60] = {
		0x5F,0x2A, 0x5F,0x34, 0x00,0x82, 0x00,0x84, 0x00,0x8A, 
		0x00,0x95, 0x00,0x9A, 0x00,0x9B, 0x00,0x9C, 0x9F,0x02, 
		0x9F,0x03, 0x9F,0x06, 0x9F,0x08, 0x9F,0x09, 0x9F,0x10, 
		0x9F,0x1A, 0x9F,0x1E, 0x9F,0x26, 0x9F,0x27, 0x9F,0x33, 
		0x9F,0x34, 0x9F,0x35, 0x9F,0x36, 0x9F,0x37, 0x9F,0x41, 
		0x9F,0x5B, 0x9F,0x63, 0xDF,0xED, 0xDF,0xEE, 0xDF,0xEF};
#endif

	UCHAR	dispbuf[2] = {"00"};
	UCHAR	i;
	UINT	idx;

	UCHAR	tag_count = 0;	//	Black	2018/09/21	[AMEX]

	switch(action)
	{
	case 1:	//-1.----Get AID from IC Card---------------
		EMV_result = api_emvk_GetDataElement(0x00,0x84,(UCHAR *)&buf);
		if(EMV_result == emvFailed)
		{
			memcpy(dispbuf,"10",2);

			ClrDisplayDN(LINE2);
			DispText(dispbuf,LINE2,0,2,FONT1);
			EMV_Message(8,0);
			return FAIL;
		}
		memcpy(gl_emvk_aid,&buf[2],8);	// ==== Echo 2014/5/3 �O�������d��AID ====

#if VERSION_MODE != VERSION_NCCC
		//	Black	2020/12/21	�ثn�Ȧ�H�Υd�n�x�sAID
//		if (gl_Credit_Host == 1)
		//	Black	2022/06/01	�ثn�Ȧ�B�x�s�Ȧ�H�Υd�n�x�sAID
		if ((gl_Credit_Host == 1) || (gl_Credit_Host == 5))
		{
			gl_input.ecr_ref[6] = buf[0];
			memcpy(&gl_input.ecr_ref[7], &buf[2], gl_input.ecr_ref[6]);
		}
#else
		gl_input.CUP_AID[0] = buf[0];	//rex 2021/05/11 
		memcpy(&gl_input.CUP_AID[1], &buf[2], buf[0]);
#endif

		break;

	case 2:	//-2.----Get PAN from IC Card---------------
		EMV_result = api_emvk_GetDataElement(0x00,0x57,(UCHAR *)&buf);
		if(EMV_result == emvFailed)
		{
			memcpy(dispbuf,"20",2);

			ClrDisplayDN(LINE2);
			DispText(dispbuf,LINE2,0,2,FONT1);
			EMV_Message(8,0);
			return FAIL;
		}

		memset(gl_input.pan, EMPTY, MAX_PAN_HALF );	// ==== set buffer to empty ====
		gl_input.trk2[0] = buf[0] * 2;	// ==== TrackII ���� ====

		//rex 2011/06/16 �ˬd�O�_��F�b�G�y�A�Y���h���ܤG�y���_�ơA�ݱN����-1	//	Black	2011/06/27
		result = FindBit((UCHAR *)&buf[2],0x0F,buf[0]);
		if (result != STR_END)
			gl_input.trk2[0]-=1;

		memcpy((UCHAR *)&gl_input.trk2[1],&buf[2],buf[0]);	// ==== �x�sTrackII Data ====
		buf[1] = FindBit(&gl_input.trk2[1],0x0D,buf[0]);	// ==== �p��XPAN������ ====
		if(buf[1] == STR_END)	// ==== �p�G�L�k��X�d��(�䤣��0x0D����m)!! ====
		{
			memcpy(dispbuf,"21",2);

			ClrDisplayDN(LINE2);
			DispText(dispbuf,LINE2,0,2,FONT1);
			EMV_Message(8,0);
			return FAIL;
		}

		memcpy(gl_input.pan,(UCHAR *)&gl_input.trk2[1],buf[1]);	// ==== �x�sPAN ====

		// ==== �ˬd�̫�b��Byte�O�_��'0x0D' ====
		result = FindBit(gl_input.pan,0x0D,buf[0]);
		if(STR_END != result)
		{
			gl_input.pan[buf[1] - 1] = gl_input.pan[buf[1] - 1] | 0x0F;
			memcpy(gl_input.exp_date,(UCHAR *)&gl_input.trk2[buf[1] + 1],2);
			//	Black	2020/06/10	�s�W�x�sService Code
			memcpy(gl_input.service_code,(UCHAR *)&gl_input.trk2[buf[1] + 3],2);
		}
		else
		{
			gl_input.exp_date[0] = ((gl_input.trk2[buf[1] + 1] << 4) & 0xF0) + ((gl_input.trk2[buf[1] + 2] >> 4) & 0x0F);
			gl_input.exp_date[1] = ((gl_input.trk2[buf[1] + 2] << 4) & 0xF0) + ((gl_input.trk2[buf[1] + 3] >> 4) & 0x0F);
			//	Black	2020/06/10	�s�W�x�sService Code
			gl_input.service_code[0] = ((gl_input.trk2[buf[1] + 3] << 4) & 0xF0) + ((gl_input.trk2[buf[1] + 4] >> 4) & 0x0F);
			gl_input.service_code[1] = ((gl_input.trk2[buf[1] + 4] << 4) & 0xF0) + ((gl_input.trk2[buf[1] + 5] >> 4) & 0x0F);
		}

		if(NOT_FOUND == InCardTable(gl_input.status))	// ==== �ھڥd����XTMS��Acquirer�PIssuer�]�w ====
			return SUCCEED;

#if VERSION_MODE != VERSION_NCCC
		//	Black	2018/09/21	[AMEX]POS Entry Mode
		if (AmexHost() == TRUE)
		{
			gl_input.entry_mode[0] = 0x05;
//			gl_input.entry_mode[1] = 0x51;
			gl_input.entry_mode[1] = 0x52;
		}
#endif

		if(CheckPan() == FAIL)
		{
			gl_get_card_error_flag = 1;	// ==== �˽X���~flag ====
			return(SUCCEED);
		}
/*
		if(!GetExpireDate(!ChkIssuerOption(20))) // ==== ���ˬd���Ĥ�� ====
		{
			EMV_Message(4,0);
			return FAIL;
		}
*/
		break;

	case 3:	//-3.----Get Name from IC Card--------------
		EMV_result = api_emvk_GetDataElement(0x5F,0x20,(UCHAR *)&buf);
		if(EMV_result == emvOK)
		{
			if(buf[0] != 0)
			{
				memset(gl_input.holder_name, ' ', HOLDER_NAME_LEN );	// ==== set buffer to empty ====
				memcpy(gl_input.holder_name,&buf[2],buf[0]);			// ==== �x�sName ====
			}
		}
		break;

	case 4:	//-4.----Get Field_55 Data from IC Card-----
		idx = 0;

#if VERSION_MODE != VERSION_NCCC
		//	Black	2016/12/13	�קﴹ��Tag�ƶq
		// ==== Echo 2014/5/3 UICC�W�[Ū��Tag 9F63 ====
//		if(gl_input.redeem_flag == 3)
//			tag_count = 20;
//		else
//			tag_count = 19;
//		tag_count = 29;
		//	Black	2019/01/23	����Tag 57
//		tag_count = 28;
		//	Black	2018/09/21	[AMEX]
		if (AmexHost() == TRUE)
			tag_count = sizeof(amex_tag_55) / 2;
		else
		{
			//	Black	2022/12/27
			if (gl_Credit_Host == 0)
				tag_count = sizeof(cub_tag_55) / 2;
			else if (gl_Credit_Host == 1)
				tag_count = sizeof(hncb_tag_55) / 2;
			else if (gl_Credit_Host == 2)
				tag_count = sizeof(ubot_tag_55) / 2;
			else if (gl_Credit_Host == 3)
				tag_count = sizeof(esun_tag_55) / 2;
			else if (gl_Credit_Host == 4)
				tag_count = sizeof(fisc_tag_55) / 2;
			else if (gl_Credit_Host == 5)
				tag_count = sizeof(taishin_tag_55) / 2;
			else if (gl_Credit_Host == 6)
				tag_count = sizeof(ctbc_tag_55) / 2;
		}
#else
//remark by rex 2020/11/26 [NCCC]
		tag_count = sizeof(emv_tag_55) / 2;
#endif

		//	Black	2018/09/21	[AMEX]
		for(i = 0 ; i < tag_count ; i++)
		{
#if VERSION_MODE != VERSION_NCCC
			//	Black	2018/09/21	[AMEX]
			if (AmexHost() == TRUE)
				EMV_result = api_emvk_GetDataElement(amex_tag_55[i*2],amex_tag_55[i*2 + 1],(UCHAR *)&buf);
			else
			{
				//	Black	2022/12/27
				if (gl_Credit_Host == 0)
					EMV_result = api_emvk_GetDataElement(cub_tag_55[i*2],cub_tag_55[i*2 + 1],(UCHAR *)&buf);
				else if (gl_Credit_Host == 1)
					EMV_result = api_emvk_GetDataElement(hncb_tag_55[i*2],hncb_tag_55[i*2 + 1],(UCHAR *)&buf);
				else if (gl_Credit_Host == 2)
					EMV_result = api_emvk_GetDataElement(ubot_tag_55[i*2],ubot_tag_55[i*2 + 1],(UCHAR *)&buf);
				else if (gl_Credit_Host == 3)
					EMV_result = api_emvk_GetDataElement(esun_tag_55[i*2],esun_tag_55[i*2 + 1],(UCHAR *)&buf);
				else if (gl_Credit_Host == 4)
					EMV_result = api_emvk_GetDataElement(fisc_tag_55[i*2],fisc_tag_55[i*2 + 1],(UCHAR *)&buf);
				else if (gl_Credit_Host == 5)
					EMV_result = api_emvk_GetDataElement(taishin_tag_55[i*2],taishin_tag_55[i*2 + 1],(UCHAR *)&buf);
				else if (gl_Credit_Host == 6)
					EMV_result = api_emvk_GetDataElement(ctbc_tag_55[i*2],ctbc_tag_55[i*2 + 1],(UCHAR *)&buf);
			}
#else
//remark by rex 2020/11/26 [NCCC]
			EMV_result = api_emvk_GetDataElement(emv_tag_55[i*2],emv_tag_55[i*2 + 1],(UCHAR *)&buf);
#endif

			if(EMV_result == emvOK)
			{
				if(buf[0] != 0)
				{
					if (idx + buf[0] >  sizeof(gl_rinit.data))		//	Black	2013/01/17	������d��55���ȶW1000
					{
						memcpy((UCHAR *)&gl_rsp_data.rspcode, "B3", 2);
						return FAIL;
					}

					//1-���g�J TAG �W��
#if VERSION_MODE != VERSION_NCCC
					//	Black	2018/09/21	[AMEX]
					if (AmexHost() == TRUE)
					{
						if(amex_tag_55[i*2] == 0x00)
						{
							gl_rinit.data[idx] = amex_tag_55[i*2 + 1];
							idx = idx + 1;
						}
						else
						{
							memcpy(&gl_rinit.data[idx],&amex_tag_55[i*2],2);
							idx = idx + 2;
						}
					}
					else
					{
						//	Black	2022/12/27
						if (gl_Credit_Host == 0)
						{
							if(cub_tag_55[i*2] == 0x00)
							{
								gl_rinit.data[idx] = cub_tag_55[i*2 + 1];
								idx = idx + 1;
							}
							else
							{
								memcpy(&gl_rinit.data[idx],&cub_tag_55[i*2],2);
								idx = idx + 2;
							}
						}
						else if (gl_Credit_Host == 1)
						{
							if(hncb_tag_55[i*2] == 0x00)
							{
								gl_rinit.data[idx] = hncb_tag_55[i*2 + 1];
								idx = idx + 1;
							}
							else
							{
								memcpy(&gl_rinit.data[idx],&hncb_tag_55[i*2],2);
								idx = idx + 2;
							}
						}
						else if (gl_Credit_Host == 2)
						{
							if(ubot_tag_55[i*2] == 0x00)
							{
								gl_rinit.data[idx] = ubot_tag_55[i*2 + 1];
								idx = idx + 1;
							}
							else
							{
								memcpy(&gl_rinit.data[idx],&ubot_tag_55[i*2],2);
								idx = idx + 2;
							}
						}
						else if (gl_Credit_Host == 3)
						{
							if(esun_tag_55[i*2] == 0x00)
							{
								gl_rinit.data[idx] = esun_tag_55[i*2 + 1];
								idx = idx + 1;
							}
							else
							{
								memcpy(&gl_rinit.data[idx],&esun_tag_55[i*2],2);
								idx = idx + 2;
							}
						}
						else if (gl_Credit_Host == 4)
						{
							if(fisc_tag_55[i*2] == 0x00)
							{
								gl_rinit.data[idx] = fisc_tag_55[i*2 + 1];
								idx = idx + 1;
							}
							else
							{
								memcpy(&gl_rinit.data[idx],&fisc_tag_55[i*2],2);
								idx = idx + 2;
							}
						}
						else if (gl_Credit_Host == 5)
						{
							if(taishin_tag_55[i*2] == 0x00)
							{
								gl_rinit.data[idx] = taishin_tag_55[i*2 + 1];
								idx = idx + 1;
							}
							else
							{
								memcpy(&gl_rinit.data[idx],&taishin_tag_55[i*2],2);
								idx = idx + 2;
							}
						}
						else if (gl_Credit_Host == 6)
						{
							if(ctbc_tag_55[i*2] == 0x00)
							{
								gl_rinit.data[idx] = ctbc_tag_55[i*2 + 1];
								idx = idx + 1;
							}
							else
							{
								memcpy(&gl_rinit.data[idx],&ctbc_tag_55[i*2],2);
								idx = idx + 2;
							}
						}
					}
#else
					if(emv_tag_55[i*2] == 0x00)
					{
						gl_rinit.data[idx] = emv_tag_55[i*2 + 1];
						idx = idx + 1;
					}
					else
					{
						memcpy(&gl_rinit.data[idx],&emv_tag_55[i*2],2);
						idx = idx + 2;
					}
#endif

					//2-�A��J����
					gl_rinit.data[idx] = buf[0];
					idx = idx + 1;

					//3-�̫��J���e
					memcpy(&gl_rinit.data[idx],&buf[2],buf[0]);
					idx = idx + buf[0];

#if VERSION_MODE != VERSION_NCCC
					if (gl_Credit_Host == 4)
					{
						//	Black	2011/03/18
//						if((fisc_tag_55[i*2] == 0x00) && (fisc_tag_55[i*2 + 1] == 0x95))	//rex 2009/03/23
						//	Black	2018/09/21	[AMEX]
						if ((fisc_tag_55[i*2] == 0x00) && (fisc_tag_55[i*2 + 1] == 0x95) && (AmexHost() == FALSE))
						{
							//�ˬd TAG 95 (TVR) �M�w TAG DFEF ����
							memcpy(tempa,&buf[2],5);
							if (tempa[3] & 0x10)
								emv_tag_DFEF[4] = 0x03;				//1503 : CAD Random Selection
							else if (gl_fall_back_flag == 1)
								emv_tag_DFEF[4] = 0x04;				//1504 : CAD not able to process ICC
							else if (tempa[3] & 0x8)
								emv_tag_DFEF[4] = 0x10;				//1510 : Over Floor Limit
						}
					}
#endif
				}
			}
		}

		if ((UINT)(idx + 9) > sizeof(gl_rinit.data))		//	Black	2013/01/17	������d��55���ȶW1000
		{
			memcpy((UCHAR *)&gl_rsp_data.rspcode, "B3", 2);
			return FAIL;
		}

#if VERSION_MODE != VERSION_NCCC
		//	Black	2021/10/21
		if ((gl_Credit_Host == 0) || (gl_Credit_Host == 1))
		{
			//	Black	2016/11/16
			memcpy(&gl_rinit.data[idx] , "\xDF\xED\x01\x01" , 4);
			idx = idx + 4;

			if (gl_tx_data.emv_flag == 2)
				memcpy(&gl_rinit.data[idx] , "\xDF\xEF\x02\x15\x04" , 5);
			else
				memcpy(&gl_rinit.data[idx] , "\xDF\xEF\x02\x15\x10" , 5);
			idx = idx + 5;
		}
		else if (gl_Credit_Host == 3)
		{
			memcpy(&gl_rinit.data[idx] , "\xDF\xED\x01\x31" , 4);
			idx = idx + 4;

			memcpy(&gl_rinit.data[idx] , "\xDF\xEE\x01\x35" , 4);
			idx = idx + 4;

			memcpy(&gl_rinit.data[idx] , "\xDF\xEF\x02\x15\x00" , 5);
			idx = idx + 5;
		}
		else if (gl_Credit_Host == 4)
		{
			//	Black	2011/03/18
			if (gl_chip_flag == TRUE)
				emv_tag_DFED[3] = '1';
			else
				emv_tag_DFED[3] = '2';

			memcpy(&gl_rinit.data[idx],emv_tag_DFED,4);	//rex 2009/03/23 �[�J TAG DFED ------------
			idx = idx + 4;

			memcpy(&gl_rinit.data[idx],emv_tag_DFEF,5);	//�[�J TAG DFEF ----------------------------
			idx = idx + 5;
		}
#else
		//rex 2020/11/26 [NCCC]
		memcpy(&gl_rinit.data[idx] , "\xDF\xEE\x01\x05" , 4);
		idx = idx + 4;

		if (gl_tx_data.emv_flag == 2)	//FallBack 
			memcpy(&gl_rinit.data[idx] , "\xDF\xEF\x02\x15\x04" , 5);
		else
			memcpy(&gl_rinit.data[idx] , "\xDF\xEF\x02\x15\x10" , 5);
		idx = idx + 5;

		if (gl_tx_data.emv_flag == 2)	//FallBack 
		{
			memcpy(&gl_rinit.data[idx] , "\xDF\xED\x01\x01" , 4);
			idx = idx + 4;
		}
#endif

		gl_rinit.length = idx;

		break;

	case 5:	//-5.----Get Field_56 Data from IC Card-----
#if VERSION_MODE != VERSION_NCCC
		idx = 0;
		for(i = 0 ; i < 10 ; i++)
		{
			EMV_result = api_emvk_GetDataElement(emv_tag_56[i*2],emv_tag_56[i*2 + 1],(UCHAR *)&buf);
			if(EMV_result == emvOK)
			{
				if(buf[0] != 0)
				{
					if(emv_tag_56[i*2] == 0x00)
					{
						gl_rinitrsp.data[idx] = emv_tag_56[i*2 + 1];
						idx = idx + 1;
					}
					else
					{
						memcpy(&gl_rinitrsp.data[idx],&emv_tag_56[i*2],2);
						idx = idx + 2;
					}
					gl_rinitrsp.data[idx] = buf[0];
					idx = idx + 1;
					memcpy(&gl_rinitrsp.data[idx],&buf[2],buf[0]);
					idx = idx + buf[0];
				}
			}
		}
		gl_rinitrsp.length = idx;
#endif
		break;

	case 6:	//-6.----Get TC value from IC Card----------
		EMV_result = api_emvk_GetDataElement(0xDF,0xE1,(UCHAR *)&buf);
		if(EMV_result == emvFailed || buf[0] == 0)
		{
			memcpy(dispbuf,"60",2);

			ClrDisplayDN(LINE2);
			DispText(dispbuf,LINE2,0,2,FONT1);
			EMV_Message(8,0);
			return FAIL;
		}

		memcpy(gl_input.TC_value,&buf[2],8);
		break;

	//	Black	2023/04/06	[AMEX]�x�sAP Label(�ϥ�charge_opt��flexible_payment)
	case 7:	//-7.----Get AP Label from IC Card----------
		memset(gl_input.charge_opt,0,sizeof(gl_input.charge_opt));
		EMV_result = api_emvk_GetDataElement(0x00,0x50,(UCHAR *)&buf);
		if((EMV_result == emvOK) && (buf[0] != 0))
		{
			if(buf[0]>16)	//alex 2013/05/24 �s�WAP Label �ק慨��AP Label�W�L16bytes(�u�L�e16)
			{
				gl_input.charge_opt[0]=16;
				memcpy(&gl_input.charge_opt[1],&buf[2],16);
			}
			else
			{
				gl_input.charge_opt[0]=buf[0];
				memcpy(&gl_input.charge_opt[1],&buf[2],buf[0]);
			}
		}
		break;

	default:
		break;
	} // ==== end of switch(action) ====
	return SUCCEED;
}
#endif

/* ----------------------------------------------
Name:	StoreEmvData
Funct:	�qSRAMŪ�gField_55�PField_56���
Input:	action		=>	0:�x�sField_55
						2:Ū��Field_55
		PAGE		=>	8:CTCB
						10:REDEEM
		record_num	=>	offset
Output: SUCCEED : ���\.
		FAIL : ����
Global:	&gl_rinit		=>	�ȦsField_55
------------------------------------------------*/
#if 0
void	StoreEmvData(UCHAR action,UCHAR	PAGE, UINT record_num)
{
	struct write_data
	{
		UCHAR length;
		UCHAR data[255];
	}__attribute__((packed));;
	typedef struct write_data WRITE_DATA;
	WRITE_DATA data1;
	EMV_DATA data;

	if(action == 0)			// ==== �x�sField_55 ====
	{
		//	Black	2016/11/29
		// ==== �����ƶW�Lbuffer ====
//		if(gl_rinit.length > 255)
//			data.len1 = 255;
//
//		data1.length = gl_rinit.length;
//		memcpy(&data1.data,&gl_rinit.data,gl_rinit.length);
		if(gl_rinit.length > 255)
			data1.length= 255;
		else
			data1.length = gl_rinit.length;
		memcpy(&data1.data,&gl_rinit.data,data1.length);

		ReadWritePage(PAGE+WRITE_PAGE, (UCHAR *)&data1, record_num * EMV_DATA_LEN, EMV_DATA_LEN);
		return;
	}
	else
	{
		// ==== Echo 2015/4/20 �۰����u���v,�����Ƭ�FFFF��,����Ū��������� ====
		if(record_num == 0xFFFF)
		{
			gl_rinitrsp.length = 0;
			return;
		}

		ReadWritePage(PAGE+READ_PAGE, (UCHAR *)&data, record_num * EMV_DATA_LEN, EMV_DATA_LEN);

		if(action == 2)			// ==== Ū��Field_55 ====
		{
			gl_rinit.length = data.len1;
			memcpy(&gl_rinit.data,&data.Field_55,gl_rinit.length);
		}
		return;
	}
}
#endif

/* ----------------------------------------------	//	Black	2011/03/28	��ηs��k
Name:	StoreEmvData
Funct:	�qSRAMŪ�gField_55�PField_56���
Input:	action		=>	0:�x�sField_55
						1:�x�sField_56
						2:Ū��Field_55
						3:Ū��Field_56
		PAGE		=>	8:NCCC
						10:REDEEM
		record_num	=>	offset
Output: SUCCEED : ���\.
		FAIL : ����
Global:	&gl_rinit		=>	�ȦsField_55
		&gl_rinitrsp	=>	�ȦsField_56
alex 09/08/20 �ק�EMV���D
------------------------------------------------*/
/*void StoreEmvData(UCHAR action,UCHAR PAGE, UINT record_num)
{
	UCHAR	result;
	struct emv_data{
		UCHAR	F55_len;
		UCHAR	F55_data[255];	//alex 09/11/18 320AGR �W�[ field55 �x�s�Ŷ�
	}__attribute__((packed));;
	struct emv_data EMV_DATA;

	switch(action)
	{
		case 0:// �x�sField_55
			EMV_DATA.F55_len = gl_rinit.length;
			memcpy(EMV_DATA.F55_data, gl_rinit.data, EMV_DATA.F55_len);	//alex 09/11/25
			result= ReadWritePage(PAGE+WRITE_PAGE, &EMV_DATA.F55_len, record_num * sizeof(EMV_DATA), sizeof(EMV_DATA.F55_data)+1);
			return ;
			break;

		case 2:// Ū��Field_55
			result= ReadWritePage(PAGE+READ_PAGE, &EMV_DATA.F55_len, record_num * sizeof(EMV_DATA), sizeof(EMV_DATA));
			if(action == 2)			// Ū��Field_55
			{
				gl_rinit.length = EMV_DATA.F55_len;
				memcpy(gl_rinit.data,EMV_DATA.F55_data,EMV_DATA.F55_len);
			}
			return ;
			break;
	}
}*/

/* ----------------------------------------------
Name:	EMV_CB_OnlineProcessing
Funct:	��IC�d�P�_�ݭn�s�u��,API�|�ӨϥΦ��{��
Input:	None
Output: *arc:Respones Code => gl_rsp_data.rspcode
		*rmsg:Field_55 => gl_txmsg.content[0~299]
			!!�ѩ�API�����u�d300 Bytes�ӦsFeild_55�����,�ҥHAP�u��^��300 Bytes�����!!
Global:	
Black	2018/09/21	[AMEX]POS Entry Mode
rex		2021/05/19	[NCCC][MPAS][PK24]
rex 	2022/09/12	[NCCC][PIN���X��1]
------------------------------------------------*/
UCHAR EMV_CB_OnlineProcessing( UCHAR *arc, UCHAR *air, UCHAR *rmsg )
{
#if 0
	UCHAR	connecting;
	UCHAR	retry, i;
	UCHAR	result, EMV_result;
	UCHAR	buf[2] = {0x00,0x00};

	// ==== Echo 2014/6/18 ����JPIN POS Entry Mode = 0x51,�S��JPIN POS Entry Mode = 0x52 ====
	// ==== Echo 2014/6/18 ���EMV Online PIN,POS Entry Mode�n���b�s�u�e ====
	//	Black	2018/09/21	[AMEX]POS Entry Mode
#if VERSION_MODE != VERSION_NCCC
	if (AmexHost() == TRUE)
	{
		gl_input.entry_mode[0] = 0x05;
//		gl_input.entry_mode[1] = 0x51;
		gl_input.entry_mode[1] = 0x52;
	}
	else
#endif
	{
//		gl_input.entry_mode[0] = 0;
//		if(gl_pin_entry == 1)
//			gl_input.entry_mode[1] = 0x51;
//		else
//			gl_input.entry_mode[1] = 0x52;
//remark by rex 2022/09/12 [NCCC][PIN���X��1]
#if VERSION_MODE != VERSION_NCCC
		gl_input.entry_mode[0] = 0;
		if(gl_pin_entry == 1)
			gl_input.entry_mode[1] = 0x51;
		else
			gl_input.entry_mode[1] = 0x52;
#else	//NCCC
		gl_input.entry_mode[0] = 0;
		if (gl_input.redeem_flag == 3)	//CUP...
		{
			if(gl_pin_entry == 1)		// pin...
				gl_input.entry_mode[1] = 0x51;
			else
				gl_input.entry_mode[1] = 0x52;	//CUP + No PIN
		}
		else
			gl_input.entry_mode[1] = 0x51;	//non CUP...
#endif
	}

	if(gl_lan_port.status == 0 || gl_lan_port.status == 4)
	{
		do
		{
			api_mdm_status(0,&connecting);
		}while(connecting == 0x80);

		if((connecting == 0x02) || (connecting == 0x01))
			connecting = SUCCEED;
		else
			connecting = FAIL;
	}

	ClrDisplayDN(LINE2);
//	if(gl_lan_port.status == 2)
	//	Black	2019/01/30
	if ((gl_lan_port.status == 2) || (gl_lan_port.status == 5))
	{
		connecting = IP_ConnectionOK(0,1);	// ==== [TMS] Echo 2014/8/27 ====
		if (connecting == FAIL)
		{
			//	Black	2020/09/08	���ϥ�
//			// ==== Echo 2015/6/15 �d�߬O�_�ŦX���u���v���� ====
//			result = Auto_Offline_Approve(0,SALE_UNDER_LMT,gl_input.tot_amount,0);
//			if(result == SUCCEED)
//			{
//				memcpy(gl_rsp_data.rspcode,"AF",2);	// ==== ���ܦ۰ʱ��v ====
//				return emvFailed;
//			}

			// ==== Echo 2015/6/15 �����ƴ��}�� ====
			if(gl_ip_change_dial_sw == 0)
				return emvFailed;

			//	Black	2019/01/30	�s�W4G
			if (gl_lan_port.status == 2)
				gl_lan_port.status = 3;	// ==== TCP/IP��Dial
			else if (gl_lan_port.status == 5)
				gl_lan_port.status = 6;	// ==== 4G��Dial

			if (!EMV_ConnectionOK(NOT_CONNECTING, (TEL_STRU *)&gl_cur_acquirer.pri_trans_tel))
				return emvFailed;

			// ==== Echo 2015/4/20 �۰����u���v ====
/*			memcpy(gl_rsp_data.rspcode,"TO",2);
			result = NoMsg_ConnectionOK(NOT_CONNECTING, (TEL_STRU *)&gl_cur_acquirer.pri_trans_tel);
			if(result == 1)			// ==== �s�u���� ====
			{
				memcpy(gl_rsp_data.rspcode,"AF",2);	// ==== ���ܦ۰ʱ��v ====
				return emvFailed;
			}
			else if(result == 2)	// ==== Echo 2015/5/13 ��ʨ����s�u ====
				return emvFailed;
*/		}
	}
	else
	{
		if (!EMV_ConnectionOK(connecting, (TEL_STRU *)&gl_cur_acquirer.pri_trans_tel))
			return emvFailed;

		//	Black	2020/09/08	���ϥ�
//		// ==== Echo 2015/4/20 �۰����u���v ====
//		memcpy(gl_rsp_data.rspcode,"TO",2);
//		result = NoMsg_ConnectionOK(connecting, (TEL_STRU *)&gl_cur_acquirer.pri_trans_tel);
//		if(result == 1)			// ==== �s�u���� ====
//		{
//			memcpy(gl_rsp_data.rspcode,"AF",2);	// ==== ���ܦ۰ʱ��v ====
//			return emvFailed;
//		}
//		else if(result == 2)	// ==== Echo 2015/5/13 ��ʨ����s�u ====
//			return emvFailed;
	}

	Reset_offline_approve_count();	// ==== Echo 2015/5/13 ���u���v�p�ƾ��k0 ====

	if (!ReversalProc())	// ==== No Pending Trans or Pending has just been sent ====
	{
		// ==== Echo 2014/5/1 ���`SAM Slot-1���������]���ݰ�ATR ====
		// ==== Echo 2014/5/1 �}��SAM�d�[�K ====
//		if(CubHost() == TRUE && (gl_lan_port.status == 2 || gl_lan_port.status == 3 || gl_lan_port.status == 4))
//			IFM_SAM_Close();		// ==== ����SAM2 ====

#if VERSION_MODE != VERSION_NCCC
		//	Black	2023/01/06	����H�U
		if (gl_Credit_Host == 6)
		{
			// ==== [SELF] Echo 2019/2/13 �D���^67,�N�Nkey�M�� ====
			if((gl_self_type == 1) && (gl_soft_encrypt == 2) && !memcmp(gl_rsp_data.rspcode,"67",2))
			{
				if(RedeemHost() == TRUE)
					gl_ctcb_key_table.masterkey_status[1][0] = 0;
				else if(InstallmentHost() == TRUE)
					gl_ctcb_key_table.masterkey_status[2][0] = 0;
				else if(CupHost() == TRUE)
					gl_ctcb_key_table.masterkey_status[3][0] = 0;
				else if(CtbcHost() == TRUE)
					gl_ctcb_key_table.masterkey_status[0][0] = 0;
			}
		}
#endif

		return emvFailed;
	}

	gl_tx_data.trans = gl_input.trans;	//	Black	2012/05/24	�s�W�]�w������O

	// ==== Ū��Field_55��� ====
	EMV_result = GetEmvData(4);
	if(EMV_result == FAIL)
	{
		// ==== Echo 2014/5/1 ���`SAM Slot-1���������]���ݰ�ATR ====
		// ==== Echo 2014/5/1 �}��SAM�d�[�K ====
//		if(CubHost() == TRUE && (gl_lan_port.status == 2 || gl_lan_port.status == 3 || gl_lan_port.status == 4))
//			IFM_SAM_Close();		// ==== ����SAM2 ====

		return emvFailed;
	}

	IncRocNum();
	do
	{
		retry=FALSE;
		PatTxCommonCode();
		PatProcCode(gl_tx_data.trans);

#if VERSION_MODE != VERSION_NCCC
		DecRocNum();
#else
//remark by rex 2020/11/26 [NCCC]�p�X���Φ��^�h 

		//rex 2020/11/26 [NCCC]	�n�ۦ沣�� RRN 
		make_ref();
		memcpy((UCHAR *)&gl_tx_data.rrn,gl_input.rrn,12);	
#endif

		gl_rinitrsp.length = 0;	// ==== �N��Field_55��ƪ�buffer���ײM��0 ====
//debug(0x45,gl_txmsg.content,40,120);

		result = SendISO(REV_PENDING);

//debug(0x46,gl_txmsg.content,40,120);
		for(i = 0 ; i < 2 ; i++)
			*arc++ = gl_rsp_data.rspcode[i];

		if(gl_rinitrsp.length == 0)
		{
			for(i = 0 ; i < 2 ; i++)
				*rmsg++ = buf[i];
		}
		else
		{
			for(i = 0 ; i < gl_rinitrsp.length ; i++)
				*rmsg++ = gl_rinitrsp.data[i];
		}

		if(result == APPROVED)
		{

#if VERSION_MODE == VERSION_NCCC && NCCC_MODE == NCCC_PARK24
			if (!(gl_rsp_data.status & MPAS_Indicator))	//	Black	2019/01/08	[����]
			{//rex 2021/05/19 [NCCC][MPAS][PK24] 	�ɧ� �Y�L MP���v�n�ڵ��A�U���R�� 
				DispRspText_MP();
				return emvFailed;
			}
#endif

			memcpy(air,gl_rsp_data.auth_code,AUTH_CODE_LEN); //alex 2011/02/18 
			UpdateHostStatus(NO_PENDING);	// ==== Signal to no pending for current acquirer ====

			// ==== Echo 2014/5/1 ���`SAM Slot-1���������]���ݰ�ATR ====
			// ==== Echo 2014/5/1 �}��SAM�d�[�K ====
//			if(CubHost() == TRUE && (gl_lan_port.status == 2 || gl_lan_port.status == 3 || gl_lan_port.status == 4))
//				IFM_SAM_Close();		// ==== ����SAM2 ====

			return emvOK;
		}
		else if(result == NOT_APPROVED)
		{
			UpdateHostStatus(NO_PENDING);	// ==== Signal to no pending for current acquirer ====

			// ==== Echo 2014/5/1 ���`SAM Slot-1���������]���ݰ�ATR ====
			// ==== Echo 2014/5/1 �}��SAM�d�[�K ====
//			if(CubHost() == TRUE && (gl_lan_port.status == 2 || gl_lan_port.status == 3 || gl_lan_port.status == 4))
//				IFM_SAM_Close();		// ==== ����SAM2 ====

#if VERSION_MODE != VERSION_NCCC
			//	Black	2023/01/06	����H�U
			if (gl_Credit_Host == 6)
			{
				// ==== [SELF] Echo 2019/1/17 �D���^67,�N�Nkey�M�� ====
				if((gl_self_type == 1) && (gl_soft_encrypt == 2) && !memcmp(gl_rsp_data.rspcode,"67",2))
				{
					if(RedeemHost() == TRUE)
						gl_ctcb_key_table.masterkey_status[1][0] = 0;
					else if(InstallmentHost() == TRUE)
						gl_ctcb_key_table.masterkey_status[2][0] = 0;
					else if(CupHost() == TRUE)
						gl_ctcb_key_table.masterkey_status[3][0] = 0;
					else if(CtbcHost() == TRUE)
						gl_ctcb_key_table.masterkey_status[0][0] = 0;
				}
			}
#endif

			return emvOK;
		}
		else if(result == TRANS_FAILED)
		{
			// ==== Echo 2014/5/1 ���`SAM Slot-1���������]���ݰ�ATR ====
			// ==== Echo 2014/5/1 �}��SAM�d�[�K ====
//			if(CubHost() == TRUE && (gl_lan_port.status == 2 || gl_lan_port.status == 3 || gl_lan_port.status == 4))
//				IFM_SAM_Close();		// ==== ����SAM2 ====

			return emvFailed;
		}
		else
		{
			// ==== Echo 2014/5/1 ���`SAM Slot-1���������]���ݰ�ATR ====
			// ==== Echo 2014/5/1 �}��SAM�d�[�K ====
//			if(CubHost() == TRUE && (gl_lan_port.status == 2 || gl_lan_port.status == 3 || gl_lan_port.status == 4))
//				IFM_SAM_Close();		// ==== ����SAM2 ====

			return emvFailed;
		}

	} while (retry);

	// ==== Echo 2014/5/1 ���`SAM Slot-1���������]���ݰ�ATR ====
	// ==== Echo 2014/5/1 �}��SAM�d�[�K ====
//	if(CubHost() == TRUE && (gl_lan_port.status == 2 || gl_lan_port.status == 3 || gl_lan_port.status == 4))
//		IFM_SAM_Close();		// ==== ����SAM2 ====

	return emvFailed;
#endif
}

/* ----------------------------------------------
Name:	EMV_CB_OnlineProcessing
Funct:	��IC�d�P�_��Call Bank��,API�|�ӨϥΦ��{��
Input:	None
Output: emvOK		=> approve
		emvFailed	=> declined
Global:	
------------------------------------------------*/
UCHAR	EMV_CB_ReferralProcessing(void)
{
#if 0
	//	Black	2020/08/26	���عq�H���LCall Bank
//	//	Black	2016/12/12	�u���H�Υd�B������Call Bank
//	if((RedeemHost() == TRUE) || (gl_input.redeem_flag == 3) || (gl_input.trans == AUTH_SWIPE) || ChkAcquirerOption(28))
//		return emvFailed;
//
//	if(gl_input.trans == SALE_SWIPE)
//	{
//		LongBeep();
//		if(ReferralProc() == SUCCEED)
//			return emvOK;
//		else
//			return emvFailed;
//	}
//	else
		return emvFailed;
#endif
}

/* ----------------------------------------------
Name:	EMV_CB_OnlineProcessing
Funct:	��IC�d�P�_�ݭn�d�ߥ����,API�|�ӨϥΦ��{��
Input:	*pan:�����d��
Output: *amt:�ھڥd���ҥ�������B
		return: emvOK		=> data found
				emvFailed	=> data not found
Global:	
Black	2021/07/08	[AMEX]�s�WAE
------------------------------------------------*/
UCHAR	EMV_CB_FindMostRecentPAN(UCHAR *pan,UCHAR *amt)
{
#if 0
	UCHAR	amount[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
	int		idx = 0;		//	Black	2012/05/17
	int		j;
	RECORD	record;
	UCHAR	BATCH_NO = 0;	//	Black	2012/05/17

#if VERSION_MODE != VERSION_NCCC
	// ==== Echo 2014/5/4 �W�[UICC��� ====
	if(RedeemHost() == TRUE)
	{
		idx = gl_record_ptr_R;
		BATCH_NO = REDEEM_PAGE1;
	}
	else if(InstallmentHost() == TRUE)
	{
		idx = gl_record_ptr_I;
		BATCH_NO = INST_PAGE1;
	}
	//	Black	2022/01/11	���p�d
	else if(CupHost() == TRUE)
	{
		idx = gl_record_ptr_U;
		BATCH_NO = CUP_PAGE1;
	}
	else if(AmexHost() == TRUE)	//	Black	2021/07/08	[AMEX]
	{
		idx = gl_record_ptr_A;
		BATCH_NO = AMEX_PAGE1;
	}
	else
#endif
	{
		idx = gl_record_ptr_C;
		BATCH_NO = CREDIT_PAGE1;
	}

	for(j = idx - 1 ; j >= 0 ; j -- )
	{
		ReadWriteRecord(BATCH_NO+READ_PAGE, (UCHAR *)&record, j);
		if(!memcmp(pan,record.pan,10))
		{
			memcpy(amount,&record.tot_amount[1],5);
			memcpy(amt,amount,6);
			return emvOK;
		}
	}
	return emvFailed;
#endif
}

/* ----------------------------------------------
Name:	EMV_CB_ShowMsg_ENTER_PIN
Funct:	IC�d�P�_�ݭn�VPIN��,API�|�ӨϥΦ��{��
Input:	*amt:��������ܤ����B 
Output:
Programer: rex 2010/03/19
LastUpdate: rex 2010/03/23
------------------------------------------------*/
void  EMV_CB_ShowMsg_ENTER_PIN(UCHAR *amt)
{
    /*
	UCHAR	msg_input_pinpad[15] = {0xBD,0xD0,0xC5,0x55,0xAB,0xC8,0xBF,0xE9,0xA4,0x4A,0xB1,0x4B,0xBD,0x58,':'};	// ==== ���U�ȿ�J�K�X: ====
	UCHAR	msg_amount[4] = {0XAA,0XF7,0XC3,0X42};	// ==== ���B ====

	UCHAR	amount[13],len=0;
	
	len = *amt;
	memcpy(amount,&amt[1],len);

	if(gl_input.redeem_flag == 3)
		DispCUPHeader(1);
	else
		DispHeader(1);
	DispText(msg_amount,LINE2,0,4,FONT2);				// ==== ���B ====
	DispText(amount,LINE2,(16-len),len,FONT1);			// ==== ��ܪ��B ====
	DispText(msg_input_pinpad,LINE3,0,14,FONT2);		// ==== ���U�ȿ�J�K�X ====

	FlushKey();	// ==== Echo 2014/7/14 ��JPIN�e���NKeyboard���Ȧs�M�� ====
    */
}

/* ----------------------------------------------
Name:	EMV_CB_ShowMsg_ENTER_PIN_BY_EXT_PINPAD
Funct:	IC�d�P�_�ݭn�VPIN��,API�|�ӨϥΦ��{��
Input:	
Output:
Echo 2014/6/24 �����ܵe��
------------------------------------------------*/
void  EMV_CB_ShowMsg_ENTER_PIN_BY_EXT_PINPAD()
{
    /*
	UCHAR	msg_input_pinpad[15] = {0xBD,0xD0,0xC5,0x55,0xAB,0xC8,0xBF,0xE9,0xA4,0x4A,0xB1,0x4B,0xBD,0x58,':'};	// ==== ���U�ȿ�J�K�X: ====
		
	if(gl_input.redeem_flag == 3)
		DispCUPHeader(1);
	else
		DispHeader(1);

	DispText(msg_input_pinpad,LINE3,0,15,FONT2);	// ==== ���U�ȿ�J�K�X: ====
    */
}

/* ----------------------------------------------
Name:	EMV_CB_ShowMsg_PIN_OK
Funct:	IC�d�P�_�ݭn�VPIN��,API�|�Өϥ���ܰT�� 
Input:	
Output:
Programer: rex 2010/03/19
Black	2018/09/21	[AMEX]�s�W�O������JOffline PIN
------------------------------------------------*/
void  EMV_CB_ShowMsg_PIN_OK()
{
    /*
	UCHAR	Cmsg[]={0XBF,0XE9,0XA4,0X4A,0XA5,0XBF,0XBD,0X54};//��J���T. 
	UCHAR	Emsg[]={"PIN OK"};
		
	if(gl_input.redeem_flag == 3)
		DispCUPHeader(1);
	else
		DispHeader(1);
	DispText(Cmsg,LINE2,4,8,FONT2);//��J���T. 
	DispText(Emsg,LINE3,5,6,FONT1);
	Short2Beep();

	//	Black	2018/09/21	[AMEX]
	gl_pin_entry = 2;	//	����J�K�X(Offline PIN���W��)
    */
}

/* ----------------------------------------------
Name:	EMV_CB_ShowMsg_INCORRECT_PIN
Funct:	IC�d�P�_�ݭn�VPIN��,API�|�Өϥ���ܰT�� 
Input:	
Output:
Programer: rex 2010/03/19
------------------------------------------------*/
void  EMV_CB_ShowMsg_INCORRECT_PIN()
{
    /*
	UCHAR	Cmsg[]={0XBF,0XE9,0XA4,0X4A,0XB1,0X4B,0XBD,0X58,0XBF,0XF9,0XBB,0X7E};//��J�K�X���~. 
	UCHAR	Emsg[]={"INCORRECT PIN"};
	
	if(gl_input.redeem_flag == 3)
		DispCUPHeader(1);
	else
		DispHeader(1);
	DispText(Cmsg,LINE2,2,12,FONT2);//��J�K�X���~. 
	DispText(Emsg,LINE3,2,13,FONT1);
	ErrorBeep();
    */
}

/* ----------------------------------------------
Name:	EMV_CB_ShowMsg_LAST_PIN_TRY
Funct:	IC�d�P�_�ݭn�VPIN��,API�|�Өϥ���ܰT�� 
Input:	
Output:
Programer: rex 2010/03/19
------------------------------------------------*/
void  EMV_CB_ShowMsg_LAST_PIN_TRY()
{
    /*
	UCHAR	Cmsg[]={0XB3,0XCC,0XAB,0XE1,0XA4,0X40,0XA6,0XB8,0XBF,0XE9,0XA4,0X4A};//�̫�@����J.
	UCHAR	Emsg[]={"LAST PIN TRY"};
	
	if(gl_input.redeem_flag == 3)
		DispCUPHeader(1);
	else
		DispHeader(1);
	DispText(Cmsg,LINE2,2,12,FONT2);//�̫�@����J.
	DispText(Emsg,LINE3,2,12,FONT1);
	LongBeep();
    */
}

/* ----------------------------------------------
Name:	EMV_CB_ShowMsg_PLEASE_WAIT
Funct:	IC�d�P�_�ݭn�VPIN��,API�|�Өϥ���ܰT�� 
Input:	
Output:
Programer: rex 2010/03/19
------------------------------------------------*/
void  EMV_CB_ShowMsg_PLEASE_WAIT()
{
    /*
	if(gl_input.redeem_flag == 3)
		DispCUPHeader(1);
	else
		DispHeader(1);
	DispText((UCHAR *)msg_connecting_1,LINE3,0,LEN_DISP,FONT2);	// ==== �B�z�� ====
    */
}

// -------------------------------------------------------------------------------------------------
// FUNC  : Setup parameters for AS350 internal PIN entry function.
// INPUT : row	   - row number of display for PIN entry.
//	   col	   - beginning column number of display for PIN entry.
//	   palette - fixed 3 bytes palette values of RGB.
// OUTPUT: none.
// RETURN: none.
// NOTE  : This function is to be implemented from AP level.
// -------------------------------------------------------------------------------------------------
void	EMV_CB_SetupPinPad( API_LCDTFT_PARA *para )
{
    /*
	ULONG FG_color;
	ULONG BG_color;
	UCHAR	buf[4];
	
	para->Row	= 6;
	para->Col	= 2;
	para->Font	= LCDTFT_FONT1+attrCLEARWRITE;
	para->RGB	= RGB_BPP16;
	para->FontHeight	= TFTLCD_FONT1_H;
	para->FontWidth		= TFTLCD_FONT1_W;
	para->CCWdegrees	= 0;

	FG_color = color_black;
	memcpy(buf,(UCHAR*)&FG_color,4);
	para->FG_Palette[0] = buf[2];
	para->FG_Palette[1] = buf[1];
	para->FG_Palette[2] = buf[0];

//	BG_color = color_blue2;
	BG_color = color_white;
	memcpy(buf,(UCHAR*)&BG_color,4);
	para->BG_Palette[0] = buf[2];
	para->BG_Palette[1] = buf[1];
	para->BG_Palette[2] = buf[0];

	glo_PINPAD = 1;	//	Black	2016/01/12	�]�����ϥ�PINPAD
    */
}

/* ----------------------------------------------
Name:	EMVOpen
Funct:	�}��EMV Reader
Input:	None
Output: 
Global:	
------------------------------------------------*/
#if 0
void	EMVOpen(void)
{
	api_emvk_OpenSessionICC();
}
#endif

/* ----------------------------------------------
Name:	EMVClose
Funct:	����EMV Reader
Input:	None
Output: 
Global:	
------------------------------------------------*/
#if 0
void	EMVClose(void)
{
	api_emvk_CloseSessionICC();
}
#endif

/* ----------------------------------------------
Name:	EMV_Get_Candidate_List
Funct:	��ܻP��JCandidate List
Input:  None
Output: 
Global:	
Black	2018/09/21	[AMEX]AE�w���v�Aapi_emvk_InitiateApplication()�ݶǤJ0x30
Black	2018/09/21	[AMEX]�s�W�x�sAID
Black	2021/01/14	�אּ�@����ܤE��AID
Black	2023/05/31	�אּ�@����ܤE��AID
------------------------------------------------*/
#if 0
UCHAR EMV_Get_Candidate_List(void)
{
	struct  LIST_ITEM
	{		
		UCHAR	link;
		UCHAR	len;
		UCHAR	name[16];
	}__attribute__((packed));;
	typedef struct LIST_ITEM list_items;

	UCHAR	EMV_result;
	UCHAR	list_cnt, list_no;
	list_items	list_item[16];
	UCHAR	count[2] = {" ."};
	UCHAR	i;
//	UCHAR	j;	//	Black	2021/01/14	�אּ�@����ܤE��AID
	UCHAR	result;
//	UCHAR	pos, pos1;	//	Black	2021/01/14	�אּ�@����ܤE��AID
//	UCHAR	p_up[1] = {0x93};		//	Black	2021/01/14	�אּ�@����ܤE��AID
//	UCHAR	p_down[1] = {0x94};		//	Black	2021/01/14	�אּ�@����ܤE��AID
//	UCHAR	p_up_down[1] = {0x92};	//	Black	2021/01/14	�אּ�@����ܤE��AID
	UCHAR	keyin;
//	UCHAR	kbdSet[5] = {0x1C,0x1C,0x1C,0x24,0x00};
//	UCHAR	kbdSet[5] = {0x3C,0x1C,0x3C,0x04,0x00};	//	1~9,��,��,CANCEL
	UCHAR	kbdSet[5] = {0x1C,0x1C,0x1C,0x04,0x00};	//	1~9,CANCEL	//	Black	2021/01/14	�אּ�@����ܤE��AID
	UCHAR	msg_emv_process[LEN_DISP] = {' ',0xB4,0xB9,0xA4,0xF9,0xA5,0x64,0xB3,0x42,0xB2,0x7A,0xA4,0xA4,'.','.','.'};	// ==== �����d�B�z�� ... ====
	UCHAR	dispbuf[2] = {"00"};
//	UCHAR	msg_inquire[LEN_DISP] = {0xC2,0x49,0xBC,0xC6,0xAC,0x64,0xB8,0xDF,' ',' ',' ',' ',' ',' ',' ',' '};	// ==== �I�Ƭd�� ====
	UCHAR	msg_choice_app[LEN_DISP] = {0xBD,0xD0,0xBF,0xEF,0xBE,0xDC,0xA5,0xE6,0xA9,0xF6,0xA4,0xE8,0xA6,0xA1,':',' '};	// ====�п�ܥ���覡: ====
	UCHAR	sw1,sw2;
	UCHAR	buf[200];	//	Black	2018/09/21	[AMEX]

	forever
	{
		//-------------------------1.���Candidate List--------------------------------------
		//	Black	2016/11/25	�����[�t
//		if(EMV_status_check() == FAIL)
//			return(FAIL);

		EMV_result = api_emvk_GetCandidateList((UCHAR *)&list_cnt,(UCHAR *)&list_item);
		if(list_cnt > 9)	// ==== ����W�L9�ӥH�W��AID ====
			list_cnt = 9;
		//	Black	2021/01/14	[���ձM��]
//		list_cnt = 9;
//		list_item[4].len = 5;
//		memcpy(list_item[4].name, (UCHAR *)"55555", list_item[4].len);
//		list_item[5].len = 6;
//		memcpy(list_item[5].name, (UCHAR *)"666666", list_item[5].len);
//		list_item[6].len = 7;
//		memcpy(list_item[6].name, (UCHAR *)"7777777", list_item[6].len);
//		list_item[7].len = 8;
//		memcpy(list_item[7].name, (UCHAR *)"88888888", list_item[7].len);
//		list_item[8].len = 9;
//		memcpy(list_item[8].name, (UCHAR *)"999999999", list_item[8].len);

		gl_devkbd = api_kbd_open(DEV_KBD,kbdSet);

		//	Black	2021/01/14	�אּ�@����ܤE��AID
//		pos = list_cnt / 4;
//		if( (list_cnt%4) != 0 )
//			pos = pos + 1;

		// �ۧU����������B�۰ʿ��AID By Sam 2020/09/07
		if(EMV_result == emvOK && !gl_self_type)
		{
			result = 1;
//			pos1 = 0;	//	Black	2023/05/31	�אּ�@����ܤE��AID
			ClrDisplayDN(LINE2);	//	Black	2021/01/14	�אּ�@����ܤE��AID
			DispText(msg_choice_app,LINE2,0,LEN_DISP,FONT2);	// ==== �п�ܥ���覡: ====
			do
			{
				//	Black	2023/05/31	�אּ�@����ܤE��AID
//				ClearLine(LINE3,LINE4,0);
//				if(list_cnt < 5)	// ==== �p�G�p��4��AID ====
//				{
					for(i = 0 ; i < list_cnt ; i++)
					{
						count[0] = i + '1';
						DispText(count,i + 4,0,2,FONT0);
						DispText(list_item[i].name,i + 4,2,list_item[i].len,FONT0);
					}
//				}
//				else	// ==== �p�G�j��4��AID ====
//				{
//					if((pos1 == (pos - 1)) && ((list_cnt%4) != 0))
//					{
//						j = 4;
//						for(i = ((list_cnt / 4) * 4) ; i < list_cnt ; i++)
//						{
//							count[0] = i + '1';
//							DispText(count,j,0,2,FONT0);
//							DispText(list_item[i].name,j,2,list_item[i].len,FONT0);
//							j = j + 1;
//						}
//						// ==== ��ܤW�Х� ====
//						DispText(p_up,LINE8,20,1,FONT0);
//					}
//					else if(pos1 < pos)
//					{
//						j = 4;
//						for(i = (pos1 * 4) ; i < (pos1 * 4 + 4) ; i++)
//						{
//							count[0] = i + '1';
//							DispText(count,j,0,2,FONT0);
//							DispText(list_item[i].name,j,2,list_item[i].len,FONT0);
//							j = j + 1;
//						}
//						// ==== ��ܤW�U�Х� ====
//						if(pos1 == 0)
//							DispText(p_down,LINE8,20,1,FONT0);
//						else if(pos1 == (pos - 1))
//							DispText(p_up,LINE8,20,1,FONT0);
//						else
//							DispText(p_up_down,LINE8,20,1,FONT0);
//					}
//				}

//				switch(keyin = getwaitkey(1800))	// ==== �����I ====
				//	Black	2019/06/06
				switch(keyin = getwaitkey(gl_Time_Out * 10))	// ==== �����I ====
				{
					case 0xFE:	// ==== Echo 2007/11/13 [ECR]����"70"�T�� ====
						EMV_Message(4,1);
						return CANCEL_RET;
					case 0xFF:
						EMV_Message(4,0);
						return CANCEL_RET;
					case CANCEL_RET:
						EMV_Message(4,0);
						return CANCEL_RET;
					//	Black	2021/01/14	�אּ�@����ܤE��AID
//					case MAP_SCROLL_UP:
//						if(pos1 > 0)
//							pos1 = pos1 - 1;
//						break;
//					case MAP_SCROLL_DOWN:
//						if(pos1 < (pos - 1))
//							pos1 = pos1 + 1;
//						break;
					default:
						list_no = keyin - '1';
						if(list_no < list_cnt)
							result = 0;
						break;
				}
			}while(result != 0);
			ClrDisplayDN(LINE2);	//	Black	2018/03/19	�Y���h��AID�ɻݲM���e��
		}
		// �ۧU����������B�۰ʿ��AID By Sam 2020/09/07
		else if(EMV_result == emvAutoSelected || (EMV_result == emvOK && gl_self_type))
			list_no = 0;
		else
		{
			memcpy(dispbuf,"03",2);

			ClrDisplayDN(LINE2);
			DispText(dispbuf,LINE2,0,2,FONT1);
			EMV_Message(2,0);
			return (FAIL);
		}

		//-------------------------2.��JCandidate List--------------------------------------
		//	Black	2016/11/25	�����[�t
//		if(EMV_status_check() == FAIL)
//			return(FAIL);

		ClrDisplayDN(LINE2);
		DispText(msg_emv_process,LINE3,0,LEN_DISP,FONT2);	// ==== �����d�B�z��... ====

		EMV_result = api_emvk_SelectApplication(list_cnt,list_no,(UCHAR *)&list_item);
		if(EMV_result == emvOK)
		{
			//	Black	2016/11/25	�����[�t
//			if(EMV_status_check() == FAIL)
//				return (FAIL);

			//	Black	2018/09/21	[AMEX]Ū��AID
			memset(buf, 0x00, sizeof(buf));
			EMV_result = api_emvk_GetDataElement(0x9F,0x06,(UCHAR *)&buf);
			if(EMV_result == emvFailed)
			{
//				memcpy(dispbuf,"10",2);
//
//				ClearLine(LINE2,LINE3,LINE4);
//				DispText(dispbuf,LINE2,0,2,FONT1);
				EMV_Message(8,0);
				return FAIL;
			}

			//	Black	2018/09/21	[AMEX]�x�sAID
			if (!memcmp(&buf[2], (UCHAR *)"\xA0\x00\x00\x00\x25", 5))
			{
				if (buf[0] > 16)
					gl_input.offset_amount[0] = 16;
				else
					gl_input.offset_amount[0] = buf[0];
				memcpy(&gl_input.offset_amount[1], &buf[2], gl_input.offset_amount[0]);
			}

			if(gl_input.trans == AUTH_SWIPE)
			{
				//	Black	2018/09/21	[AMEX]AE�w���v�Aapi_emvk_InitiateApplication()�ݶǤJ0x30
				if (!memcmp(&buf[2], (UCHAR *)"\xA0\x00\x00\x00\x25", 5))
					EMV_result = api_emvk_InitiateApplication(0x30);	// ==== Pre-Auth ====
				else
					EMV_result = api_emvk_InitiateApplication(0x38);	// ==== Pre-Auth ====
			}
			else
				EMV_result = api_emvk_InitiateApplication(0);		// ==== Sale ====

			if(EMV_result == emvOK)
				return (SUCCEED);
			else if(EMV_result == emvNotReady)
				;
			else
			{
				//memcpy(dispbuf,"05",2);

				//ClrDisplayDN(LINE2);
				//DispText(dispbuf,LINE2,0,2,FONT1);
				//EMV_Message(2,0);

				memcpy(dispbuf,"01",2);
				DispText(dispbuf,LINE4,0,2,FONT1);
				EMV_Message(1,0);
				return (FAIL);
			}
		}
		else if(EMV_result == emvNotReady)
			;
		else
		{
			memcpy(dispbuf,"04",2);

			ClrDisplayDN(LINE2);
			DispText(dispbuf,LINE2,0,2,FONT1);

			api_emvk_GetStatusBytes((UCHAR *)&sw1,(UCHAR *)&sw2);	// ==== Echo 2014/5/20 Ū�������d���G ====
			if((sw1 == 0x6A) && (sw2 == 0x82))						// ==== Echo 2014/5/20 file not found�ɭn��Fall Back ====
				EMV_Message(1,0);
			else if((sw1 == 0x62) && (sw2 == 0x83))					// ==== Echo 2015/6/15 Application Blocked  ====
				EMV_Message(10,0);
			else
				EMV_Message(2,0);

			return (FAIL);
		}
	} // ==== end forever ====
}
#endif

/* ----------------------------------------------
Name:	EMV_Trans_START
Funct:	
Input:  None
Output: FAIL	=> ����
		SUCCEED => ���\
Global:	
Note:	�ϥ�Step1 ~ Step4
Black	2016/11/25	ATR����retry 3���A��Delay 0.1��
Black	2019/03/14	ATR���Ѥ���retry
Black	2020/12/21	�ثn�Ȧ�H�Υd�n�x�sAP Label
Black	2023/03/13	�ק襤��H�UFall Back�P�_
------------------------------------------------*/
#if 0
UCHAR	EMV_Trans_START(void)
{
	UCHAR	msg_emv_process[LEN_DISP] = {' ',0xB4,0xB9,0xA4,0xF9,0xA5,0x64,0xB3,0x42,0xB2,0x7A,0xA4,0xA4,'.','.','.'};	// ==== �����d�B�z��... ====
	UCHAR	EMV_result;
	UCHAR	dispbuf[2] = {"00"};
	UCHAR	sw1,sw2;
//	UCHAR	retry;

	// ==== Echo 2015/11/12 �W�["�����d�B�z��..."�T�� ====
	ClearLine(LINE2,LINE3,LINE4);
	DispText(msg_emv_process,LINE3,0,LEN_DISP,FONT2);	// ==== �����d�B�z��... ====

	//-------Step1---------------------------------------
	//	Black	2016/11/25	�����[�t
//	if(EMV_status_check() == FAIL)
//		return FAIL;

	//	Black	2019/03/14	ATR���Ѥ���retry
	//	Black	2016/11/25	�s�Wretry
//	retry = 0;
//	forever
//	{
		EMV_result = api_emvk_EnableICC();
		if(EMV_result == emvFailed)
		{
//			if (retry > 3)
//			{
//				memcpy(dispbuf,"02",2);
//				DispText(dispbuf,LINE4,0,2,FONT1);
				//	Black	2023/03/13	����H�U
				if (gl_Credit_Host == 6)
					EMV_Message(2,0);	// ==== Echo 2009/9/1 �d���ϴ����i��Fall Back ====
				else
					EMV_Message(1,0);	//	Black	2016/12/08	������d���ϴ��i��Fall Back
				return FAIL;
//			}
//			else
//			{
//				retry ++;
//				api_emvk_DisableICC();	// ����EMV Reader�q��
//				delay(1);	//	Black	2016/11/25	��Delay 0.1��
//			}
		}
//		else
//			break;
//	}

	gl_count_CHIP = gl_count_CHIP + 1;	// ==== Echo 2013/2/1 �����d���ƭp�ƾ� ====

	//-------Step2---------------------------------------
	//	Black	2016/11/25	�����[�t
//	if(EMV_status_check() == FAIL)
//		return FAIL;

	EMV_result = api_emvk_CreateCandidateList();
	if(EMV_result == emvOK)
	{
		EMV_result = EMV_Get_Candidate_List();
		if(EMV_result == FAIL || EMV_result == CANCEL_RET)
			return FAIL;
	}
	else if(EMV_result == emvNotReady)	// ==== �X�{NotReady���ܭnFall Back ====
	{
		memcpy(dispbuf,"03",2);
		DispText(dispbuf,LINE4,0,2,FONT1);

		// ==== Echo 2015/6/15 Get Candidate List��,application blocked(6283)���i��Fall Back ====
		api_emvk_GetStatusBytes((UCHAR *)&sw1,(UCHAR *)&sw2);
		if((sw1 == 0x62) && (sw2 == 0x83))
			EMV_Message(10,0);
		else
			EMV_Message(1,0);
		return FAIL;
	}
	else
	{
		memcpy(dispbuf,"02",2);

		ClrDisplayDN(LINE2);
		DispText(dispbuf,LINE2,0,2,FONT1);

		//	Black	2023/03/13	����H�U
		if (gl_Credit_Host == 6)
		{
			// ==== Echo 2015/11/12 Get Candidate List��,Card blocked(6A81)��ܻPapplication blocked�P�˪��T�� ====
			api_emvk_GetStatusBytes((UCHAR *)&sw1,(UCHAR *)&sw2);
			if((sw1 == 0x6A) && (sw2 == 0x81))
				EMV_Message(10,0);
			else
				EMV_Message(2,0);
		}
		else
			EMV_Message(2,0);
		return FAIL;
	}

	//-------Step3---------------------------------------
	//	Black	2016/11/25	�����[�t
//	if(EMV_status_check() == FAIL)
//		return FAIL;

	EMV_result = api_emvk_ReadApplicationData();
	if(EMV_result == emvFailed)
	{
		memcpy(dispbuf,"06",2);

		ClrDisplayDN(LINE2);
		DispText(dispbuf,LINE2,0,2,FONT1);
		EMV_Message(2,0);
		return FAIL;
	}

	//-------Step4----Ū��PAN & AID & Name---------------
	EMV_result = GetEmvData(1);	// ==== AID ====
	if(EMV_result == FAIL)
		return FAIL;

	EMV_result = GetEmvData(2);	// ==== PAN ====
	if(EMV_result == FAIL)
		return FAIL;

	EMV_result = GetEmvData(3);	// ==== Name ====
	if(EMV_result == FAIL)
		return FAIL;

#if VERSION_MODE != VERSION_NCCC
	//	Black	2020/12/21	�ثn�Ȧ�H�Υd�n�x�sAP Label
	if (gl_Credit_Host == 1)
	{
		EMV_result = GetEmvData(7);	// ==== Ū��AP_Label ====
		if(EMV_result == FAIL)
			return FAIL;
	}
#endif

	return SUCCEED;
}
#endif


/* ----------------------------------------------
Name:	EMV_Trans_MSI
Funct:	
Input:  None
Output: FAIL	=> ����
		SUCCEED => ���\
Global:	
Note:	�ϥ�Step7 & Step8
------------------------------------------------*/
#if 0
UCHAR	EMV_Trans_MSI(void)
{
	UCHAR	EMV_result;
	UCHAR	dispbuf[2] = {"00"};

	//-------Step7---------------------------------------
	//	Black	2016/11/25	�����[�t
//	if(EMV_status_check() == FAIL)
//		return FAIL;

	EMV_result = api_emvk_OfflineDataAuthen();
	if(EMV_result == emvOutOfService)
	{
		memcpy(dispbuf,"07",2);

		ClrDisplayDN(LINE2);
		DispText(dispbuf,LINE2,0,2,FONT1);
		EMV_Message(2,0);
		return FAIL;
	}

	//-------Step8---------------------------------------
	//	Black	2016/11/25	�����[�t
//	if(EMV_status_check() == FAIL)
//		return FAIL;

	api_emvk_ProcessingRestrictions();
	return	SUCCEED;
}
#endif


/* ----------------------------------------------
Name:	EMV_Trans_CM
Funct:	
Input:  None
Output: FAIL	=> ����
		SUCCEED => ���\
Global:	
Note:	�ϥ�Step7 & Step8
Black	2020/09/22	���Key index
LastUpdate: rex 2021/05/07 [NCCC][MPAS]
Black	2022/04/01	�s�W�����������Online���
Black	2022/08/09	�s�W�x�s�Ȧ洹���h�f����Online���
Black	2022/08/30	�Ϥ��@�목���pKey�B�]�������pKey
rex		2022/12/16
Black	2022/12/22	���عq�H����J�K�X�ɤ��ק�I���C��
------------------------------------------------*/
#if 0
UCHAR	EMV_Trans_CM(void)
{
	UCHAR	EMV_result;
//	UCHAR	buf[9];
	UCHAR	dispbuf[2] = {"00"};
	UCHAR	j;
//	UCHAR	EMV_epb[10],ksn[12],pin_mode=0;	//rex 2010/03/19 ksn:���ϥΡA��������.
	UCHAR	EMV_epb[10],ksn[12],pin_mode = 0xFF ;	//	Black	2011/03/21	pin_mode = 0xFF �� �K�X�����` �� ���ˬd�O�_�n��J�K�X
	
	//-------Step9---------------------------------------
	//	Black	2016/11/25	�����[�t
//	if(EMV_status_check() == FAIL)
//		return FAIL;

	if(gl_pinpad.mode == 0)			// ==== ����PINPAD ====
		pin_mode = 0;
	else if(gl_pinpad.mode == 1)	// ==== �~��PINPAD ====
		pin_mode = 1;

	memset(EMV_epb,0x00,sizeof(EMV_epb));

	//	Black	2020/09/22	���Key index
	gl_pinpad.keyindex = 0;

	//	Black	2022/12/22	���عq�H����J�K�X�ɤ��ק�I���C��
//#if VERSION_MODE == VERSION_NCCC	//rex 2022/07/19
//	if (pin_mode == 0)
//		gl_BG_color = color_orange;	//�]�w�I���⬰���
//#endif

//	api_peds_OpenPinPad(pin_mode);
	Peds_OpenPinPad(pin_mode);	//	Black	2022/10/11

	EMV_result = api_emvk_CardholderVerification(pin_mode,0,0,gl_pinpad.keyindex,EMV_epb,ksn,60);	// ==== Echo 2014/6/18 ��JOnline PIN ====	// ==== Echo 2014/6/24 �W�[key index�Ѽ� ====
	api_peds_ClosePinPad(pin_mode);	// ==== ����PINPAD ====

	//	Black	2022/12/22	���عq�H����J�K�X�ɤ��ק�I���C��
//#if VERSION_MODE == VERSION_NCCC	//rex 2022/07/19
//	gl_BG_color = color_white;	//�]�w�I���⬰�զ�
//
//	if(gl_input.redeem_flag == 3)	//rex 2022/12/16 �קK����X�{�����
//		DispCUPHeader(1);
//	else
//		DispHeader(1);
//#endif

	if(EMV_result == emvOutOfService)
	{
		memcpy(dispbuf,"08",2);

		ClrDisplayDN(LINE2);
		DispText(dispbuf,LINE2,0,2,FONT1);
		EMV_Message(2,0);
		return FAIL;
	}

	if(EMV_epb[0] != 0)		// ==== Echo 2014/6/18 �����סA���ܦ���J Online PIN ====
	{
		gl_pin_entry = 1;
		memcpy(gl_pin_block,(UCHAR *)&EMV_epb[2],8);
	}

	if(gl_input.redeem_flag == 3)
		DispCUPHeader(1);
	else
		DispHeader(1);
	DispText((UCHAR *)msg_connecting_1,LINE3,0,LEN_DISP,FONT2);	// ==== �B�z�� ====

	//-------Step10--------------------------------------
	//	Black	2016/11/25	�����[�t
//	if(EMV_status_check() == FAIL)
//		return FAIL;

	api_emvk_TerminalRiskManagement();

	//-------Step11--------------------------------------
	//	Black	2016/11/25	�����[�t
//	if(EMV_status_check() == FAIL)
//		return FAIL;

	// ==== �p�G��Pre-Auth��Redeem�h�n�ϥαj��Online����� ====
//	if(gl_input.trans == AUTH_SWIPE || gl_input.redeem_flag == 1)
	//	Black	2022/04/01	�s�W�����������Online���
//	if ((gl_input.trans == AUTH_SWIPE) || (gl_input.redeem_flag == 1) || (gl_input.redeem_flag == 2))
	//	Black	2022/08/09	�s�W�x�s�Ȧ洹���h�f����Online���
	if ((gl_input.trans == AUTH_SWIPE)|| (gl_input.trans == ONL_REFUND_SW)
	|| (gl_input.redeem_flag == 1) || (gl_input.redeem_flag == 2))
		EMV_result = api_emvk_TerminalActionAnalysis(1);
	else
		EMV_result = api_emvk_TerminalActionAnalysis(0);

	//	Black	2019/02/01	�Ш|�V�m��
	if (gl_EDU == 0)
	{

#if VERSION_MODE == VERSION_NCCC
		//rex 2021/05/07 [NCCC][MPAS]
		//rex 2018/12/17 �ɧ�
		//alex 2018/10/11 MPAS
		if( (gl_Switch[SW_MPAS] == ON) && (gl_rsp_data.status & MPAS_Indicator) )	// MPAS���� EMV 2nd Generate AC �����͵��G
			return SUCCEED;
#endif 

		if(EMV_result == emvFailed)
		{
			memcpy(dispbuf,"09",2);
	
			ClrDisplayDN(LINE2);
			DispText(dispbuf,LINE2,0,2,FONT1);
			EMV_Message(2,0);
			return FAIL;
		}
		// ==== ARQC FAIL ====
		else if(EMV_result == emvNotReady)
		{
			UpdateHostStatus(REV_PENDING);
			for (j=0; j < ACQUIRER_NO_R; j++)
			{
				if ((gl_host_status[j].pending == REV_PENDING) && (gl_host_status[j].id == gl_cur_acquirer.id))
					gl_host_status[j].emv_flag = 4;
			}
	
			memcpy(dispbuf,"10",2);
	
			ClrDisplayDN(LINE2);
			DispText(dispbuf,LINE2,0,2,FONT1);
			EMV_Message(2,0);
			return FAIL;
		}
	}

	return	SUCCEED;
}
#endif

/* ----------------------------------------------
Name:	process_emv_redeem
Funct:	
Input:  None
Output: None
Global:	
Echo 2003/9/30 �����d���Q��Ϊ������b�_�s�u��C�Lñ��ѳo��B�z
------------------------------------------------*/
#if 0
void process_emv_redeem(void)
{
//	UCHAR	result_flag;
	UCHAR	Link_error[LEN_DISP] = {'T','I','M','E',' ','O','U','T',' ',0xBD,0xD0,0xAD,0xAB,0xB8,0xD5,' '};	// ==== TIME OUT,�Э��� ====
	UCHAR	connecting;

	ClrDisplayDN(LINE2);
	if(gl_lan_port.status == 0)
	{
		do
		{
			api_mdm_status(0,&connecting);
		}while(connecting == 0x80);

		if(connecting == 0x02 || connecting == 0x01)
			connecting = SUCCEED;
		else
			connecting = FAIL;
	}

//	if(gl_lan_port.status == 2)
	//	Black	2019/01/30
	if ((gl_lan_port.status == 2) || (gl_lan_port.status == 5))
	{
		connecting = IP_ConnectionOK(0,1);	// ==== [TMS] Echo 2014/8/27 ====
		if (connecting == FAIL)
		{
			// ==== Echo 2015/6/15 �����ƴ��}�� ====
			if(gl_ip_change_dial_sw == 0)
			{
				ClrDisplayDN(LINE2);
				DispText(Link_error,LINE2,0,LEN_DISP,FONT2);	// ==== TIME OUT,�Э��� ====
				DispText(gl_rsp_data.rspcode,LINE3,0,2,FONT1);
				LongBeep();
				EMV_Message(3,0);
				goto EXIT;
			}

			//	Black	2019/01/30	�s�W4G
			if (gl_lan_port.status == 2)
				gl_lan_port.status = 3;	// ==== TCP/IP��Dial
			else if (gl_lan_port.status == 5)
				gl_lan_port.status = 6;	// ==== 4G��Dial

			if (!EMV_ConnectionOK(NOT_CONNECTING, (TEL_STRU *)&gl_cur_acquirer.pri_trans_tel))
			{
				ClrDisplayDN(LINE2);
				DispText(Link_error,LINE2,0,LEN_DISP,FONT2);	// ==== TIME OUT,�Э��� ====
				DispText(gl_rsp_data.rspcode,LINE3,0,2,FONT1);
				LongBeep();
				EMV_Message(3,0);
				goto EXIT;
			}
		}
	}
	else
	{
		if (!EMV_ConnectionOK(connecting, (TEL_STRU *)&gl_cur_acquirer.pri_trans_tel))
		{
			ClrDisplayDN(LINE2);
			DispText(Link_error,LINE2,0,LEN_DISP,FONT2);	// ==== TIME OUT,�Э��� ====
			DispText(gl_rsp_data.rspcode,LINE3,0,2,FONT1);
			LongBeep();
			EMV_Message(3,0);
			goto EXIT;
		}
	}

	if (!ReversalProc())	// ==== No Pending Trans or Pending has just been sent ====
	{
		// ==== Echo 2014/5/1 ���`SAM Slot-1���������]���ݰ�ATR ====
		// ==== Echo 2014/5/1 �}��SAM�d�[�K ====
//		if(gl_lan_port.status == 2 || gl_lan_port.status == 3 || gl_lan_port.status == 4)	//	Black	2011/03/21
//			IFM_SAM_Close();		// ==== ����SAM2 ====

		// ==== �����d�I�Ƭd�̫߳�Y��Ū�촹���d�h�������� ====
//		if(EMV_status_check() == FAIL)	// ==== Echo 2016/2/22 �����d������,�Y�u�����d����ɶ� ====
//			goto EXIT;

		DispRspText();

#if VERSION_MODE != VERSION_NCCC
		//	Black	2023/01/06	����H�U
		if (gl_Credit_Host == 6)
		{
			// ==== [SELF] Echo 2019/1/17 �D���^67,�N�Nkey�M�� ====
			if((gl_self_type == 1) && (gl_soft_encrypt == 2) && !memcmp(gl_rsp_data.rspcode,"67",2))
			{
				if(RedeemHost() == TRUE)
					gl_ctcb_key_table.masterkey_status[1][0] = 0;
				else if(InstallmentHost() == TRUE)
					gl_ctcb_key_table.masterkey_status[2][0] = 0;
				else if(CtbcHost() == TRUE)
					gl_ctcb_key_table.masterkey_status[0][0] = 0;
			}
		}
#endif

		LongBeep();
		EMV_Message(3,0);
		goto EXIT;
	}

	IncTraceNum();
	IncRocNum();

	PatTxCommonCode();
	PatProcCode(gl_tx_data.trans);

	DecRocNum();

	switch (SendISO(REV_PENDING))
	{
		case APPROVED:
			// ==== �����d����̫�Y��Ū�촹���d�h�������� ====
		  	if(EMV_status_check() == FAIL)
				break;

			UpdateHostStatus(NO_PENDING);	// ==== Signal to no pending for current acquirer ====
			IncRocNum();

//			result_flag = SearchOffline();	// ==== �w���j�M�O�_��Offline�����(�S�����ܥi�H���_�u) ====
			TransAccptCommonCode();

#if VERSION_MODE != VERSION_NCCC
			// ==== Echo 2015/11/18 �C�Lñ��e���W�����u��� ====
//			if(result_flag == SUCCEED)
//			{
				if (DinersHost() == FALSE)
					OfflineProc(ONLY_ONE);
				else
					DelayResetLine();
//			}
#else
			OfflineProc(ONLY_ONE);
#endif

			// ==== Echo 2015/11/18 �C�Lñ�� ====
			PrintLatestReceipt();
			gl_change = 1;

			// ==== Echo 2014/5/1 ���`SAM Slot-1���������]���ݰ�ATR ====
			// ==== Echo 2014/5/1 �}��SAM�d�[�K ====
//			if(gl_lan_port.status == 2 || gl_lan_port.status == 3 || gl_lan_port.status == 4)	//	Black	2011/03/21
//				IFM_SAM_Close();		// ==== ����SAM2 ====

			EMV_Message(7,0);
			break;

		case NOT_APPROVED:
			UpdateHostStatus(NO_PENDING);	// ==== Signal to no pending for current acquirer ====
			// ==== �����d����̫�Y��Ū�촹���d�h�������� ====
		  	if(EMV_status_check() == FAIL)
			{
				// ==== Echo 2014/5/1 ���`SAM Slot-1���������]���ݰ�ATR ====
				// ==== Echo 2014/5/1 �}��SAM�d�[�K ====
//				if(gl_lan_port.status == 2 || gl_lan_port.status == 3 || gl_lan_port.status == 4)
//					IFM_SAM_Close();		// ==== ����SAM2 ====

				break;
			}

			LongBeep();
			//	Black	2020/08/26	���عq�H���LCall Bank
//			if (IsReferral() && !ChkAcquirerOption(28) && RedeemHost() != TRUE)
//			{
//				// ==== ��ܽаh�X�d�� ====
//				if(ReferralProc() == SUCCEED)
//				{
//					// ==== Echo 2014/5/1 ���`SAM Slot-1���������]���ݰ�ATR ====
//					// ==== Echo 2014/5/1 �}��SAM�d�[�K ====
////					if(gl_lan_port.status == 2 || gl_lan_port.status == 3 || gl_lan_port.status == 4)
////						IFM_SAM_Close();		// ==== ����SAM2 ====
//
//					EMV_Message(7,0);
//				}
//				else
//				{
//					// ==== Echo 2014/5/1 ���`SAM Slot-1���������]���ݰ�ATR ====
//					// ==== Echo 2014/5/1 �}��SAM�d�[�K ====
////					if(gl_lan_port.status == 2 || gl_lan_port.status == 3 || gl_lan_port.status == 4)
////						IFM_SAM_Close();		// ==== ����SAM2 ====
//
//					EMV_Message(4,0);
//				}
//				goto EXIT;
//			}
			DispRspText();

#if VERSION_MODE != VERSION_NCCC
			//	Black	2023/01/06	����H�U
			if (gl_Credit_Host == 6)
			{
				// ==== [SELF] Echo 2019/1/17 �D���^67,�N�Nkey�M�� ====
				if((gl_self_type == 1) && (gl_soft_encrypt == 2) && !memcmp(gl_rsp_data.rspcode,"67",2))
				{
					if(RedeemHost() == TRUE)
						gl_ctcb_key_table.masterkey_status[1][0] = 0;
					else if(InstallmentHost() == TRUE)
						gl_ctcb_key_table.masterkey_status[2][0] = 0;
					else if(CtbcHost() == TRUE)
						gl_ctcb_key_table.masterkey_status[0][0] = 0;
				}
			}
#endif

#if VERSION_MODE != VERSION_NCCC
			if (DinersHost() == FALSE)
				OfflineProc(ONLY_ONE);
			else
				DelayResetLine();
#else
			OfflineProc(ONLY_ONE);
#endif

			// ==== Echo 2014/5/1 ���`SAM Slot-1���������]���ݰ�ATR ====
			// ==== Echo 2014/5/1 �}��SAM�d�[�K ====
//			if(gl_lan_port.status == 2 || gl_lan_port.status == 3 || gl_lan_port.status == 4)	//	Black	2011/03/21
//				IFM_SAM_Close();		// ==== ����SAM2 ====	//	Black	2011/03/21	SAM1�A�u�ݭn�}�@���A�]��������

			EMV_Message(9,0);
			break;

		case TRANS_FAILED:

			// ==== Echo 2014/5/1 ���`SAM Slot-1���������]���ݰ�ATR ====
			// ==== Echo 2014/5/1 �}��SAM�d�[�K ====
//			if(gl_lan_port.status == 2 || gl_lan_port.status == 3 || gl_lan_port.status == 4)
//				IFM_SAM_Close();		// ==== ����SAM2 ====

		  	// ==== �����d����̫�Y��Ū�촹���d�h�������� ====
			if(EMV_status_check() == FAIL)
					break;

			TransFailCommonCode();
			EMV_Message(3,0);
			break;

		// ==== Echo 2014/5/1 �}��SAM�d�[�K ====
		case SAM_ERROR:	// ==== Echo 2006/3/27 SAM�d���K���~�B�z ====

			// ==== Echo 2014/5/1 ���`SAM Slot-1���������]���ݰ�ATR ====
//			IFM_SAM_Close();		// ==== ����SAM2 ====

		    // ==== �����d�I�Ƭd�̫߳�Y��Ū�촹���d�h�������� ====
			if(EMV_status_check() == FAIL)
				break;
		    DispRspText_SAM(1);
			LongBeep();
			EMV_Message(9,0);
		    break;

		default:

			// ==== Echo 2014/5/1 ���`SAM Slot-1���������]���ݰ�ATR ====
			// ==== Echo 2014/5/1 �}��SAM�d�[�K ====
//			if(CubHost() == TRUE && (gl_lan_port.status == 2 || gl_lan_port.status == 3 || gl_lan_port.status == 4))
//				IFM_SAM_Close();		// ==== ����SAM2 ====

			DelayResetLine();
			break;
	}	// ==== of switch ====

EXIT:
	ResetLine();	//	Black	2011/03/15	�_�u
	FlushMSR();
	FlushKey();
	gl_change= 1;
	return;
}
#endif

/* ----------------------------------------------
Name:	EMV_sale
Funct:	
Input:  None
Output: 
Global:	
Black	2019/01/25	���OnLineRedeem2()
Black	2018/09/21	[AMEX]�s�WAMEX�����d
Black	2018/09/21	[AMEX]�s�W�x�sAP Label
Black	2019/03/18	[AMEX]�ק��x�sAMEX�������
Black	2019/06/04	[ECR]AUXSTART()�n����OnLineRedeem2()��A�~�i���o���T������G�H�Υd�B���Q�B����
Black	2019/06/04	[ECR]��q��ECR�b�L�d��~��J���
Sam		2019/10/30	�קאּ���عq�H����ECR
Black	2020/12/18	�s�WM-TIP�����d�Kñ
rex 2021/05/07 [NCCC][MPAS]
Black	2021/05/26	�s�W�x�sLMS_Invoice_No
Black	2021/10/21	�]����T�n�ϥ�F56
rex		2022/09/12	[NCCC][PIN���X��1]
Black	2023/04/27	�]����T�B����H�U�n�ϥ�F56
------------------------------------------------*/
#if 0
void	EMV_sale(void)
{
	UCHAR	msg_emv_process[LEN_DISP] = {' ',0xB4,0xB9,0xA4,0xF9,0xA5,0x64,0xB3,0x42,0xB2,0x7A,0xA4,0xA4,'.','.','.'};					// ==== �����d�B�z��... ====
#if VERSION_MODE != VERSION_NCCC
	UCHAR	msg_not_redeem[LEN_DISP] = {' ',' ',' ',0xA6,0xAC,0xB3,0xE6,0xA6,0xE6,0xA5,0xBC,0xB6,0x7D,' ',' ',' '};						// ==== ����楼�} ====
#endif
	FONT_DEF lMsg_check_digit_err = {{' ',' ',' ',' ',0xA5,0x64,0xB8,0xB9,0xBF,0xF9,0xBB,0x7E,' ',' ',' ',' '},{" CHECK DIGIT ERR"}};	// ==== �d�����~ ====
	UCHAR	msg_invaild_card[14] = {0xA6,0xB9,0xA5,0x64,0xA4,0xF9,0xAB,0x44,0xAB,0x48,0xA5,0xCE,0xA5,0x64};								// ==== ���d���D�H�Υd ====
	UCHAR	EMV_result;
	UCHAR	result;//,status;
	UCHAR	RecordFull_flag;
	UCHAR	buf[9];
//	UCHAR	result_flag = SUCCEED;
	UCHAR	trace_buf[TRACE_NO_HALF];

	UCHAR	aid_vis[5] = {0xA0,0x00,0x00,0x00,0x03};
	UCHAR	aid_mcc[5] = {0xA0,0x00,0x00,0x00,0x04};
	UCHAR	aid_jcb[5] = {0xA0,0x00,0x00,0x00,0x65};
	UCHAR	aid_aex[5] = {0xA0,0x00,0x00,0x00,0x25};	//	Black	2018/09/21	[AMEX]

	gl_input.emv_flag = 1;	// ==== �]�w��EMV�����d��� ====
//	gl_input.entry_mode[1] = 0x52;
//remark by rex 2022/09/12 [NCCC][PIN���X��1]
#if VERSION_MODE != VERSION_NCCC
	gl_input.entry_mode[1] = 0x52;
#else
	gl_input.entry_mode[1] = 0x51;
#endif

	gl_input.trans = SALE_SWIPE;	//	Black	2012/05/24	�s�W�]�w������O

	ClrDisplayDN(LINE2);
	DispText(msg_emv_process,LINE3,0,LEN_DISP,FONT2);	// ==== �����d�B�z��... ====

	//***$$$-Step1,2,3,4---------------------------------
	// ==== Echo 2015/11/12 ���ʨ�W�@�h ====
//	if(EMV_Trans_START() == FAIL)
//		return;

	//-------Step5----��J���---------------------------
	//	Black	2019/06/04	[ECR]AUXSTART()�n����OnLineRedeem2()��A�~�i���o���T������G�H�Υd�B���Q�B����
//	// ==== Echo 2013/2/6 [ECR]���Ⱦ��_�_�� ====
//	if (gl_ecr_flag == 1)
//	{
//		// ==== �^�e�d��"60" ====
//		result = AUXSTART();
//		if(result == FAIL)
//		{
//			EMV_Message(4,1);
//			return;
//		}
//
//		// ==== ��ܥ�����B ====
//		// ==== ��J���B(gl_input.tot_amount) ====
//		compress(&gl_input.tot_amount[1],glo_ecr_data.amount,5);	// ==== ���B�[���p�� ====
//		//	Black	2019/06/03	[ECR]��������b�Ĥ@����ܦ��Ⱦ�������B
////		DispAmount(gl_input.tot_amount,LINE1,FONT1 + attrREVERSE);		// ==== ��ܪ��B ====
//	}

	FlushKey();
	// ==== Echo 2007/11/13 [ECR]���Ⱦ��s�u ====

//	result = OnLineRedeem();
	//	Black	2019/01/25	���OnLineRedeem2()
	result = OnLineRedeem2();	//	Black	2019/07/19	���ϥζǤJ��
	if (result != TRUE)
	{
		if(gl_get_redeem_error_flag != 0)
		{
			if (gl_ecr_flag != 0)
			{
				if (gl_get_redeem_error_flag == 4)
					memcpy(glo_ecr_data.resp_code, "0004", 4);
				else if (gl_get_redeem_error_flag == 5)
					memcpy(glo_ecr_data.resp_code, "0005", 4);
				// �|�bEMV_Message���^�_ By Sam 2020/11/27
				// AUXsend(0xFF);
			}

			// ==== Echo 2014/5/20 ��H�Υd�ɭY�D�H�Υd���i��� ====
			// ==== Echo 2014/6/19 �ץ����ϥλ��p�d�ɫ�CANCEL��|���"���d���D�H�Υd"�T�� ====
//			if((gl_input.redeem_flag == 0) && memcmp(gl_emvk_aid,aid_vis,5) && memcmp(gl_emvk_aid,aid_mcc,5) && memcmp(gl_emvk_aid,aid_jcb,5))
			//	Black	2018/09/21	[AMEX]�s�WAMEX�����d
			if((gl_input.redeem_flag == 0) && memcmp(gl_emvk_aid,aid_vis,5) && memcmp(gl_emvk_aid,aid_mcc,5) && memcmp(gl_emvk_aid,aid_jcb,5) && memcmp(gl_emvk_aid,aid_aex,5))
			{
				ClrDisplayDN(LINE2);
				DispText(msg_invaild_card,LINE3,1,14,FONT2);	// ==== ���d���D�H�Υd ====
				LongBeep();
				EMV_Message(3,0);
				return;
			}

			Redeem_error_message();
			LongBeep();
			EMV_Message(3,0);
		}
		else
		{
			if(result != 0xFE)
				EMV_Message(4,0);
			else
				EMV_Message(4,1);
		}
		return;
	}

	//	Black	2019/06/04	[ECR]AUXSTART()�n����OnLineRedeem2()��A�~�i���o���T������G�H�Υd�B���Q�B����
	// ==== Echo 2013/2/6 [ECR]���Ⱦ��_�_�� ====
	if (gl_ecr_flag == 1)
	{
		// ==== �^�e�d��"60" ====
		result = AUXSTART();
		if(result == FAIL)
		{
			EMV_Message(4,1);
			return;
		}

		// ==== ��ܥ�����B ====
		// ==== ��J���B(gl_input.tot_amount) ====
		compress(&gl_input.tot_amount[1],glo_ecr_data.amount,5);		// ==== ���B�[���p�� ====
		//	Black	2019/06/03	[ECR]��������b�Ĥ@����ܦ��Ⱦ�������B
//		DispAmount(gl_input.tot_amount,LINE1,FONT1 + attrREVERSE);		// ==== ��ܪ��B ====
	}

	gl_fall_back_flag = 0;	// ==== ����Fall Back ====

	if(gl_get_card_error_flag == 2)
	{
		ClrDisplayDN(LINE2);
#if VERSION_MODE != VERSION_NCCC
		if(CARD_RANGE() == FOUND)
			DispText(msg_not_redeem,LINE3,0,LEN_DISP,FONT2);	// ==== ����楼�} ====
		else
#endif
//remark by rex 2020/11/06 [NCCC]
			DispBiText((FONT_DEF *)&msg_not_support, LINE3);	// ==== ���������d ====

		LongBeep();
		EMV_Message(3,0);
		return;
	}

	// ==== �b�w���ε��b���� ====
	RecordFull_flag = RecordFull();		//	Black	2011/07/20
	if(RecordFull_flag)
	{
		ClrDisplayDN(LINE2);
		if(RecordFull_flag == 1)
		{
			DispBiText((FONT_DEF *)&msg_close_batch, LINE2);	// ==== �妸�w�� ====
			DispBiText((FONT_DEF *)&msg_close_batch_1, LINE3);	// ==== �Х����b ====
		}
		else if(RecordFull_flag == 2)
			DispBiText((FONT_DEF *)&msg_close_batch_1, LINE3);	// ==== �Х����b ====

		LongBeep();
		EMV_Message(3,0);
		return;
	}

	// ==== Acquirer Table �\��}�� ====
	if(ChkAcquirerOption(24))
	{
		ClrDisplayDN(LINE2);
		DispText((UCHAR *)msg_no_auth,LINE3,0,LEN_DISP,FONT2);	// ==== ���\�ॼ�} ====
		LongBeep();
		EMV_Message(3,0);
		return;
	}

	if(gl_get_card_error_flag == 1)
	{
		ClrDisplayDN(LINE2);
		DispBiText(&lMsg_check_digit_err,LINE3);	// ==== �d�����~ ====
		LongBeep();

		EMV_Message(3,0);
		return;
	}

	//	Black	2016/11/24	���ϥ�
	// ==== ��ܤ����I�ڤ覡 ====
//	if(gl_input.redeem_flag == 2)
//	{
//		// ==== Echo 2007/11/13 [ECR]���Ⱦ��s�u ====
//		result = Choice_Payment();
//		if(result != SUCCEED)
//		{
//			if(result != 0xFE)
//				EMV_Message(4,0);
//			else
//				EMV_Message(4,1);
//			return;
//		}
//	}

	// ==== Pre-Dial ====
	if(gl_lan_port.status == 0)
		PredialOK();

	//	Black	2019/01/30	���׬O�_���P�����A�Ҧb�L�d�e��J���B
//	// ==== Echo 2014/1/23 �L�P���\��ɿ�J���B ====
//	if(gl_Wreader_status == FAIL)
//	{
//		if (gl_ecr_flag == 0)	// ==== Echo 2014/2/20 [ECR]���Ⱦ��_�_�� ====
//		{
//			if (!GetAmount(0))
//			{
//				EMV_Message(4,0);
//				return;
//			}
//		}
//	}

	//	Black	2016/11/24	���ϥ�
	// ==== Redeem Product Code�\�� ====
//	if(gl_input.redeem_flag == 1 && (gl_term_config.options[2] & 0x01) == 0x01)
//	{
//		// ==== Echo 2007/11/13 [ECR]���Ⱦ��s�u ====
//		result = GetProduct_Code(0);
//		if (result != SUCCEED)
//		{
//			if(result != 0xFE)
//				EMV_Message(4,0);
//			else
//				EMV_Message(4,1);
//			return;
//		}
//	}

	//	Black	2019/06/04	[ECR]��q��ECR�b�L�d��~��J���
	if (gl_ecr_flag == 1)
	{
#if VERSION_MODE != VERSION_NCCC
		//	Black	2019/01/30	����L�d�e
		// ==== ��J�������ƥ\�� ====
		if(gl_input.redeem_flag == 2)
		{
			result = GetStage();
			if (result != SUCCEED)
			{
				if(result != 0xFE)
					EMV_Message(4,0);
				else
					EMV_Message(4,1);
				return;
			}
		}
#endif
//remark by rex 2020/11/26 [NCCC]

#if VERSION_MODE != VERSION_NCCC
		result = GetAdditional();
#else
//remark by rex 2020/12/17 [NCCC] ��� NCCC���� 
		result = GetAdditional_NCCC();
#endif
		if (result != SUCCEED)
		{
			EMV_Message(4,0);
			return;
		}

		// ==== �T�{���������� ====
		if(gl_input.redeem_flag == 2)
		{
			memset(gl_input.rrn,' ',12);				// ==== RRN ====
			memset(gl_input.fee_date,0xFF,4);			// ==== ����O�]�w���(YYYYMMDD) ====
			memset(gl_input.refund_tid,'0',8);			// ==== Refund Terminal ID ====
			memset(gl_input.fee_amount,0xFF,6);			// ==== ����O�`�B ====
			memset(gl_input.inst_amount,0xFF,6);		// ==== �`���B ====
			memset(gl_input.down_amount,0xFF,6);		// ==== �������B ====
			memset(gl_input.monthly_point,0xFF,6);		// ==== �C�����B ====
			memset(gl_input.offset_amount,0xFF,6);		// ==== ����O�C�����B ====	//	Black	2016/11/24
			gl_input.delay_month = 0xFF;				// ==== �������B�X�Ӥ��J�b ====

			gl_input.flexible_stageE = 0xFF;			// ==== �u�ʥI�ګe������ ====
			gl_input.flexible_stageL = 0xFF;			// ==== �u�ʥI�ګ������ ====
			memset(gl_input.flexible_payment,0xFF,6);	// ==== �u�ʥI�ګe���C������ ====
		}
		//	Black	2019/01/30	����L�d�e
	}

	// ==== ���Q��Υѳo����}�B�z!! ====
//	if(gl_input.redeem_flag != 0)
//	{
//		process_emv_redeem();
//		return;
//	}

	ClrDisplayDN(LINE2);
	DispText((UCHAR *)msg_connecting_1,LINE3,0,LEN_DISP,FONT2);	// ==== �B�z�� ====

	//-------Step6---------------------------------------
	//	Black	2016/11/25	�����[�t
//	if(EMV_status_check() == FAIL)
//		return;

	IncTraceNum();
	memcpy(trace_buf,gl_input.trace_no,TRACE_NO_HALF);

	memset(buf,0,9);
	memcpy(buf,&gl_input.tot_amount[1],5);
	api_emvk_SetTransPara(buf,trace_buf);	// ==== Echo 2014/5/3 ���H�n�D���n�ɴ����d������B�j��0xFFFFFFFF ====
//	EMV_result = api_emvk_SetTransPara(buf,trace_buf);
//	if(EMV_result == emvFailed)	// ==== Echo 2013/4/29 ��������B�j��0xFFFFFFFF��,API�|�^emvFailed ====
//	{
//		EMV_Message(2,0);
//		return;
//	}

	//***$$$-Step7,8------------------------------------
	if(EMV_Trans_MSI() == FAIL)
		return;

	//***$$$-Step9,10,11--------------------------------
	EMV_result = EMV_Trans_CM();
	if(EMV_result == FAIL)
		return;

	//-------Step12--------------------------------------
	//	Black	2016/11/25	�����[�t
//	if(EMV_status_check() == FAIL)
//		return;

	api_emvk_Completion(buf);

	//	Black	2020/12/18	M-TIP�����d�Kñ
#if CERT_MODE == CERT_MTIP
	UCHAR	buf2[100];
	EMV_result = api_emvk_GetDataElement( 0x9F, 0x34, buf2 );
	if(EMV_result == emvOK)
	{
		buf2[2] &= 0x3F; // effective only bit0-5
		if ((buf2[2] == 0x03)	//	CVR_PLAINTEXT_PIN_AND_SIGN
		|| (buf2[2] == 0x05)	//	CVR_ENCIPHERED_PIN_AND_SIGN
		|| (buf2[2] == 0x1E))	//	CVR_SIGN_PAPER
			gl_input.wave_sign = 0;	// �nñ�W
		else
			gl_input.wave_sign = 1;	// �ݨ�L���p�A���ݭnñ�W
	}
	else
		gl_input.wave_sign = 0;	// �nñ�W
#endif

	// ==== Echo 2015/4/20 �۰����u���v ====
	if(!memcmp(buf,"Z3",2) && !memcmp(gl_rsp_data.rspcode,"AF",2))
	{
		memcpy(gl_rsp_data.rspcode,"00",2);
		gl_input.emv_record_ptr = 0xFFFF;

		//	Black	2020/09/08	���ϥ�
//		result = Auto_Offline_Approve(1,SALE_UNDER_LMT,gl_input.tot_amount,0);
//		if(result == SUCCEED)
//			EMV_Message(7,0);
//		else
//		{
			memcpy(gl_rsp_data.rspcode,"Z3",2);
			EMV_Message(5,0);
//		}
		return;
	}

#if CERT_MODE == CERT_ADVT || CERT_MODE == CERT_UNIONPAY
	UCHAR	EMV_result2;
	UCHAR	buf2[300];
	EMV_result2 = api_emvk_GetDataElement(0x00,0x9B,(UCHAR *)&buf2[0]);
	if(EMV_result2 == emvOK)
	{
		if(buf2[0] != 0)
		{
			debug(0x9B, &buf2[2], buf2[0], 100);
		}
	}
#endif

	if (gl_Switch[SW_DEBUG] == ON)	//�����Ҧ� 	//	Black	2011/03/29
	{
		if(gl_debug_mode_flag & 0x02)	//alex 09/09/01
			EMV_Debug_print(); //alex 2003/09/30
	}

#if VERSION_MODE == VERSION_NCCC
	if(!memcmp(buf,"Y1",2) || !memcmp(buf,"Y3",2))//rex 2020/12/23 [NCCC] �ɧ� ���u���v��OL�ڵ� 
	{
		ClrDisplay2();
		DispText((UCHAR *)"OL �ڵ����     ", LINE3, 0, 16, FONT2);
		ErrorBeepDelay();
		EMV_Message(9,0);
		return;
	}
#endif

	if(!memcmp(buf,"00",2) || !memcmp(buf,"Y1",2) || !memcmp(buf,"Y4",2) || !memcmp(buf,"Y3",2))
	{
//remark by rex 2021/05/07 [NCCC][MPAS] ��֧�g�p�U 
#if VERSION_MODE != VERSION_NCCC
		EMV_result = GetEmvData(6);	// ==== TC_value ====
		if(EMV_result == FAIL)
			return;

		//	Black	2018/09/21	[AMEX]�x�sAP Label
		if(AmexHost() == TRUE)
		{
			EMV_result = GetEmvData(7);
			if(EMV_result == FAIL)
				return;
		}
#else
		if((gl_Switch[SW_MPAS] == ON) && (gl_rsp_data.status & MPAS_Indicator))	//rex 2021/05/07 [NCCC][MPAS] ���ܦ��ðϤ��p�B 
			;	//���� MPAS�Ҧ��A�B����� �p�B���v�ɡA���� TC 
		else
		{
			EMV_result = GetEmvData(6);	// ==== TC_value ====
			if(EMV_result == FAIL)
				return;
		}

//remark by rex 2020/12/06 [NCCC] �]�ݭn 
		EMV_result = GetEmvData(7);
		if(EMV_result == FAIL)
			return;
#endif

		EMV_result = GetEmvData(4);	// ==== Field_55 ====
#if VERSION_MODE != VERSION_NCCC
		if (RedeemHost() == TRUE)
			StoreEmvData(0,REDEEM_EMV_PAGE1,gl_emv_record_ptr_R);	// ==== �x�sField_55����� ====
		else if (InstallmentHost() == TRUE)
			StoreEmvData(0,INST_EMV_PAGE1,gl_emv_record_ptr_I);		// ==== �x�sField_55����� ====
		else if(AmexHost() == TRUE)	//	Black	2019/03/18	[AMEX]
			StoreEmvData(0,AMEX_EMV_PAGE1,gl_emv_record_ptr_A);	// ==== �x�sField_55����� ====
		else
#endif
//remark by rex 2020/12/05 [NCCC]
			StoreEmvData(0,CREDIT_EMV_PAGE1,gl_emv_record_ptr_C);	// ==== �x�sField_55����� ====
		if(EMV_result == FAIL)
			return;

		//	Black	2021/10/21	�]����T�n�ϥ�F56
//		if (gl_Credit_Host == 4)
		//	Black	2023/04/27	�]����T�B����H�U�n�ϥ�F56
		if ((gl_Credit_Host == 4) || (gl_Credit_Host == 6))
		{
			EMV_result = GetEmvData(5);	// ==== Field_56 ====
			if(EMV_result == FAIL)
				return;
		}
	}

	if(memcmp(gl_rsp_data.rspcode,"IE",RSPCODE_LEN))	// ==== Echo 2011/4/29 F55���L�����~ ====
		memcpy(gl_rsp_data.rspcode,buf,2);

	if(!memcmp(buf,"00",2))
	{
//remark by rex 2021/05/07 [NCCC][MPAS] ��֧�g�p�U 
#if VERSION_MODE != VERSION_NCCC
		gl_input.status |= OFFLINE;	// ==== �ΨӰ�TC UpLoad�ϥ� ====
#else
		if (gl_Switch[SW_MPAS] == ON)	//MPAS
		{
			if (!(gl_rsp_data.status & MPAS_Indicator))//���ߪ��ܡAMPAS�D���U�DMPAS��� 
				gl_input.status |= OFFLINE;	// �ΨӰ�TC UpLoad�ϥ�
		}
		else
			gl_input.status |= OFFLINE;	// ==== �ΨӰ�TC UpLoad�ϥ� ====
#endif
	}
	else if(!memcmp(buf,"Y4",2))	// ==== Y4 => Call Bank ====
		;
	else if(!memcmp(buf,"Y1",2) || !memcmp(buf,"Y3",2))	// ==== Y1 & Y3 => Offline ====
	{
		memcpy(gl_rsp_data.auth_code,gl_rsp_data.rspcode,2);
		gl_input.trans = SALE_UNDER_LMT;
		SetInputStatus(SALE_UNDER_LMT);

		// ==== Echo 2011/9/1 Y3���Trace No.�[1�קK�P�e����Reversal���Trace No.�ۦP�y������R�� ====
		if(!memcmp(buf,"Y3",2))
			IncTraceNum();

		ReadDateTime();   // ==== read Real time clock & store it in gl_lastdtg ====
		memcpy(&gl_rsp_data.auth_code[2],gl_lastdtg.minute,4);	// ==== �H "����" �������v�X ====
		compress((UCHAR *)&gl_rsp_data.year, (UCHAR *)&gl_lastdtg, 6);	// ==== ��s�ɶ� ====

		//	Black	2021/05/26	�s�W�x�sLMS_Invoice_No
		Update_LMS_Invoice_No();

#if VERSION_MODE == VERSION_NCCC
		IncRocNum();//rex 2020/12/17 [NCCC] ���O�o�̻ݭn�W�[ 
#endif

		PatTxCommonCode();

#if VERSION_MODE != VERSION_NCCC
		if (RedeemHost() == TRUE)
			StoreEmvData(0,REDEEM_EMV_PAGE1,gl_emv_record_ptr_R);	// ==== �x�sField_55����� ====
		else if (InstallmentHost() == TRUE)
			StoreEmvData(0,INST_EMV_PAGE1,gl_emv_record_ptr_I);		// ==== �x�sField_55����� ====
		else if(AmexHost() == TRUE)	//	Black	2019/03/18	[AMEX]
			StoreEmvData(0,AMEX_EMV_PAGE1,gl_emv_record_ptr_A);	// ==== �x�sField_55����� ====
		else
//remark by rex 2020/12/05 [NCCC]
#endif
			StoreEmvData(0,CREDIT_EMV_PAGE1,gl_emv_record_ptr_C);	// ==== �x�sField_55����� ====
	}
	else
	{
		//	Black	2020/09/02	���عq�H���LCall Bank
//		if(!ChkAcquirerOption(28) && (!memcmp(buf,"01",2) || !memcmp(buf,"02",2)))
//		{
//			// ==== Echo 2014/5/1 ���`SAM Slot-1���������]���ݰ�ATR ====
//			// ==== Echo 2014/5/1 �}��SAM�d�[�K ====
//			// ==== Echo 2007/10/27 "EMV_Message(4,0)"�S����Close SAM,��Call Bank�o�ͮɤU�@���A�P��Wave EDC�|reset ====
////			if(CubHost() == TRUE && (gl_lan_port.status == 2 || gl_lan_port.status == 3 || gl_lan_port.status == 4))
////				IFM_SAM_Close();		// ==== ����SAM2 ====	//	Black	2011/03/21	SAM1�A�u�ݭn�}�@���A�]��������
//
//			EMV_Message(4,0);
//			return;
//		}
		EMV_Message(5,0);
		return;
	}

	//	Black	2016/11/24	�קאּ���ϥ�F56�A�u�ϥ�F55
	//-------Step13--------------------------------------
#if VERSION_MODE != VERSION_NCCC
	if (RedeemHost() == TRUE)
	{
		gl_input.emv_record_ptr = gl_emv_record_ptr_R;	// ==== EMV IC�d�����ưO�� ====
//		StoreEmvData(1,REDEEM_EMV_PAGE1,gl_emv_record_ptr_R);
		StoreEmvData(0,REDEEM_EMV_PAGE1,gl_emv_record_ptr_R);
		gl_emv_record_ptr_R++;
	}
	else if (InstallmentHost() == TRUE)
	{
		gl_input.emv_record_ptr = gl_emv_record_ptr_I;	// ==== EMV IC�d�����ưO�� ====
//		StoreEmvData(1,INST_EMV_PAGE1,gl_emv_record_ptr_I);
		StoreEmvData(0,INST_EMV_PAGE1,gl_emv_record_ptr_I);
		gl_emv_record_ptr_I++;
	}
	else if(AmexHost() == TRUE)	//	Black	2018/09/21	[AMEX]
	{
		gl_input.emv_record_ptr = gl_emv_record_ptr_A;	// ==== EMV IC�d�����ưO�� ====
		StoreEmvData(0,AMEX_EMV_PAGE1,gl_emv_record_ptr_A);
		gl_emv_record_ptr_A++;
	}
	else
#endif
//remark by rex 2020/12/05 [NCCC]
	{
		gl_input.emv_record_ptr = gl_emv_record_ptr_C;	// ==== EMV IC�d�����ưO�� ====
//		StoreEmvData(1,CREDIT_EMV_PAGE1,gl_emv_record_ptr_C);
		StoreEmvData(0,CREDIT_EMV_PAGE1,gl_emv_record_ptr_C);
		gl_emv_record_ptr_C++;
	}

#if VERSION_MODE != VERSION_NCCC
	IncRocNum();
#endif
//remark by rex 2020/12/17 [NCCC] ���ɤ����ӦA�W�[ ROC�A���u�� ���u���v���ػݭn 

	TransAccptCommonCode();

#ifdef CHILDREN_PARADISE_VER
	CP_setTransactionResult(SUCCEED);
#endif

	if(gl_input.trans == SALE_SWIPE)
	{
		// ==== �W�Ƿ�����TC Upload ====
		if(TC_Upload() == SUCCEED)
		{
			// ==== Send offline trans ====
#if VERSION_MODE != VERSION_NCCC
			if (DinersHost() == FALSE)
				OfflineProc(ONLY_ONE);
			else
				ResetLine();
#else
			OfflineProc(ONLY_ONE);
#endif
		}
	}
	else
		ResetLine();

	// ==== Echo 2014/5/1 ���`SAM Slot-1���������]���ݰ�ATR ====
	// ==== Echo 2014/5/1 �}��SAM�d�[�K ====
//	if(CubHost() == TRUE && (gl_lan_port.status == 2 || gl_lan_port.status == 3 || gl_lan_port.status == 4))	//	Black	2011/03/21
//		IFM_SAM_Close();		// ==== ����SAM2 ====	//	Black	2011/03/21	SAM1�A�u�ݭn�}�@���A�]��������

	// ==== Echo 2015/11/18 �C�Lñ�� ====
	PrintLatestReceipt();
	EMV_Message(7,0);
}
#endif

#if VERSION_MODE != VERSION_NCCC
/* ----------------------------------------------
Name:	EMV_refund
Funct:
Input:
Output:
Global:
Black	2022/08/09	�s�W�x�s�Ȧ洹���h�f
------------------------------------------------*/
#if 0
void	EMV_refund(void)
{
	UCHAR	msg_emv_process[LEN_DISP] = {' ',0xB4,0xB9,0xA4,0xF9,0xA5,0x64,0xB3,0x42,0xB2,0x7A,0xA4,0xA4,'.','.','.'};					// ==== �����d�B�z��... ====
	UCHAR	msg_not_redeem[LEN_DISP] = {' ',' ',' ',0xA6,0xAC,0xB3,0xE6,0xA6,0xE6,0xA5,0xBC,0xB6,0x7D,' ',' ',' '};						// ==== ����楼�} ====
	FONT_DEF lMsg_check_digit_err = {{' ',' ',' ',' ',0xA5,0x64,0xB8,0xB9,0xBF,0xF9,0xBB,0x7E,' ',' ',' ',' '},{" CHECK DIGIT ERR"}};	// ==== �d�����~ ====
	UCHAR	msg_invaild_card[14] = {0xA6,0xB9,0xA5,0x64,0xA4,0xF9,0xAB,0x44,0xAB,0x48,0xA5,0xCE,0xA5,0x64};								// ==== ���d���D�H�Υd ====
	UCHAR	EMV_result;
	UCHAR	result;//,status;
	UCHAR	RecordFull_flag;
	UCHAR	buf[9];
//	UCHAR	result_flag = SUCCEED;
	UCHAR	trace_buf[TRACE_NO_HALF];

	UCHAR	aid_vis[5] = {0xA0,0x00,0x00,0x00,0x03};
	UCHAR	aid_mcc[5] = {0xA0,0x00,0x00,0x00,0x04};
	UCHAR	aid_jcb[5] = {0xA0,0x00,0x00,0x00,0x65};
	UCHAR	aid_aex[5] = {0xA0,0x00,0x00,0x00,0x25};	//	Black	2018/09/21	[AMEX]

	gl_input.emv_flag = 1;	// ==== �]�w��EMV�����d��� ====
	gl_input.entry_mode[1] = 0x52;

	gl_input.trans = ONL_REFUND_SW;	//	Black	2012/05/24	�s�W�]�w������O

	ClrDisplayDN(LINE2);
	DispText(msg_emv_process,LINE3,0,LEN_DISP,FONT2);	// ==== �����d�B�z��... ====

	//***$$$-Step1,2,3,4---------------------------------
	// ==== Echo 2015/11/12 ���ʨ�W�@�h ====
//	if(EMV_Trans_START() == FAIL)
//		return;

	//-------Step5----��J���---------------------------
	//	Black	2019/06/04	[ECR]AUXSTART()�n����OnLineRedeem2()��A�~�i���o���T������G�H�Υd�B���Q�B����
//	// ==== Echo 2013/2/6 [ECR]���Ⱦ��_�_�� ====
//	if (gl_ecr_flag == 1)
//	{
//		// ==== �^�e�d��"60" ====
//		result = AUXSTART();
//		if(result == FAIL)
//		{
//			EMV_Message(4,1);
//			return;
//		}
//
//		// ==== ��ܥ�����B ====
//		// ==== ��J���B(gl_input.tot_amount) ====
//		compress(&gl_input.tot_amount[1],glo_ecr_data.amount,5);	// ==== ���B�[���p�� ====
//		//	Black	2019/06/03	[ECR]��������b�Ĥ@����ܦ��Ⱦ�������B
////		DispAmount(gl_input.tot_amount,LINE1,FONT1 + attrREVERSE);		// ==== ��ܪ��B ====
//	}

	FlushKey();
	// ==== Echo 2007/11/13 [ECR]���Ⱦ��s�u ====

//	result = OnLineRedeem();
	//	Black	2019/01/25	���OnLineRedeem2()
	result = OnLineRedeem2();	//	Black	2019/07/19	���ϥζǤJ��
	if (result != TRUE)
	{
		if(gl_get_redeem_error_flag != 0)
		{
			if (gl_ecr_flag != 0)
			{
				if (gl_get_redeem_error_flag == 4)
					memcpy(glo_ecr_data.resp_code, "0004", 4);
				else if (gl_get_redeem_error_flag == 5)
					memcpy(glo_ecr_data.resp_code, "0005", 4);
				// �|�bEMV_Message���^�_ By Sam 2020/11/27
				// AUXsend(0xFF);
			}

			// ==== Echo 2014/5/20 ��H�Υd�ɭY�D�H�Υd���i��� ====
			// ==== Echo 2014/6/19 �ץ����ϥλ��p�d�ɫ�CANCEL��|���"���d���D�H�Υd"�T�� ====
//			if((gl_input.redeem_flag == 0) && memcmp(gl_emvk_aid,aid_vis,5) && memcmp(gl_emvk_aid,aid_mcc,5) && memcmp(gl_emvk_aid,aid_jcb,5))
			//	Black	2018/09/21	[AMEX]�s�WAMEX�����d
			if((gl_input.redeem_flag == 0) && memcmp(gl_emvk_aid,aid_vis,5) && memcmp(gl_emvk_aid,aid_mcc,5) && memcmp(gl_emvk_aid,aid_jcb,5) && memcmp(gl_emvk_aid,aid_aex,5))
			{
				ClrDisplayDN(LINE2);
				DispText(msg_invaild_card,LINE3,1,14,FONT2);	// ==== ���d���D�H�Υd ====
				LongBeep();
				EMV_Message(3,0);
				return;
			}

			Redeem_error_message();
			LongBeep();
			EMV_Message(3,0);
		}
		else
		{
			if(result != 0xFE)
				EMV_Message(4,0);
			else
				EMV_Message(4,1);
		}
		return;
	}

	//	Black	2019/06/04	[ECR]AUXSTART()�n����OnLineRedeem2()��A�~�i���o���T������G�H�Υd�B���Q�B����
	// ==== Echo 2013/2/6 [ECR]���Ⱦ��_�_�� ====
	if (gl_ecr_flag == 1)
	{
		// ==== �^�e�d��"60" ====
		result = AUXSTART();
		if(result == FAIL)
		{
			EMV_Message(4,1);
			return;
		}

		// ==== ��ܥ�����B ====
		// ==== ��J���B(gl_input.tot_amount) ====
		compress(&gl_input.tot_amount[1],glo_ecr_data.amount,5);		// ==== ���B�[���p�� ====
		//	Black	2019/06/03	[ECR]��������b�Ĥ@����ܦ��Ⱦ�������B
//		DispAmount(gl_input.tot_amount,LINE1,FONT1 + attrREVERSE);		// ==== ��ܪ��B ====
	}

	gl_fall_back_flag = 0;	// ==== ����Fall Back ====

	if(gl_get_card_error_flag == 2)
	{
		ClrDisplayDN(LINE2);
		if(CARD_RANGE() == FOUND)
			DispText(msg_not_redeem,LINE3,0,LEN_DISP,FONT2);	// ==== ����楼�} ====
		else
			DispBiText((FONT_DEF *)&msg_not_support, LINE3);	// ==== ���������d ====

		LongBeep();
		EMV_Message(3,0);
		return;
	}

	// ==== �b�w���ε��b���� ====
	RecordFull_flag = RecordFull();		//	Black	2011/07/20
	if(RecordFull_flag)
	{
		ClrDisplayDN(LINE2);
		if(RecordFull_flag == 1)
		{
			DispBiText((FONT_DEF *)&msg_close_batch, LINE2);	// ==== �妸�w�� ====
			DispBiText((FONT_DEF *)&msg_close_batch_1, LINE3);	// ==== �Х����b ====
		}
		else if(RecordFull_flag == 2)
			DispBiText((FONT_DEF *)&msg_close_batch_1, LINE3);	// ==== �Х����b ====

		LongBeep();
		EMV_Message(3,0);
		return;
	}

	// ==== Acquirer Table �\��}�� ====
	if(ChkAcquirerOption(24))
	{
		ClrDisplayDN(LINE2);
		DispText((UCHAR *)msg_no_auth,LINE3,0,LEN_DISP,FONT2);	// ==== ���\�ॼ�} ====
		LongBeep();
		EMV_Message(3,0);
		return;
	}

	if(gl_get_card_error_flag == 1)
	{
		ClrDisplayDN(LINE2);
		DispBiText(&lMsg_check_digit_err,LINE3);	// ==== �d�����~ ====
		LongBeep();

		EMV_Message(3,0);
		return;
	}

	//	Black	2016/11/24	���ϥ�
	// ==== ��ܤ����I�ڤ覡 ====
//	if(gl_input.redeem_flag == 2)
//	{
//		// ==== Echo 2007/11/13 [ECR]���Ⱦ��s�u ====
//		result = Choice_Payment();
//		if(result != SUCCEED)
//		{
//			if(result != 0xFE)
//				EMV_Message(4,0);
//			else
//				EMV_Message(4,1);
//			return;
//		}
//	}

	// ==== Pre-Dial ====
	if(gl_lan_port.status == 0)
		PredialOK();

	//	Black	2019/01/30	���׬O�_���P�����A�Ҧb�L�d�e��J���B
//	// ==== Echo 2014/1/23 �L�P���\��ɿ�J���B ====
//	if(gl_Wreader_status == FAIL)
//	{
//		if (gl_ecr_flag == 0)	// ==== Echo 2014/2/20 [ECR]���Ⱦ��_�_�� ====
//		{
//			if (!GetAmount(0))
//			{
//				EMV_Message(4,0);
//				return;
//			}
//		}
//	}

	//	Black	2016/11/24	���ϥ�
	// ==== Redeem Product Code�\�� ====
//	if(gl_input.redeem_flag == 1 && (gl_term_config.options[2] & 0x01) == 0x01)
//	{
//		// ==== Echo 2007/11/13 [ECR]���Ⱦ��s�u ====
//		result = GetProduct_Code(0);
//		if (result != SUCCEED)
//		{
//			if(result != 0xFE)
//				EMV_Message(4,0);
//			else
//				EMV_Message(4,1);
//			return;
//		}
//	}

	//	Black	2019/06/04	[ECR]��q��ECR�b�L�d��~��J���
	if (gl_ecr_flag == 1)
	{
		//	Black	2019/01/30	����L�d�e
		// ==== ��J�������ƥ\�� ====
		if(gl_input.redeem_flag == 2)
		{
			result = GetStage();
			if (result != SUCCEED)
			{
				if(result != 0xFE)
					EMV_Message(4,0);
				else
					EMV_Message(4,1);
				return;
			}
		}

		result = GetAdditional();
		if (result != SUCCEED)
		{
			EMV_Message(4,0);
			return;
		}

		// ==== �T�{���������� ====
		if(gl_input.redeem_flag == 2)
		{
			memset(gl_input.rrn,' ',12);				// ==== RRN ====
			memset(gl_input.fee_date,0xFF,4);			// ==== ����O�]�w���(YYYYMMDD) ====
			memset(gl_input.refund_tid,'0',8);			// ==== Refund Terminal ID ====
			memset(gl_input.fee_amount,0xFF,6);			// ==== ����O�`�B ====
			memset(gl_input.inst_amount,0xFF,6);		// ==== �`���B ====
			memset(gl_input.down_amount,0xFF,6);		// ==== �������B ====
			memset(gl_input.monthly_point,0xFF,6);		// ==== �C�����B ====
			memset(gl_input.offset_amount,0xFF,6);		// ==== ����O�C�����B ====	//	Black	2016/11/24
			gl_input.delay_month = 0xFF;				// ==== �������B�X�Ӥ��J�b ====

			gl_input.flexible_stageE = 0xFF;			// ==== �u�ʥI�ګe������ ====
			gl_input.flexible_stageL = 0xFF;			// ==== �u�ʥI�ګ������ ====
			memset(gl_input.flexible_payment,0xFF,6);	// ==== �u�ʥI�ګe���C������ ====
		}
		//	Black	2019/01/30	����L�d�e
	}

	// ==== ���Q��Υѳo����}�B�z!! ====
//	if(gl_input.redeem_flag != 0)
//	{
//		process_emv_redeem();
//		return;
//	}

	ClrDisplayDN(LINE2);
	DispText((UCHAR *)msg_connecting_1,LINE3,0,LEN_DISP,FONT2);	// ==== �B�z�� ====

	//-------Step6---------------------------------------
	//	Black	2016/11/25	�����[�t
//	if(EMV_status_check() == FAIL)
//		return;

	IncTraceNum();
	memcpy(trace_buf,gl_input.trace_no,TRACE_NO_HALF);

	memset(buf,0,9);
	memcpy(buf,&gl_input.tot_amount[1],5);
	api_emvk_SetTransPara(buf,trace_buf);	// ==== Echo 2014/5/3 ���H�n�D���n�ɴ����d������B�j��0xFFFFFFFF ====
//	EMV_result = api_emvk_SetTransPara(buf,trace_buf);
//	if(EMV_result == emvFailed)	// ==== Echo 2013/4/29 ��������B�j��0xFFFFFFFF��,API�|�^emvFailed ====
//	{
//		EMV_Message(2,0);
//		return;
//	}

	//***$$$-Step7,8------------------------------------
	if(EMV_Trans_MSI() == FAIL)
		return;

	//***$$$-Step9,10,11--------------------------------
	EMV_result = EMV_Trans_CM();
	if(EMV_result == FAIL)
		return;

	//-------Step12--------------------------------------
	//	Black	2016/11/25	�����[�t
//	if(EMV_status_check() == FAIL)
//		return;

	api_emvk_Completion(buf);

	if (gl_Switch[SW_DEBUG] == ON)	//�����Ҧ� 	//	Black	2011/03/29
	{
		if(gl_debug_mode_flag & 0x02)	//alex 09/09/01
			EMV_Debug_print(); //alex 2003/09/30
	}

	if(!memcmp(buf,"00",2))
	{
		EMV_result = GetEmvData(6);	// ==== TC_value ====
		if(EMV_result == FAIL)
			return;

		//	Black	2018/09/21	[AMEX]�x�sAP Label
		if(AmexHost() == TRUE)
		{
			EMV_result = GetEmvData(7);
			if(EMV_result == FAIL)
				return;
		}

		EMV_result = GetEmvData(4);	// ==== Field_55 ====

		if (RedeemHost() == TRUE)
			StoreEmvData(0,REDEEM_EMV_PAGE1,gl_emv_record_ptr_R);	// ==== �x�sField_55����� ====
		else if (InstallmentHost() == TRUE)
			StoreEmvData(0,INST_EMV_PAGE1,gl_emv_record_ptr_I);		// ==== �x�sField_55����� ====
		else if(AmexHost() == TRUE)	//	Black	2019/03/18	[AMEX]
			StoreEmvData(0,AMEX_EMV_PAGE1,gl_emv_record_ptr_A);	// ==== �x�sField_55����� ====
		else
			StoreEmvData(0,CREDIT_EMV_PAGE1,gl_emv_record_ptr_C);	// ==== �x�sField_55����� ====
		if(EMV_result == FAIL)
			return;
	}

	if(memcmp(gl_rsp_data.rspcode,"IE",RSPCODE_LEN))	// ==== Echo 2011/4/29 F55���L�����~ ====
		memcpy(gl_rsp_data.rspcode,buf,2);

	if(!memcmp(buf,"00",2))
		;
	else if(!memcmp(buf,"Y1",2) || !memcmp(buf,"Y3",2))	// ==== Y1 & Y3 => Offline ====
	{
		gl_rsp_data.rspcode[0] = 'Z';	// ==== �p�G�X�{Y1�PY3,�h�G�N��ܦ�Z1��Z3 ====
		EMV_Message(5,0);
		return;
	}
	else
	{
		//	Black	2020/09/02	���عq�H���LCall Bank
//		if(!ChkAcquirerOption(28) && (!memcmp(buf,"01",2) || !memcmp(buf,"02",2)))
//		{
//			// ==== Echo 2014/5/1 ���`SAM Slot-1���������]���ݰ�ATR ====
//			// ==== Echo 2014/5/1 �}��SAM�d�[�K ====
//			// ==== Echo 2007/10/27 "EMV_Message(4,0)"�S����Close SAM,��Call Bank�o�ͮɤU�@���A�P��Wave EDC�|reset ====
////			if(CubHost() == TRUE && (gl_lan_port.status == 2 || gl_lan_port.status == 3 || gl_lan_port.status == 4))
////				IFM_SAM_Close();		// ==== ����SAM2 ====	//	Black	2011/03/21	SAM1�A�u�ݭn�}�@���A�]��������
//
//			EMV_Message(4,0);
//			return;
//		}
		EMV_Message(5,0);
		return;
	}

	//	Black	2016/11/24	�קאּ���ϥ�F56�A�u�ϥ�F55
	//-------Step13--------------------------------------
	if (RedeemHost() == TRUE)
	{
		gl_input.emv_record_ptr = gl_emv_record_ptr_R;	// ==== EMV IC�d�����ưO�� ====
//		StoreEmvData(1,REDEEM_EMV_PAGE1,gl_emv_record_ptr_R);
		StoreEmvData(0,REDEEM_EMV_PAGE1,gl_emv_record_ptr_R);
		gl_emv_record_ptr_R++;
	}
	else if (InstallmentHost() == TRUE)
	{
		gl_input.emv_record_ptr = gl_emv_record_ptr_I;	// ==== EMV IC�d�����ưO�� ====
//		StoreEmvData(1,INST_EMV_PAGE1,gl_emv_record_ptr_I);
		StoreEmvData(0,INST_EMV_PAGE1,gl_emv_record_ptr_I);
		gl_emv_record_ptr_I++;
	}
	else if(AmexHost() == TRUE)	//	Black	2018/09/21	[AMEX]
	{
		gl_input.emv_record_ptr = gl_emv_record_ptr_A;	// ==== EMV IC�d�����ưO�� ====
		StoreEmvData(0,AMEX_EMV_PAGE1,gl_emv_record_ptr_A);
		gl_emv_record_ptr_A++;
	}
	else
	{
		gl_input.emv_record_ptr = gl_emv_record_ptr_C;	// ==== EMV IC�d�����ưO�� ====
//		StoreEmvData(1,CREDIT_EMV_PAGE1,gl_emv_record_ptr_C);
		StoreEmvData(0,CREDIT_EMV_PAGE1,gl_emv_record_ptr_C);
		gl_emv_record_ptr_C++;
	}

	IncRocNum();

	TransAccptCommonCode();

//	if(gl_input.trans == SALE_SWIPE)
//	{
//		// ==== �W�Ƿ�����TC Upload ====
//		if(TC_Upload() == SUCCEED)
//		{
			// ==== Send offline trans ====
			if (DinersHost() == FALSE)
				OfflineProc(ONLY_ONE);
//			else
//				ResetLine();
//		}
//	}
//	else
		ResetLine();

	// ==== Echo 2014/5/1 ���`SAM Slot-1���������]���ݰ�ATR ====
	// ==== Echo 2014/5/1 �}��SAM�d�[�K ====
//	if(CubHost() == TRUE && (gl_lan_port.status == 2 || gl_lan_port.status == 3 || gl_lan_port.status == 4))	//	Black	2011/03/21
//		IFM_SAM_Close();		// ==== ����SAM2 ====	//	Black	2011/03/21	SAM1�A�u�ݭn�}�@���A�]��������

	// ==== Echo 2015/11/18 �C�Lñ�� ====
	PrintLatestReceipt();
	EMV_Message(7,0);
}
#endif
#endif

/* ----------------------------------------------
Name:	EMV_preauth
Funct:	
Input:  None
Output: 
Global:	
Black	2019/01/25	���OnLineRedeem2()
Black	2018/09/21	[AMEX]�s�WAMEX�����d
Black	2018/10/15	[AMEX]�s�W�x�sAP Label
rex 	2020/12/06 [NCCC]�]�n�B�z AP Label
rex		2022/09/12	[NCCC][PIN���X��1]
Black	2023/04/19	����H�U�n���R�Kñ�W�T��
------------------------------------------------*/
#if 0
void	EMV_preauth(void)
{
	UCHAR	msg_emv_process[LEN_DISP] = {' ',0xB4,0xB9,0xA4,0xF9,0xA5,0x64,0xB3,0x42,0xB2,0x7A,0xA4,0xA4,'.','.','.'};	// ==== �����d�B�z��... ====
#if VERSION_MODE != VERSION_NCCC
	UCHAR	msg_not_redeem[LEN_DISP] = {' ',' ',' ',0xA6,0xAC,0xB3,0xE6,0xA6,0xE6,0xA5,0xBC,0xB6,0x7D,' ',' ',' '};		// ==== ����楼�} ====
#endif
	FONT_DEF lMsg_check_digit_err = {{' ',' ',' ',' ',0xA5,0x64,0xB8,0xB9,0xBF,0xF9,0xBB,0x7E,' ',' ',' ',' '},{" CHECK DIGIT ERR"}};	// ==== �d�����~ ====
	UCHAR	msg_invaild_card[14] = {0xA6,0xB9,0xA5,0x64,0xA4,0xF9,0xAB,0x44,0xAB,0x48,0xA5,0xCE,0xA5,0x64};	// ==== ���d���D�H�Υd ====
	UCHAR	EMV_result;
	UCHAR	result;
	UCHAR	RecordFull_flag;
	UCHAR	buf[9];
//	UCHAR	randbuf[4];
//	UCHAR	randnum[1];
//	UCHAR	i;
//	UCHAR	result_flag;
	UCHAR	trace_buf[TRACE_NO_HALF];

	UCHAR	aid_vis[5] = {0xA0,0x00,0x00,0x00,0x03};
	UCHAR	aid_mcc[5] = {0xA0,0x00,0x00,0x00,0x04};
	UCHAR	aid_jcb[5] = {0xA0,0x00,0x00,0x00,0x65};
	UCHAR	aid_aex[5] = {0xA0,0x00,0x00,0x00,0x25};	//	Black	2018/09/21	[AMEX]

	gl_input.emv_flag = 1;	// ==== �]�w��EMV�����d��� ====
//	gl_input.entry_mode[1] = 0x52;
//remark by rex 2022/09/12 [NCCC][PIN���X��1]
#if VERSION_MODE != VERSION_NCCC
	gl_input.entry_mode[1] = 0x52;
#else
	gl_input.entry_mode[1] = 0x51;
#endif

	DispHeader(1);

	DispText(msg_emv_process,LINE3,0,LEN_DISP,FONT2);	// ==== �����d�B�z��... =====

	//***$$$-Step1,2,3,4---------------------------------
	if(EMV_Trans_START() == FAIL)
		return;

	//	Black	2017/01/12	����OnLineRedeem()�e
	// ==== Echo 2014/5/20 ��H�Υd�ɭY�D�H�Υd���i��� ====
	// ==== Echo 2014/6/19 �ץ����ϥλ��p�d�ɫ�CANCEL��|���"���d���D�H�Υd"�T�� ====
//	if((gl_get_redeem_error_flag != 6) && (gl_input.redeem_flag == 0) && memcmp(gl_emvk_aid,aid_vis,5) && memcmp(gl_emvk_aid,aid_mcc,5) && memcmp(gl_emvk_aid,aid_jcb,5))
//	if(memcmp(gl_emvk_aid,aid_vis,5) && memcmp(gl_emvk_aid,aid_mcc,5) && memcmp(gl_emvk_aid,aid_jcb,5))
	//	Black	2018/09/21	[AMEX]�s�WAMEX�����d
	if(memcmp(gl_emvk_aid,aid_vis,5) && memcmp(gl_emvk_aid,aid_mcc,5) && memcmp(gl_emvk_aid,aid_jcb,5) && memcmp(gl_emvk_aid,aid_aex,5))
	{
		ClrDisplayDN(LINE2);
		DispText(msg_invaild_card,LINE3,1,14,FONT2);	// ==== ���d���D�H�Υd ====
		LongBeep();
		EMV_Message(3,0);
		return;
	}

	//-------Step5----��J���---------------------------
	FlushKey();
//	result = OnLineRedeem();
	//	Black	2019/01/25	���OnLineRedeem2()
	gl_input.redeem_flag = 0;
	result = OnLineRedeem2();	//	Black	2019/07/19	���ϥζǤJ��
	if (result != TRUE)
	{
		if (gl_ecr_flag != 0)
		{
			if (gl_get_redeem_error_flag == 4)
				memcpy(glo_ecr_data.resp_code, "0004", 4);
			else if (gl_get_redeem_error_flag == 5)
				memcpy(glo_ecr_data.resp_code, "0005", 4);
			// �|�bEMV_Message���^�_ By Sam 2020/11/27
			// AUXsend(0xFF);
		}

		// ==== Echo 2014/4/18 �ץ��o�Ϳ��~�ɤ���ܿ��~�T�������D ====
		if(gl_get_redeem_error_flag != 0)
		{
			//	Black	2017/01/12	����OnLineRedeem()�e
			// ==== Echo 2014/5/20 ��H�Υd�ɭY�D�H�Υd���i��� ====
			// ==== Echo 2014/6/19 �ץ����ϥλ��p�d�ɫ�CANCEL��|���"���d���D�H�Υd"�T�� ====
//			if((gl_get_redeem_error_flag != 6) && (gl_input.redeem_flag == 0) && memcmp(gl_emvk_aid,aid_vis,5) && memcmp(gl_emvk_aid,aid_mcc,5) && memcmp(gl_emvk_aid,aid_jcb,5))
//			{
//				ClrDisplayDN(LINE2);
//				DispText(msg_invaild_card,LINE3,1,14,FONT2);	// ==== ���d���D�H�Υd ====
//				LongBeep();
//				EMV_Message(3,0);
//				return;
//			}

			Redeem_error_message();
			LongBeep();
			EMV_Message(3,0);
		}
		else
			EMV_Message(4,0);
		return;
	}

	if(gl_get_card_error_flag == 2)
	{
		ClrDisplayDN(LINE2);
#if VERSION_MODE != VERSION_NCCC
		if(CARD_RANGE() == FOUND)
			DispText(msg_not_redeem,LINE3,0,LEN_DISP,FONT2);	// ==== ����楼�} ====
		else
#endif
//remark by rex 2020/11/06 [NCCC]
			DispBiText((FONT_DEF *)&msg_not_support, LINE3);	// ==== ���������d ====

		LongBeep();
		EMV_Message(3,0);
		return;
	}

	// ==== �b�w���ε��b���� ====
	RecordFull_flag = RecordFull();
	if(RecordFull_flag)
	{
		ClrDisplayDN(LINE2);
		if(RecordFull_flag == 1)
		{
			DispBiText((FONT_DEF *)&msg_close_batch, LINE2);	// ==== �妸�w�� ====
			DispBiText((FONT_DEF *)&msg_close_batch_1, LINE3);	// ==== �Х����b ====
		}
		else if(RecordFull_flag == 2)
			DispBiText((FONT_DEF *)&msg_close_batch_1, LINE3);	// ==== �Х����b ====

		LongBeep();
		EMV_Message(3,0);
		return;
	}

	// ==== Acquirer Table �\��}�� ====
	if(ChkAcquirerOption(27))
	{
		ClrDisplayDN(LINE2);
		DispText((UCHAR *)msg_no_auth,LINE3,0,LEN_DISP,FONT2);	// ==== ���\�ॼ�} ====
		LongBeep();
		EMV_Message(3,0);
		return;
	}

	if(gl_get_card_error_flag == 1)
	{
		ClrDisplayDN(LINE2);
		DispBiText(&lMsg_check_digit_err,LINE3);	// ==== �d�����~ ====

		LongBeep();
		EMV_Message(3,0);
		return;
	}

	if(gl_lan_port.status == 0)	// ==== Pre-Dial ====
		PredialOK();

	gl_fall_back_flag = 0;	// ==== ����Fall Back ====

	//	Black	2019/01/30	����L�d�e
//	if (!GetAmount(0))
//	{
//		EMV_Message(4,0);
//		return;
//	}
//
//	if (GetAdditional() == CANCELLED)
//	{
//		EMV_Message(4,0);
//		return;
//	}

	ClrDisplayDN(LINE2);
	DispText((UCHAR *)msg_connecting_1,LINE3,0,LEN_DISP,FONT2);	// ==== �B�z�� ====

	//-------Step6---------------------------------------
	//	Black	2016/11/25	�����[�t
//	if(EMV_status_check() == FAIL)
//		return;

	IncTraceNum();
	memcpy(trace_buf,gl_input.trace_no,TRACE_NO_HALF);

	memset(buf,0,9);
	memcpy(buf,&gl_input.tot_amount[1],5);
	api_emvk_SetTransPara(buf,gl_input.trace_no);	// ==== Echo 2014/5/3 ���H�n�D���n�ɴ����d������B�j��0xFFFFFFFF ====
//	EMV_result = api_emvk_SetTransPara(buf,gl_input.trace_no);
//	if(EMV_result == emvFailed)	// ==== Echo 2013/4/29 ��������B�j��0xFFFFFFFF��,API�|�^emvFailed ====
//	{
//		EMV_Message(2,0);
//		return;
//	}

	//***$$$-Step7,8------------------------------------
	if(EMV_Trans_MSI() == FAIL)
		return;

	//***$$$-Step9,10,11--------------------------------
	if(EMV_Trans_CM() == FAIL)
		return;
	
	//-------Step12--------------------------------------
	//	Black	2016/11/25	�����[�t
//	if(EMV_status_check() == FAIL)
//		return;

	api_emvk_Completion(buf);

	if(!memcmp(buf,"00",2))
	{
		EMV_result = GetEmvData(6);	// ==== TC_value ====
		if(EMV_result == FAIL)
			return;

#if VERSION_MODE != VERSION_NCCC
		//	Black	2018/10/15	[AMEX]�x�sAP Label
		if(AmexHost() == TRUE)
		{
			EMV_result = GetEmvData(7);
			if(EMV_result == FAIL)
				return;
		}
#else
//remark by rex 2020/12/06 [NCCC] �]�ݭn 
		EMV_result = GetEmvData(7);
		if(EMV_result == FAIL)
			return;
#endif
	}

	if(memcmp(gl_rsp_data.rspcode,"IE",RSPCODE_LEN))	// ==== Echo 2011/4/29 F55���L�����~ ====
		memcpy(gl_rsp_data.rspcode,buf,2);

	if(!memcmp(buf,"00",2))
		;
	else if(!memcmp(buf,"Y1",2) || !memcmp(buf,"Y3",2))		//	Black	2011/03/28	�i�H���������u���v���
	{
		gl_rsp_data.rspcode[0] = 'Z';	// ==== �p�G�X�{Y1�PY3,�h�G�N��ܦ�Z1��Z3 ====
		EMV_Message(5,0);
		return;
	}
	else
	{						//	Black	2011/03/28
		EMV_Message(5,0);
		return;
	}

	//-------Step13--------------------------------------
	IncRocNum();
	UpdateCurRec();

	//	Black	2023/04/19	����H�U�n���R�Kñ�W�T��
#if VERSION_MODE != VERSION_NCCC
	if (gl_Credit_Host == 6)	//	����H�U
		NoSign_Process();	// ==== �Kñ�W���R�B�z(�u��CTCB�@����Sale �P Pre-Auth�A��) ====
#endif

	// ==== Echo 2015/11/18 �Ȧs������ ====
	memcpy(glo_buffer,(UCHAR *)&gl_cur_record,RECORD_LEN);

//	LoadRecToPrtBuf(NOT_RETREIVE);
//	ClrDisplayDN(LINE2);

//	result_flag = SearchOffline();	// ==== �w���j�M�O�_��Offline�����(�S�����ܥi�H���_�u) ====
//	PrintSlip(DISABLE_KBD);

#if VERSION_MODE != VERSION_NCCC
//	if(result_flag == SUCCEED)
//	{
		// ==== Send offline trans ====
		if (DinersHost() == FALSE)
			OfflineProc(ONLY_ONE);
		else
			ResetLine();
//	}
//	else
//		ResetLine();
#else
	OfflineProc(ONLY_ONE);
#endif

	// ==== Echo 2015/11/18 ���^������ ====
	memcpy((UCHAR *)&gl_cur_record,glo_buffer,RECORD_LEN);

	// ==== Echo 2015/11/18 �C�Lñ�� ====
	LoadRecToPrtBuf(NOT_RETREIVE);
	PrintSlip(DISABLE_KBD);

	EMV_Message(7,0);
	gl_change = TRUE;
}
#endif

//#if VERSION_MODE != VERSION_NCCC
/* ----------------------------------------------
Name:	UICC_sale
Funct:	���p�����d�P��
Input:  mode :	0 = �P��
				1 = �w���v
Output:
Global:
Echo 2014/5/3
Black	2019/06/04	[ECR]��q��ECR�b�L�d��~��J���
Black	2021/05/26	�s�W�x�sLMS_Invoice_No
Black	2021/10/21	�]����T�n�ϥ�F56
rex		2022/07/22	[NCCC][�䴩���p�\��]
Black	2023/04/27	�]����T�B����H�U�n�ϥ�F56
------------------------------------------------*/
#if 0
void	UICC_sale(UCHAR mode)
{
	UCHAR	msg_emv_process[LEN_DISP] = {' ',0xB4,0xB9,0xA4,0xF9,0xA5,0x64,0xB3,0x42,0xB2,0x7A,0xA4,0xA4,'.','.','.'};					// ==== �����d�B�z��... ====
	FONT_DEF lMsg_check_digit_err = {{' ',' ',' ',' ',0xA5,0x64,0xB8,0xB9,0xBF,0xF9,0xBB,0x7E,' ',' ',' ',' '},{" CHECK DIGIT ERR"}};	// ==== �d�����~ ====
//	UCHAR	msg_invaild_card[14] = {0xA6,0xB9,0xA5,0x64,0xA4,0xF9,0xAB,0x44,0xBB,0xC8,0xC1,0x70,0xA5,0x64};								// ==== ���d���D���p�d ====
	UCHAR	EMV_result;
	UCHAR	result;//,status;
	UCHAR	RecordFull_flag;
	UCHAR	buf[9];
	UCHAR	trace_buf[TRACE_NO_HALF];

	UCHAR	aid_cup[7] = {0xA0,0x00,0x00,0x03,0x33,0x01,0x01};

	gl_input.emv_flag = 1;	// ==== �]�w��EMV�����d��� ====
	gl_input.entry_mode[1] = 0x52;

	ClrDisplayDN(LINE2);
	DispText(msg_emv_process,LINE3,0,LEN_DISP,FONT2);	// ==== �����d�B�z��... ====

	//***$$$-Step1,2,3,4---------------------------------
	// ==== Echo 2015/11/12 ���ʨ�W�@�h ====
//	if(EMV_Trans_START() == FAIL)
//		return;

	//	Black	2017/01/12	�ק����
	// ==== �D���p�����d���i��� ====
	if(memcmp(gl_emvk_aid,aid_cup,7))
	{
		DispNotCUP(1);	// ==== Echo 2015/9/15 ��ܫD���p�d�T�� ====
//		ClrDisplayDN(LINE2);
//		DispText(msg_invaild_card,LINE3,1,14,FONT2);	// ==== ���d���D���p�d ====
//		LongBeep();
//		EMV_Message(3,0);
		return;
	}

	gl_input.product_code = gl_emvk_aid[7];	// ==== �x�s���p�����d�̫�@�XAID ====

#if VERSION_MODE != VERSION_NCCC			//rex 2022/06/28 [NCCC][�䴩���p�\��]
	//	Black	2017/04/17	�N�d�O�]��CUP
	result = Set_CUP_Issuer();
#endif

	//-------Step5----��J���---------------------------
	// ==== Echo 2013/2/6 [ECR]���Ⱦ��_�_�� ====
	if (gl_ecr_flag == 1)
	{
		// ==== �^�e�d��"60" ====
		result = AUXSTART();
		if(result == FAIL)
		{
			EMV_Message(4,1);
			return;
		}

		// ==== ��ܥ�����B ====
		// ==== ��J���B(gl_input.tot_amount) ====
		compress(&gl_input.tot_amount[1],glo_ecr_data.amount,5);		// ==== ���B�[���p�� ====
		//	Black	2019/06/03	[ECR]��������b�Ĥ@����ܦ��Ⱦ�������B
//		DispAmount(gl_input.tot_amount,LINE1,FONT1 + attrREVERSE);		// ==== ��ܪ��B ====
	}

	FlushKey();

	gl_fall_back_flag = 0;	// ==== ����Fall Back ====

	// ==== �b�w���ε��b���� ====
	RecordFull_flag = RecordFull();		//	Black	2011/07/20
	if(RecordFull_flag)
	{
		ClrDisplayDN(LINE2);
		if(RecordFull_flag == 1)
		{
			DispBiText((FONT_DEF *)&msg_close_batch, LINE2);	// ==== �妸�w�� ====
			DispBiText((FONT_DEF *)&msg_close_batch_1, LINE3);	// ==== �Х����b ====
		}
		else if(RecordFull_flag == 2)
			DispBiText((FONT_DEF *)&msg_close_batch_1, LINE3);	// ==== �Х����b ====

		LongBeep();
		EMV_Message(3,0);
		return;
	}

	// ==== Acquirer Table �\��}�� ====
	if(ChkAcquirerOption(24))
	{
		ClrDisplayDN(LINE2);
		DispText((UCHAR *)msg_no_auth,LINE3,0,LEN_DISP,FONT2);	// ==== ���\�ॼ�} ====
		LongBeep();
		EMV_Message(3,0);
		return;
	}

	if(gl_get_card_error_flag == 1)
	{
		ClrDisplayDN(LINE2);
		DispBiText(&lMsg_check_digit_err,LINE3);	// ==== �d�����~ ====
		LongBeep();

		EMV_Message(3,0);
		return;
	}

	// ==== Pre-Dial ====
	if(gl_lan_port.status == 0)
		PredialOK();

	//	Black	2019/01/30	���׬O�_���P�����A�Ҧb�L�d�e��J���B
//	// ==== Echo 2016/5/24 �P�ⱵDongle�ɤ���J���B ====
//	if((gl_Wreader_status == SUCCEED) && (mode == 0))
//		;
//	else
//	{
//		if (gl_ecr_flag == 0)	// ==== Echo 2014/2/20 [ECR]���Ⱦ��_�_�� ====
//		{
//			if (!GetAmount(0))
//			{
//				EMV_Message(4,0);
//				return;
//			}
//		}
//	}

	//	Black	2019/06/04	[ECR]��q��ECR�b�L�d��~��J���
	if (gl_ecr_flag == 1)
	{
		result = GetAdditional();
		if (result != SUCCEED)
		{
			EMV_Message(4,0);
			return;
		}
	}

	// ==== ��JPIN ====
	// ==== Echo 2014/6/18 ���EMV Online PIN ====
/*	result = GetPinPad(gl_pinpad.mode);
	if(result != SUCCEED)
	{
		if (gl_ecr_flag != 0)	// ==== Echo 2013/2/3 [ECR]���Ⱦ��_�_�� ====
			EMV_Message(4,0);
		return;
	}
*/
	gl_input.redeem_flag = 3;

	memset(gl_input.offset_amount,'0',6);	// ==== ���p�d����Ǹ� ====
	memset(gl_input.net_amount,'0',4);		// ==== ���p�d�M���(MMDD) ====
	memset(gl_input.utilize_point,'0',4);	// ==== ���p�d����ǿ���(MMDD) ====
	memset(gl_input.award_point,'0',6);		// ==== ���p�d����ǿ�ɶ�(hhmmdd) ====
	memset(gl_input.fee_date,'0',4);		// ==== ��l������(MMDD) ====
	memset(gl_input.refund_tid,'0',8);		// ==== ���P���x�h�f������Terminal ID ====

	// ==== ����JPIN POS Entry Mode = 0x51,�S��JPIN POS Entry Mode = 0x52 ====
	// ==== Echo 2014/6/18 ���EMV Online PIN,POS Entry Mode�n���b�s�u�e ====
/*	gl_input.entry_mode[0] = 0;
	if(gl_pin_entry == 1)
		gl_input.entry_mode[1] = 0x51;
	else
		gl_input.entry_mode[1] = 0x52;
*/
	ClrDisplayDN(LINE2);
	DispText((UCHAR *)msg_connecting_1,LINE3,0,LEN_DISP,FONT2);	// ==== �B�z�� ====

	//-------Step6---------------------------------------
	//	Black	2016/11/25	�����[�t
//	if(EMV_status_check() == FAIL)
//		return;

	IncTraceNum();
	memcpy(trace_buf,gl_input.trace_no,TRACE_NO_HALF);

	memset(buf,0,9);
	memcpy(buf,&gl_input.tot_amount[1],5);
	api_emvk_SetTransPara(buf,trace_buf);	// ==== Echo 2014/5/3 ���H�n�D���n�ɴ����d������B�j��0xFFFFFFFF ====
//	EMV_result = api_emvk_SetTransPara(buf,trace_buf);
//	if(EMV_result == emvFailed)	// ==== Echo 2013/4/29 ��������B�j��0xFFFFFFFF��,API�|�^emvFailed ====
//	{
//		EMV_Message(2,0);
//		return;
//	}

	//***$$$-Step7,8------------------------------------
	if(EMV_Trans_MSI() == FAIL)
			return;

	//***$$$-Step9,10,11--------------------------------
	EMV_result = EMV_Trans_CM();
	if(EMV_result == FAIL)
		return;

	//-------Step12--------------------------------------
	//	Black	2016/11/25	�����[�t
//	if(EMV_status_check() == FAIL)
//		return;

	api_emvk_Completion(buf);

#if CERT_MODE == CERT_ADVT || CERT_MODE == CERT_UNIONPAY
	UCHAR	EMV_result2;
	UCHAR	buf2[300];
	EMV_result2 = api_emvk_GetDataElement(0x00,0x9B,(UCHAR *)&buf2[0]);
	if(EMV_result2 == emvOK)
	{
		if(buf2[0] != 0)
		{
			debug(0x9B, &buf2[2], buf2[0], 100);
		}
	}
#endif

	if (gl_Switch[SW_DEBUG] == ON)	//�����Ҧ� 	//	Black	2011/03/29
	{
		if(gl_debug_mode_flag & 0x02)	//alex 09/09/01
			EMV_Debug_print(); //alex 2003/09/30
	}

	if(!memcmp(buf,"00",2) || !memcmp(buf,"Y1",2) || !memcmp(buf,"Y3",2))
	{
		EMV_result = GetEmvData(6);	// ==== TC_value ====
		if(EMV_result == FAIL)
			return;

		// ==== �u���P��ݭn�AŪ��F55 & F56 ====
		if(mode == 0)
		{
			EMV_result = GetEmvData(4);	// ==== Field_55 ====
#if VERSION_MODE != VERSION_NCCC			//rex 2022/06/28 [NCCC][�䴩���p�\��]
			//	Black	2022/12/27	���p�d
			if ((gl_Credit_Host == 2) || (gl_Credit_Host == 6))
				StoreEmvData(0,CUP_EMV_PAGE1,gl_emv_record_ptr_U);	// ==== �x�sField_55����� ====
			else
#endif
				StoreEmvData(0,CREDIT_EMV_PAGE1,gl_emv_record_ptr_C);	// ==== �x�sField_55����� ====
			if(EMV_result == FAIL)
				return;

			//	Black	2021/10/21	�]����T�n�ϥ�F56
//			if (gl_Credit_Host == 4)
			//	Black	2023/04/27	�]����T�B����H�U�n�ϥ�F56
			if ((gl_Credit_Host == 4) || (gl_Credit_Host == 6))
			{
				EMV_result = GetEmvData(5);	// ==== Field_56 ====
				if(EMV_result == FAIL)
					return;
			}
		}
	}

	if(memcmp(gl_rsp_data.rspcode,"IE",RSPCODE_LEN))	// ==== Echo 2011/4/29 F55���L�����~ ====
		memcpy(gl_rsp_data.rspcode,buf,2);

	if(!memcmp(buf,"00",2))
	{
		// ==== �u���H�Υd�ݰ�TC Upload�~�n�N����]�����u���A ====
		if(mode == 0)
			gl_input.status |= OFFLINE;	// ==== �ΨӰ�TC UpLoad�ϥ� ====
	}
	else if((!memcmp(buf,"Y1",2) || !memcmp(buf,"Y3",2)) && (mode == 0))	// ==== Y1 & Y3 => Offline,�u���P��䴩Offline ====
	{
		memcpy(gl_rsp_data.auth_code,gl_rsp_data.rspcode,2);
		gl_input.trans = SALE_UNDER_LMT;
		SetInputStatus(SALE_UNDER_LMT);

		// ==== Echo 2011/9/1 Y3���Trace No.�[1�קK�P�e����Reversal���Trace No.�ۦP�y������R�� ====
		if(!memcmp(buf,"Y3",2))
			IncTraceNum();

		ReadDateTime();   // ==== read Real time clock & store it in gl_lastdtg ====
		memcpy(&gl_rsp_data.auth_code[2],gl_lastdtg.minute,4);	// ==== �H "����" �������v�X ====
		compress((UCHAR *)&gl_rsp_data.year, (UCHAR *)&gl_lastdtg, 6);	// ==== ��s�ɶ� ====

		//	Black	2021/05/26	�s�W�x�sLMS_Invoice_No
		Update_LMS_Invoice_No();

		PatTxCommonCode();
#if VERSION_MODE != VERSION_NCCC			//rex 2022/06/28 [NCCC][�䴩���p�\��]
		//	Black	2022/12/27	���p�d
		if ((gl_Credit_Host == 2) || (gl_Credit_Host == 6))
			StoreEmvData(0,CUP_EMV_PAGE1,gl_emv_record_ptr_U);	// ==== �x�sField_55����� ====
		else
#endif
			StoreEmvData(0,CREDIT_EMV_PAGE1,gl_emv_record_ptr_C);	// ==== �x�sField_55����� ====	// ==== Echo 2014/7/14 �ץ����p�d��������x�s��m ====
	}
	else
	{
		EMV_Message(5,0);
		return;
	}

	//	Black	2016/11/24	�קאּ���ϥ�F56�A�u�ϥ�F55
	//-------Step13--------------------------------------
#if VERSION_MODE != VERSION_NCCC			//rex 2022/06/28 [NCCC][�䴩���p�\��]
	//	Black	2022/12/27	���p�d
	if ((gl_Credit_Host == 2) || (gl_Credit_Host == 6))
	{
		gl_input.emv_record_ptr = gl_emv_record_ptr_U;	// ==== EMV IC�d�����ưO�� ====
		StoreEmvData(0,CUP_EMV_PAGE1,gl_emv_record_ptr_U);
		gl_emv_record_ptr_U++;
	}
	else
#endif
	{
		gl_input.emv_record_ptr = gl_emv_record_ptr_C;	// ==== EMV IC�d�����ưO�� ====
//		StoreEmvData(1,CREDIT_EMV_PAGE1,gl_emv_record_ptr_C);
		StoreEmvData(0,CREDIT_EMV_PAGE1,gl_emv_record_ptr_C);
		gl_emv_record_ptr_C++;

#if VERSION_MODE == VERSION_NCCC		//rex 2022/07/06 [NCCC][�䴩���p�\��]
		//rex 2022/07/06 [NCCC][�䴩���p�\��]    CUP�����Ʀs�Jgl_cur_record
//		memcpy(gl_input.offset_amount	,gl_rsp_data.offset_amount	,TRACE_NO_HALF);//1-CUP Trace number 
//		memcpy(gl_input.utilize_point	,gl_rsp_data.utilize_point	,DATE_LEN);		//2-CUP Transaction Date
//		memcpy(gl_input.award_point		,gl_rsp_data.award_point	,TIME_LEN);		//3-CUP Transaction Time
//		memcpy(gl_input.CUP_rrn			,gl_rsp_data.CUP_rrn		,6);			//4-CUP RRN
//		memcpy(gl_input.net_amount		,gl_rsp_data.net_amount		,2);			//5-CUP Settlement Date
//remark by rex 2022/07/22 [NCCC][�䴩���p�\��] ��Ӥ��ؼg�k�A�����Y 
		memcpy(gl_input.offset_amount	,gl_rsp_data.offset_amount	,TRACE_NO_HALF*2);	//1-CUP Trace number 
		memcpy(gl_input.utilize_point	,gl_rsp_data.utilize_point	,DATE_LEN*2);		//2-CUP Transaction Date
		memcpy(gl_input.award_point		,gl_rsp_data.award_point	,TIME_LEN*2);		//3-CUP Transaction Time
		memcpy(gl_input.CUP_rrn			,gl_rsp_data.CUP_rrn		,12);				//4-CUP RRN
		memcpy(gl_input.net_amount		,gl_rsp_data.net_amount		,4);				//5-CUP Settlement Date

#endif

	}

#if VERSION_MODE != VERSION_NCCC	//rex 2022/07/18 �p�H�� ���|���Ʋ֥[ 
	IncRocNum();
#endif

	TransAccptCommonCode();
#ifdef CHILDREN_PARADISE_VER
		CP_setTransactionResult(SUCCEED);
#endif
	// ==== Echo 2015/11/18 �C�Lñ��e���W�����u��� ====
//	if(mode == 0)
//		result = SUCCEED;
//	else
//		result = SearchOffline();	// ==== �w���j�M�O�_��Offline�����(�S�����ܥi�H���_�u) ====

//	if(result == SUCCEED)
//	{
		// ==== �W�Ƿ�����TC Upload ====
		if(mode == 0)	// ==== Pre-Auth������ݰeTC Upload ====
			result = TC_Upload();
		else
			result = SUCCEED;

		if(result == SUCCEED)
			OfflineProc(ONLY_ONE);
//	}
//	else
//		ResetLine();

	// ==== Echo 2015/11/18 �C�Lñ�� ====
	PrintLatestReceipt();
	EMV_Message(7,0);
}
#endif

#if VERSION_MODE != VERSION_NCCC	//rex 2022/11/16 �p�H�� ���|���Ʋ֥[ 
/* ----------------------------------------------
Name:	UICC_refund
Funct:	���p�����d�h�f
Input:
Output:
Global:
Black	2022/08/09	�s�W�x�s�Ȧ洹���h�f
------------------------------------------------*/
#if 0
void	UICC_refund(void)
{
	UCHAR	msg_emv_process[LEN_DISP] = {' ',0xB4,0xB9,0xA4,0xF9,0xA5,0x64,0xB3,0x42,0xB2,0x7A,0xA4,0xA4,'.','.','.'};					// ==== �����d�B�z��... ====
	FONT_DEF lMsg_check_digit_err = {{' ',' ',' ',' ',0xA5,0x64,0xB8,0xB9,0xBF,0xF9,0xBB,0x7E,' ',' ',' ',' '},{" CHECK DIGIT ERR"}};	// ==== �d�����~ ====
//	UCHAR	msg_invaild_card[14] = {0xA6,0xB9,0xA5,0x64,0xA4,0xF9,0xAB,0x44,0xBB,0xC8,0xC1,0x70,0xA5,0x64};								// ==== ���d���D���p�d ====
	UCHAR	EMV_result;
	UCHAR	result;//,status;
	UCHAR	RecordFull_flag;
	UCHAR	buf[9];
	UCHAR	trace_buf[TRACE_NO_HALF];

	UCHAR	aid_cup[7] = {0xA0,0x00,0x00,0x03,0x33,0x01,0x01};

	gl_input.emv_flag = 1;	// ==== �]�w��EMV�����d��� ====
	gl_input.entry_mode[1] = 0x52;

	ClrDisplayDN(LINE2);
	DispText(msg_emv_process,LINE3,0,LEN_DISP,FONT2);	// ==== �����d�B�z��... ====

	//***$$$-Step1,2,3,4---------------------------------
	// ==== Echo 2015/11/12 ���ʨ�W�@�h ====
//	if(EMV_Trans_START() == FAIL)
//		return;

	//	Black	2017/01/12	�ק����
	// ==== �D���p�����d���i��� ====
	if(memcmp(gl_emvk_aid,aid_cup,7))
	{
		DispNotCUP(1);	// ==== Echo 2015/9/15 ��ܫD���p�d�T�� ====
//		ClrDisplayDN(LINE2);
//		DispText(msg_invaild_card,LINE3,1,14,FONT2);	// ==== ���d���D���p�d ====
//		LongBeep();
//		EMV_Message(3,0);
		return;
	}

	gl_input.product_code = gl_emvk_aid[7];	// ==== �x�s���p�����d�̫�@�XAID ====

	//	Black	2017/04/17	�N�d�O�]��CUP
	result = Set_CUP_Issuer();

	//-------Step5----��J���---------------------------
	// ==== Echo 2013/2/6 [ECR]���Ⱦ��_�_�� ====
	if (gl_ecr_flag == 1)
	{
		// ==== �^�e�d��"60" ====
		result = AUXSTART();
		if(result == FAIL)
		{
			EMV_Message(4,1);
			return;
		}

		// ==== ��ܥ�����B ====
		// ==== ��J���B(gl_input.tot_amount) ====
		compress(&gl_input.tot_amount[1],glo_ecr_data.amount,5);		// ==== ���B�[���p�� ====
		//	Black	2019/06/03	[ECR]��������b�Ĥ@����ܦ��Ⱦ�������B
//		DispAmount(gl_input.tot_amount,LINE1,FONT1 + attrREVERSE);		// ==== ��ܪ��B ====
	}

	FlushKey();

	gl_fall_back_flag = 0;	// ==== ����Fall Back ====

	// ==== �b�w���ε��b���� ====
	RecordFull_flag = RecordFull();		//	Black	2011/07/20
	if(RecordFull_flag)
	{
		ClrDisplayDN(LINE2);
		if(RecordFull_flag == 1)
		{
			DispBiText((FONT_DEF *)&msg_close_batch, LINE2);	// ==== �妸�w�� ====
			DispBiText((FONT_DEF *)&msg_close_batch_1, LINE3);	// ==== �Х����b ====
		}
		else if(RecordFull_flag == 2)
			DispBiText((FONT_DEF *)&msg_close_batch_1, LINE3);	// ==== �Х����b ====

		LongBeep();
		EMV_Message(3,0);
		return;
	}

	// ==== Acquirer Table �\��}�� ====
	if(ChkAcquirerOption(24))
	{
		ClrDisplayDN(LINE2);
		DispText((UCHAR *)msg_no_auth,LINE3,0,LEN_DISP,FONT2);	// ==== ���\�ॼ�} ====
		LongBeep();
		EMV_Message(3,0);
		return;
	}

	if(gl_get_card_error_flag == 1)
	{
		ClrDisplayDN(LINE2);
		DispBiText(&lMsg_check_digit_err,LINE3);	// ==== �d�����~ ====
		LongBeep();

		EMV_Message(3,0);
		return;
	}

	// ==== Pre-Dial ====
	if(gl_lan_port.status == 0)
		PredialOK();

	//	Black	2019/01/30	���׬O�_���P�����A�Ҧb�L�d�e��J���B
//	// ==== Echo 2016/5/24 �P�ⱵDongle�ɤ���J���B ====
//	if((gl_Wreader_status == SUCCEED) && (mode == 0))
//		;
//	else
//	{
//		if (gl_ecr_flag == 0)	// ==== Echo 2014/2/20 [ECR]���Ⱦ��_�_�� ====
//		{
//			if (!GetAmount(0))
//			{
//				EMV_Message(4,0);
//				return;
//			}
//		}
//	}

	//	Black	2019/06/04	[ECR]��q��ECR�b�L�d��~��J���
	if (gl_ecr_flag == 1)
	{
		result = GetAdditional();
		if (result != SUCCEED)
		{
			EMV_Message(4,0);
			return;
		}
	}

	// ==== ��JPIN ====
	// ==== Echo 2014/6/18 ���EMV Online PIN ====
/*	result = GetPinPad(gl_pinpad.mode);
	if(result != SUCCEED)
	{
		if (gl_ecr_flag != 0)	// ==== Echo 2013/2/3 [ECR]���Ⱦ��_�_�� ====
			EMV_Message(4,0);
		return;
	}
*/
	gl_input.redeem_flag = 3;

	memset(gl_input.offset_amount,'0',6);	// ==== ���p�d����Ǹ� ====
	memset(gl_input.net_amount,'0',4);		// ==== ���p�d�M���(MMDD) ====
	memset(gl_input.utilize_point,'0',4);	// ==== ���p�d����ǿ���(MMDD) ====
	memset(gl_input.award_point,'0',6);		// ==== ���p�d����ǿ�ɶ�(hhmmdd) ====
	memset(gl_input.fee_date,'0',4);		// ==== ��l������(MMDD) ====
	memset(gl_input.refund_tid,'0',8);		// ==== ���P���x�h�f������Terminal ID ====

	// ==== ����JPIN POS Entry Mode = 0x51,�S��JPIN POS Entry Mode = 0x52 ====
	// ==== Echo 2014/6/18 ���EMV Online PIN,POS Entry Mode�n���b�s�u�e ====
/*	gl_input.entry_mode[0] = 0;
	if(gl_pin_entry == 1)
		gl_input.entry_mode[1] = 0x51;
	else
		gl_input.entry_mode[1] = 0x52;
*/
	ClrDisplayDN(LINE2);
	DispText((UCHAR *)msg_connecting_1,LINE3,0,LEN_DISP,FONT2);	// ==== �B�z�� ====

	//-------Step6---------------------------------------
	//	Black	2016/11/25	�����[�t
//	if(EMV_status_check() == FAIL)
//		return;

	IncTraceNum();
	memcpy(trace_buf,gl_input.trace_no,TRACE_NO_HALF);

	memset(buf,0,9);
	memcpy(buf,&gl_input.tot_amount[1],5);
	api_emvk_SetTransPara(buf,trace_buf);	// ==== Echo 2014/5/3 ���H�n�D���n�ɴ����d������B�j��0xFFFFFFFF ====
//	EMV_result = api_emvk_SetTransPara(buf,trace_buf);
//	if(EMV_result == emvFailed)	// ==== Echo 2013/4/29 ��������B�j��0xFFFFFFFF��,API�|�^emvFailed ====
//	{
//		EMV_Message(2,0);
//		return;
//	}

	//***$$$-Step7,8------------------------------------
	if(EMV_Trans_MSI() == FAIL)
			return;

	//***$$$-Step9,10,11--------------------------------
	EMV_result = EMV_Trans_CM();
	if(EMV_result == FAIL)
		return;

	//-------Step12--------------------------------------
	//	Black	2016/11/25	�����[�t
//	if(EMV_status_check() == FAIL)
//		return;

	api_emvk_Completion(buf);

	if (gl_Switch[SW_DEBUG] == ON)	//�����Ҧ� 	//	Black	2011/03/29
	{
		if(gl_debug_mode_flag & 0x02)	//alex 09/09/01
			EMV_Debug_print(); //alex 2003/09/30
	}

	if(!memcmp(buf,"00",2))
	{
		EMV_result = GetEmvData(6);	// ==== TC_value ====
		if(EMV_result == FAIL)
			return;
	}

	if(memcmp(gl_rsp_data.rspcode,"IE",RSPCODE_LEN))	// ==== Echo 2011/4/29 F55���L�����~ ====
		memcpy(gl_rsp_data.rspcode,buf,2);

	if(!memcmp(buf,"00",2))
		;
	else if(!memcmp(buf,"Y1",2) || !memcmp(buf,"Y3",2))	// ==== Y1 & Y3 => Offline,�u���P��䴩Offline ====
	{
		gl_rsp_data.rspcode[0] = 'Z';	// ==== �p�G�X�{Y1�PY3,�h�G�N��ܦ�Z1��Z3 ====
		EMV_Message(5,0);
		return;
	}
	else
	{
		EMV_Message(5,0);
		return;
	}

	//	Black	2016/11/24	�קאּ���ϥ�F56�A�u�ϥ�F55
	//-------Step13--------------------------------------
	gl_input.emv_record_ptr = gl_emv_record_ptr_C;	// ==== EMV IC�d�����ưO�� ====
//	StoreEmvData(1,CREDIT_EMV_PAGE1,gl_emv_record_ptr_C);
	StoreEmvData(0,CREDIT_EMV_PAGE1,gl_emv_record_ptr_C);
	gl_emv_record_ptr_C++;

	IncRocNum();
	TransAccptCommonCode();

	// ==== Echo 2015/11/18 �C�Lñ��e���W�����u��� ====
//	if(mode == 0)
//		result = SUCCEED;
//	else
//		result = SearchOffline();	// ==== �w���j�M�O�_��Offline�����(�S�����ܥi�H���_�u) ====

//	if(result == SUCCEED)
//	{
//		// ==== �W�Ƿ�����TC Upload ====
//		if(mode == 0)	// ==== Pre-Auth������ݰeTC Upload ====
//			result = TC_Upload();
//		else
//			result = SUCCEED;
//
//		if(result == SUCCEED)
			OfflineProc(ONLY_ONE);
//	}
//	else
//		ResetLine();

	// ==== Echo 2015/11/18 �C�Lñ�� ====
	PrintLatestReceipt();
	EMV_Message(7,0);
}
#endif
#endif	//end of  not VERSION_NCCC
