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
// TOFIX:
//
/*---------------------------------------------------------------------------*/
#ifndef TLV_PTR_H
#define TLV_PTR_H

/*---------------------------------------------------------------------------*/
// Include Files
/*---------------------------------------------------------------------------*/
// Compiler headers.
#include <stddef.h>
#include <stdint.h>


/*---------------------------------------------------------------------------*/
// Constants
/*---------------------------------------------------------------------------*/
#define TLVPTRENTRY(n,v)   { n, sizeof(v), &v }
#define TLVPTRENTRYEND     { 0, 0, NULL }


/*---------------------------------------------------------------------------*/
// Typedefs
/*---------------------------------------------------------------------------*/
// This structure is used to define a TLV table that contains pointers to
// variables that will be loaded with value data.
typedef struct
{
   uint8_t  type;
   uint8_t  length;
   void     *value_ptr;
} tlv_ptr_struct_t;


/*---------------------------------------------------------------------------*/
// Prototypes
/*---------------------------------------------------------------------------*/
size_t tlv_parse_ptr (const void *data_ptr, unsigned int data_size,
                      const tlv_ptr_struct_t *tlv_table_ptr);

size_t tlv_write_ptr (void *dest_ptr, unsigned int dest_size,
                      const tlv_ptr_struct_t *tlv_table_ptr);

#endif /* TLV_PTR_H */

// End of tlv_ptr.h
