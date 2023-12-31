/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 *
 * Copyright (C) 2016 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 */

#ifndef VFRAME_PROVIDER_H
#define VFRAME_PROVIDER_H

/* Standard Linux headers */
#include <linux/list.h>

/* Amlogic headers */
#include <linux/amlogic/media/vfm/vframe.h>
#include <linux/amlogic/media/utils/vformat.h>

struct vframe_states {
	int vf_pool_size;
	int buf_free_num;
	int buf_recycle_num;
	int buf_avail_num;
} /*vframe_states_t */;

#define VFRAME_EVENT_RECEIVER_GET               0x01
#define VFRAME_EVENT_RECEIVER_PUT               0x02
#define VFRAME_EVENT_RECEIVER_FRAME_WAIT        0x04

#define VFRAME_EVENT_RECEIVER_PARAM_SET			0x10
#define VFRAME_EVENT_RECEIVER_RESET				0x20
#define VFRAME_EVENT_RECEIVER_FORCE_UNREG			0x40
#define VFRAME_EVENT_RECEIVER_GET_AUX_DATA			0x80
#define VFRAME_EVENT_RECEIVER_DISP_MODE				0x100
#define VFRAME_EVENT_RECEIVER_DOLBY_BYPASS_EL		0x200
#define VFRAME_EVENT_RECEIVER_NEED_NO_COMP		0x400
#define VFRAME_EVENT_RECEIVER_BUF_COUNT		0x800
#define VFRAME_EVENT_RECEIVER_REQ_STATE		0x1000

enum req_state_type_e {
	REQ_STATE_INVALID = 0,
	REQ_STATE_SECURE = 1,
	REQ_STATE_MAX = 0xff,
};

	/* for VFRAME_EVENT_RECEIVER_GET_AUX_DATA*/
struct provider_aux_req_s {
	/*input*/
	struct vframe_s *vf;
	unsigned char bot_flag;
	/*output*/
	char *aux_buf;
	int aux_size;
	int dv_enhance_exist;
	int low_latency;
	struct tvin_dv_vsif_s dv_vsif;/*dolby vsi info*/
	enum vformat_e format;
};

struct provider_disp_mode_req_s {
	/*input*/
	struct vframe_s *vf;
	unsigned int req_mode;/*0:get;1:check*/
	/*output*/
	enum vframe_disp_mode_e disp_mode;
};

/* for VFRAME_EVENT_RECEIVER_REQ_STATE */
struct provider_state_req_s {
	/*input*/
	struct vframe_s *vf;
	enum req_state_type_e req_type;
	/*output*/
	u32 req_result[4];
};

struct vframe_operations_s {
	struct vframe_s *(*peek)(void *op_arg);
	struct vframe_s *(*get)(void *op_arg);
	void (*put)(struct vframe_s *vf, void *op_arg);
	int (*event_cb)(int type, void *data, void *private_data);
	int (*vf_states)(struct vframe_states *states, void *op_arg);
};

struct vframe_provider_s {
	const char *name;
	const struct vframe_operations_s *ops;
	void *op_arg;
	struct list_head list;
	atomic_t use_cnt;
	void *traceget;
	void *traceput;
} /*vframe_provider_t */;

#define PROVIDER_TABLE_NAME_SIZE 64

struct provider_table_s {
	char name[PROVIDER_TABLE_NAME_SIZE];
	struct vframe_provider_s *vframe_provider;
	bool used;
} /*vframe_provider_table_t */;

struct vframe_provider_s *vf_provider_alloc(void);
void vf_provider_init(struct vframe_provider_s *prov,
		      const char *name,
		      const struct vframe_operations_s *ops,
		      void *op_arg);
void vf_provider_free(struct vframe_provider_s *prov);

int vf_reg_provider(struct vframe_provider_s *prov);
void vf_unreg_provider(struct vframe_provider_s *prov);
int vf_notify_provider(const char *receiver_name, int event_type,
		       void *data);
int vf_notify_provider_by_name(const char *provider_name,
			       int event_type, void *data);

void vf_light_reg_provider(struct vframe_provider_s *prov);
void vf_light_unreg_provider(struct vframe_provider_s *prov);
void vf_ext_light_unreg_provider(struct vframe_provider_s *prov);
struct vframe_provider_s *vf_get_provider(const char *name);
void provide_table_init(void);
struct vframe_s *vf_peek(const char *receiver);
struct vframe_s *vf_get(const char *receiver);
int vf_put(struct vframe_s *vf, const char *receiver);
int vf_get_states(struct vframe_provider_s *vfp, struct vframe_states *states);
int vf_get_states_by_name(const char *receiver_name,
			  struct vframe_states *states);
void dump_all_provider(void (*callback)(const char *name));

unsigned int get_post_canvas(void);

struct vframe_s *get_cur_dispbuf(void);
int query_video_status(int type, int *value);

#ifdef CONFIG_V4L_AMLOGIC_VIDEO
void v4l_reg_provider(struct vframe_provider_s *prov);
void v4l_unreg_provider(void);
const struct vframe_provider_s *v4l_get_vfp(void);
#endif
#endif /* VFRAME_PROVIDER_H */
