# Note from a Human
Github Copilot created the tests and the bulk of this read README, but I wrote the TLP parser. My original version used variable pointers to know where to copy data read from the TLP buffer. That version has now been renamed to tlv_parse_ptr() and tlv_write_ptr() since there is now a second method. Copilot assisted in creating a version that is based on using a structure and offsets to the elements inside of it. I needed this for a new project and decided to just present both.

# TLVParser

Lightweight C11 TLV (Type-Length-Value) parser/writer with CRC-16/XMODEM validation.

This project supports two data mapping modes:

- Pointer tables: map TLV types directly to variable pointers.
- Struct offset tables: map TLV types to fields in a struct using offsets.

## Features

- Read and write TLV payloads with a terminator (`00 00`) and trailing CRC-16.
- CRC-16/XMODEM verification before accepting payload data.
- Bounds checks to prevent buffer overrun on parse and write.
- Unknown TLV types are skipped safely.
- Known-type length mismatches are treated as invalid data.
- Single return style for core APIs: `0` means failure, non-zero is bytes consumed/written.

## File Layout

- `tlv.h`: Public API, TLV table types, helper macros.
- `tlv.c`: Parser/writer implementation for pointer and struct modes.
- `crc.c`, `crc.h`: CRC-16/XMODEM routines.
- `get_put_values.c`, `get_put_values.h`: Byte/word read-write helpers.
- `ptr_table_tests.c`: Pointer-table test suite.
- `struct_table_tests.c`: Struct-table test suite.
- `main.c`: Test runner entry point.
- `Makefile`: Build and run targets.

## Build

```sh
make
```

## Run Tests

```sh
./tlv_test
```

Or:

```sh
make run
```

## Clean

```sh
make clean
```

## Public API

```c
size_t tlv_parse_ptr (void *data_ptr, unsigned int data_size,
                      tlv_ptr_struct_t *tlv_table_ptr);

size_t tlv_parse_struct (void *data_ptr, unsigned int data_size,
                         void *struct_ptr,
                         const tlv_offset_struct_t *tlv_table_ptr);

size_t tlv_write_ptr (void *dest_ptr, unsigned int dest_size,
                      const tlv_ptr_struct_t *tlv_table_ptr);

size_t tlv_write_struct (void *dest_ptr, unsigned int dest_size,
                         const void *struct_ptr,
                         const tlv_offset_struct_t *tlv_table_ptr);
```

Return value convention:

- `0`: failure
- `> 0`: number of bytes consumed or written

## Table Macros

Pointer-table entries:

```c
#define TLVPTRENTRY(n,v)   { n, sizeof(v), &v }
#define TLVPTRENTRYEND     { 0, 0, NULL }
```

Struct-table entries:

```c
#define TLVSTRUCTENTRY(n,s,v) { n, sizeof(((s *)0)->v), offsetof(s, v) }
#define TLVSTRUCTENTRYEND     { 0, 0, 0 }
```

## Quick Usage

### Pointer Table Example

```c
uint8_t  byte = 0x11;
uint16_t word = 0x2222;
uint32_t dword = 0x33333333;
char     string[6] = "ABCDE";

tlv_ptr_struct_t table[] =
{
    TLVPTRENTRY(1, byte),
    TLVPTRENTRY(2, word),
    TLVPTRENTRY(3, dword),
    TLVPTRENTRY(4, string),
    TLVPTRENTRYEND
};

uint8_t buffer[64] = { 0 };

// Write variables out to TLP buffer.
size_t written = tlv_write_ptr (buffer, sizeof(buffer), table);

// Parse TLV buffer and load data into variables.
size_t parsed  = tlv_parse_ptr (buffer, (unsigned int)written, table);
```

### Struct Table Example

```c
typedef struct
{
    uint8_t  byte;
    uint16_t word;
    uint32_t dword;
    char     string[6];
} my_struct_t;

my_struct_t msg = { 0x11, 0x2222, 0x33333333, "ABCDE" };

const tlv_offset_struct_t table[] =
{
    TLVSTRUCTENTRY(1, my_struct_t, byte),
    TLVSTRUCTENTRY(2, my_struct_t, word),
    TLVSTRUCTENTRY(3, my_struct_t, dword),
    TLVSTRUCTENTRY(4, my_struct_t, string),
    TLVSTRUCTENTRYEND
};

uint8_t buffer[64] = { 0 };
size_t written = tlv_write_struct (buffer, sizeof(buffer), &msg, table);

my_struct_t out = { 0 };
size_t parsed = tlv_parse_struct (buffer, (unsigned int)written, &out, table);
```

## Test Coverage Summary

Both pointer and struct suites cover:

- Small buffer write failure
- Successful write to large buffer
- Exact-fit write success and off-by-one write failure
- Bad CRC rejection
- Truncated payload rejection
- Unknown type skipping
- Overrun-length rejection
- Known-type wrong-length rejection
- Duplicate type handling
- Missing type behavior
- Reordered table behavior
- Guard-byte checks to detect unintended writes on failure paths

## Notes

- CRC bytes are stored little-endian in the generated/test buffers.
- Debug output can be controlled via `DEBUG_TLV` in `tlv.h`.

## License

No license file is currently included in this folder.
If you plan to publish publicly, consider adding one (for example MIT or BSD-3-Clause).
