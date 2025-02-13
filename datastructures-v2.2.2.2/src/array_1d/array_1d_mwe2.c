#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <array_1d.h>

/*
 * Minimum working example 2 for array_1d.c.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *
 * Version information:
 *   v1.0  2018-01-28: First public version.
 *   v1.1  2018-04-03: Split into simpler, shorter versions.
 *   v1.2  2023-01-14: Added printouts at start/end of main.
 *   v1.3  2024-03-13: Added explicit create/kill functions.
 */

#define VERSION "v1.3"
#define VERSION_DATE "2024-03-13"

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
    int *p = v;
    free(p);
}

int main(void)
{
    printf("%s, %s %s: Create integer array with kill_function.\n",
           __FILE__, VERSION, VERSION_DATE);
    printf("Code base version %s (%s).\n\n", CODE_BASE_VERSION, CODE_BASE_RELEASE_DATE);

    // Create an array with 6 positions.
    array_1d *a = array_1d_create(1, 6, int_kill);

    for (int i=array_1d_low(a); i<=array_1d_high(a); i++) {
        // Allocate memory for an integer.
        int *v=int_create(i * i);
        array_1d_set_value(a, v, i);
    }
    printf("Array after inserting i^2, i=1,...,6:\n");
    array_1d_print(a, print_int);

    array_1d_kill(a);

    printf("\nNormal exit.\n\n");
    return 0;
}
