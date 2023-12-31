// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

// drivers/amlogic/media/enhancement/amvecm/am_lut3d.c

/* #include <mach/am_regs.h> */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/amlogic/media/amvecm/amvecm.h>
#include <linux/uaccess.h>
#include "arch/vpp_regs.h"
#include "arch/cm_regs.h"
#include "arch/ve_regs.h"
#include <linux/amlogic/media/amvecm/color_tune.h>
#include "blue_stretch/blue_str.h"
#include "amve.h"

extern int (*plut)[3];
extern unsigned int (*plut_out)[3];

void bs_ct_tbl(void)
{
	int i, j;

	if (bs_3dlut_en) {
		//memcpy(&plut[0][0], plut3d, 14739 * sizeof(int));
		for (i = 0; i < 4913; i++)
			for (j = 0; j < 3; j++)
				plut[i][j] = plut3d[i * 3 + j];
	}
}

/* color tune and blue stretch set */
void lut3d_set_api(void)
{
	struct ct_func_s *ct_f = get_ct_func();

	if (!ct_f->cl_par->en || !ct_f->ct) {
		pr_info("%s: ct_en = %d, ct = %p\n", __func__, ct_f->cl_par->en, ct_f->ct);
		lut3d_update(0);
		return;
	}

	if ((vecm_latch_flag2 & LUT3D_UPDATE) && !(vecm_latch_flag2 & CT_UPDATE)) {
		vecm_latch_flag2 &= ~LUT3D_UPDATE;
		lut3d_update(plut_out);
	}

	if (vecm_latch_flag2 & CT_UPDATE) {
		bls_set();
		bs_ct_tbl();
		ct_process();
		vecm_latch_flag2 &= ~CT_UPDATE;
		vecm_latch_flag2 |= LUT3D_UPDATE;
	}
}

