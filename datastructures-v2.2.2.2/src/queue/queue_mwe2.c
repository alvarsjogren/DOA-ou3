#include <stdio.h>
#include <stdlib.h>

#include <queue.h>

/*
 * Minimum working example for queue.c.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *          Adam Dahlgren Lindstrom (dali@cs.umu.se)
 *
 * Version information:
 *   v1.0 2018-01-28: First public version.
 *   v1.1 2018-04-03: Split into versions with/without memhandler.
 *   v1.2 2023-01-14: Added printouts at start/end of main.
 *   v1.3 2024-03-14: Added explicit create/kill functions.
 */

#define VERSION "v1.3"
#define VERSION_DATE "2024-03-14"

// Integers are stored via int pointers stored as void pointers.
// Convert the given pointer and print the dereferenced value.
void print_int(const void *data)
{
    const int *v = data;
    printf("%d", *v);
}

// Create a dynamic copy of the integer i.
int *int_create(int i)
{
    // Allocate memory for an integer and set the value
    int *v=malloc(sizeof(*v));
    *v = i;
    return v;
}

// Return the memory used by the integer.
void int_kill(void *v)
{
    int *p=v;
    free(p);
}

int main(void)
{
    printf("%s, %s %s: Create integer queue with kill_function.\n",
           __FILE__, VERSION, VERSION_DATE);
    printf("Code base version %s (%s).\n\n", CODE_BASE_VERSION, CODE_BASE_RELEASE_DATE);

    // Create the queue.
    queue *q = queue_empty(int_kill);

    for (int i = 11; i <= 13; i++) {
        // Allocate memory for one int.
        int *v = int_create(i);
        // Put value in queue.
        q=queue_enqueue(q,v);
    }

    printf("--QUEUE before dequeueing--\n");
    queue_print(q, print_int);

    // Dequeue one element. The payload memory will be automatically returned.
    queue_dequeue(q);

    printf("--QUEUE after dequeueing--\n");
    queue_print(q, print_int);

    queue_kill(q);

    printf("\nNormal exit.\n\n");
    return 0;
}
