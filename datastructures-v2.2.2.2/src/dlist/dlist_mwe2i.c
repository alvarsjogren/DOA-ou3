#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dlist.h>

/*
 * Minimum working example for dlist.c that shows how the internal
 * structure of a list can be visualized. In this version, the list
 * "owns" the payload memory, i.e., the list takes over the
 * responsibility to deallocate the payload memory when the
 * corresponding elements are removed. See list_mwe1i.c for a version
 * where the list does not take over the responsibility.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *
 * Version information:
 *   v1.0 2024-03-14: First public version.
 *   v1.1 2024-03-15: Moved printout of dot instructions to separate function.
 */

#define VERSION "v1.1"
#define VERSION_DATE "2024-03-15"

// Make a dynamic copy of the input string.
char *copy_string(char *str)
{
    char *copy = calloc(strlen(str) + 1, sizeof(char));
    strcpy(copy, str);
    return copy;
}

// Return the memory occupied by the string
void kill_string(void *v)
{
    char *p = v; // Convert to a char ptr, useful in a debugger
    free(p);
}

// String print function.
void print_string(const void *value)
{
    const char *s=value;
    printf("\"%s\"", s);
}

void print_string_internal(const void *value)
{
    const char *s=value;
    // We must double-escape the double quotes since the output will
    // be parsed a second time by graphviz.
    printf("\\\"%s\\\"", s);
}

// Print cut lines before and after a call list_print_internal.
void print_internal_with_cut_lines(const dlist *l, const char *desc)
{
    // Internal counter that will be remembered between calls.

    // Used to generate sequentially numbered -- CUT HERE -- marker lines
    // to enable automatic parsing of the output.
    static int cut = 1;

    // Print starting marker line.
    printf("\n--- START CUT HERE %d ---\n", cut);

    // Call the internal print function to get the actual dot code.
    dlist_print_internal(l, print_string_internal, desc, 0);

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

// Test program.
int main(int argc, char *argv[])
{
    printf("%s, %s %s: Create list of strings without memfreehandler.\n",
           __FILE__, VERSION, VERSION_DATE);
    printf("Code base version %s (%s).\n\n", CODE_BASE_VERSION, CODE_BASE_RELEASE_DATE);

    print_dot_usage(argv[0]);

    // Names to insert in the list.
    char *names[] = {"Alfons", "Bengt", "Cia", "David", "Florian", "Gunnar"};

    // Create the list
    dlist *l = dlist_empty(kill_string);

    printf("Empty list from the outside:\n");
    dlist_print(l, print_string);
    print_internal_with_cut_lines(l, "Empty list showing the internal structure");

    dlist_pos p = dlist_first(l);
    for (int i = 0; i < sizeof(names) / sizeof(names[0]); i++) {
        // Insert dynamic copies of strings at last position.
        p = dlist_insert(l, copy_string(names[i]), p);
        p = dlist_next(l, p);
    }
    // Print the list.
    printf("DList from the outside after inserting 6 strings:\n");
    dlist_print(l, print_string);
    const char *long_desc = __FILE__
        ": Internal structure of the DList after inserting 6 strings.\n"
        "Red lines are used for the list payload.\n\n"
        "The solid red lines indicate that the payload memory is\n"
        "OWNED -- not borrowed -- by the list, i.e., the payload\n"
        "memory WILL be deallocated by the list.\n\n"
        "See dlist_mwe1i for a list example\nthat borrows the payload memory.";
    print_internal_with_cut_lines(l, long_desc);

    // Destroy the list.
    dlist_kill(l);

    printf("\nNormal exit.\n\n");
    return 0;
}
