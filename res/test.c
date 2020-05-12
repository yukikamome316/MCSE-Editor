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
    printf("Extract ");
    checkRet(
        // extract and load by %1
        extractMsscmp(argv[1])
    );

    printf("save    ");
    remove("out.msscmp");
    checkRet(
        // save to %1
        saveMsscmp("./out.msscmp")
    );
    
    printf("replace ");
    checkRet(
        //msscmp %1  ->   %2
        replaceEntryMsscmp(
            "Minecraft/ambient/cave/cave1_fixed/_16278_192512.binka",
            "replace.data"
        )
    );

    return 0;
}
