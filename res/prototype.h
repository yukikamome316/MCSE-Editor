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

typedef int* HANDLE;

int*    (__stdcall *GetProcAddress) (HANDLE module,const char* ProcName);
int      __stdcall  FreeLibrary     (HANDLE libModule);
uint32_t __stdcall  GetLastError    ();
HANDLE   __stdcall  LoadLibraryA    (char    *libFileName);
HANDLE   __stdcall  LoadLibraryW    (wchar_t *libFileName);



#ifdef UNICODE
    #define LoadLibrary LoadLibraryW
#else
    #define LoadLibrary LoadLibraryA
#endif



#endif /* !defined(_MSC_VER) */
#endif /* !defined(MSSCMP_PROTYPE_H) */