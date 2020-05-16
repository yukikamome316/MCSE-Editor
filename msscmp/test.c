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
    printf("testRet : %-8s -> ",str);
    if (ret == 1)
    {
        printf("Fail");
        abort();
    }
    else
    {
        printf("Pass");
    }
    puts("\n");
}
void basicTest()
{
    checkRet(
        "Extract",
        // extract and load by %1
        extractMsscmp(mssPath));
    checkRet(
        "replace ",
        //msscmp %1  ->   %2
        replaceEntryMsscmp(
            L"Minecraft/UI/scroll/_914_9764864.binka",
            L"di.binka"));
    remove("out.msscmp");
    checkRet(
        "save    ",
        // save to %1
        saveMsscmp(L"./out.msscmp"));
}
void binkaTest()
{
    checkRet(
        "wav2bink",
        //convert %1(wav) => %2(binka)
        wav2binka(
            L"datas/di.wav",
            L"datas/do.binka"
        ));
    checkRet(
        "bink2wav",
        //convert %1(binka) => %2(wav)
        binka2wav(
            L"datas/di.binka",
            L"datas/do.wav"
        ));
}

int main(int argc, char const *argv[])
{
    //int ret = 0;
    //if (argc <= 1)
    //{
    //    printf("usage: test <path to msscmp :path>");
    //    exit(1);
    //}
    //wchar_t msscmpPath[strlen(argv[1]) * 2];
    //size_t converted;
    //ret = mbstowcs_s(&converted, msscmpPath, strlen(argv[1]) * 2, argv[1], strlen(argv[1]) * 2);
    //if (ret != 0)
    //{
    //    char error[256];
    //    strerror_s(error, 256, errno);
    //    printf("Failed to convert MultiByte to WIdeChar: %s\n", error);
    //    abort();
    //}
    //mssPath = msscmpPath;

    //Test
    binkaTest();
    loadMsscmp(L"../../data/minecraft.msscmp");
    replaceEntryMsscmp(L"Minecraft/random/drink/_6248_9228288.binka",L"datas/rand_drink.binka");
    saveMsscmp(L"out.msscmp");
    //freopen("CON","w",stdout);
    return 0;
}
