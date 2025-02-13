#ifndef TABLE_H
#define TABLE_H

#include <stdbool.h>
#include "util.h"

/*
 * Declaration of a generic table for the "Datastructures and
 * algorithms" courses at the Department of Computing Science, Umea
 * University. The table stores void pointers, so it can be used to
 * store all types of values. After use, the function table_kill must
 * be called to de-allocate the dynamic memory used by the table
 * itself. The de-allocation of any dynamic memory allocated for the
 * key and/or value values is the responsibility of the user of the
 * table, unless a corresponding kill_function is registered in
 * table_empty.
 *
 * The internal handling of duplicates are transparent to the user of
 * the table. Depending on the table design, a duplicate may or may
 * not be stored internally. The user of the table should not make any
 * assumption about which solution is implemented.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *          Adam Dahlgren Lindstrom (dali@cs.umu.se)
 *
 * Based on earlier code by: Johan Eliasson (johane@cs.umu.se).
 *
 * Version information:
 *   v1.0  2018-02-06: First public version.
 *   v1.1  2024-04-15: Changed the comments to leave room for different
 *                     internal handling of duplicates.
 *   v1.2  2024-05-10: Added/updated print_internal to enhance encapsulation.
 */

// ==========PUBLIC DATA TYPES============
// Table type.
typedef struct table table;

// ==========DATA STRUCTURE INTERFACE==========

/**
 * table_empty() - Create an empty table.
 * @key_cmp_func: A pointer to a function to be used to compare keys. See
 *                util.h for the definition of compare_function.
 * @key_kill_func: A pointer to a function (or NULL) to be called to
 *                 de-allocate memory for keys on remove/kill.
 * @value_kill_func: A pointer to a function (or NULL) to be called to
 *                   de-allocate memory for values on remove/kill.
 *
 * Returns: Pointer to a new table.
 */
table *table_empty(compare_function key_cmp_func,
                   kill_function key_kill_func,
                   kill_function value_kill_func);

/**
 * table_is_empty() - Check if a table is empty.
 * @t: Table to check.
 *
 * Returns: True if table contains no key/value pairs, false otherwise.
 */
bool table_is_empty(const table *t);

/**
 * table_insert() - Add a key/value pair to a table.
 * @t: Table to manipulate.
 * @key: A pointer to the key value.
 * @value: A pointer to the value value.
 *
 * Insert the key/value pair into the table. If the key already
 * exists, the key/value overwrites the existing pair. The technical
 * details are internal to the datatype. At any rate, table_lookup()
 * will return the latest added value for a duplicate
 * key and table_remove() will remove all duplicates for a given key.
 *
 * Returns: Nothing.
 */
void table_insert(table *t, void *key, void *value);

/**
 * table_lookup() - Look up a given key in a table.
 * @t: Table to inspect.
 * @key: Key to look up.
 *
 * Returns: The value corresponding to a given key, or NULL if the key
 * is not found in the table. If the table contains duplicate keys,
 * the value that was latest inserted will be returned.
 */
void *table_lookup(const table *t, const void *key);

/**
 * table_choose_key() - Return an arbitrary key.
 * @t: Table to inspect.
 *
 * Return an arbitrary key stored in the table. Can be used together
 * with table_remove() to deconstruct the table. Undefined for an
 * empty table.
 *
 * Returns: An arbitrary key stored in the table.
 */
void *table_choose_key(const table *t);

/**
 * table_remove() - Remove a key/value pair in the table.
 * @t: Table to manipulate.
 * @key: Key for which to remove pair.
 *
 * Any matching duplicates will be removed. Will call any kill
 * functions set for keys/values. Does nothing if key is not found in
 * the table.
 *
 * Returns: Nothing.
 */
void table_remove(table *t, const void *key);

/**
 * table_kill() - Destroy a table.
 * @t: Table to destroy.
 *
 * Return all dynamic memory used by the table and its elements. If a
 * kill_func was registered for keys and/or values at table creation,
 * it is called each element to free any user-allocated memory
 * occupied by the element values.
 *
 * Returns: Nothing.
 */
void table_kill(table *t);

/**
 * table_print() - Print the given table.
 * @t: Table to print.
 * @print_func: Function called for each key/value pair in the table.
 *
 * Iterates over the key/value pairs in the table and prints them.
 * Will print all stored elements, including any duplicates.
 *
 * Returns: Nothing.
 */
void table_print(const table *t, inspect_callback_pair print_func);

/**
 * table_print_internal() - Output the internal structure of the table.
 * @t: Table to print.
 * @key_print_func: Function called for each key in the table.
 * @value_print_func: Function called for each value in the table.
 * @desc: String with a description/state of the list.
 * @indent_level: Indentation level, 0 for outermost
 *
 * Iterates over the list and prints code that shows its' internal structure.
 *
 * Returns: Nothing.
 */
void table_print_internal(const table *t, inspect_callback key_print_func,
                          inspect_callback value_print_func, const char *desc,
                          int indent_level);

#endif
