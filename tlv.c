/*---------------------------------------------------------------------------*/
// Includes
/*---------------------------------------------------------------------------*/
#include <inttypes.h> // uintptr_t
#include <stdbool.h>  // for bool/true/false
#include <stddef.h>   // for NULL
#include <stdio.h>    // for printf
#include <string.h>   // for memcpy ()

#include "tlv.h"

#include "crc.h"
#include "get_put_values.h"

#if (DEBUG_TLV > 0)
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#endif


/*---------------------------------------------------------------------------*/
// Prototypes
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
// Read and parse TLV data from a buffer and copy into variables.
/*---------------------------------------------------------------------------*/
size_t tlv_parse_ptr (void *data_ptr, unsigned int data_size,
                       tlv_ptr_struct_t *tlv_table_ptr)
{
    size_t          bytes_consumed = 0;
    unsigned int    type = 0;
    unsigned int    length = 0;
    uint8_t         *end_ptr = NULL;
    uint8_t         *ptr = NULL;
    unsigned int    table_entry = 0;
    bool            keep_scanning = true;

#if (DEBUG_TLV > 1)
    DEBUG_PRINTF ("tlv_parse_ptr (0x%x, %u, 0x%x)\r\n",
                  (unsigned int)((uintptr_t)data_ptr),
                  data_size, (unsigned int)(uintptr_t)tlv_table_ptr);
#endif

    if ((NULL != data_ptr) && (NULL != tlv_table_ptr))
    {
        // To avoid parsing corrupt data, we first need to scan and find the
        // end of the TLV data and then check the CRC.

        ptr = (uint8_t *)data_ptr; // Start of data.
        end_ptr = (uint8_t *)data_ptr + data_size;

        while ((ptr + 2) <= end_ptr) // Room for TL header.
        {
            type = get_u8(&ptr);
            length = get_u8(&ptr);

            if ((0 == type) && (0 == length)) // End of TLV data found.
            {
                if ((ptr + sizeof(uint16_t)) > end_ptr)
                {
                    bytes_consumed = 0;
                }
                else
                {
                    // Calculate CRC over the used memory.
                    uint16_t calculatedCrc = crc_calculate (data_ptr,
                                           (size_t)(ptr - (uint8_t *)data_ptr));

                    // Get the 16-bit CRC stored after the used memory.
                    uint16_t crc = get_u16(&ptr);

                    if (crc == calculatedCrc) // Verify CRC.
                    {
                        bytes_consumed = (size_t)(ptr - (uint8_t *)data_ptr);
                    }
                    else
                    {
#if (DEBUG_TLV > 0)
                        DEBUG_PRINTF ("Bad CRC - Expected 0x%04x, read 0x%04x\r\n",
                                      crc, calculatedCrc);
#endif
                        bytes_consumed = 0;
                    }
                }

                break;
            }

            if ((ptr + length) > end_ptr)
            {
                bytes_consumed = 0;

                break;
            }

            // Skip the data.
            ptr += length;
        }

        if (0 != bytes_consumed)
        {
            // Reset so we can scan and parse.
            ptr = (uint8_t *)data_ptr; // Start of data.
            end_ptr = (uint8_t *)data_ptr + data_size;
            keep_scanning = true;

            while ((ptr + 2) <= end_ptr) // Room for TL header.
            {
#if (DEBUG_TLV > 0)
                bool type_found = false;
#endif
                type = get_u8(&ptr);
                length = get_u8(&ptr);

                // Stop only on an explicit 00 00 terminator.
                if ((0 == type) && (0 == length))
                {
                    break;
                }

                if ((ptr + length) > end_ptr)
                {
#if (DEBUG_TLV > 0)
                    DEBUG_PRINTF ("tlv_parse_ptr: Type %u length %u exceeds buffer.\n",
                                  type, length);
#endif
                    bytes_consumed = 0;
                    break;
                }

                // See if this type appears in the table.
                table_entry = 0;
                while ((tlv_table_ptr[table_entry].value_ptr != NULL) &&
                       (true == keep_scanning))
                {
#if (DEBUG_TLV > 0)
                    type_found = false;
#endif
                    if (tlv_table_ptr[table_entry].type == type)
                    {
                    // TODO: This implementation will NOT let you copy between different
                    // sized elements. i.e., if the EEPROM Config value is an 8-bit value,
                    // and you try to copy it to a 32-bit value, it copies the first byte
                    // over making a messed up variable.  Extra code will be added to
                    // detect this and translate the values (i.e., copy to an 8-bit, then
                    // assign that to the 32-bit destination).

                    // Verify that the EEPROM value size matches the expected
                    // size of our variable. This will help catch bad data.
                        if (tlv_table_ptr[table_entry].length == length)
                        {
                            memcpy(tlv_table_ptr[table_entry].value_ptr, ptr, tlv_table_ptr[table_entry].length);
#if (DEBUG_TLV > 0)
                            // DEBUG_PRINTF ("%u (%u)\r\n", tlv_table_ptr[table_entry].type, tlv_table_ptr[table_entry].length);
                            type_found = true;
#endif
                        }
                        else
                        {
#if (DEBUG_TLV > 0)
                            DEBUG_PRINTF ("tlv_parse_ptr: T:%u L:%u is wrong (expected L:%u). Skipping rest of this section.\r\n",
                                          type, length, tlv_table_ptr[table_entry].length);
#endif
                            // In the event bad data is ever written with a good
                            // CRC, we also make sure we have a known type with
                            // its expected length. If not, parse no further.
                            // Something is clearly wrong.

                            // EEPROM TLV length does not match our variable size,
                            // so there must be something wrong with this data.
                            // Scan no further, to prevent parsing bad data.
                            keep_scanning = false;

                            bytes_consumed = 0; // Flag the data bad.
                        }

                        break;
                    }

                    table_entry++;
                } // end of while (TLVTablePtr[table_entry].valuePtr != NULL)

#if (DEBUG_TLV > 0)
                if ((true == keep_scanning) && (true != type_found))
                {
                    DEBUG_PRINTF ("Skipping unknown type %3u (%u bytes).\r\n", type, length);
                }
#endif
                // If parsing found a type we expect, but the length
                // was not what we expect, consider any data from this
                // point forward to be bad and do not parse it.
                if (0 == bytes_consumed)
                {
                    break;
                }

                ptr += length;
            } // end of while (ptr < (uint8_t*)dataPtr+dataSize)
        }
    }

    return bytes_consumed;
}


/*---------------------------------------------------------------------------*/
// Read and parse TLV data from a buffer and copy into a structure.
/*---------------------------------------------------------------------------*/
size_t tlv_parse_struct (void *data_ptr, unsigned int data_size,
                         void *struct_ptr,
                         const tlv_offset_struct_t *tlv_table_ptr)
{
    size_t          bytes_consumed = 0;
    unsigned int    type = 0;
    unsigned int    length = 0;
    uint8_t         *end_ptr = NULL;
    uint8_t         *ptr = NULL;
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
        ptr = (uint8_t *)data_ptr;
        end_ptr = (uint8_t *)data_ptr + data_size;

        while ((ptr + 2) <= end_ptr)
        {
            type = get_u8(&ptr);
            length = get_u8(&ptr);

            if ((0 == type) && (0 == length))
            {
                if ((ptr + sizeof(uint16_t)) <= end_ptr)
                {
                    uint16_t calculated_crc = crc_calculate (data_ptr,
                                           (size_t)(ptr - (uint8_t *)data_ptr));
                    uint16_t crc = get_u16(&ptr);

                    if (crc == calculated_crc)
                    {
                        bytes_consumed = (size_t)(ptr - (uint8_t *)data_ptr);
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
            ptr = (uint8_t *)data_ptr;
            end_ptr = (uint8_t *)data_ptr + data_size;
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
// Write variables in the TLV structure to a TLV buffer.
/*---------------------------------------------------------------------------*/
size_t tlv_write_ptr (void *destPtr, unsigned int destSize,
                       const tlv_ptr_struct_t *tlv_table_ptr)
{
    size_t bytes_written = 0;
    uint8_t *end_ptr = NULL;

#if (DEBUG_TLV > 1)
    DEBUG_PRINTF ("tlv_write_ptr (0x%x, %u, 0x%x)\r\n",
                  (unsigned int)(uintptr_t)destPtr,
                  destSize, (unsigned int)(uintptr_t)tlv_table_ptr);
#endif

    if ((NULL != destPtr) && (NULL != tlv_table_ptr))
    {
        unsigned int tableEntry = 0;
        uint8_t *ptr = NULL;

        ptr = (uint8_t *)destPtr;
        end_ptr = (uint8_t *)destPtr + destSize;

        tableEntry = 0;
        while (tlv_table_ptr[tableEntry].value_ptr != NULL)
        {
            size_t entry_size = sizeof(uint8_t) + sizeof(uint8_t) +
                                tlv_table_ptr[tableEntry].length;

#if (DEBUG_TLV > 0)
            DEBUG_PRINTF ("%u. %u,%u\r\n", tableEntry,
                          tlv_table_ptr[tableEntry].type, tlv_table_ptr[tableEntry].length);
#endif

            if ((ptr + entry_size) > end_ptr)
            {
#if (DEBUG_TLV > 0)
                DEBUG_PRINTF ("tlv_write_ptr: EEPROM area too small to fit all.\n");
#endif
                bytes_written = 0;
                ptr = NULL;
                break;
            }

            put_u8 (&ptr, tlv_table_ptr[tableEntry].type);
            put_u8 (&ptr, tlv_table_ptr[tableEntry].length);
            put_data (&ptr, tlv_table_ptr[tableEntry].value_ptr, tlv_table_ptr[tableEntry].length);

            tableEntry++;
        }

        if (ptr != NULL)
        {
            // Ensure an "empty" entry is written to the end, if room
            // is available. (Type 0, Length 0)
            // Also include room for 16-bit CRC.
            if ((ptr + sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint16_t)) <= end_ptr)
            {
                put_u8 (&ptr, 0); // No type.
                put_u8 (&ptr, 0); // No length. This signals the end of data.

                // Calculate CRC over the buffer.
                uint16_t crc = crc_calculate (destPtr,
                                              (size_t)(ptr - (uint8_t *)destPtr));

                // Add the 16-bit CRC at the end.
                put_u16 (&ptr, crc);

                bytes_written = (size_t)(ptr - (uint8_t *)destPtr); // All data + terminator fit.
            }
            else
            {
#if (DEBUG_TLV > 0)
                DEBUG_PRINTF ("tlv_write_ptr: No room for terminator and/or CRC.\n");
#endif
                bytes_written = 0; // Data was too large to write + terminator.
            }
        }
    }

    return bytes_written;
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

// End of tlv.c
