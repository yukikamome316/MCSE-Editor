#include <stdio.h>
#include <wchar.h>

#include "msscmp.hpp"

int main(int argc, char const *argv[]) {
  loadMsscmp(L"/home/syoch/work/wiiu/msscmp/vita.msscmp");
  showMsscmp();

  return 0;
}
