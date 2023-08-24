#ifndef __WVAD_ERROR_H__
#define __WVAD_ERROR_H__
#include "../errcode_module.h"
enum
{
	WVAD_SUCCESS							=  SUCCESS,
	/*Common errors */
	WVAD_ERROR_GENERAL						= ERROR_W_VAD,
	WVAD_ERROR_ALREADY_INIT,
	WVAD_ERROR_NOT_INIT,
	WVAD_ERROR_ALREADY_START,
	WVAD_ERROR_NOT_START,
	WVAD_ERROR_RESOURCE_NOT_EXIST,
	WVAD_ERROR_RESOURCE_ALREADY_EXIST,
	WVAD_ERROR_LOAD_FILE,
	WVAD_ERROR_INVALID_PARA,
	WVAD_ERROR_INVALID_PARA_VALUE,
	WVAD_ERROR_NULL_HANDLE,
	WVAD_ERROR_INVALID_HANDLE,
	WVAD_ERROR_AUDIO_ENOUGH,
	WVAD_ERROR_AUDIO_NOT_END,
	WVAD_ERROR_INST_NULL,
	WVAD_ERROR_LOAD_LIBRARY,
	WVAD_ERROR_NO_ENOUGH_BUFFER,
	WVAD_ERROR_UNKNOW_TYPE,
	WVAD_ERROR_FAIL,
	WVAD_ERROR_EXCEPTION
};

#endif


