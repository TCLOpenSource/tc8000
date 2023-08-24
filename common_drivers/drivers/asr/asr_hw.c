// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/dcache.h>
#include <linux/err.h>
#include <linux/string.h>
#include <asm/fcntl.h>
#include <asm/processor.h>
#include <asm/uaccess.h>
#include <linux/thread_info.h>
#include <linux/vmalloc.h>
#include <linux/amlogic/major.h>
#include <linux/amlogic/vad_api.h>
#include <linux/seq_file.h>
#include <linux/list.h>
#include <linux/highmem.h>
#include <linux/file.h>
#include "asr_debug_data.h"
#include <linux/vmalloc.h>
#include "iflytek/include/ivw_defines.h"
#include "iflytek/include/param_module.h"
#include "iflytek/include/ivw/w_ivw.h"


#define TAG_ASR_MODULE					"aml_asr: "

#define ASR_DUMP_DATA_SIZE				(4 * 1024 * 1024)
#define DEVICE_NAME 					"asr_dump_data"
#define CLASS_NAME						"asr"

static struct class*					g_asr_class;
static struct device*					g_asr_device;
static int								g_asr_major;
static char *							g_asr_dump_data = NULL;
static unsigned int 					g_asr_dump_data_position = 0;
static unsigned int 					g_asr_dump_remain_size = 0;

#define AM_LOGE(fmt, ...)	pr_err(TAG_ASR_MODULE "[%s:%d] " fmt "\n", __func__,__LINE__, ##__VA_ARGS__)
#define AM_LOGI(fmt, ...)	pr_info(TAG_ASR_MODULE "[%s:%d] " fmt "\n", __func__,__LINE__, ##__VA_ARGS__)

#define PERIOD_FRAMES				(IVW_FIXED_WRITE_BUF_LEN / 2)
#define ASR_FRAME_SIZE				(2)
#define ASR_VAD_DEBUG_TIMEOUT_CNT	(200)


#define ASR_IOC_TYPE							'C'
#define ASR_IOC_TYPE_GET_ASR_DATA_SIZE					0
#define ASR_IOC_TYPE_RESET_ASR_DATA 					1

#define ASR_IOC_TYPE_CMD_GET_ASR_DATA_SIZE				\
		_IOR(ASR_IOC_TYPE, ASR_IOC_TYPE_GET_ASR_DATA_SIZE, unsigned int)
#define ASR_IOC_TYPE_CMD_RESET_ASR_DATA 			 \
		_IO(ASR_IOC_TYPE, ASR_IOC_TYPE_RESET_ASR_DATA)

static bool g_need_wakeup;
static WIVW_INST g_ivw_inst;
static ivChar *g_callback_param = "ivw engine from iflytek";
u16 g_temp_buf16[PERIOD_FRAMES];
u32 g_remain_frames;
static int g_timeout_reboot_cnt = ASR_VAD_DEBUG_TIMEOUT_CNT;
static int g_audio_vad_debug_mode;

enum E_AUDIO_VAD_DEBUG_MODE {
	E_AUDIO_VAD_DEBUG_MODE_NONE		= 0,
	E_AUDIO_VAD_DEBUG_MODE_CLEAR_DATA		= 1,
	E_AUDIO_VAD_DEBUG_MODE_TIMEOUT			= 2,
	E_AUDIO_VAD_DEBUG_MODE_ENALBE_LOG		= 3,
};

ivInt IvwCallBackWakeup(const ivChar *param, void *p_user_param)
{
	const char *p_user_info = (const char *)p_user_param;

	AM_LOGE("ivw param=%s user_param=%s", param, p_user_info);
	g_need_wakeup = true;
	return 0;
}

ivInt IvwCallBackWarmup(const ivChar *param, void *p_user_param)
{
	const char *p_user_info = (const char *)p_user_param;

	AM_LOGE("ivw param=%s user_param=%s", param, p_user_info);
	return 0;
}

ivInt IvwCallBackPreWakeup(const ivChar *param, void *p_user_param)
{
	const char *p_user_info = (const char *)p_user_param;

	AM_LOGE("ivw param=%s user_param=%s", param, p_user_info);
	return 0;
}

int iflytek_asr_enable(void)
{
	int inst_size = 0;
	int value = 0;
	ivInt ret;

	g_asr_dump_data_position = 0;
	g_asr_dump_remain_size = 0;

	g_timeout_reboot_cnt = ASR_VAD_DEBUG_TIMEOUT_CNT;
	if (g_ivw_inst) {
		AM_LOGE("wakeup_init has been inited, return");
		return -1;
	}

	inst_size = wIvwGetInstSize();
	AM_LOGE("version:%s, inst_size:%d, debug_mode:%d", wIvwGetVersion(),
		inst_size, g_audio_vad_debug_mode);
	g_ivw_inst = (WIVW_INST)vmalloc(inst_size);
	if (!g_ivw_inst)
		return -1;

	ret = wIvwCreate(g_ivw_inst, NULL, NULL);
	//ret = wIvwCreate(&g_ivw_inst, mlp_res, filler_res);
	ret = wIvwRegisterCallBacks(g_ivw_inst, CallBackFuncNameWakeUp,
			IvwCallBackWakeup, g_callback_param);
	ret = wIvwRegisterCallBacks(g_ivw_inst, CallBackFuncNameWarmUp,
			IvwCallBackWarmup, g_callback_param);
	//ret = wIvwRegisterCallBacks(g_ivw_inst, CallBackFuncNamePreWakeup,
	//IvwCallBackPreWakeup, g_callback_param);
	//ret = wIvwStart(g_ivw_inst);

	wIvwGetParameter(g_ivw_inst, PARAM_W_IVW_VADON, &value);
	pr_debug("PARAM_W_IVW_VADON: %d\n", value);
	wIvwGetParameter(g_ivw_inst, PARAM_W_FEA_MLP_JUMPFREAM_NUM, &value);
	pr_debug("PARAM_W_FEA_MLP_JUMPFREAM_NUM: %d\n", value);
	wIvwGetParameter(g_ivw_inst, PARAM_W_FEA_DO_CMN, &value);
	pr_debug("PARAM_W_FEA_DO_CMN: %d\n", value);
	wIvwGetParameter(g_ivw_inst, PARAM_W_DEC_NCMTHRESHOLD_KEYWORD1_ARC1, &value);
	pr_debug("PARAM_W_DEC_NCMTHRESHOLD_KEYWORD1_ARC1: %d\n", value);
	wIvwGetParameter(g_ivw_inst, PARAM_W_DEC_NCMTHRESHOLD_KEYWORD2_ARC1, &value);
	pr_debug("PARAM_W_DEC_NCMTHRESHOLD_KEYWORD2_ARC1: %d\n", value);
	wIvwGetParameter(g_ivw_inst, PARAM_W_DEC_NLMPENALTY, &value);
	pr_debug("PARAM_W_DEC_NLMPENALTY: %d\n", value);

	value = 0;
	wIvwSetParameter(g_ivw_inst, PARAM_W_IVW_VADON, value);
	wIvwGetParameter(g_ivw_inst, PARAM_W_IVW_VADON, &value);
	pr_debug("PARAM_W_IVW_VADON: %d\n", value);

	value = 1;
	wIvwSetParameter(g_ivw_inst, PARAM_W_FEA_MLP_JUMPFREAM_NUM, value);
	wIvwGetParameter(g_ivw_inst, PARAM_W_FEA_MLP_JUMPFREAM_NUM, &value);
	pr_debug("PARAM_W_FEA_MLP_JUMPFREAM_NUM: %d\n", value);

	wIvwGetParameter(g_ivw_inst, PARAM_W_DEC_MAX_STATE_LOOP, &value);
	pr_debug("default_PARAM_W_DEC_MAX_STATE_LOOP: %d\n", value);
	value = 50;
	wIvwSetParameter(g_ivw_inst, PARAM_W_DEC_MAX_STATE_LOOP, value);
	wIvwGetParameter(g_ivw_inst, PARAM_W_DEC_MAX_STATE_LOOP, &value);
	pr_debug("PARAM_W_DEC_MAX_STATE_LOOP: %d\n", value);

	wIvwGetParameter(g_ivw_inst, PARAM_W_DEC_NCMTHRESHOLD_KEYWORD1_ARC1, &value);
	pr_debug("PARAM_W_DEC_NCMTHRESHOLD_KEYWORD1_ARC1: %d\n", value);

	wIvwGetParameter(g_ivw_inst, PARAM_W_DEC_NCMTHRESHOLD_PREWAKE_KEYWORD1_ARC1, &value);
	pr_debug("PARAM_W_DEC_NCMTHRESHOLD_PREWAKE_KEYWORD1_ARC1: %d\n", value);

	wIvwGetParameter(g_ivw_inst, PARAM_W_DEC_NCMTHRESHOLD_KEYWORD2_ARC1, &value);
	pr_debug("PARAM_W_DEC_NCMTHRESHOLD_KEYWORD2_ARC1: %d\n", value);

	wIvwGetParameter(g_ivw_inst, PARAM_W_DEC_NCMTHRESHOLD_PREWAKE_KEYWORD2_ARC1, &value);
	pr_debug("PARAM_W_DEC_NCMTHRESHOLD_PREWAKE_KEYWORD2_ARC1: %d\n", value);

	wIvwGetParameter(g_ivw_inst, PARAM_W_DEC_NLMPENALTY, &value);
	pr_debug("PARAM_W_DEC_NLMPENALTY: %d\n", value);
	ret = wIvwStart(g_ivw_inst);
	pr_info("aml_asr:%s wakeup_init\n", __func__);
	return ret;
}

int iflytek_asr_disable(void)
{
	ivInt ret = 0;

	g_timeout_reboot_cnt = ASR_VAD_DEBUG_TIMEOUT_CNT;

	AM_LOGE("iflytek_asr_disable, enter");
	if (!g_ivw_inst) {
		AM_LOGE("wakeup_init has been deinited, return");
		return -1;
	}
	AM_LOGE("iflytek_asr_disable, call wIvwStop");
	ret = wIvwStop(g_ivw_inst);
	if (ret != 0)
		AM_LOGE("wIvwStop ret:%d", ret);
	AM_LOGE("iflytek_asr_disable, call wIvwDestroy");
	ret = wIvwDestroy(g_ivw_inst);
	if (ret != 0)
		AM_LOGE("wIvwDestroy ret:%d\n", ret);

	AM_LOGE("iflytek_asr_disable, call vfree");
	vfree(g_ivw_inst);
	g_ivw_inst = NULL;
	g_need_wakeup = false;

	AM_LOGE("iflytek_asr_disable, exit");
	return ret;
}

int aml_asr_dump_data(char *buf, u32 bytes)
{
	static char *write_ptr = 0;
	if (g_asr_dump_data) {
		AM_LOGE("hello root enter, bytes:%d", bytes);
		write_ptr = g_asr_dump_data + g_asr_dump_data_position;
		if (ASR_DUMP_DATA_SIZE - g_asr_dump_data_position < bytes) {
			memmove(g_asr_dump_data, g_asr_dump_data + g_asr_dump_data_position, bytes);
			g_asr_dump_data_position = g_asr_dump_data_position - bytes;
			write_ptr = g_asr_dump_data + g_asr_dump_data_position;
			AM_LOGE("write override,  write_ptr:%d", write_ptr);
		}
		AM_LOGE("memory g_asr_dump_data_position:%d. bytes:%d", g_asr_dump_data_position, bytes);
		memcpy(write_ptr, buf, bytes);
		g_asr_dump_data_position += bytes;
		g_asr_dump_remain_size = g_asr_dump_data_position;
	}
	return 0;
}

/* only supports 1 channels, 16bit, 16Khz.
 * And, the wIvwWrite function only supports 160 frames(10ms) per write.
 */
int iflytek_asr_feed(char *buf, u32 frames)
{
	u16 *p_temp_buf16 = NULL;
	int i, ret = -1;
	u32 frames_total = frames + g_remain_frames;

	if (g_audio_vad_debug_mode != E_AUDIO_VAD_DEBUG_MODE_NONE)
		AM_LOGE("frames:%d debug_mode:%d", frames, g_audio_vad_debug_mode);

	if (g_audio_vad_debug_mode == E_AUDIO_VAD_DEBUG_MODE_CLEAR_DATA)
		fill_in_clear_data(buf, frames);
	aml_asr_dump_data(buf, frames * 2);

	p_temp_buf16 = (u16 *)buf;
	if (!g_ivw_inst) {
		AM_LOGE("asr is not init!");
		return 0;
	}

	for (i = 0; i < frames_total / PERIOD_FRAMES; i++) {
		if (g_remain_frames) {
			int extra_frames = PERIOD_FRAMES - g_remain_frames;

			memcpy(g_temp_buf16 + g_remain_frames, p_temp_buf16,
				extra_frames * ASR_FRAME_SIZE);
			p_temp_buf16 += extra_frames;
			ret = wIvwWrite(g_ivw_inst, g_temp_buf16, IVW_FIXED_WRITE_BUF_LEN);
			if (ret != 0)
				AM_LOGE("wIvwWrite fail ret:%d", ret);
			g_remain_frames = 0;
		} else {
			memcpy(g_temp_buf16, p_temp_buf16, IVW_FIXED_WRITE_BUF_LEN);
			p_temp_buf16 += PERIOD_FRAMES;
			ret = wIvwWrite(g_ivw_inst, g_temp_buf16, IVW_FIXED_WRITE_BUF_LEN);
			if (ret != 0)
				AM_LOGE("wIvwWrite fail ret:%d", ret);
		}
		if (g_audio_vad_debug_mode == E_AUDIO_VAD_DEBUG_MODE_TIMEOUT) {
			if (g_timeout_reboot_cnt < 0) {
				AM_LOGE("==============timeout_reboot=================");
				g_need_wakeup = true;
				g_timeout_reboot_cnt = ASR_VAD_DEBUG_TIMEOUT_CNT;
			} else {
				AM_LOGE("timeout_reboot_cnt:%d", g_timeout_reboot_cnt);
			}
		}
		if (g_need_wakeup) {
			TWakeUpResult wakeup_rst;

			wIvwGetLastWakeupRst(g_ivw_inst, &wakeup_rst);
			g_need_wakeup = false;
			AM_LOGE("VAD wakeup now ^_^!");
			AM_LOGE("+++++++++++++++++++++++++");
			return 1;
		}
	}
	if (g_audio_vad_debug_mode == E_AUDIO_VAD_DEBUG_MODE_TIMEOUT)
		g_timeout_reboot_cnt--;

	g_remain_frames = frames_total % PERIOD_FRAMES;
	if (g_remain_frames)
		memcpy(g_temp_buf16, p_temp_buf16, g_remain_frames * ASR_FRAME_SIZE);

	return 0;
}

// uboot cmd. eg: "setenv initargs $initargs audio_vad_debug_mode=cleardata"
static int __init set_debug_mode(char *str)
{
	char debug_mode_string[20] = {};

	if (str)
		snprintf(debug_mode_string, 10, "%s", str);

	if (strncmp(debug_mode_string, "cleardata", 9) == 0)
		g_audio_vad_debug_mode = E_AUDIO_VAD_DEBUG_MODE_CLEAR_DATA;
	else if (strncmp(debug_mode_string, "timeout", 7) == 0)
		g_audio_vad_debug_mode = E_AUDIO_VAD_DEBUG_MODE_TIMEOUT;
	else if (strncmp(debug_mode_string, "debug", 5) == 0)
		g_audio_vad_debug_mode = E_AUDIO_VAD_DEBUG_MODE_ENALBE_LOG;
	else
		g_audio_vad_debug_mode = E_AUDIO_VAD_DEBUG_MODE_NONE;

	return 0;
}
__setup("audio_vad_debug_mode=", set_debug_mode);

int aml_asr_enable(void)
{
	return iflytek_asr_enable();
}
EXPORT_SYMBOL_GPL(aml_asr_enable);

int aml_asr_disable(void)
{
	return iflytek_asr_disable();
}
EXPORT_SYMBOL_GPL(aml_asr_disable);

int aml_asr_feed(char *buf, u32 frames)
{
	return iflytek_asr_feed(buf, frames);
}
EXPORT_SYMBOL_GPL(aml_asr_feed);

static int asr_release(struct inode *inodep, struct file *filep)
{
	AM_LOGE("");
	return 0;
}

static int asr_open(struct inode *inodep, struct file *filep)
{
	AM_LOGE("");
	return 0;
}

static ssize_t asr_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	int ret = 0;
	AM_LOGE("avail:%d, pos:%d", g_asr_dump_remain_size, g_asr_dump_data_position, len);

	if (len > ASR_DUMP_DATA_SIZE) {
		AM_LOGE("read overflow.");
		ret = -EFAULT;
		goto out;
	}
	if (g_asr_dump_remain_size < len) {
		AM_LOGE("buffer is not enough, remain:%d", g_asr_dump_remain_size);
		ret = -EFAULT;
		goto out;
	}

	if (g_asr_dump_data == NULL) {
		AM_LOGE("not in debug mode, no data.");
		ret = -EFAULT;
		goto out;
	}

	if (copy_to_user(buffer, g_asr_dump_data + (g_asr_dump_data_position - g_asr_dump_remain_size), len) == 0) {
		g_asr_dump_remain_size -= len;
		ret = len;
	} else {
		AM_LOGE("copy_to_user fail.");
		ret = -EFAULT;
	}

out:
	return ret;
}

static long asr_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	void __user *argp;
	int mcd_nr = -1;

	mcd_nr = _IOC_NR(cmd);
	AM_LOGE("_IOC_DIR cmd: %#x, cmd_nr: %#x", _IOC_DIR(cmd), mcd_nr);

	argp = (void __user *)arg;
	switch (mcd_nr) {
	case ASR_IOC_TYPE_GET_ASR_DATA_SIZE:
		if (copy_to_user(argp, &g_asr_dump_remain_size, sizeof(unsigned int))) {
			AM_LOGE("GET_ASR_DATA_SIZE error.");
			ret = -EFAULT;
		} else {
			AM_LOGE("get available dump data size: %d", g_asr_dump_remain_size);
		}
		break;
	case ASR_IOC_TYPE_RESET_ASR_DATA:
		g_asr_dump_remain_size = g_asr_dump_data_position;
		AM_LOGE("RESET_ASR_DATA bytes:%d", g_asr_dump_remain_size);
		break;

	default:
		AM_LOGE("not support_IOC_DIR cmd: %#x, cmd_nr: %#x", _IOC_DIR(cmd), mcd_nr);\
		ret = -EINVAL;
		break;
	}

	return ret;
}
#ifdef CONFIG_COMPAT
static long asr_ioctl_compact_ioctl(struct file *f,
				   unsigned int cmd, unsigned long arg)
{
	arg = (unsigned long)compat_ptr(arg);
	return asr_ioctl(f, cmd, arg);
}
#endif

static const struct file_operations mchar_fops = {
	.open = asr_open,
	.read = asr_read,
	.release = asr_release,
	.unlocked_ioctl = asr_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl	= asr_ioctl_compact_ioctl,
#endif
	.owner = THIS_MODULE,
};

static int __init aml_asr_module_init(void)
{
	int ret = 0;

	AM_LOGE("enter, debug mode:%d", g_audio_vad_debug_mode);
	g_ivw_inst = NULL;
	g_need_wakeup = false;
	g_remain_frames = 0;

	if (g_audio_vad_debug_mode != E_AUDIO_VAD_DEBUG_MODE_ENALBE_LOG) {
		goto out;
	}
	g_asr_major = register_chrdev(0, DEVICE_NAME, &mchar_fops);
	if (g_asr_major < 0) {
		AM_LOGE("fail to register g_asr_major number!");
		ret = g_asr_major;
		goto out;
	}

	g_asr_class = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(g_asr_class)){
		unregister_chrdev(g_asr_major, DEVICE_NAME);
		ret = PTR_ERR(g_asr_class);
		AM_LOGE("class_create error: %s", ret);
		goto out;
	}

	g_asr_device = device_create(g_asr_class, NULL, MKDEV(g_asr_major, 0), NULL, DEVICE_NAME);
	if (IS_ERR(g_asr_device)) {
		class_destroy(g_asr_class);
		unregister_chrdev(g_asr_major, DEVICE_NAME);
		ret = PTR_ERR(g_asr_device);
		AM_LOGE("device_create error: %s", ret);
		goto out;
	}

	g_asr_dump_data = (char *)vmalloc(ASR_DUMP_DATA_SIZE);
	if (g_asr_dump_data == NULL) {
		ret = -ENOMEM;
		AM_LOGE("kmalloc fault!");
		goto out;
	}
out:
	return ret;
}

static void __exit aml_asr_module_exit(void)
{
	AM_LOGE("");
	if (g_audio_vad_debug_mode != E_AUDIO_VAD_DEBUG_MODE_ENALBE_LOG) {
		return;
	}
	device_destroy(g_asr_class, MKDEV(g_asr_major, 0));
	class_unregister(g_asr_class);
	class_destroy(g_asr_class);
	unregister_chrdev(g_asr_major, DEVICE_NAME);
	if (g_asr_dump_data) {
		vfree(g_asr_dump_data);
	}
}

module_init(aml_asr_module_init);
module_exit(aml_asr_module_exit);

MODULE_AUTHOR("AMLOGIC");
MODULE_DESCRIPTION("AMLOGIC ASR");
MODULE_LICENSE("GPL v2");

