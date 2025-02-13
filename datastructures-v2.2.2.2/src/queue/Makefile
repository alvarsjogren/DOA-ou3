MWE = queue_mwe1 queue_mwe2 queue_mwe1i queue_mwe2i queue_example queue_example_internal

SRC = queue.c
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

queue_example_internal: queue_example_internal.c queue.c ../list/list.c
	gcc -o $@ $(CFLAGS) $^

queue_example: queue_example.c queue.c ../list/list.c
	gcc -o $@ $(CFLAGS) $^

queue_mwe1: queue_mwe1.c queue.c ../list/list.c
	gcc -o $@ $(CFLAGS) $^

queue_mwe2: queue_mwe2.c queue.c ../list/list.c
	gcc -o $@ $(CFLAGS) $^

queue_mwe1i: queue_mwe1i.c queue.c ../list/list.c
	gcc -o $@ $(CFLAGS) $^

queue_mwe2i: queue_mwe2i.c queue.c ../list/list.c
	gcc -o $@ $(CFLAGS) $^

memtest1: queue_mwe1
	valgrind --leak-check=full --show-reachable=yes ./$<

memtest2: queue_mwe2
	valgrind --leak-check=full --show-reachable=yes ./$<

memtest3: queue_mwe1i
	valgrind --leak-check=full --show-reachable=yes ./$<

memtest4: queue_mwe2i
	valgrind --leak-check=full --show-reachable=yes ./$<
