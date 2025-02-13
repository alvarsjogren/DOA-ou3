#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <stack.h>

/*
 * Implementation of a generic stack for the "Datastructures and
 * algorithms" courses at the Department of Computing Science, Umea
 * University.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *          Adam Dahlgren Lindstrom (dali@cs.umu.se)
 *
 * Based on earlier code by: Johan Eliasson (johane@cs.umu.se).
 *
 * Version information:
 *   v1.0  2018-01-28: First public version.
 *   v1.1  2022-03-24: Minor update to always take care of returned pointer.
 *   v2.0  2024-03-14: Added stack_print_internal to output dot code for visualization.
 *                     Renamed free_* stuff to kill_*. Converted to 4-tabs.
 *   v2.1  2024-05-10: Updated print_internal with improved encapsulation.
 */

// ===========INTERNAL DATA TYPES============

/*
 * The stack elements are implemented as one-cells with forward and
 * links. The stack has a single element pointer.
 */
typedef struct cell {
    struct cell *next;
    void *val;
} cell;

struct stack {
    cell *top;
    kill_function kill_func;
};

// ===========INTERNAL FUNCTION IMPLEMENTATIONS============

/**
 * stack_empty() - Create an empty stack.
 * @kill_func: A pointer to a function (or NULL) to be called to
 *             de-allocate memory on remove/kill.
 *
 * Returns: A pointer to the new stack.
 */
stack *stack_empty(kill_function kill_func)
{
    // Allocate memory for stack structure.
    stack *s = calloc(1, sizeof(stack));
    s->top = NULL;
    s->kill_func = kill_func;

    return s;
}

/**
 * stack_is_empty() - Check if a stack is empty.
 * @s: Stack to check.
 *
 * Returns: True if stack is empty, otherwise false.
 */
bool stack_is_empty(const stack *s)
{
    return s->top == NULL;
}

/**
 * stack_push() - Push a value on top of a stack.
 * @s: Stack to manipulate.
 * @v: Value (pointer) to be put on the stack.
 *
 * Returns: The modified stack.
 *          NOTE: After the call, the input stack should be considered invalid.
 */
stack *stack_push(stack *s, void *v)
{
    // Allocate memory for element.
    cell *e = calloc(1, sizeof(*e));
    // Set element value.
    e->val = v;
    // Link to current top.
    e->next = s->top;
    // Put element on top of stack.
    s->top = e;
    // Return modified stack.
    return s;
}

/**
 * stack_pop() - Remove the element at the top of a stack.
 * @s: Stack to manipulate.
 *
 * NOTE: Undefined for an empty stack.
 *
 * Returns: The modified stack.
 *          NOTE: After the call, the input stack should be considered invalid.
 */
stack *stack_pop(stack *s)
{
    if (stack_is_empty(s)) {
        fprintf(stderr, "stack_pop: Warning: pop on empty stack\n");
    } else {
        // Remember top element.
        cell *e = s->top;
        // Link past top element.
        s->top = s->top->next;
        // De-allocate user memory.
        if (s->kill_func != NULL) {
            s->kill_func(e->val);
        }
        // De-allocate element memory.
        free(e);
    }
    return s;
}

/**
 * stack_top() - Inspect the value at the top of the stack.
 * @s: Stack to inspect.
 *
 * Returns: The value at the top of the stack.
 *          NOTE: The return value is undefined for an empty stack.
 */
void *stack_top(const stack *s)
{
    if (stack_is_empty(s)) {
        fprintf(stderr, "stack_top: Warning: top on empty stack\n");
    }
    return s->top->val;
}

/**
 * stack_kill() - Destroy a given stack.
 * @s: Stack to destroy.
 *
 * Return all dynamic memory used by the stack and its elements. If a
 * kill_func was registered at stack creation, also calls it for each
 * element to kill any user-allocated memory occupied by the element values.
 *
 * Returns: Nothing.
 */
void stack_kill(stack *s)
{
    while (!stack_is_empty(s)) {
        s = stack_pop(s);
    }
    free(s);
}

/**
 * stack_print() - Iterate over the stack elements and print their values.
 * @s: Stack to inspect.
 * @print_func: Function called for each element.
 *
 * Iterates over the stack and calls print_func with the value stored
 * in each element.
 *
 * Returns: Nothing.
 */
void stack_print(const stack *s, inspect_callback print_func)
{
    printf("{ ");
    cell *e = s->top;
    while (e != NULL) {
        print_func(e->val);
        e = e->next;
        if (e != NULL) {
            printf(", ");
        }
    }
    printf(" }\n");
}

// ===========INTERNAL FUNCTIONS USED BY stack_print_internal ============

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
 * print_head_node() - Print a node corresponding to the stack head struct.
 * @indent_level: Indentation level.
 * @s: Stack to inspect.
 *
 * Returns: Nothing.
 */
static void print_head_node(int indent_level, const stack *s)
{
    iprintf(indent_level, "m%04lx [shape=record label=\"kill\\n%04lx|<t>top\\n%04lx\"]\n",
            PTR2ADDR(s), PTR2ADDR(s->kill_func), PTR2ADDR(s->top));
}

// Print edges from the stack head to the head cell.
static void print_head_edge(int indent_level, const stack *s)
{
    print_edge(indent_level, s, s->top, "t", "top", NULL);
}

// Print a node corresponding to the cell at position p.
static void print_elem_node(int indent_level, const cell *p)
{
    iprintf(indent_level, "m%04lx [shape=record label=\"<v>val\\n%04lx|<n>next\\n%04lx\"]\n",
            PTR2ADDR(p), PTR2ADDR(p->val),  PTR2ADDR(p->next));
}

// Print edges from the cell at position p to the next and previous
// cells and the value.  The value pointer is red, the others are
// black. If the stack owns the memory, the edge is solid, otherwise
// dashed.
static void print_elem_edges(int indent_level, const stack *s, const cell *p)
{
    print_edge(indent_level, p, p->next, "n", "next", NULL);

    if (s->kill_func) {
        print_edge(indent_level, p, p->val, "v", "val", "color=red");
    } else {
        print_edge(indent_level, p, p->val, "v", "val", "color=red style=dashed");
    }
}

// Print the node for the memory block at p using the user-supplied
// print_func to print the label.
static void print_value_node(int indent_level, const void *p, inspect_callback print_func)
{
    iprintf(indent_level, "m%04lx [label=\"", PTR2ADDR(p));
    if (print_func != NULL) {
        print_func(p);
    }
    printf("\" xlabel=\"%04lx\"]\n", PTR2ADDR(p));
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
 * stack_print_internal() - Print the stacks internal structure in dot format.
 * @l: Stack to inspect.
 * @print_func: Function called for each element value.
 * @desc: String with a description/state of the stack, or NULL for no description.
 * @indent_level: Indentation level, 0 for outermost
 *
 * Iterates over the stack and outputs dot code that shows the internal
 * structure of the stack.  The dot code can be visualized by Graphviz.
 *
 * On linux system, the output can be parsed by the dot program, e.g.
 *
 *   <stack_program> | dot -Tsvg > /tmp/dot.svg; firefox /tmp/dot.svg
 *
 * where <stack_program> is the name of the executable
 *
 * The output may also be possible to visualize online on
 * https://dreampuf.github.io/GraphvizOnline/ or google "graphviz
 * online".
 *
 * For documention of the dot language, see graphviz.org.
 *
 * Returns: Nothing.
 */
void stack_print_internal(const stack *s, inspect_callback print_func, const char *desc,
                          int indent_level)
{
    static int graph_number = 0;
    graph_number++;
    int il = indent_level;

    if (indent_level == 0) {
        // If this is the outermost datatype, start a graph...
        printf("digraph STACK_%d {\n", graph_number);

        // Specify default shape and fontname
        il++;
        iprintf(il, "node [shape=rectangle fontname=\"Courier New\"]\n");
        iprintf(il, "ranksep=0.01\n");
        iprintf(il, "subgraph cluster_nullspace {\n");
        iprintf(il+1, "NULL\n");
        iprintf(il, "}\n");
    }

    if (desc != NULL) {
        // Escape the string before printout
        char *escaped = escape_chars(desc);
        // Use different names on inner description nodes
        if (indent_level == 0) {
            iprintf(il, "description [label=\"%s\"]\n", escaped);
        } else {
            iprintf(il, "cluster_stack_%d_description [label=\"%s\"]\n", graph_number, escaped);
        }
        // Return the memory used by the escaped string
        free(escaped);
    }

    if (indent_level == 0) {
        // Use a single "pointer" edge as a starting point for the
        // outermost datatype
        iprintf(il, "s [label=\"%04lx\" xlabel=\"s\"]\n", PTR2ADDR(s));
        iprintf(il, "s -> m%04lx\n", PTR2ADDR(s));
    }

    // Print the subgraph to surround the Stack content
    iprintf(il, "subgraph cluster_stack_%d { label=\"Stack\"\n", graph_number);
    il++;

    // Output the head node
    print_head_node(il, s);

    // Output the element nodes
    cell *p = s->top;
    while (p != NULL) {
        print_elem_node(il, p);
        p = p->next;
    }

    // Close the subgraph
    il--;
    iprintf(il, "}\n");

    if (indent_level == 0) {
        // Put the user nodes in userspace
        iprintf(il, "subgraph cluster_userspace { label=\"User space\"\n");
        il++;
    }

    // Output the value nodes
    p = s->top;
    while (p != NULL) {
        if (p->val) {
            print_value_node(il, p->val, print_func);
        }
        p = p->next;
    }

    if (indent_level == 0) {
        // Close userspace
        il--;
        iprintf(il, "}\n");
    }

    // Output the edges from the head
    print_head_edge(il, s);

    // Output the edges from each element
    p = s->top;
    while (p != NULL) {
        print_elem_edges(il, s, p);
        p = p->next;
    }

    if (indent_level == 0) {
        // Termination of graph
        printf("}\n");
    }
}
