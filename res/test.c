#include <stdio.h>
#include "msscmp.h"

int main(int argc, char const *argv[])
{
    int ret=0;
    if(argc <= 1){
        printf("usage: test <path to msscmp :path>");
        exit(1);
    }
    //printf("TestLoad: Start\n");
    //extractMsscmp(argv[1]);
    //printf("TestLoad: Passed\n\n");
    printf("TestSave: Start\n");
    ret=remove("out.msscmp");
    printf("TestSave: Ret     : dele = %d\n",ret);
    if(ret == 1){
        printf("TestSave: Error   : dele failed");
        return 1;
    }
    
    ret=loadMsscmp(argv[1]);
    printf("TestSave: Ret     : load = %d\n",ret);
    if(ret == 1){
        printf("TestSave: Error   : load failed");
        return 1;
    }
    
    ret=saveMsscmp("./out.msscmp");
    printf("TestSave: Ret     : save = %d\n",ret);
    if(ret == 1){
        printf("TestSave: Error   : save failed");
        return 1;
    }
    

    printf("TestSave: Passed\n\n");
    return 0;
}
