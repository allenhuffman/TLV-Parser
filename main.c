#include <stdio.h>

#include "ptr_table_tests.h"
#include "struct_table_tests.h"

int main (void)
{
    printf ("TLV Parser test.\n");

    run_ptr_table_tests ();
    run_struct_table_tests ();

    return 0;
}
