/** @file tlv_ptr.h
 *
 * @author Allen C. Huffman
 * @copyright Copyright (c) 2026 Sub-Etha Software
 * @note Origin: https://github.com/allenhuffman
 * @note This file follows the Barr-C Embedded C Coding Standard.
 *
 * @brief Public interface for the TLV pointer module.
 *
 * @details This module is implemented according to the Barr Group
 * Embedded C Coding Standard (Barr-C).
 *
 * @section history File History
 * - 2026-XX-XX allenh - Created.
 *
 * @todo Add module-specific functionality.
 * @todo Document any API-specific edge cases.
 */
#ifndef TLV_PTR_H
#define TLV_PTR_H

/* System headers */

#include <stddef.h>
#include <stdint.h>


/* Public macros: all #define items, constants and function-like macros */

#define TLVPTRENTRY(n,v)   { n, sizeof(v), &v }
#define TLVPTRENTRYEND     { 0, 0, NULL }


/* Public macros: all #define items, constants and function-like macros */

// This structure is used to define a TLV table that contains pointers to
// variables that will be loaded with value data.
typedef struct
{
   uint8_t  type;
   uint8_t  length;
   void     *value;
} tlv_ptr_entry_t;


/* Public function prototypes */

size_t tlv_parse_ptr (const void * p_buf,
                      unsigned int buf_size,
                      const tlv_ptr_entry_t * p_tlv_table);

size_t tlv_write_ptr (void * p_dest,
                      unsigned int dest_size,
                      const tlv_ptr_entry_t * p_tlv_table);

#endif /* TLV_PTR_H */

/*** end of file ***/
