#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <int_array_1d.h>

/*
 * Implementation of a generic 1D array for the "Datastructures and
 * algorithms" courses at the Department of Computing Science, Umea
 * University.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *          Adam Dahlgren Lindstrom (dali@cs.umu.se)
 *          Lars Karlsson (larsk@cs.umu.se)
 *
 * Based on earlier code by: Johan Eliasson (johane@cs.umu.se).
 *
 * Version information:
 *   v1.0  2018-01-28: First public version.
 *   v1.1  2024-05-10: Added print_internal.
 */

// =========== INTERNAL DATA TYPES ============

struct int_array_1d {
    int low; // Low index limit.
    int high; // High index limit.
    int array_size; // Number of array elements.
    int *values; // Pointer to where the actual values are stored.
};

// =========== INTERNAL FUNCTION IMPLEMENTATIONS ============

/**
 * int_array_1d_create() - Create an array without values.
 * @lo: low index limit.
 * @hi: high index limit.
 *
 * The index limits are inclusive, i.e. all indices i such that low <=
 * i <= high are defined.
 *
 * Returns: A pointer to the new array, or NULL if not enough memory
 * was available.
 */
int_array_1d *int_array_1d_create(int lo, int hi)
{
    // Allocate array structure.
    int_array_1d *a = calloc(1, sizeof(*a));
    // Store index limit.
    a->low = lo;
    a->high = hi;

    // Number of elements.
    a->array_size = hi - lo + 1;

    a->values=calloc(a->array_size, sizeof(a->values[0]));

    // Check whether the allocation succeeded.
    if (a->values == NULL) {
        free(a);
        a = NULL;
    }
    return a;
}

/**
 * int_array_1d_low() - Return the low index limit for the array.
 * @a: array to inspect.
 *
 * Returns: The low index limit.
 */
int int_array_1d_low(const int_array_1d *a)
{
    return a->low;
}

/**
 * int_array_1d_high() - Return the high index limit for the array.
 * @a: array to inspect.
 *
 * Returns: The high index limit.
 */
int int_array_1d_high(const int_array_1d *a)
{
    return a->high;
}

/**
 * int_array_1d_inspect_value() - Inspect a value at a given array position.
 * @a: array to inspect.
 * @i: index of position to inspect.
 *
 * Returns: The element value at the specified position. The result is
 *          undefined if no value are stored at that position.
 */
int int_array_1d_inspect_value(const int_array_1d *a, int i)
{
    int offset=i - int_array_1d_low(a);
    // Return the value.
    return a->values[offset];
}

/**
 * int_array_1d_has_value() - Check if a value is set at a given array position.
 * @a: array to inspect.
 * @i: index of position to inspect.
 *
 * Returns: True if a value is set at the specified position, otherwise false.
 */
bool int_array_1d_has_value(const int_array_1d *a, int i)
{
    int offset=i - int_array_1d_low(a);
    // Return true if the value is not 0.
    return a->values[offset] != 0;
}

/**
 * int_array_1d_set_value() - Set a value at a given array position.
 * @a: array to modify.
 * @v: value to set element to, or 0 to clear value.
 * @i: index of position to modify.
 *
 * Returns: Nothing.
 */
void int_array_1d_set_value(int_array_1d *a, int v, int i)
{
    int offset=i - int_array_1d_low(a);
    // Set value.
    a->values[offset] = v;
}

/**
 * int_array_1d_kill() - Return memory allocated by array.
 * @a: array to kill.
 *
 * Returns: Nothing.
 */
void int_array_1d_kill(int_array_1d *a)
{
    // Free actual storage.
    free(a->values);
    // Free array structure.
    free(a);
}

/**
 * int_array_1d_print() - Iterate over the array element and print their values.
 * @a: Array to inspect.
 * @print_func: Function called for each non-NULL element.
 *
 * Iterates over each position in the array. Calls print_func for each
 * non-NULL value.
 *
 * Returns: Nothing.
 */
void int_array_1d_print(const int_array_1d *a)
{
    printf("[ ");
    for (int i=int_array_1d_low(a); i<=int_array_1d_high(a); i++) {
        if (int_array_1d_has_value(a, i)) {
            printf("[%d]", int_array_1d_inspect_value(a, i));
        } else {
            printf(" []");
        }
        if (i<int_array_1d_high(a)) {
            printf(", ");
        }
    }
    printf(" ]\n");
}

// ===========INTERNAL FUNCTIONS USED BY int_array_1d_print_internal ============

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
 * print_edge() - Print a edge between two addresses.
 * @from: The address of the start of the edge. Should be non-NULL.
 * @to: The address of the destination for the edge, including NULL.
 * @port: The name of the port on the source node, or NULL.
 * @label: The label for the edge, or NULL.
 * @options: A string with other edge options, or NULL.
 *
 * Print an edge from port PORT on node FROM to TO with label
 * LABEL. If to is NULL, the destination is the NULL node, otherwise a
 * memory node. If the port is NULL, the edge starts at the node, not
 * a specific port on it. If label is NULL, no label is used. The
 * options string, if non-NULL, is printed before the label.
 *
 * Returns: Nothing.
 */
static void print_edge(int indent_level, const void *from, const void *to, const char *port,
                       const char *label, const char *options)
{
    indent(indent_level);
    if (port) {
        printf("m%04lx:%s -> ", PTR2ADDR(from), port);
    } else {
        printf("m%04lx -> ", PTR2ADDR(from));
    }
    if (to == NULL) {
        printf("NULL");
    } else {
        printf("m%04lx", PTR2ADDR(to));
    }
    printf(" [");
    if (options != NULL) {
        printf("%s", options);
    }
    if (label != NULL) {
        printf(" label=\"%s\"",label);
    }
    printf("]\n");
}

/**
 * print_head_node() - Print a node corresponding to the array head struct.
 * @indent_level: Indentation level.
 * @a: Array to inspect.
 *
 * Returns: Nothing.
 */
static void print_head_node(int indent_level, const int_array_1d *a)
{
    iprintf(indent_level, "m%04lx [shape=record label=\"low\\n%d|high\\n%d"
            "|array_size\\n%d|<v>values\\n%04lx\"]\n", PTR2ADDR(a),
            a->low, a->high, a->array_size, PTR2ADDR(a->values));
}

/**
 * print_values() - Print a node containing all positions in the array.
 * @indent_level: Indentation level.
 * @a: Array to inspect.
 * @max_values_to_print: Maximum number of values to output.
 *
 * Will print dot code to display each value in the array, up to
 * max_values_to_print. If there are more values to print, adds an
 * ellipsis (...) at the end.
 *
 * Returns: Nothing.
 */
void print_values(int indent_level, const int_array_1d *a, int max_values_to_print)
{
    int values_to_print = a->array_size;
    int truncated = false;
    if (values_to_print > max_values_to_print) {
        values_to_print = max_values_to_print;
        truncated = true;
    }
    iprintf(indent_level, "m%04lx [shape=record label=\"", PTR2ADDR(a->values));
    for (int i=0; i < values_to_print; i++) {
        printf("%d\\n%02d\\n%d", i + a->low, i, a->values[i]);
        if (i < values_to_print - 1) {
            printf("|");
        }
    }
    if (truncated) {
        // Add ellipsis
        printf("|\\n...\\n");
    }
    printf("\"]\n");
}

// Print edge from the array head to the values array.
static void print_head_edge(int indent_level, const int_array_1d *a)
{
    print_edge(indent_level, a, a->values, "v", "values", NULL);
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

/**
 * int_array_1d_print_internal() - Print the arrays internal structure in dot format.
 * @a: Array to inspect.
 * @desc: String with a description/state of the array, or NULL for no description.
 * @indent_level: Indentation level, 0 for outermost
 *
 * Iterates over the array and outputs dot code that shows the internal
 * structure of the array.  The dot code can be visualized by
 * Graphviz.
 *
 * On linux system, the output can be parsed by the dot program, e.g.
 *
 *   <array_program> | dot -Tsvg > /tmp/dot.svg; firefox /tmp/dot.svg
 *
 * where <array_program> is the name of the executable
 *
 * The output may also be possible to visualize online on
 * https://dreampuf.github.io/GraphvizOnline/ or google "graphviz
 * online".
 *
 * For documention of the dot language, see graphviz.org.
 *
 * Returns: Nothing.
 */
void int_array_1d_print_internal(const int_array_1d *a, const char *desc, int indent_level)
{
    static int graph_number = 0;
    graph_number++;
    int il = indent_level;

    if (indent_level == 0) {
        // If this is the outermost datatype, start a graph...
        printf("digraph ARRAY_1D_%d {\n", graph_number);

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
            iprintf(il, "cluster_int_array_1d_%d_description [label=\"%s\"]\n", graph_number,
                    escaped);
        }
        // Return the memory used by the escaped string
        free(escaped);
    }

    if (indent_level == 0) {
        // Use a single "pointer" edge as a starting point for the
        // outermost datatype
        iprintf(il, "a [label=\"%04lx\" xlabel=\"a\"]\n", PTR2ADDR(a));
        iprintf(il, "a -> m%04lx\n", PTR2ADDR(a));
    }

    // Print the subgraph to surround the Array content
    iprintf(il, "subgraph cluster_int_array_1d_%d { label=\"Array_1d\"\n", graph_number);
    il++;

    // Output the head node
    print_head_node(il, a);

    // Output the values array. Limit output to 20 elements.
    print_values(il, a, 20);

    // Close the subgraph
    il--;
    iprintf(il, "}\n");

    // Output the edges from the head
    print_head_edge(il, a);

    if (indent_level == 0) {
        // Termination of graph
        printf("}\n");
    }
}
