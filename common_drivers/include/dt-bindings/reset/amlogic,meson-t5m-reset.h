/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef _DT_BINDINGS_AMLOGIC_MESON_T5M_RESET_H
#define _DT_BINDINGS_AMLOGIC_MESON_T5M_RESET_H

/* RESET0 */
/*					0-2*/
#define RESET_USB			3
#define RESET_U2DRDX1			4
#define RESET_U2DRDX0			5
#define RESET_U3DRD			6
/*					7*/
#define RESET_U2PHY21			8
#define RESET_U2PHY20			9
/*					10*/
#define RESET_HDMI20_AES		11
#define RESET_HDMIRX			12
#define RESET_HDMIRX_APB		13
/*					14*/
#define RESET_VPU_HDMI_AXI		15
/*					16*/
#define RESET_BRG_VCBUS_DEC		17
#define RESET_VCBUS			18
#define RESET_VID_PLL_DIV		19
#define RESET_VDI6			20
#define RESET_GE2D			21
/*					22*/
#define RESET_VID_LOCK			23
#define RESET_VENC0			24
#define RESET_VDAC			25
#define RESET_VENC2			26
#define RESET_VENC1			27
#define RESET_RDMA			28
/*					29*/
#define RESET_VIU			30
#define RESET_VENC			31

/* RESET1 */
#define RESET_AUDIO			32
#define RESET_MALI_CAPB3		33
#define RESET_MALI			34
#define RESET_DDRPLL			35
#define RESET_DMC			36
#define RESET_DOS_CAPB3			37
#define RESET_DOS			38
/*					39-41*/
#define RESET_M31_UTMI			42
/*					43-33*/
#define RESET_PCIE_PHY			45
#define RESET_PCIE_APB			46
#define RESET_M31PHY_PCIE_PIPE		47
#define RESET_ETH			48
/*					49-50*/
#define RESET_COMBO_DPHY_CHAN0		51
/*					52-55*/
#define RESET_DMC1			56
#define RESET_DEMOD			57
/*					58-61*/
#define RESET_DDR1PLL			62
/*					63*/

/* RESET2 */
/*					64*/
#define RESET_IR_CTRL			65
/*					66-67*/
#define RESET_SPICC_2			68
#define RESET_TCON			69
/*					70-71*/
#define RESET_SMART_CARD		72
#define RESET_SPICC_0			73
#define RESET_SPICC_1			74
#define RESET_LED_CTRL			75
/*					76*/
#define RESET_FRC_APB			77
#define RESET_FRC_RDMA			78
#define RESET_FRC			79
#define RESET_MSR_CLK			80
#define RESET_SPIFC			81
#define RESET_SAR_ADC			82
/*					83-87*/
#define RESET_ACODEC			88
#define RESET_CEC			89
/*					90*/
#define RESET_WATCHDOG			91
/*					92*/
#define RESET_TVFE			93
#define RESET_ATV_DMD			94
#define RESET_ADEC			95

/* RESET3 */
/*					96-126*/
#define RESET_A55_ACE			127

/* RESET4 */
/*					128-131*/
#define RESET_PWM_AB			132
#define RESET_PWM_CD			133
#define RESET_PWM_EF			134
#define RESET_PWM_GH			135
/*					136-137*/
#define RESET_UART_A			138
#define RESET_UART_B			139
#define RESET_UART_C			140
/*					141-142*/
#define RESET_CIPLUS			143
#define RESET_I2C_S_A			144
#define RESET_I2C_M_A			145
#define RESET_I2C_M_B			146
#define RESET_I2C_M_C			147
#define RESET_I2C_M_D			148
#define RESET_I2C_M_E			149
/*					150-152*/
#define RESET_SD_EMMC_B			153
#define RESET_SD_EMMC_C			154
/*					155*/
#define RESET_TS_CPU			156
/*					157*/
#define RESET_TS_VPU			158
/*					159*/

/* RESET5 */
/*					160-167*/
#define RESET_BRG_NICHDMIUSB_SYS	168
#define RESET_BRG_NICHDMIUSB_MAIN	169
#define RESET_BRG_NICHDMIUSB_HDMI	170
#define RESET_BRG_NICHDMIUSB_ALL	171
/*					172*/
#define RESET_BRG_NICGPU_SYS		173
#define RESET_BRG_NICGPU_MAIN		174
#define RESET_BRG_NICGPU_ALL		175
#define RESET_BRG_NICHCOD_GE2D_HCODEC	176
#define RESET_BRG_NICHCOD_GE2D_GE2D	177
/*					178*/
#define RESET_BRG_NICHCOD_GE2D_SYS	179
#define RESET_BRG_NICHCOD_GE2D_MAIN	180
#define RESET_BRG_NICHCOD_GE2D_ALL	181
#define RESET_BRG_NICCPU_SYS		182
#define RESET_BRG_NICCPU_MAIN		183
#define RESET_BRG_NICCPU_ALL		184
#define RESET_BRG_NICSYS_EMMCB		185
#define RESET_BRG_NICSYS_EMMCC		186
#define RESET_BRG_NICSYS_MAIN		187
#define RESET_BRG_NICSYS_VAPB		188
#define RESET_BRG_NICSYS_SYS		189
#define RESET_BRG_NICSYS_CPU		190
#define RESET_BRG_NICSYS_ALL		191

/* RESET6 */
#define RESET_BRG_VDEC_PIPEL			192
#define RESET_BRG_HEVCF_DMC_PIPEL		193
#define RESET_BRG_VPU_TOP_APB_PIPEL		194
#define RESET_BRG_NICGPUTODDR0_PIPEL		195
#define RESET_BRG_NICGPUTODDR1_PIPEL		196
#define RESET_BRG_NICCPUTODDR0_PIPEL		197
#define RESET_BRG_NICCPUTODDR1_PIPEL		198
#define RESET_BRG_NICSYSTODDR1_PIPEL		199
#define RESET_BRG_NICHCODGE2DTODDR1_PIPEL	200
#define RESET_BRG_VPU0TODDR0_PIPEL		201
#define RESET_BRG_FRCTODDR1_PIPEL		202
#define RESET_BRG_NICHCODGE2DTODDR0_PIPEL	203
#define RESET_BRG_EMMCCTONICSYS_PIPEL		204
#define RESET_BRG_MALITONICGPU_PIPEL		205
#define RESET_BRG_NICHDMIUSBTONICSYS_PIPEL	206
/*					207*/
#define RESET_BRG_ETH_HDMIRX_APB_PIPEL		208
#define RESET_BRG_DOS_APB_PIPEL			209
#define RESET_BRG_GE2D_APB_PIPEL		210
#define RESET_BRG_ACODEC_APB_PIPEL		211
#define RESET_BRG_USB_PCIE_CTRL_APB_PIPEL	212
#define RESET_BRG_USB_PCIE_PHY_APB_PIPEL	213
#define RESET_BRG_ETH_AHB_PIPEL			214
#define RESET_BRG_USB0_AHB_PIPEL		215
#define RESET_BRG_USB1_AHB_PIPEL		216
#define RESET_BRG_USB2_AHB_PIPEL		217
/*					218*/
#define RESET_BRG_AMPIPE_ETH			219
#define RESET_BRG_FRC_TOP_APB_PIPEL		220
/*					221*/
#define RESET_BRG_AM2AXI1			222
#define RESET_BRG_AM2AXI2			223

#endif
