#!/bin/sh
cd `dirname $0`


echo "copying binary files"  
i686-w64-mingw32-objcopy -I binary -Bi386 -Ope-i386 ../res/binkaencode.exe obj/binkaencode.o
i686-w64-mingw32-objcopy -I binary -Bi386 -Ope-i386 ../res/binwin.asi obj/binwin.o
i686-w64-mingw32-objcopy -I binary -Bi386 -Ope-i386 ../res/mss32.dll obj/mss32.o

i686-w64-mingw32-g++ \
  -static \
  -static-libgcc -static-libstdc++ \
  -I/usr/i686-w64-mingw32/include/ \
  -o msscmp.exe \
  -std=c++17 \
  ../*.cpp ../test.c \
  obj/*.o

echo "running"
wine ./msscmp.exe