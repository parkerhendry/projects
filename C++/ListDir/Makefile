CC = g++
CFLAGS = -o
OBJECTS = listdir.o

all: $(OBJECTS) listdir

listdir.o: listdir.cc
	$(CC) -c listdir.cc
listdir:	listdir.o
	$(CC) $(CFLAGS) listdir $(OBJECTS)
clean:
	rm -f $(OBJECTS) listdir
