#pragma once

#ifdef MSSCMP_EXPORTS
#define MSSCMP_API __attribute__((visibility("default")))
#else
#define MSSCMP_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

MSSCMP_API int wav2binka(wchar_t* wav, wchar_t* binka);
MSSCMP_API int binka2wav(wchar_t* binka, wchar_t* wav);

#ifdef __cplusplus
}
#endif