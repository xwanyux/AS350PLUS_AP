#include "OS_PROCS.h"
#include "IPC_client.h"
#include "POSAPI.h"
#include "ECL_LV1_Define.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



UCHAR	spi_flgOpen=FALSE;
ULONG   baud_rate = 8000000; //default

UCHAR SPI_Open(unsigned long iptBaudrate)
{
	baud_rate = iptBaudrate;
}

UCHAR SPI_Close(void)
{
}


UCHAR SPI_Transmit(unsigned int datLen, unsigned char *datBuffer)
{
UCHAR retval;
UCHAR args[2+datLen+datLen];
	memmove(args,&datLen,2);
	memmove(&args[2],datBuffer,datLen);
	IPC_clientHandler(psDEV_SPI,1,2,datLen+2,args,&retval);
	memmove(datBuffer,&args[2+datLen],datLen);
	return( retval );

}
UCHAR mml_spi_transmit(int not_use, UCHAR* datBuffer, unsigned int dataLen ){

	return SPI_Transmit(dataLen, datBuffer);

}
