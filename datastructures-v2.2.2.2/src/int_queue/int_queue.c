#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <int_queue.h>

/*
 * Implementation of an integer queue for the "Datastructures and
 * algorithms" courses at the Department of Computing Science, Umea
 * University. The queue stores integers directly and does not use
 * dynamic memory. Thus, the clean-up function queue_kill is strictly
 * not necessary, but recommended to maintain symmetry with untyped,
 * generic queue implementations.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *
 * Version information:
 *   v1.0  2025-01-10: First public version.
 */

/**
 * queue_empty() - Create an empty queue.
 *
 * Returns: A new, empty, queue.
 */
queue queue_empty(void)
{
    queue q;
    q.first_free_pos = 0;
    // Set all elements to zero. Not really needed, but otherwise
    // the output from print_internal becomes unreadable.
    for (int i = 0; i < sizeof(q.elements)/sizeof(q.elements[0]); i++) {
        q.elements[i] = 0;
    }
    return q;
}

/**
 * queue_is_empty() - Check if a queue is empty.
 * @q: Queue to check.
 *
 * Returns: True if queue is empty, otherwise false.
 */
bool queue_is_empty(const queue q)
{
    // The queue is empty if no positions are occupied with elements,
    // i.e., the first free position is zero
    return q.first_free_pos == 0;
}

/**
 * queue_enqueue() - Put a value at the end of a queue.
 * @q: Queue to manipulate.
 * @v: Value (integer) to be put in the queue.
 *
 * Returns: The modified queue.
 */
queue queue_enqueue(queue q, int v)
{
    // Store value at first free position.
    q.elements[q.first_free_pos] = v;
    // Update first free position.
    q.first_free_pos++;

    // Return the new queue.
    return q;
}

/**
 * queue_dequeue() - Remove the element at the beginning of a queue.
 * @q: Queue to manipulate.
 *
 * NOTE: Undefined for an empty queue.
 *
 * Returns: The modified queue.
 */
queue queue_dequeue(queue q)
{
    if (queue_is_empty(q)) {
        fprintf(stderr, "queue_dequeue: Warning: dequeue on empty queue\n");
    } else {
	// Shift all elements one step to the left
	for (int i = 0; i < q.first_free_pos - 1; i++) {
	    q.elements[i] = q.elements[i + 1];
	}
	// Update the position of the first free element
        q.first_free_pos--;
    }
    return q;
}

/**
 * queue_front() - Inspect the value at the front of the queue.
 * @q: Queue to inspect.
 *
 * Returns: The value at the front of the queue.
 *              NOTE: The return value is undefined for an empty queue.
 */
int queue_front(const queue q)
{
    if (queue_is_empty(q)) {
        fprintf(stderr, "queue_front: Warning: front on empty queue\n");
    }
    return q.elements[0];
}

/**
 * queue_kill() - Destroy a given queue.
 * @q: Queue to destroy.
 *
 * Does nothing since the queue does not use any dynamic
 * memory. Included for symmetry with generic queue.h.
 *
 * Returns: Nothing.
 */
void queue_kill(queue q)
{
    // Do nothing.
}

/**
 * queue_print() - Iterate over the queue elements and print their values.
 * @q: Queue to inspect.
 *
 * Iterates over the queue and prints each integer.
 *
 * Returns: Nothing.
 */
void queue_print(const queue q)
{
    printf("{ ");
    // Print elements from the top down.
    for (int i = 0; i < q.first_free_pos; i++) {
        printf("[%d]", q.elements[i]);
        if (i < q.first_free_pos - 1) {
            printf(", ");
        }
    }
    printf(" }\n");
}

// ===========INTERNAL FUNCTIONS USED BY queue_print_internal ============

// The functions below output code in the dot language, used by
// GraphViz. For documention of the dot language, see graphviz.org.

/**
 * indent() - Output indentation string.
 * @n: Indentation level.
 *
 * Print n tab characters.
 *
 * Returns: Nothing.
 */
static void indent(int n)
{
    for (int i=0; i<n; i++) {
        printf("\t");
    }
}
/**
 * iprintf(...) - Indent and print.
 * @n: Indentation level
 * @...: printf arguments
 *
 * Print n tab characters and calls printf.
 *
 * Returns: Nothing.
 */
static void iprintf(int n, const char *fmt, ...)
{
    // Indent...
    indent(n);
    // ...and call printf
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

/**
 * print_head_node() - Print a node corresponding to the queue struct.
 * @indent_level: Indentation level.
 * @max_elems: Maximum element to print.
 * @q: Queue to inspect.
 *
 * Returns: Nothing.
 */
static void print_head_node(int indent_level, int max_elems, const queue q)
{
    int elems_to_print = sizeof(q.elements)/sizeof(q.elements[0]);
    if (max_elems < elems_to_print) {
        elems_to_print = max_elems;
    }
    iprintf(indent_level, "q [shape=record label=\"first_free_pos\\n%d", q.first_free_pos);
    for (int i = 0; i < elems_to_print; i++) {
        printf("|%d\\n%d", i, q.elements[i]);
    }
    printf("\" xlabel=\"q\"]\n");
}

// Create an escaped version of the input string. The most common
// control characters - newline, horizontal tab, backslash, and double
// quote - are replaced by their escape sequence. The returned pointer
// must be deallocated by the caller.
static char *escape_chars(const char *s)
{
    int i, j;
    int escaped = 0; // The number of chars that must be escaped.

    // Count how many chars need to be escaped, i.e. how much longer
    // the output string will be.
    for (i = escaped = 0; s[i] != '\0'; i++) {
        if (s[i] == '\n' || s[i] == '\t' || s[i] == '\\' || s[i] == '\"') {
            escaped++;
        }
    }
    // Allocate space for the escaped string. The variable i holds the input
    // length, escaped how much the string will grow.
    char *t = malloc(i + escaped + 1);

    // Copy-and-escape loop
    for (i = j = 0; s[i] != '\0'; i++) {
        // Convert each control character by its escape sequence.
        // Non-control characters are copied as-is.
        switch (s[i]) {
        case '\n': t[i+j] = '\\'; t[i+j+1] = 'n';  j++; break;
        case '\t': t[i+j] = '\\'; t[i+j+1] = 't';  j++; break;
        case '\\': t[i+j] = '\\'; t[i+j+1] = '\\'; j++; break;
        case '\"': t[i+j] = '\\'; t[i+j+1] = '\"'; j++; break;
        default:   t[i+j] = s[i]; break;
        }
    }
    // Terminal the output string
    t[i+j] = '\0';
    return t;
}

// Print edge from the description to the queue.
static void print_desc_edge(int indent_level)
{
    iprintf(indent_level, "description -> q [style=invis] "
            "// Dummy line to place description above\n");
}

/**
 * queue_print_internal() - Print the internal structure of the queue in dot format.
 * @q: Queue to inspect.
 * @desc: String with a description/state of the queue, or NULL for no description.
 * @indent_level: Indentation level, 0 for outermost.
 * @max_elems: Maximum number of elements to print.
 *
 * Iterates over the queue and outputs dot code that shows the
 * internal structure of the queue.  The dot code can be visualized by
 * Graphviz.
 *
 * On linux system, the output can be parsed by the dot program, e.g.
 *
 *   <queue_program> | dot -Tsvg > /tmp/dot.svg; firefox /tmp/dot.svg
 *
 * where <queue_program> is the name of the executable
 *
 * The output may also be possible to visualize online on
 * https://dreampuf.github.io/GraphvizOnline/ or google "graphviz
 * online".
 *
 * For documention of the dot language, see graphviz.org.
 *
 * Returns: Nothing.
 */
void queue_print_internal(const queue q, const char *desc, int indent_level, int max_elems)
{
    static int graph_number = 0;
    graph_number++;
    int il = indent_level;

    if (indent_level == 0) {
        // If this is the outermost datatype, start a graph...
        printf("digraph QUEUE_%d {\n", graph_number);

        // Specify default shape and fontname
        il++;
        iprintf(il, "node [shape=rectangle fontname=\"Courier New\"]\n");
        iprintf(il, "ranksep=0.01\n");
    }

    if (desc != NULL) {
        // Escape the string before printout
        char *escaped = escape_chars(desc);
        // Use different names on inner description nodes
        if (indent_level == 0) {
            iprintf(il, "description [label=\"%s\"]\n", escaped);
        } else {
            iprintf(il, "cluster_queue_%d_description [label=\"%s\"]\n", graph_number, escaped);
        }
        // Return the memory used by the escaped string
        free(escaped);
    }

    // Output the head node
    print_head_node(il, max_elems, q);

    // Output a dummy line to place the description at the top
    print_desc_edge(il);

    if (indent_level == 0) {
        // Termination of graph
        printf("}\n");
    }
}
