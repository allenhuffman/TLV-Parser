/**
 * @file get_put_values.c
 *
 * @author Allen C. Huffman
 * @copyright Copyright (c) 2026 Sub-Etha Software
 * @note Origin: https://github.com/allenhuffman
 * @note This file follows the Barr-C Embedded C Coding Standard.
 *
 * @brief Read/write values to/from a packed buffer.
 *
 * @details This module is implemented according to the Barr Group
 * Embedded C Coding Standard (Barr-C).
 *
 * @section history File History
 * - 2026-05-08 allenh - Updated to Barr-C style.
 *
 * @todo Possibly have put return count of how many bytes were written.
 */

/* System headers */
#include <stdint.h>
#include <string.h> // for memset(), memcpy()

/* This module's header (must be first among project headers) */
#include "get_put_values.h"

/* Public function definitions */

/**
 * @brief Read data from a buffer and advance the pointer.
 *
 * @param[in] pp_read   Pointer to the buffer pointer.
 * @param[out] p_data   Pointer to the variable where the data will be stored.
 * @param[in] data_size Size of the buffer where variable will be stored.
 */
void
get_data (const uint8_t **pp_read, void *p_data, unsigned int data_size)
{
    memcpy (p_data, *pp_read, data_size);
    *pp_read += data_size;
}


/**
 * @brief Read data from a buffer and advance the pointer.
 *
 * @param[in] pp_read Pointer to the buffer pointer.
 *
 * @return The value read from the buffer.
 */
uint8_t
get_u8 (const uint8_t **pp_read)
{
    uint8_t value;
    memcpy (&value, *pp_read, sizeof (value));
    *pp_read += sizeof (value);
    return value;
}

/**
 * @brief Read data from a buffer and advance the pointer.
 *
 * @param[in] pp_read Pointer to the buffer pointer.
 *
 * @return The value read from the buffer.
 */
uint16_t
get_u16 (const uint8_t **pp_read)
{
    uint16_t value;
    memcpy (&value, *pp_read, sizeof (value));
    *pp_read += sizeof (value);
    return value;
}


/**
 * @brief Read data from a buffer and advance the pointer.
 *
 * @param[in] pp_read Pointer to the buffer pointer.
 *
 * @return The value read from the buffer.
 */
uint32_t
get_u32 (const uint8_t **pp_read)
{
    uint32_t value;
    memcpy (&value, *pp_read, sizeof (value));
    *pp_read += sizeof (value);
    return value;
}


/**
 * @brief Read data from a buffer and advance the pointer.
 *
 * @param[in] pp_read Pointer to the buffer pointer.
 *
 * @return The value read from the buffer.
 */
float
get_float (const uint8_t **pp_read)
{
    float value;
    memcpy (&value, *pp_read, sizeof (value));
    *pp_read += sizeof (value);
    return value;
}


/**
 * @brief Read data from a buffer and advance the pointer.
 *
 * @param[in] pp_read Pointer to the buffer pointer.
 *
 * @return The value read from the buffer.
 */
double
get_double (const uint8_t **pp_read)
{
    double value;
    memcpy (&value, *pp_read, sizeof (value));
    *pp_read += sizeof (value);
    return value;
}


/**
 * @brief Write data to a buffer and advance the pointer.
 *
 * @param[in] pp_write Pointer to the buffer pointer.
 * @param[in] p_data Pointer to the variable containing the data to write.
 * @param[in] data_size Size of the data to write.
 */
void
put_data (uint8_t ** pp_write, const void *p_data, unsigned int data_size)
{
    memcpy (*pp_write, p_data, data_size);
    *pp_write += data_size;
}

/**
 * @brief Write data to a buffer and advance the pointer.
 *
 * @param[in] pp_write Pointer to the buffer pointer.
 * @param[in] value The value to write to the buffer.
 * 
 * @return None.
 */
void
put_u8 (uint8_t **pp_write, uint8_t value)
{
    memcpy (*pp_write, &value, sizeof (value));
    *pp_write += sizeof (value);
}


/**
 * @brief Write data to a buffer and advance the pointer.
 *
 * @param[in] pp_write Pointer to the buffer pointer.
 * @param[in] value The value to write to the buffer.
 *
 * @return None.
 */
void
put_u16 (uint8_t **pp_write, uint16_t value)
{
    memcpy (*pp_write, &value, sizeof (value));
    *pp_write += sizeof (value);
}


/**
 * @brief Write data to a buffer and advance the pointer.
 *
 * @param[in] pp_write Pointer to the buffer pointer.
 * @param[in] value The value to write to the buffer.
 *
 * @return None.
 */
void
put_u32 (uint8_t **pp_write, uint32_t value)
{
    memcpy (*pp_write, &value, sizeof (value));
    *pp_write += sizeof (value);
}


/**
 * @brief Write data to a buffer and advance the pointer.
 *
 * @param[in] pp_write Pointer to the buffer pointer.
 * @param[in] value The value to write to the buffer.
 *
 * @return None.
 */
void
put_float (uint8_t **pp_write, float value)
{
    memcpy (*pp_write, &value, sizeof (value));
    *pp_write += sizeof (value);
}


/**
 * @brief Write data to a buffer and advance the pointer.
 *
 * @param[in] pp_write Pointer to the buffer pointer.
 * @param[in] value The value to write to the buffer.
 *
 * @return None.
 */
void
put_double (uint8_t **pp_write, double value)
{
    memcpy (*pp_write, &value, sizeof (value));
    *pp_write += sizeof (value);
}

/*** end of file ***/
