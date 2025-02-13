MWE = list_mwe1 list_mwe2 list_mwe1i list_mwe2i
TEST = list_test1 list_test2

SRC = list.c
OBJ = $(SRC:.c=.o)

CC = gcc
CFLAGS = -std=c99 -Wall -I../../include -g

all:	mwe test

# Minimum working examples.
mwe:	$(MWE)

# Object file for library
obj:	$(OBJ)

test: list_test1 list_test2

# Clean up
clean:
	-rm -f $(MWE) $(TEST) $(OBJ)

list_mwe1: list_mwe1.c list.c
	gcc -o $@ $(CFLAGS) $^

list_mwe2: list_mwe2.c list.c
	gcc -o $@ $(CFLAGS) $^

list_mwe1i: list_mwe1i.c list.c
	gcc -o $@ $(CFLAGS) $^

list_mwe2i: list_mwe2i.c list.c
	gcc -o $@ $(CFLAGS) $^

memtest_mwe1: list_mwe1
	valgrind --leak-check=full --show-reachable=yes ./$<

memtest_mwe2: list_mwe2
	valgrind --leak-check=full --show-reachable=yes ./$<

memtest_mwe1i: list_mwe1i
	valgrind --leak-check=full --show-reachable=yes ./$<

memtest_mwe2i: list_mwe2i
	valgrind --leak-check=full --show-reachable=yes ./$<

list_test1: list_test1.c list.c
	gcc -o $@ $(CFLAGS) $^

list_test2: list_test2.c list.c
	gcc -o $@ $(CFLAGS) $^

test_run1: list_test1
	# Run the test
	./$<

test_run2: list_test2
	# Run the test
	./$<

memtest_test1: list_test1
	valgrind --leak-check=full --show-reachable=yes ./$<

memtest_test2: list_test2
	valgrind --leak-check=full --show-reachable=yes ./$<

memtests: memtest_mwe1 memtest_mwe2 memtest_mwe1i memtest_mwe2i memtest_test1 memtest_test2
