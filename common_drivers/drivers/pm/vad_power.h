/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __VAD_POWER_H__
#define __VAD_POWER_H__

struct pm_ops {
	int (*vad_pm_init)(struct platform_device *pdev);
	int (*vad_pm_suspend)(struct platform_device *pdev);
	int (*vad_pm_resume)(struct platform_device *pdev);
};

#define SMCID_POWER_MANAGER	0x82000079
	#define SMCSUBID_PM_DDR_ASR	0x1000
	#define SMCSUBID_PM_DUMP_INFO	0x1001

struct pm_data {
	struct device *dev;
	bool vad_wakeup_disable;
	int pm_wakeup_irq;
	/* Define platform private data */
	const char *name;
	void *data;
	struct pm_ops *ops;
};

extern struct pm_ops t3_pm_ops;
extern struct pm_ops t5m_pm_ops;

int vad_wakeup_power_init(struct platform_device *pdev, struct pm_data *p_data);
int vad_wakeup_power_suspend(struct device *dev);
int vad_wakeup_power_resume(struct device *dev);

#endif