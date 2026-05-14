# Created by Github Copilot.
CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O0 -g -I. -Itests -Ieeprom
LDFLAGS ?=
DEPFLAGS ?= -MMD -MP
STRICT_WARNINGS ?= -Wshadow -Wconversion -Wsign-conversion -Wcast-qual -Wcast-align -Wstrict-prototypes -Wmissing-prototypes -Wformat=2 -Wundef -Wwrite-strings
STRICT_CFLAGS ?= $(CFLAGS) $(STRICT_WARNINGS) -Werror

TARGET := tlv_test
SRCS := $(wildcard *.c) $(wildcard tests/*.c) $(wildcard eeprom/*.c)
OBJS := $(SRCS:.c=.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all run clean strict

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

strict:
	$(MAKE) clean
	$(MAKE) CFLAGS='$(STRICT_CFLAGS)'

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)

-include $(DEPS)
