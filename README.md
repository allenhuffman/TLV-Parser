# TLV-Parser

## Note from a Human
Github Copilot created the tests and the bulk of this README, but I wrote the original TLV parser. My first version used variable pointers to know where to copy data read from the TLV buffer. That version has now been renamed to tlv_parse_ptr() and tlv_write_ptr() since there is now a second method. Copilot assisted in creating a version that is based on using a structure and offsets to the elements inside of it. I needed this for a new project and decided to just present both.

T-L-V (Type-Length-Value) it a way to encode types of variable values (like an int, a double, a string, etc.) into a buffer, and load them back into variables later.

Data is represented by a numeric type, followed by a numeric length of that type, then the actual data. When parsing a buffer of data in this TLV format, the parsing routine can easily skip over any types it does not recognize. This means as new types or added, older code can still parse the types it knows while skipping over newer unknown types.

Here is the Wikipedia entry about TLV:

https://en.wikipedia.org/wiki/Type–length–value

I originally wrote a TLV parser some years ago at a job where we had been using a fixed structure to store device configuration in an EEPROM. If we wanted to change that data (adding new items or removing/changing existing ones), that would break compatibility with all the tools that expected the data to be in a specific format. For example

```
typedef struct
{
    uint8_t default_color;
    uint8_t default_mode;
    uint16_t default_value;
} configuration_struct_t;
```

If that data was written out to a buffer (for saving to an EEPROM, or a file, or sending across a network), it might be packed together so you have the bytes like:

```
 color mode     value
 ----  ----  ----------
[0x01][0x02][0x11][0x22]
```

When loading that data, it would expect the first byte to be the default color, the second byte to be the default mode, and the third and fourth bytes to be the default value. If default mode needed to be changed to a uint16_t, needing two bytes, the old parser would be unable to properly process this data. Such changes require updating both the writing and parsing routines. (Thus, "a breaking change".)

But with TLVs, the data is encoded with extra "type" and "length" values. For this implemention I used bytes for those since I did not need more than 255 variables, but it could be easily changed to use a uint16_2 or even uint32_t. If any data was expected to be larger than 255 bytes, the length type could also be increased.

But I digress.

Using a TLV method, the variables would have type numbers that writer and parse would both know about:

```
Types
[01] - uint8_t default_color
[02] - uint8_t default_mode
[03] - uint16_t default_value
```

Data would be stored with that type value, followed by the length of the data, then the actual data itself. For the above configuration, it might look like:

```
default_color = 0x42;
default_mode = 0x99;
default_value = 0x1234;

...

[0x01][0x01][0x42][0x02][0x01][0x99][0x03][0x12][0x34]
```

Breaking it apart may make it easier to see:

```
[0x01] - type 1, whatever that is
[0x01] - one byte size
[0x42] - the value that goes into that byte

[0x02] - type 2, whatever that is
[0x01] - one byte in size
[0x99] - the value that goes into that byte

[0x03] - type 3, whatever that is
[0x02] - two bytes in size
[0x12] - the values that go into those two bytes
[0x34] - 
```

If both writer and parser know about types 1, 2 and 3, they can write and parse these values. But if the writer was updated with a new type 4, such as a 4-byte floating point variable, that would be an unknown type to the reader. The reader would get to the type 4, not know what to do with it, and just skip past "length" bytes and try to parse whatever is after it.

This allows new items to be added to an EEPROM configuration without breaking compatibility. A parser just skips things it does not know what to do with.

If you are unfamiliar with TLVs, hopefully this helps make some sense of it.

Anyway, the goal of this implementation was to make it super simple. You create variables (standalone or as part of a structure) then define a special TLV table that is used to know where each of those variables is in memory, as well as its type and size. The parsing routine can just be handed a buffer and a lookup table, then it will parse the buffer and copy the values into the appropriate spot where the variable(s) are in memory.

The original version required the variables to exist in memory before the TLV table could be declared, which worked well for my intended use.

The new version requires variables to be elements of a structure, and the table contains offsets (from start of structure to wherever the element is) to know where to copy value data to. This means you can declare the tables based on the structure, rather than having to have variables in existence first.

Confusing? Well, read the rest of this (mostly written by the robot) and see if it helps. I will try to add some examples to this repo when I get a moment.

# Description #

Lightweight C11 TLV (Type-Length-Value) parser/writer with CRC-16/XMODEM validation.

This project supports two data mapping modes:

- Pointer tables: map TLV types directly to variable pointers.
- Struct offset tables: map TLV types to fields in a struct using offsets.

## Architecture

The code is split by API style:

- Pointer API implementation: `tlv_ptr.c` + `tlv_ptr.h`
- Struct-offset API implementation: `tlv_struct.c` + `tlv_struct.h`

Shared helpers are:

- CRC support: `crc.c` + `crc.h`
- Buffer get/put helpers: `get_put_values.c` + `get_put_values.h`

## Features

- Read and write TLV payloads with a terminator (`00 00`) and trailing CRC-16.
- CRC-16/XMODEM verification before accepting payload data.
- Bounds checks to prevent buffer overrun on parse and write.
- Unknown TLV types are skipped safely.
- Known-type length mismatches are treated as invalid data.
- Single return style for core APIs: `0` means failure, non-zero is bytes consumed/written.

## File Layout

- `tlv_ptr.h`: Pointer-table API, macros, and typedefs.
- `tlv_ptr.c`: Pointer-table parser/writer implementation.
- `tlv_struct.h`: Struct-offset API, macros, and typedefs.
- `tlv_struct.c`: Struct-offset parser/writer implementation.
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

From `tlv_ptr.h` and `tlv_struct.h`:

```c
size_t tlv_parse_ptr (const void *p_buf, unsigned int buf_size,
                      const tlv_ptr_entry_t *p_tlv_table);

size_t tlv_parse_struct (const void *p_buf, unsigned int buf_size,
                         const tlv_offset_entry_t *p_tlv_table,
                         void *p_struct);

size_t tlv_write_ptr (void *p_dest, unsigned int dest_size,
                      const tlv_ptr_entry_t *p_tlv_table);

size_t tlv_write_struct (void *p_dest, unsigned int dest_size,
                         const tlv_offset_entry_t *p_tlv_table,
                         const void *p_struct);
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
// These must remain in scope for the duration of the tlv_write_ptr and
// tlv_parse_ptr calls. If they go out of scope, then the pointers in the
// table will be invalid. Use global or static variables if needed, or
// make sure you do it all within the same function scope.
uint8_t  byte = 0x11;
uint16_t word = 0x2222;
uint32_t dword = 0x33333333;
char     string[6] = "ABCDE";

tlv_ptr_entry_t table[] =
{
    TLVPTRENTRY(1, byte),
    TLVPTRENTRY(2, word),
    TLVPTRENTRY(3, dword),
    TLVPTRENTRY(4, string),
    TLVPTRENTRYEND
};

uint8_t buffer[64] = { 0 };

// Write variables out to TLV buffer.
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

const tlv_offset_entry_t table[] =
{
    TLVSTRUCTENTRY(1, my_struct_t, byte),
    TLVSTRUCTENTRY(2, my_struct_t, word),
    TLVSTRUCTENTRY(3, my_struct_t, dword),
    TLVSTRUCTENTRY(4, my_struct_t, string),
    TLVSTRUCTENTRYEND
};

uint8_t buffer[64] = { 0 };
size_t written = tlv_write_struct (buffer, sizeof(buffer), table, &msg);

my_struct_t out = { 0 };
size_t parsed = tlv_parse_struct (buffer, (unsigned int)written, table, &out);
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
- Debug output can be controlled via `DEBUG_TLV` in `tlv_ptr.h` and `tlv_struct.h`.

## License

No license file is currently included in this folder.
If you plan to publish publicly, consider adding one (for example MIT or BSD-3-Clause).
