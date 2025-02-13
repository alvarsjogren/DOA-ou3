#include <stdlib.h>
#include <stdio.h>

#include <int_list.h>

/*
 * Minimum working example for int_list.c. Create a list, insert two
 * elements, print list, free list.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *          Adam Dahlgren Lindstrom (dali@cs.umu.se)
 *
 * Version information:
 *   v1.0  2018-01-28: First public version.
 *   v1.1  2023-01-14: Added printouts at start/end of main.
 *   v1.11 2024-01-16: Fix include to be with brackets, not citation marks.
 */

#define VERSION "v1.11"
#define VERSION_DATE "2024-01-16"

// Print cut lines before and after a call list_print_internal.
void print_internal_with_cut_lines(const list *l, const char *desc)
{
    // Internal counter that will be remembered between calls.

    // Used to generate sequentially numbered -- CUT HERE -- marker lines
    // to enable automatic parsing of the output.
    static int cut = 1;

    // Print starting marker line.
    printf("\n--- START CUT HERE %d ---\n", cut);

    // Call the internal print function to get the actual dot code.
    list_print_internal(l, desc, 0);

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

int main(void)
{
    printf("%s, %s %s: Create typed integer list.\n",
           __FILE__, VERSION, VERSION_DATE);
    printf("Code base version %s (%s).\n\n", CODE_BASE_VERSION, CODE_BASE_RELEASE_DATE);

    // Create the list.
    list *l = list_empty();

    printf("Empty list from the outside.\n");
    list_print(l);
    print_internal_with_cut_lines(l, "Empty list showing the internals");

    // Insert the value 5 first in the list.
    list_insert(l, 5, list_first(l));

    printf("List from the outside after inserting one value:\n");
    list_print(l);
    print_internal_with_cut_lines(l, "List after inserting one value showing the internals");

    // Insert the value 8 last in the list.
    list_insert(l, 8, list_end(l));

    printf("List from the outside after inserting second value at the end:\n");
    list_print(l);
    print_internal_with_cut_lines(l, "List after inserting second value at the end showing the internals");

    printf("List after removing first element:\n");
    list_print(l);
    print_internal_with_cut_lines(l, "List after removing first element showing the internals");

    // Done, kill the list.
    list_kill(l);

    printf("\nNormal exit.\n\n");
    return 0;
}
