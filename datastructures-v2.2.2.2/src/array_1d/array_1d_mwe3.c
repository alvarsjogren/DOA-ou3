#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <array_1d.h>

/*
 * Minimum working example 3 for array_1d.c.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *
 * Version information:
 *   v1.0  2018-01-28: First public version.
 *   v1.1  2018-04-03: Split into simpler, shorter versions.
 *                     Removed use of automatic, magic, free_handler.
 *   v1.2  2023-01-14: Added printouts at start/end of main.
 *   v1.3  2024-03-14: Added explicit create/kill functions.
 */

#define VERSION "v1.3"
#define VERSION_DATE "2024-03-13"

// Struct with month name and number of days.
typedef struct month {
    char *name;
    int days;
} month;

// Months are stored via void pointers. Convert the given pointer and
// print the dereferenced values.
void print_month(const void *data)
{
    // Convert void pointer to pointer to month.
    const month *m=data;
    printf("(%s, %d)", m->name, m->days);
}

// Allocate space for and populate a month structure
month *month_create(const char *name, int days)
{
    // Allocate memory for a month structure.
    month *m = malloc(sizeof(*m));
    // Allocate memory for the month name.
    m->name = calloc(strlen(name) + 1, sizeof(char));
    // Copy the string.
    strcpy(m->name, name);
    // Set days.
    m->days = days;
    return m;
}

// Function to free both month structure and char * with name.
void month_kill(void *data)
{
    // Convert void pointer to pointer to month.
    month *m=data;
    free(m->name);
    free(m);
}

int main(void)
{
    printf("%s, %s %s: Create month array without kill_function.\n",
           __FILE__, VERSION, VERSION_DATE);
    printf("Code base version %s (%s).\n\n", CODE_BASE_VERSION, CODE_BASE_RELEASE_DATE);

    const char *month_names[12] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
    int month_days[12] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    // Create an array with 12 positions.
    array_1d *a = array_1d_create(1, 12, NULL);

    for (int i=array_1d_low(a); i<=array_1d_high(a); i++) {
        // Allocate and populate a month structure.
        month *m=month_create(month_names[i - 1], month_days[i - 1]);
        // Set value in array.
        array_1d_set_value(a,m,i);
    }
    printf("Array after inserting 12 month structures (Jan, 31), ..., (Dec, 31):\n");
    array_1d_print(a, print_month);

    // Empty the array.
    for (int i=array_1d_low(a); i<=array_1d_high(a); i++) {
        if (array_1d_has_value(a,i)) {
            month *v=array_1d_inspect_value(a,i);
            month_kill(v);
        }
    }
    array_1d_kill(a);

    printf("\nNormal exit.\n\n");
    return 0;
}
