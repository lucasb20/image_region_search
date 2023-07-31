CC = g++
CFLAGS = -Wall -Wextra -Wpedantic -std=c++17
LDFLAGS = -lm

SRCS = main.cpp pgm.cpp funcs.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = hello

.PHONY: all clean DEBUG_MODE

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

DEBUG_MODE: CFLAGS += -g
DEBUG_MODE: all

clean:
	rm -f $(OBJS) $(TARGET)