#include <stdio.h>
#include "msscmp.h"

wchar_t *mssPath;

void checkRet(int ret)
{
    if (ret == 1)
    {
        printf(" Fail");
        abort();
    }
    else
    {
        printf(" Pass");
    }
    putchar('\n');
}

void extractTest()
{
    printf("Extract ");
    checkRet(
        // extract and load by %1
        extractMsscmp(mssPath));
}

void saveTest()
{
    printf("save    ");
    remove("out.msscmp");
    checkRet(
        // save to %1
        saveMsscmp(L"./out.msscmp"));
}
void replaceTest()
{
    printf("replace ");
    checkRet(
        //msscmp %1  ->   %2
        replaceEntryMsscmp(
            L"Minecraft/ambient/cave/cave10/_17388_110592.binka",
            L"replace.data"));
}

void binka2wavTest()
{
    printf("bink2wav");
    checkRet(
        //convert %1(binka) => %2(wav)
        binka2wav(
            L"di.binka",
            L"do.wav"
        ));
}
void wav2binkaTest()
{
    printf("wav2bink");
    checkRet(
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
    //Init
    init();
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
    basicTest();
    binkaTest();
    return 0;
}
