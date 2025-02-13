MWE = int_list_array_mwe1 int_list_array_mwe1i
TEST = int_list_array_test

SRC = int_list_array.c
OBJ = $(SRC:.c=.o)

CC = gcc
CFLAGS = -std=c99 -Wall -I../../include -g

all:	mwe test

# Minimum working examples.
mwe:	$(MWE)

# Test programs.
test:	$(TEST)

# Object file for library
obj:	$(OBJ)

# Clean up
clean:
	-rm -f $(MWE) $(TEST) $(OBJ)

int_list_array_mwe1: int_list_array_mwe1.c int_list_array.c
	gcc -o $@ $(CFLAGS) $^

int_list_array_mwe1i: int_list_array_mwe1i.c int_list_array.c
	gcc -o $@ $(CFLAGS) $^

int_list_array_test: int_list_array_test.c int_list_array.c
	gcc -o $@ $(CFLAGS) $^

test_run: test
	# Run the test
	./$(TEST)

memtest_mwe1: int_list_array_mwe1
	valgrind --leak-check=full --show-reachable=yes ./$<

memtest_test: int_list_array_test
	valgrind --leak-check=full --show-reachable=yes ./$<

memtest: memtest_mwe1 memtest_test
