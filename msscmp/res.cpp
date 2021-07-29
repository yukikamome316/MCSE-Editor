#include "res.hpp"

#include <fstream>

#include "log.hpp"
#ifdef __cplusplus
extern "C" {
#endif

char* _binary_binka_encode_start;
char* _binary_binka_encode_size;

char* _binary_binwin_asi_start;
char* _binary_binwin_asi_size;

char* _binary_mss32_dll_start;
char* _binary_mss32_dll_size;

#ifdef __cplusplus
}
#endif

// out is file path
void extractRes(libId id, std::string path) {
  char* start;
  int size;

  switch (id) {
    case libId::RES_binkaEncode_exe:
      size = (int)(uint64_t)&_binary_binka_encode_size[0];
      break;
    case libId::RES_binkaWin_asi:
      start = _binary_binwin_asi_start;
      size = (int)(uint64_t)&_binary_binwin_asi_size[0];
      break;
    case libId::RES_mss32_dll:
      start = _binary_mss32_dll_start;
      size = (int)(uint64_t)&_binary_mss32_dll_size[0];
      break;
    default:
      log_print("warn:Invalid library id (ignore)\n");
      return;
      break;
  }
  std::ofstream out(path, std::ios::binary);
  out.write(start, size);
  out.close();
}