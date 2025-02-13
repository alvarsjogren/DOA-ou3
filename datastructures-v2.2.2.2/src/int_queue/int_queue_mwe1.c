#include <stdio.h>
#include <stdlib.h>

#include <int_queue.h>

/*
 * Minimum working example for int_queue.c.
 *
 * Author: Niclas Borlin (niclas@cs.umu.se)
 *
 * Version information:
 *   v1.0 2025-01-10: First public version.
 */

#define VERSION "v1.0"
#define VERSION_DATE "2025-01-10"

int main(void)
{
    printf("%s, %s %s: Create typed integer queue.\n",
           __FILE__, VERSION, VERSION_DATE);
    printf("Code base version %s (%s).\n\n", CODE_BASE_VERSION, CODE_BASE_RELEASE_DATE);

    // Create the queue.
    queue q = queue_empty();
    printf("--Empty QUEUE--\n");
    queue_print(q);


    for (int i = 11; i <= 13; i++) {
        // Put some values in the queue.
        q = queue_enqueue(q, i);
	printf("--QUEUE during insert--\n");
	queue_print(q);
    }

    int v = queue_front(q);
    printf("The value at the front of the queue is: %d.\n", v);

    printf("--QUEUE after enqueue, before dequeueing--\n");
    queue_print(q);

    // Remove one element from the queue.
    q = queue_dequeue(q);

    printf("--QUEUE after dequeueing--\n");
    queue_print(q);

    // Kill queue. Strictly not necessary for int_queue since it
    // does not use any dynamic memory. Included for symmetry with
    // queue.c.
    queue_kill(q);

    printf("\nNormal exit.\n\n");
    return 0;
}
