#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "OS_FONT.h"
#include "POSAPI.h"
#include "LCDTFTAPI.h"
#include "DEV_LCD.h"
#include "bsp_types.h"
#include "GUI.h"
#include "OS_PROCS.h"
#include "IPC_client.h"
struct timeval GetNowTime() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv;
}
UCHAR	api_lcdtft_open( UCHAR deviceid )
{
UCHAR ret=1;
  IPC_clientHandler(psDEV_LCD,1,1,1,&deviceid,&ret);
  
	return ret;
  
}
// ---------------------------------------------------------------------------
// FUNCTION: To disable the display device, no more message can be shown on the screen.
// INPUT   : dhn
//	     The specified device handle number.
//	     0x00 = to close all opened tasks.
// OUTPUT  : none.
// RETURN  : apiOK
//	     apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_lcdtft_close( UCHAR dhn )
{
UCHAR ret=1;
  IPC_clientHandler(psDEV_LCD,2,1,1,&dhn,&ret);
  
    return ret;
}
// ---------------------------------------------------------------------------
// FUNCTION: To clear the designated display area.
// INPUT   : dhn
//	     The specified device handle number.
//
//	     para
//	     ------ TFT ------	// parameter structure 16 bytes
//	     UINT   row;	// beginning row number
//	     UINT   col;	// total pixel rows to be cleared (x8)
//	     UCHAR  font;	// font id & attribute
//	     UCHAR  rgb;	// RGB mode
//	     UCHAR  FG_palette[3];	// foreground palette (3 bytes)
//	     UCHAR  fontH;	// font height in dots
//	     UCHAR  fontW;	// font width in dots
//	     UCHAR  BG_palette[3];	// background palette (3 bytes)
//	     UCHAR  RFU[2];	// reserved (2 bytes)
//
// OUTPUT  : none.
// RETURN  : apiOK
//	     apiFailed
// NOTE    : If the "start row position" is -1, it means to clear the whole screen,
//           only the "font attribute" is effective.
// ---------------------------------------------------------------------------
//UCHAR	api_lcdtft_clear( UCHAR dhn, UCHAR *sbuf )
UCHAR	api_lcdtft_clear( UCHAR dhn, API_LCDTFT_PARA para )
{
UCHAR argulen=1+sizeof(API_LCDTFT_PARA);
UCHAR buffer[argulen];
UCHAR ret=1;
  buffer[0]=dhn;
  memmove(&buffer[1],&para,argulen-1);
  IPC_clientHandler(psDEV_LCD,3,2,argulen,buffer,&ret);
      return ret;
}

//UCHAR	api_lcdtft_putstring( UCHAR dhn, UCHAR *sbuf, UCHAR *dbuf )
UCHAR api_lcdtft_putstring( UCHAR dhn, API_LCDTFT_PARA para, UCHAR *dbuf )
{
UCHAR dbuflen=1+(*dbuf);//len(1B)+datalen
UCHAR argulen=1+sizeof(API_LCDTFT_PARA)+dbuflen;
UCHAR buffer[argulen+1];
UCHAR ret=1;
struct timeval nowtime;
  buffer[0]=dhn;
  memmove(&buffer[1],&para,sizeof(API_LCDTFT_PARA));
  memmove(&buffer[1+sizeof(API_LCDTFT_PARA)],dbuf,dbuflen);
	
  ret=0;
//   nowtime=GetNowTime();
	// printf("!!Client:%d us\n",nowtime.tv_usec);
  IPC_clientHandler(psDEV_LCD,4,3,argulen,buffer,&ret);
//   nowtime=GetNowTime();
	// printf("!!Client:%d us\n",nowtime.tv_usec);
      return ret;
}

// ---------------------------------------------------------------------------
// FUNCTION: To write the given image data to the display at the designated area.
// INPUT   : dhn
//	     The specified device handle number.
//
//	     image
//	     ------ TFT ------	// 
//	     UINT		ID;		// id number of the built-in icon (0..n)
//	     UCHAR		RGB;		// RGB mode
//
//	     GUI_BITMAP
//		UINT		XSize;			// width  size in dots
//		UINT		YSize;			// height size in dots
//		UINT		BytesPerLine;		// bytes per line
//		UINT		BitsPerPixel;		// bits  per pixel
//		UCHAR		*pData;			// pointer to picture data
//		UCHAR		*pPal;			// pointer to palette
//		UCHAR		*pMethods;		// pointer to methods
// 
// OUTPUT  : none.
// RETURN  : apiOK
//	     apiFailed
// ---------------------------------------------------------------------------
//UCHAR	api_lcdtft_putgraphics( UCHAR dhn, API_LCDTFT_GRAPH *sbuf )
UCHAR	api_lcdtft_putgraphics( UCHAR dhn, API_LCDTFT_GRAPH graph)
{
ULONG gdatalen=graph.Bitmap.BytesPerLine*graph.Bitmap.YSize;
ULONG argulen=1+sizeof(API_LCDTFT_GRAPH)+gdatalen;//dhn(1B)+API_LCDTFT_GRAPH(24B)-(*pData-*pPal-*pMethods)(12B)+graphic data length
UCHAR buffer[argulen+1];//+1 API return value
UCHAR ret=1;
	memset(buffer,0,argulen);
  	buffer[0]=dhn;
  	memmove(&buffer[1],&graph,sizeof(API_LCDTFT_GRAPH));
  	memmove(&buffer[argulen-gdatalen],graph.Bitmap.pData,gdatalen);
	// printf("client send: ");
	// for(ULONG i=0;i<60;i++)
	// {
	// 	printf("%d ",buffer[i]);
	// }
	// printf("\n");
	for(ULONG i=0,value=0;;i++)
	    {
	    	// if(value=*(graph.Bitmap.pData+i)>0)
	    	if(buffer[argulen-gdatalen+i]>0)
	    	{
	    		printf("client i=%d value= ",i );
				for(int g=0;g<10;g++)
					printf("%d ",buffer[argulen-gdatalen+i+g]);
	    		break;
	    	}
	    }
      printf("\n");
	IPC_clientHandler(psDEV_LCD,5,2,argulen,buffer,&ret);
	
	
  	return ret;

}
//UCHAR	api_lcdtft_showICON( UCHAR dhn, API_LCDTFT_ICON *sbuf )
UCHAR   api_lcdtft_showICON( UCHAR dhn, API_LCDTFT_ICON icon )
{
UCHAR argulen=1+sizeof(API_LCDTFT_ICON);
UCHAR buffer[argulen];
UCHAR ret=1;
  buffer[0]=dhn;
  memmove(&buffer[1],&icon,sizeof(API_LCDTFT_ICON));
  IPC_clientHandler(psDEV_LCD,6,2,argulen,buffer,&ret);
      return ret;
}
// ---------------------------------------------------------------------------
// FUNCTION: To control related graphics appearance for PCD application,
//	     such as TAP ICON, LED ICON...
// INPUT   : dhn
//	     The specified device handle number.
//
//	     para
//	     ------ TFT ------	// 
//	     UINT   ID;		// id of the built-in icon (0..n)
//	     UINT   BlinkOn;	// time for icon ON period in 10ms units  (0xFFFF=always ON, 0x0000=no blinking)
//	     UINT   BlinkOff;	// time for icon OFF period in 10ms units
// 	     UCHAR  RFU[10];
// OUTPUT  : none.
// RETURN  : apiOK
//	     apiFailed
// NOTE    : PCD kernel shall handle the LEDs while running contactless app.
// ---------------------------------------------------------------------------
UCHAR	api_lcdtft_showPCD( UCHAR dhn, API_PCD_ICON icon )
{
UCHAR argulen=1+sizeof(API_PCD_ICON);
UCHAR buffer[argulen];
UCHAR ret=1;
  buffer[0]=dhn;
  memmove(&buffer[1],&icon,sizeof(API_PCD_ICON));
  IPC_clientHandler(psDEV_LCD,7,2,argulen,buffer,&ret);
      return ret;
}
// ---------------------------------------------------------------------------
// FUNCTION: To write the given Windows bmp image data to the display at the designated area.
// INPUT   : dhn
//	     The specified device handle number.
//
//	     bmp
//	     The Windows format BMP data. (monochrome, black & white)
//
//	     degrees
//	     CCW_0, CCW_90, CCW_180, CCW_270
// OUTPUT  : none.
// RETURN  : apiOK
//	     apiFailed
// ---------------------------------------------------------------------------
//UCHAR	api_lcdtft_putwinbmp( UCHAR dhn, UCHAR *sbuf, UCHAR *winbmp )
UCHAR	api_lcdtft_putwinbmp( UCHAR dhn, API_LCDTFT_WINBMP *bmppara, UCHAR *winbmp )
{
UINT  winbmpsize=320*240/8;
ULONG argulen=1+sizeof(API_LCDTFT_WINBMP)+winbmpsize;//dhn(1B)+API_LCDTFT_WINBMP+winbmp data length
UCHAR buffer[argulen+1];//+1 API return value
UCHAR ret=1;
	memset(buffer,0,argulen);
  	buffer[0]=dhn;
  	memmove(&buffer[1],bmppara,sizeof(API_LCDTFT_WINBMP));
  	memmove(&buffer[argulen-winbmpsize],winbmp,winbmpsize);
  	IPC_Sender(psDEV_LCD,8,3,argulen,buffer);

  	while(IPC_Reciever(buffer,argulen+1)!=TRUE);
  	ret=buffer[0];
	memmove(bmppara,&buffer[2],sizeof(API_LCDTFT_WINBMP));
	memmove(winbmp,&buffer[argulen+1-winbmpsize],winbmpsize);
  	return ret;
}
//UCHAR	SIGNPAD_lcdtft_putstring( UCHAR dhn, UCHAR *sbuf, UCHAR *dbuf, UINT CCWdegrees )
UCHAR  SIGNPAD_lcdtft_putstring( UCHAR dhn, API_LCDTFT_PARA para, UCHAR *dbuf, UINT CCWdegrees )
{
UCHAR strlen=1+*dbuf;
ULONG argulen=1+sizeof(API_LCDTFT_PARA)+strlen+2;
UCHAR buffer[argulen];
UCHAR ret=1;
	memset(buffer,0,argulen);
  	buffer[0]=dhn;
  	memmove(&buffer[1],&para,sizeof(API_LCDTFT_PARA));
  	memmove(&buffer[1+sizeof(API_LCDTFT_PARA)],dbuf,strlen);
  	memmove(&buffer[argulen-2],&CCWdegrees,2);
  	IPC_clientHandler(psDEV_LCD,9,4,argulen,buffer,&ret);
      return ret;
}

UCHAR api_lcdtft_initfont( API_LCD_FONT ft  )
{
ULONG	FontCodeLen=ft.codEndAddr-ft.codStAddr+1;
ULONG	FontBmpLen=ft.bmpEndAddr-ft.bmpStAddr+1;
ULONG argulen=sizeof(API_LCD_FONT)+FontCodeLen+FontBmpLen+8;
UCHAR buffer[argulen];
UCHAR ret=1;
//buffer=
// struct API_LCD_FONT+
// font code map length(4B)+
// font code map+
// font bitmap length(4B)+
// font bitmap
	memset(buffer,0,argulen);
  	memmove(buffer,&ft,sizeof(API_LCD_FONT));
  	memmove(&buffer[sizeof(API_LCD_FONT)],&FontCodeLen,4);
  	memmove(&buffer[sizeof(API_LCD_FONT)+4],ft.codStAddr,FontCodeLen);
  	memmove(&buffer[sizeof(API_LCD_FONT)+FontCodeLen+4],&FontBmpLen,4);
  	memmove(&buffer[sizeof(API_LCD_FONT)+FontCodeLen+8],ft.bmpStAddr,FontBmpLen);
	printf("client FontCodeLen=%ld  FontBmpLen=%ld\n",FontCodeLen,FontBmpLen);
  	IPC_clientHandler(psDEV_LCD,10,1,argulen,buffer,&ret);
	return ret;
}

UCHAR api_lcdtft_fillRECT( UCHAR dhn, API_LCDTFT_RECT rect)
{
    UCHAR argulen = 1 + sizeof(API_LCDTFT_RECT);
    UCHAR buffer[argulen];
    UCHAR ret = 1;


    buffer[0] = dhn;
    memmove(&buffer[1], &rect, sizeof(API_LCDTFT_RECT));
    IPC_clientHandler(psDEV_LCD, 11, 2, argulen, buffer, &ret);
        return ret;
}
