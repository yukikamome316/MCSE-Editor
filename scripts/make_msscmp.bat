@echo off
setlocal enabledelayedexpansion
cd %~dp0/../res

set objs=
for %%a in (resources\*) do if "%%~xa"==".o" (
    set objs=!objs! %%a
)

echo [-]Building msscmp.dll to res/msscmp.dll
tcc -m32 -Wall -shared %objs% res.c log.c msscmp.c -o msscmp.dll
del msscmp.def
copy msscmp.dll "%~dp0/../bin/Debug/msscmp.dll"

endlocal