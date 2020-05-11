@echo off
setlocal
cd %~dp0

call make_msscmp.bat

echo Run test
tcc -Wall msscmp.dll -run ../res/test.c %msscmpPath%
echo Done with code:%errorlevel%

echo del dll
del msscmp.dll msscmp.def
echo done


endlocal

rem debug code 
rem tcc -Wall -o test.exe ../res/msscmp/msscmp.c ../res/msscmp/test.c && gdb --args test.exe C:\Users\syoch\Downloads\project\WIIU\MCSEEditor\data\Minecraft.msscmp