#include <stdio.h>
#include <stdlib.h>

#include <list.h>

/*
 * Minimum working example for list.c.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *          Adam Dahlgren Lindstrom (dali@cs.umu.se)
 *
 * Version information:
 *   v1.0 2018-01-28: First public version.
 *   v1.1 2018-04-03: Split into versions with/without memhandler.
 *   v1.2 2023-01-14: Added printouts at start/end of main.
 *   v1.3 2024-03-13: Added explicit create/kill functions.
 */

#define VERSION "v1.3"
#define VERSION_DATE "2024-03-13"

// Integers are stored via int pointers stored as void pointers.
// Convert the given pointer and print the dereferenced value.
static void print_int(const void *data)
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
    printf("%s, %s %s: Create integer list without kill_function.\n",
           __FILE__, VERSION, VERSION_DATE);
    printf("Code base version %s (%s).\n\n", CODE_BASE_VERSION, CODE_BASE_RELEASE_DATE);

    // Create the list.
    list *l = list_empty(NULL);

    // Create a dynamic integer with value 5.
    int *val = int_create(5);

    // Insert the value first in the list.
    list_insert(l, val, list_first(l));

    printf("List after inserting one value:\n");
    list_print(l, print_int);

    // Allocate space for another integer.
    val = int_create(8);
    // Insert the value last in the list.
    list_insert(l, val, list_end(l));

    printf("List after inserting second value at the end:\n");
    list_print(l, print_int);

    // Allocate space for a third integer.
    val = int_create(2);
    // Insert the value at the second position in the list.
    list_insert(l, val, list_next(l, list_first(l)));

    printf("List after inserting a third value in the middle:\n");
    list_print(l, print_int);

    // Remove first element.
    list_pos p=list_first(l);
    int *v=list_inspect(l, p);
    // Remove element from list.
    list_remove(l, p);
    // Free element content.
    int_kill(v);

    printf("List after removing first element:\n");
    list_print(l, print_int);

    // Empty the list.
    while (!list_is_empty(l)) {
        list_pos p=list_first(l);
        int *v=list_inspect(l,p);
        int_kill(v);
        list_remove(l,p);
    }
    // Done, kill the list.
    list_kill(l);

    printf("\nNormal exit.\n\n");
    return 0;
}
