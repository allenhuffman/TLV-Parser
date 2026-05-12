#ifndef STRUCT_TABLE_TESTS_H
#define STRUCT_TABLE_TESTS_H

/* Public macros: all #define items, constants and function-like macros */

// CCS PCD compiler for PIC24 handles const differently that standard C
// compilers. For environments like that, this macro can be modified to
// remove the use of const in this module.
#ifndef CONST
    #define CONST const
    //#define CONST
#endif

/*---------------------------------------------------------------------------*/
// Prototypes
/*---------------------------------------------------------------------------*/
void run_struct_table_tests (void);

#endif

// End of struct_table_tests.h
