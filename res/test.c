#ifdef FAST
    #define MSSCMP_EXPORT
#endif

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include "prototype.h"
#include "msscmp.h"

wchar_t *mssPath;

void checkRet(char str[8],int ret)
{
    printf("testRet :%8s -> ",str);
    if (ret == 1)
    {
        printf("Fail");
        abort();
    }
    else
    {
        printf("Pass");
    }
    putchar('\n');
}

void extractTest()
{
    checkRet(
        "Extract",
        // extract and load by %1
        extractMsscmp(mssPath));
}

void saveTest()
{
    remove("out.msscmp");
    checkRet(
        "save    ",
        // save to %1
        saveMsscmp(L"./out.msscmp"));
}
void replaceTest()
{
    checkRet(
        "replace ",
        //msscmp %1  ->   %2
        replaceEntryMsscmp(
            L"Minecraft/ambient/cave/cave10/_17388_110592.binka",
            L"do.binka"));
}

void binka2wavTest()
{
    checkRet(
        "bink2wav",
        //convert %1(binka) => %2(wav)
        binka2wav(
            L"di.binka",
            L"do.wav"
        ));
}
void wav2binkaTest()
{
    checkRet(
        "wav2bink",
        //msscmp %1  ->   %2
        wav2binka(
            L"di.wav",
            L"do.binka"
        ));
}
void basicTest()
{
    extractTest();
    replaceTest();
    saveTest();
}
void binkaTest()
{
    wav2binkaTest();
    binka2wavTest();
}

int main(int argc, char const *argv[])
{
    int ret = 0;
    if (argc <= 1)
    {
        printf("usage: test <path to msscmp :path>");
        exit(1);
    }
    wchar_t msscmpPath[strlen(argv[1]) * 2];
    size_t converted;
    ret = mbstowcs_s(&converted, msscmpPath, strlen(argv[1]) * 2, argv[1], strlen(argv[1]) * 2);
    if (ret != 0)
    {
        char error[256];
        strerror_s(error, 256, errno);
        printf("Failed to convert MultiByte to WIdeChar: %s\n", error);
        abort();
    }
    mssPath = msscmpPath;
    //Test
    binkaTest();
    basicTest();
    extractMsscmp(L"out.msscmp");
    //extractTest();
    //replaceTest();
    //saveTest();
    //extractMsscmp(L"out.msscmp");

    //freopen("CON","w",stdout);
    return 0;
}
