#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <array_1d.h>

/*
 * Minimum working example 1 for array_1d.c.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *
 * Version information:
 *   v1.0  2024-04-05: Adapted from array_1d_mwe1.c.
 */

#define VERSION "v1.0"
#define VERSION_DATE "2024-04-05"

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

// Print cut lines before and after a call array_print_internal.
void print_internal_with_cut_lines(const array_1d *a, const char *desc)
{
    // Internal counter that will be remembered between calls.

    // Used to generate sequentially numbered -- CUT HERE -- marker lines
    // to enable automatic parsing of the output.
    static int cut = 1;

    // Print starting marker line.
    printf("\n--- START CUT HERE %d ---\n", cut);

    // Call the internal print function to get the actual dot code.
    array_1d_print_internal(a, print_int, desc, 0);

    // Print ending marker line
    printf("--- END CUT HERE %d ---\n\n", cut);

    // Increment the cut number. Will be remembered next time the
    // function is called since cut is a static variable.
    cut++;
}

// Print a message with intructions how to use the dot output. prog is
// the name of the executable.
void print_dot_usage(char *prog)
{
    printf("\nGenerate dot code to visualize internal structure with GraphViz. ");
    printf("Use\n\n%s ", prog);
    printf("| sed -n '/START CUT HERE X/,/END CUT HERE X/{//!p}' | dot -Tsvg > /tmp/dot.svg\n\n");
    printf("to generate an svg file of cut X (replace X by the requested cut number).\n");
    printf("The generated file can then be visualized with\n\n");
    printf("firefox /tmp/dot.svg\n\n");
    printf("Use -Tpng to generate a .png file instead. "
           "See graphviz.org and %s for documentation.\n", __FILE__);

    printf("\n--- Start of normal output ---\n\n");
}

int main(int argc, char *argv[])
{
    printf("%s, %s %s: Create integer array without kill_function.\n",
           __FILE__, VERSION, VERSION_DATE);
    printf("Code base version %s (%s).\n\n", CODE_BASE_VERSION, CODE_BASE_RELEASE_DATE);

    print_dot_usage(argv[0]);

    // Create an array with 6 positions.
    array_1d *a = array_1d_create(1, 6, NULL);

    printf("Empty array from the outside:\n");
    array_1d_print(a, print_int);
    print_internal_with_cut_lines(a, "Empty array showing the internal structure");

    int i;

    // Set values of 50% of the elements
    for (i = array_1d_low(a); i <= array_1d_high(a)/2; i++) {
        // Allocate memory for an integer.
        int *v=int_create(i * i);
        array_1d_set_value(a,v,i);
    }
    printf("Array from the outside after setting half the values:\n");
    array_1d_print(a, print_int);
    const char *long_desc = __FILE__
        ": Internal structure of the Array after setting 3 values.\n"
        "Red lines are used for the array payload.\n\n"
        "The dashed red lines indicate that the payload memory is\n"
        "BORROWED by the array, i.e., the payload\n"
        "memory will NOT be deallocated by the array.\n\n"
        "See array_1d_mwe2i for an array example\nthat owns the payload memory.";
    print_internal_with_cut_lines(a, long_desc);

    // Set the rest of the element values.
    // Note: The empty initialization is on purpose.
    for ( ; i <= array_1d_high(a) ; i++) {
        // Allocate memory for an integer.
        int *v=int_create(i * i);
        array_1d_set_value(a,v,i);
    }

    printf("Array from the outside after setting all the values:\n");
    array_1d_print(a, print_int);
    const char *long_desc2 = __FILE__
        ": Internal structure of the Array after setting all 6 values.\n"
        "Red lines are used for the array payload.\n\n"
        "The dashed red lines indicate that the payload memory is\n"
        "BORROWED by the array, i.e., the payload\n"
        "memory will NOT be deallocated by the array.\n\n"
        "See array_1d_mwe2i for an array example\nthat owns the payload memory.";
    print_internal_with_cut_lines(a, long_desc2);

    // Empty the array.
    for (int i=array_1d_low(a); i<=array_1d_high(a); i++) {
        if (array_1d_has_value(a, i)) {
            int *v=array_1d_inspect_value(a,i);
            int_kill(v);
        }
    }
    array_1d_kill(a);

    printf("\nNormal exit.\n\n");
    return 0;
}
