CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O0 -g
LDFLAGS ?=

TARGET := tlv_test
SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
