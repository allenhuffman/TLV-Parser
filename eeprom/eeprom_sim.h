/** @file eeprom_sim.h
 *
 * @author Allen C. Huffman
 * @copyright Copyright (c) 2026 Sub-Etha Software
 * @note Origin: https://github.com/allenhuffman
 * @note This file follows the Barr-C Embedded C Coding Standard.
 *
 * @brief Public interface for the eeprom_sim module.
 *
 * @details This module is implemented according to the Barr Group
 * Embedded C Coding Standard (Barr-C).
 *
 * @section history File History
 * - 2026-05-13 allenh - Created.
 */

#ifndef EEPROM_SIM_H
#define EEPROM_SIM_H

/* System headers */
#include <stdbool.h>
#include <stdint.h>

/* External module headers */

/* Public macros: all #define items, constants and function-like macros */

/* Public constants: typed, debugger-visible constants (prefer static const) */

/* Public typedefs: type aliases and opaque handles   */

/* Public enums */
typedef enum
{
    EEPROM_OK = 0,
    EEPROM_ERR_PARAM,
    EEPROM_ERR_RANGE,
    EEPROM_ERR_BUSY,
    EEPROM_ERR_HW,
    EEPROM_ERR_VERIFY
} eeprom_status_t;

/* Public structs: concrete data layouts used by this module */

/* Public variables */
// extern uint8_t    g_counter;        /* global */
// extern uint8_t  * gp_led_reg;       /* global pointer */
// extern uint8_t ** gpp_vector_table; /* global pointer to pointer */
// extern bool       gb_done_yet;      /* global boolean */

/* Public function prototypes */
eeprom_status_t eeprom_write(uint16_t address, const uint8_t *p_data, size_t length);
eeprom_status_t eeprom_read(uint16_t address, uint8_t *p_data, size_t length);

eeprom_status_t eeprom_erase(uint16_t address, size_t length);
uint16_t eeprom_get_size(void);

/* Debug */
eeprom_status_t eeprom_dump(uint16_t address, size_t length);

#endif /* EEPROM_SIM_H */

/*** end of file ***/
