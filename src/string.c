#include "grow.h"

/**
 * @brief Allocate a string buffer
 *
 * @param size   Number of strings to allocate.
 * @param length Length of each string.
 *
 * @returns A pointer to the allocated string buffer.
 */
char **string_buff_alloc(int size, int length)
{
    char **buff = (char **)calloc(size, sizeof(char *));
    if (length > 0)
    {
        for (int i = 0; i < size; i++)
            buff[i] = (char *)malloc(length * sizeof(char));
    }
    return buff;
}

/**
 * @brief Deallocate a string buffer
 *
 * @param buff Buffer to deallocate.
 */
void string_buff_free(char **buff)
{
    char *item = NULL;
    int i = 0;
    while ((item = buff[i++]) != NULL)
        free(item);
    free(buff);
}

/**
 * @brief Print a string buffer
 * 
 * @param buff Buffer to print.
 */
void string_buff_print(char **buff)
{
    char *item = NULL;
    int i = 0;
    while ((item = buff[i++]) != NULL)
        printf("%s\n", item);
}

/**
 * @brief Read a single line from an input stream
 * 
 * @param buff   Buffer to store the result.
 * @param stream Input stream.
 * 
 * @returns 0 if EOF is reached, otherwise 1.
 */
int string_readline(char *buff, FILE *stream)
{
    char c;
    int i = 0;
    while ((c = fgetc(stream)) != EOF)
    {
        if (c == '\n')
            break;
        buff[i++] = c;
    }
    buff[i] = '\0';
    return c != EOF;
}

/**
 * @Initialize a string stack
 * 
 * @returns A pointer to the StringStack struct.
 */
StringStack *stack_init(void)
{
    StringStack *stack = (StringStack *)malloc(sizeof(StringStack));
    stack->head = 0;
    stack->max_size = 256;
    for (int i = 0; i < stack->max_size; i++)
        stack->items[i][0] = '\0';
    return stack;
}

/**
 * @brief Push a value on the stack
 * 
 * @param stack Stack pointer.
 * @param value Value to push on the stack.
 */
void stack_push(StringStack *stack, char *value)
{
    /* We've reached the stack limit */
    if (stack->head == stack->max_size)
        return;
    strcpy(stack->items[stack->head++], value);
}

/**
 * @brief Delete values from the top of the stack
 * 
 * @param stack Stack pointer.
 * @param delta Number of elements to delete from the stack.
 */
void stack_delete(StringStack *stack, int delta)
{
    /* We're trying to delete items that doesn't exist */
    if (stack->head - delta < 0)
        return;
    stack->head -= delta;
    stack->items[stack->head][0] = '\0';
}

/**
 * @brief Empty the stack
 * 
 * @param stack Stack pionter.
 */
void stack_flush(StringStack *stack)
{
    stack->head = 0;
    stack->items[stack->head][0] = '\0';
}

/**
 * @brief Print the stack contents
 * 
 * @param stack Stack pointer.
 */
void stack_print(StringStack *stack)
{
    int i = stack->head;
    while (--i >= 0)
        printf("%s\n", stack->items[i]);
}

/**
 * @brief Join all stack items into a string separated by '/'
 * 
 * @param stack Stack pointer.
 * @param buff  Buffer to store the result.
 */
void stack_join(StringStack *stack, char *buff)
{
    int i = 0;
    while (i < stack->head)
    {
        char name[PATH_MAX + 1];
        sprintf(name, "%s/", stack->items[i]);
        strcat(buff, name);
        i++;
    }
    /* Remove trailing slash */
    buff[max(0, strlen(buff) - 1)] = '\0';
}

/**
 * @brief Calculate the indentation of a string
 * 
 * @param s String.
 * 
 * @returns The string indentation.
 */
int string_indent(char *s)
{
    int i = 0;
    while (s[i++] == ' ');
    return i - 1;
}

/**
 * @brief Check if a string contains an item
 * 
 * @param s    String.
 * @param item Item to check for.
 * 
 * @returns 1 if the item exists, otherwise 0.
 */
int string_contains(char *s, char item)
{
    char c;
    int i = 0;
    while ((c = s[i++]) != '\0')
    {
        if (c == item)
            return 1;
    }
    return 0;
}

/**
 * @brief Strip items from a string
 * 
 * @param s     String.
 * @param items Items to remove from the string.
 */
void string_strip(char *s, char *items)
{
    char buff[PATH_MAX];
    strcpy(buff, s);
    int size = strlen(buff);
    if (!size)
        return;
    int j = size - 1;
    while (j >= 0 && string_contains(items, buff[j]))
        j--;
    buff[j + 1] = '\0';
    int i = 0;
    while (i < j && string_contains(items, buff[i]))
        i++;
    strcpy(s, buff + i);
}

/**
 * @brief Check if a string is empty
 * 
 * @param s String.
 * 
 * @returns 1 if the string is empty, otherwise 0.
 */
int string_isempty(char *s)
{
    char c;
    int i = 0;
    while ((c = s[i++]) != '\0')
    {
        if ((c != ' ') && (c != '\t') && (c != '\n'))
            return 0;
    }
    return 1;
}

/**
 * @brief Check for inconsistent string indentation
 * 
 * @param s     String.
 * @param sinfo StringInfo struct.
 */
void string_assert_indent(char *s, StringInfo *sinfo)
{
    int indent = string_indent(s);
    if (sinfo->base_indent > 0)
    {
        if ((indent % sinfo->base_indent != 0) ||
            ((indent - sinfo->prev_indent) > sinfo->base_indent))
        {
            LOG_ERROR("Inconsistent indentation.");
            exit(1);
        }
    }
    else
    {
        sinfo->base_indent = indent;
    }
    sinfo->prev_indent = indent;
}
