#include "binka.hpp"

#include <wchar.h>
#include <windows.h>

#include <filesystem>
#include <fstream>
#include <ios>
#include <string>

#include "log.hpp"
#include "res.hpp"
using namespace std::literals::string_literals;
// EXTERNED
// convert wav to binka
MSSCMP_API int wav2binka(const wchar_t *wav, const wchar_t *binka) {
  extractRes(RES_binkaEncode_exe, "encode.exe");

  log_print("wav2bink: Converting %ls to %ls\n", wav, binka);
  std::wstring command =
      L"encode" + L" "s + wav + L" " + binka + L" 1> enclog.txt 2>&1";
  log_print("wav2bink: |   executing %s\n", command);
  int ret = _wsystem(command.c_str());
  if (ret != 0) {
    log_print("wav2bink: |   failed to convert %ls to %ls\n", wav, binka);
    return -1;
  }
  return 0;
}

// EXTERNED
// convert binka2wav
MSSCMP_API int binka2wav(const wchar_t *binka, const wchar_t *wav) {
  extractRes(RES_mss32_dll, "./mss32.dll");
  extractRes(RES_binkaWin_asi, "./binkawin.asi");

  log_print("bink2wav: Converting %ls to %ls\n", binka, wav);

  char *data;
  uint32_t data_size;

  std::fstream fs(binka, std::ios::in | std::ios::binary);
  if (!fs.is_open()) {
    log_print("bink2wav: |   failed to open %ls\n", binka);
    return -1;
  }

  fs.seekg(0, std::ios::end);
  data_size = fs.tellg();
  fs.seekg(0, std::ios::beg);

  data = new char[data_size];
  fs.read(data, data_size);
  fs.close();
  if (data == nullptr) {
    log_print("bink2wav: |   failed to read %ls\n", binka);
    return -1;
  }
  log_print("bink2wav: |   Convert...\n");

  HMODULE mss32 = LoadLibraryA("mss32.dll");
  if (mss32 == nullptr) {
    log_print("bink2wav: |   failed to load mss32.dll\n");
    return 1;
  }

  auto AIL_set_redist_directory = (void *(*)(const char *))GetProcAddress(
      mss32, "_AIL_set_redist_directory@4");
  auto AIL_startup = (int (*)())GetProcAddress(mss32, "_AIL_startup@0");
  auto AIL_decompress_ASI =
      (int (*)(char *, uint32_t, const char *, char **, uint32_t *,
               uint32_t))GetProcAddress(mss32, "_AIL_decompress_ASI@24");
  auto AIL_mem_free_lock =
      (void (*)(char **))GetProcAddress(mss32, "_AIL_mem_free_lock@4");
  auto AIL_shutdown = (int (*)())GetProcAddress(mss32, "_AIL_shutdown@0");

  AIL_set_redist_directory(".");
  AIL_startup();

  char *converted;
  uint32_t num = 0;

  if (AIL_decompress_ASI(data, (uint32_t)data_size, ".binka", &converted, &num,
                         0U) == 0) {
    log_print("bink2wav: |   failed to decompress %ls\n", binka);
    return 1;
  }

  AIL_mem_free_lock(&data);
  AIL_shutdown();
  FreeLibrary(mss32);
  log_print("bink2wav: |   Convert... done\n");
  std::fstream fs2(wav, std::ios::out | std::ios::binary);
  if (!fs2.is_open()) {
    log_print("bink2wav: |   failed to open %ls\n", wav);
    return -1;
  }
  fs2.write(converted, num);
  fs2.close();
  delete[] converted;
  std::filesystem::remove("./mss32.dll");
  std::filesystem::remove("./binkawin.asi");
  return 0;
}