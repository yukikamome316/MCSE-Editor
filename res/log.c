#include "log.h"

FILE *debugfile(int mode, int reason)
{
    static FILE *fp;
    if (mode == DF_SET)
    {
        if (reason == DF_CLOSE)
        {
            fclose(fp);
            fp = NULL;
        }
    }
    else
    {
        if (fp == NULL)
        {
            fopen_s(&fp, "mss.log", "wb");
            if (fp == NULL)
            {
                char error[256];
                strerror_s(error, 256, errno);
                fprintf(stdout, "Failed to open outputfile: %s\n", error);
            }
        }
    }
    return fp;
}
void _dprintf(char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    //vfprintf(stdout,fmt,va);
    vfprintf(debugfile(DF_GET,DF_NONE), fmt, va);
    va_end(va);
}
