#include <stdio.h>
#include "msscmp.h"

void checkRet(int ret){
    if(ret == 1){
        printf("Fail");
        abort();
    }else{
        printf("Pass");
    }
    putchar('\n');
}

int main(int argc, char const *argv[])
{
    int ret=0;
    if(argc <= 1){
        printf("usage: test <path to msscmp :path>");
        exit(1);
    }

    //Init
    //init();
    wchar_t msscmpPath[strlen(argv[1])*2];
    size_t converted;
    ret=mbstowcs_s(&converted,msscmpPath,strlen(argv[1])*2,argv[1],strlen(argv[1])*2);
    if(ret!=0){
        char error[256];
        strerror_s(error,256,errno);
        printf("Failed to convert MultiByte to WIdeChar: %s\n",error);
        abort();
    }
    //Test
    printf("Extract ");
    checkRet(
        // extract and load by %1
        extractMsscmp(msscmpPath)
    );

    printf("save    ");
    remove("out.msscmp");
    checkRet(
        // save to %1
        saveMsscmp(L"./out.msscmp")
    );
    
    printf("replace ");
    checkRet(
        //msscmp %1  ->   %2
        replaceEntryMsscmp(
            L"Minecraft/ambient/cave/cave1_fixed/_16278_192512.binka",
            L"replace.data"
        )
    );

    return 0;
}
