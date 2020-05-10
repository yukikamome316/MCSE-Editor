#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include "msscmp.h"

File file;
int error;

//Code by https://qiita.com/fireflower0/items/dc54f3ec1b3698a98b14
//Thanks for qiita user '@fireflower0'
int isDelimiter(char p, char delim)
{
    return p == delim;
}

int split(char *dst[], char *src, char delim)
{
    int count = 0;

    for (;;)
    {
        while (isDelimiter(*src, delim))
        {
            src++;
        }

        if (*src == '\0')
            break;

        dst[count++] = src;

        while (*src && !isDelimiter(*src, delim))
        {
            src++;
        }

        if (*src == '\0')
            break;

        *src++ = '\0';
    }
    return count;
}

//Read 32bit integer by file pointer (big endian)
uint32_t readFile32bit(FILE *fp)
{
    return fgetc(fp) << 0x18 |
           fgetc(fp) << 0x10 |
           fgetc(fp) << 0x08 |
           fgetc(fp);
}

//Read 32bit integer by file pointer (little endian)
uint32_t readFile32bitLE(FILE *fp)
{
    return fgetc(fp) << 0x00 |
           fgetc(fp) << 0x08 |
           fgetc(fp) << 0x10 |
           fgetc(fp) << 0x18 ;
}

//Skip reading
void skipRead(FILE *fp, size_t pos)
{
    for (int i = 0; i < pos; i++)
    {
        fgetc(fp);
    }
}

//check exist file
bool existFile(char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        return false;
    }
    fclose(fp);
    return true;
}

//create the file
bool createFile(char *filename)
{
    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        return 1;
    }
    fclose(fp);
    return 0;
}

//EXTERNED
//extract msscmp (Minecraft Sound Source CoMPressed ?)
void extractMsscmp(const char *path)
{
    uint32_t foldNameOffset, fileNameOffset;
    uint32_t fileInfoOffset, offset, sampleRate, size;
    uint32_t i, j;
    uint32_t realfilename_length = 0;
    char filename[300], foldname[300];
    char realfilename[600] = {'t', 'm', 'p', '/', 0};
    FILE *destfp;
    Entry *entry;
    char *pathParts[30], tmppath[300];
    int pathParts_size = 0;
    char *buf;

    file.fp = fopen(path, "rb");
    if (file.fp == NULL)
    {
        error = 1;
        printf("Failed to open target file: %s", path);
    }
    fseek(file.fp, 0x00000018, SEEK_SET);
    file.filetableOffset = readFile32bit(file.fp);
    fseek(file.fp, 0x00000034, SEEK_SET);
    file.entryCount = readFile32bit(file.fp);
    _mkdir("tmp");
    file.entries=malloc(sizeof(Entry)*file.entryCount);
    for (i = 0; i < file.entryCount; i++)
    {
        fseek(file.fp, file.filetableOffset, SEEK_SET);
        foldNameOffset = readFile32bit(file.fp);
        fileInfoOffset = readFile32bit(file.fp);

        entry=malloc(sizeof(Entry));
        fseek(file.fp, fileInfoOffset + 4, SEEK_SET);
        fileNameOffset = readFile32bit(file.fp) + fileInfoOffset;
        entry->offset = readFile32bitLE(file.fp);
        skipRead(file.fp, 8);
        entry->sampleRate = readFile32bit(file.fp);
        entry->size = readFile32bit(file.fp);

        fseek(file.fp, foldNameOffset, SEEK_SET);
        fgets(foldname, 300, file.fp);
        fseek(file.fp, fileNameOffset, SEEK_SET);
        fgets(filename, 300, file.fp);

        strcpy(realfilename + 4, foldname);
        strcat(realfilename + 4, "/");
        strcat(realfilename + 4, filename);
        ///'*' to '_'
        realfilename_length = strlen(realfilename);
        for (j = 0; j < realfilename_length; j++)
            if (realfilename[j] == '*')
                realfilename[j] = '_';

        //Make full directory
        pathParts_size = split(pathParts, foldname, '/');
        memset(tmppath, 0, sizeof(tmppath));
        strcpy(tmppath, "tmp/");
        for (j = 0; j < pathParts_size; j++)
        {
            strcat(tmppath, pathParts[j]);
            strcat(tmppath, "/");
            _mkdir(tmppath);
        }
        destfp = fopen(realfilename, "wb");
        if (destfp == NULL)
        {
            error = 1;
            printf("Failed to open dest fp:%s\n", strerror(errno));
            printf("%s\n", realfilename);
            return;
        }

        fseek(file.fp, offset, SEEK_SET);
        buf=malloc(size);
        fread(buf,1,size,file.fp);
        fwrite(buf,1,size,destfp);
        free(buf);
        fclose(destfp);

        file.filetableOffset += 8;
    }
    file.filetableOffset-=8*file.entryCount;
}