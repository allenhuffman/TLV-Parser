/*---------------------------------------------------------------------------*/
// Sub-Etha Software's TLV Parser (structure offset version)
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
#include "tlv_struct.h"

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

#define TLV_HEADER_SIZE   (sizeof(uint8_t) + sizeof(uint8_t))
#define TLV_CRC_SIZE      (sizeof(uint16_t))


/*---------------------------------------------------------------------------*/
// Functions
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
// Read and parse TLV data from a buffer and copy into a structure.
/*---------------------------------------------------------------------------*/
size_t tlv_parse_struct (const void * p_buf,
                         unsigned int buf_size,
                         const tlv_offset_entry_t * p_tlv_table,
                         void * p_struct)
{
    size_t bytes_consumed = 0;

#if (DEBUG_TLV > 1)
     DEBUG_PRINTF ("tlv_parse_struct (0x%x, %u, 0x%x, 0x%x)\r\n",
                   (unsigned int)((uintptr_t)p_buf),
                   buf_size,
                   (unsigned int)((uintptr_t)p_struct),
                   (unsigned int)(uintptr_t)p_tlv_table);
#endif

    // Must have seemingly valid pointers.
    if ((NULL != p_buf) && (NULL != p_tlv_table) && (NULL != p_struct))
    {
        const uint8_t * p_read  = NULL;
        const uint8_t * p_end   = NULL;
        unsigned int    type    = 0;
        unsigned int    length  = 0;

        // To avoid parsing corrupt data, we first need to scan and find the
        // end of the TLV data and then check the CRC.

        // Set p_read to start of buffer.
        p_read = (const uint8_t *)p_buf;

        // Set p_end to end of buffer.
        p_end = (const uint8_t *)p_buf + buf_size;

        // Must have at least 3 more bytes to parse before the end of buffer.
        while ((p_read + 2) <= p_end)
        {
            // Get type byte and length byte.
            type = get_u8 (&p_read);
            length = get_u8 (&p_read);

            // If both are zero, it is the end of the TLV data.
            if ((0 == type) && (0 == length))
            {
                // If there is NOT room for the 2-byte CRC...
                if ((p_read + sizeof (uint16_t)) > p_end)
                {
                    bytes_consumed = 0;
                }
                else
                {
                    // Calculate CRC over buffer up to this point.
                    uint16_t calculated_crc = crc_calculate (p_buf,
                        (size_t)(p_read - (const uint8_t *)p_buf));
                    
                    // Get 2-byte CRC from buffer.
                    uint16_t crc = get_u16 (&p_read);

                    // If it matches, update bytes_consumed value.
                    if (crc == calculated_crc)
                    {
                        bytes_consumed = (size_t)(p_read - 
                            (const uint8_t *)p_buf);
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
            if ((p_read + length) > p_end)
            {
                // ...data is corrupt or invalid. Set bytes consumed to 0.
                bytes_consumed = 0;
                
                // Exit out of byte scanning loop.
                break;
            }

            // Otherwise, move pointer forward length bytes, skipping the data.
            p_read += length;
        } // end of while ((p_write + 2) <= p_end)

        // Done scanning, but proceed only if bytes consumed is non-zero.
        if (0 != bytes_consumed)
        {
            // Now we scan the validated buffer again and try to load data.
            bool keep_scanning = true;

            // Reset pointer to start of buffer.
            p_read = (const uint8_t *)p_buf;

            // Reset end pointer to end of buffer.
            p_end = (const uint8_t *)p_buf + buf_size;

            // Must have at least 3 more bytes to parse before the end of buffer.
            while ((p_read + 2) <= p_end)
            {
#if (DEBUG_TLV > 0)
                bool type_found = false;
#endif
                // Get type byte and length byte.
                type = get_u8 (&p_read);
                length = get_u8 (&p_read);

                // If both are zero, it is the end of the TLV data.
                if ((0 == type) && (0 == length))
                {
                    // Exit out of byte scanning loop #2.
                    break;
                }

                // If stored length would exceed size of buffer...
                if ((p_read + length) > p_end)
                {
#if (DEBUG_TLV > 0)
                    DEBUG_PRINTF ("Type %u length %u exceeds buffer.\n",
                                  type, length);
#endif
                    // ...data is corrupt or invalid. Set bytes consumed to 0.
                    bytes_consumed = 0;
                    
                    // Exit out of byte scanning loop #2.
                    break;
                }

                // Scan through TLV table starting with the first entry.
                unsigned int table_entry = 0;
                
                // Scan as long as table has non-zero type and length.
                while ((true == keep_scanning) && 
                       (0 != p_tlv_table[table_entry].type) &&
                       (0 != p_tlv_table[table_entry].length))
                {
#if (DEBUG_TLV > 0)
                    type_found = false;
#endif
                    // Compare table type with read type.
                    if (p_tlv_table[table_entry].type == type)
                    {
                        // Compare table length with read length.
                        if (p_tlv_table[table_entry].length == length)
                        {
                            // Copy length bytes from buffer into structure
                            // at table entry offset.
                            memcpy ((uint8_t *)p_struct + p_tlv_table[table_entry].offset,
                                    p_read, p_tlv_table[table_entry].length);
#if (DEBUG_TLV > 0)
                            type_found = true;
#endif
                        }
                        else
                        {
                            // Buffer length for this type did not match...
#if (DEBUG_TLV > 0)
                            DEBUG_PRINTF ("T:%u L:%u is wrong (expected L:%u). Skipping rest of this section.\r\n",
                                          type, length, p_tlv_table[table_entry].length);
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
                    } // end of if (p_tlv_table[table_entry].type == type)

                    // Move to next table entry.
                    table_entry++;
                } // end of while ((true == keep_scanning)

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
                p_read += length;
            } // end of while ((p_write + 2) <= end_ptr)
        } // end of if (0 != bytes_consumed)
    } // end of if ((NULL != data_ptr) . . .

    // 0 will mean couldn't parse, else # of bytes parsed.
    return bytes_consumed;
}


/*---------------------------------------------------------------------------*/
// Write elements of a structure in the TLV structure to a TLV buffer.
/*---------------------------------------------------------------------------*/
size_t tlv_write_struct (void * p_dest,
                         unsigned int dest_size,
                         const tlv_offset_entry_t * p_tlv_table,
                         const void * p_struct)
{
    size_t bytes_written = 0;

#if (DEBUG_TLV > 1)
    DEBUG_PRINTF ("tlv_write_struct (0x%x, %u, 0x%x, 0x%x)\r\n",
                  (unsigned int)(uintptr_t)p_dest,
                  dest_size, (unsigned int)(uintptr_t)p_tlv_table,
                  (unsigned int)(uintptr_t)p_struct,
);
#endif

    if ((NULL != p_dest) && (NULL != p_tlv_table) && (NULL != p_struct))
    {
        uint8_t      * p_write     = NULL;
        uint8_t      * p_end       = NULL;
        unsigned int   table_entry = 0;

        p_write = (uint8_t *)p_dest;
        p_end = (uint8_t *)p_dest + dest_size;

		// While table entry is valid (type and length non-zero).
        while ((0 != p_tlv_table[table_entry].type) &&
               (0 != p_tlv_table[table_entry].length))
        {
            size_t entry_size = sizeof(uint8_t) + sizeof(uint8_t) +
                                p_tlv_table[table_entry].length;

#if (DEBUG_TLV > 0)
            DEBUG_PRINTF ("%u. %u,%u\r\n", table_entry,
                          p_tlv_table[table_entry].type,
                          p_tlv_table[table_entry].length);
#endif

            if ((p_write + entry_size) > p_end)
            {
#if (DEBUG_TLV > 0)
                DEBUG_PRINTF ("EEPROM area too small to fit all.\n");
#endif
                bytes_written = 0;

                break;
            }

            put_u8 (&p_write, p_tlv_table[table_entry].type);
            put_u8 (&p_write, p_tlv_table[table_entry].length);
            put_data (&p_write,
                      ((const uint8_t *)p_struct + p_tlv_table[table_entry].offset),
                      p_tlv_table[table_entry].length);

            bytes_written = bytes_written + TLV_HEADER_SIZE + p_tlv_table[table_entry].length;

            table_entry++;
        }

        if (0 != bytes_written)
        {
            // Ensure an "empty" entry is written to the end, if room
            // is available. (Type 0, Length 0)
            // Also include room for 16-bit CRC.
            if ((p_write + TLV_HEADER_SIZE + TLV_CRC_SIZE) <= p_end)
            {
                put_u8 (&p_write, 0); // No type.
                put_u8 (&p_write, 0); // No length. This signals the end of data.

                // Calculate CRC over the buffer.
                uint16_t crc = crc_calculate (p_dest,
                                              (size_t)(p_write - (uint8_t *)p_dest));

                // Add the 16-bit CRC at the end.
                put_u16 (&p_write, crc);
				
				bytes_written = bytes_written + TLV_HEADER_SIZE + TLV_CRC_SIZE;

                //bytes_written = (size_t)(p_write - (uint8_t *)p_dest); // All data + terminator fit.
            }
            else
            {
#if (DEBUG_TLV > 0)
                DEBUG_PRINTF ("No room for terminator and/or CRC.\n");
#endif
                bytes_written = 0; // Data was too large to write + terminator.
            }
        }
    }

    return bytes_written;
}

// End of tlv_struct.c
