CC = gcc
CFLAGS = -o
OBJECTS = main.o

all: $(OBJECTS) main

main.o: List.h main.c
	cc -c main.c
main: main.o
	cc $(CFLAGS) main $(OBJECTS)
clean: 
	rm -f main $(OBJECTS) 
