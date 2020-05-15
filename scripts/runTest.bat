@echo off
setlocal enabledelayedexpansion
cd %~dp0/../work

rem parsing aruments
set fastmode=0
set buildeddll=0
set direct=0
shift
:parsearg
    set arg=%0
    if "%arg%"=="" (
        goto parsearg_end
    ) else if "%arg%"=="--fast" (
        set fastmode=1
    ) else if "%arg%"=="--buildeddll" (
        set buildeddll=1
    ) else if "%arg%"=="--direct" (
        set direct=1
    ) else (
        echo unknown option "%arg%"
        exit /b 1
    )
    shift
    goto parsearg
:parsearg_end


rem delete datas
for %%f in (do.wav do.binka
            enclog.txt msscmp.txt out.txt
            encode.exe msscmp.dll
            out.msscmp
            ) do rem if exist %%~ff del %%~ff

rem direct option
set compileOpt=-lkernel32 -Wall
if "%direct%"=="1" (
    set compileOpt=%compileOpt% -DDIRECT
)



rem fast test option
if "%fastmode%"=="1" (
    echo [-]run test in fast mode
    tcc %compileOpt% ../res/resource.c ../res/res.c ../res/log.c ../res/msscmp.c -run ../res/test.c %msscmpPath%
) else (
    rem Builded dll option
    if "%buildeddll%"=="0" (
        call ../scripts/make_msscmp.bat %compileOpt%
        move ../res/msscmp.dll ./
    )
    echo [-]compile test
    tcc %compileOpt% msscmp.dll ../res/test.c -o test.exe
    echo [-]run test
    test.exe %msscmpPath%
)

endlocal

@rem debug code 
@rem tcc -Wall -o test.exe ../res/msscmp/msscmp.c ../res/msscmp/test.c && gdb --args test.exe C:\Users\syoch\Downloads\project\WIIU\MCSEEditor\data\Minecraft.msscmp