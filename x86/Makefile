CC = gcc
CFLAGS = -o
OBJECTS = main.o 

all: $(OBJECTS) main
main.o: main.c
	$(CC) -c main.c
main: main.o 
	$(CC) $(CFLAGS) main $(OBJECTS) power.s
clean: 
	rm -f main $(OBJECTS) 

