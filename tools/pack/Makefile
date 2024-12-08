# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -O2

# Target executable name
TARGET = pack

# Source files
SRCS = pack.c

# Object files (derived from sources)
OBJS = $(SRCS:.c=.o)

# Default rule: build the executable
all: $(TARGET)

# Rule to link the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to compile the source files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
