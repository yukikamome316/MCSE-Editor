#include <stdio.h>
#include "msscmp.h"

int main(int argc, char const *argv[])
{
    if(argc <= 1){
        printf("usage: test <path to msscmp :path>");
        exit(1);
    }
    printf("TestLoad: Start\n");
    extractMsscmp(argv[1]);
    printf("TestLoad: Passed\n\n");
    printf("TestSave: Start");
    saveMsscmp("./out.msscmp");
    printf("TestSave: Passed\n\n");
    return 0;
}
