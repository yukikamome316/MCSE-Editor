#ifndef MSSCMP_PROTYPE_H
#define MSSCMP_PROTYPE_H
#ifndef _MSC_VER

#include <stdio.h>
#include <errno.h>
#include <stddef.h>

int fopen_s(FILE** pFile,const char *filename,const char *mode);
int strcat_s(char *strDestination,size_t numberOfElements,const char *strSource);
int strcpy_s(char *restrict dest, size_t destsz, const char *restrict src);
int strerror_s( char *buf, size_t bufsz, int errnum );

#endif /* !defined(_MSC_VER) */
#endif /* !defined(MSSCMP_PROTYPE_H) */