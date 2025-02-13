#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <table.h>

/*
 * Minimum working example for table.c. Inserts 3 key-value pairs into
 * a table, no duplicates. Makes one lookup and prints the result. The
 * responsibility to deallocate the key-value pairs is NOT handed over
 * to the table. Uses table_choose_key() to extract keys and values to
 * be able to destroy the table without memory leaks or externally
 * stored pointers.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *          Adam Dahlgren Lindstrom (dali@cs.umu.se)
 *
 * Version information:
 *   v1.0  2018-02-06: First public version.
 *   v1.1  2023-01-14: Added printouts at start/end of main.
 *   v1.2  2024-04-11: Added explicit create/kill of both strings and ints.
 */

#define VERSION "v1.2"
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
void print_int_string_pair(const void *key, const void *value)
{
    const int *k=key;
    const char *s=value;
    printf("[%d, %s]\n", *k, s);
}

// Compare two keys (int *).
int compare_ints(const void *k1, const void *k2)
{
    int key1 = *(int *)k1;
    int key2 = *(int *)k2;

    if ( key1 == key2 )
        return 0;
    if ( key1 < key2 )
        return -1;
    return 1;
}

int main(void)
{
    printf("%s, %s %s: Create (integer, string) table without any kill_function "
           "nor externally stored pointers, .\n", __FILE__, VERSION, VERSION_DATE);
    printf("Code base version %s (%s).\n\n", CODE_BASE_VERSION, CODE_BASE_RELEASE_DATE);

    // Keep the deallocation responsibility.
    table *t = table_empty(compare_ints, NULL, NULL);

    int *key;
    char *value;

    key = int_create(90187);
    value = string_copy("Umea");
    table_insert(t, key, value);

    key = int_create(90184);
    value = string_copy("Umea");
    table_insert(t, key, value);

    key = int_create(98185);
    value = string_copy("Kiruna");
    table_insert(t, key, value);

    printf("Table after inserting 3 pairs:\n");
    table_print(t, print_int_string_pair);

    int v = 90187;
    const char *s=table_lookup(t, &v);
    printf("Lookup of postal code %d: %s.\n", v, s);

    // Simple cleanup that works if the table is without duplicates.
    while (!table_is_empty(t)) {
        // Get one key from the table.
        key = table_choose_key(t);
        // Lookup the corresponding value.
        value = table_lookup(t, key);
        // Remove the key-value pair from the table.
        table_remove(t, key);
        // De-allocate key and value.
        int_kill(key);
        string_kill(value);
    }
    // Kill what is left by the table.
    table_kill(t);

    printf("\nNormal exit.\n\n");
    return 0;
}
