/*---------------------------------------------------------------------------*/
// Sub-Etha Software's Get/Put Data routines.
// By Allen C. Huffman
// www.subethasoftware.com
// https://github.com/allenhuffman/GetPutData/
//
// This is a simple set of functions to read and write various data types to
// and from buffers.
//
// FILE HISTORY:
//
// 2026-05-08 allenh - Added this header.
//
// TODO:
//
// TOFIX:
//
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
// Include Files
/*---------------------------------------------------------------------------*/
// Compiler headers.
#include <stdint.h>
#include <string.h> // for memset(), memcpy()

// This file's header.
#include "get_put_values.h"


/*---------------------------------------------------------------------------*/
// GET Functions
/*---------------------------------------------------------------------------*/

void get_data (const uint8_t **ptr, void *data_ptr, unsigned int data_size)
{
    memcpy (data_ptr, *ptr, data_size);
    *ptr += data_size;
}

uint8_t get_u8 (const uint8_t **ptr)
{
    uint8_t value;
    memcpy (&value, *ptr, sizeof(value));
    *ptr += sizeof (value);
    return value;
}

uint16_t get_u16 (const uint8_t **ptr)
{
    uint16_t value;
    memcpy (&value, *ptr, sizeof (value));
    *ptr += sizeof (value);
    return value;
}

uint32_t get_u32 (const uint8_t **ptr)
{
    uint32_t value;
    memcpy (&value, *ptr, sizeof (value));
    *ptr += sizeof (value);
    return value;
}

float get_float (const uint8_t **ptr)
{
    float value;
    memcpy (&value, *ptr, sizeof (value));
    *ptr += sizeof (value);
    return value;
}

double get_double (const uint8_t **ptr)
{
    double value;
    memcpy (&value, *ptr, sizeof (value));
    *ptr += sizeof (value);
    return value;
}


/*---------------------------------------------------------------------------*/
// PUT Functions
/*---------------------------------------------------------------------------*/

void put_data (uint8_t **ptr, const void *data_ptr, unsigned int data_size)
{
    memcpy (*ptr, data_ptr, data_size);
    *ptr += data_size;
}

void put_u8 (uint8_t **ptr, uint8_t value)
{
    memcpy (*ptr, &value, sizeof (value));
    *ptr += sizeof (value);
}

void put_u16 (uint8_t **ptr, uint16_t value)
{
    memcpy (*ptr, &value, sizeof (value));
    *ptr += sizeof (value);
}

void put_u32 (uint8_t **ptr, uint32_t value)
{
    memcpy (*ptr, &value, sizeof (value));
    *ptr += sizeof (value);
}

void put_float (uint8_t **ptr, float value)
{
    memcpy (*ptr, &value, sizeof (value));
    *ptr += sizeof (value);
}

void put_double (uint8_t **ptr, double value)
{
    memcpy (*ptr, &value, sizeof (value));
    *ptr += sizeof (value);
}

// End of get_put_values.c
