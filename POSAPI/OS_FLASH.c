





#include "POSAPI.h"
#include "OS_PROCS.h"
#include "IPC_client.h"


/**
 *    this function is used to write data to flash by address
 *    @param[in] BaseAddr           the start address to be write
 *    @param[in] pData              the data to be write
 *    @param[in] Len                the length of data
 *    @return error code
 *    @retval apiOK                 write success
 *    @retval apiFailed             write failed
 */ 
UINT32	FLASH_WriteData( void *BaseAddr, void *pData, UINT32 Len ){
UCHAR retval;
ULONG address=(ULONG)BaseAddr;
UCHAR sbuf[sizeof(ULONG)*2+Len];
    memmove(sbuf,&address,sizeof(ULONG));
    memmove(&sbuf[sizeof(ULONG)],&Len,sizeof(ULONG));
    memmove(&sbuf[sizeof(ULONG)*2],pData,Len);
    IPC_clientHandler(psDEV_FLS,1,3,sizeof(ULONG)*2+Len,sbuf,&retval);
    return( retval );

}

/**
 *    this function is used to read data to flash by address
 *    @param[in] BaseAddr           the start address to be read
 *    @param[out] pData             the data to be read
 *    @param[in] Len                the length of data want to read
 */ 
void FLASH_ReadData( void *BaseAddr, void *pData, UINT32 Len ){
UCHAR retval;
ULONG address=(ULONG)BaseAddr;
UCHAR sbuf[sizeof(ULONG)*2+Len];
    memmove(sbuf,&address,sizeof(ULONG));
    memmove(&sbuf[sizeof(ULONG)],&Len,sizeof(ULONG));    
    IPC_clientHandler(psDEV_FLS,2,3,sizeof(ULONG)*2,sbuf,&retval);
    memmove(pData,&sbuf[sizeof(ULONG)*2],Len);
    return( retval );
}

/**
 *    this function is used to write data to flash by address
 *    @param[in] addr               the start address to be write
 *    @param[in] len                the length of data
 *    @param[in] data               the data to be write
 *    @return error code
 *    @retval apiOK                 write success
 *    @retval apiFailed             write failed
 */ 
UINT32 OS_FLS_PutData( UINT32 addr, UINT32 len, UINT8 *data ){

    return FLASH_WriteData( addr, data, len );
}

/**
 *    this function is used to write data to flash by address
 *    @param[in] addr               the start address to be read
 *    @param[in] len                the length of data
 *    @param[out] data              the data to be read
 */ 
void OS_FLS_GetData( UINT32 addr, UINT32 len, UINT8 *data ){

    FLASH_ReadData( addr, data, len );

}

/**
 *    this function is used to clear the memory to 0 
 *     from Sectorbase of size FLASG_PAGE_SECTOR_SIZE
 *    @param[in] SectorBase           the start address to be clear
 *    @return error code
 *    @retval apiOK                    clear success
 *    @retval apiFailed                clear failed
 */ 
UINT32 FLASH_EraseSector( UINT32 SectorBase ) {
UCHAR retval;
    IPC_clientHandler(psDEV_FLS,3,1,4,&SectorBase,&retval);
    return( retval );
}