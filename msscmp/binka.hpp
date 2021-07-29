#pragma once

#ifdef MSSCMP_EXPORTS
#define MSSCMP_API __attribute__((visibility("default")))
#else
#define MSSCMP_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

MSSCMP_API int wav2binka(const wchar_t* wav, const wchar_t* binka);
MSSCMP_API int binka2wav(const wchar_t* binka, const wchar_t* wav);

#ifdef __cplusplus
}
#endif