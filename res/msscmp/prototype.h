#ifndef MSSCMP_PROTYPE_H
#define MSSCMP_PROTYPE_H
#ifndef _MSC_VER

#include <stdio.h>
#include <errno.h>
#include <stddef.h>

errno_t fopen_s(FILE** pFile,const char *filename,const char *mode);
errno_t strcat_s(char *strDestination,size_t numberOfElements,const char *strSource);
errno_t strcpy_s(char *restrict dest, size_t destsz, const char *restrict src);
errno_t strerror_s( char *buf, size_t bufsz, errno_t errnum );

#endif /* !defined(_MSC_VER) */
#endif /* !defined(MSSCMP_PROTYPE_H) */