#ifndef __FEA_TYPE_H__
#define __FEA_TYPE_H__
#include "../ivw_defines.h"

typedef enum
{
	WFEA_FEATURE_NONE = 0,
	WFEA_FEATURE_MORE,
	WFEA_FEATURE_FINISH
} WFEA_FEATURE_STATUS;

typedef struct tagWFEAFeatureData
{	
	ivShort				nFrameNum;
	ivShort				nFtrDim;
	ivInt				featureStatus;
} WFEA_FEATURE_DATA;

#endif
