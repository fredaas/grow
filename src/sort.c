#include "grow.h"

/**
 * @brief Callback for sorting a 2D string buffer in ascending order
 *
 * @returns Values defined by strcmp.
 */
int sort_string_buff_asc(const void *a, const void *b)
{
    const char *x = *(const char **)a;
    const char *y = *(const char **)b;
    return strcmp(x, y);
}
