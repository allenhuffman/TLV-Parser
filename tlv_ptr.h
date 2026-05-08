#ifndef TLV_PTR_H
#define TLV_PTR_H


/*---------------------------------------------------------------------------*/
// Includes
/*---------------------------------------------------------------------------*/
#include <stddef.h>
#include <stdbool.h>


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
   unsigned int   type;
   unsigned int   length;
   void           *value_ptr;
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
