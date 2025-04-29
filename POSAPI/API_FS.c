//
//============================================================================
//****************************************************************************
//**                                                                        **
//**  PROJECT  : IMX6UL							    **
//**  PRODUCT  : AS350	                                                    **
//**                                                                        **
//**  FILE     : API_FS.C 	                                            **
//**  MODULE   : api_fs_select()					    **
//**		 api_fs_init()						    **
//**		 api_fs_format()					    **
//**		 api_fs_open()						    **
//**		 api_fs_close()						    **
//**		 api_fs_create()					    **
//**		 api_fs_delete()					    **
//**		 api_fs_read()						    **
//**		 api_fs_write()						    **
//**		 api_fs_seek()						    **
//**		 api_fs_tell()						    **
//**		 api_fs_sync()		RFU				    **
//**		 api_fs_directory()	RFU				    **
//**									    **
//**		 api_sd_init()						    **
//**		 api_sd_open()						    **
//**		 api_sd_close()						    **
//**		 api_sd_create()					    **
//**		 api_sd_read()						    **
//**		 api_sd_write()						    **
//**		 api_sd_seek()						    **
//**		 api_sd_tell()						    **
//**		 api_sd_delete()					    **
//**									    **
//**  FUNCTION : API::FS (File System for FLASH or SD Card)		    **
//**  VERSION  : V1.00                                                      **
//**  DATE     : 2023/06/12						    **
//**  EDITOR   : James Hsieh                                                **
//**                                                                        **
//**  Copyright(C) 2023-2025 SymLink Corporation. All rights reserved.	    **
//**                                                                        **
//****************************************************************************
//============================================================================
//
//----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h> /*for system function*/
#include <string.h>

#include "POSAPI.h"
#include "OS_PROCS.h"
#include "IPC_client.h"
//#include "FSAPI.h"
#include "FS.h"

struct	FILE		AP_FS_fh;
struct	FILE_DIR	AP_FS_dir;


// ---------------------------------------------------------------------------
// FUNCTION: Select media of the FS.
// INPUT   : media - MEDIA_FLASH/MEDIA_SD/MEDIA_RAM.
// OUTPUT  : none.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_fs_select( UCHAR media )
{
UCHAR	retval;
UCHAR	args[1];


	args[0] =  media;
	IPC_clientHandler( psDEV_FS, 1, 1, 1, args, &retval );
	
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To initialize file system.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_fs_init( void )
{
UCHAR	retval;
UCHAR	args[1];


	IPC_clientHandler( psDEV_FS, 2, 0, 0, args, &retval );
	
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To format and delete all files in the media. (n/a for SD)
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : none.
// ---------------------------------------------------------------------------
void	api_fs_format( void )
{
UCHAR	retval;
UCHAR	args[1];


	IPC_clientHandler( psDEV_FS, 3, 0, 0, args, &retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: Open file.
// INPUT   : fileName
//	     mode	(RFU)
// OUTPUT  : none.
// RETURN  : file handle.
// ---------------------------------------------------------------------------
struct	FILE *api_fs_open( char *fileName, UCHAR mode )
{
UCHAR	retval;
ULONG	len;
ULONG	heaplen;
UCHAR	*sbuf;


	sbuf = malloc( strlen(fileName) + 1 + sizeof(struct FILE) );
	if( sbuf == NULL )
	  return( NULL );
	
	// FileName(n), Mode(1), struct FILE(n)
	len = strlen(fileName);
	memmove( &sbuf[0], fileName, len );
	sbuf[len] = mode;
	
//	heaplen = api_fs_heap_size();
//	POSAPI_DispHexByte( 0, 0, (heaplen & 0xff000000) >> 24 );
//	POSAPI_DispHexByte( 0, 2, (heaplen & 0x00ff0000) >> 16 );
//	POSAPI_DispHexByte( 0, 4, (heaplen & 0x0000ff00) >> 8 );
//	POSAPI_DispHexByte( 0, 6, (heaplen & 0x000000ff) );

	IPC_clientHandler( psDEV_FS, 20, 2, len+1, sbuf, &retval );	// api_fs_open()
	
//	heaplen = api_fs_heap_size();
//	POSAPI_DispHexByte( 1, 0, (heaplen & 0xff000000) >> 24 );
//	POSAPI_DispHexByte( 1, 2, (heaplen & 0x00ff0000) >> 16 );
//	POSAPI_DispHexByte( 1, 4, (heaplen & 0x0000ff00) >> 8 );
//	POSAPI_DispHexByte( 1, 6, (heaplen & 0x000000ff) );
//	for(;;);
	
	if( retval == apiOK )
	  {
	  memmove( &AP_FS_fh, &sbuf[len+1], sizeof(struct FILE) );
	  free( sbuf);
	  
	  return( &AP_FS_fh );
	  }
	else
	  {
	  free(sbuf);
	  return( NULL );
	  }
}

// ---------------------------------------------------------------------------
// FUNCTION: Close file.
// INPUT   : file handle.
// OUTPUT  : none.
// RETURN  : none.
// ---------------------------------------------------------------------------
void	api_fs_close( struct FILE *pf )
{
UCHAR	retval;
ULONG	len;
UCHAR	*sbuf;


	sbuf = malloc( sizeof(struct FILE) );
	if( sbuf == NULL )
	  return;
	
	// struct FILE(n)
	memmove( &sbuf[0], (UCHAR *)pf, sizeof(struct FILE) );

	// UINT8 PID, UINT8 APInum, UINT8 ArgsNum, UINT32 IptSize, UINT8 *Arg, UINT8 *retval
	IPC_clientHandler( psDEV_FS, 21, 1, sizeof(struct FILE), sbuf, &retval );	// api_fs_close()
	
	free( sbuf);
}

// ---------------------------------------------------------------------------
// FUNCTION: Create file.
// INPUT   : fname
//	     fileType	(RFU)
// OUTPUT  : none.
// RETURN  : apiOK
//	     apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_fs_create( char fname[], unsigned short fileType )
{
UCHAR	retval;
ULONG	len;
UCHAR	*sbuf;


	sbuf = malloc( 1 + strlen(fname) + 2 );
	if( sbuf == NULL )
	  return( apiFailed );
	
	// api_fs2_create( UCHAR len, char fname[], unsigned short fileType )
	len = strlen(fname);
	sbuf[0] = len;
	memmove( &sbuf[1], fname, len );
	fileType = 0;	// 2023-09-18, always 0
	sbuf[1+len+0] = fileType & 0x00ff;
	sbuf[1+len+1] = (fileType & 0xff00) >> 8;

	IPC_clientHandler( psDEV_FS, 6, 3, 1+len+2, sbuf, &retval );
	
	free( sbuf );
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: Delete file.
// INPUT   : fname
// OUTPUT  : none.
// RETURN  : apiOK
//	     apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_fs_delete( char fname[] )
{
UCHAR	retval;
ULONG	len;
UCHAR	*sbuf;


	sbuf = malloc( 1 + strlen(fname) );
	if( sbuf == NULL )
	  return( apiFailed );
	
	// api_fs2_delete( UCHAR len, char fname[] )
	len = strlen(fname);
	sbuf[0] = len;
	memmove( &sbuf[1], fname, len );

	IPC_clientHandler( psDEV_FS, 7, 1, 1+len, sbuf, &retval );
	
	free( sbuf );
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: Read file.
// INPUT   : pFile
//	     length
// OUTPUT  : buff
// RETURN  : length of data read.
// ---------------------------------------------------------------------------
ULONG	api_fs_read( struct FILE *pFile, UCHAR *buff, ULONG length )
{
UCHAR	retval;
ULONG	len;
UCHAR	*sbuf;


	sbuf = malloc( sizeof(struct FILE)*2 + 4 + length );
	if( sbuf == NULL )
	  return( 0 );
	
	// ULONG api_fs_read( struct FILE *pFile, UCHAR *buff, ULONG length )
	// INPUT:  pFile, length1
	// OUTPUT: pFile, length2, buff, (pFile)
	len = sizeof(struct FILE);
	memmove( &sbuf[0], (UCHAR *)pFile, len );
	sbuf[len+0] = length & 0x000000ff;
	sbuf[len+1] = (length & 0x0000ff00) >> 8;
	sbuf[len+2] = (length & 0x00ff0000) >> 16;
	sbuf[len+3] = (length & 0xff000000) >> 24;

	IPC_clientHandler( psDEV_FS, 22, 2, len+4, sbuf, &retval );
	
	len = sbuf[len+0] + sbuf[len+1]*0x100 + sbuf[len+2]*0x10000 + sbuf[len+3]*0x1000000;
	memmove( buff, &sbuf[sizeof(struct FILE)+4], len );
	
	// update file handle
	memmove( (UCHAR *)pFile, &sbuf[sizeof(struct FILE)+4+length], sizeof(struct FILE) );
//	POSAPI_DispHexWord( 2, 0, pFile->logic_position );
	
	free( sbuf );
	return( len );
}

// ---------------------------------------------------------------------------
// FUNCTION: Write file.
// INPUT   : pFile
//	     buff
//	     length
// OUTPUT  : none.
// RETURN  : length of data written.
// ---------------------------------------------------------------------------
ULONG	api_fs_write( struct FILE *pFile, UCHAR *buff, ULONG length )
{
UCHAR	retval;
ULONG	len;
UCHAR	*sbuf;


	sbuf = malloc( sizeof(struct FILE)*2 + 4 + length );
	if( sbuf == NULL )
	  return( 0 );
	
	// ULONG api_fs_write( struct FILE *pFile, UCHAR *buff, ULONG length )
	// INPUT:  pFile, length1, buff
	// OUTPUT: pFile, length2, buff, (pFile)
	len = sizeof(struct FILE);
	memmove( &sbuf[0], (UCHAR *)pFile, len );
	sbuf[len+0] = length & 0x000000ff;
	sbuf[len+1] = (length & 0x0000ff00) >> 8;
	sbuf[len+2] = (length & 0x00ff0000) >> 16;
	sbuf[len+3] = (length & 0xff000000) >> 24;
	
	memmove( &sbuf[len+4], buff, length );
	
	IPC_clientHandler( psDEV_FS, 23, 3, len+4+length, sbuf, &retval );
	
	len = sbuf[len+0] + sbuf[len+1]*0x100 + sbuf[len+2]*0x10000 + sbuf[len+3]*0x1000000;

	// update file handle
	memmove( (UCHAR *)pFile, &sbuf[sizeof(struct FILE)+4+length], sizeof(struct FILE) );
//	POSAPI_DispHexWord( 2, 0, pFile->logic_position );

	free( sbuf );
	return( len );
}

// ---------------------------------------------------------------------------
// FUNCTION: Seek file to the position.
// INPUT   : pFile
//	     position
// OUTPUT  : none.
// RETURN  : apiOK
//	     apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_fs_seek( struct FILE *pFile, ULONG position )
{
UCHAR	retval;
ULONG	len;
UCHAR	*sbuf;


	sbuf = malloc( sizeof(struct FILE)*2 + 4 );
	if( sbuf == NULL )
	  return( 0 );
	
	// UCHAR api_fs_seek( struct FILE *pFile, ULONG position )
	// INPUT:  pFile, position
	// OUTPUT: pFile, (position), (pFile)
	len = sizeof(struct FILE);
	memmove( &sbuf[0], (UCHAR *)pFile, len );
	sbuf[len+0] = position & 0x000000ff;
	sbuf[len+1] = (position & 0x0000ff00) >> 8;
	sbuf[len+2] = (position & 0x00ff0000) >> 16;
	sbuf[len+3] = (position & 0xff000000) >> 24;
	
	IPC_clientHandler( psDEV_FS, 24, 2, len+4, sbuf, &retval );
	
	// update file handle
	memmove( (UCHAR *)pFile, &sbuf[sizeof(struct FILE)+4], sizeof(struct FILE) );
	
	free( sbuf );
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: Tell the position of file.
// INPUT   : pFile
// OUTPUT  : none.
// RETURN  : position of file.
// ---------------------------------------------------------------------------
ULONG	api_fs_tell( struct FILE *pFile )
{
UCHAR	retval;
ULONG	len;
UCHAR	*sbuf;

#if	0
	sbuf = malloc( sizeof(struct FILE) + 4 );
	if( sbuf == NULL )
	  return( 0 );
	
	// ULONG api_fs_tell( struct FILE *pFile )
	// INPUT:  pFile
	// OUTPUT: pFile, length2
	len = sizeof(struct FILE);
	memmove( &sbuf[0], (UCHAR *)pFile, len );

	IPC_clientHandler( psDEV_FS, 25, 1, len, sbuf, &retval );
	len = sbuf[sizeof(struct FILE)+0] + sbuf[sizeof(struct FILE)+1]*0x100 + sbuf[sizeof(struct FILE)+2]*0x10000 + sbuf[sizeof(struct FILE)+3]*0x1000000;
	
	free( sbuf );
	return( len );
#else
	return( pFile->logic_position );
#endif
}

// ---------------------------------------------------------------------------
// FUNCTION: Get the contents of the FS directory.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : FILE_DIR
// ---------------------------------------------------------------------------
struct	FILE_DIR *api_fs_directory( void )
{
UCHAR	retval;
ULONG	len;
UCHAR	*sbuf;


	sbuf = malloc( sizeof(struct FILE_DIR) );
	if( sbuf == NULL )
	  return( NULL );
	
	IPC_clientHandler( psDEV_FS, 26, 0, 0, sbuf, &retval );	// api_fs_open()
	
	if( retval == apiOK )
	  {
	  memmove( &AP_FS_dir, &sbuf[0], sizeof(struct FILE_DIR) );
	  free( sbuf);
	  
	  return( &AP_FS_dir );
	  }
	else
	  {
	  free(sbuf);
	  return( NULL );
	  }
}

// ---------------------------------------------------------------------------
// FUNCTION: 
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : 
// ---------------------------------------------------------------------------
UINT	SFS_max_file_cnt( void )
{
UCHAR	retval;
ULONG	len;
UCHAR	sbuf[32];


	IPC_clientHandler( psDEV_FS, 27, 0, 0, sbuf, &retval );
	len = sbuf[0] + sbuf[1]*0x100;
	
	return( len );
}


// ---------------------------------------------------------------------------
static inline	UCHAR	api_fs_open_EX( UCHAR nameLen, char *fileName, ULONG *fid )
{
UCHAR	retval;
UCHAR	sbuf[FILE_NAME_SIZE+16];
UCHAR	length;


	length = nameLen;
	if( length > (FILE_NAME_SIZE+1) )
	  return( apiFailed );
	
	sbuf[0] = nameLen;			// length of file name
	memmove( &sbuf[1], fileName, length );	// file name
	
//	&sbuf[1+nameLen] = *fp;
	IPC_clientHandler( psDEV_FS,4,3,1+nameLen,sbuf,&retval );
	
	if( retval == apiOK )
	  {
	  *fid = *(sbuf+1+nameLen+0) + *(sbuf+1+nameLen+1)*0x100 + *(sbuf+1+nameLen+2)*0x10000 + *(sbuf+1+nameLen+3)*0x1000000;
	  }
	  
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To open the file specified by the file name.
// INPUT   : fileName
//	     mode (RFU)
// OUTPUT  : none.
// RETURN  : file id (0x8xxxxxxx)
//	     0 = error
// ---------------------------------------------------------------------------
ULONG	api_fs2_open( char *fileName, UCHAR mode )
{
UCHAR	result;
ULONG	fid;
UCHAR	len;


	len = strlen(fileName) + 1;	// ASCII-Z
	
	result = api_fs_open_EX( len, fileName, &fid );
	if( result == apiOK )
	  return( fid );	// file pointer
	else
	  return( 0 );
}

// ---------------------------------------------------------------------------
static	inline	UCHAR	api_fs_read_EX( ULONG fid ,ULONG length, UCHAR *buff, ULONG *rd_bytes )
{
UCHAR	retval = apiFailed;
UCHAR	*sbuf;
ULONG	len;


	sbuf = malloc( 4+4+length+4 );
	if( sbuf )
	  {
	  sbuf[0] =  fid & 0x000000ff;
	  sbuf[1] = (fid & 0x0000ff00) >> 8;
	  sbuf[2] = (fid & 0x00ff0000) >> 16;
	  sbuf[3] = (fid & 0xff000000) >> 24;
	
	  sbuf[4] =  length & 0x000000ff;
	  sbuf[5] = (length & 0x0000ff00) >> 8;
	  sbuf[6] = (length & 0x00ff0000) >> 16;
	  sbuf[7] = (length & 0xff000000) >> 24;
	  
	  IPC_clientHandler( psDEV_FS,8,3,4+4,sbuf,&retval );
	  if( retval == apiOK )
	    {
//	    len = sbuf[8] + sbuf[9]*0x100 + sbuf[10]*0x10000 + sbuf[11]*0x1000000;
//	    memmove( buff, &sbuf[12], len );
	    len = sbuf[4] + sbuf[5]*0x100 + sbuf[6]*0x10000 + sbuf[7]*0x1000000;
	    memmove( buff, &sbuf[8], len );
	    *rd_bytes = len;
	    }
	    
	  free( sbuf );
	  }
	  
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To read file.
// INPUT   : FID
//	     length
// OUTPUT  : buff
// RETURN  : number data you read or 0 (EOF or failed)
// ---------------------------------------------------------------------------
ULONG	api_fs2_read( ULONG fid ,UCHAR *buff, ULONG length )
{
ULONG	rd_bytes = 0;


	api_fs_read_EX( fid ,length, buff, &rd_bytes );
	
	return( rd_bytes );
}

// ---------------------------------------------------------------------------
static	inline	UCHAR	api_fs_write_EX( ULONG fid ,ULONG length, UCHAR *buff, ULONG *wt_bytes )
{
UCHAR	retval = apiFailed;
UCHAR	*sbuf;
ULONG	len;


	sbuf = malloc( 4+4+length+4 );
	if( sbuf )
	  {
	  sbuf[0] =  fid & 0x000000ff;
	  sbuf[1] = (fid & 0x0000ff00) >> 8;
	  sbuf[2] = (fid & 0x00ff0000) >> 16;
	  sbuf[3] = (fid & 0xff000000) >> 24;
	
	  sbuf[4] =  length & 0x000000ff;
	  sbuf[5] = (length & 0x0000ff00) >> 8;
	  sbuf[6] = (length & 0x00ff0000) >> 16;
	  sbuf[7] = (length & 0xff000000) >> 24;
	  
	  memmove( &sbuf[8], buff, length );	// data to write
	  
	  IPC_clientHandler( psDEV_FS,9,3,4+4+length,sbuf,&retval );
	  if( retval == apiOK )
	    {
//	    len = sbuf[8] + sbuf[9]*0x100 + sbuf[10]*0x10000 + sbuf[11]*0x1000000;
	    len = length;
	    *wt_bytes = len;
	    }
	    
	  free( sbuf );
	  }
	  
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To read file.
// INPUT   : FID
//	     length
// OUTPUT  : buff
// RETURN  : number data you write or 0 (EOF or failed)
// ---------------------------------------------------------------------------
ULONG	api_fs2_write( ULONG fid, UCHAR *buff, ULONG length )
{
ULONG	wt_bytes = 0;


	api_fs_write_EX( fid ,length, buff, &wt_bytes );
	
	return( wt_bytes );
}

// ---------------------------------------------------------------------------
// FUNCTION: To close a file.
// INPUT   : FID
// OUTPUT  : none.
// RETURN  : apiOK
//	     apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_fs2_close( ULONG fid )
{
UCHAR	retval;
UCHAR	sbuf[4];


	sbuf[0] =  fid & 0x000000ff;
	sbuf[1] = (fid & 0x0000ff00) >> 8;
	sbuf[2] = (fid & 0x00ff0000) >> 16;
	sbuf[3] = (fid & 0xff000000) >> 24;
	  
	IPC_clientHandler( psDEV_FS,5,1,4,sbuf,&retval );
	
	return( retval );
}

// ---------------------------------------------------------------------------
static	inline	UCHAR	api_fs_create_EX( UCHAR nameLen, char *fileName, unsigned short fileType )
{
UCHAR	retval;
UCHAR	sbuf[FILE_NAME_SIZE+16];
UCHAR	length;


	length = nameLen;
	if( length > (FILE_NAME_SIZE+1) )
	  return( apiFailed );
	
	sbuf[0] = nameLen;			// length of file name
	memmove( &sbuf[1], fileName, length );	// file name
	sbuf[1+length+0] = fileType & 0x00ff;
	sbuf[1+length+1] = (fileType & 0xff00) >> 8;
	
	IPC_clientHandler( psDEV_FS,6,3,1+nameLen+2,sbuf,&retval );
	
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To create a new file.
//	     if the given file name already exists, its contents will be destroyed.
// INPUT   : fineName
//	     fileType (RFU)
// OUTPUT  : none.
// RETURN  : apiOK
//	     apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_fs2_create( char *fileName, unsigned short fileType )
{
UCHAR	result;
ULONG	fid;
UCHAR	len;


	len = strlen(fileName) + 1;	// ASCII-Z
	
	result = api_fs_create_EX( len, fileName, fileType );
	
	return( result );
}

// ---------------------------------------------------------------------------
// FUNCTION: Moves the file pointer to a specified location.
// INPUT   : fid	- file id.
//	     offset	- number of bytes from origin.
//	     origin	- initial position.
//		SEEK_SET (beginning of file)
//		SEEK_END (end of file)
//		SEEK_CUR (current position of file pointer)
// OUTPUT  : none.
// RETURN  : apiOK
//	     apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_fs2_seek( ULONG fid, ULONG offset, int origin )
{
UCHAR	retval;
UCHAR	sbuf[16];


	sbuf[0] =  fid & 0x000000ff;
	sbuf[1] = (fid & 0x0000ff00) >> 8;
	sbuf[2] = (fid & 0x00ff0000) >> 16;
	sbuf[3] = (fid & 0xff000000) >> 24;
	
	sbuf[4] =  offset & 0x000000ff;
	sbuf[5] = (offset & 0x0000ff00) >> 8;
	sbuf[6] = (offset & 0x00ff0000) >> 16;
	sbuf[7] = (offset & 0xff000000) >> 24;
	
	sbuf[8] =   origin & 0x000000ff;
	sbuf[9] =  (origin & 0x0000ff00) >> 8;
	sbuf[10] = (origin & 0x00ff0000) >> 16;
	sbuf[11] = (origin & 0xff000000) >> 24;
	  
	IPC_clientHandler( psDEV_FS,10,3,12,sbuf,&retval );
	
	return( retval );
}

// ---------------------------------------------------------------------------
static	inline	UCHAR	api_fs_tell_EX( ULONG fid, ULONG *position )
{
UCHAR	retval;
UCHAR	sbuf[16];


	sbuf[0] =  fid & 0x000000ff;
	sbuf[1] = (fid & 0x0000ff00) >> 8;
	sbuf[2] = (fid & 0x00ff0000) >> 16;
	sbuf[3] = (fid & 0xff000000) >> 24;
	  
	IPC_clientHandler( psDEV_FS,11,1,4,sbuf,&retval );
	if( retval == apiOK )
	  *position = sbuf[4] + sbuf[5]*0x100 + sbuf[6]*0x10000 + sbuf[7]*0x1000000;
	
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: Gets the current position of a file pointer.
// INPUT   : fid	- file id.
// OUTPUT  : none.
// RETURN  : current position of the file pointer .
//	     -1 on error
// ---------------------------------------------------------------------------
ULONG	api_fs2_tell( ULONG fid )
{
UCHAR	result;
ULONG	position = 0;


	result = api_fs_tell_EX( fid, &position );
	if( result == apiOK )
	  return( position );
	else
	  return( 0xffffffff );	// error
}

// ---------------------------------------------------------------------------
static	inline	UCHAR	api_fs_delete_EX( UCHAR nameLen, char *fileName )
{
UCHAR	retval;
UCHAR	sbuf[FILE_NAME_SIZE+16];
UCHAR	length;


	length = nameLen;
	if( length > (FILE_NAME_SIZE+1) )
	  return( apiFailed );
	
	sbuf[0] = nameLen;			// length of file name
	memmove( &sbuf[1], fileName, length );	// file name
	
	IPC_clientHandler( psDEV_FS,7,2,1+nameLen,sbuf,&retval );
	
	return( retval );
}

// ---------------------------------------------------------------------------
// FUNCTION: To delete a file by name.
// INPUT   : fileName
// OUTPUT  : none.
// RETURN  : apiOK
//	     apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_fs2_delete( char *fileName )
{
UCHAR	result;
ULONG	fid;
UCHAR	len;


	len = strlen(fileName) + 1;	// ASCII-Z
	
	result = api_fs_delete_EX( len, fileName );
	
	return( result );
}

// ---------------------------------------------------------------------------
//	SD Card API
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// FUNCTION: To initialize file system.
// INPUT   : none.
// OUTPUT  : none.
// RETURN  : apiOK
//           apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_sd_init( void )
{
UCHAR	result = apiFailed;


	if( api_fs_select( MEDIA_SD ) == apiOK )
	  result = api_fs_init();
	
	return( result );
}

// ---------------------------------------------------------------------------
// FUNCTION: To open the file specified by the file name.
// INPUT   : fileName
//	     mode (RFU)
// OUTPUT  : none.
// RETURN  : file id (0x8xxxxxxx)
//	     0 = error
// ---------------------------------------------------------------------------
ULONG	api_sd_open( char *fileName, UCHAR mode )
{
	return( api_fs2_open( fileName, mode ) );
}

// ---------------------------------------------------------------------------
// FUNCTION: To close a file.
// INPUT   : FID
// OUTPUT  : none.
// RETURN  : apiOK
//	     apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_sd_close( ULONG fid )
{
	return( api_fs2_close( fid ) );
}

// ---------------------------------------------------------------------------
// FUNCTION: To create a new file.
//	     if the given file name already exists, its contents will be destroyed.
// INPUT   : fineName
//	     fileType (RFU)
// OUTPUT  : none.
// RETURN  : apiOK
//	     apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_sd_create( char *fileName, unsigned short fileType )
{
	return( api_fs2_create( fileName, fileType ) );
}

// ---------------------------------------------------------------------------
// FUNCTION: To read file.
// INPUT   : FID
//	     length
// OUTPUT  : buff
// RETURN  : number data you read or 0 (EOF or failed)
// ---------------------------------------------------------------------------
ULONG	api_sd_read( ULONG fid ,UCHAR *buff, ULONG length )
{
	return( api_fs2_read( fid, buff, length ) );
}

// ---------------------------------------------------------------------------
// FUNCTION: To read file.
// INPUT   : FID
//	     length
// OUTPUT  : buff
// RETURN  : number data you write or 0 (EOF or failed)
// ---------------------------------------------------------------------------
ULONG	api_sd_write( ULONG fid, UCHAR *buff, ULONG length )
{
	return( api_fs2_write( fid, buff, length ) );
}

// ---------------------------------------------------------------------------
// FUNCTION: Moves the file pointer to a specified location.
// INPUT   : fid	- file id.
//	     offset	- number of bytes from origin.
//	     origin	- initial position.
//		SEEK_SET (beginning of file)
//		SEEK_END (end of file)
//		SEEK_CUR (current position of file pointer)
// OUTPUT  : none.
// RETURN  : apiOK
//	     apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_sd_seek( ULONG fid, ULONG offset, int origin )
{
	return( api_fs2_seek( fid, offset, origin ) );
}

// ---------------------------------------------------------------------------
// FUNCTION: Gets the current position of a file pointer.
// INPUT   : fid	- file id.
// OUTPUT  : none.
// RETURN  : current position of the file pointer .
//	     -1 on error
// ---------------------------------------------------------------------------
ULONG	api_sd_tell( ULONG fid )
{
	return( api_fs2_tell( fid ) );
}

// ---------------------------------------------------------------------------
// FUNCTION: To delete a file by name.
// INPUT   : fileName
// OUTPUT  : none.
// RETURN  : apiOK
//	     apiFailed
// ---------------------------------------------------------------------------
UCHAR	api_sd_delete( char *fileName )
{
	return( api_fs2_delete( fileName ) );
}
