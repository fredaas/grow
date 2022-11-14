#ifndef GROW_H
#define GROW_H

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
#define min(a, b) ((a) <= (b) ? (a) : (b))

/* log.c */
#define LOG_TYPE_DEBUG   4
#define LOG_TYPE_INFO    3
#define LOG_TYPE_WARNING 2
#define LOG_TYPE_ERROR   1

#define LOG_ERROR(...) log_message(LOG_TYPE_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...) log_message(LOG_TYPE_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARNING(...) log_message(LOG_TYPE_WARNING, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_DEBUG(...) log_message(LOG_TYPE_DEBUG, __FILE__, __LINE__, __VA_ARGS__)

void log_set_level(void);
void log_message(int type, char *filename, int lineno, const char *message, ...);

/* string.c */
typedef struct StringStack StringStack;
struct StringStack {
    int head;
    int max_size;
    char items[256][PATH_MAX];
};

typedef struct StringInfo StringInfo;
struct StringInfo {
    int base_indent;
    int prev_indent;
    int lineno;
};

char **string_buff_alloc(int size, int length);
void string_buff_free(char **buff);
void string_buff_print(char **buff);
int string_readline(char *buff, FILE *stream);
StringStack *stack_init(void);
void stack_push(StringStack *stack, char *value);
void stack_delete(StringStack *stack, int delta);
void stack_flush(StringStack *stack);
void stack_print(StringStack *stack);
void stack_join(StringStack *stack, char *buff);
int string_indent(char *s);
int string_contains(char *s, char symbol);
void string_strip(char *s, char *token);
int string_isempty(char *s);
void string_assert_indent(StringInfo *si, char *s);

/* sort.c */
int sort_2dchar_asc(const void *a, const void *b);

/* path.c */
int isdir(const char *path);
char **listdir(char **buff, char *path);
void path_basename(char *basename, char *path);
void print_directory(char *path, int depth);
void parse_directory(char **buff);

#endif /* GROW_H */
