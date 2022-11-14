#include "grow.h"

/**
 * @brief Comparison function for sorting a
 *        2D array of strings in ascending order
 *
 * @returns Values defined by strcmp.
 */
int sort_2dchar_asc(const void *a, const void *b)
{
    const char *x = *(const char **)a;
    const char *y = *(const char **)b;
    return strcmp(x, y);
}
