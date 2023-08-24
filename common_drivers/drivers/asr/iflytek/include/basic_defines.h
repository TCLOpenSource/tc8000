#ifndef __BASIC_DEFINES_H__
#define __BASIC_DEFINES_H__

//#define MLP_CNN_OFFLINE

#define  MD5_LEN	     	        (32)
#define  MAX_STATE_COUNT_ARC		 (2)
#define MAX_KEYWORD_WORD_NUM             (4)
//#define USE_WORD_THRESHOLD
#define USE_CM_SIRANBI

#define  W_LOG_ZERO					(-0x3FFFFFFF)

#define ivMax(a, b)  (((a) > (b))?(a):(b))
#define ivMin(a, b)  (((a) < (b))?(a):(b))

#ifdef _WIN32
#include <assert.h>
//#define IV_ASSERT(exp)			_ASSERT(exp)
#define IV_ASSERT(exp)			assert(exp)
//#define snprintf _snprintf
#else
#define IV_ASSERT(exp)
#define HMODULE	void*
#endif


#define STRING2(x) #x
#define STRING(x) STRING2(x)
#define MLP_ALIGN_SIZE_32			(32)

#ifdef _MSC_VER
// For The Compile Infomation
#define To_msg(msg)	__FILE__ "(" STRING(__LINE__) ") : "##msg
#define CompileMsg(msg) 	__pragma(message(To_msg("#######"##msg##"######")))
#else
#define CompileMsg(msg)
#endif

#define RDeclareApi(func)		extern Proc_##func  func##_;
#define DeclareApi(func)		Proc_##func  func##_;

#define GetProcApi(func)\
	{\
	func##_ = (Proc_##func)GetProcAddress(hand, #func);\
	if(func##_ == NULL)\
		{\
		sglog_error_assert_return(0, ("GetProcAddress | err, %s = NULL", #func), -1);\
		}\
	}
#define GetProcApiVar(func)\
	{\
	func##_ = (Proc_##func)( func);\
	}

#ifdef __cplusplus

#define DISALLOW_COPY_AND_ASSIGN(T)   T(T const&);   T& operator=(T const&){ return *this;}
#define MAKE_SINGLETON_NO_CONSTRUCT(T)   static T& get_inst(){	static T inst;	return inst;}
#define MAKE_SINGLETON(T)   T(){} static T& get_inst(){	static T inst;	return inst;}

#else

#ifdef _MSC_VER
typedef unsigned int bool;
//#	define true		1
//#	define false	0
#else
//#	include	<stdbool.h>
 #include <linux/types.h>
#endif

#endif

/*
#define __SCHAR_MAX__ 127
#define SCHAR_MAX __SCHAR_MAX__
#define SCHAR_MIN (-SCHAR_MAX - 1)
#define UCHAR_MAX (SCHAR_MAX*2U + 1)

#define __SHRT_MAX__ 32767
#define SHRT_MAX __SHRT_MAX__
#define SHRT_MIN (-SHRT_MAX-1)
#define USHRT_MAX (SCHAR_MAX*2U + 1)

#define __INT_MAX__ 2147483647
#define INT_MAX __INT_MAX__
#define INT_MIN (-INT_MAX-1)
#define UINT_MAX (INT_MAX * 2U + 1)

#define __LONG_LONG_MAX__ 9223372036854775807LL
#define LONG_LONG_MAX __LONG_LONG_MAX__
#define LONG_LONG_MIN (-LONG_LONG_MAX-1)
#define ULONG_LONG_MAX (LONG_LONG_MAX * 2ULL + 1)
*/
#endif
