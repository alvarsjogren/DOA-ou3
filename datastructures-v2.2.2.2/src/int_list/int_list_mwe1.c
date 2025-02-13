#include <stdlib.h>
#include <stdio.h>

#include <int_list.h>

/*
 * Minimum working example for int_list.c. Create a list, insert two
 * elements, print list, free list.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *          Adam Dahlgren Lindstrom (dali@cs.umu.se)
 *
 * Version information:
 *   v1.0  2018-01-28: First public version.
 *   v1.1  2023-01-14: Added printouts at start/end of main.
 *   v1.11 2024-01-16: Fix include to be with brackets, not citation marks.
 */

#define VERSION "v1.11"
#define VERSION_DATE "2024-01-16"

int main(void)
{
    printf("%s, %s %s: Create typed integer list.\n",
           __FILE__, VERSION, VERSION_DATE);
    printf("Code base version %s (%s).\n\n", CODE_BASE_VERSION, CODE_BASE_RELEASE_DATE);

    // Create the list.
    list *l = list_empty();

    // Insert the value 5 first in the list.
    list_insert(l, 5, list_first(l));

    printf("List after inserting one value:\n");
    list_print(l);

    // Insert the value 8 last in the list.
    list_insert(l, 8, list_end(l));

    printf("List after inserting second value at the end:\n");
    list_print(l);

    // Insert the value 2 at the second position in the list.
    list_insert(l, 2, list_next(l, list_first(l)));

    printf("List after inserting a third value in the middle:\n");
    list_print(l);

    // Remove first element.
    list_remove(l, list_first(l));

    printf("List after removing first element:\n");
    list_print(l);

    // Done, kill the list.
    list_kill(l);

    printf("\nNormal exit.\n\n");
    return 0;
}
