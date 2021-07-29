#include "res.hpp"

#include <fstream>
#include <iostream>

#include "log.hpp"
#ifdef __cplusplus
extern "C" {
#endif

extern char binkaencode[] asm("_binary____res_binkaencode_exe_start");
extern char binwin_asi[] asm("_binary____res_binwin_asi_start");
extern char mss32_dll[] asm("_binary____res_mss32_dll_start");

extern char binkaencode_end[] asm("_binary____res_binkaencode_exe_end");
extern char binwin_asi_end[] asm("_binary____res_binwin_asi_end");
extern char mss32_dll_end[] asm("_binary____res_mss32_dll_end");

#ifdef __cplusplus
}
#endif

// out is file path
void extractRes(libId id, std::string path) {
  char *start, *end;
  switch (id) {
    case libId::RES_binkaEncode_exe:
      start = binkaencode;
      end = binkaencode_end;
      break;
    case libId::RES_binkaWin_asi:
      start = binwin_asi;
      end = binwin_asi_end;
      break;
    case libId::RES_mss32_dll:
      start = mss32_dll;
      end = mss32_dll_end;
      break;
    default:
      log_print("loadres : Invalid library id (ignore)\n");
      return;
      break;
  }

  std::ofstream out(path, std::ios::binary);
  if (!out.is_open()) {
    log_print("loadres : Failed to open file %s\n", path.c_str());
    return;
  }

  for (auto ptr = start; ptr < end; ++ptr) {
    out.write(reinterpret_cast<char *>(ptr), 1);
  }
  out.close();
}