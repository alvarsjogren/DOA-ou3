MWE = int_queue_mwe1 int_queue_mwe1i int_queue_example int_queue_example_internal

SRC = int_queue.c
OBJ = $(SRC:.c=.o)

CC = gcc
CFLAGS = -std=c99 -Wall -I../../include -g

all:	mwe

# Minimum working examples.
mwe:	$(MWE)

# Object file for library
obj:	$(OBJ)

# Clean up
clean:
	-rm -f $(MWE) $(OBJ)

int_queue_mwe1: int_queue_mwe1.c int_queue.c
	gcc -o $@ $(CFLAGS) $^

int_queue_mwe1i: int_queue_mwe1i.c int_queue.c
	gcc -o $@ $(CFLAGS) $^

int_queue_example: int_queue_example.c int_queue.c
	gcc -o $@ $(CFLAGS) $^

int_queue_example_internal: int_queue_example_internal.c int_queue.c
	gcc -o $@ $(CFLAGS) $^

memtest1: int_queue_mwe1
	valgrind --leak-check=full --show-reachable=yes ./$<
