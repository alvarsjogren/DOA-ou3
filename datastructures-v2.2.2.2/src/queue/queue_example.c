#include <stdio.h>
#include <stdlib.h>

#include <queue.h>

/*
 * A small example that uses the generic queue to store integers.
 * Based on int_queue_example.c from int_queue.
 *
 * Author: Niclas Borlin (niclas@cs.umu.se)
 *
 * Version information:
 *   v1.0 2025-01-23: First public version.
 */

// Create a dynamic copy of the integer i.
int *int_create(int i)
{
    // Allocate memory for an integer and set the value
    int *v = malloc(sizeof(*v));
    *v = i;
    return v;
}

// Return the memory used by the integer.
void int_kill(void *v)
{
    int *p = v; // Convert to a readable pointer - useful in a debugger
    free(p);
}

int main(void)
{
    // Create the queue.
    queue *q = queue_empty(int_kill);

    // The queue should be empty. Is it?
    if (queue_is_empty(q)) {
	printf("The newly created queue is empty.\n");
    } else {
	printf("The newly created queue is NOT empty.\n");
    }

    for (int i = 11; i <= 13; i++) {
	printf("Add the value %d at the end of the queue.\n", i);
	int *v = int_create(i);
        // Put some values in the queue.
        q = queue_enqueue(q, v);
    }

    int *v = queue_front(q);
    printf("The value at the front of the queue is: %d.\n", *v);

    // Remove one element from the queue.
    printf("Remove one element.\n");
    q = queue_dequeue(q);

    v = queue_front(q);
    printf("The value at the front of the queue is now: %d.\n", *v);

    // Kill queue. Strictly not necessary for int_queue since it
    // does not use any dynamic memory. Included for symmetry with
    // queue.c.
    queue_kill(q);

    printf("\nNormal exit.\n\n");
    return 0;
}
