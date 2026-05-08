#ifndef CRC_H
#define CRC_H

// 0 = No debug messages, 1 = Debug messages, 2 = Even more debug messages.
#define DEBUG_CRC 0

/*---------------------------------------------------------------------------*/
// Includes
/*---------------------------------------------------------------------------*/
#include <stddef.h> // size_t


/*---------------------------------------------------------------------------*/
// Prototypes
/*---------------------------------------------------------------------------*/
uint16_t crc_calculate (const void *buffer, size_t size);

uint16_t crc (uint8_t inByte, uint16_t crc_Dbyte);

uint16_t crc_bytes (const uint8_t *inBytes, unsigned int length, uint16_t crc_Dbyte);

uint16_t crc_string (const char *inString, uint16_t crc_Dbyte);

#endif // CRC_H

// End of crc.h
