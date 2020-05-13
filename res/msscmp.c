#define MSSCMP_EXPORT
#include "msscmp.h"

File file;
int error;
char *backup;

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
           fgetc(fp) << 0x18;
}

//Write 32bit integer by file pointer (big endian)
void writeFile32bitBE(FILE *fp,uint32_t val)
{
    fputc(val>>0x18,fp);
    fputc(val>>0x10,fp);
    fputc(val>>0x08,fp);
    fputc(val>>0x00,fp);
}

//Write 32bit integer by file pointer (little endian)
void writeFile32bitLE(FILE *fp,uint32_t val)
{
    fputc(val>>0x00,fp);
    fputc(val>>0x08,fp);
    fputc(val>>0x10,fp);
    fputc(val>>0x18,fp);
}

//Read file while to null
void readFileString(FILE *fp, char *dest, int max)
{
    for (int i = 0; i < max; i++)
    {
        dest[i] = fgetc(fp);
    }
}

//Skip reading
void skipRead(FILE *fp, int pos)
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
    fopen_s(&fp, filename, "r");
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
    fopen_s(&fp, filename, "w");
    if (fp == NULL)
    {
        return 1;
    }
    fclose(fp);
    return 0;
}

//EXTERNED
// init system (for debug?)
void __stdcall DLLAPI init(){
    FILE* fp;
    AllocConsole();
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);

    setlocale(LC_ALL,"JPN");
}
//EXTERNED
//extract msscmp (Minecraft Sound Source CoMPressed ?)
int __stdcall DLLAPI extractMsscmp(const wchar_t *path)
{
    Entry *entry;
    Offsets *offsets;
    Paths *paths;
    int pathPartsLen, i, j;
    char *pathParts[30], tmppath[600], *cw, *buf;
    FILE *destfp;

    _wfopen_s(&file.fp, path, L"rb");
    if (file.fp == NULL)
    {
        error = 1;
        wprintf(L"Failed to open target file: %s", path);
        return 1;
    }

    //backup
    backup = malloc(msscmpDataStart);
    if (backup == NULL)
    {
        error = 1;
        printf("Failed to Malloc msscmp backup\n");
        return 1;
    }
    i = 0;
    while (i != msscmpDataStart)
        backup[i++] = fgetc(file.fp);

    fseek(file.fp, 0x00000000, SEEK_SET);
    if (readFile32bitBE(file.fp) != 0x42414e4b)
    {
        error = 1;
        printf("Failed to Check msscmp Signeture\n");
        return 1;
    }
    fseek(file.fp, 0x00000018, SEEK_SET);
    file.filetableOffset = readFile32bitBE(file.fp);
    fseek(file.fp, 0x00000034, SEEK_SET);
    file.entryCount = readFile32bitBE(file.fp);
    _mkdir("tmp");
    file.entries = malloc(sizeof(Entry *) * file.entryCount);

    if (file.entryCount == 0)
    {
        error = 0;
        printf("Not found files\n");
        return 1;
    }

    for (i = 0; i < file.entryCount; i++)
    {
        entry = malloc(sizeof(Entry));
        if (entry == NULL)
        {
            printf("Failed to malloc entry\n");
            error = 1;
            return 1;
        }
        offsets = &entry->offsets;
        paths = &entry->paths;

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
        readFileString(file.fp, paths->path, 300);
        fseek(file.fp, offsets->name, SEEK_SET);
        readFileString(file.fp, paths->name, 300);

        cw = paths->full;
        memset(cw, 0, 600);
        sprintf_s(cw, 600, "tmp/%s/%s", paths->path, paths->name);
        paths->fullLen = strlen(cw);
        for (j = 0; j < paths->fullLen; j++)
            if (paths->full[j] == '*')
                paths->full[j] = '_';

        pathPartsLen = split(pathParts, paths->path, '/');
        memset(tmppath, 0, 600);
        strcpy_s(tmppath, 600, "tmp/");
        for (j = 0; j < pathPartsLen; j++)
        {
            strcat_s(tmppath, 600, pathParts[j]);
            strcat_s(tmppath, 600, "/");
            _mkdir(tmppath);
        }

        fopen_s(&destfp, paths->full, "wb");
        if (destfp == NULL)
        {
            error = 1;
            char errorbuffer[256];
            strerror_s(errorbuffer, 256, errno);
            printf("Failed to open dest fp:%s\n", errorbuffer);
            printf("%s\n", paths->full);
            return 1;
        }

        fseek(file.fp, offsets->data, SEEK_SET);
        buf = malloc(entry->size);
        fread(buf, 1, entry->size, file.fp);
        fwrite(buf, 1, entry->size, destfp);
        fclose(destfp);
        entry->data = buf;
        file.filetableOffset += 8;
        file.entries[i] = entry;
    }
    file.filetableOffset -= 8 * file.entryCount;
    return 0;
}

//EXTERNED (β)
//load msscmp to internal
int __stdcall DLLAPI loadMsscmp(const wchar_t *path)
{
    Entry *entry;
    Offsets *offsets;
    Paths *paths;
    int pathPartsLen, i, j;
    char *pathParts[30], tmppath[600], *cw, *buf;
    _wfopen_s(&file.fp,path,L"rb");
    if (file.fp == NULL)
    {
        error = 1;
        printf("Failed to open target file: %s", path);
        return 1;
    }

    //backup
    backup = malloc(msscmpDataStart);
    if (backup == NULL)
    {
        error = 1;
        printf("Failed to Malloc msscmp backup\n");
        return 1;
    }
    i = 0;
    while (i != msscmpDataStart)
        backup[i++] = fgetc(file.fp);

    fseek(file.fp, 0x00000000, SEEK_SET);
    if (readFile32bitBE(file.fp) != 0x42414e4b)
    {
        error = 1;
        printf("Failed to Check msscmp Signeture\n");
        return 1;
    }
    fseek(file.fp, 0x00000018, SEEK_SET);
    file.filetableOffset = readFile32bitBE(file.fp);
    fseek(file.fp, 0x00000034, SEEK_SET);
    file.entryCount = readFile32bitBE(file.fp);
    _mkdir("tmp");
    file.entries = malloc(sizeof(Entry *) * file.entryCount);

    if (file.entryCount == 0)
    {
        error = 0;
        printf("Not found files\n");
        return 1;
    }

    for (i = 0; i < file.entryCount; i++)
    {
        entry = malloc(sizeof(Entry));
        if (entry == NULL)
        {
            printf("Failed to malloc entry\n");
            error = 1;
            return 1;
        }
        offsets = &entry->offsets;
        paths = &entry->paths;

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
        readFileString(file.fp, paths->path, 300);
        fseek(file.fp, offsets->name, SEEK_SET);
        readFileString(file.fp, paths->name, 300);

        cw = paths->full;
        memset(cw, 0, 600);
        sprintf_s(cw, 600, "tmp/%s/%s", paths->path, paths->name);
        paths->fullLen = strlen(cw);
        for (j = 0; j < paths->fullLen; j++)
            if (paths->full[j] == '*')
                paths->full[j] = '_';

        pathPartsLen = split(pathParts, paths->path, '/');
        memset(tmppath, 0, 600);
        strcpy_s(tmppath, 600, "tmp/");
        for (j = 0; j < pathPartsLen; j++)
        {
            strcat_s(tmppath, 600, pathParts[j]);
            strcat_s(tmppath, 600, "/");
            _mkdir(tmppath);
        }
        fseek(file.fp, offsets->data, SEEK_SET);
        buf = malloc(entry->size);
        fread(buf, 1, entry->size, file.fp);
        entry->data = buf;
        file.filetableOffset += 8;
        file.entries[i] = entry;
    }
    file.filetableOffset -= 8 * file.entryCount;
    return 0;
}

//EXTERNED
//save msscmp
int __stdcall DLLAPI saveMsscmp(const wchar_t *path)
{
    FILE *fp = 0;
    int ret;
    char *tmp;
    int size;
    _wremove(path);
    _wfopen_s(&fp, path, L"wb+");
    if (fp == NULL)
    {
        char errorbuffer[256];
        strerror_s(errorbuffer, 256, errno);
        printf("Failed to open target file: %s\n", errorbuffer);
        return 1;
    }
    //--------------------------------------------------------------
    //               Write Msscmp
    //--------------------------------------------------------------
    //write header(raw)
    clearerr(fp);

    ret = fseek(fp, 0, SEEK_SET);
    if (ret != 0 || ferror(file.fp) != 0)
    {
        char errorbuffer[256];
        strerror_s(errorbuffer, 256, errno);
        printf("Failed to seek target file header: %s\n", errorbuffer);
        printf("ret = %d\n", ret);
        return 1;
    }
    ret = fwrite(backup, 0x0001B000, 1, fp);
    if (ret < 1 || ferror(file.fp) != 0)
    {
        char errorbuffer[256];
        strerror_s(errorbuffer, 256, errno);
        printf("Failed to write target file header: %s\n", errorbuffer, errno);
        printf("ret = %d\n", ret);
        return 1;
    }

    //write Datas
    uint32_t
        currentPos = 0x0001B000,
        currentPosBak = 0;
    for (int i = 0; i < file.entryCount; i++)
    {
        file.entries[i]->offsets.data=currentPos;
        ret = fseek(fp, currentPos, SEEK_SET);
        if (ret != 0 || ferror(fp) != 0)
        {
            char errorbuffer[256];
            strerror_s(errorbuffer, 256, errno);
            printf("\nFailed to seek target file: %s\n", errorbuffer);
            return 1;
        }
        ret = fwrite(file.entries[i]->data, file.entries[i]->size, 1, fp);
        if (ret < 1 || ferror(fp) != 0)
        {
            char errorbuffer[256];
            strerror_s(errorbuffer, 256, errno);
            printf("\nFailed to write target file entry: %s\n", errorbuffer, errno);
            return 1;
        }
        currentPos += ((int)ceil((float)file.entries[i]->size / msscmpDataAlign)) * msscmpDataAlign;
    }

    //Edit datas entry and size
    uint32_t fileinfo;
    for (int i = 0; i < file.entryCount; i++)
    {
        ret = fseek(fp, file.filetableOffset + i * 8, SEEK_SET);
        if (ret != 0 || ferror(file.fp) != 0)
        {
            char errorbuffer[256];
            strerror_s(errorbuffer, 256, errno);
            printf("Failed to seek target file table: %s\n", errorbuffer);
            printf("ret = %d\n", ret);
            return 1;
        }
        skipRead(fp, 4); //Skip path offset
        // gotofile info
        fileinfo=readFile32bitBE(fp)+4;
        //printf("fileinfo %08x\n",fileinfo);
        ret = fseek(fp, fileinfo, SEEK_SET);
        if (ret != 0 || ferror(file.fp) != 0)
        {
            char errorbuffer[256];
            strerror_s(errorbuffer, 256, errno);
            printf("Failed to seek target file info: %s\n", errorbuffer);
            printf("ret = %d\n", ret);
            return 1;
        }
        skipRead(fp,4);//skip name offset
        writeFile32bitLE(fp,file.entries[i]->offsets.data);
        skipRead(fp,8);//Skip ????
        writeFile32bitLE(fp,file.entries[i]->sampleRate);
        writeFile32bitLE(fp,file.entries[i]->size);

    }
    //end
    fclose(fp);
    return 0;
}

//ENTERNED
//replace msscmp entry data
int  __stdcall DLLAPI replaceEntryMsscmp(wchar_t *_path, wchar_t *replacePath)
{
    // Get Entry Number
    int i;
    size_t converted;
    char path[wcslen(_path)*2];
    wcstombs_s(&converted,path,wcslen(_path)*2,_path,wcslen(_path)*2);

    for(i=0;i<file.entryCount;i++){
        if(!strcmp(path,file.entries[i]->paths.full+4)){
            break;
        }
    }
    // Open replace Path in `rb`
    FILE *fp=NULL;
    _wfopen_s(&fp,replacePath,L"rb");
    if(fp==NULL){
        char error[256];
        strerror_s(error,256,errno);
        printf("Failed to open file replace: %s",error);
        return 1;
    }
    // Get file size of `replace Path` to `fsiz`
    struct stat fileInfo;
    if(fstat(fileno(fp),&fileInfo)!=0){
        char error[256];
        strerror_s(error,256,errno);
        printf("Failed to get file info: %s",error);
        return 1;
    }
    int fsiz=fileInfo.st_size;
    // Read file to `data` by `fp`
    char *data=malloc(fsiz);
    if(data==NULL){
        char error[256];
        strerror_s(error,256,errno);
        printf("Failed to malloc data: %s",error);
        return 1;
    }
    if(fread(data,fsiz,1,fp)<1){
        char error[256];
        strerror_s(error,256,errno);
        printf("Failed to read file data: %s",error);
        return 1;
    }
    // Write to G`file`
    file.entries[i]->data=data;
    file.entries[i]->size=fsiz;
    return 0;
}

int  __stdcall DLLAPI wav2binka(wchar_t* _wav,wchar_t* _binka){
    char wav[wcslen(_wav)*2];
    char binka[wcslen(_binka)*2];
    int converted;
    if(wcstombs_s(&converted,wav,wcslen(_wav)*2,_wav,wcslen(_wav))!=0){
        char error[256];
        strerror_s(error,256,errno);
        printf("Failed to convert WSTR2STR wav  : %s\n",error);
        return 1;
    }
    if(wcstombs_s(&converted,binka,wcslen(_binka)*2,_binka,wcslen(_binka))!=0){
        char error[256];
        strerror_s(error,256,errno);
        printf("Failed to convert WSTR2STR binka: %s\n",error);
        return 1;
    }

    int max=strlen(wav)+strlen(binka)+2+11+4+6;
    char command[max];
    memset(command,0,max-1);
    
    sprintf_s(command,max,"binkaencode \"%s\" \"%s\" >nul\0",wav,binka);
    system(command);
}
int  __stdcall DLLAPI binka2wav(wchar_t* binka,wchar_t* wav){

}