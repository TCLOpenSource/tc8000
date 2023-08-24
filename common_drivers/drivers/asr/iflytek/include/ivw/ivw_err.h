#ifndef __WIVW_ERROR_H__
#define __WIVW_ERROR_H__
enum
{
	/* Generic Error defines */
	WIVW_SUCCESS							=0,
	/* Common errors */
	WIVW_ERROR_GENERAL						=WIVW_SUCCESS+1,
	WIVW_ERROR_ALREADY_INIT,
	WIVW_ERROR_NOT_INIT,
	WIVW_ERROR_ALREADY_START,
	WIVW_ERROR_NOT_START,
	WIVW_ERROR_RESOURCE_NOT_EXIST,
	WIVW_ERROR_RESOURCE_ALREADY_EXIST,
	WIVW_ERROR_RESOURCE_TYPE,
	WIVW_ERROR_LOAD_FILE,
	WIVW_ERROR_INVALID_PARA,
	WIVW_ERROR_INVALID_PARA_VALUE,
	WIVW_ERROR_NULL_HANDLE,
	WIVW_ERROR_INVALID_HANDLE,
	WIVW_ERROR_BUF_NOT_ENOUGH,
	WIVW_ERROR_AUDIO_ENOUGH,
	WIVW_ERROR_NO_VALID_AUDIO,
	WIVW_ERROR_LOAD_LIBRARY,
	WIVW_ERROR_DONNT_SUPPORT,
	WIVW_ERROR_FAIL,
	WIVW_ERROR_EXCEPTION,
	WIVW_ERROR_STATESIZE_NOT_SET,
	WIVW_ERROR_WRITE_DATA_NOT_ALIGNED,	//write buffer length should be aligned to IVW_FIXED_WRITE_BUF_LEN or others, 
	WIVW_ERROR_AUTH_ERROR
};

#endif	//__WIVW_ERROR_H__


