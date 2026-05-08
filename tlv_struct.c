/*---------------------------------------------------------------------------*/
// Includes
/*---------------------------------------------------------------------------*/
// Compiler headers.
#include <inttypes.h> // uintptr_t
#include <stdbool.h>  // for bool/true/false
#include <stddef.h>   // for NULL
#include <stdio.h>    // for printf
#include <string.h>   // for memcpy ()

// This file's header.
#include "tlv_struct.h"

// Other headers.
#include "crc.h"
#include "get_put_values.h"

// 0 = No debug messages, 1 = Debug messages, 2 = Even more debug messages.
#define DEBUG_TLV 0

#if (DEBUG_TLV > 0)
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#endif


/*---------------------------------------------------------------------------*/
// Read and parse TLV data from a buffer and copy into a structure.
/*---------------------------------------------------------------------------*/
size_t tlv_parse_struct (const void *data_ptr, unsigned int data_size,
                         void *struct_ptr,
                         const tlv_offset_struct_t *tlv_table_ptr)
{
    size_t          bytes_consumed = 0;
    unsigned int    type = 0;
    unsigned int    length = 0;
    const uint8_t   *end_ptr = NULL;
    const uint8_t   *ptr = NULL;
    unsigned int    table_entry = 0;
    bool            keep_scanning = true;

#if (DEBUG_TLV > 1)
     DEBUG_PRINTF ("tlv_parse_struct (0x%x, %u, 0x%x, 0x%x)\r\n",
                   (unsigned int)((uintptr_t)data_ptr),
                   data_size,
                   (unsigned int)((uintptr_t)struct_ptr),
                   (unsigned int)(uintptr_t)tlv_table_ptr);
#endif

    if ((NULL != data_ptr) && (NULL != struct_ptr) && (NULL != tlv_table_ptr))
    {
        ptr = (const uint8_t *)data_ptr;
        end_ptr = (const uint8_t *)data_ptr + data_size;

        while ((ptr + 2) <= end_ptr)
        {
            type = get_u8(&ptr);
            length = get_u8(&ptr);

            if ((0 == type) && (0 == length))
            {
                if ((ptr + sizeof(uint16_t)) <= end_ptr)
                {
                    uint16_t calculated_crc = crc_calculate (data_ptr,
                                           (size_t)(ptr - (const uint8_t *)data_ptr));
                    uint16_t crc = get_u16(&ptr);

                    if (crc == calculated_crc)
                    {
                        bytes_consumed = (size_t)(ptr - (const uint8_t *)data_ptr);
                    }
                    else
                    {
#if (DEBUG_TLV > 0)
                        DEBUG_PRINTF ("Bad CRC - Expected 0x%04x, read 0x%04x\r\n",
                                      calculated_crc, crc);
#endif
                    }
                }

                break;
            }

            if ((ptr + length) > end_ptr)
            {
                bytes_consumed = 0;
                break;
            }

            ptr += length;
        }

        if (bytes_consumed != 0)
        {
            ptr = (const uint8_t *)data_ptr;
            end_ptr = (const uint8_t *)data_ptr + data_size;
            keep_scanning = true;

            while ((ptr + 2) <= end_ptr)
            {
#if (DEBUG_TLV > 0)
                bool type_found = false;
#endif
                type = get_u8(&ptr);
                length = get_u8(&ptr);

                if ((0 == type) && (0 == length))
                {
                    break;
                }

                if ((ptr + length) > end_ptr)
                {
#if (DEBUG_TLV > 0)
                    DEBUG_PRINTF ("TLVParseStruct: Type %u length %u exceeds buffer.\n",
                                  type, length);
#endif
                    bytes_consumed = 0;
                    break;
                }

                table_entry = 0;
                while (((tlv_table_ptr[table_entry].type != 0) ||
                        (tlv_table_ptr[table_entry].length != 0) ||
                        (tlv_table_ptr[table_entry].offset != 0)) &&
                       (keep_scanning == true))
                {
#if (DEBUG_TLV > 0)
                    type_found = false;
#endif
                    if (tlv_table_ptr[table_entry].type == type)
                    {
                        if (tlv_table_ptr[table_entry].length == length)
                        {
                            memcpy ((uint8_t *)struct_ptr + tlv_table_ptr[table_entry].offset,
                                    ptr,
                                    tlv_table_ptr[table_entry].length);
#if (DEBUG_TLV > 0)
                            type_found = true;
#endif
                        }
                        else
                        {
#if (DEBUG_TLV > 0)
                            DEBUG_PRINTF ("T:%u L:%u is wrong (expected L:%u). Skipping rest of this section.\r\n",
                                          type, length, tlv_table_ptr[table_entry].length);
#endif
                            keep_scanning = false;
                            bytes_consumed = 0;
                        }

                        break;
                    }

                    table_entry++;
                }

#if (DEBUG_TLV > 0)
                if ((keep_scanning == true) && (type_found != true))
                {
                    DEBUG_PRINTF ("Skipping unknown type %3u (%u bytes).\r\n", type, length);
                }
#endif
                if (bytes_consumed == 0)
                {
                    break;
                }

                ptr += length;
            }
        }
    }

    return bytes_consumed;
}


/*---------------------------------------------------------------------------*/
// Write elements of a structure in the TLV structure to a TLV buffer.
/*---------------------------------------------------------------------------*/
size_t tlv_write_struct (void *dest_ptr, unsigned int dest_size,
                         const void *struct_ptr,
                         const tlv_offset_struct_t *tlv_table_ptr)
{
    size_t bytes_written = 0;
    uint8_t *end_ptr = NULL;

#if (DEBUG_TLV > 1)
    DEBUG_PRINTF ("tlv_write_struct (0x%x, %u, 0x%x, 0x%x)\r\n",
                  (unsigned int)(uintptr_t)dest_ptr,
                  dest_size,
                  (unsigned int)(uintptr_t)struct_ptr,
                  (unsigned int)(uintptr_t)tlv_table_ptr);
#endif

    if ((NULL != dest_ptr) && (NULL != struct_ptr) && (NULL != tlv_table_ptr))
    {
        unsigned int table_entry = 0;
        uint8_t *ptr = NULL;

        ptr = (uint8_t *)dest_ptr;
        end_ptr = (uint8_t *)dest_ptr + dest_size;

        while ((tlv_table_ptr[table_entry].offset != 0) ||
               (tlv_table_ptr[table_entry].type != 0) ||
               (tlv_table_ptr[table_entry].length != 0))
        {
            size_t entry_size = sizeof(uint8_t) + sizeof(uint8_t) +
                                tlv_table_ptr[table_entry].length;

#if (DEBUG_TLV > 0)
            DEBUG_PRINTF ("%u. %u,%u\r\n", table_entry,
                          tlv_table_ptr[table_entry].type,
                          tlv_table_ptr[table_entry].length);
#endif

            if ((ptr + entry_size) > end_ptr)
            {
#if (DEBUG_TLV > 0)
                DEBUG_PRINTF ("tlv_write_struct: EEPROM area too small to fit all.\n");
#endif
                bytes_written = 0;
                ptr = NULL;
                break;
            }

            put_u8 (&ptr, tlv_table_ptr[table_entry].type);
            put_u8 (&ptr, tlv_table_ptr[table_entry].length);
            put_data (&ptr,
                      (void *)((const uint8_t *)struct_ptr + tlv_table_ptr[table_entry].offset),
                      tlv_table_ptr[table_entry].length);

            table_entry++;
        }

        if (ptr != NULL)
        {
            if ((ptr + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint16_t)) <= end_ptr)
            {
                put_u8 (&ptr, 0);
                put_u8 (&ptr, 0);

                {
                    uint16_t crc = crc_calculate (dest_ptr,
                                                  (size_t)(ptr - (uint8_t *)dest_ptr));
                    put_u16 (&ptr, crc);
                }

                bytes_written = (size_t)(ptr - (uint8_t *)dest_ptr);
            }
            else
            {
#if (DEBUG_TLV > 0)
                DEBUG_PRINTF ("tlv_write_struct: No room for terminator and/or CRC.\n");
#endif
                bytes_written = 0;
            }
        }
    }

    return bytes_written;
}

// End of tlv_struct.c
