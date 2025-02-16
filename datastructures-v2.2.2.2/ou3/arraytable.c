#include <stdlib.h>
#include <stdio.h>

#include <table.h>
#include <array_1d.h>


#define MAXSIZE 40000


struct table {
    array_1d *entries; // The table entries are stored in a directed list
    int item_count;
    compare_function *key_cmp_func;
    kill_function key_kill_func;
    kill_function value_kill_func;
};

typedef struct table_entry {
    void *key;
    void *value;
} table_entry;

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

void table_entry_kill(void *v)
{
    table_entry *e = v; // Convert the pointer (useful if debugging the code)

    // All we need to do is to deallocate the struct.
    free(e);
}


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

    // Counts item in table
    t->item_count = 0;

    return t;
}

bool table_is_empty(const table *t)
{
    return t->item_count == 0;
}

//  Använder item count som index
void table_insert(table *t, void *key, void *value)
{
    // Allocate the key/value structure.
    table_entry *e = table_entry_create(key, value);

    array_1d_set_value(t->entries, e, t->item_count);
    t->item_count++;
}

//  Kollar efter nästa lediga position
// void table_insert(table *t, void *key, void *value)
// {
//     // Allocate the key/value structure.
//     table_entry *e = table_entry_create(key, value);

//     int i = 0;
//     while (array_1d_inspect_value(t->entries, i) != NULL)
//     {
//         i++;
//     }

//     array_1d_set_value(t->entries, e, i);
//     t->item_count++;
// }


void *table_lookup(const table *t, const void *key)
{
    // Iterate over the list. Return first match.

    dlist_pos pos = dlist_first(t->entries);

    while (!dlist_is_end(t->entries, pos)) {
        // Inspect the table entry
        table_entry *e = dlist_inspect(t->entries, pos);
        // Check if the entry key matches the search key.
        if (t->key_cmp_func(e->key, key) == 0) {
            // If yes, return the corresponding value pointer.
            return e->value;
        }
        // Continue with the next position.
        pos = dlist_next(t->entries, pos);
    }
    // No match found. Return NULL.
    return NULL;
}