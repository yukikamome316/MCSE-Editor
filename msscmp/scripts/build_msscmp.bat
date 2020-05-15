@echo off
setlocal enabledelayedexpansion
cd %~dp0/..
set opt=
if not "%*"=="" (
    set opt=%*
) else (
    set opt=-Wall
)
echo [-]Building /res/msscmp.dll to /res/msscmp.dll
tcc -m32 %opt% -shared res/resource.c res/res.c log.c msscmp.c -o msscmp.dll
del msscmp.def

endlocal