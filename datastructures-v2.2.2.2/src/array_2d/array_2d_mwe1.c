#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <array_2d.h>

/*
 * Minimum working example for array_2d.c.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *
 * Version information:
 *   v1.0  2018-01-28: First public version.
 *   v1.1  2018-04-03: Split into versions with/without memhandler.
 *   v1.2  2023-01-14: Added printouts at start/end of main.
 */

#define VERSION "v1.2"
#define VERSION_DATE "2023-01-14"

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
    printf("%s, %s %s: Create 4-by-3 array of integers without kill function.\n",
           __FILE__, VERSION, VERSION_DATE);
    printf("Code base version %s (%s).\n\n", CODE_BASE_VERSION, CODE_BASE_RELEASE_DATE);

    // Create a 4-by-3 array.
    array_2d *a = array_2d_create(1, 4, 1, 3, NULL);

    for (int i = array_2d_low(a, 1); i <= array_2d_high(a, 1); i++) {
        for (int j = array_2d_low(a, 2); j <= array_2d_high(a, 2); j++) {
            // Allocate memory for an integer.
            int *v=int_create(i * 10 + j);
            array_2d_set_value(a, v, i, j);
        }
    }
    printf("After filling the array with values:\n");
    array_2d_print(a, print_int);

    // Empty the array.
    for (int i = array_2d_low(a, 1); i <= array_2d_high(a, 1); i++) {
        for (int j = array_2d_low(a, 2); j <= array_2d_high(a, 2); j++) {
            if (array_2d_has_value(a, i, j)) {
                int *v=array_2d_inspect_value(a, i, j);
                int_kill(v);
            }
        }
    }
    // Return remaining memory.
    array_2d_kill(a);

    printf("\nNormal exit.\n\n");
    return 0;
}
