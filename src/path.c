#include "grow.h"

/**
 * @brief Check if a file is a directory
 *
 * @param path Absolute path to the file.
 *
 * @returns 1 if the file is a directory, otherwise 0.
 */
int path_isdir(const char *path)
{
    struct stat sb;
    return (stat(path, &sb) == 0) && S_ISDIR(sb.st_mode);
}

/**
 * @brief List all files within a directory sorted in ascending order
 *
 * @param buff Buffer to store the results.
 * @param path Absolute path to the directory.
 *
 * @returns A pointer to the allocated string buffer containing the
 *          directory contents.
 *
 * @remark It's left up to the caller to free the heap memory
 *         allocated by this function.
 */
char **path_listdir(char **buff, char *path)
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
    qsort(buff, i, sizeof(char *), sort_string_buff_asc);
    return buff;
}

/**
 * @brief Extract the basename from a path
 *
 * @param basename Buffer to store the result.
 * @param path     Path to process.
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
 * @param path  Absolute path to the directory.
 * @param depth Current depth of the abstract tree created 
 *              by parsing the directory structure, starting at 0.
 */
void path_print_directory(char *path, int depth)
{
    char basename[PATH_MAX];
    path_basename(basename, path);

    if (path_isdir(path))
    {
        printf("%*s%s/\n", depth * 4, "", basename);
    }
    else
    {
        printf("%*s%s\n", depth * 4, "", basename);
        return;
    }

    char **buff = string_buff_alloc(256, 0);

    path_listdir(buff, path);
    char *item = NULL;
    int i = 0;
    while ((item = buff[i++]) != NULL)
        path_print_directory(item, depth + 1);

    string_buff_free(buff);
}

/**
 * @brief Read an input stream into a buffer
 * 
 * @param buff    Buffer to store the result.
 * @param stream  Input stream.
 * 
 * @remark It's left up to the caller to free the heap memory
 *         allocated by this function.
 */
StringInfo *path_read_stream(char **buff, FILE *stream)
{
    StringInfo *sinfo = (StringInfo *)calloc(1, sizeof(StringInfo));
    char line[PATH_MAX];
    int i = 0;
    while (string_readline(line, stream))
    {
        if (string_isempty(line))
            continue;
        string_assert_indent(line, sinfo);
        buff[i] = (char *)malloc(PATH_MAX * sizeof(char));
        strcpy(buff[i], line);
        i++;
    }
    return sinfo;
}

/**
 * @brief Create a directory structure from an AST representation
 * 
 * @param buff  AST representation.
 * @param sinfo StringInfo struct.
 */
void path_create_directory(char **buff, StringInfo *sinfo)
{
    StringStack *stack = stack_init();
    char path[PATH_MAX];
    char parent[PATH_MAX];
    char *item;
    int i = 0;
    while ((item = buff[i++]) != NULL)
    {
        /* Normalize indent */
        int indent = string_indent(item) / sinfo->base_indent;
        string_strip(item, " ");
        /* This node is a root level node */
        if (indent == 0)
        {
            stack_flush(stack);
            printf("%s\n", item);
        }
        /* This node is one level down from the previous, store its parent */
        else if (indent - stack->head > 0)
        {
            stack_push(stack, parent);
            stack_join(stack, path);
            printf("%s/%s\n", path, item);
        }
        /* This node is on the same level as the previous */
        else if (indent - stack->head == 0)
        {
            stack_join(stack, path);
            printf("%s/%s\n", path, item);
        }
        /* This node is one or more levels up from the previous */
        else
        {
            stack_delete(stack, stack->head - indent);
            stack_join(stack, path);
            printf("%s/%s\n", path, item);
        }
        memset(path, 0, PATH_MAX * sizeof(char));
        strcpy(parent, item);
    }
    free(stack);
}
