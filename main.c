/*---------------------------------------------------------------------------*/
// Include Files
/*---------------------------------------------------------------------------*/
// Compiler headers.
#include <stdio.h>

// Other headers.
#include "ptr_table_tests.h"
#include "struct_table_tests.h"

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
