#ifndef __VAD_H__
#define __VAD_H__

#include "vad_type.h"
#include "vad_err.h"
#include "../ivw_defines.h"

typedef  void*  WVAD_INST;

#if defined(_MSC_VER)                 /* Microsoft Visual C++ */
#if !defined(VADAPI)
#define VADAPI __stdcall
#endif
#pragma pack(push, 8)
#else                                          /* Any other including Unix */
#if !defined(VADAPI)
#define VADAPI  __attribute__ ((visibility("default")))
#endif
#endif


#ifdef __cplusplus
extern "C" {
#endif

ivInt VADAPI wVadInitialize(WVAD_INST wVadInst);
ivInt VADAPI wVadUninitialize(void);
ivInt VADAPI wVadStart(WVAD_INST wVadInst);
ivInt VADAPI wVadStop(WVAD_INST wVadInst);
ivInt VADAPI wVadWrite(WVAD_INST wVadInst, const short *samples, const ivInt nSamplesInChar);
ivInt VADAPI wVadRead(WVAD_INST wVadInst, short *samples, ivInt nMaxBufferSize, ivInt *nOutSize, VadReadStatus *eVadReadSatus);
ivInt VADAPI wVadSetParameter(WVAD_INST wVadInst,  const short *param, const short *value);
ivInt VADAPI wVadGetParameter(WVAD_INST wVadInst,  const short *param, short *value, ivInt len);

#ifdef __cplusplus
};
#endif


/* Reset the structure packing alignments for different compilers. */
#if defined(_MSC_VER)                /* Microsoft Visual C++ */
#pragma pack(pop)
#endif


#endif /* __SAD_H__ */
