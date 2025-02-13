#include <stdio.h>
#include <stdlib.h>

#include <int_queue.h>

/*
 * A small example that uses an integer queue.
 *
 * Author: Niclas Borlin (niclas@cs.umu.se)
 *
 * Version information:
 *   v1.0 2025-01-16: First public version.
 */

int main(void)
{
    // Create the queue.
    queue q = queue_empty();

    // The queue should be empty. Is it?
    if (queue_is_empty(q)) {
	printf("The newly created queue is empty.\n");
    } else {
	printf("The newly created queue is NOT empty.\n");
    }

    for (int i = 11; i <= 13; i++) {
	printf("Add the value %d at the end of the queue.\n", i);
        // Put some values in the queue.
        q = queue_enqueue(q, i);
    }

    int v = queue_front(q);
    printf("The value at the front of the queue is: %d.\n", v);

    // Remove one element from the queue.
    printf("Remove one element.\n");
    q = queue_dequeue(q);

    v = queue_front(q);
    printf("The value at the front of the queue is now: %d.\n", v);

    // Kill queue. Strictly not necessary for int_queue since it
    // does not use any dynamic memory. Included for symmetry with
    // queue.c.
    queue_kill(q);

    printf("\nNormal exit.\n\n");
    return 0;
}
