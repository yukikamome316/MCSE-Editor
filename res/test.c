#include <stdio.h>
#include "msscmp.h"

int main(int argc, char const *argv[])
{
    int ret=0;
    if(argc <= 1){
        printf("usage: test <path to msscmp :path>");
        exit(1);
    }
    printf("TestLoad: Start\n");
    extractMsscmp(argv[1]);
    printf("TestLoad: Passed\n\n");
    printf("Test    : Start\n");
    ret=remove("out.msscmp");
    printf("Test    : Ret     : dele = %d\n",ret);
    if(ret == 1){
        printf("Test    : Error   : dele failed");
        return 1;
    }
    
    ret=loadMsscmp(argv[1]);
    printf("Test    : Ret     : load = %d\n",ret);
    if(ret == 1){
        printf("Test    : Error   : load failed");
        return 1;
    }
    
    ret=saveMsscmp("./out.msscmp");
    printf("Test     : Ret     : save = %d\n",ret);
    if(ret == 1){
        printf("Test    : Error   : save failed");
        return 1;
    }
    
    ret=replaceEntryMsscmp("Minecraft/ambient/cave/cave1_fixed/_16278_192512.binka","replace.data");
    printf("Test    : Ret     : edit = %d\n",ret);
    if(ret == 1){
        printf("Test    : Error   : edit failed");
        return 1;
    }

    printf("Test    : Passed\n\n");
    return 0;
}
