#ifndef __STACK_H
#define __STACK_H

#include <stdbool.h>
#include "util.h"

/*
 * Declaration of a generic stack for the "Datastructures and
 * algorithms" courses at the Department of Computing Science, Umea
 * University. The stack stores void pointers, so it can be used to
 * store all types of values. After use, the function stack_kill must
 * be called to de-allocate the dynamic memory used by the stack
 * itself. The de-allocation of any dynamic memory allocated for the
 * element values is the responsibility of the user of the stack,
 * unless a kill_function is registered in stack_empty.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *          Adam Dahlgren Lindstrom (dali@cs.umu.se)
 *
 * Based on earlier code by: Johan Eliasson (johane@cs.umu.se).
 *
 * Version information:
 *   v1.0  2018-01-28: First public version.
 *   v1.1  2024-05-10: Added/updated print_internal to enhance encapsulation.
 */

// ==========PUBLIC DATA TYPES============

// Stack type.
typedef struct stack stack;

// ==========DATA STRUCTURE INTERFACE==========

/**
 * stack_empty() - Create an empty stack.
 * @kill_func: A pointer to a function (or NULL) to be called to
 *             de-allocate memory on remove/kill.
 *
 * Returns: A pointer to the new stack.
 */
stack *stack_empty(kill_function kill_func);

/**
 * stack_is_empty() - Check if a stack is empty.
 * @s: Stack to check.
 *
 * Returns: True if stack is empty, otherwise false.
 */
bool stack_is_empty(const stack *s);

/**
 * stack_push() - Push a value on top of a stack.
 * @s: Stack to manipulate.
 * @v: Value (pointer) to be put on the stack.
 *
 * Returns: The modified stack.
 *          NOTE: After the call, the input stack should be considered invalid.
 */
stack *stack_push(stack *s, void *v);

/**
 * stack_pop() - Remove the element at the top of a stack.
 * @s: Stack to manipulate.
 *
 * NOTE: Undefined for an empty stack.
 *
 * Returns: The modified stack.
 *          NOTE: After the call, the input stack should be considered invalid.
 */
stack *stack_pop(stack *s);

/**
 * stack_top() - Inspect the value at the top of the stack.
 * @s: Stack to inspect.
 *
 * Returns: The value at the top of the stack.
 *          NOTE: The return value is undefined for an empty stack.
 */
void *stack_top(const stack *s);

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
void stack_kill(stack *s);

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
void stack_print(const stack *s, inspect_callback print_func);

/**
 * stack_print_internal() - Print the stacks internal structure in dot format.
 * @l: Stack to inspect.
 * @print_func: Function called for each element value.
 * @desc: String with a description/state of the stack, or NULL for no description.
 * @indent_level: Indentation level, 0 for outermost
 *
 * Iterates over the stack and outputs dot code that shows the internal
 * structure of the stack.  The dot code can be visualized by
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
void stack_print_internal(const stack *s, inspect_callback print_func, const char *desc,
                          int indent_level);

#endif
