#ifndef __W_IVW_DEF_H__
#define __W_IVW_DEF_H__

#include "../../../include/param_module.h"
#include "../../../include/ivw_defines.h"
#include "../../../include/ivw/ivw_type.h"
#include "../../../include/ivw/ivw_cfg.h"
#include "../../../include/ivw/ivw_err.h"
#include "../../../include/dec/dec_cfg.h"

#include "../../../include/fea/wfea_api.h"
#include "../../../include/dec/dec_api.h"
#include "../../../include/vad/wvad_api.h"
#include "../w_ivw/w_ivw_def.h"
#include "../w_vad/w_vad_def.h"
#include "../w_fea/w_fea_def.h"
#include "../w_dec/dec_imp.h"
#include "../w_agc/agc.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 资源信息块格式 */
typedef struct _tagResBlockInfo
{
	int			nResID;							/* 资源ID */
	char		sResType[16];					/* 资源描述，如“IVW_MLP” */
	int			nResSize;						/* 资源大小 */
	int			nResOffset;						/* 资源相对于资源包起始位置偏移 */
}TResBlockInfo;

typedef struct _tagCnnResHead
{
	char			sResInfo[32];			/* 资源描述 */
	char			sResVersion[16];		/* 资源版本 */
	int				nResNum;				/* 资源数 */
	TResBlockInfo	tResInfo[4];
}TCnnResHead;

/////////////////////////////////////////////////////////////////////
#define IVWINST_NSAMPLES_PER_FRAME_SHORT	(NSAMPLES_PER_FRAME)
#define IVWINST_NSAMPLES_PER_FRAME_CHAR		(IVWINST_NSAMPLES_PER_FRAME_SHORT*sizeof(short))
#define IVWINST_MAX_FEA_BUFF_SHORT			(MAX_FILLER_ARC_nArc_)
typedef struct IvwInst
{
	int		iFrame;
	int		bVadOn;
	int		bAgcOn;
	
	int		ivw_status;
	short	 pVadBuf_[IVWINST_NSAMPLES_PER_FRAME_SHORT];
	short	 pFeaBuf_[IVWINST_MAX_FEA_BUFF_SHORT * 4];
	ivChar   rlt_buf[IVW_MAX_STR_BUF_LEN];

	PIVWCallBack 	pFuncCallBack_[CallBackFuncNameCount];
	void 			*pFuncCallBackUserParam_[CallBackFuncNameCount];

	TVadInst  mVadInst;
	TFeaInst  mFeaInst;
	TDecInst  mDecInst;

	PVadInst wVadInst_;
	PFeaInst wFeaInst_;
	PDecInst wDecInst_;

	struct AGC wAgcInst_;
	
	void (*init)(void *pThis);
	void (*uninit)(void *pThis);
	ivInt (*Start)(void *pThis);
	ivInt (*Stop)(void *pThis);
	ivInt (*Write)(void *pThis, const short* samples, ivInt nSamplesInChar);
	ivInt (*ResourceAdd)(void *pThis, RES res, void *pBuf);
	ivInt (*ResourceDelete)(void *pThis, RES res);
	ivInt (*SetParameter)(void *pThis, PARAM param, int value);
	ivInt (*GetParameter)(void *pThis, PARAM param, int *value);	
	ivInt (*RegisterCallBacks)(void *pThis, CallBackType FuncType, const PIVWCallBack pFunc,void *pUserParam);
	ivInt (*UnRegisterCallBacks)(void *pThis, CallBackType FuncType);
	void  (*reset_call_back)(void *pThis);
	void  (*dec_frames)(void *pThis, const ivShort *pFea,int nFrames,int nDim);
	ivInt (*WriteNoVad)(void *pThis, const short *samples, ivInt nSamplesInChar);
	ivInt (*WriteVad)(void *pThis, const short *samples, ivInt nSamplesInChar);
	ivInt (*ReadWriteFeaVad)(void *pThis);
	ivInt (*GetLastWakeupRst)(void *pThis, void *rst);
	ivInt (*Flush)(void *pThis_tmp);
}TIvwInst;
void IvwInst_struct_init(TIvwInst *pThis);


#ifdef __cplusplus
};
#endif


int DefaultWakeUpCallBack(const ivChar *pIvwParam,void *pUserParam);
int DefaultDoNothingCallBack(const ivChar *pIvwParam,void *pUserParam);

#endif	//__W_IVW_DEF_H__
