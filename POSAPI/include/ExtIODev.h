#ifndef _EXTIODEV_H_
#define _EXTIODEV_H_
#include "bsp_types.h"
#include "POSAPI.h"

#define	SPI_10MHz	10000000
#define	SPI_8MHz	8000000
// #define	SPI_10MHz	25000000
// #define	SPI_8MHz	25000000
void SPI_Transfer(UINT8 *tx,UINT8 *rx,UINT32 SPEED,UINT8 switchMODE,UINT32 length);

#endif