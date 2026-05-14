/*---------------------------------------------------------------------------*/
// Sub-Etha Software's TLV Parser (test main)
// By Allen C. Huffman
// www.subethasoftware.com
//
// This is a simple TLV (Type-Length-Value) parser that can be used to read
// and write TLV data to and from buffers.
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
#include <stdio.h>

// Other headers.
#include "tests/ptr_table_tests.h"
#include "tests/struct_table_tests.h"

/*---------------------------------------------------------------------------*/
// Functions
/*---------------------------------------------------------------------------*/

int main (void)
{
    printf ("TLV Parser test.\n");

    run_ptr_table_tests ();
    run_struct_table_tests ();

    return 0;
}

// End of main.c
