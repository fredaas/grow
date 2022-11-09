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
    char **buff = (char **)malloc(sizeof(char *) * size);
    for (int i = 0; i < size; i++)
        buff[i] = (char *)malloc(sizeof(char) * length);
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

/**
 * @brief List all files within a directory
 *        sorted in ascending order
 *
 * @param path Absolute path.
 *
 * @returns A pointer to the allocated string buffer containing the
 *          directory contents.
 */
char **listdir(char *path)
{
    DIR *d = opendir(path);
    if (!d)
        return NULL;
    char **buff = string_buff_alloc(256, PATH_MAX);
    int i = 0;
    struct dirent *dir;
    while ((dir = readdir(d)) != NULL)
    {
        char *dirname = dir->d_name;
        if (strcmp(dirname, ".") == 0 || strcmp(dirname, "..") == 0)
            continue;
        char pathname[PATH_MAX];
        sprintf(pathname, "%s/%s", path, dirname);
        strcpy(buff[i++], pathname);
    }
    strcpy(buff[i], "\0");
    closedir(d);
    qsort(buff, i, sizeof(char *), qsort_string_asc);
    return buff;
}

/**
 * @brief Extract the basename from a path
 *
 * If the path ends with a trailing '/', and empty string is
 * returned.
 *
 * @param basename The path basename.
 * @param path     The path to process.
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
void search(char *path, int depth)
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

    char **buff = listdir(path);
    char *item = buff[0];
    int i = 0;
    while (item[0] != '\0')
    {
        search(item, depth + 1);
        item = buff[++i];
    }
    string_buff_free(buff);
}

int main(int argc, char **argv)
{
    argc--;

    if (argc == 0)
    {
        printf("[ERROR] Please provide a directory as argument.");
        exit(1);
    }

    char path[256];
    strcpy(path, argv[1]);

    char *abspath = realpath(path, NULL);

    search(abspath, 0);

    free(abspath);

    return 0;
}