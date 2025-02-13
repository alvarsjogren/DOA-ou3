#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h> // For isspace()
#include <stdarg.h>

#include <table.h>
#include <dlist.h>

/*
 * Implementation of a generic table for the "Datastructures and
 * algorithms" courses at the Department of Computing Science, Umea
 * University.
 *
 * Duplicates are handled by inspect and remove.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *          Adam Dahlgren Lindstrom (dali@cs.umu.se)
 *
 * Based on earlier code by: Johan Eliasson (johane@cs.umu.se).
 *
 * Version information:
 *   v1.0  2018-02-06: First public version.
 *   v1.1  2019-03-04: Bugfix in table_remove.
 *   v1.2  2024-04-15: Added table_print_internal.
 *   v2.0  2024-05-10: Updated print_internal with improved encapsulation.
 */

// ===========INTERNAL DATA TYPES ============

struct table {
    dlist *entries; // The table entries are stored in a directed list
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
    t->entries = dlist_empty(table_entry_kill);
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

/**
 * table_insert() - Add a key/value pair to a table.
 * @table: Table to manipulate.
 * @key: A pointer to the key value.
 * @value: A pointer to the value value.
 *
 * Insert the key/value pair into the table. No test is performed to
 * check if key is a duplicate. table_lookup() will return the latest
 * added value for a duplicate key. table_remove() will remove all
 * duplicates for a given key.
 *
 * Returns: Nothing.
 */
void table_insert(table *t, void *key, void *value)
{
    // Allocate the key/value structure.
    table_entry *e = table_entry_create(key, value);

    dlist_insert(t->entries, e, dlist_first(t->entries));
}

/**
 * table_lookup() - Look up a given key in a table.
 * @table: Table to inspect.
 * @key: Key to look up.
 *
 * Returns: The value corresponding to a given key, or NULL if the key
 * is not found in the table. If the table contains duplicate keys,
 * the value that was latest inserted will be returned.
 */
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
void *table_choose_key(const table *t)
{
    // Return first key value.
    dlist_pos pos = dlist_first(t->entries);
    table_entry *e = dlist_inspect(t->entries, pos);

    return e->key;
}

/**
 * table_remove() - Remove a key/value pair in the table.
 * @table: Table to manipulate.
 * @key: Key for which to remove pair.
 *
 * Any matching duplicates will be removed. Will call any kill
 * functions set for keys/values. Does nothing if key is not found in
 * the table.
 *
 * Returns: Nothing.
 */
void table_remove(table *t, const void *key)
{
    // Will be set if we need to delay a free.
    void *deferred_ptr = NULL;

    // Start at beginning of the list.
    dlist_pos pos = dlist_first(t->entries);

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

/*
 * table_kill() - Destroy a table.
 * @table: Table to destroy.
 *
 * Return all dynamic memory used by the table and its elements. If a
 * kill_func was registered for keys and/or values at table creation,
 * it is called each element to kill any user-allocated memory
 * occupied by the element values.
 *
 * Returns: Nothing.
 */
void table_kill(table *t)
{
    // Iterate over the list. Destroy all elements.
    dlist_pos pos = dlist_first(t->entries);

    while (!dlist_is_end(t->entries, pos)) {
        // Inspect the key/value pair.
        table_entry *e = dlist_inspect(t->entries, pos);
        // Kill key and/or value if given the authority to do so.
        if (t->key_kill_func != NULL) {
            t->key_kill_func(e->key);
        }
        if (t->value_kill_func != NULL) {
            t->value_kill_func(e->value);
        }
        // Move on to next element.
        pos = dlist_next(t->entries, pos);
    }

    // Kill what's left of the list...
    dlist_kill(t->entries);
    // ...and the table struct.
    free(t);
}

/**
 * table_print() - Print the given table.
 * @t: Table to print.
 * @print_func: Function called for each key/value pair in the table.
 *
 * Iterates over the key/value pairs in the table and prints them.
 * Will print all stored elements, including duplicates.
 *
 * Returns: Nothing.
 */
void table_print(const table *t, inspect_callback_pair print_func)
{
    // Iterate over all elements. Call print_func on keys/values.
    dlist_pos pos = dlist_first(t->entries);

    while (!dlist_is_end(t->entries, pos)) {
        table_entry *e = dlist_inspect(t->entries, pos);
        // Call print_func
        print_func(e->key, e->value);
        pos = dlist_next(t->entries, pos);
    }
}

// ===========INTERNAL FUNCTIONS USED BY list_print_internal ============

// The functions below output code in the dot language, used by
// GraphViz. For documention of the dot language, see graphviz.org.

/**
 * indent() - Output indentation string.
 * @n: Indentation level.
 *
 * Print n tab characters.
 *
 * Returns: Nothing.
 */
static void indent(int n)
{
    for (int i=0; i<n; i++) {
        printf("\t");
    }
}
/**
 * iprintf(...) - Indent and print.
 * @n: Indentation level
 * @...: printf arguments
 *
 * Print n tab characters and calls printf.
 *
 * Returns: Nothing.
 */
static void iprintf(int n, const char *fmt, ...)
{
    // Indent...
    indent(n);
    // ...and call printf
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

/**
 * print_edge() - Print a edge between two addresses.
 * @from: The address of the start of the edge. Should be non-NULL.
 * @to: The address of the destination for the edge, including NULL.
 * @port: The name of the port on the source node, or NULL.
 * @label: The label for the edge, or NULL.
 * @options: A string with other edge options, or NULL.
 *
 * Print an edge from port PORT on node FROM to TO with label
 * LABEL. If to is NULL, the destination is the NULL node, otherwise a
 * memory node. If the port is NULL, the edge starts at the node, not
 * a specific port on it. If label is NULL, no label is used. The
 * options string, if non-NULL, is printed before the label.
 *
 * Returns: Nothing.
 */
static void print_edge(int indent_level, const void *from, const void *to, const char *port,
                       const char *label, const char *options)
{
    indent(indent_level);
    if (port) {
        printf("m%04lx:%s -> ", PTR2ADDR(from), port);
    } else {
        printf("m%04lx -> ", PTR2ADDR(from));
    }
    if (to == NULL) {
        printf("NULL");
    } else {
        printf("m%04lx", PTR2ADDR(to));
    }
    printf(" [");
    if (options != NULL) {
        printf("%s", options);
    }
    if (label != NULL) {
        printf(" label=\"%s\"",label);
    }
    printf("]\n");
}

/**
 * print_head_node() - Print a node corresponding to the table struct.
 * @indent_level: Indentation level.
 * @t: Table to inspect.
 *
 * Returns: Nothing.
 */
static void print_head_node(int indent_level, const table *t)
{
    iprintf(indent_level, "m%04lx [shape=record "
            "label=\"<e>entries\\n%04lx|cmp\\n%04lx|key_kill\\n%04lx|value_kill\\n%04lx\"]\n",
            PTR2ADDR(t), PTR2ADDR(t->entries), PTR2ADDR(t->key_cmp_func),
            PTR2ADDR(t->key_kill_func), PTR2ADDR(t->value_kill_func));
}

// Internal function to print the head--entries edge in dot format.
static void print_head_edge(int indent_level, const table *t)
{
    print_edge(indent_level, t, t->entries, "e", "entries", NULL);
}

// Internal function to print the table entry node in dot format.
static void print_element_node(int indent_level, const table_entry *e)
{
    iprintf(indent_level, "m%04lx [shape=record label=\"<k>key\\n%04lx|<v>value\\n%04lx\"]\n",
            PTR2ADDR(e), PTR2ADDR(e->key), PTR2ADDR(e->value));
}

// Internal function to print the table entry node in dot format.
static void print_key_value_nodes(int indent_level, const table_entry *e,
                                  inspect_callback key_print_func,
                                  inspect_callback value_print_func)
{
    if (e->key != NULL) {
        iprintf(indent_level, "m%04lx [label=\"", PTR2ADDR(e->key));
        if (key_print_func != NULL) {
            key_print_func(e->key);
        }
        printf("\" xlabel=\"%04lx\"]\n", PTR2ADDR(e->key));
    }
    if (e->value != NULL) {
        iprintf(indent_level, "m%04lx [label=\"", PTR2ADDR(e->value));
        if (value_print_func != NULL) {
            value_print_func(e->value);
        }
        printf("\" xlabel=\"%04lx\"]\n", PTR2ADDR(e->value));
    }
}

// Internal function to print edges from the table entry node in dot format.
// Memory "owned" by the table is indicated by solid red lines. Memory
// "borrowed" from the user is indicated by red dashed lines.
static void print_key_value_edges(int indent_level, const table *t, const table_entry *e)
{
    // Print the key edge
    if (e->key == NULL) {
        print_edge(indent_level, e, e->key, "k", "key", NULL);
    } else {
        if (t->key_kill_func) {
            print_edge(indent_level, e, e->key, "k", "key", "color=red");
        } else {
            print_edge(indent_level, e, e->key, "k", "key", "color=red style=dashed");
        }
    }

    // Print the value edge
    if (e->value == NULL) {
        print_edge(indent_level, e, e->value, "v", "value", NULL);
    } else {
        if (t->value_kill_func) {
            print_edge(indent_level, e, e->value, "v", "value", "color=red");
        } else {
            print_edge(indent_level, e, e->value, "v", "value", "color=red style=dashed");
        }
    }
}

// Internal function to print nodes and edges of all table entries in dot format.
static void print_entries(int indent_level, const table *t, inspect_callback key_print_func,
                          inspect_callback value_print_func)
{
    if (t->entries == NULL) {
        return;
    }
    dlist *l = t->entries;
    dlist_pos p = dlist_first(l);
    while (!dlist_is_end(l, p)) {
        table_entry *e = dlist_inspect(l, p);
        print_element_node(indent_level, e);
        print_key_value_nodes(indent_level, e, key_print_func, value_print_func);
        print_key_value_edges(indent_level, t, e);
        p = dlist_next(l, p);
    }
}

// Create an escaped version of the input string. The most common
// control characters - newline, horizontal tab, backslash, and double
// quote - are replaced by their escape sequence. The returned pointer
// must be deallocated by the caller.
static char *escape_chars(const char *s)
{
    int i, j;
    int escaped = 0; // The number of chars that must be escaped.

    // Count how many chars need to be escaped, i.e. how much longer
    // the output string will be.
    for (i = escaped = 0; s[i] != '\0'; i++) {
        if (s[i] == '\n' || s[i] == '\t' || s[i] == '\\' || s[i] == '\"') {
            escaped++;
        }
    }
    // Allocate space for the escaped string. The variable i holds the input
    // length, escaped how much the string will grow.
    char *t = malloc(i + escaped + 1);

    // Copy-and-escape loop
    for (i = j = 0; s[i] != '\0'; i++) {
        // Convert each control character by its escape sequence.
        // Non-control characters are copied as-is.
        switch (s[i]) {
        case '\n': t[i+j] = '\\'; t[i+j+1] = 'n';  j++; break;
        case '\t': t[i+j] = '\\'; t[i+j+1] = 't';  j++; break;
        case '\\': t[i+j] = '\\'; t[i+j+1] = '\\'; j++; break;
        case '\"': t[i+j] = '\\'; t[i+j+1] = '\"'; j++; break;
        default:   t[i+j] = s[i]; break;
        }
    }
    // Terminal the output string
    t[i+j] = '\0';
    return t;
}

/**
 * first_white_spc() - Return pointer to first white-space char.
 * @s: String.
 *
 * Returns: A pointer to the first white-space char in s, or NULL if none is found.
 *
 */
static const char *find_white_spc(const char *s)
{
    const char *t = s;
    while (*t != '\0') {
        if (isspace(*t)) {
            // We found a white-space char, return a point to it.
            return t;
        }
        // Advance to next char
        t++;
    }
    // No white-space found
    return NULL;
}

/**
 * insert_table_name() - Maybe insert the name of the table src file in the description string.
 * @s: Description string.
 *
 * Parses the description string to find of if it starts with a c file
 * name. In that case, the file name of this file is spliced into the
 * description string. The parsing is not very intelligent: If the
 * sequence ".c:" (case insensitive) is found before the first
 * white-space, the string up to and including ".c" is taken to be a c
 * file name.
 *
 * Returns: A dynamic copy of s, optionally including with the table src file name.
 */
static char *insert_table_name(const char *s)
{
    // First, determine if the description string starts with a c file name
    // a) Search for the string ".c:"
    const char *dot_c = strstr(s, ".c:");
    // b) Search for the first white-space
    const char *spc = find_white_spc(s);

    bool prefix_found;
    int output_length;

    // If both a) and b) are found AND a) is before b, we assume that
    // s starts with a file name
    if (dot_c != NULL && spc != NULL && dot_c < spc) {
        // We found a match. Output string is input + 3 chars + __FILE__
        prefix_found = true;
        output_length = strlen(s) + 3 + strlen(__FILE__);
    } else {
        // No match found. Output string is just input
        prefix_found = false;
        output_length = strlen(s);
    }

    // Allocate space for the whole string
    char *out = calloc(1, output_length + 1);
    strcpy(out, s);
    if (prefix_found) {
        // Overwrite the output buffer from the ":"
        strcpy(out + (dot_c - s + 2), " (");
        // Now out will be 0-terminated after "(", append the file name and ")"
        strcat(out, __FILE__);
        strcat(out, ")");
        // Finally append the input string from the : onwards
        strcat(out, dot_c + 2);
    }
    return out;
}

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
                          int indent_level)
{
    static int graph_number = 0;
    graph_number++;
    int il = indent_level;

    if (indent_level == 0) {
        // If this is the outermost datatype, start a graph and set up defaults
        printf("digraph TABLE_%d {\n", graph_number);

        // Specify default shape and fontname
        il++;
        iprintf(il, "node [shape=rectangle fontname=\"Courier New\"]\n");
        iprintf(il, "ranksep=0.01\n");
        iprintf(il, "subgraph cluster_nullspace {\n");
        iprintf(il+1, "NULL\n");
        iprintf(il, "}\n");
    }

    if (desc != NULL) {
        // Escape the string before printout
        char *escaped = escape_chars(desc);
        // Optionally, splice the source file name
        char *spliced = insert_table_name(escaped);

        // Use different names on inner description nodes
        if (indent_level == 0) {
            iprintf(il, "description [label=\"%s\"]\n", spliced);
        } else {
            iprintf(il, "\tcluster_list_%d_description [label=\"%s\"]\n", graph_number, spliced);
        }
        // Return the memory used by the spliced and escaped strings
        free(spliced);
        free(escaped);
    }

    if (indent_level == 0) {
        // Use a single "pointer" edge as a starting point for the
        // outermost datatype
        iprintf(il, "t [label=\"%04lx\" xlabel=\"t\"]\n", PTR2ADDR(t));
        iprintf(il, "t -> m%04lx\n", PTR2ADDR(t));
    }

    if (indent_level == 0) {
        // Put the user nodes in userspace
        iprintf(il, "subgraph cluster_userspace { label=\"User space\"\n");
        il++;

        // Iterate over the list to print the payload nodes
        dlist_pos p = dlist_first(t->entries);
        while (!dlist_is_end(t->entries, p)) {
            print_key_value_nodes(il, dlist_inspect(t->entries, p), key_print_func,
                                  value_print_func);
            // Advance
            p = dlist_next(t->entries, p);
        }

        // Close the subgraph
        il--;
        iprintf(il, "}\n");
    }

    // Print the subgraph to surround the DList content
    iprintf(il, "subgraph cluster_table_%d { label=\"Table\"\n", graph_number);
    il++;

    // Output the head node
    print_head_node(il, t);

    // Output the edges from the head
    print_head_edge(il, t);

    if (t->entries) {
        // First, ask the dlist to output its internal structure.
        dlist_print_internal(t->entries, NULL, NULL, il);
    }

    // Close the subgraph
    il--;
    iprintf(il, "}\n");

    // Next, print each element stored in the list
    print_entries(il, t, key_print_func, value_print_func);

    if (indent_level == 0) {
        // Termination of graph
        printf("}\n");
    }
}
