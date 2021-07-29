#ifndef RES_H
#define RES_H

#include <stdio.h>

#include "../log.h"
#include "resource.h"

enum libId {
  RES_binkaWin_asi,
  RES_binkaEncode_exe,
  RES_mss32_dll,
};

#ifdef __cplusplus
extern "C" {
#endif

void extractRes(enum libId id, char* out);

#ifdef __cplusplus
}
#endif

#endif /* !defined(RES_H) */