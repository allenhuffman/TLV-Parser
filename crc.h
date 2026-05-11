/** @file crc.h
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
#ifndef CRC_H
#define CRC_H

/* System headers */

#include <stddef.h> // size_t
#include <stdint.h> // uint8_t, uint16_t

/* Public function prototypes */
uint16_t crc_calculate(const void * p_buf, size_t buf_size);

uint16_t crc(uint8_t in_byte, uint16_t crc_running);

uint16_t crc_bytes(const uint8_t * p_bytes, size_t length, uint16_t crc_running);

uint16_t crc_string(const char * p_string, uint16_t crc_running);

#endif /* CRC_H */

/*** end of file ***/
