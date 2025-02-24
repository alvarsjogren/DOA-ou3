#include <stdlib.h>
#include <stdio.h>

#include <table.h>
#include <array_1d.h>

/**
 * @file    arraytable.c
 * 
 * @brief   An implementation of a generic table 
 *          using an one-dimensional array.
 * 
 * @authors Alvar Sjögren, id24asn  
 *          Josefin Berg, id24jbg
 * @version 1.0
 * @date    2025-02-20
 */


// Sets the maximum size of the table
#define MAXSIZE 80000

// ===========INTERNAL DATA TYPES ============

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

// ===========INTERNAL FUNCTION IMPLEMENTATIONS ============

/**
 * @brief table_entry_create() - Allocate and populate a table entry.
 * 
 * @param key A pointer to a function to be used to compare keys.
 * @param value A pointer to a function (or NULL) to be called to
 *              de-allocate memory for keys on remove/kill.
 * @return Returns a pointer to the newly created table entry.
 */
table_entry *table_entry_create(void *key, void *value)
{
    table_entry *e = calloc(1, sizeof(*e));

    e->key = key;
    e->value = value;

    return e;
}

/**
 * @brief table_entry_kill() - Return the memory allocated to a table entry.
 * 
 * @param v The table entry to deallocate.
 */
void table_entry_kill(void *v)
{
    table_entry *e = v; // Convert the pointer (useful if debugging the code)

    // All we need to do is to deallocate the struct.
    free(e);
}

/**
 * @brief table_empty() - 
 * 
 * @param key_cmp_func The key compare function.
 * @param key_kill_func The key kill function.
 * @param value_kill_func The value kill function.
 * @return Returns a pointer to the new empty table.
 */
table *table_empty(compare_function *key_cmp_func,
                   kill_function key_kill_func,
                   kill_function value_kill_func)
{
    table *t = calloc(1, sizeof(table));

    t->entries = array_1d_create(0, MAXSIZE, NULL);

    t->key_cmp_func = key_cmp_func;
    t->key_kill_func = key_kill_func;
    t->value_kill_func = value_kill_func;

    t->item_count = 0;

    return t;
}

/**
 * @brief table_is_empty() - Check if a table is empty.
 * 
 * @param t The table to check.
 * @return true 
 * @return false 
 */
bool table_is_empty(const table *t)
{
    return t->item_count == 0;
}

/**
 * @brief table_insert() - Insert a key-value pair into the table.
 *        If the key already exists, the old value is replaced.
 * 
 * @param t The table to insert into.
 * @param key The key to insert.
 * @param value The value to insert.
 */
void table_insert(table *t, void *key, void *value)
{
    table_entry *e = table_entry_create(key, value);
    
    int index = array_1d_low(t->entries);
    while (index <= t->item_count-1)
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

/**
 * @brief table_lookup() - Look up a value in the table using a the provided key.
 * 
 * @param t The table to search.
 * @param key The key to search for.
 * @return Returs the value corresponding to the key, or NULL if the key is not found.
 */
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

/**
 * @brief table_choose_key() - Return an arbitrary key from the table.
 * 
 * @param t The table to choose a key from.
 * @return Returns the chosen key.
 */
void *table_choose_key(const table *t)
{
    table_entry *e = array_1d_inspect_value(t->entries, 0);
    return e->key;
}

/**
 * @brief table_remove() - Remove a key-value pair from the table.
 *        After the removal, the array is compacted.
 *        The item count is decremented if a item is removed.
 *        A deferred pointer is used to avoid memory errors.
 * 
 * @param t The table to remove from.
 * @param key The key to remove.
 */
void table_remove(table *t, const void *key)
{
    void *deferred_ptr = NULL;
    int items_to_remove = 0;
    
    int index = array_1d_low(t->entries);
    while (index <= t->item_count-1) {
        table_entry *e = array_1d_inspect_value(t->entries, index);
        if (t->key_cmp_func(e->key, key) == 0) {
            if (t->key_kill_func != NULL) {
                if (e->key == key) {
                    deferred_ptr = e->key;
                } else {
                    t->key_kill_func(e->key);
                }
            }
            if (t->value_kill_func != NULL) {
                t->value_kill_func(e->value);
            }
            array_1d_set_value(t->entries, NULL, index);
            table_entry_kill(e);
            items_to_remove++;
        }
        index++;
    }
    if (deferred_ptr != NULL) {
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

/**
 * @brief table_kill() - Destroy a table using the 
 *        key kill function and value kill function if they are provided.
 *        Freeing the memory held by the table.
 * 
 * @param t The table to destroy.
 */
void table_kill(table *t) {

    // Iterate over the list. Destroy all elements.
    int index = array_1d_low(t->entries);

    while (index <= t->item_count-1){
        table_entry *e = array_1d_inspect_value(t->entries, index);

        if (t->key_kill_func != NULL) {
            t->key_kill_func(e->key);
        }
        if (t->value_kill_func != NULL) {
            t->value_kill_func(e->value);
        }
        table_entry_kill(e);
        index++;
    }

    array_1d_kill(t->entries);
    free(t);
}

/**
 * @brief table_print() - Print the given table.
 * 
 * @param t The table to print.
 * @param print_func A function to print the key and value.
 *        The print function is called on each element of the table.
 */
void table_print(const table *t, inspect_callback_pair print_func)
{
    int i = array_1d_low(t->entries);

    while (i <= t->item_count-1) {
        table_entry *e = array_1d_inspect_value(t->entries, i);
        print_func(e->key, e->value);
        i++;
    }
}