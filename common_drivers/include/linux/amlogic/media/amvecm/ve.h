/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * include/linux/amlogic/media/amvecm/ve.h
 *
 * Copyright (C) 2017 Amlogic, Inc. All rights reserved.
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

#ifndef __VE_H
#define __VE_H

/* ******************************************************************* */
/* *** enum definitions ********************************************* */
/* ******************************************************************* */

enum ve_demo_pos_e {
	VE_DEMO_POS_TOP = 0,
	VE_DEMO_POS_BOTTOM,
	VE_DEMO_POS_LEFT,
	VE_DEMO_POS_RIGHT,
};

enum ve_dnlp_rt_e {
	VE_DNLP_RT_0S = 0,
	VE_DNLP_RT_1S = 6,
	VE_DNLP_RT_2S,
	VE_DNLP_RT_4S,
	VE_DNLP_RT_8S,
	VE_DNLP_RT_16S,
	VE_DNLP_RT_32S,
	VE_DNLP_RT_64S,
	VE_DNLP_RT_FREEZE,
};

/* ******************************************************************* */
/* *** struct definitions ********************************************* */
/* ******************************************************************* */

struct ve_bext_s {
	unsigned char en;
	unsigned char start;
	unsigned char slope1;
	unsigned char midpt;
	unsigned char slope2;
};

#if defined(CONFIG_AMLOGIC_MEDIA_ENHANCEMENT_VECM)
#define DNLP_SCURV_LEN 65
#define GAIN_VAR_LUT_LEN 49
#define WEXT_GAIN_LEN 48
#define ADP_THRD_LEN 33
#define REG_BLK_BOOST_LEN 13
#define REG_ADP_OFSET_LEN 20
#define REG_MONO_PROT_LEN 6
#define TREND_WHT_EXP_LUT_LEN 9
#define C_HIST_GAIN_LEN 65
#define S_HIST_GAIN_LEN 65
#define DNLP_PARM_MAX_NUM 100
#define DNLP_VPP_HIST_BIN_NUM 64
#define HDR_HIST_BIN_NUM 128
#define HUE_HIST_BIN_NUM 32
#define SAT_HIST_BIN_NUM 32
struct ve_dnlp_s {
	unsigned int      en;
	unsigned int rt;    /* 0 ~ 255, */
	unsigned int rl;    /* 0 ~  15, 1.0000x ~ 1.9375x, step 0.0625x */
	unsigned int black; /* 0 ~  16, weak ~ strong */
	unsigned int white; /* 0 ~  16, weak ~ strong */
};

struct ve_hist_s {
	unsigned int sum;
	int width;
	int height;
	int ave;
};

struct vpp_hist_param_s {
	unsigned int vpp_hist_pow;
	unsigned int vpp_luma_sum;
	unsigned int vpp_pixel_sum;
	unsigned short vpp_histgram[DNLP_VPP_HIST_BIN_NUM];
	unsigned short vpp_dark_hist[DNLP_VPP_HIST_BIN_NUM];
	unsigned int hdr_histgram[HDR_HIST_BIN_NUM];
	unsigned int hue_histgram[HUE_HIST_BIN_NUM];
	unsigned int sat_histgram[SAT_HIST_BIN_NUM];
};

struct ve_dnlp_curve_param_s {
	unsigned int ve_dnlp_scurv_low[DNLP_SCURV_LEN];
	unsigned int ve_dnlp_scurv_mid1[DNLP_SCURV_LEN];
	unsigned int ve_dnlp_scurv_mid2[DNLP_SCURV_LEN];
	unsigned int ve_dnlp_scurv_hgh1[DNLP_SCURV_LEN];
	unsigned int ve_dnlp_scurv_hgh2[DNLP_SCURV_LEN];
	unsigned int ve_gain_var_lut49[GAIN_VAR_LUT_LEN];
	unsigned int ve_wext_gain[WEXT_GAIN_LEN];
	unsigned int ve_adp_thrd[ADP_THRD_LEN];
	unsigned int ve_reg_blk_boost_12[REG_BLK_BOOST_LEN];
	unsigned int ve_reg_adp_ofset_20[REG_ADP_OFSET_LEN];
	unsigned int ve_reg_mono_protect[REG_MONO_PROT_LEN];
	unsigned int ve_reg_trend_wht_expand_lut8[TREND_WHT_EXP_LUT_LEN];
	unsigned int ve_c_hist_gain[C_HIST_GAIN_LEN];
	unsigned int ve_s_hist_gain[S_HIST_GAIN_LEN];
	unsigned int param[DNLP_PARM_MAX_NUM];
};

enum dnlp_param_e {
	ve_dnlp_enable = 0,
	ve_dnlp_respond,
	ve_dnlp_sel,
	ve_dnlp_respond_flag,
	ve_dnlp_smhist_ck,
	ve_dnlp_mvreflsh,
	ve_dnlp_pavg_btsft,
	ve_dnlp_dbg_i2r,
	ve_dnlp_cuvbld_min,
	ve_dnlp_cuvbld_max,
	ve_dnlp_schg_sft,
	ve_dnlp_bbd_ratio_low,
	ve_dnlp_bbd_ratio_hig,
	ve_dnlp_limit_rng,
	ve_dnlp_range_det,
	ve_dnlp_blk_cctr,
	ve_dnlp_brgt_ctrl,
	ve_dnlp_brgt_range,
	ve_dnlp_brght_add,
	ve_dnlp_brght_max,
	ve_dnlp_dbg_adjavg,
	ve_dnlp_auto_rng,
	ve_dnlp_lowrange,
	ve_dnlp_hghrange,
	ve_dnlp_satur_rat,
	ve_dnlp_satur_max,
	ve_dnlp_set_saturtn,
	ve_dnlp_sbgnbnd,
	ve_dnlp_sendbnd,
	ve_dnlp_clashbgn,
	ve_dnlp_clashend,
	ve_dnlp_var_th,
	ve_dnlp_clahe_gain_neg,
	ve_dnlp_clahe_gain_pos,
	ve_dnlp_clahe_gain_delta,
	ve_dnlp_mtdbld_rate,
	ve_dnlp_adpmtd_lbnd,
	ve_dnlp_adpmtd_hbnd,
	ve_dnlp_blkext_ofst,
	ve_dnlp_whtext_ofst,
	ve_dnlp_blkext_rate,
	ve_dnlp_whtext_rate,
	ve_dnlp_bwext_div4x_min,
	ve_dnlp_irgnbgn,
	ve_dnlp_irgnend,
	ve_dnlp_dbg_map,
	ve_dnlp_final_gain,
	ve_dnlp_cliprate_v3,
	ve_dnlp_cliprate_min,
	ve_dnlp_adpcrat_lbnd,
	ve_dnlp_adpcrat_hbnd,
	ve_dnlp_scurv_low_th,
	ve_dnlp_scurv_mid1_th,
	ve_dnlp_scurv_mid2_th,
	ve_dnlp_scurv_hgh1_th,
	ve_dnlp_scurv_hgh2_th,
	ve_dnlp_mtdrate_adp_en,
	ve_dnlp_clahe_method,
	ve_dnlp_ble_en,
	ve_dnlp_norm,
	ve_dnlp_scn_chg_th,
	ve_dnlp_step_th,
	ve_dnlp_iir_step_mux,
	ve_dnlp_single_bin_bw,
	ve_dnlp_single_bin_method,
	ve_dnlp_reg_max_slop_1st,
	ve_dnlp_reg_max_slop_mid,
	ve_dnlp_reg_max_slop_fin,
	ve_dnlp_reg_min_slop_1st,
	ve_dnlp_reg_min_slop_mid,
	ve_dnlp_reg_min_slop_fin,
	ve_dnlp_reg_trend_wht_expand_mode,
	ve_dnlp_reg_trend_blk_expand_mode,
	ve_dnlp_ve_hist_cur_gain,
	ve_dnlp_ve_hist_cur_gain_precise,
	ve_dnlp_reg_mono_binrang_st,
	ve_dnlp_reg_mono_binrang_ed,
	ve_dnlp_c_hist_gain_base,
	ve_dnlp_s_hist_gain_base,
	ve_dnlp_mvreflsh_offset,
	ve_dnlp_luma_avg_th,
	ve_dnlp_param_max,
};

enum dnlp_curve_e {
	ve_scurv_low = 1000,
	ve_scurv_mid1,
	ve_scurv_mid2,
	ve_scurv_hgh1,
	ve_scurv_hgh2,
	ve_curv_var_lut49,
	ve_curv_wext_gain,
	ve_adp_thrd = 1013,
	ve_reg_blk_boost_12,
	ve_reg_adp_ofset_20,
	ve_reg_mono_protect,
	ve_reg_trend_wht_expand_lut8,
	ve_c_hist_gain = 1019,
	ve_s_hist_gain,
};
#else
struct ve_dnlp_s {
	unsigned char en;
	enum  ve_dnlp_rt_e rt;
	unsigned char gamma[64];
};
#endif

struct ve_lc_curve_parm_s {
	unsigned int ve_lc_saturation[63];
	unsigned int ve_lc_yminval_lmt[16];
	unsigned int ve_lc_ypkbv_ymaxval_lmt[16];
	unsigned int ve_lc_ymaxval_lmt[16];
	unsigned int ve_lc_ypkbv_lmt[16];
	unsigned int ve_lc_ypkbv_ratio[4];
	unsigned int param[100];
};

enum lc_alg_param_e {
	lc_dbg_parm0 = 0,
	lc_dbg_parm1,
	lc_dbg_parm2,
	lc_dbg_parm3,
	lc_dbg_parm4,
	lc_dbg_parm_max,
};

struct ve_hsvs_s {
	unsigned char en;
	unsigned char peak_gain_h1;
	unsigned char peak_gain_h2;
	unsigned char peak_gain_h3;
	unsigned char peak_gain_h4;
	unsigned char peak_gain_h5;
	unsigned char peak_gain_v1;
	unsigned char peak_gain_v2;
	unsigned char peak_gain_v3;
	unsigned char peak_gain_v4;
	unsigned char peak_gain_v5;
	unsigned char peak_gain_v6;
	unsigned char hpeak_slope1;
	unsigned char hpeak_slope2;
	unsigned char hpeak_thr1;
	unsigned char hpeak_thr2;
	unsigned char hpeak_nlp_cor_thr;
	unsigned char hpeak_nlp_gain_pos;
	unsigned char hpeak_nlp_gain_neg;
	unsigned char vpeak_slope1;
	unsigned char vpeak_slope2;
	unsigned char vpeak_thr1;
	unsigned char vpeak_thr2;
	unsigned char vpeak_nlp_cor_thr;
	unsigned char vpeak_nlp_gain_pos;
	unsigned char vpeak_nlp_gain_neg;
	unsigned char speak_slope1;
	unsigned char speak_slope2;
	unsigned char speak_thr1;
	unsigned char speak_thr2;
	unsigned char speak_nlp_cor_thr;
	unsigned char speak_nlp_gain_pos;
	unsigned char speak_nlp_gain_neg;
	unsigned char peak_cor_gain;
	unsigned char peak_cor_thr_l;
	unsigned char peak_cor_thr_h;
	unsigned char vlti_step;
	unsigned char vlti_step2;
	unsigned char vlti_thr;
	unsigned char vlti_gain_pos;
	unsigned char vlti_gain_neg;
	unsigned char vlti_blend_factor;
	unsigned char hlti_step;
	unsigned char hlti_thr;
	unsigned char hlti_gain_pos;
	unsigned char hlti_gain_neg;
	unsigned char hlti_blend_factor;
	unsigned char vlimit_coef_h;
	unsigned char vlimit_coef_l;
	unsigned char hlimit_coef_h;
	unsigned char hlimit_coef_l;
	unsigned char cti_444_422_en;
	unsigned char cti_422_444_en;
	unsigned char cti_blend_factor;
	unsigned char vcti_buf_en;
	unsigned char vcti_buf_mode_c5l;
	unsigned char vcti_filter;
	unsigned char hcti_step;
	unsigned char hcti_step2;
	unsigned char hcti_thr;
	unsigned char hcti_gain;
	unsigned char hcti_mode_median;
};

struct ve_ccor_s {
	unsigned char en;
	unsigned char slope;
	unsigned char thr;
};

struct ve_benh_s {
	unsigned char en;
	unsigned char cb_inc;
	unsigned char cr_inc;
	unsigned char gain_cr;
	unsigned char gain_cb4cr;
	unsigned char luma_h;
	unsigned char err_crp;
	unsigned char err_crn;
	unsigned char err_cbp;
	unsigned char err_cbn;
};

struct ve_cbar_s {
	unsigned char en;
	unsigned char wid;
	unsigned char cr;
	unsigned char cb;
	unsigned char y;
};

struct ve_demo_s {
	unsigned char bext;
	unsigned char dnlp;
	unsigned char hsvs;
	unsigned char ccor;
	unsigned char benh;
	enum  ve_demo_pos_e  pos;
	unsigned long wid;
	struct ve_cbar_s   cbar;
};

struct vdo_meas_s {
	/* ... */
};

struct ve_regmap_s {
	unsigned long reg[43];
};

#define EOTF_LUT_SIZE 33
#define OSD_OETF_LUT_SIZE 41

/********************OSD HDR registers backup********************************/
struct hdr_osd_lut_s {
	u32 r_map[33];
	u32 g_map[33];
	u32 b_map[33];
	u32 or_map[41];
	u32 og_map[41];
	u32 ob_map[41];
};

struct hdr_osd_reg_s {
	u32 viu_osd1_matrix_ctrl; /* 0x1a90 */
	u32 viu_osd1_matrix_coef00_01; /* 0x1a91 */
	u32 viu_osd1_matrix_coef02_10; /* 0x1a92 */
	u32 viu_osd1_matrix_coef11_12; /* 0x1a93 */
	u32 viu_osd1_matrix_coef20_21; /* 0x1a94 */
	u32 viu_osd1_matrix_colmod_coef42; /* 0x1a95 */
	u32 viu_osd1_matrix_offset0_1; /* 0x1a96 */
	u32 viu_osd1_matrix_offset2; /* 0x1a97 */
	u32 viu_osd1_matrix_pre_offset0_1; /* 0x1a98 */
	u32 viu_osd1_matrix_pre_offset2; /* 0x1a99 */
	u32 viu_osd1_matrix_coef22_30; /* 0x1a9d */
	u32 viu_osd1_matrix_coef31_32; /* 0x1a9e */
	u32 viu_osd1_matrix_coef40_41; /* 0x1a9f */
	u32 viu_osd1_eotf_ctl; /* 0x1ad4 */
	u32 viu_osd1_eotf_coef00_01; /* 0x1ad5 */
	u32 viu_osd1_eotf_coef02_10; /* 0x1ad6 */
	u32 viu_osd1_eotf_coef11_12; /* 0x1ad7 */
	u32 viu_osd1_eotf_coef20_21; /* 0x1ad8 */
	u32 viu_osd1_eotf_coef22_rs; /* 0x1ad9 */
	u32 VIU_OSD1_EOTF_3X3_OFST_0; /* 0x1aa0*/
	u32 VIU_OSD1_EOTF_3X3_OFST_1; /* 0x1aa1*/
	u32 viu_osd1_oetf_ctl; /* 0x1adc */
	struct hdr_osd_lut_s lut_val;
	/* -1: invalid, 0: not shadow, >1: delay count */
	s32 shadow_mode;
};

struct db_cabc_aad_param_s {
	unsigned int length;
	union {
		void *cabc_aad_param_ptr;
		long long cabc_aad_param_ptr_len;
	};
};

struct db_aad_param_s {
	int aad_param_cabc_aad_en;
	int aad_param_aad_en;
	int aad_param_tf_en;
	int aad_param_force_gain_en;
	int aad_param_sensor_mode;
	int aad_param_mode;
	int aad_param_dist_mode;
	int aad_param_tf_alpha;
	int aad_param_sensor_input[3];
	struct db_cabc_aad_param_s	 db_LUT_Y_gain;
	struct db_cabc_aad_param_s	 db_LUT_RG_gain;
	struct db_cabc_aad_param_s	 db_LUT_BG_gain;
	struct db_cabc_aad_param_s	 db_gain_lut;
	struct db_cabc_aad_param_s	 db_xy_lut;
};

struct db_cabc_param_s {
	int cabc_param_cabc_en;
	int cabc_param_hist_mode;
	int cabc_param_tf_en;
	int cabc_param_sc_flag;
	int cabc_param_bl_map_mode;
	int cabc_param_bl_map_en;
	int cabc_param_temp_proc;
	int cabc_param_max95_ratio;
	int cabc_param_hist_blend_alpha;
	int cabc_param_init_bl_min;
	int cabc_param_init_bl_max;
	int cabc_param_tf_alpha;
	int cabc_param_sc_hist_diff_thd;
	int cabc_param_sc_apl_diff_thd;
	int cabc_param_patch_bl_th;
	int cabc_param_patch_on_alpha;
	int cabc_param_patch_bl_off_th;
	int cabc_param_patch_off_alpha;
	struct db_cabc_aad_param_s db_o_bl_cv;
	struct db_cabc_aad_param_s db_maxbin_bl_cv;
};

extern struct hdr_osd_reg_s hdr_osd_reg;
/***********************OSD HDR registers*******************************/

/* ******************************************************************* */
/* *** MACRO definitions ********** */
/* ******************************************************************* */

/* ******************************************************************* */
/* *** FUNCTION definitions ********** */
/* ******************************************************************* */

#endif  /* _VE_H */
