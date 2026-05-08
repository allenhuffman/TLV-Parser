#ifndef CRC_H
#define CRC_H

/*---------------------------------------------------------------------------*/
// Include Files
/*---------------------------------------------------------------------------*/
// Compiler headers.
#include <stddef.h> // size_t
#include <stdint.h> // uint8_t, uint16_t


/*---------------------------------------------------------------------------*/
// Prototypes
/*---------------------------------------------------------------------------*/
uint16_t crc_calculate (const void *buffer, size_t size);

uint16_t crc (uint8_t inByte, uint16_t crc_Dbyte);

uint16_t crc_bytes (const uint8_t *inBytes, unsigned int length, uint16_t crc_Dbyte);

uint16_t crc_string (const char *inString, uint16_t crc_Dbyte);

#endif // CRC_H

// End of crc.h
