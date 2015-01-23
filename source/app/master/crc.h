#ifndef __CRC_H
#define __CRC_H

typedef unsigned char	uint8;
typedef unsigned short	uint16;
typedef unsigned int	uint32;

uint16 crc16(uint8 *puchMsg, uint16 usDataLen);

#endif