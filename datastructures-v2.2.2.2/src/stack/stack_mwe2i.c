#include <stdio.h>
#include <stdlib.h>

#include <stack.h>

/*
 * Minimum working example for stack.c that shows how the internal
 * structure of a stack can be visualized. In this version, the stack
 * "owns" the payload memory, i.e., the stack takes over the
 * responsibility to deallocate the payload memory when the
 * corresponding elements are removed. See stack_mwe1i.c for a version
 * where the stack does not take over the responsibility.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *
 * Version information:
 *   v1.0 2024-03-14: First public version.
 *   v1.1 2024-03-15: Moved printout of dot instructions to separate function.
 */

#define VERSION "v1.1"
#define VERSION_DATE "2024-03-15"

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

// Print cut lines before and after a call stack_print_internal.
void print_internal_with_cut_lines(const stack *s, const char *desc)
{
    // Internal counter that will be remembered between calls.

    // Used to generate sequentially numbered -- CUT HERE -- marker lines
    // to enable automatic parsing of the output.
    static int cut = 1;

    // Print starting marker line.
    printf("\n--- START CUT HERE %d ---\n", cut);

    // Call the internal print function to get the actual dot code.
    stack_print_internal(s, print_int, desc, 0);

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
    printf("%s, %s %s: Create integer stack with kill_function.\n",
           __FILE__, VERSION, VERSION_DATE);
    printf("Code base version %s (%s).\n\n", CODE_BASE_VERSION, CODE_BASE_RELEASE_DATE);

    print_dot_usage(argv[0]);

    // Create the stack.
    stack *s = stack_empty(int_kill);

    printf("Empty stack from the outside:\n");
    stack_print(s, print_int);
    print_internal_with_cut_lines(s, "Empty stack showing the internal structure");

    for (int i=1; i<=3; i++) {
        // Allocate memory for one int.
        int *v = int_create(i);
        // Push value on stack.
        s = stack_push(s, v);
    }

    printf("Stack from the outside after pushing 3 values:\n");
    stack_print(s, print_int);
    const char *long_desc = __FILE__
        ": Internal structure of the Stack after pushing 3 values.\n"
        "Red lines are used for the stack payload.\n\n"
        "The solid red lines indicate that the payload memory is\n"
        "OWNED -- not borrowed -- by the stack, i.e., the payload\n"
        "memory WILL be deallocated by the stack.\n\n"
        "See stack_mwe1i for a stack example\nthat borrows the payload memory.";
    print_internal_with_cut_lines(s, long_desc);

    // Kill the stack. The payload memory is returned automatically.
    stack_kill(s);

    printf("\nNormal exit.\n\n");
    return 0;
}
