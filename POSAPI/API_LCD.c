//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : MAX32550 (32-bit Platform)				    **
//**  PRODUCT  : AS330_mPOS						    **
//**                                                                        **
//**  FILE     : API_LCD.C 	                                            **
//**  MODULE   : api_lcd_open()				                    **
//**		 api_lcd_close()					    **
//**		 api_lcd_clear()					    **
//**		 api_lcd_putstring()					    **
//**		 api_lcd_putgraphics()					    **
//**		 api_lcd_getgraphics()					    **
//**									    **
//**  FUNCTION : API::LCD (Liquid Crystal Display Module)		    **
//**  VERSION  : V1.00                                                      **
//**  DATE     : 2018/04/18                                                 **
//**  EDITOR   : James Hsieh                                                **
//**                                                                        **
//**  Copyright(C) 2018 SymLink Corporation. All rights reserved.           **
//**                                                                        **
//****************************************************************************
//============================================================================
//
//----------------------------------------------------------------------------
#include <string.h>

#include "POSAPI.h"

#include "OS_PROCS.h"
#include "OS_FONT.h"

#include "DEV_LCD.h"
#include "bsp_gpio.h"
// #include "DEV_LCDTFT.h"
// #include "LCDTFTAPI.h"


UCHAR		os_DHN_LCD = 0;
UCHAR		os_LCD_ATTR = 0;


// ---------------------------------------------------------------------------
// FUNCTION: To enable the display device, which is enabled by system default.
// INPUT   : deviceid (RFU)
//	     The device ID, there are two basic types of display are used in POS
//	     application, the operator's display and the customer's display.
//	     0x00 - default monochrome STN LCD
//	     0x01 - TFT color LCD (GF320x240)
// OUTPUT  : none.
// RETURN  : DeviceHandleNo
//	     apiOutOfService
// NOTE    : Prior to calling this function, OS_LCD_Init() shall be called first.
// ---------------------------------------------------------------------------
#if	1
UCHAR api_lcd_open( UCHAR deviceid )
{
#ifdef	_LCD_ENABLED_

	if( os_DHN_LCD != 0 )	// already opened?
	  return( apiOutOfService );
	
//	OS_LCD_Start( os_pLcd );
	  
	os_DHN_LCD = api_lcdtft_open( deviceid);
	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$os_DHN_LCD =%d\n",os_DHN_LCD);
	return( os_DHN_LCD );
	// return( api_lcdtft_open( deviceid) );
#else

	return( api_lcdtft_open( deviceid) );
#endif
}
#endif

// ---------------------------------------------------------------------------
// FUNCTION: To disable the display device, no more message can be shown on the screen.
// INPUT   : dhn
//	     The specified device handle number.
//	     0x00 = to close all opened tasks.
// OUTPUT  : none.
// RETURN  : apiOK
//	     apiFailed
// ---------------------------------------------------------------------------
#if	1
UCHAR api_lcd_close( UCHAR dhn )
{
	return( api_lcdtft_close( dhn ) );
}
#endif

// ---------------------------------------------------------------------------
// FUNCTION: To clear the designated display area.
// INPUT   : dhn
//	     The specified device handle number.
//	     sbuf
//	     UCHAR  row ;      // start row position.
//	     UCHAR  count ;    // total rows to be cleared.
//	     UCHAR  font ;     // font ID & attribute.
// OUTPUT  : none.
// RETURN  : apiOK
//	     apiFailed
// NOTE    : If the "start row position" is -1, it means to clear the whole screen,
//           only the "font attribute" is effective.
// ---------------------------------------------------------------------------
UCHAR	api_lcd_clear( UCHAR dhn, UCHAR *sbuf )
{
UCHAR	row;
UCHAR	cnt;
UCHAR	font;
UCHAR	fid;
ULONG	i;
ULONG	hbyte;
ULONG	tcnt;
OS_FDT	*pFdt;


API_LCDTFT_PARA para;
UCHAR	attr;


	row = sbuf[0];
	cnt = sbuf[1];
	font = sbuf[2];	// attr + fid
	attr = font & 0xF0;
	  
	if( row == 255 )	// clear whole screen?
	  {
	  memset( (UINT8 *)&para, 0x00, sizeof(API_LCDTFT_PARA) );
	  para.Row = 0xFFFF;
	  
	  if( attr & LCD_ATTR_REVERSE_MASK )
	    {
	    para.BG_Palette[0] = 0x00;	// black
	    para.BG_Palette[1] = 0x00;
	    para.BG_Palette[2] = 0x00;
	    }
	  else
	    {
	    para.BG_Palette[0] = 0xFF;	// white
	    para.BG_Palette[1] = 0xFF;
	    para.BG_Palette[2] = 0xFF;
	    }
	  
	  return( api_lcdtft_clear( dhn, para ) );
	  }
	else
	  {
	  memset( (UINT8 *)&para, 0x00, sizeof(API_LCDTFT_PARA) );
	  para.Row = row;
	  para.Col = cnt;
	  
	  switch( font & 0x0F )
	        {
	        case FONT0:
	        
            	     para.Font = LCDTFT_FONT0 + attr;
            	     para.FontHeight = TFTLCD_FONT0_H;
            	     para.FontWidth  = TFTLCD_FONT0_W;
	             break;
	             
	        case FONT1:

            	     para.Font = LCDTFT_FONT1 + attr;
            	     para.FontHeight = TFTLCD_FONT1_H;
            	     para.FontWidth  = TFTLCD_FONT1_W;
	             break;
	             
	        case FONT2:

            	     para.Font = LCDTFT_FONT2 + attr;
            	     para.FontHeight = TFTLCD_FONT2_H;
            	     para.FontWidth  = TFTLCD_FONT2_W;
	             break;

	        case FONT3:

            	     para.Font = LCDTFT_FONT3 + attr;
            	     para.FontHeight = TFTLCD_FONT3_H;
            	     para.FontWidth  = TFTLCD_FONT3_W;
	             break;
	              
	        default:

            	     para.Font = LCDTFT_FONT0 + attr;
            	     para.FontHeight = TFTLCD_FONT0_H;
            	     para.FontWidth  = TFTLCD_FONT0_W;
	             break;
	        }
	  
//          if( (font & 0x0F) == FONT0 )
//            {
//            para.Font = LCDTFT_FONT0 + attr;
//            para.FontHeight = TFTLCD_FONT0_H;
//            para.FontWidth  = TFTLCD_FONT0_W;
//            }
//          else
//            {
//            para.Font = LCDTFT_FONT1 + attr;
//            para.FontHeight = TFTLCD_FONT1_H;
//            para.FontWidth  = TFTLCD_FONT1_W;
//            }

	para.RGB = 0;	// 2013-08-13

	  if( attr & LCD_ATTR_REVERSE_MASK )
	    {
	    para.BG_Palette[0] = 0x00;	// black
	    para.BG_Palette[1] = 0x00;
	    para.BG_Palette[2] = 0x00;
	    }
	  else
	    {
	    para.BG_Palette[0] = 0xFF;	// white
	    para.BG_Palette[1] = 0xFF;
	    para.BG_Palette[2] = 0xFF;
	    }       
	  
//	  lite_printf("\r\nRow=%x", para.Row);
//	  lite_printf("\r\nCol=%x", para.Col);
//	  lite_printf("\r\nFontID=%x", para.Font);
//	  lite_printf("\r\nFontHeight=%x", para.FontHeight);
//	  lite_printf("\r\nFontWidth=%x", para.FontWidth);
//	  lite_printf("\r\nBG0=%x", para.BG_Palette[0] );
//	  lite_printf("\r\nBG1=%x", para.BG_Palette[1] );
//	  lite_printf("\r\nBG2=%x", para.BG_Palette[2] );
	  
	  return( api_lcdtft_clear( dhn, para ) );
	  }
}

// ---------------------------------------------------------------------------
// FUNCTION: To write the given string to the display at the specified position.
// INPUT   : dhn
//	     The specified device handle number.
//	     sbuf
//	     UCHAR  row ;      // start row position of the cursor.
//	     UCHAR  col ;      // start column position of the cursor.
//	     UCHAR  font ;     // font ID & attribute.
//	     dbuf
//	     UCHAR  length;        // length of the given string.
//	     UCHAR  data[length];  // character string to be displayed.
// OUTPUT  : none.
// RETURN  : apiOK
//	     apiFailed
// ---------------------------------------------------------------------------
UCHAR api_lcd_putstring( UCHAR dhn, UCHAR *sbuf, UCHAR *dbuf )
{
UCHAR	row;
UCHAR	col;
UCHAR	font;
UCHAR	fid;
UCHAR	*pData;
UCHAR	*pImage;
ULONG	i;
ULONG	len;
ULONG	tcnt = 0;
OS_FDT	*pFdt = 0;
OS_FDT	*pFdt1 = 0;

API_LCDTFT_PARA para;
UCHAR	attr;

	
	memset( (UCHAR *)&para, 0x00, sizeof(API_LCDTFT_PARA) );
	
	row = sbuf[0];
	col = sbuf[1];
	font = sbuf[2];		// attr + fid
	attr = font & 0xF0;
	// printf("row=%d  col=%d  attr=0x%02x\n",row,col,attr);
	para.Row = row;
	para.Col = col;

	switch( font & 0x0F )
	      {
	      case FONT0:
	      
          	   para.Font = LCDTFT_FONT0 + attr;
          	   para.FontHeight = TFTLCD_FONT0_H;
          	   para.FontWidth  = TFTLCD_FONT0_W;
	           break;
	           
	      case FONT1:

          	   para.Font = LCDTFT_FONT1 + attr;
          	   para.FontHeight = TFTLCD_FONT1_H;
          	   para.FontWidth  = TFTLCD_FONT1_W;
	           break;
	           
	      case FONT2:

          	   para.Font = LCDTFT_FONT2 + attr;
          	   para.FontHeight = TFTLCD_FONT2_H;
          	   para.FontWidth  = TFTLCD_FONT2_W;
	           break;

	      case FONT3:

          	   para.Font = LCDTFT_FONT3 + attr;
          	   para.FontHeight = TFTLCD_FONT3_H;
          	   para.FontWidth  = TFTLCD_FONT3_W;
	           break;

	      default:
	      
            	   para.Font = LCDTFT_FONT0 + attr;
            	   para.FontHeight = TFTLCD_FONT0_H;
            	   para.FontWidth  = TFTLCD_FONT0_W;
	           break;
	      }

//	if( (font & 0x0F) == FONT0 )
//	  {
//	  para.Font = LCDTFT_FONT0 + attr;
//	  para.FontHeight = TFTLCD_FONT0_H;
//	  para.FontWidth  = TFTLCD_FONT0_W;
//	  }
//	else
//	  {
//	  para.Font = LCDTFT_FONT1 + attr;
//	  para.FontHeight = TFTLCD_FONT1_H;
//	  para.FontWidth  = TFTLCD_FONT1_W;
//	  }
	
	para.RGB = 0;	// 2013-08-13
	
	para.FG_Palette[0] = 0x00;
	para.FG_Palette[1] = 0x00;
	para.FG_Palette[2] = 0x00;
	
	para.BG_Palette[0] = 0xff;
	para.BG_Palette[1] = 0xff;
	para.BG_Palette[2] = 0xff;
	
	return( api_lcdtft_putstring( dhn, para, dbuf ) );

}
UCHAR api_lcd_initfont( API_LCD_FONT ft  )
{
api_lcdtft_initfont( ft  );
}

void	api_lcd_convertfont( UCHAR flag )
{
	
}