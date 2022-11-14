#include "grow.h"

StringInfo sinfo;

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

void string_buff_stream(char **buff, FILE *stream)
{
    char line[PATH_MAX];
    int i = 0;
    while (string_readline(line, stream))
    {
        if (string_isempty(line))
            continue;
        string_assert_indent(&sinfo, line);
        buff[i] = (char *)malloc(PATH_MAX * sizeof(char));
        strcpy(buff[i], line);
        i++;
    }
}

void string_buff_parse(char **buff)
{
    StringStack *stack = stack_init();
    char path[PATH_MAX];
    char parent[PATH_MAX];
    char *item;
    int i = 0;
    while ((item = buff[i++]) != NULL)
    {
        int indent = string_indent(item);
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
            stack_delete(stack, (stack->head - indent) / sinfo.base_indent);
            stack_join(stack, path);
            printf("%s/%s\n", path, item);
        }
        memset(path, 0, PATH_MAX * sizeof(char));
        strcpy(parent, item);
    }
    free(stack);
}

int main(int argc, char **argv)
{
    log_set_level();

    // char **buff = string_buff_alloc(256, 0);
    // string_buff_stream(buff, stdin);
    // string_buff_print(buff);
    // string_buff_parse(buff);

    char **buff = parse_argv(argc, argv);
    parse_directory(buff);
    string_buff_free(buff);

    return 0;
}
