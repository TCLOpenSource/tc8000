/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __A1_CLKC_H
#define __A1_CLKC_H

#define CLKID_EE_CORE				0
#define CLKID_FIXED_PLL_DCO			1
#define CLKID_FIXED_PLL				2
#define CLKID_FCLK_DIV2				3
#define CLKID_FCLK_DIV3				4
#define CLKID_FCLK_DIV5				5
#define CLKID_FCLK_DIV7				6
#define CLKID_FCLK_DIV2_DIV			7
#define CLKID_FCLK_DIV3_DIV			8
#define CLKID_FCLK_DIV5_DIV			9
#define CLKID_FCLK_DIV7_DIV			10
#define CLKID_SYS_A_SEL				11
#define CLKID_SYS_A_DIV				12
#define CLKID_SYS_A				13
#define CLKID_SYS_B_SEL				14
#define CLKID_SYS_B_DIV				15
#define CLKID_SYS_B				16
#define CLKID_SYS_CLK				17
#define CLKID_HIFI_PLL				18
#define CLKID_SYS_PLL				19
#define CLKID_AUD_DDS				20

#define CLKID_CPU_BASE				21
#define CLKID_CPU_FSOURCE_SEL0			(CLKID_CPU_BASE + 0)
#define CLKID_CPU_FSOURCE_DIV0			(CLKID_CPU_BASE + 1)
#define CLKID_CPU_FSEL0				(CLKID_CPU_BASE + 2)
#define CLKID_CPU_FSOURCE_SEL1			(CLKID_CPU_BASE + 3)
#define CLKID_CPU_FSOURCE_DIV1			(CLKID_CPU_BASE + 4)
#define CLKID_CPU_FSEL1				(CLKID_CPU_BASE + 5)
#define CLKID_CPU_FCLK				(CLKID_CPU_BASE + 6)
#define CLKID_CPU_CLK				(CLKID_CPU_BASE + 7)

/* CLKTREE_SYS_OSCIN_CTRL gates*/
#define XTAL_BASE				(CLKID_CPU_BASE + 8)
#define CLKID_XTAL_CLKTREE			(XTAL_BASE + 0)
#define CLKID_XTAL_FIXPLL			(XTAL_BASE + 1)
#define CLKID_XTAL_USB_PHY			(XTAL_BASE + 2)
#define CLKID_XTAL_USB_CTRL			(XTAL_BASE + 3)
#define CLKID_XTAL_HIFIPLL			(XTAL_BASE + 4)
#define CLKID_XTAL_SYSPLL			(XTAL_BASE + 5)
#define CLKID_XTAL_DDS				(XTAL_BASE + 6)
/* CLKTREE_SYS_CLK_EN0 gates*/
#define GATE_BASE0				(XTAL_BASE + 7)
#define CLKID_CLKTREE				(GATE_BASE0 + 0)
#define CLKID_RESET_CTRL			(GATE_BASE0 + 1)
#define CLKID_ANALOG_CTRL			(GATE_BASE0 + 2)
#define CLKID_PWR_CTRL				(GATE_BASE0 + 3)
#define CLKID_PAD_CTRL				(GATE_BASE0 + 4)
#define CLKID_SYS_CTRL				(GATE_BASE0 + 5)
#define CLKID_TEMP_SENSOR			(GATE_BASE0 + 6)
#define CLKID_AM2AXI_DIV			(GATE_BASE0 + 7)
#define CLKID_SPICC_B				(GATE_BASE0 + 8)
#define CLKID_SPICC_A				(GATE_BASE0 + 9)
#define CLKID_CLK_MSR				(GATE_BASE0 + 10)
#define CLKID_AUDIO				(GATE_BASE0 + 11)
#define CLKID_JTAG_CTRL				(GATE_BASE0 + 12)
#define CLKID_SARADC				(GATE_BASE0 + 13)
#define CLKID_PWM_EF				(GATE_BASE0 + 14)
#define CLKID_PWM_CD				(GATE_BASE0 + 15)
#define CLKID_PWM_AB				(GATE_BASE0 + 16)
#define CLKID_CEC				(GATE_BASE0 + 17)
#define CLKID_I2C_S				(GATE_BASE0 + 18)
#define CLKID_IR_CTRL				(GATE_BASE0 + 19)
#define CLKID_I2C_M_D				(GATE_BASE0 + 20)
#define CLKID_I2C_M_C				(GATE_BASE0 + 21)
#define CLKID_I2C_M_B				(GATE_BASE0 + 22)
#define CLKID_I2C_M_A				(GATE_BASE0 + 23)
#define CLKID_ACODEC				(GATE_BASE0 + 24)
#define CLKID_OTP				(GATE_BASE0 + 25)
#define CLKID_SD_EMMC_A				(GATE_BASE0 + 26)
#define CLKID_USB_PHY				(GATE_BASE0 + 27)
#define CLKID_USB_CTRL				(GATE_BASE0 + 28)
#define CLKID_SYS_DSPB				(GATE_BASE0 + 29)
#define CLKID_SYS_DSPA				(GATE_BASE0 + 30)
#define CLKID_DMA				(GATE_BASE0 + 31)
/* CLKTREE_SYS_CLK_EN1 gates*/
#define GATE_BASE1				(GATE_BASE0 + 32)
#define CLKID_IRQ_CTRL				(GATE_BASE1 + 0)
#define CLKID_NIC				(GATE_BASE1 + 1)
#define CLKID_GIC				(GATE_BASE1 + 2)
#define CLKID_UART_C				(GATE_BASE1 + 3)
#define CLKID_UART_B				(GATE_BASE1 + 4)
#define CLKID_UART_A				(GATE_BASE1 + 5)
#define CLKID_SYS_PSRAM				(GATE_BASE1 + 6)
#define CLKID_RSA				(GATE_BASE1 + 7)
#define CLKID_CORESIGHT				(GATE_BASE1 + 8)
/* CLKTREE_AXI_CLK_EN gates */
#define GATE_BASE2				(GATE_BASE1 + 9)
#define CLKID_AM2AXI_VAD			(GATE_BASE2 + 0)
#define CLKID_AUDIO_VAD				(GATE_BASE2 + 1)
#define CLKID_AXI_DMC				(GATE_BASE2 + 2)
#define CLKID_AXI_PSRAM				(GATE_BASE2 + 3)
#define CLKID_RAMB				(GATE_BASE2 + 4)
#define CLKID_RAMA				(GATE_BASE2 + 5)
#define CLKID_AXI_SPIFC				(GATE_BASE2 + 6)
#define CLKID_AXI_NIC				(GATE_BASE2 + 7)
#define CLKID_AXI_DMA				(GATE_BASE2 + 8)
#define CLKID_CPU_CTRL				(GATE_BASE2 + 9)
#define CLKID_ROM				(GATE_BASE2 + 10)
#define CLKID_PROC_I2C				(GATE_BASE2 + 11)
/* dsp clks */
#define DSP_BASE				(GATE_BASE2 + 12)
#define CLKID_DSPA_A_SEL			(DSP_BASE + 0)
#define CLKID_DSPA_A_DIV			(DSP_BASE + 1)
#define CLKID_DSPA_A				(DSP_BASE + 2)
#define CLKID_DSPA_B_SEL			(DSP_BASE + 3)
#define CLKID_DSPA_B_DIV			(DSP_BASE + 4)
#define CLKID_DSPA_B				(DSP_BASE + 5)
#define CLKID_DSPA_SEL				(DSP_BASE + 6)
#define CLKID_DSPB_A_SEL			(DSP_BASE + 7)
#define CLKID_DSPB_A_DIV			(DSP_BASE + 8)
#define CLKID_DSPB_A				(DSP_BASE + 9)
#define CLKID_DSPB_B_SEL			(DSP_BASE + 10)
#define CLKID_DSPB_B_DIV			(DSP_BASE + 11)
#define CLKID_DSPB_B				(DSP_BASE + 12)
#define CLKID_DSPB_SEL				(DSP_BASE + 13)
#define CLKID_DSPA_EN_DSPA			(DSP_BASE + 14)
#define CLKID_DSPA_EN_NIC			(DSP_BASE + 15)
#define CLKID_DSPB_EN_DSPB			(DSP_BASE + 16)
#define CLKID_DSPB_EN_NIC			(DSP_BASE + 17)
/* 32k: rtc and ceca clock */
#define RTC_32K_BASE				(DSP_BASE + 18)
#define CLKID_RTC_32K_CLKIN			(RTC_32K_BASE + 0)
#define CLKID_RTC_32K_DIV			(RTC_32K_BASE + 1)
#define CLKID_RTC_32K_XTAL			(RTC_32K_BASE + 2)
#define CLKID_RTC_32K_SEL			(RTC_32K_BASE + 3)
#define CLKID_RTC_CLK				(RTC_32K_BASE + 4)
#define CLKID_CECA_32K_CLKIN			(RTC_32K_BASE + 5)
#define CLKID_CECA_32K_DIV			(RTC_32K_BASE + 6)
#define CLKID_CECA_32K_SEL_PRE			(RTC_32K_BASE + 7)
#define CLKID_CECA_32K_SEL			(RTC_32K_BASE + 8)
#define CLKID_CECA_32K				(RTC_32K_BASE + 9)
#define CLKID_CECB_32K_CLKIN			(RTC_32K_BASE + 10)
#define CLKID_CECB_32K_DIV			(RTC_32K_BASE + 11)
#define CLKID_CECB_32K_SEL_PRE			(RTC_32K_BASE + 12)
#define CLKID_CECB_32K_SEL			(RTC_32K_BASE + 13)
#define CLKID_CECB_32K				(RTC_32K_BASE + 14)
/* 12m/24m/gen clks */
#define BASIC_BASE				(RTC_32K_BASE + 15)
#define CLKID_24M				(BASIC_BASE + 0)
#define CLKID_12M				(BASIC_BASE + 1)
#define CLKID_DIV2_PRE				(BASIC_BASE + 2)
#define CLKID_FCLK_DIV2_DIVN			(BASIC_BASE + 3)
#define CLKID_24M_DIV2				(BASIC_BASE + 4)
#define CLKID_GEN_SEL				(BASIC_BASE + 5)
#define CLKID_GEN_DIV				(BASIC_BASE + 6)
#define CLKID_GEN				(BASIC_BASE + 7)
#define CLKID_SARADC_SEL			(BASIC_BASE + 8)
#define CLKID_SARADC_DIV			(BASIC_BASE + 9)
#define CLKID_SARADC_GATE			(BASIC_BASE + 10)
#define CLKID_PWM_A_SEL				(BASIC_BASE + 11)
#define CLKID_PWM_A_DIV				(BASIC_BASE + 12)
#define CLKID_PWM_A				(BASIC_BASE + 13)
#define CLKID_PWM_B_SEL				(BASIC_BASE + 14)
#define CLKID_PWM_B_DIV				(BASIC_BASE + 15)
#define CLKID_PWM_B				(BASIC_BASE + 16)
#define CLKID_PWM_C_SEL				(BASIC_BASE + 17)
#define CLKID_PWM_C_DIV				(BASIC_BASE + 18)
#define CLKID_PWM_C				(BASIC_BASE + 19)
#define CLKID_PWM_D_SEL				(BASIC_BASE + 20)
#define CLKID_PWM_D_DIV				(BASIC_BASE + 21)
#define CLKID_PWM_D				(BASIC_BASE + 22)
#define CLKID_PWM_E_SEL				(BASIC_BASE + 23)
#define CLKID_PWM_E_DIV				(BASIC_BASE + 24)
#define CLKID_PWM_E				(BASIC_BASE + 25)
#define CLKID_PWM_F_SEL				(BASIC_BASE + 26)
#define CLKID_PWM_F_DIV				(BASIC_BASE + 27)
#define CLKID_PWM_F				(BASIC_BASE + 28)
#define CLKID_SPICC_SEL				(BASIC_BASE + 29)
#define CLKID_SPICC_DIV				(BASIC_BASE + 30)
#define CLKID_SPICC_GATE			(BASIC_BASE + 31)
#define CLKID_SPICC				(BASIC_BASE + 32)
#define CLKID_TS_DIV				(BASIC_BASE + 33)
#define CLKID_TS				(BASIC_BASE + 34)
#define BASIC_BASE2				(BASIC_BASE + 35)
#define CLKID_SPIFC_SEL				(BASIC_BASE2 + 0)
#define CLKID_SPIFC_DIV				(BASIC_BASE2 + 1)
#define CLKID_SPIFC_GATE			(BASIC_BASE2 + 2)
#define CLKID_SPIFC				(BASIC_BASE2 + 3)
#define CLKID_USB_BUS_SEL			(BASIC_BASE2 + 4)
#define CLKID_USB_BUS_DIV			(BASIC_BASE2 + 5)
#define CLKID_USB_BUS				(BASIC_BASE2 + 6)
#define CLKID_SD_EMMC_SEL			(BASIC_BASE2 + 7)
#define CLKID_SD_EMMC_DIV			(BASIC_BASE2 + 8)
#define CLKID_SD_EMMC_GATE			(BASIC_BASE2 + 9)
#define CLKID_SD_EMMC				(BASIC_BASE2 + 10)
#define CLKID_PSRAM_SEL				(BASIC_BASE2 + 11)
#define CLKID_PSRAM_DIV				(BASIC_BASE2 + 12)
#define CLKID_PSRAM_GATE			(BASIC_BASE2 + 13)
#define CLKID_PSRAM				(BASIC_BASE2 + 14)
#define CLKID_DMC_SEL				(BASIC_BASE2 + 15)
#define CLKID_DMC_DIV				(BASIC_BASE2 + 16)
#define CLKID_DMC_GATE				(BASIC_BASE2 + 17)
#define CLKID_DMC				(BASIC_BASE2 + 18)
#define CLKID_FIXED_PLL_DIV2			(BASIC_BASE2 + 19)

#endif /* __A1_CLKC_H */
