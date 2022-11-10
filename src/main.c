#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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

int main(int argc, char **argv)
{
    char **buff = parse_argv(argc, argv);

    parse_directory(buff);

    string_buff_free(buff);

    return 0;
}
