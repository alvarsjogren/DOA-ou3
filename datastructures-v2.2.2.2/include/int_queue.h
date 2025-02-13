#ifndef __INT_QUEUE_H
#define __INT_QUEUE_H

#include <stdbool.h>
#include "util.h"

/*
 * Declaration of an integer queue for the "Datastructures and
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

// ==========PUBLIC CONSTANTS AND DATA TYPES============

// Maximum size of the queue.
#define MAX_QUEUE_SIZE 100

// We must define the struct publically for the compiler to know its
// size for copying, etc.
typedef struct queue {
    int first_free_pos;
    int elements[MAX_QUEUE_SIZE];
} queue;

// ==========DATA STRUCTURE INTERFACE==========

/**
 * queue_empty() - Create an empty queue.
 *
 * Returns: A new, empty, queue.
 */
queue queue_empty(void);

/**
 * queue_is_empty() - Check if a queue is empty.
 * @q: Queue to check.
 *
 * Returns: True if queue is empty, otherwise false.
 */
bool queue_is_empty(const queue q);

/**
 * queue_enqueue() - Put a value at the end of a queue.
 * @q: Queue to manipulate.
 * @v: Value (integer) to be put in the queue.
 *
 * Returns: The modified queue.
 */
queue queue_enqueue(queue q, int v);

/**
 * queue_dequeue() - Remove the element at the beginning of a queue.
 * @q: Queue to manipulate.
 *
 * NOTE: Undefined for an empty queue.
 *
 * Returns: The modified queue.
 */
queue queue_dequeue(queue q);

/**
 * queue_front() - Inspect the value at the front of the queue.
 * @q: Queue to inspect.
 *
 * Returns: The value at the front of the queue.
 *          NOTE: The return value is undefined for an empty queue.
 */
int queue_front(const queue q);

/**
 * queue_kill() - Destroy a given queue.
 * @q: Queue to destroy.
 *
 * Does nothing since the queue does not use any dynamic
 * memory. Included for symmetry with generic queue.h.
 *
 * Returns: Nothing.
 */
void queue_kill(queue q);

/**
 * queue_print() - Iterate over the queue elements and print their values.
 * @q: Queue to inspect.
 *
 * Iterates over the queue and prints each integer.
 *
 * Returns: Nothing.
 */
void queue_print(const queue q);

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
void queue_print_internal(const queue q, const char *desc, int indent_level, int max_elems);

#endif
