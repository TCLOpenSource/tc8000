#ifndef __IVW_TYPE_H__
#define __IVW_TYPE_H__
#include "../ivw_defines.h"

typedef char str64[64];

typedef struct WakeUpResult
{
	int iFrameStart_;
	int nFrameDuration_;
	int nFillerScore_;
	int nKeyWordScore_;
	int nCM_;
	int nCM_keyword_;
	int nCM_filler_;

	int nCM_Thresh_;
	//int nCM_;	
	int iResID_;
	str64 *pSzLabel_;
}TWakeUpResult;

typedef enum
{
	CallBackFuncNameWakeUp =0,
	CallBackFuncNameWarmUp,
	CallBackFuncNamePreWakeup,
	CallBackFuncNameGetResult,
	CallBackFuncNameVadBegin,
	CallBackFuncNameSfgd,
	CallBackFuncNameCount
}CallBackType;


typedef enum 
{
	RES_MLP = 0,
	RES_FILLER_KEYWORDS,
	ResMax
}RES;

typedef ivInt (*PIVWCallBack)(const ivChar* pIvwParam,void *pUserParam); 

#endif
