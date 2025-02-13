#include <stdio.h>
#include <stdlib.h>

#include <queue.h>

// Size of internal string buffer
#define BUFLEN 400

/*
 * A small example that uses a generic queue to store integers.
 *
 * Generates two kinds of output:
 * 1) The 'outside' view showing the stored values only.
 * 2) The 'inside' view showing the internal organization.
 *
 * Author: Niclas Borlin (niclas@cs.umu.se)
 *
 * Version information:
 *   v1.0 2025-01-24: First public version.
 */

#define VERSION "v1.0"
#define VERSION_DATE "2025-01-24"

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

// Print a message, then the queue and wait for the user to press return.
void print_and_wait(const queue *q, const char *msg)
{
    printf("\nThe output from queue_print() shows the values stored in the queue\n");
    printf("\n%s\n", msg);
    queue_print(q, print_int);
    printf("\nPress Enter to continue...");
    getchar(); // Waits for the user to press Enter
    printf("\n");
}

// Print cut lines before and after a call array_print_internal.
void print_internal_and_wait(const queue *q, const char *desc)
{
    // Print starting marker line.
    printf("Direct your web browser to:\n    https://dreampuf.github.io/GraphvizOnline/?engine=dot#digraph%%20G%%20%%7B%%7D%%0A\n");
    printf("\n\n1) Copy the lines between the cut marks\n"
	   "2) Paste into the left half of the browser window.\n"
	   "3) The right half of the window should now show a visualization of the\n   internal structure of the queue.\n");
    printf("\n--- CUT HERE ---\n\n");

    // Call the internal print function to get the actual dot code.
    queue_print_internal(q, print_int, desc, 0);

    // Print ending marker line
    printf("\n--- END CUT HERE ---\n\n");

    printf("\nPress Enter to continue...");
    getchar(); // Waits for the user to press Enter
    printf("\n");
}

int main(void)
{
    // Message buffer
    char buf[BUFLEN];

    printf("%s, %s %s: Example use of a typed integer queue.\n",
           __FILE__, VERSION, VERSION_DATE);
    printf("Code base version %s (%s).\n\n", CODE_BASE_VERSION, CODE_BASE_RELEASE_DATE);

    // Create the queue.
    queue *q = queue_empty(int_kill);

    // Call print functions to show the content of the queue
    print_and_wait(q, "The queue is empty");
    print_internal_and_wait(q, "The queue is empty");

    for (int i = 11; i <= 13; i++) {
	int *v = int_create(i);
        q = queue_enqueue(q, v);
	snprintf(buf, BUFLEN, "The value %d was added to the queue", i);
	print_and_wait(q, buf);
	print_internal_and_wait(q, buf);
    }

    int *w = queue_front(q);
    printf("The value at the front of the queue is: %d.\n", *w);

    // Remove one element from the queue.
    q = queue_dequeue(q);
    snprintf(buf, BUFLEN, "The front element was removed from the queue");
    print_and_wait(q, buf);
    print_internal_and_wait(q, buf);

    w = queue_front(q);
    printf("The value at the front of the queue is now: %d.\n", *w);

    // Kill queue. Strictly not necessary for int_queue since it
    // does not use any dynamic memory. Included for symmetry with
    // queue.c.
    queue_kill(q);

    printf("\nNormal exit.\n\n");
    return 0;
}
