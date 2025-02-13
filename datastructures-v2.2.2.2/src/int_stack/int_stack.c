#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <int_stack.h>

/*
 * Implementation of an integer stack for the "Datastructures and
 * algorithms" courses at the Department of Computing Science, Umea
 * University. The stack stores integers directly and does not use
 * dynamic memory. Thus, the clean-up function stack_kill is strictly
 * not necessary, but recommended to maintain symmetry with untyped,
 * generic stack implementations.
 *
 * Author: Niclas Borlin (niclas@cs.umu.se)
 *
 * Based on earlier code by: Johan Eliasson (johane@cs.umu.se)
 *                           Niclas Borlin (niclas@cs.umu.se)
 *                           Adam Dahlgren Lindstrom (dali@cs.umu.se)
 *
 * Version information:
 *   v1.0  2022-03-24: First public version.
 *   v1.1  2024-04-10: Added print_internal.
 *   v1.2  2024-05-10: Added/updated print_internal with improved encapsulation.
 */

/**
 * stack_empty() - Create an empty stack.
 *
 * Returns: An empty stack.
 */
stack stack_empty(void)
{
    stack s;
    s.first_free_pos = 0;
    // Set all elements to zero. Not really needed, but otherwise
    // the output from print_internal becomes unreadable.
    for (int i = 0; i < sizeof(s.elements)/sizeof(s.elements[0]); i++) {
        s.elements[i] = 0;
    }
    return s;
}

/**
 * stack_is_empty() - Check if a stack is empty.
 * @s: Stack to check.
 *
 * Returns: True if stack is empty, otherwise false.
 */
bool stack_is_empty(const stack s)
{
    // The stack is empty if no positions are occupied with elements,
    // i.e., the first free position is zero
    return s.first_free_pos == 0;
}

/**
 * stack_push() - Push a value on top of a stack.
 * @s: Stack to manipulate.
 * @v: Value (integer) to be put on the stack.
 *
 * Returns: The modified stack.
 *          NOTE: After the call, the input stack should be considered invalid.
 */
stack stack_push(stack s, int v)
{
    // Store value at first free position.
    s.elements[s.first_free_pos] = v;
    // Update first free position.
    s.first_free_pos++;

    // Return the new stack.
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
stack stack_pop(stack s)
{
    if (stack_is_empty(s)) {
        fprintf(stderr, "stack_pop: Warning: pop on empty stack\n");
    } else {
        // We only have to decrease the first free position to
        // indicate that the element that used to be on top of the
        // stack is now free for use.
        s.first_free_pos--;
    }
    return s;
}

/**
 * stack_top() - Inspect the value at the top of the stack.
 * @s: Stack to inspect.
 *
 * Returns: The integer at the top of the stack.
 *              NOTE: The return value is undefined for an empty stack.
 */
int stack_top(const stack s)
{
    if (stack_is_empty(s)) {
        fprintf(stderr, "stack_top: Warning: top on empty stack\n");
    }
    return s.elements[s.first_free_pos - 1];
}

/**
 * stack_kill() - Destroy a given stack.
 * @s: Stack to destroy.
 *
 * Does nothing since the stack does not use any dynamic
 * memory. Included for symmetry with generic stack.h.
 *
 * Returns: Nothing.
 */
void stack_kill(stack s)
{
    // Do nothing.
}

/**
 * stack_print() - Iterate over the stack elements and print their values.
 * @s: Stack to inspect.
 *
 * Iterates over the stack and prints each integer.
 *
 * Returns: Nothing.
 */
void stack_print(const stack s)
{
    printf("{ ");
    // Print elements from the top down.
    for (int i = s.first_free_pos - 1; i >= 0; i--) {
        printf("[%d]", s.elements[i]);
        if (i>0) {
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
 * print_head_node() - Print a node corresponding to the stack struct.
 * @indent_level: Indentation level.
 * @max_elems: Maximum element to print.
 * @s: Stack to inspect.
 *
 * Returns: Nothing.
 */
static void print_head_node(int indent_level, int max_elems, const stack s)
{
    int elems_to_print = sizeof(s.elements)/sizeof(s.elements[0]);
    if (max_elems < elems_to_print) {
        elems_to_print = max_elems;
    }
    iprintf(indent_level, "s [shape=record label=\"first_free_pos\\n%d", s.first_free_pos);
    for (int i = 0; i < elems_to_print; i++) {
        printf("|%d\\n%d", i, s.elements[i]);
    }
    printf("\" xlabel=\"s\"]\n");
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

// Print edge from the description to the stack.
static void print_desc_edge(int indent_level)
{
    iprintf(indent_level, "description -> s [style=invis] "
            "// Dummy line to place description above\n");
}

/**
 * stack_print_internal() - Print the internal structure of the stack in dot format.
 * @s: Stack to inspect.
 * @desc: String with a description/state of the stack, or NULL for no description.
 * @indent_level: Indentation level, 0 for outermost.
 * @max_elems: Maximum number of elements to print.
 *
 * Iterates over the stack and outputs dot code that shows the
 * internal structure of the stack.  The dot code can be visualized by
 * Graphviz.
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
void stack_print_internal(const stack s, const char *desc, int indent_level, int max_elems)
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

    // Output the head node
    print_head_node(il, max_elems, s);

    // Output a dummy line to place the description at the top
    print_desc_edge(il);

    if (indent_level == 0) {
        // Termination of graph
        printf("}\n");
    }
}
