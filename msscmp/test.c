#include <stdio.h>
#include <wchar.h>

#include "msscmp.hpp"

int main(int argc, char const *argv[]) {
  loadMsscmp(L"vita.msscmp");
  showMsscmp();

  return 0;
}
