all: hello.e

hello.e: pgm.o main.o funcs.o
	gcc -o hello.e main.o pgm.o funcs.o -lm

main.o: main.c
	gcc -o main.o -c main.c

pmg.o: pgm.c
	gcc -o pgm.o -c pgm.c

funcs.o: funcs.c
	gcc -o funcs.o -c funcs.c

clean:
	rm -f *.o hello.e

cleanwin:
	del *.o hello.e