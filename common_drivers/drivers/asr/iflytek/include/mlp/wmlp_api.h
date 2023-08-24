#ifndef __WMLP_API_H__
#define __WMLP_API_H__

#include "wmlp_err.h"
#include "wmlp_type.h"

#ifndef WIN32
//#include <stddef.h>
#endif


#if defined(_MSC_VER)                 /* Microsoft Visual C++ */
#if !defined(WMLPAPI)
#define WMLPAPI __stdcall
#endif
#pragma pack(push, 8)
#else                                          /* Any other including Unix */
#if !defined(WMLPAPI)
#define WMLPAPI  __attribute__ ((visibility("default")))
#endif
#endif


#ifdef __cplusplus
extern "C" {
#endif

int WMLPAPI wMLPInitialize(const void *workdir);
int WMLPAPI wMLPUninitialize(void);
int WMLPAPI wMLPStart(MLPHandle mlpHandle,int nResCount);
int WMLPAPI wMLPStop(MLPHandle mlpHandle);
void WMLPAPI wMLPReset(MLPHandle mlpHandle);
int WMLPAPI wMLPCreate(MLPHandle *phHandle);
void WMLPAPI wMLPDestroy(MLPHandle hHandle);
int WMLPAPI wMLPParameterSet(MLPHandle phHandle, const short *para,  const short *value);
int WMLPAPI wMLPParameterGet(MLPHandle phHandle, const short *para,  short *value, int len);
int WMLPAPI wMLPPushTask(MLPHandle mlpHandle, const void *in, short *out);
int WMLPAPI wMLPGetOutputFeatureSize(const MLPHandle mlpHandle);

#ifdef __cplusplus
};
#endif


/* Reset the structure packing alignments for different compilers. */
#if defined(_MSC_VER)                /* Microsoft Visual C++ */
#pragma pack(pop)
#endif

#endif /* __WFEXTR_API_H__ */
