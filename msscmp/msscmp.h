#ifndef MSSCMP_H
#define MSSCMP_H

#include <stdint.h>

#define msscmpDataAlign 0x00001000
#define msscmpDataStart 0x0001B000

#if defined(__TINYC__)||defined(__GNUC__)
  #define DLLOPT(a) __attribute__ ((a))
#else
  #define DLLOPT(a) __declspec(a)
#endif

#ifdef MSSCMP_EXPORT
  #include "log.h"
  #include "res/res.h"
  #include <direct.h>
  #include <sys/stat.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <stdbool.h>
  #include <math.h>
  #include <locale.h>
  #include <string.h>
  #include "prototype.h"
  #define DLLAPI DLLOPT(dllexport)
#else
  #define DLLAPI
#endif

#ifdef __cplusplus
extern "C"{
#endif



typedef struct _Offsets{
    uint32_t path;
    uint32_t name;
    uint32_t data;
    uint32_t info;
} Offsets;

typedef struct _Paths{
    char path[300];
    char name[300];
    char full[600];uint32_t fullLen;
} Paths;

typedef struct _Entry{
    Offsets offsets;
    Paths paths;
    uint32_t size;
    uint32_t sampleRate;
    char * data;
} Entry;

typedef struct _File{
    FILE* fp;
    uint32_t entryCount;
    uint32_t filetableOffset;
    Entry **entries;
} File;


int  __stdcall DLLAPI extractMsscmp(const wchar_t *path);
int  __stdcall DLLAPI loadMsscmp(const wchar_t *path);
int  __stdcall DLLAPI saveMsscmp(const wchar_t *path);
int  __stdcall DLLAPI replaceEntryMsscmp(wchar_t *path, wchar_t *replacePath);
int  __stdcall DLLAPI wav2binka(wchar_t* wav,wchar_t* binka);
int  __stdcall DLLAPI binka2wav(wchar_t* binka,wchar_t* wav);


#ifdef __cplusplus
}
#endif

#endif