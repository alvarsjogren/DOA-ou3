#include <stdio.h>
#include <stdlib.h>

#include <int_stack.h>

/*
 * Minimum working example for int_stack.c.
 *
 * Author: Niclas Borlin (niclas@cs.umu.se)
 *
 * Version information:
 *   v1.0 2024-04-10: Adapted from int_stack_mwe1.c.
 */

#define VERSION "v1.0"
#define VERSION_DATE "2024-04-10"

// Print cut lines before and after a call array_print_internal.
void print_internal_with_cut_lines(stack s, const char *desc)
{
    // Internal counter that will be remembered between calls.

    // Used to generate sequentially numbered -- CUT HERE -- marker lines
    // to enable automatic parsing of the output.
    static int cut = 1;

    // Print starting marker line.
    printf("\n--- START CUT HERE %d ---\n", cut);

    // Call the internal print function to get the actual dot code.
    stack_print_internal(s, desc, 0, 10);

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
    printf("%s, %s %s: Create typed integer stack.\n", __FILE__, VERSION, VERSION_DATE);
    printf("Code base version %s (%s).\n\n", CODE_BASE_VERSION, CODE_BASE_RELEASE_DATE);

    print_dot_usage(argv[0]);

    // Create the stack.
    stack s=stack_empty();

    printf("Empty stack from the outside:\n");
    stack_print(s);
    print_internal_with_cut_lines(s, "Empty stack showing the inside");

    for (int i=1; i<=3; i++) {
        // Push value on stack.
        s = stack_push(s, 10 + i);
    }

    printf("Stack for the outside after pushing 3 elements:\n");
    stack_print(s);
    print_internal_with_cut_lines(s, "Inside of the stack after pushing 3 elements");

    // Pop one element from stack.
    s = stack_pop(s);

    printf("Stack for the outside after popping 1 element:\n");
    stack_print(s);
    print_internal_with_cut_lines(s, "Inside of the stack after popping 1 element");

    // Pop remaining elements
    while (!stack_is_empty(s)) {
        s = stack_pop(s);
    }

    printf("Stack from the outside after popping all elements:\n");
    stack_print(s);
    print_internal_with_cut_lines(s, "Inside of the stack after popping all elements");

    // Kill stack. Strictly not necessary for int_stack since it
    // does not use any dynamic memory. Included for symmetry with
    // stack_mwe1.c.
    stack_kill(s);

    printf("\nNormal exit.\n\n");
    return 0;
}
