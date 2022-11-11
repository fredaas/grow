#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define max(a, b) ((a) >= (b) ? (a) : (b))

/**
 * @brief Comparison function for sorting a
 *        2D array of strings in ascending order
 *
 * @returns Values defined by strcmp.
 */
int qsort_string_asc(const void *a, const void *b)
{
    const char *x = *(const char **)a;
    const char *y = *(const char **)b;
    return strcmp(x, y);
}

/**
 * @brief Check if a file is a directory
 *
 * @param path Absolute path to the file.
 *
 * @returns 1 if the file is a directory, otherwise 0.
 */
int isdir(const char *path)
{
    struct stat sb;
    return (stat(path, &sb) == 0) && S_ISDIR(sb.st_mode);
}

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

/**
 * @brief List all files within a directory
 *        sorted in ascending order
 *
 * @param buff String buffer to store the results.
 * @param path Absolute path to the directory.
 *
 * @returns A pointer to the allocated string buffer containing the
 *          directory contents.
 *
 * @remark It's left up to the caller to free the heap memory
 *         allocated by this function.
 */
char **listdir(char **buff, char *path)
{
    DIR *d = opendir(path);
    if (!d)
        return NULL;
    int i = 0;
    struct dirent *dir;
    while ((dir = readdir(d)) != NULL)
    {
        char *dirname = dir->d_name;
        if (strcmp(dirname, ".") == 0 || strcmp(dirname, "..") == 0)
            continue;
        char pathname[PATH_MAX];
        sprintf(pathname, "%s/%s", path, dirname);
        buff[i] = (char *)malloc(PATH_MAX * sizeof(char));
        strcpy(buff[i++], pathname);
    }
    closedir(d);
    qsort(buff, i, sizeof(char *), qsort_string_asc);
    return buff;
}

/**
 * @brief Extract the basename from a path
 *
 * @param basename The path basename.
 * @param path     The path to process.
 *
 * @remark If the path ends with a trailing '/', and empty string is
 *         returned.
 */
void path_basename(char *basename, char *path)
{
    int i = strlen(path);
    while (i >= 0 && (path[i]) != '/')
        i--;
    strcpy(basename, path + i + 1);
}

/**
 * @brief Perform a DFS search on a directory
 *
 * @param path  The absolute path to the file.
 * @param depth The current depth of the abstract tree
 *              created by parsing the directory structure, starting at 0.
 */
void print_directory(char *path, int depth)
{
    char basename[PATH_MAX];
    path_basename(basename, path);

    if (isdir(path))
    {
        printf("%*s%s/\n", depth * 4, "", basename);
    }
    else
    {
        printf("%*s%s\n", depth * 4, "", basename);
        return;
    }

    char **buff = string_buff_alloc(256, 0);

    listdir(buff, path);
    char *item = NULL;
    int i = 0;
    while ((item = buff[i++]) != NULL)
        print_directory(item, depth + 1);

    string_buff_free(buff);
}

/**
 * @brief Recursively print the contents of all directories
 *        passed as argument
 *
 * @param buff String buffer containg file arguments.
 */
void parse_directory(char **buff)
{
    char *item = NULL;
    int i = 0;
    while ((item = buff[i++]) != NULL)
    {
        char *path = realpath(item, NULL);
        print_directory(path, 0);
        free(path);
    }
}

/**
 * @brief Parse file arguments from the command line
 *
 * @param argc Argument count.
 * @param argv Arguments.
 *
 * @remark It's left up to the caller to free the heap memory
 *         allocated by this function.
 */
char **parse_argv(int argc, char **argv)
{
    char **buff = string_buff_alloc(256, 0);
    if (argc == 1)
    {
        char *path = realpath(".", NULL);
        listdir(buff, path);
    }
    else
    {
        char *item = NULL;
        int i = 1;
        int j = 0;
        while ((item = argv[i++]) != NULL)
        {
            buff[j] = (char *)malloc(PATH_MAX * sizeof(char));
            strcpy(buff[j++], item);
        }
        buff[j] = NULL;
    }
    return buff;
}

int readline(char *buff)
{
    char c;
    int i = 0;
    while ((c = fgetc(stdin)) != EOF)
    {
        if (c == '\n')
            break;
        buff[i++] = c;
    }
    buff[i] = '\0';
    return c != EOF;
}

typedef struct StringStack StringStack;

struct StringStack {
    int head;
    int max_size;
    char items[256][PATH_MAX];
};

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
        char name[PATH_MAX];
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

void tree_parse_notation(void)
{
    StringStack *stack = stack_init();
    char curr[PATH_MAX];
    char prev[PATH_MAX];
    char path[PATH_MAX];
    while (readline(curr))
    {
        int indent  = string_indent(curr);
        string_strip(curr, " ");
        /* This node is a root level node */
        if (indent == 0)
        {
            stack_flush(stack);
            printf("%s\n", curr);
        }
        /* This node is one level down from the previous, store its parent */
        else if (indent - stack->head > 0)
        {
            stack_push(stack, prev);
            stack_join(stack, path);
            printf("%s/%s\n", path, curr);
        }
        /* This node is on the same level as the previous */
        else if (indent - stack->head == 0)
        {
            stack_join(stack, path);
            printf("%s/%s\n", path, curr);
        }
        /* This node is one or more levels up from the previous */
        else
        {
            stack_delete(stack, stack->head - indent);
            stack_join(stack, path);
            printf("%s/%s\n", path, curr);
        }
        memset(path, 0, PATH_MAX * sizeof(char));
        strcpy(prev, curr);
    }
    free(stack);
}

void create_tree(void)
{
    tree_parse_notation();
}

int main(int argc, char **argv)
{
    char **buff = parse_argv(argc, argv);
    parse_directory(buff);
    string_buff_free(buff);

    // create_tree();

    return 0;
}
