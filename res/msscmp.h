#ifndef MSSCMP_H
#define MSSCMP_H

#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <direct.h>

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
} Entry;

typedef struct _File{
    FILE* fp;
    uint32_t entryCount;
    uint32_t filetableOffset;
    Entry *entries;
} File;



void __attribute__((dllexport)) extractMsscmp (const char* path);



#ifdef __cplusplus
}
#endif

#endif