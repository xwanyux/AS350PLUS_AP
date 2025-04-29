


#include "POSAPI.h"
#include "OS_PROCS.h"
#include "IPC_client.h"
#include <stdio.h>
#include <stdlib.h> /*for system function*/
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
// #include <pthread.h>
#ifdef _4G_ENABLED_
#include <net/if.h>
#endif

UINT		LAN_RxreadyLength = 0;

/**
 *     This function is to set the IP, gateway , netmask.
 *     @param[in] config            necessary information to set IP, gateway, netmask
 *     @retval apiOK                always success
 *     @note this function is just implement the linux command to do this for us
 *           ifconfig eht0 192.168.0.20 netmask 255.255.255.0  set ip and mask
 *           route add default gw 192.168.0.1  set gateway
 */ 
UCHAR api_lan_setIPconfig( API_IPCONFIG config )
{
UCHAR retval;
UCHAR iptlen=sizeof(API_IPCONFIG);
	IPC_clientHandler(psDEV_LAN,1,1,iptlen,(UCHAR *)&config,&retval);
	return( retval );
}


/**
 *  This function is to get back the IP, gateway, netmask, MAC.
 *  @param[out] config          the data structure can contain IP, gatway, netmask MAC
 *  @retval apiOK               always success
 *  @note this function is implement the linux command , with more complex linux command.
 */ 
UCHAR api_lan_getIPconfig( API_IPCONFIG *config )
{
UCHAR retval;
	IPC_clientHandler(psDEV_LAN,2,0,0,(UCHAR *)config,&retval);
	return( retval );
}



// ---------------------------------------------------------------------------
// FUNCTION: To enable the ethernet device (LAN) and establish the link to remote host.
// INPUT   : API_LAN lan 
//	     
// OUTPUT  : none.
// RETURN  : DeviceHandleNo
//           apiOutOfService
// ---------------------------------------------------------------------------

UCHAR api_lan_open( API_LAN lan ){
UCHAR retval;
UCHAR iptlen=sizeof(API_LAN);
	IPC_clientHandler(psDEV_LAN,3,1,iptlen,(UCHAR *)&lan,&retval);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To disable the LAN device and disconnect the link to host.
// INPUT   : dhn
//	     The specified device handle number.
//	     0x00 = to close all opened tasks.
// OUTPUT  : none.
// RETURN  : apiOK
//           apiFailed
// NOTE	   : by using api_lan_satatus(dhn=0) to check current link status
//           to make sure the disconnection status, it will avoid corss-talk
//           with next link
// ---------------------------------------------------------------------------
UCHAR	api_lan_close( UCHAR dhn )
{
UCHAR retval;
	IPC_clientHandler(psDEV_LAN,4,1,1,&dhn,&retval);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To determine whether LAN is ready to transmit the next data string.
// INPUT   : dhn
//           The specified device handle number.
// OUTPUT  : none.
// RETURN  : apiReady
//           apiNotReady
//	     apiFailed
// ---------------------------------------------------------------------------

/*
    only check global socket connection, if it fail return apifailed 
*/
UCHAR	api_lan_txready( UCHAR dhn )
{
UCHAR retval;
	IPC_clientHandler(psDEV_LAN,5,1,1,&dhn,&retval);
	return( retval );
}



// ---------------------------------------------------------------------------
// FUNCTION: To write the outgoing data string to LAN transmitting stream buffer.
// INPUT   : dhn
//           The specified device handle number.
//           sbuf
//           UINT  length ;     	// length of data string to be transmitted.
//	     UCHAR data[length] ;	// data string.	
// OUTPUT  : none.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_lan_txstring( UCHAR dhn, UCHAR *sbuf )
{
UCHAR retval;
UCHAR *sendbuff;
UINT  iptlen;
	
	iptlen=(*sbuf)|(*(sbuf+1)<<8)+2;
	sendbuff=malloc(iptlen+1);
	sendbuff[0]=dhn;
	memmove(sendbuff+1,sbuf,iptlen);
	IPC_clientHandler(psDEV_LAN,6,2,iptlen+1,sendbuff,&retval);
	free(sendbuff);
	return( retval );
}
// ---------------------------------------------------------------------------
// FUNCTION: To determine whether data is ready for input from the LAN device.
// INPUT   : dhn
//           The specified device handle number.
// OUTPUT  : dbuf
//           UINT  length ;     // length of the received data string.
// RETURN  : apiReady
//           apiNotReady
//	     apiFailed
// ---------------------------------------------------------------------------

/*
    for what I understand, this function to check rx have receive a complete data
    i use recv() return is 0 or not,
    if it's 0 mean finsh, else > 0 not finish

*/
UCHAR	api_lan_rxready( UCHAR dhn, UCHAR *dbuf )
{
UCHAR retval;
UCHAR sendbuff[3];

	sendbuff[0]=dhn;
	IPC_clientHandler(psDEV_LAN,7,1,1,sendbuff,&retval);
	if(retval==apiReady)
	{
		memmove(dbuf,sendbuff+1,2);
		LAN_RxreadyLength=(*dbuf)|(*(dbuf+1)<<8);
	}
	
	return( retval );
}
// ---------------------------------------------------------------------------
// FUNCTION: To read the incoming data string from LAN receiving block-stream buffer.
// INPUT   : dhn
//           The specified device handle number.
// OUTPUT  : dbuf
//           UINT  length ;     	// length of the received data string.
//	     UCHAR data[length] ;	// data string.	
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_lan_rxstring( UCHAR dhn, UCHAR *dbuf )
{
UCHAR retval;
UCHAR *sendbuff;

	sendbuff=malloc(LAN_RxreadyLength+2+1);//length of message + (UINT 2B)length + (1B)dhn
	sendbuff[0]=dhn;
	IPC_clientHandler(psDEV_LAN,8,1,1,sendbuff,&retval);
	memmove(dbuf,sendbuff+1,LAN_RxreadyLength+2);
	free(sendbuff);
	LAN_RxreadyLength=0;
	return( retval );
}
/**
 *    This function is to check the physical line is connected or not
 *    @retval apiOK            the physical line is connected
 *    @retval apiFailed        the physical line is not connected
 *    @note this function is work when you set at lesat one time ifconfig 
 *           (make sure system run at lesat one time befroe using it)
 */
UCHAR api_lan_lstatus(void)
{
UCHAR retval;
UCHAR sendbuff;
	IPC_clientHandler(psDEV_LAN,9,0,0,&sendbuff,&retval);
	return( retval );
}



// ---------------------------------------------------------------------------
// FUNCTION: To get the connection status from LAN device.
// INPUT   : dhn
//	     The specified device handle number.
// OUTPUT  : dbuf
//	     LAN status byte.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
/*
    just check the socket connection again
*/
UCHAR	api_lan_status( UCHAR dhn, UCHAR *dbuf )
{
UCHAR retval;
UCHAR recbuff[2];

	recbuff[0]=dhn;
	IPC_clientHandler(psDEV_LAN,10,1,1,recbuff,&retval);
    *dbuf=recbuff[1];
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To enable or disable DHCP.
// INPUT   : flag	FALSE = disable, TRUE = enable
// OUTPUT  : none.
// RETURN  : apiOK	-- start or stop DHCP
//           apiFailed	-- line dropped
// ---------------------------------------------------------------------------
UCHAR	api_lan_setup_DHCP( UCHAR flag )
{   
UCHAR retval;
	IPC_clientHandler(psDEV_LAN,11,1,1,&flag,&retval);
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To get the final status of DHCP process.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : apiReady	 -- DHCP has completed
//           apiNotReady -- DHCP in processing
// ---------------------------------------------------------------------------

UCHAR	api_lan_status_DHCP( void )
{
UCHAR retval;
UCHAR recbuff;
	IPC_clientHandler(psDEV_LAN,12,0,0,&recbuff,&retval);
	return( retval );
}



// ---------------------------------------------------------------------------
// FUNCTION: To PING the target IP address.
// INPUT   : dhn
//           The specified device handle number.
//           ip_len - length of IP string.
//           ip     - the target IP string.	
// OUTPUT  : ms     - connection time in mini-second.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_lan_ping( UCHAR ip_len, UCHAR *ip, ULONG *time )
{
UCHAR retval;
UCHAR buffer[ip_len+1+4];

    buffer[0] = ip_len;
    memmove(&buffer[1], ip, ip_len);
	IPC_clientHandler(psDEV_LAN,13,2,ip_len+1,buffer,&retval);
    memmove(time,&buffer[ip_len+1],4);
	return( retval );
}




