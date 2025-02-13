#include <stdio.h>
#include <stdlib.h>
#include <strings.h> // For bcopy
#include <stdarg.h>

#include <int_list_array.h>

/*
 * Implementation of a typed, undirected list of integers for the
 * "Datastructures and algorithms" courses at the Department of
 * Computing Science, Umea University. The implementation uses a
 * static array.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *
 * Version information:
 *   v1.0  2018-01-28: First public version.
 *   v1.1  2023-01-19: Added list_pos_equal and list_pos_is_valid functions. Bugfix
 *                     in list_remove.
 *   v1.2  2023-01-20: Renamed list_pos_equal to list_pos_are_equal.
 *   v1.3  2023-03-23: Renamed list_pos_are_equal to list_pos_is_equal.
 *   v1.4  2024-05-10: Added print_internal.
 */

// ===========INTERNAL DATA TYPES============

/*
 * The list is implemented as a static array.
 */
struct list {
    int last_used_pos;
    int *values;
    int array_size;
};

// ===========INTERNAL FUNCTION IMPLEMENTATIONS============

#define ARRAY_MAX_SIZE 100

/**
 * list_empty() - Create an empty list.
 *
 * Returns: A pointer to the new list.
 */
list *list_empty(void)
{
    // Allocate memory for the list head.
    list *l=malloc(sizeof(list));
    // Set the size
    l->array_size = ARRAY_MAX_SIZE;
    // Allocate memory for the elements.
    l->values=calloc(l->array_size,sizeof(l->values[0]));
    // Set last used position.
    l->last_used_pos=-1;
    return l;
}

/**
 * list_is_empty() - Check if a list is empty.
 * @l: List to check.
 *
 * Returns: True if the list is empty, otherwise false.
 */
bool list_is_empty(const list * l)
{
    // List is empty if no elements are used.
    return l->last_used_pos < 0;
}

/**
 * list_first() - Return the first position of a list, i.e. the
 *                position of the first element in the list.
 * @l: List to inspect.
 *
 * Returns: The first position in the given list.
 */
list_pos list_first(const list * l)
{
    // First position is always 0.
    return 0;
}

/**
 * list_end() - Return the last position of a list, i.e. the position
 *              after the last element in the list.
 * @l: List to inspect.
 *
 * Returns: The last position in the given list.
 */
list_pos list_end(const list * l)
{
    // Last position is position *after* last used element.
    return l->last_used_pos + 1;
}

/**
 * list_next() - Return the next position in a list.
 * @l: List to inspect.
 * @p: Any valid position except the last in the list.
 *
 * Returns: The position in the list after the given position.
 *          NOTE: The return value is undefined for the last position.
 */
list_pos list_next(const list * l, const list_pos p)
{
    if (list_pos_is_equal(l, p, list_end(l))) {
        // This should really throw an error.
        fprintf(stderr,"list_next: Warning: Trying to navigate "
                "past end of list!");
    }
    return p + 1;
}

/**
 * list_prev() - Return the previous position in a list.
 * @l: List to inspect.
 * @p: Any valid position except the first in the list.
 *
 * Returns: The position in the list before the given position.
 *          NOTE: The return value is undefined for the first position.
 */
list_pos list_prev(const list * l, const list_pos p)
{
    if (list_pos_is_equal(l, p, list_first(l))) {
        // This should really throw an error.
        fprintf(stderr,"list_prev: Warning: Trying to navigate "
                "past beginning of list!\n");
    }
    return p - 1;
}

/**
 * list_inspect() - Return the value of the element at a given
 *                  position in a list.
 * @l: List to inspect.
 * @p: Any valid position in the list, except the last.
 *
 * Returns: The integer value stored in the element at postiion pos.
 *          NOTE: The return value is undefined for the last position.
 */
int list_inspect(const list * l, const list_pos p)
{
    if (list_pos_is_equal(l, p, list_end(l))) {
        // This should really throw an error.
        fprintf(stderr,"list_inspect: Warning: Trying to inspect "
                "position at end of list!\n");
    }
    return l->values[p];
}

/**
 * list_insert() - Insert a new element with a given value into a list.
 * @l: List to manipulate.
 * @v: Integer value to be inserted into the list.
 * @p: Position in the list before which the value should be inserted.
 *
 * Creates a new element and inserts it into the list before p.
 * Stores data in the new element.
 *
 * Returns: The position of the newly created element.
 */
list_pos list_insert(list * l, int v, const list_pos p)
{
    // Move elements at position pos and later forward.
    bcopy(l->values + p, l->values + p + 1,
          sizeof(int) * (l->last_used_pos - p + 1));

    // Set value.
    l->values[p] = v;

    // Increment number of used elements.
    l->last_used_pos++;

    // Return the position of the new value.
    return p;
}

/**
 * list_remove() - Remove an element from a list.
 * @l: List to manipulate.
 * @p: Position in the list of the element to remove.
 *
 * Removes the element at position p from the list.
 *
 * Returns: The position after the removed element.
 */
list_pos list_remove(list *l, const list_pos p)
{
    // Move elements at position pos and later forward.
    bcopy(l->values + p + 1, l->values + p, sizeof(int) * (l->last_used_pos - p));

    // Decrement number of used elements.
    l->last_used_pos--;

    // p now refers to the position after the removed element.
    return p;
}

/**
 * list_kill() - Destroy a given list.
 * @l: List to destroy.
 *
 * Returns all dynamic memory used by the list and its elements.
 *
 * Returns: Nothing.
 */
void list_kill(list * l)
{
    free(l->values);
    free(l);
}

/**
 * list_print() - Iterate over the list element and print their values.
 * @l: List to inspect.
 *
 * Iterates over the list and print each stored integer.
 *
 * Returns: Nothing.
 */
void list_print(const list * l)
{
    // Start at the beginning of the list.
    list_pos p = list_first(l);

    while (!list_pos_is_equal(l, p, list_end(l))) {
        // Call print_func with the element value at the
        // current position.
        printf("[%d]\n",list_inspect(l, p));
        p = list_next(l, p);
    }
}

/**
 * list_pos_is_equal() - Return true if two positions in a list are equal.
 * @l: List to inspect.
 * @p1: First position to compare.
 * @p2: Second position to compare.
 *
 * Returns: True if p1 and p2 refer to the same position in l, otherwise False.
 *          NOTE: The result is defined only if p1 and p2 are valid positions in l.
 */
bool list_pos_is_equal(const list *l, const list_pos p1, const list_pos p2)
{
    // Since we don't need to check whether p1 or p2 are valid, we
    // only need to compare them directly.
    return p1 == p2;
}

/**
 * list_pos_is_valid() - Return true for a valid position in a list
 * @l: List to inspect.
 * @p: Any position.
 *
 * Returns: True if p is a valid position in the list, otherwise false.
 */
bool list_pos_is_valid(const list *l, const list_pos p)
{
    // The position is valid if it is between 0 and last_used_pos + 1, inclusive.
    return p >= 0 && p <= l->last_used_pos+1;

}

// ===========INTERNAL FUNCTIONS USED BY int_list_array_print_internal ============

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
 * print_head_node() - Print a node corresponding to the list head struct.
 * @indent_level: Indentation level.
 * @l: List to inspect.
 *
 * Returns: Nothing.
 */
static void print_head_node(int indent_level, const list *l)
{
    iprintf(indent_level, "m%04lx [shape=record "
            "label=\"last_used_pos\\n%d|array_size\\n%d|<v>values\\n%04lx\"]\n",
            PTR2ADDR(l), l->last_used_pos, l->array_size, PTR2ADDR(l->values));
}

/**
 * print_values() - Print a node containing all positions in the array.
 * @indent_level: Indentation level.
 * @l: List to inspect.
 * @max_values_to_print: Maximum number of values to output.
 *
 * Will print dot code to display each value in the array, up to
 * max_values_to_print. If there are more values to print, adds an
 * ellipsis (...) at the end.
 *
 * Returns: Nothing.
 */
void print_values(int indent_level, const list *l, int max_values_to_print)
{
    int values_to_print = l->array_size;
    int truncated = false;
    if (values_to_print > max_values_to_print) {
        values_to_print = max_values_to_print;
        truncated = true;
    }
    iprintf(indent_level, "m%04lx [shape=record label=\"", PTR2ADDR(l->values));
    for (int i=0; i < values_to_print; i++) {
        printf("%02d\\n%d", i, l->values[i]);
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

// Print edges from the list head to the head and tail cells.
static void print_head_edge(int indent_level, const list *l)
{
    print_edge(indent_level, l, l->values, "v", "values", NULL);
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
 * list_print_internal() - Print the lists internal structure in dot format.
 * @l: List to inspect.
 * @desc: String with a description/state of the list, or NULL for no description.
 * @indent_level: Indentation level, 0 for outermost
 *
 * Iterates over the list and outputs dot code that shows the internal
 * structure of the list.  The dot code can be visualized by
 * Graphviz.
 *
 * On linux system, the output can be parsed by the dot program, e.g.
 *
 *   <list_program> | dot -Tsvg > /tmp/dot.svg; firefox /tmp/dot.svg
 *
 * where <list_program> is the name of the executable
 *
 * The output may also be possible to visualize online on
 * https://dreampuf.github.io/GraphvizOnline/ or google "graphviz
 * online".
 *
 * For documention of the dot language, see graphviz.org.
 *
 * Returns: Nothing.
 */
void list_print_internal(const list *l, const char *desc, int indent_level)
{
    static int graph_number = 0;
    graph_number++;
    int il = indent_level;

    if (indent_level == 0) {
        // If this is the outermost datatype, start a graph...
        printf("digraph LIST_%d {\n", graph_number);

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
            iprintf(il, "\tcluster_int_list_%d_description [label=\"%s\"]\n", graph_number, escaped);
        }
        // Return the memory used by the escaped string
        free(escaped);
    }

    if (indent_level == 0) {
        // Use a single "pointer" edge as a starting point for the
        // outermost datatype
        iprintf(il, "l [label=\"%04lx\" xlabel=\"l\"]\n", PTR2ADDR(l));
        iprintf(il, "l -> m%04lx\n", PTR2ADDR(l));
    }

    // Print the subgraph to surround the List content
    iprintf(il, "subgraph cluster_int_list_%d { label=\"List\"\n", graph_number);
    il++;

    // Output the head node
    print_head_node(il, l);

    // Output the values array. Limit the output to 10 elements.
    print_values(il, l, 10);

    // Close the subgraph
    il--;
    iprintf(il, "}\n");

    // Output the edges from the head
    print_head_edge(il, l);

    if (indent_level == 0) {
        // Termination of graph
        printf("}\n");
    }
}
