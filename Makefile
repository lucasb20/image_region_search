CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c99
LDFLAGS = -lm

SRCS = main.c pgm.c funcs.c
OBJS = $(SRCS:.c=.o)
TARGET = hello

.PHONY: all clean DEBUG_MODE

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

DEBUG_MODE: CFLAGS += -g
DEBUG_MODE: all

clean:
	rm -f $(OBJS) $(TARGET)