#ifndef __DEC_TYPE_H__
#define __DEC_TYPE_H__
#include "../ivw_defines.h"


typedef enum
{
	IVW_MAP= 0,
	IVW_FILLER,
	IVW_KEYWORD,
	IVW_UNDEFINE
}WDEC_RES_TYPE;

typedef struct tagDecResSet
{
	void*			pRes_;
	int				res_id_;
	WDEC_RES_TYPE	szResType_; //RES_MAP,RES_NET
}DecResSet, *pDecResSet;


#endif

