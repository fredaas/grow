#include "grow.h"

static int LOG_LEVEL = 2;

static char *log_label[] = {
    [LOG_TYPE_DEBUG]   = "DEBUG",
    [LOG_TYPE_INFO]    = "INFO",
    [LOG_TYPE_WARNING] = "WARNING",
    [LOG_TYPE_ERROR]   = "ERROR",
};

/**
 * @brief Log a message to stdout
 * 
 * @param type     Log output type.
 * @param filename Caller filename.
 * @param linenum  Caller line number.
 * @param message  Log message.
 */
void log_message(int type, char *filename, int linenum, const char *message, ...)
{
    if (type > LOG_LEVEL)
        return;
    va_list argptr;
    va_start(argptr, message);
    char buff[1024];
    sprintf(buff, "%s - %s - %d - %s\n", filename, log_label[type], linenum, message);
    vfprintf(stdout, buff, argptr);
    va_end(argptr);
}

/**
 * @brief Set the log level
 */
void log_set_level(void)
{
    char *value = getenv("LOG_LEVEL");
    if (value)
    {
        LOG_LEVEL = atoi(value);
        LOG_LEVEL = min(LOG_LEVEL, LOG_TYPE_DEBUG);
    }
}
