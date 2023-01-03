CC = gcc
CFLAGS = -o

all: main

main: main.s
	$(CC) -g -no-pie $(CFLAGS) main main.s
clean:
	rm -f main all

