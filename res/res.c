#include "res.h"
void extractRes(enum libId id,char* out){
    char *start;
    int size;

    switch (id)
    {
    case RES_binkaEncode_exe:
        start = binary_binkaencodeExe;
        size = (int)binary_binkaencodeExe_Size;
        break;
    case RES_binkaWin_asi:
        start = binary_binwinAsi;
        size = (int)binary_binwinAsi_Size;
        break;
    case RES_mss32_dll:
        start = binary_mss32Dll;
        size = (int)binary_mss32Dll_Size;
        break;
    default:
        printf("warn:Invalid library id (ignore)\n");
        return;
        break;
    }
    
    FILE *fp;

    fopen_s(&fp,out,"wb");
    if(fp==NULL){
        char error[256];strerror_s(error,255,errno);
        printf("Failed to open resource dest: %s\n",error);
        return;
    }
    if(fwrite(start,1,size,fp)<size){
        char error[256];strerror_s(error,255,errno);
        printf("Failed to extract lib: %s",error);
    }
    fclose(fp);
}