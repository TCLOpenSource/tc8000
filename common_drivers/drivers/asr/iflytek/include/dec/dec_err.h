#ifndef __WDEC_ERROR_H__
#define __WDEC_ERROR_H__
#include "../errcode_module.h"
enum
{
	/* Generic Error defines */
	WDEC_SUCCESS							=SUCCESS,
	
	/* Common errors */
	WDEC_ERROR_GENERAL						=ERROR_W_DEC+1,
	WDEC_ERROR_ALREADY_INIT,
	WDEC_ERROR_NOT_INIT,
	WDEC_ERROR_ALREADY_START,
	WDEC_ERROR_NOT_START,
	WDEC_ERROR_RESOURCE_NOT_EXIST,
	WDEC_ERROR_RESOURCE_ALREADY_EXIST,
	WDEC_ERROR_RESOURCE_TYPE,
	WDEC_ERROR_LOAD_FILE,
	WDEC_ERROR_INVALID_PARA,
	WDEC_ERROR_INVALID_PARA_VALUE,
	WDEC_ERROR_NULL_HANDLE,
	WDEC_ERROR_INVALID_HANDLE,
	WDEC_ERROR_BUF_NOT_ENOUGH,
	WDEC_ERROR_AUDIO_ENOUGH,
	WDEC_ERROR_NO_VALID_AUDIO,
	WDEC_ERROR_LOAD_LIBRARY,
	WDEC_ERROR_DONNT_SUPPORT,
	WDEC_ERROR_FAIL,
	WDEC_ERROR_EXCEPTION,
	WDEC_ERROR_STATESIZE_NOT_SET
};

#endif	//__WDEC_ERROR_H__



