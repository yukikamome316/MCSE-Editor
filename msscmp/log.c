#include "log.h"

#include <errno.h>

FILE *debugfile(int mode, int reason) {
  static FILE *fp;
  if (mode == DF_SET) {
    if (reason == DF_CLOSE) {
      fclose(fp);
      fp = NULL;
    }
  } else {
    if (fp == NULL) {
      fopen_s(&fp, "mss.log", "wb");
      if (fp == NULL) {
        fprintf(stdout, "Failed to open outputfile: %d\n", errno);
      }
    }
  }
  return fp;
}
void _dprintf(const char *fmt, ...) {
  va_list va;
  va_start(va, fmt);
  // #ifdef DIRECT
  vfprintf(stdout, fmt, va);
  // #endif
  vfprintf(debugfile(DF_GET, DF_NONE), fmt, va);
  va_end(va);
}
