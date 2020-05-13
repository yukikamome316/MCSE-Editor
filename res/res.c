#include "res.h"
void extractRes(enum libId id,char* out){
    char *start;
    int size;

    switch (id)
    {
    case RES_binkaEncode_exe:
        start = _binary_binka_encode_exe_start;
        size = (int)_binary_binka_encode_exe_size;
        break;
    case RES_binkaWin_asi:
        start = _binary_binkawin_asi_start;
        size = (int)_binary_binkawin_asi_size;
        break;
    case RES_mss32_dll:
        start = _binary_mss32_dll_start;
        size = (int)_binary_mss32_dll_size;
        break;
    default:
        start=NULL;
        size=0;
        printf("warn:Invalid library id (ignore)\n");
        return;
        break;
    }
    
    FILE *fp;
    fopen_s(&fp,out,"wb");
    if(fp==NULL){
        printf("Failed to open resource dest (ignore)\n");
        return;
    }
    if(fwrite(start,1,size,fp)<size){
        char error[256];strerror_s(error,255,errno);
        printf("Failed to extract lib: %s",error);
    }
    fclose(fp);
}