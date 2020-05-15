#ifndef MSSCMP_PROTYPE_H
#define MSSCMP_PROTYPE_H
#ifndef _MSC_VER

#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <errno.h>

int fopen_s(FILE **File, const char *filename, const char *mode);
int _wfopen_s(FILE **file, const wchar_t *filename, const wchar_t *mode);
int strcat_s(char *dest, size_t max, const char *src);
int strcpy_s(char *dest, size_t max, const char *src);
int strerror_s(char *dest, size_t max, int err);
int sprintf_s(char *dest, size_t max, const char *fmt, ...);
int mbstowcs_s(size_t *converted, wchar_t *dest, size_t destsize, const char *src, size_t srcsize);
int wcstombs_s(size_t *converted, char *dest, size_t destsize, const wchar_t *src, size_t srcsize);

typedef int *HANDLE;
typedef void (*__stdcall PROC)();
typedef struct _PROCESS_INFORMATION
{
    HANDLE hProcess;
    HANDLE hThread;
    uint32_t dwProcessId;
    uint32_t dwThreadId;
} PROCESS_INFORMATION;
typedef struct _STARTUPINFOA
{
    uint32_t cb;
    char* lpReserved;
    char* lpDesktop;
    char* lpTitle;
    uint32_t dwX;
    uint32_t dwY;
    uint32_t dwXSize;
    uint32_t dwYSize;
    uint32_t dwXCountChars;
    uint32_t dwYCountChars;
    uint32_t dwFillAttribute;
    uint32_t dwFlags;
    uint16_t wShowWindow;
    uint16_t cbReserved2;
    char* lpReserved2;
    HANDLE hStdInput;
    HANDLE hStdOutput;
    HANDLE hStdError;
} STARTUPINFOA;

PROC __stdcall GetProcAddress(HANDLE module, const char *ProcName);
int __stdcall FreeLibrary(HANDLE libModule);
uint32_t __stdcall GetLastError();
HANDLE __stdcall LoadLibraryA(char *libFileName);
HANDLE __stdcall LoadLibraryW(wchar_t *libFileName);
int    __stdcall CloseHandle(HANDLE hObject);
int __stdcall CreateProcessA(
    char *ApplicationName,
    char *CommandLine,
    void **ProcessAttributes,
    void **ThreadAttributes,
    int InheritHandles,
    uint32_t CreationFlags,
    void *Environment,
    char *CurrentDirectory,
    STARTUPINFOA *StartupInfo,
    PROCESS_INFORMATION *ProcessInformation);

#ifdef UNICODE
#define LoadLibrary LoadLibraryW
#else
#define LoadLibrary LoadLibraryA
#endif

#endif /* !defined(_MSC_VER) */
#endif /* !defined(MSSCMP_PROTYPE_H) */