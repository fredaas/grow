#include "grow.h"

/**
 * @brief Allocate a string buffer
 *
 * @param size   The number of strings to allocate.
 * @param length The length of each string.
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
 * @param buff The buffer to deallocate.
 */
void string_buff_free(char **buff)
{
    char *item = NULL;
    int i = 0;
    while ((item = buff[i++]) != NULL)
        free(item);
    free(buff);
}

void string_buff_print(char **buff)
{
    char *item = NULL;
    int i = 0;
    while ((item = buff[i++]) != NULL)
        printf("%s\n", item);
}

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

StringStack *stack_init(void)
{
    StringStack *stack = (StringStack *)malloc(sizeof(StringStack));
    stack->head = 0;
    stack->max_size = 256;
    for (int i = 0; i < stack->max_size; i++)
        stack->items[i][0] = '\0';
    return stack;
}

void stack_push(StringStack *stack, char *value)
{
    /* We've reached the stack limit */
    if (stack->head == stack->max_size)
        return;
    strcpy(stack->items[stack->head++], value);
}

void stack_delete(StringStack *stack, int delta)
{
    /* We're trying to delete items that doesn't exist */
    if (stack->head - delta < 0)
        return;
    stack->head -= delta;
    stack->items[stack->head][0] = '\0';
}

void stack_flush(StringStack *stack)
{
    stack->head = 0;
    stack->items[stack->head][0] = '\0';
}

void stack_print(StringStack *stack)
{
    int i = stack->head;
    while (--i >= 0)
        printf("%s\n", stack->items[i]);
}

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

int string_indent(char *s)
{
    int i = 0;
    while (s[i++] == ' ');
    return i - 1;
}

int string_contains(char *s, char symbol)
{
    char c;
    int i = 0;
    while ((c = s[i++]) != '\0')
    {
        if (c == symbol)
            return 1;
    }
    return 0;
}

void string_strip(char *s, char *token)
{
    char buff[PATH_MAX];
    strcpy(buff, s);
    int size = strlen(buff);
    if (!size)
        return;
    int j = size - 1;
    while (j >= 0 && string_contains(token, buff[j]))
        j--;
    buff[j + 1] = '\0';
    int i = 0;
    while (i < j && string_contains(token, buff[i]))
        i++;
    strcpy(s, buff + i);
}

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

void string_assert_indent(StringInfo *sinfo, char *s)
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
