CC = gcc
CFLAGS = -g -Wall

all: greet memgrind

greet:
	@echo "Now running MAKEFILE.."

memgrind: mymalloc.c memgrind.c
	@echo "Compiling mymalloc.c..."
	$(CC) $(CFLAGS) -c mymalloc.c
	@echo "Creating executable for memgrind..."
	$(CC) $(CFLAGS) -o memgrind memgrind.c mymalloc.o
clean:
	@echo "Cleaning up..."
	rm -f mymalloc.o memgrind