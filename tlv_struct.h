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
// TOFIX:
//
/*---------------------------------------------------------------------------*/
#ifndef TLV_STRUCT_H
#define TLV_STRUCT_H

/*---------------------------------------------------------------------------*/
// Include Files
/*---------------------------------------------------------------------------*/
// Compiler headers.
#include <stddef.h>
#include <stdint.h>


/*---------------------------------------------------------------------------*/
// Constants
/*---------------------------------------------------------------------------*/
#define TLVSTRUCTENTRY(n,s,v) { n, sizeof(((s *)0)->v), offsetof(s, v) }
#define TLVSTRUCTENTRYEND     { 0, 0, 0 }


/*---------------------------------------------------------------------------*/
// Typedefs
/*---------------------------------------------------------------------------*/
// This structure is used to define a TLV table that contains offsets to
// where elements are within a structure that will be loaded with value data.
typedef struct
{
   uint8_t  type;
   uint8_t  length;
   size_t   offset;
} tlv_offset_struct_t;


/*---------------------------------------------------------------------------*/
// Prototypes
/*---------------------------------------------------------------------------*/
size_t tlv_parse_struct (const void *data_ptr, unsigned int data_size,
                         void *struct_ptr,
                         const tlv_offset_struct_t *tlv_table_ptr);

size_t tlv_write_struct (void *dest_ptr, unsigned int dest_size,
                         const void *struct_ptr,
                         const tlv_offset_struct_t *tlv_table_ptr);

#endif /* TLV_STRUCT_H */

// End of tlv_struct.h
