#ifndef __VAD_TYPE_H__
#define __VAD_TYPE_H__
#include "../ivw_defines.h"

typedef struct tagVadReadStatus
{	
	ivInt				iFrame;
	ivInt				nVadStatus;
} VadReadStatus;

typedef enum
{
	WVAD_STATUS_NONE = 0,
	WVAD_STATUS_BEGIN ,
	WVAD_STATUS_CONTINUE,
	WVAD_STATUS_END,
	WVAD_STATUS_FINISH
} WVAD_FRAME_STATUS;

#endif
