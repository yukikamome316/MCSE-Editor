#ifndef MSSCMP_MSSCMP_HPP_
#define MSSCMP_MSSCMP_HPP_

#include <fstream>
#include <vector>

#define msscmpDataAlign 0x00001000

#ifdef MSSCMP_EXPORTS
#define MSSCMP_API __attribute__((visibility("default")))
#else
#define MSSCMP_API
#endif

class Entry {
  class Offsets {
   public:
    uint32_t path;
    uint32_t name;
    uint32_t data;
    uint32_t info;
  };
  class Paths {
   public:
    std::string path;
    std::string name;
    std::string full;
  };

 public:
  Offsets offsets;
  Paths paths;
  uint32_t size;
  uint32_t sampleRate;
  std::vector<uint8_t> data;
};

class File {
 public:
  std::fstream stream;

  size_t entryCount;
  size_t filetableOffset;
  size_t entryStart;
  std::vector<Entry> entries;
};

#ifdef __cplusplus
extern "C" {
#endif

MSSCMP_API int loadMsscmp(const wchar_t* path);
MSSCMP_API int closeMsscmp();  // not impremented
MSSCMP_API int saveMsscmp(const wchar_t* path);

MSSCMP_API int extractMsscmp(const wchar_t* path);
MSSCMP_API int replaceEntryMsscmp(wchar_t* path, wchar_t* replacePath);
MSSCMP_API int wav2binka(wchar_t* wav, wchar_t* binka);
MSSCMP_API int binka2wav(wchar_t* binka, wchar_t* wav);

// internal
MSSCMP_API int extractLoadedMsscmp();

// debug
MSSCMP_API int showMsscmp();

// private
MSSCMP_API void Mprint(char* fmt);

#ifdef __cplusplus
}
#endif

#endif