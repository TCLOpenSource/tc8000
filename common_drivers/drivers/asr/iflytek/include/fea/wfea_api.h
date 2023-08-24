#ifndef __FEA_H__
#define __FEA_H__

#include "fea_type.h"
#include "fea_err.h"


typedef  void*  WFEA_INST;

#if defined(_MSC_VER)                 /* Microsoft Visual C++ */
#if !defined(FEAAPI)
#define FEAAPI __stdcall
#endif
#pragma pack(push, 8)
#else                                          /* Any other including Unix */
#if !defined(FEAAPI)
#define FEAAPI  __attribute__ ((visibility("default")))
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

ivInt FEAAPI wFeaInitialize(WFEA_INST wFeaInst, const void *mlp_res);
ivInt FEAAPI wFeaUninitialize(WFEA_INST wFeaInst);
ivInt FEAAPI wFeaStart(WFEA_INST wFeaInst);
ivInt FEAAPI wFeaStop(WFEA_INST wFeaInst);
ivInt FEAAPI wFeaAudioWrite(WFEA_INST wFeaInst, const ivShort *samples,int sample_size);
ivInt FEAAPI wFeaFeatureRead(WFEA_INST wFeaInst, void* pfFeatures,int max_buf_size,WFEA_FEATURE_DATA* feature_data);
ivInt FEAAPI wFeaReset(WFEA_INST wFeaInst);
ivInt FEAAPI wFeaFlush(WFEA_INST wFeaInst);

#ifdef __cplusplus
};
#endif


/* Reset the structure packing alignments for different compilers. */
#if defined(_MSC_VER)                /* Microsoft Visual C++ */
#pragma pack(pop)
#endif


#endif /* __SAD_H__ */
