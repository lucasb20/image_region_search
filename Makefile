all: hello.e

hello.e: pmg.o main.o funcs.o
	gcc -o hello.e main.o pmg.o funcs.o -lm

main.o: main.c
	gcc -o main.o -c main.c

pmg.o: pmg_mod.c
	gcc -o pmg.o -c pmg_mod.c

funcs.o: funcs.c
	gcc -o funcs.o -c funcs.c

clean:
	rm -f *.o hello.e