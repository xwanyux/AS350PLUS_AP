

//#ifndef _build_DSS_
#include "POSAPI.h"
#include "OS_PROCS.h"
#include "IPC_client.h"

/**
 *  this funciton is to open the device
 *  @param[in] acceptor        should be macro, ICC1, SAM1, SAM2, SAM3, SAM4
 *  @return apiOutOfService if open fail, device handle number if oepn success
 */
UCHAR api_ifm_open(UCHAR acceptor){
UCHAR retval;
	IPC_clientHandler(psDEV_SCR,1,1,1,&acceptor,&retval);
	return( retval );

}

/**
 *     this function is use to close the device
 *     @param[in] dhn          device handle number
 *     @retval apiFailed       the device is not open
 *     @retval apiOK           close success
 */
UCHAR api_ifm_close(UCHAR dhn){
UCHAR retval;
	IPC_clientHandler(psDEV_SCR,2,1,1,&dhn,&retval);
	return( retval );
    
}

/**
 *      this function is used to check the ICC card present or not
 *      @param[in] dhn          device handle number
 *      @retval apiReady        if the ICC is present
 *      @retval apiNotReady     if the ICC is not present
 *      @note this function only work for ICC, is other sam value, always return apiReady
 */
UCHAR api_ifm_present(UCHAR dhn){
UCHAR retval;
	IPC_clientHandler(psDEV_SCR,3,1,1,&dhn,&retval);
	return( retval );

}



/**
 *      this function is used to do cold reset to the card
 *      @param[in] dhn      device handle number
 *      @param[in] mode     reserve to future use (not used now)
 *      @param[out] atr     first byte of the atr array is the length of atr, the rest are the content of atr
 */
UCHAR api_ifm_reset(UCHAR dhn, UCHAR mode, UCHAR *atr){
UCHAR retval;
UCHAR sbuf[100];
    sbuf[0]=dhn;
    sbuf[1]=mode;
	IPC_clientHandler(psDEV_SCR,4,3,2,sbuf,&retval);
    memmove(atr,&sbuf[2],sbuf[2]);
	return( retval );
}

/**
 *      this function is to deactivate the IC card.
 *      @param[in] dhn      device handle number
 *      @retval apiFailed   the device is not open
 *      @retval apiOK       the device is deactivated
 */ 
UCHAR api_ifm_deactivate(UCHAR dhn){
UCHAR retval;
	IPC_clientHandler(psDEV_SCR,5,1,1,&dhn,&retval);
	return( retval );
 

}




/**
 *     this function is to exchange the apdu to card
 *     @param[in] dhn       device handle number
 *     @param[in] c_apdu    the first two byte must be the length of transmit apdu command (use little Edian),
 *                          the three byte start as the actual command.
 *     @param[out] r_apdu   the first two byte are the length of response apdu command (use little Endian)
 *                          the three byte start as the actual response.
 *     @retval apiFailed    the transmission of apdu failed
 *     @retval apiOK        the transmission of apdu success
 */        
UCHAR api_ifm_exchangeAPDU(UCHAR dhn, UCHAR* c_apdu, UCHAR* r_apdu){
UCHAR retval;
UCHAR sbuf[4096];
UINT16 iptLen=*c_apdu+(*(c_apdu+1))*256+2;
UINT16 optLen=0;
    sbuf[0]=dhn;
    memmove(&sbuf[1],c_apdu,iptLen);
	IPC_clientHandler(psDEV_SCR,6,3,iptLen+1,sbuf,&retval);
    optLen=sbuf[1+iptLen]+(sbuf[1+iptLen+1])*256+2;
	printf("optLen=%d\n",optLen);
    memmove(r_apdu,&sbuf[1+iptLen],optLen);
    memset(sbuf, 0x00, sizeof(sbuf));
	return( retval );
}
//#endif