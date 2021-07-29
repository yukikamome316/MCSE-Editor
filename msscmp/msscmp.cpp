#define MSSCMP_EXPORT
#include "msscmp.hpp"

#include <cmath>
#include <filesystem>
#include <fstream>

#include "log.hpp"

File file;
int error;
char *backup;
int no = 1334;
std::wstring path;

enum ENDIAN { LITTLE, BIG } endian;

// Code by https://qiita.com/iseki-masaya/items/70b4ee6e0877d12dafa8
// Thanks for qiita user '@iseki-masaya'
std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  std::string item;
  for (char ch : s) {
    if (ch == delim) {
      if (!item.empty()) elems.push_back(item);
      item.clear();
    } else {
      item += ch;
    }
  }
  if (!item.empty()) elems.push_back(item);
  return elems;
}

// Converters
std::string wstr2str(std::wstring str) {
  std::string ret;
  for (auto c : str) {
    ret += c;
  }
  return ret;
}
std::wstring str2wstr(std::string str) {
  std::wstring ret;
  for (auto c : str) {
    ret += c;
  }
  return ret;
}
uint32_t to_uint32(const uint8_t *data) {
  if (endian == LITTLE) {
    return data[0] << 0x00 | data[1] << 0x08 | data[2] << 0x10 |
           data[3] << 0x18;
  } else {
    return data[3] << 0x00 | data[2] << 0x08 | data[1] << 0x10 |
           data[0] << 0x18;
  }
}
char *to_char(uint32_t data) {
  char *data_char = new char[4];
  if (endian == LITTLE) {
    data_char[0] = (char)data;
    data_char[1] = (char)(data >> 8);
    data_char[2] = (char)(data >> 16);
    data_char[3] = (char)(data >> 24);
  } else {
    data_char[3] = (char)data;
    data_char[2] = (char)(data >> 8);
    data_char[1] = (char)(data >> 16);
    data_char[0] = (char)(data >> 24);
  }
  return data_char;
}

// Read 32bit integer by file pointer
uint32_t readFile32bit() {
  uint8_t *a = new uint8_t[4];
  file.stream.read((char *)a, 4);
  auto val = to_uint32(a);
  return val;
}
// Write 32bit integer by file pointer
void writeFile32bit(uint32_t val) { file.stream << to_char(val); }

// Read file while to null
std::string readFileString() {
  std::string ret;
  char ch = 0xff;
  while (ch != '\0') {
    file.stream >> ch;
    ret += ch;
  }
  return ret;
}

// seek
void seek(int pos) {
  file.stream.seekg(pos, std::ios::beg);
  file.stream.seekp(pos, std::ios::beg);
}

// Skip by seek
void skip(int pos) {
  file.stream.seekg(pos, std::ios::cur);
  file.stream.seekp(pos, std::ios::cur);
}

// check exist file
bool existFile(std::string path) {
  std::ifstream ifs(path);
  return ifs.is_open();
}

// create the file
// source by: https://qiita.com/hirocueki/items/f5288b9fc757b10addb6
void createFile(char *filename) { std::ofstream strm(filename); }

// EXTERNED
// extract loaded msscmp
MSSCMP_API int extractLoadedMsscmp() {
  log_print("extract :　Extracting\n");
  for (int i = 0; i < file.entryCount; i++) {
    std::filesystem::create_directories(file.entries[i].paths.path);
    std::fstream fs(file.entries[i].paths.full);
    for (auto &&byte : file.entries[i].data) {
      fs.write(reinterpret_cast<char *>(&byte), 1);
    }
  }
  return 0;
}

// EXTERNED
// remapping msscmp entries
MSSCMP_API void remapMsscmp() {
  uint32_t currentPos = file.entryStart;
  for (int i = 0; i < file.entryCount; i++) {
    file.entries[i].offsets.data = currentPos;
    currentPos +=
        (int)std::ceil((float)file.entries[i].data.size() / msscmpDataAlign) *
        msscmpDataAlign;
  }
}

// EXTERNED
// extract msscmp (Minecraft Sound Source CoMPressed ?)
MSSCMP_API int extractMsscmp(const wchar_t *path) {
  log_print("extract : Extracting %ls\n", path);
  if (loadMsscmp(path) == 1) return 1;
  if (extractLoadedMsscmp() == 1) return 1;
  return 0;
}

// EXTERNED
// load msscmp to internal storage
MSSCMP_API int loadMsscmp(const wchar_t *_path) {
  auto path_tmp = wstr2str(_path);
  path = _path;
  log_print("load    : Loading %s\n", path_tmp.c_str());
  file.stream.open(path_tmp, std::ios::in | std::ios::out | std::ios::binary);
  if (!file.stream.is_open()) {
    log_print("load    : Failed to open %ls\n", path);
    return 1;
  }
  endian = BIG;
  uint32_t magic = readFile32bit();
  log_print("load    : Magic: %08X\n", magic);
  if (magic == 0x42414e4b) {
    endian = BIG;
    log_print("load    : detected big endian Platform\n");
  } else if (magic == 0x4b4e4142) {
    endian = LITTLE;
    log_print("load    : detected little endian Platform\n");
  } else {
    error = 1;
    log_print("load    : Failed to Check msscmp Signeture\n");
    return 1;
  }

  seek(0x00000018);
  file.filetableOffset = readFile32bit();
  log_print("load    : file table offset = %d\n", file.filetableOffset);
  seek(0x00000034);
  file.entryCount = readFile32bit();
  log_print("load    : entry count =  %d\n", file.entryCount);
  std::filesystem::create_directory("tmp");
  log_print("load    : tmp directory created\n");

  if (file.entryCount == 0) {
    error = 0;
    log_print("load    : Not found files\n");
    return 1;
  }

  log_print("load    : Loading Files\n");
  Entry entry;
  for (int i = 0; i < file.entryCount; i++) {
    // entry = new Entry;
    // if (entry == NULL) {
    //   log_print("load    : Failed to malloc entry\n");
    //   error = 1;
    //   return 1;
    // }

    seek(file.filetableOffset + 8 * i);
    entry.offsets.path = readFile32bit();
    entry.offsets.info = readFile32bit();

    seek(entry.offsets.info + 4);
    entry.offsets.name = readFile32bit() + entry.offsets.info;
    entry.offsets.data = readFile32bit();
    skip(8);
    entry.sampleRate = readFile32bit();
    entry.size = readFile32bit();

    seek(entry.offsets.path);
    entry.paths.path = readFileString();

    seek(entry.offsets.name);
    entry.paths.name = readFileString();

    entry.paths.full = "tmp/" + entry.paths.path + "/" + entry.paths.name;

    seek(entry.offsets.data);
    entry.data.resize(entry.size);
    file.stream.read((char *)entry.data.data(), entry.size);
    file.entries.push_back(entry);
  }

  // Get entry startpoint
  log_print("load    : |   get file start point\n");
  uint32_t startEntry = 0xffffffff;
  for (int i = 0; i < file.entryCount; i++) {
    auto offset = file.entries[i].offsets.data;
    if (startEntry > offset) {
      startEntry = offset;
    }
  }
  file.entryStart = startEntry;

  // Get backup ( header )
  log_print("load    : |   get file headers backup\n");

  seek(0);

  backup = new char[startEntry];
  if (backup == NULL) {
    error = 1;
    log_print("load    : Failed to Malloc msscmp backup\n");
    return 1;
  }
  file.stream.read(backup, startEntry);

  log_print("load    : +   %08x \n", file.entryStart);
  return 0;
}

// EXTERNED
// save msscmp
MSSCMP_API int saveMsscmp(const wchar_t *dest) {
  log_print("save    : saving %ls\n", dest);
  int ret;

  std::filesystem::remove(dest);
  std::filesystem::copy(path, dest);
  std::fstream fs(wstr2str(dest), std::ios::out | std::ios::binary);
  if (!fs.is_open()) {
    error = 1;
    log_print("save    : Failed to open file\n");
    return 1;
  }
  // Write Msscmp Header
  fs.write(backup, file.entryStart);

  remapMsscmp();

  // Write Msscmp Entries
  for (int i = 0; i < file.entryCount; i++) {
    // Write data
    fs.seekp(file.entries[i].offsets.data, std::ios::beg);
    fs.write((char *)file.entries[i].data.data(), file.entries[i].size);
    // Write info
    fs.seekp(file.entries[i].offsets.info, std::ios::beg);
    fs.seekp(8, std::ios::cur);
    fs.write(to_char(file.entries[i].offsets.data), 4);
    fs.seekp(12, std::ios::cur);
    fs.write(to_char(file.entries[i].size), 4);
  }
  fs.close();
  return 0;
}

// EXTERNED
// replace msscmp entry data
MSSCMP_API int replaceEntryMsscmp(wchar_t *_path, wchar_t *replacePath) {
  log_print("replace : Replacing %ls to %ls\n", _path, replacePath);
  // Get Entry Number
  int i;
  auto path = wstr2str(std::wstring(_path));
  for (i = 0; i < file.entryCount; i++)
    if (path == file.entries[i].paths.full.substr(4)) break;
  if (i >= file.entryCount) {
    log_print("Failed to Search target file\n");
    return 1;
  }

  no = i;
  log_print("replace : |   replace file index = %d\n", no);

  // Open replace Path in `rb`
  std::fstream fs(wstr2str(replacePath), std::ios::in | std::ios::binary);
  if (!fs.is_open()) {
    error = 1;
    log_print("replace : Failed to open file\n");
    return 1;
  }

  // Get replace file size
  file.entries[no].size = fs.tellg();
  fs.seekg(0, std::ios::end);
  file.entries[no].size = fs.tellg() - file.entries[no].size;
  fs.seekg(0, std::ios::beg);
  log_print("replace : +   set size %s[%d] -> 0x%08x\n",
            file.entries[no].paths.full, i, file.entries[no].size);

  // Read replace file
  file.entries[no].data.resize(file.entries[no].size);
  fs.read((char *)file.entries[no].data.data(), file.entries[no].size);

  // Close replace file
  fs.close();
  return 0;
}

// EXTERNED
// close msscmp (not impremented)
MSSCMP_API int closeMsscmp() {
  // Close entries
  Entry *ent;
  for (int i = 0; i < file.entryCount; i++) {
    file.entries[i].data.clear();
  }
  file.entries.clear();
  // reset internal variables
  file.entryCount = 0;
  file.entryStart = 0;
  file.filetableOffset = 0;
  // close files
  file.stream.close();
  return 0;
}

// EXTERNED
// show msscmp
MSSCMP_API int showMsscmp() {
  int i;
  log_print("show    : Show Current msscmp files\n");
  for (i = 0; i < 10; i++) {
    log_print("show    : |   %-8d{size=%08x, fdata=%08x, finfo=%08x %s}\n", i,
              file.entries[i].size, file.entries[i].offsets.data,
              file.entries[i].offsets.info, file.entries[i].paths.full.c_str());
    seek(file.entries[i].offsets.name);
  }
  log_print("show    : +\n");
  return 0;
}

// PRIVATE EXTERNED
// print
MSSCMP_API void Mprint(char *fmt) { log_print("%s", fmt); }
