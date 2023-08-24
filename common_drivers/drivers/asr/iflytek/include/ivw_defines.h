#ifndef __IVW_DEFINES_H__
#define __IVW_DEFINES_H__



#define SIZEOF_CHAR					(1)
#define SIZEOF_SHORT				(2)
#define SIZEOF_INT					(4)

#define IVW_MAX_STR_BUF_LEN			(256)
#define IVW_FIXED_WRITE_BUF_LEN		(320)	//fixed write buffer length in bytes. same as NSAMPLE_FRAME_SHIFT

typedef	int					ivBool;	
#ifndef __cplusplus	
#ifndef true
//#define true 1
#endif
#ifndef false
//#define false 0
#endif
#endif

typedef	signed char			ivInt8;		/* 8-bit */
typedef	unsigned char		ivUInt8;	/* 8-bit */
typedef	char				ivChar;		/* 8-bit */
typedef	unsigned char		ivUChar;	/* 8-bit */


typedef	signed short		ivInt16;	/* 16-bit */
typedef	unsigned short		ivUInt16;	/* 16-bit */
typedef	signed short		ivShort;	/* 16-bit */
typedef	unsigned short		ivUShort;	/* 16-bit */

typedef	signed int			ivInt32;	/* 32-bit */
typedef	unsigned int		ivUInt32;	/* 32-bit */
typedef	signed int			ivInt;	/* 32-bit */
typedef	unsigned int		ivUInt;	/* 32-bit */
typedef void				ivVoid;


#ifdef IV_TYPE_INT64
typedef	signed long long	ivInt64;	/* 64-bit */
typedef	unsigned long long 	ivUInt64;	/* 64-bit */
#endif

#endif
