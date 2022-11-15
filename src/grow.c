#include "grow.h"

char **parse_argv(int argc, char **argv)
{
    char **buff = string_buff_alloc(256, 0);
    if (argc == 1)
    {
        char *path = realpath(".", NULL);
        path_listdir(buff, path);
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

void print_directory(char **buff)
{
    char *item = NULL;
    int i = 0;
    while ((item = buff[i++]) != NULL)
    {
        char *path = realpath(item, NULL);
        path_print_directory(path, 0);
        free(path);
    }
}

int main(int argc, char **argv)
{
    // log_set_level();

    // char **buff = string_buff_alloc(256, 0);
    // StringInfo *sinfo = path_read_stream(buff, stdin);
    // path_create_directory(buff, sinfo);

    char **buff = parse_argv(argc, argv);
    print_directory(buff);
    string_buff_free(buff);

    return 0;
}
