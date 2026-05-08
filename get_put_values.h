// https://github.com/allenhuffman/GetPutData/

/*---------------------------------------------------------------------------*/
// Include Files
/*---------------------------------------------------------------------------*/
// Compiler headers.
#include <stdint.h>

/*---------------------------------------------------------------------------*/
// Prototypes
/*---------------------------------------------------------------------------*/
void get_data (const uint8_t **ptr, void *data_ptr, unsigned int data_size);
uint8_t get_u8 (const uint8_t **ptr);
uint16_t get_u16 (const uint8_t **ptr);
uint32_t get_u32 (const uint8_t **ptr);
float get_float (const uint8_t **ptr);
double get_double (const uint8_t **ptr);

void put_data (uint8_t **ptr, void *data_ptr, unsigned int data_size);
void put_u8 (uint8_t **ptr, uint8_t value);
void put_u16 (uint8_t **ptr, uint16_t value);
void put_u32 (uint8_t **ptr, uint32_t value);
void put_float (uint8_t **ptr, float value);
void put_double (uint8_t **ptr, double value);

// End of get_put_values.h
