/*---------------------------------------------------------------------------*/
// Includes
/*---------------------------------------------------------------------------*/
// Compiler headers.
#include <stddef.h> // size_t
#include <stdint.h> // uint16_t

// This file's header.
#include "crc.h"

/*---------------------------------------------------------------------------*/
// Functions
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
// Calculate CRC over a range of bytes.
/*---------------------------------------------------------------------------*/
uint16_t crc_calculate (const void *buffer, size_t size)
{
    uint16_t crc = 0;

    // NULL check.
    if ((NULL != buffer) && (0 != size))
    {
        // Not NULL, and some data.
        crc = crc_bytes (buffer, size, 0);
    }

    return crc;
}


/*---------------------------------------------------------------------------*/
// @brief CCS .hex file CRC routine, converted to C by "PCM programmer".
//        Original C# version from:
//        https://curioussystem.com/2012/12/11/editing-pic-hex-files/
//
//        NOTE: First call, crc_Dbyte must be initialized to 0.
//
//        See: https://crccalc.com/ (CRC-16/XMODEM)
//
// @param inByte - next byte to add to CRC calculation.
// @param crc_Dbyte - current CRC value.
//
// @return  uint16_t - new CRC value.
/*---------------------------------------------------------------------------*/
uint16_t crc (uint8_t inByte, uint16_t crc_Dbyte)
{
    uint8_t bit_counter;
    uint16_t c;

    // DEBUG_PRINTF ("%02x ", inByte);

    c = ((uint16_t)inByte << 8) & 0xFF00;

    for (bit_counter = 0; bit_counter < 8; bit_counter++)
    {
        if (((crc_Dbyte ^ c) & 0x8000) != 0)
        {
            crc_Dbyte <<= 1;
            crc_Dbyte ^= 0x1021;
        }
        else
        {
            crc_Dbyte <<= 1;
        }

        c <<= 1;
    }

    return crc_Dbyte;
}

/*---------------------------------------------------------------------------*/
// @brief  Calculate CRC over a range of bytes.
//
// @param  inBytes - pointer to data bytes.
// @param  length - length of the data bytes.
// @param  crc_Dbyte - current CRC value.
//
// @return uint16_t - new CRC value.
/*---------------------------------------------------------------------------*/
uint16_t crc_bytes (const uint8_t *inBytes, size_t length, uint16_t crc_Dbyte)
{
    if (inBytes != NULL)
    {
        size_t byte_counter = 0;

        for (byte_counter = 0; byte_counter < length; byte_counter++)
        {
            crc_Dbyte = crc (inBytes[byte_counter], crc_Dbyte);
        }
    }

    return crc_Dbyte;
}

/*---------------------------------------------------------------------------*/
// @brief  Calculate CRC over a null-terminated string.
//
// @param  inString - pointer to a null-terminated string.
// @param  crc_Dbyte - current CRC value.
//
// @return uint16_t - new CRC value.
/*---------------------------------------------------------------------------*/
uint16_t crc_string (const char *inString, uint16_t crc_Dbyte)
{
    if (inString != NULL)
    {
        while (*inString != '\0')
        {
            crc_Dbyte = crc ((uint8_t)*inString, crc_Dbyte);
            inString++;
        }
    }

    return crc_Dbyte;
}

// End of crc.c
