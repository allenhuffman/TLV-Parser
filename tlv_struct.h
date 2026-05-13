/** @file tlv_struct.h
 *
 * @author Allen C. Huffman
 * @copyright Copyright (c) 2026 Sub-Etha Software
 * @note Origin: https://github.com/allenhuffman
 * @note This file follows the Barr-C Embedded C Coding Standard.
 *
 * @brief Public interface for the TLV structure module.
 *
 * @details This module is implemented according to the Barr Group
 * Embedded C Coding Standard (Barr-C).
 *
 * @section history File History
 * - 2026-05-08 allenh - Updated to Barr-C style.
 */
#ifndef TLV_STRUCT_H
#define TLV_STRUCT_H

/* System headers */

#include <stddef.h>
#include <stdint.h>


/* Public macros: all #define items, constants and function-like macros */

// CCS PCD compiler for PIC24 handles const differently that standard C
// compilers. For environments like that, this macro can be modified to
// remove the use of const in this module.
#ifndef CONST
    #define CONST const
    //#define CONST
#endif

#define TLVSTRUCTENTRY(n,s,v) { n, sizeof(((s *)0)->v), offsetof(s, v) }
#define TLVSTRUCTENTRYEND     { 0, 0, 0 }

/* Public structs: concrete data layouts used by this module */

// This structure is used to define a TLV table that contains offsets to
// where elements are within a structure that will be loaded with value data.
typedef struct
{
   uint8_t  type;
   uint8_t  length;
   uint16_t offset; // or uint8_t if not struct is > 255 bytes.
} tlv_offset_entry_t;


/* Public function prototypes */

size_t tlv_decode_offset (CONST void * p_buf,
                          unsigned int buf_size,
                          CONST tlv_offset_entry_t * p_tlv_table,
                          void * p_struct);

size_t tlv_encode_offset (void * p_dest,
                          unsigned int dest_size,
                          CONST tlv_offset_entry_t * p_tlv_table,
                          CONST void * p_struct);

#endif /* TLV_STRUCT_H */

/*** end of file ***/
