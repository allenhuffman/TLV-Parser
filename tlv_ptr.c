/*---------------------------------------------------------------------------*/
// Sub-Etha Software's TLV Parser (variable pointer version)
// By Allen C. Huffman
// www.subethasoftware.com
//
// This is a simple TLV (Type-Length-Value) parser that can be used to read
// and write TLV data to and from buffers.
//
// FILE HISTORY:
//
// 2026-05-08 allenh - Added more comments.
//
// TODO:
//
// * Add support for big/little endien conversion when copying data.
// * Add ability to copy smaller data into larger variable (e.g. copy 1 byte
//   into uint16_t).
//
// TOFIX:
//
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
// Include Files
/*---------------------------------------------------------------------------*/
// Compiler headers.
#include <inttypes.h> // uintptr_t
#include <stdbool.h>  // for bool/true/false
#include <stddef.h>   // for NULL
#include <stdio.h>    // for printf
#include <string.h>   // for memcpy ()

// This file's header.
#include "tlv_ptr.h"

// Other headers.
#include "crc.h"
#include "get_put_values.h"

/*---------------------------------------------------------------------------*/
// Constants
/*---------------------------------------------------------------------------*/
// 0 = No debug messages, 1 = Debug messages, 2 = Even more debug messages.
#define DEBUG_TLV 0

#if (DEBUG_TLV > 0)
    #define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
    #define DEBUG_PRINTF(...)
#endif


/*---------------------------------------------------------------------------*/
// Functions
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
// Read and parse TLV data from a buffer and copy into variables.
/*---------------------------------------------------------------------------*/
size_t tlv_parse_ptr (const void *data_ptr, unsigned int data_size,
                      const tlv_ptr_struct_t *tlv_table_ptr)
{
    size_t bytes_consumed = 0;

#if (DEBUG_TLV > 1)
    DEBUG_PRINTF ("tlv_parse_ptr (0x%x, %u, 0x%x)\r\n",
                  (unsigned int)((uintptr_t)data_ptr),
                  data_size, (unsigned int)(uintptr_t)tlv_table_ptr);
#endif

    // Must have seemingly valid pointers.
    if ((NULL != data_ptr) && (NULL != tlv_table_ptr))
    {
        unsigned int    type = 0;
        unsigned int    length = 0;
        const uint8_t   *end_ptr = NULL;
        const uint8_t   *ptr = NULL;
        unsigned int    table_entry = 0;

        // To avoid parsing corrupt data, we first need to scan and find the
        // end of the TLV data and then check the CRC.

        // Set ptr to start of buffer.
        ptr = (const uint8_t *)data_ptr;
        // Set end_ptr to end of buffer.
        end_ptr = (const uint8_t *)data_ptr + data_size;

        // Must have at least 3 more bytes to parse before the end of buffer.
        while ((ptr + 2) <= end_ptr)
        {
            // Get type byte and length byte.
            type = get_u8 (&ptr);
            length = get_u8 (&ptr);

            // If both are zero, it is the end of the TLV data.
            if ((0 == type) && (0 == length))
            {
                // If there is NOT room for the 2-byte CRC...
                if ((ptr + sizeof (uint16_t)) > end_ptr)
                {
                    bytes_consumed = 0;
                }
                else
                {
                    // Calculate CRC over the used memory.
                    uint16_t calculated_crc = crc_calculate (data_ptr,
                                           (size_t)(ptr - (const uint8_t *)data_ptr));

                    // Get 2-byte CRC from buffer.
                    uint16_t crc = get_u16 (&ptr);

                    // If it matches, update bytes_consumed value.
                    if (crc == calculated_crc)
                    {
                        bytes_consumed = (size_t)(ptr - (const uint8_t *)data_ptr);
                    }
                    else
                    {
#if (DEBUG_TLV > 0)
                        DEBUG_PRINTF ("Bad CRC - Expected 0x%04x, read 0x%04x\r\n",
                                      crc, calculated_crc);
#endif
                        bytes_consumed = 0;
                    }
                }

                // Exit out of byte scanning loop.
                break;
            } // end of if ((0 == type) && (0 == length))

            // If stored length would exceed size of buffer...
            if ((ptr + length) > end_ptr)
            {
                // ...data is corrupt or invalid. Set bytes consumed to 0.
                bytes_consumed = 0;

                // Exit out of byte scanning loop.
                break;
            }

            // Otherwise, move pointer forward length bytes, skipping the data.
            ptr += length;
        } // end of while ((ptr + 2) <= end_ptr)

        // Done scanning, but proceed only if bytes consumed is non-zero.
        if (0 != bytes_consumed)
        {
            // Now we scan the validated buffer again and try to load data.
            bool keep_scanning = true;

            // Reset ptr to start of buffer.
            ptr = (const uint8_t *)data_ptr;
            // Reset end_ptr to end of buffer.
            end_ptr = (const uint8_t *)data_ptr + data_size;

            // Must have at least 3 more bytes to parse before the end of buffer.
            while ((ptr + 2) <= end_ptr)
            {
#if (DEBUG_TLV > 0)
                bool type_found = false;
#endif
                // Get type byte and length byte.
                type = get_u8 (&ptr);
                length = get_u8 (&ptr);

                // If both are zero, it is the end of the TLV data.
                if ((0 == type) && (0 == length))
                {
                    // Exit out of byte scanning loop #2.
                    break;
                }

                // If stored length would exceed size of buffer...
                if ((ptr + length) > end_ptr)
                {
#if (DEBUG_TLV > 0)
                    DEBUG_PRINTF ("tlv_parse_ptr: Type %u length %u exceeds buffer.\n",
                                  type, length);
#endif
                    // ...data is corrupt or invalid. Set bytes consumed to 0.
                    bytes_consumed = 0;

                    // Exit out of byte scanning loop #2.
                    break;
                }

                // Scan through TLV table starting with the first entry.
                table_entry = 0;

                // Scan as long as table has non-zero entries.
                while ((tlv_table_ptr[table_entry].value_ptr != NULL) &&
                       (true == keep_scanning))
                {
#if (DEBUG_TLV > 0)
                    type_found = false;
#endif
                    // If table entry type matches type read from buffer...
                    if (tlv_table_ptr[table_entry].type == type)
                    {
                        // ...and entry length matches length read from buffer.
                        if (tlv_table_ptr[table_entry].length == length)
                         {
                            // Copy length bytes from buffer into variable
                            // at table entry pointer.
                            memcpy (tlv_table_ptr[table_entry].value_ptr,
                                    ptr, tlv_table_ptr[table_entry].length);
#if (DEBUG_TLV > 0)
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
                            keep_scanning = false;

                            bytes_consumed = 0;
                        }

                        // Exit byte scanning loop #2.
                        break;
                    } // end of if (tlv_table_ptr[table_entry].type  . . .

                    // Move to next table entry.
                    table_entry++;
                } // end of while (tlv_table_ptr[table_entry].value_ptr . . .

#if (DEBUG_TLV > 0)
                if ((true == keep_scanning) && (true != type_found))
                {
                    DEBUG_PRINTF ("Skipping unknown type %u (%u bytes).\r\n", type, length);
                }
#endif
                // If parsing found a type we expect, but the length
                // was not what we expect, consider any data from this
                // point forward to be bad and do not parse it.
                if (0 == bytes_consumed)
                {
                    // Exit out of byte scanning loop #2.
                    break;
                }

                // Otherwise, move forward in buffer by length bytes.
                ptr += length;
            } // end of while ((ptr + 2) <= end_ptr)
        } // end of if (0 != bytes_consumed)
    } // end of if ((NULL != data_ptr) . . .

    // 0 will mean couldn't parse, else # of bytes parsed.
    return bytes_consumed;
}


/*---------------------------------------------------------------------------*/
// Write variables in the TLV structure to a TLV buffer.
/*---------------------------------------------------------------------------*/
size_t tlv_write_ptr (void *destPtr, unsigned int destSize,
                       const tlv_ptr_struct_t *tlv_table_ptr)
{
    size_t bytes_written = 0;

#if (DEBUG_TLV > 1)
    DEBUG_PRINTF ("tlv_write_ptr (0x%x, %u, 0x%x)\r\n",
                  (unsigned int)(uintptr_t)destPtr,
                  destSize, (unsigned int)(uintptr_t)tlv_table_ptr);
#endif

    if ((NULL != destPtr) && (NULL != tlv_table_ptr))
    {
        uint8_t *ptr = NULL;
        uint8_t *end_ptr = NULL;
        unsigned int tableEntry = 0;

        ptr = (uint8_t *)destPtr;
        end_ptr = (uint8_t *)destPtr + destSize;

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

        if (NULL != ptr)
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

// End of tlv_ptr.c
