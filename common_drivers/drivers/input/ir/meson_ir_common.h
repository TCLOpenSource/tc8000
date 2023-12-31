/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __UAPI_IR_COMMON_MESON_H__
#define __UAPI_IR_COMMON_MESON_H__

#include <linux/types.h>

#define MAX_KEYMAP_SIZE 256
#define CUSTOM_NAME_LEN 64

/*to ensure kernel and user spase use the same header file*/
#define SHARE_DATA_VERSION "v1.1.1"

union _codemap {
	struct ir_key_map {
		__u16 keycode;
		__u16 scancode;
		} map;
	__u32 code;
};

/*
 *struct cursor_codemap - codemap for mouse mode
 *
 *@fn_key_scancode: scancode of fn key which used to switch mode
 *@cursor_left_scancode: scancode of left key
 *@cursor_right_scancode: scancode of right key
 *@cursor_up_scancode: scancode of up key
 *@cursor_down_scancode: scancode of down key
 *@cursor_ok_scancode: scancode of ok key
 */
struct cursor_codemap {
	__u16 fn_key_scancode;
	__u16 cursor_left_scancode;
	__u16 cursor_right_scancode;
	__u16 cursor_up_scancode;
	__u16 cursor_down_scancode;
	__u16 cursor_ok_scancode;
};

/**
 *struct ir_map_table - the IR key map table for different remote-control
 *
 *@custom_name: table name
 *@cursor_code: mouse mode need
 *@map_size: number of IR key
 *@custom_code: custom code, identify different key mapping table
 *@release_delay: release delay time
 *@codemap[0]: code for IR key
 */
struct ir_map_tab {
	char custom_name[CUSTOM_NAME_LEN];
	struct cursor_codemap cursor_code;
	__u16 map_size;
	__u32 custom_code;
	__u32 release_delay;
	__u32 vendor;
	__u32 product;
	__u32 version;
	union _codemap codemap[0];
};

/**
 *struct ir_sw_decode_para - configuration parameters for software decode
 *
 *@max_frame_time: maximum frame time
 */
struct ir_sw_decode_para {
	unsigned int  max_frame_time;
};

/*IOCTL commands*/
#define IR_IOC_SET_KEY_NUMBER        _IOW('I', 3, __u32)
#define IR_IOC_SET_KEY_MAPPING_TAB   _IOW('I', 4, __u32)
#define IR_IOC_SET_SW_DECODE_PARA    _IOW('I', 5, __u32)
#define IR_IOC_GET_DATA_VERSION      _IOR('I', 121, __u32)

#endif
