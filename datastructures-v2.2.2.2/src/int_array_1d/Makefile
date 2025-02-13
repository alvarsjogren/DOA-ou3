MWE = int_array_1d_mwe1 int_array_1d_mwe1i

SRC = int_array_1d.c
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

int_array_1d_mwe1: int_array_1d_mwe1.c int_array_1d.c
	gcc -o $@ $(CFLAGS) $^

int_array_1d_mwe1i: int_array_1d_mwe1i.c int_array_1d.c
	gcc -o $@ $(CFLAGS) $^

memtest: int_array_1d_mwe1
	valgrind --leak-check=full --show-reachable=yes ./$<
