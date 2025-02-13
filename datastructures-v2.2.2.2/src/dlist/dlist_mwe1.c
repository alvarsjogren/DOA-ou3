#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dlist.h>

/*
 * Minimum working example 1 for dlist.c.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *          Lars Karlsson (larsk@cs.umu.se)
 *
 * Version information:
 *   v1.0 2018-01-28: First public version.
 *   v1.1 2018-04-03: Split into versions with/without memhandler.
 *   v1.2 2023-01-14: Added printouts at start/end of main.
 *   v1.3 2024-03-14: Added explicit copy/kill string functions.
 */

#define VERSION "v1.3"
#define VERSION_DATE "2024-03-14"

// Make a dynamic copy of the input string.
char *copy_string(char *str)
{
    char *copy = calloc(strlen(str) + 1, sizeof(char));
    strcpy(copy, str);
    return copy;
}

// String print function.
void print_string(const void *value)
{
    const char *s=value;
    printf("\"%s\"", s);
}

// Return the memory occupied by the string
void kill_string(void *v)
{
    char *p = v; // Convert to a char ptr, useful in a debugger
    free(p);
}

// Test program.
int main(void)
{
    printf("%s, %s %s: Create list of strings without memfreehandler.\n",
           __FILE__, VERSION, VERSION_DATE);
    printf("Code base version %s (%s).\n\n", CODE_BASE_VERSION, CODE_BASE_RELEASE_DATE);

    // Names to insert in the list.
    char *names[] = {"Alfons", "Bengt", "Cia", "David", "Florian", "Gunnar"};

    // Create the list
    dlist *l = dlist_empty(NULL);
    dlist_pos p = dlist_first(l);
    for (int i = 0; i < sizeof(names) / sizeof(names[0]); i++) {
        // Insert dynamic copies of strings at last position.
        p = dlist_insert(l, copy_string(names[i]), p);
        p = dlist_next(l, p);
    }
    // Print the list.
    printf("DList after inserting 6 strings:\n");
    dlist_print(l, print_string);

    // Traverse the list and deallocate each value.
    p = dlist_first(l);
    while (!dlist_is_end(l,p)) {
        char *v=dlist_inspect(l,p);
        kill_string(v);
        p=dlist_remove(l,p);
    }
    // Destroy the list.
    dlist_kill(l);

    printf("\nNormal exit.\n\n");
    return 0;
}
