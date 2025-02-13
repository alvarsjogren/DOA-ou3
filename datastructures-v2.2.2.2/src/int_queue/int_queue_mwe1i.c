#include <stdio.h>
#include <stdlib.h>

#include <int_queue.h>

/*
 * Minimum working example for int_queue.c.
 *
 * Author: Niclas Borlin (niclas@cs.umu.se)
 *
 * Version information:
 *   v1.0 2025-01-10: Adapted from int_queue_mwe1.c.
 */

#define VERSION "v1.0"
#define VERSION_DATE "2025-01-10"

// Print cut lines before and after a call array_print_internal.
void print_internal_with_cut_lines(queue s, const char *desc)
{
    // Internal counter that will be remembered between calls.

    // Used to generate sequentially numbered -- CUT HERE -- marker lines
    // to enable automatic parsing of the output.
    static int cut = 1;

    // Print starting marker line.
    printf("\n--- START CUT HERE %d ---\n", cut);

    // Call the internal print function to get the actual dot code.
    queue_print_internal(s, desc, 0, 10);

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
    printf("%s, %s %s: Create typed integer queue.\n",
           __FILE__, VERSION, VERSION_DATE);
    printf("Code base version %s (%s).\n\n", CODE_BASE_VERSION, CODE_BASE_RELEASE_DATE);

    print_dot_usage(argv[0]);

    // Create the queue.
    queue q=queue_empty();

    printf("Empty queue from the outside:\n");
    queue_print(q);
    print_internal_with_cut_lines(q, "Empty queue showing the inside");

    for (int i = 11; i <= 13; i++) {
        // Put values in the queue.
        q = queue_enqueue(q, i);
    }

    printf("Queue for the outside after adding 3 elements:\n");
    queue_print(q);
    print_internal_with_cut_lines(q, "Inside of the queue after adding 3 elements");

    // Remove one element from queue.
    q = queue_dequeue(q);

    printf("Queue for the outside after removing 1 element:\n");
    queue_print(q);
    print_internal_with_cut_lines(q, "Inside of the queue after removing 1 element");

    // Pop remaining elements
    while (!queue_is_empty(q)) {
        q = queue_dequeue(q);
    }

    printf("Queue from the outside after removing all elements:\n");
    queue_print(q);
    print_internal_with_cut_lines(q, "Inside of the queue after removing all elements");

    // Kill queue. Strictly not necessary for int_queue since it
    // does not use any dynamic memory. Included for symmetry with
    // queue_mwe1.c.
    queue_kill(q);

    printf("\nNormal exit.\n\n");
    return 0;
}
