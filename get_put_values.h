/** @file get_put_values.h
 *
 * @author Allen C. Huffman
 * @copyright Copyright (c) 2026 Sub-Etha Software
 * @note Origin: https://github.com/allenhuffman
 * @note This file follows the Barr-C Embedded C Coding Standard.
 *
 * @brief Public interface for the get_put_values module.
 *
 * @details This module is implemented according to the Barr Group
 * Embedded C Coding Standard (Barr-C).
 *
 * @section history File History
 * - 2026-05-08 allenh - Updated to Barr-C style.
 */
#ifndef GET_PUT_VALUES_H
#define GET_PUT_VALUES_H

/* System headers */
#include <stdint.h>

/* Public function prototypes */
void     get_data(const uint8_t ** pp_read, void *p_data, unsigned int data_size);
uint8_t  get_u8(const uint8_t ** pp_read);
uint16_t get_u16(const uint8_t ** pp_read);
uint32_t get_u32(const uint8_t ** pp_read);
float    get_float(const uint8_t ** pp_read);
double   get_double(const uint8_t ** pp_read);

void put_data(uint8_t ** pp_write, const void *p_data, unsigned int data_size);
void put_u8(uint8_t ** pp_write, uint8_t value);
void put_u16(uint8_t ** pp_write, uint16_t value);
void put_u32(uint8_t ** pp_write, uint32_t value);
void put_float(uint8_t ** pp_write, float value);
void put_double(uint8_t ** pp_write, double value);

#endif /* GET_PUT_VALUES_H */

/*** end of file ***/
