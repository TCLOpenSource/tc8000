#ifndef __W_DEC_H__
#define __W_DEC_H__
#include "dec_type.h"
#include "dec_err.h"
#if defined(_MSC_VER)                 /* Microsoft Visual C++ */
#if !defined(WDECAPI)
#define WDECAPI __stdcall
#endif
#pragma pack(push, 8)
#else                                          /* Any other including Unix */
#if !defined(WDECAPI)
#define WDECAPI  __attribute__ ((visibility("default")))
#endif
#endif

typedef  void*  WDEC_INST;

#ifdef __cplusplus
extern "C" {
#endif

ivInt WDECAPI wDecInitialize(WDEC_INST wIvwInst, const void *filler_keywords_res);
ivInt WDECAPI wDecUninitialize(void);

#ifdef __cplusplus
};
#endif

/* Reset the structure packing alignments for different compilers. */
#if defined(_MSC_VER)                /* Microsoft Visual C++ */
#pragma pack(pop)
#endif

#endif	// __WFST_DC_H__

