#include "grow.h"

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
    qsort(buff, i, sizeof(char *), sort_2dchar_asc);
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
