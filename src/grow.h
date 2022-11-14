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

/* logging.c */
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

#endif /* GROW_H */
