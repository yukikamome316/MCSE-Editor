#ifndef LOG_H
#define LOG_H

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>

#define DF_SET 0
#define DF_GET 1
#define DF_CLOSE 1
#define DF_NONE 0

#define printf(fmt, arg...) _dprintf(fmt, ##arg)
#define wprintf(fmt, arg...) _dwprintf(fmt, ##arg)

FILE *debugfile(int mode, int reason);
void _dprintf(char *fmt, ...);
void _dwprintf(wchar_t *fmt, ...);

#endif