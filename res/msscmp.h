#ifndef MSSCMP_H
#define MSSCMP_H

#include <tiny_stdlib.h>

#ifdef __cplusplus
extern "C"{
#endif



typedef struct _Entry{
    uint32_t offset;
    uint32_t size;
    uint32_t sampleRate;
    char fullpath[200];
    char name[100];
    char path[100];
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