@echo off
setlocal
cd %~dp0/../work

call ../scripts/make_msscmp.bat
move ../res/msscmp.dll ./

echo Compile test
tcc -Wall msscmp.dll ../res/test.c -o test.exe
move test.exe ../work

echo run test
test.exe %msscmpPath%

echo del dll
del msscmp.dll


endlocal

rem debug code 
rem tcc -Wall -o test.exe ../res/msscmp/msscmp.c ../res/msscmp/test.c && gdb --args test.exe C:\Users\syoch\Downloads\project\WIIU\MCSEEditor\data\Minecraft.msscmp