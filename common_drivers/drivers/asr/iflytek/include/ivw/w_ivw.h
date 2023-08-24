#ifndef __W_IVW_H__
#define __W_IVW_H__

#include "../../include/param_module.h"
#include "../../include/basic_defines.h"
#include "../../include/ivw/ivw_type.h"


typedef void *WIVW_INST;

/*
typedef struct WakeUpResult
{
	ivInt iFrameStart_;
	ivInt nFrameDuration_;
	ivInt nFillerScore_;
	ivInt nKeyWordScore_;

	ivInt nCM_Thresh_;
	ivInt nCM_;	
	ivInt iResID_;
	char  pSzLabel_[64];
}TWakeUpResult;
*/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define USE_EXT_MEM
ivInt wIvwGetInstSize(void);
#ifdef USE_EXT_MEM
ivInt wIvwCreate(WIVW_INST wIvwInst, const void *mlp_res, const void *filler_keywords_res);
#else
ivInt wIvwCreate(WIVW_INST *wIvwInst, const void *mlp_res, const void *filler_keywords_res);
#endif
ivInt wIvwDestroy(WIVW_INST wIvwInst);
ivInt wIvwSetParameter(WIVW_INST wIvwInst, PARAM param, ivInt value);
ivInt wIvwGetParameter(WIVW_INST wIvwInst, PARAM param, ivInt *value);
ivInt wIvwRegisterCallBacks(WIVW_INST wIvwInst, CallBackType FuncType, const PIVWCallBack pFunc,void *pUserParam);
ivInt wIvwUnRegisterCallBacks(WIVW_INST wIvwInst, CallBackType FuncType);
ivInt wIvwStart(WIVW_INST wIvwInst);
ivInt wIvwStop(WIVW_INST wIvwInst);
ivInt wIvwWrite(WIVW_INST wIvwInst, const short *samples, ivInt len);
ivInt wIvwGetLastWakeupRst(WIVW_INST wIvwInst, void *rst);
ivChar *wIvwGetVersion(void);
ivInt wIvwFlush(WIVW_INST wIvwInst);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //__W_IVW_H__

