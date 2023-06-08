CC = g++
CFLAGS = -o
OBJECTS = shell.o

all: $(OBJECTS) shell

shell.o: shell.cc
	$(CC) -c shell.cc
shell:	shell.o
	$(CC) $(CFLAGS) shell $(OBJECTS)
clean:
	rm -f $(OBJECTS) shell
