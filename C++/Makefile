CC = g++
CFLAGS = -o
OBJECTS = main.o

all: $(OBJECTS) main

main.o: Tree.h Tree.cc main.cc
	$(CC) -c main.cc
main: main.o
	$(CC) $(CFLAGS) main $(OBJECTS)
clean: 
	rm -f main $(OBJECTS) 
