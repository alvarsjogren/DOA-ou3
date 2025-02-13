#ifndef __INT_STACK_H
#define __INT_STACK_H

#include <stdbool.h>
#include "util.h"

/*
 * Declaration of an integer stack for the "Datastructures and
 * algorithms" courses at the Department of Computing Science, Umea
 * University. The stack stores integers directly and does not use
 * dynamic memory. Thus, the clean-up function stack_kill is strictly
 * not necessary, but recommended to maintain symmetry with untyped,
 * generic stack implementations.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *          Adam Dahlgren Lindstrom (dali@cs.umu.se)
 *
 * Based on earlier code by: Johan Eliasson (johane@cs.umu.se).
 *
 * Version information:
 *   v1.0  2018-01-28: First public version.
 *   v1.1  2024-04-15: Reduced default stack size to 15 to make output
 *                     from print_internal readable.
 *   v1.2  2024-05-10: Updated print_internal to enhance encapsulation.
 */

// ==========PUBLIC DATA TYPES============

// Stack type.
#define MAX_STACK_SIZE 100

// We must define the struct publically for the compiler to know its
// size for copying, etc.
typedef struct stack {
    int first_free_pos;
    int elements[MAX_STACK_SIZE];
} stack;

// ==========DATA STRUCTURE INTERFACE==========

/**
 * stack_empty() - Create an empty stack.
 *
 * Returns: A new stack.
 */
stack stack_empty(void);

/**
 * stack_is_empty() - Check if a stack is empty.
 * @s: Stack to check.
 *
 * Returns: True if stack is empty, otherwise false.
 */
bool stack_is_empty(const stack s);

/**
 * stack_push() - Push a value on top of a stack.
 * @s: Stack to manipulate.
 * @v: Value (integer) to be put on the stack.
 *
 * Returns: The modified stack.
 *          NOTE: After the call, the input stack should be considered invalid.
 */
stack stack_push(stack s, int v);

/**
 * stack_pop() - Remove the element at the top of a stack.
 * @s: Stack to manipulate.
 *
 * NOTE: Undefined for an empty stack.
 *
 * Returns: The modified stack.
 *          NOTE: After the call, the input stack should be considered invalid.
 */
stack stack_pop(stack s);

/**
 * stack_top() - Inspect the value at the top of the stack.
 * @s: Stack to inspect.
 *
 * Returns: The value at the top of the stack.
 *          NOTE: The return value is undefined for an empty stack.
 */
int stack_top(const stack s);

/**
 * stack_kill() - Destroy a given stack.
 * @s: Stack to destroy.
 *
 * Does nothing since the stack does not use any dynamic
 * memory. Included for symmetry with generic stack.h.
 *
 * Returns: Nothing.
 */
void stack_kill(stack s);

/**
 * stack_print() - Iterate over the stack elements and print their values.
 * @s: Stack to inspect.
 *
 * Iterates over the stack and prints each integer.
 *
 * Returns: Nothing.
 */
void stack_print(const stack s);

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
void stack_print_internal(const stack s, const char *desc, int indent_level, int max_elems);

#endif
