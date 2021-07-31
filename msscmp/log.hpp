#ifndef LOG_H
#define LOG_H

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>

#define DF_SET 0
#define DF_GET 1
#define DF_CLOSE 1
#define DF_NONE 0

#ifdef _cplusplus
extern "C" {
#endif

void open_log_file();
void log_print(const char *fmt, ...);

#ifdef _cplusplus
}
#endif

#endif