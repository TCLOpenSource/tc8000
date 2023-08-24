// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <linux/suspend.h>
#include <linux/pm.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/errno.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/amlogic/pm.h>
#include <linux/kobject.h>
#include <linux/gpio.h>
#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/amlogic/scpi_protocol.h>
#include <linux/amlogic/pm.h>
#include <linux/pm_wakeirq.h>
#include <linux/pm_wakeup.h>
#include <linux/interrupt.h>
#include "vad_power.h"

static suspend_state_t pm_state;
static pm_private_send_data_fn_t pm_send_data_fn;

unsigned int is_pm_s2idle_mode(void)
{
	if (pm_state == PM_SUSPEND_TO_IDLE)
		return 1;
	else
		return 0;
}
EXPORT_SYMBOL_GPL(is_pm_s2idle_mode);

static int frz_begin(void)
{
	pm_state = PM_SUSPEND_TO_IDLE;
	return 0;
}

static void frz_end(void)
{
	pm_state = PM_SUSPEND_ON;
}

#define FREEZE_ENTRY 0x01
#define FREEZE_EXIT	0x02
static int frz_prepare_late(void)
{
	u32 data;

	printk("[pm-frz] frz prepare late.\n");
	data = FREEZE_ENTRY;
	if (pm_send_data_fn) {
		printk("[pm-frz] Send message.\n");
		pm_send_data_fn((void *)&data, sizeof(data), SCPI_AOCPU,
			SCPI_CMD_PM_FREEZE, NULL, 0);
	}
	return 0;
}

static void frz_restore_early(void)
{
	u32 data;

	data = FREEZE_EXIT;
	printk("[pm-frz] frz restore early.\n");
	if (pm_send_data_fn) {
		printk("[pm-frz] Send message.\n");
		pm_send_data_fn((void *)&data, sizeof(data), SCPI_AOCPU,
			SCPI_CMD_PM_FREEZE, NULL, 0);
	}
}

static const struct platform_s2idle_ops meson_gx_frz_ops = {
	.begin = frz_begin,
	.end = frz_end,
	.prepare_late = frz_prepare_late,
	.restore_early = frz_restore_early,
};

int pm_set_private_send_data_callback(pm_private_send_data_fn_t fn)
{
	if (!fn) {
		pr_err("%s:%d, Pass invalid parameter.\n", __func__, __LINE__);
		return -EINVAL;
	}

	pm_send_data_fn = fn;

	return 0;
}
EXPORT_SYMBOL(pm_set_private_send_data_callback);

static int default_pm_init(struct platform_device *pdev)
{
	dev_warn(&pdev->dev,
			"Does not match the item type, use the default [init] function.\n");
	return 0;
}

static int defalut_pm_suspend(struct platform_device *pdev)
{
	dev_warn(&pdev->dev,
			"Does not match the item type, use the default [suspend] function.\n");
	return 0;
}

static int default_pm_resume(struct platform_device *pdev)
{
	dev_warn(&pdev->dev,
			"Does not match the item type, use the default [resume] function.\n");
	return 0;
}

struct pm_ops pm_default_ops = {
	.vad_pm_init = default_pm_init,
	.vad_pm_suspend = defalut_pm_suspend,
	.vad_pm_resume = default_pm_resume,
};

struct pm_item_table {
	const char *name;
	struct pm_ops *ops;
};

struct pm_item_table pm_support_item_list[] = {
	{"T3", &pm_default_ops},
	{"T5M", &pm_default_ops},
};

struct pm_ops *find_item_ops(const char *name)
{
	int i;

	if (!name)
		return &pm_default_ops;

	/* Find matching item form the table */
	for (i = 0; i < ARRAY_SIZE(pm_support_item_list); i++) {
		if (!strcmp(name, pm_support_item_list[i].name))
			return pm_support_item_list[i].ops;
	}

	return &pm_default_ops;
}

static void __iomem  *interrupt_clr;
static irqreturn_t pm_wakeup_isr(int irq, void *data __maybe_unused)
{
	pr_info("[PM]Wakeup form bl30\n");

	if (interrupt_clr)
		writel(0x00, interrupt_clr);
	return IRQ_HANDLED;
}

static int frz_power_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	struct pm_data *p_data;
	int ret;

	s2idle_set_ops(&meson_gx_frz_ops);
	p_data = devm_kzalloc(&pdev->dev, sizeof(struct pm_data), GFP_KERNEL);
	if (!p_data)
		return -ENOMEM;
	p_data->dev = &pdev->dev;
	platform_set_drvdata(pdev, p_data);

	/* check vad global flag */
	/* Resolve the project name in dts */
	p_data->name = of_get_property(np, "vad-item-name", NULL);
	if (!p_data->name) {
		dev_warn(&pdev->dev,
				"Not found item name in dts.The default item will be used!\n");
	}

	p_data->ops = find_item_ops(p_data->name);
	interrupt_clr = of_iomap(pdev->dev.of_node, 0);
	if (!interrupt_clr) {
		dev_warn(&pdev->dev,
				"Read dmc_asr addr fail\n");
	}

	p_data->pm_wakeup_irq = platform_get_irq_byname(pdev, "pm_wakeup");
	if (p_data->pm_wakeup_irq < 0) {
		dev_warn(&pdev->dev, "Failed to get pm_wakeup interrupt source:%d\n",
				p_data->pm_wakeup_irq);
	} else {
		ret = request_irq(p_data->pm_wakeup_irq,
				pm_wakeup_isr, IRQF_SHARED, "pm_wakeup",
				p_data);
		if (ret) {
			dev_err(&pdev->dev, "failed to claim irq_wakeup\n");
			return -ENXIO;
		}

		device_init_wakeup(&pdev->dev, 1);
		dev_pm_set_wake_irq(&pdev->dev, p_data->pm_wakeup_irq);
	}
	dev_warn(&pdev->dev,
			"freeze power probe done!\n");

	return 0;
}

int frz_power_suspend(struct device *dev)
{
	struct pm_data *p_data = dev_get_drvdata(dev);
	struct pm_ops *ops = p_data->ops;

	if (!is_pm_s2idle_mode()) {
		dev_info(dev, "Power mode is not freeze, return directly\n");
		return 0;
	}

	ops->vad_pm_suspend(to_platform_device(dev));

	return 0;
}

int frz_power_resume(struct device *dev)
{
	struct pm_data *p_data = dev_get_drvdata(dev);
	struct pm_ops *ops = p_data->ops;

	if (!is_pm_s2idle_mode()) {
		dev_info(dev, " Power mode is not freeze!!!\n");
		dev_info(dev, "return directly!!\n");
		return 0;
	}

	ops->vad_pm_resume(to_platform_device(dev));

	return 0;
}

static const struct dev_pm_ops meson_pm_frz_noirq_ops = {
	.suspend_noirq = frz_power_suspend,
	.resume_noirq = frz_power_resume,
};

static const struct of_device_id amlogic_pm_frz_dt_match[] = {
		{.compatible = "amlogic, pm-frz", },
		{}
};

static struct platform_driver meson_pm_frz_driver = {
	.probe = frz_power_probe,
	.driver = {
		.name = "pm-meson-frz",
		.owner = THIS_MODULE,
		.of_match_table = amlogic_pm_frz_dt_match,
		.pm = &meson_pm_frz_noirq_ops,
	},
};

module_platform_driver(meson_pm_frz_driver);
MODULE_AUTHOR("Amlogic");
MODULE_DESCRIPTION("Amlogic freeze suspend driver");
MODULE_LICENSE("GPL");
