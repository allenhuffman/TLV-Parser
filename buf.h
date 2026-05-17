/** @file buf.h
 *
 * @author Allen C. Huffman
 * @copyright Copyright (c) 2026 Sub-Etha Software
 * @note Origin: https://github.com/allenhuffman
 * @note This file follows the Barr-C Embedded C Coding Standard.
 *
 * @brief Public interface for the buf module.
 *
 * @details This module is implemented according to the Barr Group
 * Embedded C Coding Standard (Barr-C).
 *
 * @section history File History
 * - 2026-05-08 allenh - Updated to Barr-C style.
 */
#ifndef BUF_H
#define BUF_H

/*---------------------------------------------------------------------------*/
// System headers
/*---------------------------------------------------------------------------*/
#include <stdint.h>

/*---------------------------------------------------------------------------*/
// Public macros: all #define items, constants and function-like macros
/*---------------------------------------------------------------------------*/

// CCS PCD compiler for PIC24 handles const differently that standard C
// compilers. For environments like that, this macro can be modified to
// remove the use of const in this module.
#ifndef CONST
    #define CONST const
    //#define CONST
#endif

/*---------------------------------------------------------------------------*/
// Public function prototypes
/*---------------------------------------------------------------------------*/
void     buf_read_data(CONST uint8_t ** pp_read, void *p_data, unsigned int data_size);
uint8_t  buf_read_u8(CONST uint8_t ** pp_read);
uint16_t buf_read_u16(CONST uint8_t ** pp_read);
uint32_t buf_read_u32(CONST uint8_t ** pp_read);
float    buf_read_float(CONST uint8_t ** pp_read);
double   buf_read_double(CONST uint8_t ** pp_read);

void buf_write_data(uint8_t ** pp_write, CONST void *p_data, unsigned int data_size);
void buf_write_u8(uint8_t ** pp_write, uint8_t value);
void buf_write_u16(uint8_t ** pp_write, uint16_t value);
void buf_write_u32(uint8_t ** pp_write, uint32_t value);
void buf_write_float(uint8_t ** pp_write, float value);
void buf_write_double(uint8_t ** pp_write, double value);

#endif /* BUF_H */

/*** end of file ***/
