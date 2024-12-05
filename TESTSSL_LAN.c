//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : 		                                                    **
//**  PRODUCT  : AS350                                                      **
//**                                                                        **
//**  FILE     : TESTSSL_LAN.C		                                        **
//**  MODULE   : 					                                        **
//**									                                    **
//**  FUNCTION : SSL Library Test Program.				                    **
//**		 		    					                                **
//**  VERSION  : V1.00                                                      **
//**  DATE     : 2024/04/18                                                 **
//**  EDITOR   : James Hsieh                                                **
//**                                                                        **
//**  Copyright(C) 2024 SymLink Corporation. All rights reserved.	        **
//**                                                                        **
//****************************************************************************
//============================================================================
//
//----------------------------------------------------------------------------
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <errno.h>

#include <openssl/rand.h>

#include "POSAPI.h"
#include "UTILS.h"
#include "OS_LIB.h"

#include "crypto.h"
#include "x509.h"
#include "pem.h"
#include "ssl.h"
#include "err.h"


//#include "WINSOCK.h"
//#include "SSLFILE.h"


#define	CERTF	"/home/AP/client.crt"		// client certificate
#define	KEYF	"/home/AP/client.key"		// client private key
#define	CACERT	"/home/AP/ca.crt"		// CA certificate

#define	SOCKET_ERROR			-1

//const UCHAR CLIENT_CRT_H_FILE[]	= {	// contents of client certificate
//#include "client.crt.h"
//};

//const UCHAR CLIENT_KEY_H_FILE[]	= {	// contents of client private key
//#include "client.key.h"
//};

//const UCHAR CA_CRT_H_FILE[]	= {	// contents of CA certificate
//#include "ca.crt.h"
//};

// UCHAR	SSL_dhn_lan;		// system default variable for SSL connection


char	buf[1024];

extern	ULONG api_sys_random_len( UCHAR *dbuf, UINT len );


// ---------------------------------------------------------------------------
// FUNCTION: show digit string with right-justified. (max 20 digits)
// INPUT   : row  - row position to show.
//           buf  - L-V, the digit string.
//           type -
//           idle - character to be shown when idle. (eg. '0','-'...)
//           font - the font to be showed
// OUTPUT  : none.
// RETURN  : none.
// ---------------------------------------------------------------------------
void    ShowKey(UCHAR type, UCHAR idle, UCHAR font, UCHAR row, UCHAR *buf)
{
    UCHAR i;
    UCHAR len;
    UCHAR index;
    UCHAR max_dsp_cnt;
    UCHAR newbuf[30];

    memmove(newbuf, buf, buf[0] + 1);

    UTIL_ClearRow(row, 1, font);

    if((font & 0x0f) == FONT1)
        max_dsp_cnt = MAX_DSP_FONT0_CNT;
    else
        max_dsp_cnt = MAX_DSP_FONT1_CNT;

    // check non-zero idle prompt
    if((buf[0] == 0) && (idle != '0'))
    {
        newbuf[1] = idle;
        UTIL_PutStr(row, max_dsp_cnt - 1, font, 1, &newbuf[1]);
        return;
    }

    if(newbuf[0] == 0) // no data in buffer, show '0'
    {
        newbuf[1] = idle;
        UTIL_PutStr(row, max_dsp_cnt - 1, font, 1, &newbuf[1]);
    }
    else
    {   // check special prompt
        if((type & NUM_TYPE_STAR) != 0)
        {
            type &= 0xf7;

            for(i = 0 ; i < newbuf[0] ; i++)
                newbuf[i + 1] = '*';
        }

        if((type & NUM_TYPE_COMMA) == 0)
        {   // NNNNNNNNNNNNNNNN...N
SHOW_NORM:
            if((max_dsp_cnt - newbuf[0]) > 0)
            {
                UTIL_PutStr(row, max_dsp_cnt - newbuf[0], font, newbuf[0], &newbuf[1]);
            }
            else
            {
                index = newbuf[0] - max_dsp_cnt + 1;
                UTIL_PutStr(row, 0, font, max_dsp_cnt, &newbuf[index]);
            }
        }  //  9   6   3   0   7   4
        else    // NN,NNN,NNN,NNN,NNN,NNN,NNN
        {
            len = newbuf[0];
            if(len < 4)
                goto SHOW_NORM;
        }
    }
}

// ---------------------------------------------------------------------------
// FUNCTION: get keys & show numeric digits.
// INPUT   : tout - time out before ENTER key pressed. (in seconds, 0=always)
//           row  - row position to show.
//           len  - max. length allowed.
//           type - bit 7 6 5 4 3 2 1 0
//                            | | | | |_ pure digits (NNNNNN)
//                            | | | |___ with sperator (NN,NNN)
//                            | | |_____ with decimal point (NNN.NN) cf. currency exponent.
//                            | |_______ special prompt (eg. ****) cf.
//                            |_________ accept leading '0' (eg. "0123", rather "123")
//
//           idle - character to be shown when idle. (eg. '0','-'...)
//           font - the font to be showed
// OUTPUT  : buf - the final result. (sizeof(buf) = len+1)
//                 format: LEN[1] DIGIT[n]
// REF     :
// RETURN  : TRUE  = confirmed. (by entering OK)
//           FALSE = aborted or timeout.   (by entering CANCEL)
// ---------------------------------------------------------------------------
UCHAR   GetNumKey(UINT tout, UCHAR type, UCHAR idle, UCHAR font, UCHAR row, UCHAR len, UCHAR *buf)
{
    UCHAR i = 0;
    UCHAR key;
    UCHAR dhn_tim;
    UCHAR dhn_tim2;
    UINT  tick1, tick2;
    UINT  tick3, tick4;


    dhn_tim = api_tim_open(100); // time tick = 1sec
    api_tim_gettick(dhn_tim, (UCHAR *)&tick1);

    if((buf[0] != 0) && (buf[0] <= len))	// to keep old data at first time
    {
        i = buf[0] + 1;
        ShowKey(type, idle, font, row, buf);
        goto KEYIN_WAIT;
    }

KEYIN_CLEAR:

    for(i = 0 ; i <= len ; i++)
        buf[i] = 0;  // clear output buffer

    i = 1;
    buf[0] = 0;
    key = 0;

BEGIN:
    while(1)
    {
        if(i == 1)
            ShowKey(type, idle, font, row, buf);

KEYIN_WAIT:
        if(tout != 0)
        {
            // wait key
            do
            {
                // check timeout
                api_tim_gettick(dhn_tim, (UCHAR *)&tick2);
                if((tick2 - tick1) >= tout)
                {
                    api_tim_close(dhn_tim);
                    return (FALSE);
                }
            } while(UTIL_GetKeyStatus() == apiNotReady);
        }

        key = UTIL_WaitKey();

        switch(key)
        {
            case 'x': // cancel
                for(i = 0 ; i <= len ; i++)
                    buf[i] = 0;

                api_tim_close(dhn_tim);
                return FALSE;
KEYIN_NO:
            case 'n': // clear
                UTIL_ClearRow(row, 1, FONT2);
                i = 1;
                buf[0] = 0;
                goto KEYIN_CLEAR;
KEYIN_YES:
            case 'y': // ok
                //    if(i == 1)
                //      continue;

                if(buf[0] == 0)
                {
                    buf[0] = 1;
                    buf[1] = '0';
                }

                api_tim_close(dhn_tim);
                return TRUE;

            case '#': // backspace
                if(i != 1)
                {
                    buf[--i] = 0;
                    buf[0] -= 1;
                    ShowKey(type, idle, font, row, buf);
                    continue;
                }
                else
                    goto KEYIN_CLEAR;

            case '*':   //'.'
                buf[0] = i; // length of current keyin
                key = '.';
                buf[i++] = key;
                ShowKey(type, idle, font, row, buf);
                continue;

            default: // '0'~'9'
                if(i > len) // overflow?
                    continue;

                //    if((type & NUM_TYPE_STAR) == 0)
                if((type & NUM_TYPE_LEADING_ZERO) == 0)
                {
                    if((key == '0') && (buf[0] == 0))
                    {
                        buf[0] = 1;
                        buf[1] = '0';
                        ShowKey(type, idle, font, row, buf);
                        buf[0] = 0;
                        goto KEYIN_WAIT;
                    }
                }

                buf[0] = i; // length of current keyin

                //    if( (type & 0x08) != 0 ) // check special prompt
                //      {
                //      type &= 0xf7;
                //      key = '*';
                //      }

                buf[i++] = key;
                ShowKey(type, idle, font, row, buf);
        }
    }
}

// ---------------------------------------------------------------------------
// FUNCTION: Set remote port.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR   setRemotePort(UCHAR *remotePort)
{
    UCHAR defaultRemotePort[] = {4, '4', '4', '3', '3'};
    UCHAR msg_Q_YES_NO[] = {"(Y/N)?"};
    UCHAR key;
    UCHAR tempBuf[23];


    memmove(tempBuf, defaultRemotePort, sizeof(tempBuf));

    UTIL_ClearScreen();
    UTIL_PutStr(0, 0,  FONT0, strlen("Remote Port:"), (UCHAR *)"Remote Port:");

    if(tempBuf[0] != 0)
    {
        UTIL_PutStr(3, 21-tempBuf[0], FONT0, tempBuf[0], (UCHAR *)&tempBuf[1]);
        
        UTIL_PutStr(4, 0, FONT0, sizeof(msg_Q_YES_NO) - 1, msg_Q_YES_NO);
        while(1)
        {
            key = UTIL_WaitKey();
            if(key == 'y')  // ENTER
            {
                memmove(remotePort, tempBuf, sizeof(tempBuf));
                return apiOK;
            }
            if((key == 'x') || (key == 'n'))    // CANCEL or CLEAR
            {
                goto SETUP;
            }

        }
    }
    else
    {
SETUP:
        UTIL_ClearScreen();
        UTIL_PutStr(0, 0,  FONT0, strlen("Remote Port:"), (UCHAR *)"Remote Port:");

        if(GetNumKey(0, NUM_TYPE_LEADING_ZERO + NUM_TYPE_DEC, '_', FONT0, 3, 21, tempBuf) == FALSE)
            return apiFailed;
        else
        {
            memmove(remotePort, tempBuf, sizeof(tempBuf));
            return apiOK;
        }
    }
}

// ---------------------------------------------------------------------------
// FUNCTION: Set remote IP.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR   setRemoteIP(UCHAR *remoteIP)
{
    UCHAR defaultRemoteIP[] = {12, '1', '7', '2', '.', '1', '6', '.', '1', '.', '1', '2', '4'};
    UCHAR msg_Q_YES_NO[] = {"(Y/N)?"};
    UCHAR key;
    UCHAR tempBuf[23];

    
    memmove(tempBuf, defaultRemoteIP, sizeof(tempBuf));

    UTIL_ClearScreen();
    UTIL_PutStr(0, 0,  FONT0, strlen("Remote IP:"), (UCHAR *)"Remote IP:");

    if(tempBuf[0] != 0)
    {
        UTIL_PutStr(3, 21-tempBuf[0], FONT0, tempBuf[0], (UCHAR *)&tempBuf[1]);
        
        UTIL_PutStr(4, 0, FONT0, sizeof(msg_Q_YES_NO) - 1, msg_Q_YES_NO);
        while(1)
        {
            key = UTIL_WaitKey();
            if(key == 'y')  // ENTER
            {
                memmove(remoteIP, tempBuf, sizeof(tempBuf));
                return apiOK;
            }
            if((key == 'x') || (key == 'n'))    // CANCEL or CLEAR
            {
                goto SETUP;
            }

        }
    }
    else
    {
SETUP:
        UTIL_ClearScreen();
        UTIL_PutStr(0, 0,  FONT0, strlen("Remote IP:"), (UCHAR *)"Remote IP:");

        if(GetNumKey(0, NUM_TYPE_LEADING_ZERO + NUM_TYPE_DEC, '_', FONT0, 3, 21, tempBuf) == FALSE)
            return apiFailed;
        else
        {
            memmove(remoteIP, tempBuf, sizeof(tempBuf));
            return apiOK;
        }
    }
}

// ---------------------------------------------------------------------------
// FUNCTION: Demo program to test SSL functions.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
// ---------------------------------------------------------------------------
void	TEST_SSL( void )
{
int	err;
int	sd;
struct sockaddr_in sa;
SSL_CTX	*ctx;
SSL	*ssl;
X509	*server_cert;
SSL_METHOD *meth;
int	seed_int[100];
int	i;
UCHAR j;
UCHAR ip[23];
UCHAR port[23];
UINT portValue = 0;

API_LAN		lan;


    while(1)
    {
        if(setRemoteIP(ip) == apiOK)
            break;
    }

    while(1)
    {
        if(setRemotePort(port) == apiOK)
        {
            for(j = 1 ; j <= port[0] ; j++)
            {
                portValue = 10 * portValue + (port[j] - '0');
            }

            break;
        }
    }

START:
	UTIL_ClearScreen();
	UTIL_PutStr( 0, 0,  FONT0, strlen("OP TEST"), (UCHAR *)"OP TEST" );
	UTIL_PutStr( 2, 0,  FONT0, strlen("HIT ANY KEY TO START..."), (UCHAR *)"HIT ANY KEY TO START..." );
	UTIL_WaitKey();
	
	// --------------------------------------------------------------------------------------------
	// STEP 1:
	// Initialize SSL file system
	// --------------------------------------------------------------------------------------------
#if	0
	UTIL_PutStr( 0, 0,  FONT0+attrCLEARWRITE, strlen("INIT FILE SYSTEM"), (UCHAR *)"INIT FILE SYSTEM" );
	
	if( !SSL_FS_init() )
	  {
	  UTIL_PutStr( 7, 0,  FONT0+attrCLEARWRITE, strlen("ERR:SSL_FS"), (UCHAR *)"ERR:SSL_FS" );
	  goto ERROR;
	  }
	
	// load contents of related keys and certificates to SSL file system
	SSL_FS_file( CERTF,  (char *)&CLIENT_CRT_H_FILE, sizeof(CLIENT_CRT_H_FILE) );
	SSL_FS_file( KEYF,   (char *)&CLIENT_KEY_H_FILE, sizeof(CLIENT_KEY_H_FILE) );
	SSL_FS_file( CACERT, (char *)&CA_CRT_H_FILE,     sizeof(CA_CRT_H_FILE) );
#else
	api_tls_cpkey();	// copy key & certificates to AP
#endif

	// --------------------------------------------------------------------------------------------
	// STEP 2:
	// Initialize SSL
	// --------------------------------------------------------------------------------------------
// START:
	UTIL_ClearScreen();
	UTIL_PutStr( 0, 0,  FONT0+attrCLEARWRITE, strlen("INIT SSL"), (UCHAR *)"INIT SSL" );
	
	SSLeay_add_ssl_algorithms();	// initialize SSL library by registering algorithms
	UTIL_DispHexByte( 0, 19, 0x01 );
		
//	meth = (SSL_METHOD *)TLSv1_client_method();	// using TLSv1 protocol at client
	meth = (SSL_METHOD *)TLSv1_2_client_method();	// ==== TLS 1.2 ====
	UTIL_DispHexByte( 0, 19, 0x02 );
		
	SSL_load_error_strings();	// register the error strings for "libcrypto" & "libssl"
	UTIL_DispHexByte( 0, 19, 0x03 );
	
//	SSL_library_init();
	
	ctx = SSL_CTX_new(meth);	// create a new SSL_CTX object as framework for TLS/SSL enabled functions
	UTIL_DispHexByte( 0, 19, 0x04 );
	if( ctx == NULL )
	  {
	  UTIL_PutStr( 7, 0,  FONT0+attrCLEARWRITE, strlen("ERR:CTX_new"), (UCHAR *)"ERR:CTX_new" );
	  goto ERROR;
	  }
	
//	UTIL_WaitKey();
	
	// --------------------------------------------------------------------------------------------
	// STEP 3:
	// Setup SSL parameters
	// --------------------------------------------------------------------------------------------

	UTIL_PutStr( 0, 0,  FONT0+attrCLEARWRITE, strlen("SETUP SSL"), (UCHAR *)"SETUP SSL" );


	// exclude weak ciphers, such as CHACHA20
//	if( !SSL_CTX_set_cipher_list( ctx, "HIGH:!aNULL:!CHACHA20" ) )
	if( !SSL_CTX_set_cipher_list( ctx, "HIGH:!aNULL:!CHACHA20:!CAMELLIA:!ARIA:!SHA" ) )	// 2024-08-22, only 33 suites
	  {
	  UTIL_PutStr( 7, 0, FONT0+attrCLEARWRITE, strlen("ERR:SET_CIPHER"), (UCHAR *)"ERR:SET_CIPHER" );
	  goto ERROR;
	  }


	SSL_CTX_set_verify( ctx, SSL_VERIFY_PEER, NULL );	// set the verification flags for SSL to be mode
	UTIL_DispHexByte( 0, 19, 0x01 );
	
//	UTIL_WaitKey();
		
	SSL_CTX_load_verify_locations( ctx, CACERT, NULL );	// set default locations for trusted CA certificate
	UTIL_DispHexByte( 0, 19, 0x02 );
	
//	UTIL_WaitKey();
	
//	SSL_CTX_set_security_level( ctx, 0 );		// for TEST ONLY, to lower security level (L..H, 0..5)

	if( SSL_CTX_use_certificate_file( ctx, CERTF, SSL_FILETYPE_PEM ) != 1 )	// load CLIENT certificate in file into CTX
	  {
	  UTIL_PutStr( 7, 0, FONT0+attrCLEARWRITE, strlen("ERR:CERTF"), (UCHAR *)"ERR:CERTF" );
	  goto ERROR;
	  }
	UTIL_DispHexByte( 0, 19, 0x03 );
	
//	UTIL_WaitKey();
		
	if( SSL_CTX_use_PrivateKey_file( ctx, KEYF, SSL_FILETYPE_PEM ) != 1 )	// add CLIENT private key in file into CTX
	  {
	  UTIL_PutStr( 7, 0, FONT0+attrCLEARWRITE, strlen("ERR:KEYF"), (UCHAR *)"ERR:KEYF" );
	  goto ERROR;
	  }
	UTIL_DispHexByte( 0, 19, 0x04 );
	
//	UTIL_WaitKey();
	  	
	if( SSL_CTX_check_private_key(ctx) != 1 )	// check the consistency of a private key with the corresponding certificate in CTX
	  {
	  UTIL_PutStr( 7, 0, FONT0+attrCLEARWRITE, strlen("ERR:CHECK PRIV KEY"), (UCHAR *)"ERR:CHECK PRIV KEY" );
	  goto ERROR;
	  }
	UTIL_DispHexByte( 0, 19, 0x05 );
	
//	UTIL_WaitKey();
	
	// --------------------------------------------------------------------------------------------
	// STEP 4:
	// Create a socket and connect to server using normal socket calls
	// --------------------------------------------------------------------------------------------
	
	UTIL_PutStr( 0, 0,  FONT0+attrCLEARWRITE, strlen("CREATE TCP SOCKET"), (UCHAR *)"CREATE TCP SOCKET" );

#if	0
	srand( (unsigned)time( NULL ) );		// generate random numbers for authentication
	for( i = 0; i < 100;i++ )			//
	     seed_int[i] = rand();			//
#else
	api_sys_random_len( (UCHAR *)seed_int, sizeof(seed_int) );
#endif
	RAND_seed( seed_int, sizeof(seed_int) );	//

	UTIL_DispHexByte( 0, 19, 0x01 );
	
	sd = socket( AF_INET, SOCK_STREAM, 0 );		// create a socket for connection
	if( sd == SOCKET_ERROR )
	  {
	  UTIL_PutStr( 7, 0, FONT0+attrCLEARWRITE, strlen("ERR:SOCKET"), (UCHAR *)"ERR:SOCKET" );
	  goto ERROR;
	  }
	
	UTIL_DispHexByte( 0, 19, 0x02 );
	
//	UTIL_WaitKey();
	
	// --------------------------------------------------------------------------------------------
	// STEP 5:
	// Connect TCP server
	// --------------------------------------------------------------------------------------------
	
	UTIL_PutStr( 0, 0,  FONT0+attrCLEARWRITE, strlen("CONNECT TCP..."), (UCHAR *)"CONNECT TCP..." );
	
	// CONNECT: using WIN socket method
#if	1
	memset( &sa, '\0', sizeof(sa) );
	sa.sin_family      = AF_INET;
	// sa.sin_addr.s_addr = inet_addr("172.16.1.124");		// server IP (for test only)
    sa.sin_addr.s_addr = inet_addr(&ip[1]);		// server IP (for test only)
	// sa.sin_port        = htons(4433);			// server port number (for test only)
    sa.sin_port        = htons(portValue);			// server port number (for test only)
	
	err = connect(sd, (struct sockaddr*) &sa,sizeof(sa));     
	if( err == SOCKET_ERROR )
	  {
	  UTIL_PutStr( 7, 0, FONT0+attrCLEARWRITE, strlen("ERR:TCP_connect"), (UCHAR *)"ERR:TCP_connect" );
	  goto ERROR;
	  }
#endif
	
	// CONNECT: using LAN API method
#if	0
	lan.LenType = 0xFF;		// bypass
	lan.ClientPort = 65100;		// dynamic client port number
	lan.ServerPort = 4433;		// server port number (for test only)
	lan.ServerIP_Len = 12;		// length of server IP
	memmove( &lan.ServerIP[0], "172.16.1.124", lan.ServerIP_Len );	// server IP (for test only)
	
	SSL_dhn_lan = api_lan_open( lan );	// shall assign the return value to system default variable "SSL_dhn_lan"
	if( SSL_dhn_lan == apiOutOfService )
	  {
	  UTIL_PutStr( 7, 0, FONT0+attrCLEARWRITE, strlen("ERR:TCP_connect"), (UCHAR *)"ERR:TCP_connect" );
	  goto ERROR;
	  }
#endif

//	UTIL_WaitKey();
	
	// --------------------------------------------------------------------------------------------
	// STEP 6:
	// Connect SSL server, start SSL negotiation.
	// --------------------------------------------------------------------------------------------
	
	UTIL_PutStr( 0, 0,  FONT0+attrCLEARWRITE, strlen("CONNECT SSL..."), (UCHAR *)"CONNECT SSL..." );
	
	ssl = SSL_new(ctx);		// create a new SSL structure which is needed to hold the data for a TLS/SSL connection
	
	UTIL_DispHexByte( 0, 19, 0x01 );
	
	if( ssl == NULL )
	  {
	  UTIL_PutStr( 7, 0, FONT0+attrCLEARWRITE, strlen("ERR:SSL_new"), (UCHAR *)"ERR:SSL_new" );
	  goto ERROR;
	  }
	  
	err = SSL_set_fd( ssl, sd );	// set file descriptor
	if( !err )
	  {
	  UTIL_PutStr( 7, 0, FONT0+attrCLEARWRITE, strlen("ERR:SSL_set_fd"), (UCHAR *)"ERR:SSL_set_fd" );
	  goto ERROR;
	  }
	
	UTIL_DispHexByte( 0, 19, 0x02 );
	
	err = SSL_connect(ssl);		// initiate the TLS/SSL handshake with a server
	if( err != 1 )
	  {
	  UTIL_PutStr( 7, 0, FONT0+attrCLEARWRITE, strlen("ERR:SSL_connect"), (UCHAR *)"ERR:SSL_connect" );
	  goto ERROR;
	  }
	
	UTIL_DispHexByte( 0, 19, 0x03 );
	
//	UTIL_WaitKey();
	
	// --------------------------------------------------------------------------------------------
	// Following steps are optional and not required for data exchange to be successful
	// --------------------------------------------------------------------------------------------
	
#if	0
	// Get the cipher - option
	str = SSL_get_cipher(ssl);
	if( str == NULL )
	  {
	  UTIL_PutStr( 7, 0, FONT0+attrCLEARWRITE, strlen("ERR:SSL_get_cphr"), (UCHAR *)"ERR:SSL_get_cphr" );
	  goto ERROR;
	  }
	else
	  {
	  // display cipher used here if needed
	  UTIL_PutStr( 1, 0, FONT0+attrCLEARWRITE, strlen("CIPHER USED:"), (UCHAR *)"CIPHER USED:" );
	  UTIL_DumpHexData2( 1, 2, strlen(str), str );
	  UTIL_ClearScreen();
	  }
	
	// Get server's X509 certificate (note: beware of dynamic allocation) - option
	server_cert = SSL_get_peer_certificate( ssl );
	if( server_cert == NULL )
	  {
	  UTIL_PutStr( 7, 0, FONT0+attrCLEARWRITE, strlen("ERR:SSL_get_cert"), (UCHAR *)"ERR:SSL_get_cert" );
	  goto ERROR;
	  }
	else
	  {
	  // display server's certificate here if needed
	  UTIL_PutStr( 1, 0, FONT0+attrCLEARWRITE, strlen("SVR CERTIFICATE:"), (UCHAR *)"SVR CERTIFICATE:" );
	  UTIL_DumpHexData2( 1, 2, strlen(server_cert), server_cert );
	  }

	// --------------------------------------------------------------------------------------------
	str = X509_NAME_oneline( X509_get_subject_name(server_cert), 0, 0 );
	OPENSSL_free( str );

	str = X509_NAME_oneline( X509_get_issuer_name(server_cert), 0, 0 );
	OPENSSL_free( str );

	X509_free( server_cert );	// free up the X509 structure "server_cert" if used
#endif

	// --------------------------------------------------------------------------------------------
	// STEP 7:
	// SSL DATA EXCHANGE - Send a message to server (for test only)
	// --------------------------------------------------------------------------------------------
	
	UTIL_PutStr( 0, 0,  FONT0+attrCLEARWRITE, strlen("SSL WRITE..."), (UCHAR *)"SSL WRITE..." );
		
	err = SSL_write( ssl, "Hello World!", strlen("Hello World!") );	// send a message
	if( err <= 0 )
	  {
	  UTIL_PutStr( 7, 0, FONT0+attrCLEARWRITE, strlen("ERR:SSL_write"), (UCHAR *)"ERR:SSL_write" );
	  goto ERROR;
	  }
	
	// --------------------------------------------------------------------------------------------
	// STEP 8:
	// SSL DATA EXCHANGE - Receive a message from server (for test only)
	// --------------------------------------------------------------------------------------------
	
	UTIL_PutStr( 0, 0,  FONT0+attrCLEARWRITE, strlen("SSL READ..."), (UCHAR *)"SSL READ..." );
	
	do{
	  memset( buf, 0, sizeof(buf) );
	  err = SSL_read( ssl, buf, sizeof(buf)-1 );	// waiting for a message (default timeout = 10 sec)
	  if( err <= 0 )
	    {
	    UTIL_PutStr( 7, 0, FONT0+attrCLEARWRITE, strlen("ERR:SSL_read"), (UCHAR *)"ERR:SSL_read" );
	    continue;
	    }
	  else
	    {
	    UTIL_BUZ_Beep1();
	    
	    i = strlen(buf);
	    if( i > 21 )
	      i = 21;
	    UTIL_PutStr( 2, 0,  FONT0+attrCLEARWRITE, strlen(buf), buf );	// show the text read
	    }
	  } while( buf[0] != 'x' );	// quit if the leading char of string is 'x' (test only)
	
	UTIL_ClearScreen();
	UTIL_PutStr( 7, 0, FONT0+attrCLEARWRITE, strlen("ANY KEY TO CLOSE"), (UCHAR *)"ANY KEY TO CLOSE" );
	UTIL_WaitKey();

	// --------------------------------------------------------------------------------------------
	// STEP 9:
	// Disconnect SSL & TCP
	// --------------------------------------------------------------------------------------------
	
	UTIL_PutStr( 0, 0,  FONT0+attrCLEARWRITE, strlen("DISCONNECT SSL..."), (UCHAR *)"DISCONNECT SSL..." );
	
	SSL_shutdown( ssl );		// send SSL/TLS close_notify
	
#if	1
	close(sd);		// DISCONNECT: using WIN socket method
#endif

#if	0
	api_lan_close( SSL_dhn_lan );	// DISCONNECT: using LAN API method
#endif

	// --------------------------------------------------------------------------------------------
	// STEP 10:
	// Clean up SSL
	// --------------------------------------------------------------------------------------------

	ERR_free_strings();
	
	if( ssl )
	  SSL_free( ssl );
	if( ctx )
	SSL_CTX_free( ctx );

    api_tls_rmkey();

	// --------------------------------------------------------------------------------------------
	UTIL_ClearScreen();
	UTIL_PutStr( 6, 0, FONT0+attrCLEARWRITE, strlen("SSL CLOSED"), (UCHAR *)"SSL CLOSED" );
	UTIL_PutStr( 7, 0, FONT0+attrCLEARWRITE, strlen("ANY KEY TO RE-TEST"), (UCHAR *)"ANY KEY TO RE-TEST" );
	
	UTIL_WaitKey();
	goto START;

	// --------------------------------------------------------------------------------------------
	// Error Handler
	// --------------------------------------------------------------------------------------------
ERROR:
	ERR_print_errors_fp(stderr);
	
	// SSL clean up
	ERR_free_strings();
	
	if( ssl )
	  SSL_free( ssl );
	if( ctx )
	  SSL_CTX_free( ctx );

	for(;;);
}

void    TEST_TCP()
{
    int	err;
    int	sd;
    struct sockaddr_in sa;
    struct timeval tv;

    API_LAN		lan;
    UCHAR	SSL_dhn_lan;		// system default variable for SSL connection

    UCHAR keyin;
    UINT port = 8000;
    UCHAR i;
    UCHAR dhnBuf[8];
    UCHAR connectionNum;


    START:
	UTIL_ClearScreen();
	UTIL_PutStr( 0, 0,  FONT0, strlen("OP TEST"), (UCHAR *)"OP TEST" );
	UTIL_PutStr( 2, 0,  FONT0, strlen("HIT ANY KEY TO START..."), (UCHAR *)"HIT ANY KEY TO START..." );
	UTIL_WaitKey();

    UTIL_ClearScreen();
    UTIL_PutStr( 0, 0,  FONT0, strlen("1. SINGLE CONNECTION"), (UCHAR *)"1. SINGLE CONNECTION" );
    UTIL_PutStr( 1, 0,  FONT0, strlen("2. MULTIPLE CONNECTION"), (UCHAR *)"2. MULTIPLE CONNECTION" );
    keyin = UTIL_WaitKey();
    if((keyin != '1') && (keyin != '2'))
        goto START;
    
    UTIL_ClearScreen();

// CONNECT: using LAN API method
#if	1
    if(keyin == '1')
    {
        lan.LenType = 0xFF;		// bypass
        lan.ClientPort = 65100;		// dynamic client port number
        lan.ServerPort = 8001;		// server port number (for test only)
        lan.ServerIP_Len = 11;		// length of server IP
        memmove(&lan.ServerIP[0], "172.16.1.66", lan.ServerIP_Len);	// server IP (for test only)

        SSL_dhn_lan = api_lan_open(lan); // shall assign the return value to system default variable "SSL_dhn_lan"
        if(SSL_dhn_lan == apiOutOfService)
        {
            UTIL_PutStr(7, 0, FONT0 + attrCLEARWRITE, strlen("ERR:TCP_connect"), (UCHAR *)"ERR:TCP_connect");
            goto ERROR;
        }
        else
        {
            UTIL_PutStr(7, 0, FONT0 + attrCLEARWRITE, strlen("CONNECT OK"), (UCHAR *)"CONNECT OK");
            UTIL_WaitKey();
        }
    }
	else if(keyin == '2')
    {
        memset(dhnBuf, 0, sizeof(dhnBuf));

        for(i = 1 ; i <= 3 ; i++)
        {
            lan.LenType = 0xFF;		// bypass
            lan.ClientPort = 65100;		// dynamic client port number
            lan.ServerPort = port + i;		// server port number (for test only)
            lan.ServerIP_Len = 11;		// length of server IP
            memmove(&lan.ServerIP[0], "172.16.1.66", lan.ServerIP_Len);	// server IP (for test only)

            SSL_dhn_lan = api_lan_open(lan); // shall assign the return value to system default variable "SSL_dhn_lan"
            if(SSL_dhn_lan == apiOutOfService)
            {
                UTIL_PutStr(7, 0, FONT0 + attrCLEARWRITE, strlen("ERR:TCP_connect"), (UCHAR *)"ERR:TCP_connect");
                goto ERROR;
            }
            else
            {
                dhnBuf[i - 1] = SSL_dhn_lan;
                UTIL_PutStr(7, 2, FONT0 + attrCLEARWRITE, strlen("CONNECT OK"), (UCHAR *)"CONNECT OK");
                UTIL_PutChar(7, 0, FONT0, i + '0');
                UTIL_WaitKey();
            }
        }

        // goto START;
        goto DISCONNECT;
    }
#else
    UTIL_ClearScreen();
    sd = socket(AF_INET, SOCK_STREAM, 0); // create a socket for connection
    if(sd == SOCKET_ERROR)
    {
        UTIL_PutStr(7, 0, FONT0 + attrCLEARWRITE, strlen("ERR:SOCKET"), (UCHAR *)"ERR:SOCKET");
        goto ERROR;
    }

    UTIL_DispHexByte(0, 19, 0x02);

    tv.tv_sec = 0;
    tv.tv_usec = 500000; // recv timeout 0.5s
    setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    // for connect to timeout
    tv.tv_sec = 7; // tx time out with 7 sec
    tv.tv_usec = 0; 
    setsockopt(sd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&tv, sizeof tv);

    UTIL_PutStr( 0, 0,  FONT0+attrCLEARWRITE, strlen("CONNECT TCP..."), (UCHAR *)"CONNECT TCP..." );
	
	// CONNECT: using WIN socket method
    memset(&sa, '\0', sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr("172.16.1.66");		// server IP (for test only)
    sa.sin_port        = htons(8001);			// server port number (for test only)

    err = connect(sd, (struct sockaddr *)&sa, sizeof(sa));
    if(err == SOCKET_ERROR)
    {
        UTIL_PutStr(7, 0, FONT0 + attrCLEARWRITE, strlen("ERR:TCP_connect"), (UCHAR *)"ERR:TCP_connect");
        goto ERROR;
    }
#endif

    UTIL_ClearScreen();
	UTIL_PutStr( 7, 0, FONT0+attrCLEARWRITE, strlen("ANY KEY TO CLOSE"), (UCHAR *)"ANY KEY TO CLOSE" );
	UTIL_WaitKey();

    // Disconnect SSL & TCP	
	UTIL_PutStr( 0, 0,  FONT0+attrCLEARWRITE, strlen("DISCONNECT SSL..."), (UCHAR *)"DISCONNECT SSL..." );

DISCONNECT:
#if	1
    if(keyin == '1')
	    api_lan_close(SSL_dhn_lan);	// DISCONNECT: using LAN API method
    else if(keyin == '2')
    {
        // for(i = 0 ; i < 3 ; i++)
        // {
        //     if(api_lan_close(dhnBuf[i]) == apiFailed)
        //     {
        //         UTIL_PutStr(6, 0, FONT0 + attrCLEARWRITE, strlen("DHN:"), (UCHAR *)"DHN:");
        //         UTIL_DispHexByte(6, 4, dhnBuf[i]);
        //         UTIL_PutStr(7, 0, FONT0 + attrCLEARWRITE, strlen("ERR:TCP_disconnect"), (UCHAR *)"ERR:TCP_disconnect");
        //         goto ERROR;
        //     }
        // }

        while(1)
        {
            UTIL_ClearScreen();
            UTIL_PutStr(0, 0,  FONT0, strlen("Close any one of the connection #1~3"), (UCHAR *)"Close any one of the connection #1~3");
            UTIL_PutStr(1, 0,  FONT0, strlen("Enter 0 to close all connections"), (UCHAR *)"Enter 0 to close all connections");
            keyin = UTIL_WaitKey();

            if(keyin == 'x')
            {
                UTIL_ClearScreen();
                UTIL_PutStr( 7, 0, FONT0, strlen("ANY KEY TO RE-TEST"), (UCHAR *)"ANY KEY TO RE-TEST" );
                UTIL_WaitKey();
                goto START;
            }
            else if(keyin == '0')
            {
                for(i = 0 ; i < 3 ; i++)
                {
                    api_lan_close(dhnBuf[i]);
                }

                UTIL_ClearScreen();

                UTIL_PutStr(7, 0, FONT0, strlen("CLOSE ALL OK"), (UCHAR *)"CLOSE ALL OK");
                UTIL_WaitKey();

                UTIL_PutStr( 7, 0, FONT0 + attrCLEARWRITE, strlen("ANY KEY TO RE-TEST"), (UCHAR *)"ANY KEY TO RE-TEST" );
                UTIL_WaitKey();
                goto START;
            }
            else if((keyin == '1') || (keyin == '2') || (keyin == '3'))
            {
                connectionNum = keyin - '0' - 1;

                if(api_lan_close(dhnBuf[connectionNum]) == apiFailed)
                {
                    UTIL_PutStr(6, 0, FONT0 + attrCLEARWRITE, strlen("DHN:"), (UCHAR *)"DHN:");
                    UTIL_DispHexByte(6, 4, dhnBuf[connectionNum]);
                    UTIL_PutStr(7, 0, FONT0 + attrCLEARWRITE, strlen("ERR:TCP_disconnect"), (UCHAR *)"ERR:TCP_disconnect");
                    goto ERROR;
                }
                else
                {
                    UTIL_ClearScreen();
                    UTIL_PutStr(7, 2, FONT0, strlen("CLOSE OK"), (UCHAR *)"CLOSE OK");
                    UTIL_PutChar(7, 0, FONT0, keyin);
                    UTIL_WaitKey();
                }
            }
        }
    }
#else
	close(sd);		// DISCONNECT: using WIN socket method
#endif

    // Clean up SSL
	ERR_free_strings();

	// --------------------------------------------------------------------------------------------
	UTIL_ClearScreen();
	UTIL_PutStr( 6, 0, FONT0+attrCLEARWRITE, strlen("SSL CLOSED"), (UCHAR *)"SSL CLOSED" );
	UTIL_PutStr( 7, 0, FONT0+attrCLEARWRITE, strlen("ANY KEY TO RE-TEST"), (UCHAR *)"ANY KEY TO RE-TEST" );
	
	UTIL_WaitKey();
	goto START;

    // Error Handler
ERROR:
	ERR_print_errors_fp(stderr);
	
	// SSL clean up
	ERR_free_strings();

	for(;;);
}
