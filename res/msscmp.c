#define MSSCMP_EXPORT
#include "msscmp.h"

File file;
int error;
char *backup;
int no=0;

//Code by https://qiita.com/fireflower0/items/dc54f3ec1b3698a98b14
//Thanks for qiita user '@fireflower0'
int split(char *dst[], char *src, char delim)
{
    int count = 0;

    for (;;)
    {
        while (*src == delim)
        {
            src++;
        }

        if (*src == '\0')
            break;

        dst[count++] = src;

        while (*src && *src != delim)
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
void writeFile32bitBE(FILE *fp, uint32_t val)
{
    fputc(val >> 0x18, fp);
    fputc(val >> 0x10, fp);
    fputc(val >> 0x08, fp);
    fputc(val >> 0x00, fp);
}

//Write 32bit integer by file pointer (little endian)
void writeFile32bitLE(FILE *fp, uint32_t val)
{
    fputc(val >> 0x00, fp);
    fputc(val >> 0x08, fp);
    fputc(val >> 0x10, fp);
    fputc(val >> 0x18, fp);
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

//EXTERNeD
// Dll Entry Point (a.k.a. Dllmain)
bool __stdcall DllMain(void* hinstDLL, uint32_t fdwReason, void* lpvReserved)
{
    switch (fdwReason)
    {
    case 1:
        setlocale(LC_ALL, "JPN");
        break;
    case 0:
        debugfile(DF_SET, DF_CLOSE);
        break;
    }
    return true;
}


//EXTERNED
//extract msscmp (Minecraft Sound Source CoMPressed ?)
int __stdcall DLLAPI extractMsscmp(const wchar_t *path)
{
    printf("extract : Extracting %ls\n", path);
    char tmppath[600], *pathParts[30];
    FILE *destfp;
    loadMsscmp(path);
    int j, pathPartsLen;
    for (int i = 0; i < file.entryCount; i++)
    {
        pathPartsLen = split(pathParts, file.entries[i]->paths.path, '/');
        memset(tmppath, 0, 600);
        strcpy_s(tmppath, 600, "tmp/");
        for (j = 0; j < pathPartsLen; j++)
        {
            strcat_s(tmppath, 600, pathParts[j]);
            strcat_s(tmppath, 600, "/");
            _mkdir(tmppath);
        }

        fopen_s(&destfp, file.entries[i]->paths.full, "wb");
        if (destfp == NULL)
        {
            error = 1;
            char errorbuffer[256];
            strerror_s(errorbuffer, 256, errno);
            printf("Failed to open dest fp:%s\n", errorbuffer);
            printf("%s\n", file.entries[i]->paths.full);
            return 1;
        }
        fwrite(file.entries[i]->data, 1, file.entries[i]->size, destfp);
        fclose(destfp);
    }
    return 0;
}

//EXTERNED (β)
//load msscmp to internal
int __stdcall DLLAPI loadMsscmp(const wchar_t *path)
{
    printf("load    : load %ls\n", path);
    Entry *entry;
    Offsets *offsets;
    Paths *paths;
    int i, j;
    char  *cw, *buf;

    _wfopen_s(&file.fp, path, L"rb");
    if (file.fp == NULL)
    {
        error = 1;
        char error[256];
        strerror_s(error,256,errno);
        printf("load    : Failed to open target file: %ls %s\n", path,error);
        return 1;
    }

    //backup
    backup = malloc(msscmpDataStart);
    if (backup == NULL)
    {
        error = 1;
        printf("load    : Failed to Malloc msscmp backup\n");
        return 1;
    }
    i = 0;
    while (i != msscmpDataStart)
        backup[i++] = fgetc(file.fp);

    fseek(file.fp, 0x00000000, SEEK_SET);
    if (readFile32bitBE(file.fp) != 0x42414e4b)
    {
        error = 1;
        printf("load    : Failed to Check msscmp Signeture\n");
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
        printf("load    : Not found files\n");
        return 1;
    }

    for (i = 0; i < file.entryCount; i++)
    {
        entry = malloc(sizeof(Entry));
        if (entry == NULL)
        {
            printf("load    : Failed to malloc entry\n");
            error = 1;
            return 1;
        }
        offsets = &entry->offsets;
        paths = &entry->paths;

        fseek(file.fp, file.filetableOffset + 8 * i, SEEK_SET);
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

        if(i==0)printf("load    : |   %d = [0x%08x]%s\n",no, entry->size,paths->full);

        fseek(file.fp, offsets->data, SEEK_SET);
        buf = malloc(entry->size);
        fread(buf, 1, entry->size, file.fp);
        entry->data = buf;
        file.entries[i] = entry;
    }
    printf("load    : + \n");
    return 0;
}

//EXTERNED
//save msscmp
int __stdcall DLLAPI saveMsscmp(const wchar_t *path)
{
    printf("save    : saving %ls\n", path);
    FILE *fp = 0;
    int ret;
    _wremove(path);
    _wfopen_s(&fp, path, L"wb+");
    if (fp == NULL)
    {
        char errorbuffer[256];
        strerror_s(errorbuffer, 256, errno);
        printf("save    : Failed to open target file: %s\n", errorbuffer);
        return 1;
    }
    
    //--------------------------------------------------------------
    //               Write Msscmp
    //--------------------------------------------------------------
    //write header(raw)
    if (fseek(fp, 0, SEEK_SET) != 0)
    {
        char errorbuffer[256];
        strerror_s(errorbuffer, 256, errno);
        printf("save    : Failed to seek target file header: %s\n", errorbuffer);
        return 1;
    }
    if (fwrite(backup, 0x0001B000, 1, fp) < 1)
    {
        char errorbuffer[256];
        strerror_s(errorbuffer, 256, errno);
        printf("save    : Failed to write target file header: %s\n", errorbuffer, errno);
        return 1;
    }

    //write Datas
    uint32_t currentPos = 0x0001B000;
    for (int i = 0; i < file.entryCount; i++)
    {
        file.entries[i]->offsets.data = currentPos;
        ret = fseek(fp, currentPos, SEEK_SET);
        if (ret != 0 || ferror(fp) != 0)
        {
            char errorbuffer[256];
            strerror_s(errorbuffer, 256, errno);
            printf("save    : \nFailed to seek target file: %s\n", errorbuffer);
            return 1;
        }
        ret = fwrite(file.entries[i]->data, file.entries[i]->size, 1, fp);
        if (ret < 1 || ferror(fp) != 0)
        {
            char errorbuffer[256];
            strerror_s(errorbuffer, 256, errno);
            printf("save    : \nFailed to write target file entry: %s\n", errorbuffer, errno);
            return 1;
        }
        currentPos += ((int)ceil((float)file.entries[i]->size / msscmpDataAlign)) * msscmpDataAlign;
    }

    //Edit datas entry and size
    uint32_t fileinfo;
    for (int i = 0; i < file.entryCount; i++)
    {
        // gotofile info
        if (fseek(fp, file.entries[i]->offsets.info+8, SEEK_SET) != 0)
        {
            char errorbuffer[256];
            strerror_s(errorbuffer, 256, errno);
            printf("save    : Failed to seek target file info: %s\n", errorbuffer);
            return 1;
        }
        //if(i==0)fprintf(stdout,"%08x\n",ftell(fp));
        writeFile32bitLE(fp, file.entries[i]->offsets.data);
        skipRead(fp, 8); //Skip ????
        writeFile32bitBE(fp, file.entries[i]->sampleRate);
        writeFile32bitBE(fp, file.entries[i]->size);
    }
    //end
    fclose(fp);
    return 0;
}

//EXTERNED
//replace msscmp entry data
int __stdcall DLLAPI replaceEntryMsscmp(wchar_t *_path, wchar_t *replacePath)
{
    printf("replace : Replacing %ls to %ls\n", _path, replacePath);
    // Get Entry Number
    int i;
    size_t converted;
    char path[wcslen(_path) * 2];
    wcstombs_s(&converted, path, wcslen(_path) * 2, _path, wcslen(_path) * 2);

    for (i = 0; i < file.entryCount; i++)
    {
        if (!strcmp(path, file.entries[i]->paths.full + 4))
        {
            break;
        }
    }
    printf("replace : |   replace file index = %d\n", i);
    // Open replace Path in `rb`
    FILE *fp = NULL;
    _wfopen_s(&fp, replacePath, L"rb");
    if (fp == NULL)
    {
        char error[256];
        strerror_s(error, 256, errno);
        printf("replace : Failed to open file replace: %s", error);
        return 1;
    }
    // Get file size of `replace Path` to `fsiz`
    struct stat fileInfo;
    if (fstat(fileno(fp), &fileInfo) != 0)
    {
        char error[256];
        strerror_s(error, 256, errno);
        printf("replace : Failed to get file info: %s", error);
        return 1;
    }
    int fsiz = fileInfo.st_size;
    // Read file to `data` by `fp`
    char *data = malloc(fsiz);
    if (data == NULL)
    {
        char error[256];
        strerror_s(error, 256, errno);
        printf("replace : Failed to malloc data: %s", error);
        return 1;
    }
    if (fread(data, fsiz, 1, fp) < 1)
    {
        char error[256];
        strerror_s(error, 256, errno);
        printf("replace : Failed to read file data: %s", error);
        return 1;
    }
    // Write to G`file`
    printf("replace : +   set size %s -> 0x%08x\n",file.entries[i]->paths.full,fsiz);
    file.entries[i]->data = data;
    file.entries[i]->size = fsiz;
    no=i;
    return 0;
}

//EXTERNED
//convert wav to binka
int __stdcall DLLAPI wav2binka(wchar_t *wav, wchar_t *binka)
{
    printf("wav2bink: Converting %ls to %ls\n", wav, binka);
    int max = wcslen(wav) + wcslen(binka) + 2 + 11 + 4 + 6 + 24;
    char command[max];
    memset(command, 0, sizeof(command));

    extractRes(RES_binkaEncode_exe, "encode.exe");
    sprintf(command, "encode \"%ls\" \"%ls\" 1> enclog.txt 2>&1", wav, binka);
    printf("wav2bink: |   executing %s\n", command);
    
    STARTUPINFOA si = {sizeof(STARTUPINFOA)};
    PROCESS_INFORMATION pi;
    if(!CreateProcessA(NULL, command, NULL, NULL, false, 0x8000000, NULL, NULL, &si, &pi)){
        printf("Failed to execute\n");
        return 1;
    }
    WaitForSingleObject(pi.hProcess,0xffffffff);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    printf("wav2bink: |   done\n");
    remove("./encode.exe");
    return 0;
}

//EXTERNED
//convert binka2wav
int __stdcall DLLAPI binka2wav(wchar_t *binka, wchar_t *wav)
{
    printf("bink2wav: Converting %ls to %ls\n", binka, wav);

    extractRes(RES_mss32_dll, "./mss32.dll");
    extractRes(RES_binkaWin_asi, "./binkawin.asi");

    char *data;
    struct stat st;
    FILE *fp;
    uint32_t data_size;

    printf("bink2wav: |   Reading binka\n");
    _wfopen_s(&fp, binka, L"rb");
    if (fp == NULL)
    {
        char error[256];
        strerror_s(error, 256, errno);
        printf("Failed to open binka: %s\n", error);
        return 1;
    }
    fstat(fileno(fp), &st);

    data_size = (uint32_t)st.st_size;
    data = malloc(data_size);
    if (data == NULL)
    {
        char error[256];
        strerror_s(error, 256, errno);
        printf("Failed to malloc data: %s\n", error);
        return 1;
    }

    if (fread(data, 1, data_size, fp) < data_size)
    {
        char error[256];
        strerror_s(error, 256, errno);
        printf("Failed to Read binka: %s\n", error);
        return 1;
    }

    fclose(fp);

    printf("bink2wav: |   Convert...\n");
    HANDLE mss32 = LoadLibraryA("mss32.dll");
    if (mss32 == NULL)
    {
        char error[256];
        strerror_s(error, 256, errno);
        printf("Failed to open dll mss32: %s,%d\n", error, GetLastError());
        return 1;
    }
    void *(*AIL_set_redist_directory)(char *)=(void* (*) (char*))
        GetProcAddress(mss32, "_AIL_set_redist_directory@4");
    int (*AIL_startup)() =
        (int (*)())
            GetProcAddress(mss32, "_AIL_startup@0");
    int (*AIL_decompress_ASI)(char*,uint32_t,char*,void**,uint32_t*,uint32_t) =
        (int (*)(char*,uint32_t,char*,void**,uint32_t*,uint32_t))
            GetProcAddress(mss32, "_AIL_decompress_ASI@24");
    void (*AIL_mem_free_lock)() =
        (void (*)(int *))
            GetProcAddress(mss32, "_AIL_mem_free_lock@4");
    int (*AIL_shutdown)() =
        (int (*)())
            GetProcAddress(mss32, "_AIL_shutdown@0");
    AIL_set_redist_directory(".");
    AIL_startup();

    char *converted;
    uint32_t num = 0;
    int ret = AIL_decompress_ASI(data, (uint32_t)data_size, ".binka", &converted, &num, 0U);
    if (ret == 0)
    {
        char error[256];
        strerror_s(error, 256, errno);
        printf("Failed to run Dllapi:\"AIL_decompress_ASI\": %s\n", error);
        return 1;
    }
    char *tmp = malloc(num);
    if (tmp == NULL)
    {
        char error[256];
        strerror_s(error, 256, errno);
        printf("Failed to malloc tmp mem: %s\n", error);
        return 1;
    }
    memcpy(tmp, converted, num);
    AIL_mem_free_lock(converted);
    AIL_shutdown();

    printf("bink2wav: |   writing wav\n");
    _wfopen_s(&fp, wav, L"wb");
    if (fp == NULL)
    {
        char error[256];
        strerror_s(error, 256, errno);
        printf("Failed to open wav: %s\n", error);
        return 1;
    }
    if (fwrite(tmp, 1, num, fp) < num)
    {
        char error[256];
        strerror_s(error, 256, errno);
        printf("Failed to write wav: %s\n", error);
        return 1;
    }
    printf("bink2wav: +   Done\n");
    fclose(fp);
    free(data);
    FreeLibrary(mss32);
    remove("./mss32.dll");
    remove("./binkawin.asi");
    return 0;
}