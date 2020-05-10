#ifndef L_TINYWIN_H
#define L_TINYWIN_H

#include <tiny_stdlib.h>

//********************
//Constants
//********************
#define FALSE 0
#define TRUE 1
#define MAX_PATH 260
#define WINAPI __stdcall
#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)
#define CONSOLE_TEXTMODE_BUFFER 1

#define CS_VREDRAW 0x0001
#define CS_HREDRAW 0x0002

#define WHITE_BRUSH 0
#define LTGRAY_BRUSH 1
#define GRAY_BRUSH 2
#define DKGRAY_BRUSH 3
#define BLACK_BRUSH 4
#define NULL_BRUSH 5
#define HOLLOW_BRUSH NULL_BRUSH
#define WHITE_PEN 6
#define BLACK_PEN 7
#define NULL_PEN 8

#define IDI_APPLICATION 32512
#define IDI_HAND 32513
#define IDI_QUESTION 32514
#define IDI_EXCLAMATION 32515
#define IDI_ASTERISK 32516
#define IDI_WINLOGO 32517
#define IDC_ARROW 32512
#define IDC_IBEAM 32513
#define IDC_WAIT 32514
#define IDC_CROSS 32515
#define IDC_UPARROW 32516
#define IDC_SIZE 32640
#define IDC_ICON 32641
#define IDC_SIZENWSE 32642
#define IDC_SIZENESW 32643
#define IDC_SIZEWE 32644
#define IDC_SIZENS 32645
#define IDC_SIZEALL 32646
#define IDC_NO 32648
#define IDC_HAND 32649
#define IDC_APPSTARTING 32650
#define IDC_HELP 32651

#define COL_BLUE 0x1
#define COL_GREEN 0x2
#define COL_RED 0x4
#define COL_INTENSITY 0x8

#define FILE_ATTRIBUTE_READONLY 0x00000001
#define FILE_ATTRIBUTE_HIDDEN 0x00000002
#define FILE_ATTRIBUTE_SYSTEM 0x00000004
#define FILE_ATTRIBUTE_DIRECTORY 0x00000010
#define FILE_ATTRIBUTE_ARCHIVE 0x00000020
#define FILE_ATTRIBUTE_NORMAL 0x00000080
#define FILE_ATTRIBUTE_ENCRYPTED 0x00004000

#define STD_INPUT_HANDLE ((DWORD)(-10))
#define STD_OUTPUT_HANDLE ((DWORD)(-11))
#define STD_ERROR_HANDLE ((DWORD)(-12))

#define GENERIC_READ (0x80000000L)
#define GENERIC_WRITE (0x40000000L)
#define GENERIC_EXECUTE (0x20000000L)
#define GENERIC_ALL (0x10000000L)

#define CREATE_NEW 1
#define CREATE_ALWAYS 2
#define OPEN_EXISTING 3
#define OPEN_ALWAYS 4
#define TRUNCATE_EXISTING 5

#define VK_LBUTTON 0x01
#define VK_RBUTTON 0x02
#define VK_MBUTTON 0x04
#define VK_BACK 0x08
#define VK_TAB 0x09
#define VK_CLEAR 0x0C
#define VK_RETURN 0x0D
#define VK_SHIFT 0x10
#define VK_CONTROL 0x11
#define VK_MENU 0x12
#define VK_PAUSE 0x13
#define VK_CAPITAL 0x14
#define VK_SPACE 0x20
#define VK_NEXT 0x22
#define VK_END 0x23
#define VK_HOME 0x24
#define VK_LEFT 0x25
#define VK_UP 0x26
#define VK_RIGHT 0x27
#define VK_DOWN 0x28
#define VK_INSERT 0x2D
#define VK_DELETE 0x2E
#define VK_F1 0x70
#define VK_F2 0x71
#define VK_F3 0x72
#define VK_F4 0x73
#define VK_F5 0x74
#define VK_F6 0x75
#define VK_F7 0x76
#define VK_F8 0x77
#define VK_F9 0x78
#define VK_F10 0x79
#define VK_F11 0x7A
#define VK_F12 0x7B
#define VK_NUMLOCK 0x90
#define VK_SCROLL 0x91
#define VK_LSHIFT 0xA0
#define VK_RSHIFT 0xA1
#define VK_LCONTROL 0xA2
#define VK_RCONTROL 0xA3
#define VK_LMENU 0xA4
#define VK_RMENU 0xA5
#define VK_LWIN 0x5B
#define VK_RWIN 0x5C
#define VK_PROCESSKEY 0xE5
#define VK_PACKET 0xE7
#define VK_ATTN 0xF6
#define VK_CRSEL 0xF7
#define VK_EREOF 0xF9
#define VK_PLAY 0xFA
#define VK_ZOOM 0xFB

//********************
//Windows Type
//********************
//Base(0)
typedef struct { int unused; }  *MODULE;
typedef     int WINBOOL;
typedef   void* HANDLE;
typedef wchar_t wchar;
typedef    char achar;

//Base(1)
typedef  MODULE HINSTANCE;
typedef  MODULE HWND;
typedef  MODULE HICON;
typedef  MODULE HCURSOR;
typedef  MODULE HBRUSH;

//Univarsal char(2)
#ifdef UNICODE
typedef wchar ochar;
#else
typedef achar ochar;
#endif

//Advance Int(3)
#ifdef _WIN64 //win64
typedef     __int64   INT_PTR, * PINT_PTR;
typedef     _uint64  UINT_PTR, * PUINT_PTR;
typedef     __int64  LONG_PTR, * PLONG_PTR;
typedef     _uint64 ULONG_PTR, * PULONG_PTR;
#else //win32
typedef           int   INT_PTR, * PINT_PTR;
typedef unsigned  int  UINT_PTR, * PUINT_PTR;
typedef          long  LONG_PTR, * PLONG_PTR;
typedef unsigned long ULONG_PTR, * PULONG_PTR;
#endif

//WIN type(4)
typedef                void   VOID;
typedef       unsigned char   BYTE;
typedef       unsigned short  WORD;
typedef       unsigned int    UINT;
typedef                int    INT;
typedef                long   LONG;
typedef       unsigned long   DWORD;
typedef                ochar   TCHAR, * TSTR;
typedef                wchar   WCHAR, * WSTR;
typedef                achar   ACHAR, * ASTR;
typedef const          ochar  CTCHAR, * CTSTR;
typedef const          wchar  CWCHAR, * CWSTR;
typedef const          achar  CACHAR, * CASTR;


//advanced win type(5)
typedef __int64 SOCKET;
typedef UINT_PTR WPARAM;
typedef LONG_PTR LPARAM;
typedef LONG_PTR LRESULT;

//functions(6)
typedef LRESULT(WINAPI* WNDPROC) (HWND, UINT, WPARAM, LPARAM);

//********************
//Basic Struct type
//********************

typedef struct
{
    short X;
    short Y;
} COORD;

typedef struct
{
    DWORD LowDateTime;
    DWORD HighDateTime;
} FILETIME;

typedef struct
{
    short Left;
    short Top;
    short Right;
    short Bottom;
} SMALL_RECT;

typedef struct
{
    COORD Size;
    COORD CursorPosition;
    WORD Attributes;
    SMALL_RECT WindowSize;
    COORD MaximumWindowSize;
} CONSOLE_SCREEN_BUFFER_INFO;

typedef struct
{
    DWORD Length;
    VOID* SecurityDescriptor;
    WINBOOL InheritHandle;
} SECURITY_ATTRIBUTES;

typedef struct
{
    DWORD FileAttributes;
    FILETIME CreationTime;
    FILETIME LastAccessTime;
    FILETIME LastWriteTime;
    DWORD FileSizeHigh;
    DWORD FileSizeLow;
    DWORD Reserved0;
    DWORD Reserved1;
    TCHAR FileName[MAX_PATH];
    TCHAR AlternateFileName[14];
} WIN32_FIND_DATA;

typedef struct {
    LONG x;
    LONG y;
} POINT;

typedef struct {
    HWND hwnd;
    UINT message;
    WPARAM wParam;
    LPARAM lParam;
    DWORD time;
    POINT pt;
} MSG;

typedef struct tagWNDCLASSW {
    UINT style;
    WNDPROC lpfnWndProc;
    int cbClsExtra;
    int cbWndExtra;
    HINSTANCE hInstance;
    HICON hIcon;
    HCURSOR hCursor;
    HBRUSH hbrBackground;
    CASTR lpszMenuName;
    CASTR lpszClassName;
} WNDCLASS;

//********************
//Functions Prototype Definetion
//********************

//Win API
WINBOOL WINAPI CloseHandle(HANDLE handle);

//Console API
WINBOOL WINAPI SetConsoleCursorPosition(HANDLE cons, COORD cursorpos);
WINBOOL WINAPI SetConsoleTextAttribute(HANDLE cons, WORD addr);
WINBOOL WINAPI GetConsoleScreenBufferInfo(HANDLE cons, CONSOLE_SCREEN_BUFFER_INFO* dest);
HANDLE  WINAPI CreateConsoleScreenBuffer(DWORD AccessFlag, DWORD ShareMode, const SECURITY_ATTRIBUTES* securityattr, DWORD flag, VOID* ScreenBufferData);
WINBOOL WINAPI SetConsoleActiveScreenBuffer(HANDLE cons);
WINBOOL WINAPI WriteConsoleA(HANDLE cons, const VOID* writebuf, DWORD write, DWORD* writed, VOID* Zero);
WINBOOL WINAPI WriteConsoleW(HANDLE cons, const VOID* writebuf, DWORD write, DWORD* writed, VOID* Zero);

//User API
short   WINAPI GetAsyncKeyState(int keynum);
HANDLE  WINAPI GetStdHandle(DWORD handle);
VOID    WINAPI Sleep(DWORD MilliSec);

//FileAPI
HANDLE  WINAPI FindFirstFileW(WSTR* name, WIN32_FIND_DATA* writeto);
HANDLE  WINAPI FindFirstFileA(ASTR* name, WIN32_FIND_DATA* writeto);
WINBOOL WINAPI FindNextFileW(HANDLE handle, WIN32_FIND_DATA* writeto);
WINBOOL WINAPI FindNextFileA(HANDLE handle, WIN32_FIND_DATA* writeto);
WINBOOL WINAPI FindClose(HANDLE handle);
HANDLE WINAPI CreateFileA(CASTR FileName, DWORD DesiredAccess, DWORD ShareMode, SECURITY_ATTRIBUTES* SecurityAttributes, DWORD CreationDisposition, DWORD FlagsAndAttributes, HANDLE TemplateFile);
HANDLE WINAPI CreateFileW(CWSTR FileName, DWORD DesiredAccess, DWORD ShareMode, SECURITY_ATTRIBUTES* SecurityAttributes, DWORD CreationDisposition, DWORD FlagsAndAttributes, HANDLE TemplateFile);
DWORD WINAPI GetModuleFileNameA(MODULE Module, achar* Filename, DWORD Size);
DWORD WINAPI GetModuleFileNameW(MODULE Module, wchar* Filename, DWORD Size);
VOID _splitpath(const char* path, char* drive, char* dir, char* fname, char* ext);

//Switching
#ifdef UNICODE
#define FindFirstFile FindFirstFileW
#define FindNextFile  FindNextFileW
#define WriteConsole  WriteConsoleW
#define CreateFile    CreateFileW
#else
#define FindFirstFile FindFirstFileA
#define FindNextFile  FindNextFileA
#define WriteConsole  WriteConsoleA
#define CreateFile    CreateFileA
#endif


#endif