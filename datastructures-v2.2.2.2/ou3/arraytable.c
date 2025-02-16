#include <stdlib.h>
#include <stdio.h>

#include <table.h>
#include <array_1d.h>


#define MAXSIZE 40000


struct table {
    array_1d *entries; // The table entries are stored in a directed list
    compare_function *key_cmp_func;
    kill_function key_kill_func;
    kill_function value_kill_func;
};

typedef struct table_entry {
    void *key;
    void *value;
} table_entry;

// ===========INTERNAL FUNCTION IMPLEMENTATIONS ============

/**
 * table_entry_create() - Allocate and populate a table entry.
 * @key: A pointer to a function to be used to compare keys.
 * @value: A pointer to a function (or NULL) to be called to
 *                 de-allocate memory for keys on remove/kill.
 *
 * Returns: A pointer to the newly created table entry.
 */
table_entry *table_entry_create(void *key, void *value)
{
    // Allocate space for a table entry. Use calloc as a defensive
    // measure to ensure that all pointers are initialized to NULL.
    table_entry *e = calloc(1, sizeof(*e));
    // Populate the entry.
    e->key = key;
    e->value = value;

    return e;
}

/**
 * table_entry_kill() - Return the memory allocated to a table entry.
 * @e: The table entry to deallocate.
 *
 * Returns: Nothing.
 */
void table_entry_kill(void *v)
{
    table_entry *e = v; // Convert the pointer (useful if debugging the code)

    // All we need to do is to deallocate the struct.
    free(e);
}

/**
 * table_empty() - Create an empty table.
 * @key_cmp_func: A pointer to a function to be used to compare keys.
 * @key_kill_func: A pointer to a function (or NULL) to be called to
 *                 de-allocate memory for keys on remove/kill.
 * @value_kill_func: A pointer to a function (or NULL) to be called to
 *                   de-allocate memory for values on remove/kill.
 *
 * Returns: Pointer to a new table.
 */
table *table_empty(compare_function *key_cmp_func,
                   kill_function key_kill_func,
                   kill_function value_kill_func)
{
    // Allocate the table header.
    table *t = calloc(1, sizeof(table));
    // Create the list to hold the table_entry-ies.
    t->entries = array_1d_create(0, MAXSIZE, NULL);
    // Store the key compare function and key/value kill functions.
    t->key_cmp_func = key_cmp_func;
    t->key_kill_func = key_kill_func;
    t->value_kill_func = value_kill_func;

    return t;
}

/**
 * table_is_empty() - Check if a table is empty.
 * @table: Table to check.
 *
 * Returns: True if table contains no key/value pairs, false otherwise.
 */
bool table_is_empty(const table *t)
{
    return dlist_is_empty(t->entries);
}