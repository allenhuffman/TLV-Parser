/**
 * @file crc16.c
 *
 * @author Allen C. Huffman
 * @copyright Copyright (c) 2026 Sub-Etha Software
 * @note Origin: https://github.com/allenhuffman
 * @note This file follows the Barr-C Embedded C Coding Standard.
 *
 * @brief CRC calculation functions.
 *
 * @details This module is implemented according to the Barr Group
 * Embedded C Coding Standard (Barr-C).
 *
 * @section history File History
 * - 2026-05-08 allenh - Updated to Barr-C style.
 *
 * @todo None
 */

/* System headers */
#include <stddef.h> // size_t
#include <stdint.h> // uint16_t

/* This module's header (must be first among project headers) */
#include "crc16.h"

/* Public function definitions */

/**
 * @brief Calculate CRC over a buffer.
 *
 * @param[in] p_buf    Pointer to the data buffer.
 * @param[in] buf_size Size of the data buffer.
 *
 * @return The calculated CRC value.
 */
uint16_t
crc16_calculate (CONST void *p_buf, size_t buf_size)
{
    uint16_t crc16 = 0;

    // NULL check.
    if ((NULL != p_buf) && (0 != buf_size))
    {
        // Not NULL, and some data.
        crc16 = crc16_bytes(p_buf, buf_size, 0);
    }

    return crc16;
}

/**
 * @brief CCS .hex file CRC routine, converted to C by "PCM programmer".
 *        Original C# version from:
 * 
 *        https://curioussystem.com/2012/12/11/editing-pic-hex-files/
 *
 *        NOTE: First call, crc_Dbyte must be initialized to 0.
 *
 *        See: https://crccalc.com/ (CRC-16/XMODEM)
 *
 * @param[in] in_byte    Next byte to add to CRC calculation.
 * @param[in] crc_running Current CRC value.
 *
 * @return New CRC value.
 */
uint16_t
crc16 (uint8_t in_byte, uint16_t crc_running)
{
    uint8_t  bit_counter;
    uint16_t shifted;

    shifted = ((uint16_t)in_byte << 8) & 0xFF00;

    for (bit_counter = 0; bit_counter < 8; bit_counter++)
    {
        if (((crc_running ^ shifted) & 0x8000) != 0)
        {
            crc_running <<= 1;
            crc_running ^= 0x1021;
        }
        else
        {
            crc_running <<= 1;
        }

        shifted <<= 1;
    }

    return crc_running;
}



/**
 * @brief Calculate CRC over a range of bytes.
 *
 * @param[in] p_bytes    Pointer to the data bytes.
 * @param[in] length     Length of the data bytes.
 * @param[in] crc_running Current CRC value.
 *
 * @return The new CRC value.
 */
uint16_t
crc16_bytes (CONST uint8_t * p_bytes, size_t length, uint16_t crc_running)
{
    if (NULL != p_bytes)
    {
        size_t byte_counter = 0;

        for (byte_counter = 0; byte_counter < length; byte_counter++)
        {
            crc_running = crc16 (p_bytes[byte_counter], crc_running);
        }
    }

    return crc_running;
}


/**
 * @brief Calculate CRC over a null-terminated string.
 *
 * @param[in] p_string   Pointer to a null-terminated string.
 * @param[in] crc_running Current CRC value.
 *
 * @return The new CRC value.
 */
uint16_t
crc16_string (CONST char * p_string, uint16_t crc_running)
{
    if (NULL != p_string)
    {
        while ('\0' != *p_string)
        {
            crc_running = crc16 ((uint8_t)*p_string, crc_running);
            p_string++;
        }
    }

    return crc_running;
}

/*** end of file ***/
