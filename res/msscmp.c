#include <tiny_stdlib.h>
#include "msscmp.h"

FILE *fp;
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

//Read 32bit integer by file pointer
uint32_t readFile32bit(FILE *fp)
{
    return fgetc(fp) << 0x18 |
           fgetc(fp) << 0x10 |
           fgetc(fp) << 0x08 |
           fgetc(fp);
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
    uint32_t fileTable, entryCount;
    uint32_t foldNameOffset, fileNameOffset;
    uint32_t fileInfoOffset, offset, sampleRate, size;
    uint32_t i, j;
    uint32_t realfilename_length = 0;
    char filename[300], foldname[300];
    char realfilename[600] = {'t', 'm', 'p', '/', 0};
    FILE *destfp;
    char *pathParts[30], tmppath[300];
    int pathParts_size = 0;
    char *buf;

    fp = fopen(path, "rb");
    if (fp == NULL)
    {
        error = 1;
        printf("Failed to open target file: %s", path);
    }
    fseek(fp, 0x00000018, SEEK_SET);
    fileTable = readFile32bit(fp);
    fseek(fp, 0x00000034, SEEK_SET);
    entryCount = readFile32bit(fp);
    _mkdir("tmp");

    for (i = 0; i < entryCount; i++)
    {
        fseek(fp, fileTable, SEEK_SET);
        foldNameOffset = readFile32bit(fp);
        fileInfoOffset = readFile32bit(fp);

        fseek(fp, fileInfoOffset + 4, SEEK_SET);
        fileNameOffset = readFile32bit(fp) + fileInfoOffset;
        offset = readFile32bit(fp);
        skipRead(fp, 8);
        sampleRate = readFile32bit(fp);
        size = readFile32bit(fp);

        fseek(fp, foldNameOffset, SEEK_SET);
        fgets(foldname, 300, fp);
        fseek(fp, fileNameOffset, SEEK_SET);
        fgets(filename, 300, fp);

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

        fseek(fp, offset, SEEK_SET);
        buf=malloc(size);
        fread(buf,1,size,fp);
        fwrite(buf,1,size,destfp);
        free(buf);
        fclose(destfp);

        fileTable += 8;
    }
}