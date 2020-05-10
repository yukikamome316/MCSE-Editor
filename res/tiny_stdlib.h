#ifndef _TINY_STDLIB_
#define _TINY_STDLIB_

//********************
//System header block
//********************
#define _STDDEF_H



//********************
// Argument
//********************
#ifdef __x86_64__
    #ifndef _WIN64
        typedef struct {
            unsigned int gp_offset;
            unsigned int fp_offset;
            union {
                unsigned int overflow_offset;
                char *overflow_arg_area;
            };
            char *reg_save_area;
        } __va_list_struct;

        typedef __va_list_struct va_list[1];
        void __va_start(__va_list_struct *ap, void *fp);
        void *__va_arg(__va_list_struct *ap, int arg_type, int size, int align);
        #define va_start(ap, last) __va_start(ap, __builtin_frame_address(0))
        #define va_arg(ap, type) (*(type *)(__va_arg(ap, __builtin_va_arg_types(type), sizeof(type), __alignof__(type))))
        #define va_copy(dest, src) (*(dest) = *(src))
        #define va_end(ap)
    #else
        typedef char *va_list;
        #define va_start(ap,last) __builtin_va_start(ap,last)
        #define va_arg(ap, t) ((sizeof(t) > 8 || (sizeof(t) & (sizeof(t) - 1))) \? **(t **)((ap += 8) - 8) : *(t  *)((ap += 8) - 8))
        #define va_copy(dest, src) ((dest) = (src))
        #define va_end(ap)
    #endif
#elif __arm__
    typedef char *va_list;
    #define _tcc_alignof(type) ((int)&((struct {char c;type x;} *)0)->x)
    #define _tcc_align(addr,type) (((unsigned)addr + _tcc_alignof(type) - 1) \
                                   & ~(_tcc_alignof(type) - 1))
    #define va_start(ap,last) ap = ((char *)&(last)) + ((sizeof(last)+3)&~3)
    #define va_arg(ap,type) (ap = (void *) ((_tcc_align(ap,type)+sizeof(type)+3) \
                            &~3), *(type *)(ap - ((sizeof(type)+3)&~3)))
    #define va_copy(dest, src) (dest) = (src)
    #define va_end(ap)

#elif defined(__aarch64__)
    typedef struct {
        void *__stack;
        void *__gr_top;
        void *__vr_top;
        int   __gr_offs;
        int   __vr_offs;
    } va_list;
    #define va_start(ap, last) __va_start(ap, last)
    #define va_arg(ap, type) __va_arg(ap, type)
    #define va_end(ap)
    #define va_copy(dest, src) ((dest) = (src))
#else /* __i386__ */
    typedef char *va_list;
    #define va_start(ap,last) ap = ((char *)&(last)) + ((sizeof(last)+3)&~3)
    #define va_arg(ap,type) (ap += (sizeof(type)+3)&~3, *(type *)(ap - ((sizeof(type)+3)&~3)))
    #define va_copy(dest, src) (dest) = (src)
    #define va_end(ap)
#endif
typedef va_list __gnuc_va_list;



//********************
//Call Type
//********************
#define __cdecl   __attribute__((__cdecl__))
#define __stdcall __attribute__((__stdcall__))


//********************
//Constants
//********************
#define NULL ((void *)0)
#define EOF (-1)
#define _O_TEXT 0x4000
#define _IOFBF 0x0000
#define _IOLBF 0x0040
#define _IONBF 0x0004
#define SEEK_CUR 1
#define SEEK_END 2
#define SEEK_SET 0
#define EPERM 1
#define ENOENT 2
#define ESRCH 3
#define EINTR 4
#define EIO 5
#define ENXIO 6
#define E2BIG 7
#define ENOEXEC 8
#define EBADF 9
#define ECHILD 10
#define EAGAIN 11
#define ENOMEM 12
#define EACCES 13
#define EFAULT 14
#define EBUSY 16
#define EEXIST 17
#define EXDEV 18
#define ENODEV 19
#define ENOTDIR 20
#define EISDIR 21
#define ENFILE 23
#define EMFILE 24
#define ENOTTY 25
#define EFBIG 27
#define ENOSPC 28
#define ESPIPE 29
#define EROFS 30
#define EMLINK 31
#define EPIPE 32
#define EDOM 33
#define EDEADLK 36
#define ENAMETOOLONG 38
#define ENOLCK 39
#define ENOSYS 40
#define ENOTEMPTY 41
#define EINVAL 22
#define ERANGE 34
#define EILSEQ 42
#define STRUNCATE 80
#define EDEADLOCK EDEADLK



//********************
//Stream
//********************
#define stdin (&__iob_func()[0])
#define stdout (&__iob_func()[1])
#define stderr (&__iob_func()[2])

//********************
//Struct
//********************
typedef struct _iobuf
{
    char *_ptr;
    int _cnt;
    char *_base;
    int _flag;
    int _file;
    int _charbuf;
    int _bufsiz;
    char *_tmpfname;
} FILE;

//********************
//Type Define
//********************
typedef unsigned          char uint8_t ,_uint8;
typedef   signed          char  int8_t ,__int8;
typedef unsigned short     int uint16_t,_uint16;
typedef   signed short     int  int16_t,__int16;
typedef unsigned long      int uint32_t,_uint32;
typedef   signed long      int  int32_t,__int32;
typedef unsigned long long int uint64_t,_uint64;
typedef   signed long long int  int64_t,__int64;

typedef  int32_t wchar_t;
typedef  int64_t fpos_t;
typedef  int64_t int64_t;
typedef  int64_t off64_t;
typedef uint32_t errno_t;
typedef uint32_t size_t;
typedef uint64_t intptr_t;

#define bool _Bool
#define true 1
#define false 0

//********************
//Macros
//********************
#if defined(_WIN64)
    FILE* __cdecl __iob_func();
#elif defined(_MSVCRT_)
    extern FILE _iob[];
    #define __iob_func()    (_iob)
#else
    extern FILE* (*_imp___iob)[];
    #define __iob_func()    (*_imp___iob)
    #define _iob __iob_func()
#endif



//********************
//Errorno
//********************
extern int *__cdecl _errno(void);
#define errno (*_errno())
errno_t __cdecl _set_errno(int _Value);
errno_t __cdecl _get_errno(int *_Value);



//********************
//Function
//********************

//System
int      __cdecl system(const char *_Command);

//Console
int      __cdecl printf(const char *fmt, ...);
int      __cdecl puts(const char *Str);
int      __cdecl putchar(int Char);
int      __cdecl putch(int Char);
int      __cdecl getchar(void);
int      __cdecl getch(void);
char*    __cdecl gets(char *Buffer);

//File
FILE*    __cdecl fopen(const char *FileName, const char *Mode);
int      __cdecl fclose(FILE *File);
int      __cdecl fprintf(FILE *File, const char *fmt, ...);
int      __cdecl fscanf(FILE *File, const char *fmt, ...);
int      __cdecl fputs(const char *Str, FILE *File);
char*    __cdecl fgets(char *Buf,int Max,FILE *From);
int      __cdecl fputc(int Char, FILE *File);
int      __cdecl fgetc(FILE *From);
size_t   __cdecl fwrite(const void *Src, size_t Size, size_t Count, FILE *File);
size_t   __cdecl fread(void *Dest, size_t ElemSize, size_t Count, FILE *File);
int      __cdecl  fseek   (FILE *File,long    Offset,int Origin);
int               fseeko64(FILE *File,off64_t Offset,int Origin);
int      __cdecl _fseeki64(FILE *File,int64_t Offset,int Origin);
int      __cdecl  fsetpos (FILE *File,fpos_t *Pos);
long     __cdecl  ftell   (FILE *File);
off64_t           ftello64(FILE *File);
int64_t  __cdecl _ftelli64(FILE *File);
int      __cdecl  fgetpos (FILE *File,fpos_t *Pos);
int      __cdecl feof(FILE *File);
int      __cdecl ferror(FILE *_File);
int      __cdecl fflush(FILE *File);
intptr_t __cdecl _get_osfhandle(int FileHandle);
int      __cdecl _open_osfhandle(intptr_t FileHandle,int Flags);
int      __cdecl setvbuf(FILE *File,char *Buffer,int Mode,size_t Size);
FILE*    __cdecl fdopen(int FileHandle,const char *fmt);
FILE*    __cdecl _fdopen(int FileHandle,const char *Mode);
FILE*    __cdecl freopen(const char *Filename, const char *Mode, FILE *File);
int      __cdecl remove(const char *Filename);

//Directry
int      __cdecl _mkdir(const char *_Path);

//String
int      __cdecl sprintf(char* Target,const char* fmt,...);
size_t   __cdecl strlen(const char *Str);
char*    __cdecl strcpy(char *Target,const char *Sourece);
int      __cdecl strcmp(const char *a,const char *b);
char*    __cdecl strerror(int);
char*    __cdecl strcat(char *Dest,const char *Source);
//Utils
int      __cdecl isprint(int Ch);
void     __cdecl  exit(int Code);

//Memory
void     __cdecl free(void *Memory);
void*    __cdecl calloc(size_t NumOfElements,size_t SizeOfElements);
void*    __cdecl malloc(size_t Size);
void*    __cdecl realloc(void *Memory,size_t NewSize);
int      __cdecl memcmp(const void *a,const void *b,size_t max);
void*    __cdecl memcpy(void *target,const void *source,size_t Size);
void*    __cdecl memset(void *target,int Value,size_t Size);

#endif
