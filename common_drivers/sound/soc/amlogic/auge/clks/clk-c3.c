// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#undef pr_fmt
#define pr_fmt(fmt) "c3_audio_clocks: " fmt

#include <dt-bindings/clock/amlogic,c3-audio-clk.h>

#include "audio_clks.h"
#include "../regs.h"

extern spinlock_t aclk_lock;

static const char *const mclk_parent_names[] = {
	"mpll0", "mpll1", "mpll2", "rtc_pll", "hifi_pll",
	"fclk_div3", "fclk_div4", "cts_oscin"};

static const char *const audioclk_parent_names[] = {
	"mclk_a", "mclk_b", "mclk_c", "mclk_d", "mclk_e",
	"mclk_f", "i_slv_sclk_a", "i_slv_sclk_b", "i_slv_sclk_c",
	"i_slv_sclk_d", "i_slv_sclk_e", "i_slv_sclk_f", "i_slv_sclk_g",
	"i_slv_sclk_h", "i_slv_sclk_i", "i_slv_sclk_j"};

static const char *const mclk_pad_parent_names[] = {
	"mclk_a", "mclk_b", "mclk_c",
	"mclk_d", "mclk_e", "mclk_f"
};

CLOCK_GATE(audio_ddr_arb, AUD_ADDR_OFFSET(EE_AUDIO_CLK_GATE_EN0), 0, sys_clk);
CLOCK_GATE(audio_pdm, AUD_ADDR_OFFSET(EE_AUDIO_CLK_GATE_EN0), 1, sys_clk);
CLOCK_GATE(audio_tdmina, AUD_ADDR_OFFSET(EE_AUDIO_CLK_GATE_EN0), 2, sys_clk);
CLOCK_GATE(audio_tdminlb, AUD_ADDR_OFFSET(EE_AUDIO_CLK_GATE_EN0), 5, sys_clk);
CLOCK_GATE(audio_tdmouta, AUD_ADDR_OFFSET(EE_AUDIO_CLK_GATE_EN0), 6, sys_clk);
CLOCK_GATE(audio_tdmoutb, AUD_ADDR_OFFSET(EE_AUDIO_CLK_GATE_EN0), 7, sys_clk);
CLOCK_GATE(audio_frddra, AUD_ADDR_OFFSET(EE_AUDIO_CLK_GATE_EN0), 9, sys_clk);
CLOCK_GATE(audio_frddrb, AUD_ADDR_OFFSET(EE_AUDIO_CLK_GATE_EN0), 10, sys_clk);
CLOCK_GATE(audio_frddrc, AUD_ADDR_OFFSET(EE_AUDIO_CLK_GATE_EN0), 11, sys_clk);
CLOCK_GATE(audio_toddra, AUD_ADDR_OFFSET(EE_AUDIO_CLK_GATE_EN0), 12, sys_clk);
CLOCK_GATE(audio_toddrb, AUD_ADDR_OFFSET(EE_AUDIO_CLK_GATE_EN0), 13, sys_clk);
CLOCK_GATE(audio_loopbacka, AUD_ADDR_OFFSET(EE_AUDIO_CLK_GATE_EN0), 15, sys_clk);
CLOCK_GATE(audio_resamplea, AUD_ADDR_OFFSET(EE_AUDIO_CLK_GATE_EN0), 18, sys_clk);
CLOCK_GATE(audio_eqdrc, AUD_ADDR_OFFSET(EE_AUDIO_CLK_GATE_EN0), 22, sys_clk);
CLOCK_GATE(audio_tovad, AUD_ADDR_OFFSET(EE_AUDIO_CLK_GATE_EN0), 27, sys_clk);
CLOCK_GATE(audio_audiolocker, AUD_ADDR_OFFSET(EE_AUDIO_CLK_GATE_EN0), 28, sys_clk);

static struct clk_gate *c3_audio_clk_gates[] = {
	&audio_ddr_arb,
	&audio_pdm,
	&audio_tdmina,
	&audio_tdminlb,
	&audio_tdmouta,
	&audio_tdmoutb,
	&audio_frddra,
	&audio_frddrb,
	&audio_frddrc,
	&audio_toddra,
	&audio_toddrb,
	&audio_loopbacka,
	&audio_resamplea,
	&audio_eqdrc,
	&audio_tovad,
	&audio_audiolocker,
};

/* Array of all clocks provided by this provider */
static struct clk_hw *c3_audio_clk_hws[] = {
	[CLKID_AUDIO_GATE_DDR_ARB]      = &audio_ddr_arb.hw,
	[CLKID_AUDIO_GATE_PDM]      = &audio_pdm.hw,
	[CLKID_AUDIO_GATE_TDMINA]      = &audio_tdmina.hw,
	[CLKID_AUDIO_GATE_TDMINLB]     = &audio_tdminlb.hw,
	[CLKID_AUDIO_GATE_TDMOUTA]     = &audio_tdmouta.hw,
	[CLKID_AUDIO_GATE_TDMOUTB]     = &audio_tdmoutb.hw,
	[CLKID_AUDIO_GATE_FRDDRA]      = &audio_frddra.hw,
	[CLKID_AUDIO_GATE_FRDDRB]      = &audio_frddrb.hw,
	[CLKID_AUDIO_GATE_FRDDRC]      = &audio_frddrc.hw,
	[CLKID_AUDIO_GATE_TODDRA]      = &audio_toddra.hw,
	[CLKID_AUDIO_GATE_TODDRB]      = &audio_toddrb.hw,
	[CLKID_AUDIO_GATE_LOOPBACKA]   = &audio_loopbacka.hw,
	[CLKID_AUDIO_GATE_RESAMPLEA]   = &audio_resamplea.hw,
	[CLKID_AUDIO_GATE_EQDRC]       = &audio_eqdrc.hw,
	[CLKID_AUDIO_GATE_TOVAD]     = &audio_tovad.hw,
	[CLKID_AUDIO_GATE_AUDIOLOCKER] = &audio_audiolocker.hw,
};

static int c3_clk_gates_init(struct clk **clks, void __iomem *iobase)
{
	int clkid;

	if (ARRAY_SIZE(c3_audio_clk_gates) != CLKID_AUDIO_GATE_MAX) {
		pr_err("error: check clk gates number\n");
		return -EINVAL;
	}

	for (clkid = 0; clkid < CLKID_AUDIO_GATE_MAX; clkid++) {
		c3_audio_clk_gates[clkid]->reg = iobase;
		clks[clkid] = clk_register(NULL, c3_audio_clk_hws[clkid]);
		WARN_ON(IS_ERR_OR_NULL(clks[clkid]));
	}

	return 0;
}

/* mclk_a */
CLOCK_COM_MUX(mclk_a, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_A_CTRL(1)), 0x7, 24);
CLOCK_COM_DIV(mclk_a, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_A_CTRL(1)), 0, 16);
CLOCK_COM_GATE(mclk_a, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_A_CTRL(1)), 31);
/* mclk_b */
CLOCK_COM_MUX(mclk_b, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_B_CTRL(1)), 0x7, 24);
CLOCK_COM_DIV(mclk_b, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_B_CTRL(1)), 0, 16);
CLOCK_COM_GATE(mclk_b, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_B_CTRL(1)), 31);
/* mclk_c */
CLOCK_COM_MUX(mclk_c, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_C_CTRL(1)), 0x7, 24);
CLOCK_COM_DIV(mclk_c, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_C_CTRL(1)), 0, 16);
CLOCK_COM_GATE(mclk_c, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_C_CTRL(1)), 31);
/* mclk_d */
CLOCK_COM_MUX(mclk_d, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_D_CTRL(1)), 0x7, 24);
CLOCK_COM_DIV(mclk_d, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_D_CTRL(1)), 0, 16);
CLOCK_COM_GATE(mclk_d, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_D_CTRL(1)), 31);
/* mclk_e */
CLOCK_COM_MUX(mclk_e, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_E_CTRL(1)), 0x7, 24);
CLOCK_COM_DIV(mclk_e, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_E_CTRL(1)), 0, 16);
CLOCK_COM_GATE(mclk_e, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_E_CTRL(1)), 31);
/* mclk_f */
CLOCK_COM_MUX(mclk_f, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_F_CTRL(1)), 0x7, 24);
CLOCK_COM_DIV(mclk_f, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_F_CTRL(1)), 0, 16);
CLOCK_COM_GATE(mclk_f, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_F_CTRL(1)), 31);

/* mclk_pad0 */
CLOCK_COM_MUX(mclk_pad0, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_PAD_CTRL0), 0x7, 8);
CLOCK_COM_DIV(mclk_pad0, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_PAD_CTRL0), 0, 8);
CLOCK_COM_GATE(mclk_pad0, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_PAD_CTRL0), 15);
/* mclk_pad1 */
CLOCK_COM_MUX(mclk_pad1, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_PAD_CTRL0), 0x7, 24);
CLOCK_COM_DIV(mclk_pad1, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_PAD_CTRL0), 16, 8);
CLOCK_COM_GATE(mclk_pad1, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_PAD_CTRL0), 31);
/* mclk_pad2 */
CLOCK_COM_MUX(mclk_pad2, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_PAD_CTRL1), 0x7, 8);
CLOCK_COM_DIV(mclk_pad2, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_PAD_CTRL1), 0, 8);
CLOCK_COM_GATE(mclk_pad2, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_PAD_CTRL1), 15);
/* mclk_pad3 */
CLOCK_COM_MUX(mclk_pad3, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_PAD_CTRL1), 0x7, 24);
CLOCK_COM_DIV(mclk_pad3, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_PAD_CTRL1), 16, 8);
CLOCK_COM_GATE(mclk_pad3, AUD_ADDR_OFFSET(EE_AUDIO_MCLK_PAD_CTRL1), 31);

/* audio resample_a */
CLOCK_COM_MUX(resample_a,
	AUD_ADDR_OFFSET(EE_AUDIO_CLK_RESAMPLEA_CTRL), 0xf, 24);
CLOCK_COM_DIV(resample_a, AUD_ADDR_OFFSET(EE_AUDIO_CLK_RESAMPLEA_CTRL), 0, 8);
CLOCK_COM_GATE(resample_a, AUD_ADDR_OFFSET(EE_AUDIO_CLK_RESAMPLEA_CTRL), 31);
/* audio locker_out */
CLOCK_COM_MUX(locker_out, AUD_ADDR_OFFSET(EE_AUDIO_CLK_LOCKER_CTRL), 0xf, 24);
CLOCK_COM_DIV(locker_out, AUD_ADDR_OFFSET(EE_AUDIO_CLK_LOCKER_CTRL), 16, 8);
CLOCK_COM_GATE(locker_out, AUD_ADDR_OFFSET(EE_AUDIO_CLK_LOCKER_CTRL), 31);
/* audio locker_in */
CLOCK_COM_MUX(locker_in, AUD_ADDR_OFFSET(EE_AUDIO_CLK_LOCKER_CTRL), 0xf, 8);
CLOCK_COM_DIV(locker_in, AUD_ADDR_OFFSET(EE_AUDIO_CLK_LOCKER_CTRL), 0, 8);
CLOCK_COM_GATE(locker_in, AUD_ADDR_OFFSET(EE_AUDIO_CLK_LOCKER_CTRL), 15);
/* pdmin0 */
CLOCK_COM_MUX(pdmin0, AUD_ADDR_OFFSET(EE_AUDIO_CLK_PDMIN_CTRL0), 0xf, 24);
CLOCK_COM_DIV(pdmin0, AUD_ADDR_OFFSET(EE_AUDIO_CLK_PDMIN_CTRL0), 0, 16);
CLOCK_COM_GATE(pdmin0, AUD_ADDR_OFFSET(EE_AUDIO_CLK_PDMIN_CTRL0), 31);
/* pdmin1 */
CLOCK_COM_MUX(pdmin1, AUD_ADDR_OFFSET(EE_AUDIO_CLK_PDMIN_CTRL1), 0x7, 24);
CLOCK_COM_DIV(pdmin1, AUD_ADDR_OFFSET(EE_AUDIO_CLK_PDMIN_CTRL1), 0, 16);
CLOCK_COM_GATE(pdmin1, AUD_ADDR_OFFSET(EE_AUDIO_CLK_PDMIN_CTRL1), 31);
/* audio eqdrc  */
CLOCK_COM_MUX(eqdrc, AUD_ADDR_OFFSET(EE_AUDIO_CLK_EQDRC_CTRL0), 0x7, 24);
CLOCK_COM_DIV(eqdrc, AUD_ADDR_OFFSET(EE_AUDIO_CLK_EQDRC_CTRL0), 0, 16);
CLOCK_COM_GATE(eqdrc, AUD_ADDR_OFFSET(EE_AUDIO_CLK_EQDRC_CTRL0), 31);

/* audio vad  */
CLOCK_COM_MUX(vad, AUD_ADDR_OFFSET(EE_AUDIO_CLK_VAD_CTRL), 0xf, 24);
CLOCK_COM_DIV(vad, AUD_ADDR_OFFSET(EE_AUDIO_CLK_VAD_CTRL), 0, 16);
CLOCK_COM_GATE(vad, AUD_ADDR_OFFSET(EE_AUDIO_CLK_VAD_CTRL), 31);

static int c3_clks_init(struct clk **clks, void __iomem *iobase)
{
	IOMAP_COM_CLK(mclk_a, iobase);
	clks[CLKID_AUDIO_MCLK_A] = REGISTER_CLK_COM(mclk_a);
	WARN_ON(IS_ERR_OR_NULL(clks[CLKID_AUDIO_MCLK_A]));

	IOMAP_COM_CLK(mclk_b, iobase);
	clks[CLKID_AUDIO_MCLK_B] = REGISTER_CLK_COM(mclk_b);
	WARN_ON(IS_ERR_OR_NULL(clks[CLKID_AUDIO_MCLK_B]));

	IOMAP_COM_CLK(mclk_c, iobase);
	clks[CLKID_AUDIO_MCLK_C] = REGISTER_CLK_COM(mclk_c);
	WARN_ON(IS_ERR_OR_NULL(clks[CLKID_AUDIO_MCLK_C]));

	IOMAP_COM_CLK(mclk_d, iobase);
	clks[CLKID_AUDIO_MCLK_D] = REGISTER_CLK_COM(mclk_d);
	WARN_ON(IS_ERR_OR_NULL(clks[CLKID_AUDIO_MCLK_D]));

	IOMAP_COM_CLK(mclk_e, iobase);
	clks[CLKID_AUDIO_MCLK_E] = REGISTER_CLK_COM(mclk_e);
	WARN_ON(IS_ERR_OR_NULL(clks[CLKID_AUDIO_MCLK_E]));

	IOMAP_COM_CLK(mclk_f, iobase);
	clks[CLKID_AUDIO_MCLK_F] = REGISTER_CLK_COM(mclk_f);
	WARN_ON(IS_ERR_OR_NULL(clks[CLKID_AUDIO_MCLK_F]));

	IOMAP_COM_CLK(resample_a, iobase);
	clks[CLKID_AUDIO_RESAMPLE_A] = REGISTER_AUDIOCLK_COM(resample_a);
	WARN_ON(IS_ERR_OR_NULL(clks[CLKID_AUDIO_RESAMPLE_A]));

	IOMAP_COM_CLK(locker_out, iobase);
	clks[CLKID_AUDIO_LOCKER_OUT] = REGISTER_AUDIOCLK_COM(locker_out);
	WARN_ON(IS_ERR_OR_NULL(clks[CLKID_AUDIO_LOCKER_OUT]));

	IOMAP_COM_CLK(locker_in, iobase);
	clks[CLKID_AUDIO_LOCKER_IN] = REGISTER_AUDIOCLK_COM(locker_in);
	WARN_ON(IS_ERR_OR_NULL(clks[CLKID_AUDIO_LOCKER_IN]));

	IOMAP_COM_CLK(pdmin0, iobase);
	clks[CLKID_AUDIO_PDMIN0] = REGISTER_CLK_COM(pdmin0);
	WARN_ON(IS_ERR_OR_NULL(clks[CLKID_AUDIO_PDMIN0]));

	IOMAP_COM_CLK(pdmin1, iobase);
	clks[CLKID_AUDIO_PDMIN1] = REGISTER_CLK_COM(pdmin1);
	WARN_ON(IS_ERR_OR_NULL(clks[CLKID_AUDIO_PDMIN1]));

	IOMAP_COM_CLK(eqdrc, iobase);
	clks[CLKID_AUDIO_EQDRC] = REGISTER_CLK_COM(eqdrc);
	WARN_ON(IS_ERR_OR_NULL(clks[CLKID_AUDIO_EQDRC]));

	IOMAP_COM_CLK(vad, iobase);
	clks[CLKID_AUDIO_VAD] = REGISTER_CLK_COM(vad);
	WARN_ON(IS_ERR_OR_NULL(clks[CLKID_AUDIO_VAD]));

	IOMAP_COM_CLK(mclk_pad0, iobase);
	clks[CLKID_AUDIO_MCLK_PAD0] =
			REGISTER_CLK_COM_PARENTS(mclk_pad0, mclk_pad);
	WARN_ON(IS_ERR_OR_NULL(clks[CLKID_AUDIO_MCLK_PAD0]));

	IOMAP_COM_CLK(mclk_pad1, iobase);
	clks[CLKID_AUDIO_MCLK_PAD1] =
			REGISTER_CLK_COM_PARENTS(mclk_pad1, mclk_pad);
	WARN_ON(IS_ERR_OR_NULL(clks[CLKID_AUDIO_MCLK_PAD1]));

	IOMAP_COM_CLK(mclk_pad2, iobase);
	clks[CLKID_AUDIO_MCLK_PAD2] =
			REGISTER_CLK_COM_PARENTS(mclk_pad2, mclk_pad);
	WARN_ON(IS_ERR_OR_NULL(clks[CLKID_AUDIO_MCLK_PAD2]));

	IOMAP_COM_CLK(mclk_pad3, iobase);
	clks[CLKID_AUDIO_MCLK_PAD3] =
			REGISTER_CLK_COM_PARENTS(mclk_pad3, mclk_pad);
	WARN_ON(IS_ERR_OR_NULL(clks[CLKID_AUDIO_MCLK_PAD3]));

	return 0;
}

struct audio_clk_init c3_audio_clks_init = {
	.clk_num   = NUM_AUDIO_CLKS,
	.clk_gates = c3_clk_gates_init,
	.clks      = c3_clks_init,
};
