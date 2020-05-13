@rem use `gnu objcopy`, `tcc`

@echo off
setlocal enabledelayedexpansion
cd %~dp0/../res/resources


rem build res/resources/*
set objs=
for %%a in (*) do if not "%%~xa"==".o" (
    echo building %%~dpna.o
    objcopy -I binary -O elf32-i386 -B i386 %%a %%~dpna.o
    set objs=!objs! %%~na.o
)

endlocal