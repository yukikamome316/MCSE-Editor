#define MSSCMP_EXPORT
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
uint32_t readFile32bitBE(FILE *fp)
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
    FILE *fp;
    fopen_s(&fp,filename,"r");
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
    FILE *fp;
    fopen_s(&fp,filename, "w");
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
    Entry *entry;
    Offsets *offsets;
    Paths *paths;
    int pathPartsLen,i,j;
    char *pathParts[30],tmppath[600],*cw,*buf;
    FILE *destfp;

    fopen_s(&file.fp,path, "rb");
    if (file.fp == NULL)
    {
        error = 1;
        printf("Failed to open target file: %s", path);
    }

    fseek(file.fp, 0x00000018, SEEK_SET);
    file.filetableOffset = readFile32bitBE(file.fp);
    fseek(file.fp, 0x00000034, SEEK_SET);
    file.entryCount = readFile32bitBE(file.fp);
    _mkdir("tmp");
    file.entries=malloc(sizeof(Entry*)*file.entryCount);

    for (i = 0; i < file.entryCount; i++)
    {
        entry=malloc(sizeof(Entry));
        offsets=&entry->offsets;
        paths=&entry->paths;

        fseek(file.fp, file.filetableOffset, SEEK_SET);
        offsets->path = readFile32bitBE(file.fp);
        offsets->info = readFile32bitBE(file.fp);
        fseek(file.fp, offsets->info + 4, SEEK_SET);

        offsets->name = readFile32bitBE(file.fp) + offsets->info;
        offsets->data = readFile32bitLE(file.fp);
        skipRead(file.fp, 8);
        entry->sampleRate = readFile32bitBE(file.fp);
        entry->size = readFile32bitBE(file.fp);

        fseek(file.fp, offsets->path, SEEK_SET);
        fgets(paths->path, 300, file.fp);
        fseek(file.fp, offsets->name, SEEK_SET);
        fgets(paths->name, 300, file.fp);

        cw=paths->full;
        memset(cw,0,600);
        strcpy_s(cw,600, "tmp/");
        strcat_s(cw,600, paths->path);
        strcat_s(cw,600, "/");
        strcat_s(cw,600, paths->name);
        paths->fullLen = strlen(cw);
        for (j = 0; j < paths->fullLen; j++)
            if (paths->full[j] == '*')
                paths->full[j] = '_';

        //Make full directory
        pathPartsLen = split(pathParts, paths->path, '/');
        memset(tmppath, 0, 600);
        strcpy_s(tmppath,4, "tmp/");
        for (j = 0; j < pathPartsLen; j++)
        {
            strcat_s(tmppath,strlen(pathParts[j]), pathParts[j]);
            strcat_s(tmppath,1, "/");
            _mkdir(tmppath);
        }

        fopen_s(&destfp,paths->full, "wb");
        if (destfp == NULL)
        {
            error = 1;
            char errorbuffer[256];
            strerror_s(errorbuffer,256,errno);
            printf("Failed to open dest fp:%s\n", errorbuffer);
            printf("%s\n", paths->full);
            return;
        }

        fseek(file.fp, offsets->data, SEEK_SET);
        buf=malloc(entry->size);
        fread(buf,1,entry->size,file.fp);
        fwrite(buf,1,entry->size,destfp);
        free(buf);
        fclose(destfp);

        file.filetableOffset += 8;
        file.entries[i]=entry;
    }
    file.filetableOffset-=8*file.entryCount;
}