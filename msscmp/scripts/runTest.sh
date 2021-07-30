#!/bin/sh
cd `dirname $0`


echo "copying binary files"  
i686-w64-mingw32-objcopy -I binary -Bi386 -Ope-i386 ../res/binkaencode.exe obj/binkaencode.o
i686-w64-mingw32-objcopy -I binary -Bi386 -Ope-i386 ../res/binwin.asi obj/binwin.o
i686-w64-mingw32-objcopy -I binary -Bi386 -Ope-i386 ../res/mss32.dll obj/mss32.o

echo "compiling msscmp library"
i686-w64-mingw32-g++ \
  -shared \
  -static-libgcc -static-libstdc++ \
  -I/usr/i686-w64-mingw32/include/ \
  -o msscmp.dll \
  -std=c++17 \
  -ffunction-sections \
  -fdata-sections \
  -Wl,--gc-sections \
  obj/*.o ../*.cpp ../test.c

echo "copy msscmp.dll -> msscmpd.dll"
cp msscmp.dll msscmpd.dll

echo "strip msscmp.dll (but remind library functions)"
i686-w64-mingw32-strip \
  -K _wav2binka -K _binka2wav \
  -K _loadMsscmp -K _saveMsscmp -K _closeMsscmp \
  -K _extractMsscmp -K _extractLoadedMsscmp \
  -K _replaceEntryMsscmp -K _showMsscmp \
  msscmp.dll -o msscmp.dll

echo "linking test executable"
i686-w64-mingw32-g++ -static -std=c++17 -o msscmp.exe msscmp.dll ../test.c

echo "clearing"
rm -dRf tmp

echo "running"
wine ./msscmp.exe

ls -R tmp/Minecraft/ambient/cave/cave10