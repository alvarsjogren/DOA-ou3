#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <table.h>

/*
 * Minimum working example for table.c. Inserts 4 key-value pairs into
 * a table, including one duplicate. Output dot/graphviz code to show
 * the internal structure of the table. In this version, the table
 * "borrows" the payload memory, i.e., the user of the table is
 * responsible for deallocating the payload memory. See table_mwe2i.c
 * for a version where the table takes over the responsibility.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *
 * Version information:
 *   v1.0  2024-04-11: First public version. Adapted from table_mwe1.c.
 */

#define VERSION "v1.0"
#define VERSION_DATE "2024-04-11"

// Create a dynamic copy of the string str.
char* string_copy(const char *str)
{
    // Use calloc to ensure a '\0' termination.
    char *copy = calloc(strlen(str) + 1, sizeof(*copy));
    strcpy(copy, str);
    return copy;
}

// Create a dynamic copy of the string str.
void string_kill(void *v)
{
    // Convert pointer - useful in debugging.
    char *s = v;
    free(s);
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
    int *p = v;
    free(p);
}

// Interpret the supplied key and value pointers and print their content.
void print_string_int_pair(const void *key, const void *value)
{
    const char *k=key;
    const int *v=value;
    printf("[%s, %d]\n", k, *v);
}

// Interpret the supplied key pointer and print its content.
void print_int(const void *key)
{
    const int *k=key;
    printf("%d", *k);
}

// Interpret the supplied value pointer and print its content.
void print_string(const void *value)
{
    const char *v=value;
    printf("\\\"%s\\\"", v);
}

// Compare two keys (char *).
int compare_strings(const void *k1, const void *k2)
{
    const char *key1 = k1;
    const char *key2 = k2;

    return strcmp(key1, key2);
}

// Print cut lines before and after a call list_print_internal.
void print_internal_with_cut_lines(const table *t, const char *desc)
{
    // Internal counter that will be remembered between calls.

    // Used to generate sequentially numbered -- CUT HERE -- marker lines
    // to enable automatic parsing of the output.
    static int cut = 1;

    // Print starting marker line.
    printf("\n--- START CUT HERE %d ---\n", cut);

    // Call the internal print function to get the actual dot code.
    table_print_internal(t, print_string, print_int, desc, 0);

    // Print ending marker line
    printf("--- END CUT HERE %d ---\n\n", cut);

    // Increment the cut number. Will be remembered next time the
    // function is called since cut is a static variable.
    cut++;
}

// Print a message with intructions how to use the dot output. prog is
// the name of the executable.
void print_dot_usage(char *prog)
{
    printf("\nGenerate dot code to visualize internal structure with GraphViz. ");
    printf("Use\n\n%s ", prog);
    printf("| sed -n '/START CUT HERE X/,/END CUT HERE X/{//!p}' | dot -Tsvg > /tmp/dot.svg\n\n");
    printf("to generate an svg file of cut X (replace X by the requested cut number).\n");
    printf("The generated file can then be visualized with\n\n");
    printf("firefox /tmp/dot.svg\n\n");
    printf("Use -Tpng to generate a .png file instead. "
           "See graphviz.org and %s for documentation.\n", __FILE__);

    printf("\n--- Start of normal output ---\n\n");
}

int main(int argc, char *argv[])
{
    printf("%s, %s %s: Create (integer, string) table without any kill_function.\n",
           __FILE__, VERSION, VERSION_DATE);
    printf("Code base version %s (%s).\n\n", CODE_BASE_VERSION, CODE_BASE_RELEASE_DATE);

    print_dot_usage(argv[0]);

    // Keep track of the key-value pairs we allocate.
    char *key[3];
    int *value[3];

    // Keep the deallocation responsibility of the keys/values we allocate.
    table *t = table_empty(compare_strings, NULL, NULL);

    printf("Empty table from the outside:\n");
    table_print(t, print_string_int_pair);
    print_internal_with_cut_lines(t, __FILE__ ": Empty table showing the inside structure.");

    key[0] = string_copy("Jan");
    value[0] = int_create(31);
    table_insert(t, key[0], value[0]);

    key[1] = string_copy("Feb");
    value[1] = int_create(28);
    table_insert(t, key[1], value[1]);

    key[2] = string_copy("Mar");
    value[2] = int_create(31);
    table_insert(t, key[2], value[2]);

    printf("Table from the outside after adding 3 key-value pairs:\n");
    table_print(t, print_string_int_pair);
    print_internal_with_cut_lines(t, __FILE__ ": Table with 3 key-value pairs showing the inside structure.");

    // Kill the table, excluding the keys and values we entered.
    table_kill(t);

    // Free key/value pairs that we put in the table.
    for (int i = 0; i < sizeof(key)/sizeof(key[0]); i++) {
        string_kill(key[i]);
        int_kill(value[i]);
    }

    printf("\nNormal exit.\n\n");
    return 0;
}
