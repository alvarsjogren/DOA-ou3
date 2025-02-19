#include <stdlib.h>
#include <stdio.h>

#include <table.h>
#include <array_1d.h>


#define MAXSIZE 40000


struct table {
    array_1d *entries; // The table entries are stored in a directed list
    int item_count;
    int max_index;
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
    t->max_index = 0;

    return t;
}

bool table_is_empty(const table *t)
{
    return t->item_count == 0;
}

void table_insert(table *t, void *key, void *value)
{
    // Allocate the key/value structure.
    table_entry *e = table_entry_create(key, value);
    
    int index = array_1d_low(t->entries);

    while (array_1d_inspect_value(t->entries, index) != NULL)
    {
        table_entry *n = array_1d_inspect_value(t->entries, index);
        if (t->key_cmp_func(n->key, key) == 0)
        {
            table_remove(t, n->key);
        } else {
            index++;
        }
    }
    
    array_1d_set_value(t->entries, e, t->item_count);
    t->item_count++;
}

void *table_lookup(const table *t, const void *key)
{
    int index = array_1d_low(t->entries);

    while (index <= t->item_count-1)
    {
        table_entry *e = array_1d_inspect_value(t->entries, index);

        if (t->key_cmp_func(e->key, key) == 0) {
            return e->value;
        }
        index++;
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
    int items_to_remove = 0;
    int index = array_1d_low(t->entries);

    // Iterate over the list. Remove any entries with matching keys.
    while (index <= t->item_count-1) {
        table_entry *e = array_1d_inspect_value(t->entries, index);
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
            array_1d_set_value(t->entries, NULL, index);
            
            // Deallocate the table entry structure.
            table_entry_kill(e);
            items_to_remove++;
        }

        index++;
    }
    if (deferred_ptr != NULL) {
        // Take care of the delayed free.
        t->key_kill_func(deferred_ptr);
    }

    index = 0;
    while (index <= t->item_count-1)
    {
        if (array_1d_inspect_value(t->entries, index) == NULL)
        {
            table_entry *n = array_1d_inspect_value(t->entries, index+1);
            array_1d_set_value(t->entries, n, index);
            array_1d_set_value(t->entries, NULL, index+1);
        }
        index++;
    }

    t->item_count -= items_to_remove;
    
}

void table_kill(table *t) {

    // Iterate over the list. Destroy all elements.
    int index = array_1d_low(t->entries);

    while (index <= t->item_count-1){

        // Inspect the key/value pair.
        table_entry *e = array_1d_inspect_value(t->entries, index);
        // Kill key and/or value if given the authority to do so.

        if (t->key_kill_func != NULL) {
            t->key_kill_func(e->key);
        }
        if (t->value_kill_func != NULL) {
            t->value_kill_func(e->value);
        }
        // Move on to next element.
        index++;
        // Deallocate the table entry structure.
        table_entry_kill(e);
    }

    // Kill what's left of the list...
    array_1d_kill(t->entries);
    // ...and the table struct.
    free(t);
}

void table_print(const table *t, inspect_callback_pair print_func)
{
    // Iterate over all elements. Call print_func on keys/values.
    int i = array_1d_low(t->entries);

    while (i <= t->max_index) {
        table_entry *e = array_1d_inspect_value(t->entries, i);
        // Call print_func
        print_func(e->key, e->value);
        i++;
    }
}