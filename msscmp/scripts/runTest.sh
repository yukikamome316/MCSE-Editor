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
  obj/*.o ../*.cpp ../test.c

echo "linking test binary"
i686-w64-mingw32-g++ -static -std=c++17 -o msscmp.exe msscmp.dll ../test.c

echo "running"
wine ./msscmp.exe