#pragma once
#include <stdint.h>

#define MAX_RES_NUMS (10)

typedef enum
{
	MLP_RES = 0,
	FILLER_KEYWORDS_RES,
}eIvwResType;

// res info in flash
typedef struct TagIvwResHeader
{
	uint16_t	res_count_;
	eIvwResType res_types_[MAX_RES_NUMS];
	uint32_t	res_size_arr_[MAX_RES_NUMS];
}tIvwResHeader, *pIvwResHeader;

// res info in psram
typedef struct TagIvwResMgr
{
	uint16_t	res_count_;
	eIvwResType res_types_[MAX_RES_NUMS];
	uint32_t	res_size_arr_[MAX_RES_NUMS];
	char*   	res_buf_arr_[MAX_RES_NUMS];
}tIvwResMgr, *pIvwResMgr;

