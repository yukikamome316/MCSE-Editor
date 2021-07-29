#pragma once

#include <string>

enum libId {
  RES_binkaWin_asi,
  RES_binkaEncode_exe,
  RES_mss32_dll,
};

#ifdef __cplusplus
extern "C" {
#endif

void extractRes(libId id, std::string path);

#ifdef __cplusplus
}
#endif