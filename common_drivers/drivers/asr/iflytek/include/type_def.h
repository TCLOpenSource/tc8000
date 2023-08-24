#pragma once

/* data type */
typedef char str64[64];

/* macro */
#ifdef WIN32
#include <crtdbg.h>
#define W_ASSERT						_ASSERT
#define W_INLINE						__inline
#define W_ASSERT_EQUAL(x, y, epsion)	_ASSERT( fabs(x-y) < epsion)
#else
#define W_ASSERT						assert
#define W_INLINE						inline
#define W_ASSERT_EQUAL(x, y, epsion)	assert(fabs(x-y) < epsion)
#endif