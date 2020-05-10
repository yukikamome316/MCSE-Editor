#include <stdio.h>
#include "msscmp.h"

int main(int argc, char const *argv[])
{
    if(argc <= 1){
        printf("usage: test <path to msscmp :path>");
        exit(1);
    }
    printf("TestExtr: Start\n");
    extractMsscmp(argv[1]);
    printf("TestExtr: Passed\n\n");
    
    return 0;
}
