#include <stdio.h>
#include <int_array_1d.h>

/*
 * Minimum working example for int_array_1d.c.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *
 * Version information:
 *   v1.0 2024-04-08: Adapted from int_array_1d_mwe1.c.
 */

#define VERSION "v1.0"
#define VERSION_DATE "2024-04-08"

// Print cut lines before and after a call array_print_internal.
void print_internal_with_cut_lines(const int_array_1d *a, const char *desc)
{
    // Internal counter that will be remembered between calls.

    // Used to generate sequentially numbered -- CUT HERE -- marker lines
    // to enable automatic parsing of the output.
    static int cut = 1;

    // Print starting marker line.
    printf("\n--- START CUT HERE %d ---\n", cut);

    // Call the internal print function to get the actual dot code.
    int_array_1d_print_internal(a, desc, 0);

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
    printf("%s, %s %s: Create typed integer array.\n",
           __FILE__, VERSION, VERSION_DATE);
    printf("Code base version %s (%s).\n\n", CODE_BASE_VERSION, CODE_BASE_RELEASE_DATE);

    print_dot_usage(argv[0]);

    // Create an array with 6 positions.
    int_array_1d *a = int_array_1d_create(1, 6);

    printf("Empty array from the outside:\n");
    int_array_1d_print(a);
    print_internal_with_cut_lines(a, "Empty array");

    for (int i=int_array_1d_low(a); i<=int_array_1d_high(a); i++) {
        // Store square of index.
        int_array_1d_set_value(a, i*i, i);
    }
    // Print array after setting all values.
    printf("Full array from the outside:\n");
    int_array_1d_print(a);
    print_internal_with_cut_lines(a, "Full array from the inside");

    int_array_1d_kill(a);

    printf("\nNormal exit.\n\n");
    return 0;
}
