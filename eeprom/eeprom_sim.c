/**
 * @file template.c
 *
 * @author Allen C. Huffman
 * @copyright Copyright (c) 2026 Sub-Etha Software
 * @note Origin: https://github.com/allenhuffman
 * @note This file follows the Barr-C Embedded C Coding Standard.
 *
 * @brief Implementation of the module.
 *
 * @details This module is implemented according to the Barr Group
 * Embedded C Coding Standard (Barr-C).
 *
 * @section history File History
 * - 2026-05-13 allenh - Created.
 *
 * @todo Add eeprom_erase and maybe get size?
 */

/*---------------------------------------------------------------------------*/
// System headers
/*---------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>

/*---------------------------------------------------------------------------*/
// This module's header (must be first among project headers)
/*---------------------------------------------------------------------------*/
#include "eeprom_sim.h"

/*---------------------------------------------------------------------------*/
// External module headers
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
// Public data definitions
/*---------------------------------------------------------------------------*/
/* In the case of a variable name requiring multiple of the above prefixes, the
   order of their inclusion before the first underscore shall be
   [g][p|pp][b|h]. */
// uint8_t    g_counter        = 0U;    /* global */
// uint8_t  * gp_led_reg       = NULL;  /* global pointer */
// uint8_t ** gpp_vector_table = NULL;  /* global pointer to pointer */
// bool       gb_done_yet      = false; /* global boolean */

/*---------------------------------------------------------------------------*/
// Private macros: all #define items, constants and function-like macros
/*---------------------------------------------------------------------------*/
#define EEPROM_SIZE_BYTES (8192U)

/*---------------------------------------------------------------------------*/
// Private constants: typed, debugger-visible constants (prefer static const)
/*---------------------------------------------------------------------------*/
/* Array bounds at file scope must use an integer constant expression in C. */

/*---------------------------------------------------------------------------*/
// Private typedefs: type aliases and opaque handles
/*---------------------------------------------------------------------------*/
//typedef struct template_context template_context_t;

/*---------------------------------------------------------------------------*/
// Private enums
/*---------------------------------------------------------------------------*/
// typedef enum
// {
//     TEMPLATE_STATE_IDLE,
//     TEMPLATE_STATE_BUSY
// } template_state_t;

/*---------------------------------------------------------------------------*/
// Private structs: concrete data layouts used by this module
/*---------------------------------------------------------------------------*/
// typedef struct
// {
//     uint8_t  id;
//     uint16_t counter;
// } template_context_t;

/*---------------------------------------------------------------------------*/
// Private static variables
/*---------------------------------------------------------------------------*/
static uint8_t eeprom_memory[EEPROM_SIZE_BYTES] = { 0 }; /* Simulated EEPROM memory */

/*---------------------------------------------------------------------------*/
// Private function prototypes
/*---------------------------------------------------------------------------*/
//static void template_reset_context(void);

/*---------------------------------------------------------------------------*/
// Public function definitions
/*---------------------------------------------------------------------------*/

/**
 * @brief Write data to the EEPROM.
 *
 * @param[in] address  The EEPROM address to write to.
 * @param[in] p_data   Pointer to the data to be written.
 * @param[in] length   Number of bytes to write.
 *
 * @return The status of the EEPROM write operation.
 */
eeprom_status_t eeprom_write (uint16_t address, const uint8_t *p_data, size_t length)
{
    eeprom_status_t status = EEPROM_OK;

    if (NULL == p_data)
    {
        status = EEPROM_ERR_PARAM; /* Invalid parameter */
    }
    else
    {
        if (((size_t)address >= EEPROM_SIZE_BYTES) ||
            (length > (EEPROM_SIZE_BYTES - (size_t)address)))
        {
            status = EEPROM_ERR_RANGE; /* Address out of range */
        }
        else
        {
            /* Simulate writing to EEPROM by copying data to the eeprom_memory array */
            for (size_t idx = 0; idx < length; idx++)
            {
                eeprom_memory[address + idx] = p_data[idx];
            }
        }
    }

    return status;
}


/**
 * @brief Read data from the EEPROM.
 *
 * @param[in] address  The EEPROM address to read from.
 * @param[in] p_data   Pointer to the buffer to store the read data.
 * @param[in] length   Number of bytes to read.
 *
 * @return The status of the EEPROM read operation.
 */
eeprom_status_t eeprom_read (uint16_t address, uint8_t *p_data, size_t length)
{
    eeprom_status_t status = EEPROM_OK;

    if (NULL == p_data)
    {
        status = EEPROM_ERR_PARAM; /* Invalid parameter */
    }
    else
    {
        if (((size_t)address >= EEPROM_SIZE_BYTES) ||
            (length > (EEPROM_SIZE_BYTES - (size_t)address)))
        {
            status = EEPROM_ERR_RANGE; /* Address out of range */
        }
        else
        {
            /* Simulate reading from EEPROM by copying data from the eeprom_memory array */
            for (size_t idx = 0; idx < length; idx++)
            {
                p_data[idx] = eeprom_memory[address + idx];
            }
        }
    }

    return status;
}


/**
 * @brief Erase data from the EEPROM.
 *
 * @param[in] address  The EEPROM address to erase from.
 * @param[in] length   Number of bytes to erase.
 *
 * @return The status of the EEPROM erase operation.
 */
eeprom_status_t eeprom_erase(uint16_t address, size_t length)
{
    eeprom_status_t status = EEPROM_OK;

    if (((size_t)address >= EEPROM_SIZE_BYTES) ||
        (length > (EEPROM_SIZE_BYTES - (size_t)address)))
    {
        status = EEPROM_ERR_RANGE; /* Address out of range */
    }
    else
    {
        /* Simulate erasing EEPROM by setting the specified range to 0xFF */
        for (size_t idx = 0; idx < length; idx++)
        {
            eeprom_memory[address + idx] = 0xFF;
        }
    }

    return status;
}


/**
 * @brief Get the size of the EEPROM.
 *
 * @return The size of the EEPROM in bytes.
 */
uint16_t eeprom_get_size (void)
{
    return EEPROM_SIZE_BYTES;
}

/* Debug */

/**
 * @brief Dump the contents of the EEPROM for debugging purposes.
 *
 * @param[in] address  The EEPROM address to start dumping from.
 * @param[in] length   Number of bytes to dump.
 *
 * @return The status of the EEPROM dump operation.
 */
eeprom_status_t eeprom_dump (uint16_t address, size_t length)
{
    eeprom_status_t status = EEPROM_OK;

    if (((size_t)address >= EEPROM_SIZE_BYTES) ||
        (length > (EEPROM_SIZE_BYTES - (size_t)address)))
    {
        status = EEPROM_ERR_RANGE; /* Address out of range */
    }
    else
    {
        printf ("EEPROM Dump (Address: 0x%04X, Length: %zu bytes):\n", address, length);
        for (size_t idx = 0; idx < length; idx++)
        {
            printf ("%02X ", eeprom_memory[address + idx]);
            if ((idx + 1) % 16 == 0)
            {
                printf ("\n");
            }
        }
        printf ("\n");
    }

    return status;
}


/*---------------------------------------------------------------------------*/
// Private function definitions
/*---------------------------------------------------------------------------*/

// static void
// template_reset_context (void)
// {
//     template_ctx.id      = 0U;
//     template_ctx.counter = 0U;
// }

/*** end of file ***/
