#ifndef RES_H
#define RES_H

#include <stdio.h>
#include <stdlib.h>
#include "prototype.h"

enum libId{
    RES_binkaWin_asi,
    RES_binkaEncode_exe,
    RES_mss32_dll,
};

#ifdef __cplusplus
extern "C"{
#endif

extern char _binary_binkawin_asi_start[];
extern char _binary_binkawin_asi_size[];

extern char _binary_binka_encode_exe_start[];
extern char _binary_binka_encode_exe_size[];

extern char _binary_mss32_dll_start[];
extern char _binary_mss32_dll_size[];


void extractRes(enum libId id,char* out);


#ifdef __cplusplus
}
#endif

#endif /* !defined(RES_H) */