@rem use gnu objcopy

@echo off
setlocal
cd %~dp0/../res/resources


rem build res/resources/*
for %%a in (*) do (
    echo building %%a
    objcopy -I binary -O pe-i386 -B i386 %%a %%~dpna.o
)



endlocal