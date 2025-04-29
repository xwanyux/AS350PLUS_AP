#include "POSAPI.h"
#include "OS_PROCS.h"
#include "IPC_client.h"

/**
 *  this function is used to create the sram file system
 *  @note first create the directory pages in /home/root/
 *        then create the file 0..num_page-1, each page 
 *        file initialize with size PAGE_SIZE with value 0
 *        if file already exist, this function will not change
 *        the existing file value (when the size of each page
 *        remain the same)
 */ 
ULONG api_sram_PageInit( void ){
UCHAR retval;
UCHAR args;
	IPC_clientHandler(psDEV_SRAM,1,0,0,args,&retval);
	return( retval );

}


/**
 *  this function to check the page initialzation success or not
 *  @param[in] Page     page number 
 *  @param[out] pSram   the valid start address and end address of query page
 *  @return error code
 *  @retval apiOK       page initalize success
 *  @retval apiFailed   page initialize failed
 */
ULONG api_sram_PageSelect( UCHAR Page, API_SRAM_ADDR *pSram ){
UCHAR retval;
UCHAR args[1+sizeof(API_SRAM_ADDR)];
	args[0]=Page;
	
	IPC_clientHandler(psDEV_SRAM,2,2,1,args,&retval);
	memmove(pSram,&args[1],sizeof(API_SRAM_ADDR));
	return( retval );

}


/**
 *  this function is used to link page or free page
 *  @param[in] pSram        provide information of start page and end page
 *  @param[in] Action       0/1   link/free
 *  @return error code
 *  @retval apiFailed       link/free failed
 *  @retval apiOK           link/free success
 *  @note when doing linking, range from start page and end page should not be link before
 *        when doing freem the start page and end page should be the same link sturcutre
 */
ULONG api_sram_PageLink( API_SRAM pSram, UCHAR Action ){
UCHAR retval;
UCHAR args[1+sizeof(API_SRAM)];
	
	memmove(args,&pSram,sizeof(API_SRAM));
    args[sizeof(API_SRAM)]=Action;
	IPC_clientHandler(psDEV_SRAM,3,2,1+sizeof(API_SRAM),args,&retval);
	
	return( retval );

}


                
/**
 *  this function is used to read data from page
 *  @param[in] pSram        necessary information for Sram
 *  @param[out] pData       read data from page
 *  @return error code
 *  @retval apiOK           read success
 *  @retval apiFailed       read failed
 *  @note the pSram lengh should be in the range of link page
 *        if length is bigger than the link page , will return apiFailed
 */
ULONG api_sram_PageRead( API_SRAM pSram, UCHAR *pData ){
UCHAR retval;
UCHAR args[sizeof(API_SRAM)+pSram.Len];
	
	memmove(args,&pSram,sizeof(API_SRAM));
	IPC_clientHandler(psDEV_SRAM,4,2,sizeof(API_SRAM),args,&retval);
	memmove(pData,args+sizeof(API_SRAM),pSram.Len);
	return( retval );
}

/**
 *  this function is used to write data to page
 *  @param[in] pSram        necessary information for Sram
 *  @param[in] pData        data be written to page
 *  @return error code
 *  @retval apiOK           write success
 *  @retval apiFailed       write failed
 *  @note the pSram lengh should be in the range of link page
 *        if length is bigger than the link page , will return apiFailed
 */

ULONG api_sram_PageWrite( API_SRAM pSram, UCHAR *pData ){
UCHAR retval;
UCHAR args[sizeof(API_SRAM)+pSram.Len];
	
	memmove(args,&pSram,sizeof(API_SRAM));
	memmove(&args[sizeof(API_SRAM)],pData,pSram.Len);
	IPC_clientHandler(psDEV_SRAM,5,2,sizeof(API_SRAM)+pSram.Len,args,&retval);
	return( retval );
}

/**
 *  this function is used to clear the page with some value
 *  @param[in] pSram        necessary information for sram 
 *  @param[in] Pattern      the clear value
 *  @return error code
 *  @retval apiOK           clear success
 *  @retval apiFailed       clear failed
 *  @note pSram.Len is 0, the clear function will clear all the page be link before
 *        pSram.Len is not 0, will only clean the length.
 *        if clean lengh is bigger than the link page, api will reutrn apiFailed
 */ 
ULONG api_sram_PageClear( API_SRAM pSram, UCHAR Pattern ){
UCHAR retval;
UCHAR args[sizeof(API_SRAM)+1];
	
	memmove(args,&pSram,sizeof(API_SRAM));
	args[sizeof(API_SRAM)]=Pattern;
	IPC_clientHandler(psDEV_SRAM,6,2,sizeof(API_SRAM),args,&retval);
	return( retval );
}


