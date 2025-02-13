#include <stdio.h>
#include <stdlib.h>

#include <stack.h>

/*
 * Minimum working example for stack.c.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *          Adam Dahlgren Lindstrom (dali@cs.umu.se)
 *
 * Version information:
 *   v1.0 2018-01-28: First public version.
 *   v1.1 2018-04-03: Split into versions with/without memhandler.
 *   v1.2 2022-03-24: Update to always take care of returned pointer.
 *   v1.3 2023-01-14: Added printouts at start/end of main.
 *   v1.4 2024-03-14: Added explicit create/kill functions.
 */

#define VERSION "v1.4"
#define VERSION_DATE "2024-03-14"

// Integers are stored via int pointers stored as void pointers.
// Convert the given pointer and print the dereferenced value.
void print_int(const void *data)
{
    const int *v = data;
    printf("%d", *v);
}

// Create a dynamic copy of the integer i.
int *int_create(int i)
{
    // Allocate memory for an integer and set the value
    int *v = malloc(sizeof(*v));
    *v = i;
    return v;
}

// Return the memory used by the integer.
void int_kill(void *v)
{
    int *p = v; // Convert to a readable pointer - useful in a debugger
    free(p);
}

int main(void)
{
    printf("%s, %s %s: Create integer stack with kill_function.\n",
           __FILE__, VERSION, VERSION_DATE);
    printf("Code base version %s (%s).\n\n", CODE_BASE_VERSION, CODE_BASE_RELEASE_DATE);

    // Create the stack. Make the stack responsible for
    // deallocation pushed values.
    stack *s = stack_empty(int_kill);

    for (int i=1; i<=3; i++) {
        // Allocate memory for one int.
        int *v = int_create(i);
        // Push value on stack.
        s = stack_push(s, v);
    }

    printf("--STACK before popping--\n");
    stack_print(s, print_int);

    // Pop'ing will automatically deallocate the payload
    s = stack_pop(s);

    printf("--STACK after popping--\n");
    stack_print(s, print_int);

    // Killing will automatically deallocate the payload
    stack_kill(s);

    printf("\nNormal exit.\n\n");
    return 0;
}
