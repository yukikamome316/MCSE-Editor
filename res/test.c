#include <tiny_stdlib.h>
#include "msscmp.h"

int main(int argc, char const *argv[])
{
    if(argc <= 1){
        printf("usage: test <path to msscmp :path>");
        exit(1);
    }
    printf("Test    : Try extract\n");
    extractMsscmp(argv[1]);
    printf("Done");
    return 0;
}
