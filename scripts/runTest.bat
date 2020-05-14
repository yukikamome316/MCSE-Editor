@echo off
setlocal enabledelayedexpansion
cd %~dp0/../work

rem parsing aruments
set fastmode=0
set buildeddll=0
:parsearg
    set arg=%0
    if "%arg%"=="" (
        goto parsearg_end
    ) else if "%arg%"=="--fast" (
        set fastmode=1
    ) else if "%arg%"=="--buildeddll" (
        set buildeddll=1
    )
    shift
    goto parsearg
:parsearg_end

echo [-]Detecting object files
set objs=
for %%a in (..\res\resources\*) do if "%%~xa"==".o" (
    set objs=!objs! %%a
    echo [ ]found %%a
)

rem delete datas
for %%f in (do.wav do.binka
            enclog.txt msscmp.txt out.txt
            encode.exemsscmp.dll
            out.msscmp) do if exist %%~ff del %%~ff

rem Builded dll option
if "%buildeddll%"=="0" (
    call ../scripts/make_msscmp.bat
    move ../res/msscmp.dll ./
)

rem fast test option
if "%fastmode%"=="1" (
    echo [-]run test in fast
    tcc -Wall msscmp.dll -run ../res/test.c %msscmpPath%
) else (
    echo [-]compile test
    tcc -Wall msscmp.dll ../res/test.c -o test.exe
    echo [-]run test
    test.exe %msscmpPath%
)

endlocal

@rem debug code 
@rem tcc -Wall -o test.exe ../res/msscmp/msscmp.c ../res/msscmp/test.c && gdb --args test.exe C:\Users\syoch\Downloads\project\WIIU\MCSEEditor\data\Minecraft.msscmp