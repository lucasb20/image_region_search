CC = g++
CFLAGS = -Wall -Wextra -Wpedantic -std=c++17
LDFLAGS = -lm

SRCS = main.cpp pgm.cpp funcs.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = hello

main.o: main.c
	gcc -o main.o -c main.c

pmg.o: pgm.c
	gcc -o pgm.o -c pgm.c

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

DEBUG_MODE: CFLAGS += -g
DEBUG_MODE: all

clean:
	rm -f *.o hello.e

cleanwin:
	del *.o hello.e