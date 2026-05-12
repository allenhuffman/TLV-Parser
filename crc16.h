/** @file crc16.h
 *
 * @author Allen C. Huffman
 * @copyright Copyright (c) 2026 Sub-Etha Software
 * @note Origin: https://github.com/allenhuffman
 * @note This file follows the Barr-C Embedded C Coding Standard.
 *
 * @brief Public interface for the CRC module.
 *
 * @details This module is implemented according to the Barr Group
 * Embedded C Coding Standard (Barr-C).
 *
 * @section history File History
 * - 2026-05-08 allenh - Updated to Barr-C style.
 */
#ifndef CRC16_H
#define CRC16_H

/* System headers */

#include <stddef.h> // size_t
#include <stdint.h> // uint8_t, uint16_t

/* Public macros: all #define items, constants and function-like macros */
// CCS PCD compiler for PIC24 handles const differently that standard C
// compilers. For environments like that, this macro can be modified to
// remove the use of const in this module.
#ifndef CONST
    #define CONST const
    //#define CONST
#endif

/* Public function prototypes */
uint16_t crc16_calculate(CONST void * p_buf, size_t buf_size);

uint16_t crc16(uint8_t in_byte, uint16_t crc_running);

uint16_t crc16_bytes(CONST uint8_t * p_bytes, size_t length, uint16_t crc_running);

uint16_t crc16_string(CONST char * p_string, uint16_t crc_running);

#endif /* CRC16_H */

/*** end of file ***/
