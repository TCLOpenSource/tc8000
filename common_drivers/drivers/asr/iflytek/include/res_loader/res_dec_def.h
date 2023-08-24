#ifndef __RES_DEC_DEF_H__
#define __RES_DEC_DEF_H__
#include "../ivw/ivw_type.h"
#include "../basic_defines.h"
//typedef char str64[64];
typedef char str8[8];

typedef struct Filler_Arc
{
	short state_id_[MAX_STATE_COUNT_ARC];
	short nStateCount_;
	short nLabelID_;
	short nLmScore;	
}TFiller_Arc;

typedef struct FillerArcNet
{
	TFiller_Arc	*pArcs_;
	int			nArc_;
	str8		*pSzLabel_;
	int			nLablel_;
}TFillerArcNet;


typedef struct tagSubTreshold_Info
{
	short index;
	short subNcm;
}SubTreshold_Info;


typedef struct KeyWord_Arc
{
	//short state_id_[10*48];
	short state_id_[48];
	short nStateCount_;
	short nLabelID_;

	short nSubCmCount_;
	short nBoundCount_;
	short state_bound_[MAX_KEYWORD_WORD_NUM];
	SubTreshold_Info subCmThresHold[MAX_KEYWORD_WORD_NUM];

	short nLowCmThreshold;
	
	#pragma anon_unions
	union 
	{
		short nCmThresHold;
		short nLmScore;
	};	
}TKeyWord_Arc;

typedef struct KeywordArcNet
{
	TKeyWord_Arc	*pArcs_;
	int				nArc_;
	str64			*pSzLabel_;
	int				nLablel_;
}TKeywordArcNet;

#endif
