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

// Vi fortsätter att anta att item count också är slutet på arrayen
void *table_lookup(const table *t, const void *key)
{
    int i = 0;

    while (i != t->item_count-1)
    {
        table_entry *e = array_1d_inspect_value(t->entries, i);

        if (t->key_cmp_func(e->key, key) == 0) {
            return e->value;
        }
    }

    return NULL;
}

void *table_choose_key(const table *t)
{
    table_entry *e = array_1d_inspect_value(t->entries, 0);

    return e->key;
}

void table_remove(table *t, const void *key)
{
    // Will be set if we need to delay a free.
    void *deferred_ptr = NULL;

    // Start at beginning of the list.
    int pos = dlist_first(t->entries);

    // Iterate over the list. Remove any entries with matching keys.
    while (!dlist_is_end(t->entries, pos)) {
        // Inspect the table entry
        table_entry *e = dlist_inspect(t->entries, pos);

        // Compare the supplied key with the key of this entry.
        if (t->key_cmp_func(e->key, key) == 0) {
            // If we have a match, call kill on the key
            // and/or value if given the responsiblity
            if (t->key_kill_func != NULL) {
                if (e->key == key) {
                    // The given key points to the same
                    // memory as entry->key. Freeing it here
                    // would trigger a memory error in the
                    // next iteration. Instead, defer free
                    // of this pointer to the very end.
                    deferred_ptr = e->key;
                } else {
                    t->key_kill_func(e->key);
                }
            }
            if (t->value_kill_func != NULL) {
                t->value_kill_func(e->value);
            }
            // Remove the list element itself.
            pos = dlist_remove(t->entries, pos);
            // Deallocate the table entry structure.
            table_entry_kill(e);
        } else {
            // No match, move on to next element in the list.
            pos = dlist_next(t->entries, pos);
        }
    }

    if (deferred_ptr != NULL) {
        // Take care of the delayed free.
        t->key_kill_func(deferred_ptr);
    }
}

void table_kill(table *t){

        // Iterate over the list. Destroy all elements.
        int i = array_1d_low(t->entries);

        while(i <=(t->item_count)){

            // Inspect the key/value pair.
            table_entry *e = array_1d_inspect_value(t->entries, i);
            // Kill key and/or value if given the authority to do so.
            if (t->key_kill_func != NULL) {
                t->key_kill_func(e->key);
            }
            if (t->value_kill_func != NULL) {
                t->value_kill_func(e->value);
            }
            // Move on to next element.
            i ++;
            // Deallocate the table entry structure.
            table_entry_kill(e);
        }
    
        // Kill what's left of the list...
        array_1d_kill(t->entries);
        // ...and the table struct.
    free(t);
}
