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
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/unistd.h>
#include <linux/mutex.h>

#include <linux/amlogic/major.h>
static struct mutex ssc_lock;

#define  DDR0_CNTL1			0xfc00e004
#define  DDR1_CNTL1			0xfb00e004
#define  DDR0_CNTL2			0xfc00e008
#define  DDR1_CNTL2			0xfb00e008

static ssize_t ddr_ssc_show(struct class *cla, struct class_attribute *attr, char *buf)
{
	void __iomem *vaddr;
	unsigned int regs[4] = {0};

	mutex_lock(&ssc_lock);
	vaddr = ioremap((DDR0_CNTL1), 0x4);
	regs[0] = readl(vaddr);
	iounmap(vaddr);
	vaddr = ioremap((DDR1_CNTL1), 0x4);
	regs[1] = readl(vaddr);
	iounmap(vaddr);
	vaddr = ioremap((DDR0_CNTL2), 0x4);
	regs[2] = readl(vaddr);
	iounmap(vaddr);
	vaddr = ioremap((DDR1_CNTL2), 0x4);
	regs[3] = readl(vaddr);
	iounmap(vaddr);
	mutex_unlock(&ssc_lock);

	return sprintf(buf, "0x%x, 0x%x, 0x%x, 0x%x\n", regs[0], regs[1], regs[2], regs[3]);
}

static ssize_t ddr_ssc_store(struct class *cla,
		struct class_attribute *attr, const char *buf, size_t count)
{
	void __iomem *vaddr;
	unsigned int ddr0_ctrl1, ddr1_ctrl1, ddr0_ctrl2, ddr1_ctrl2;
	int ret = 0;

	ret = sscanf(buf, "%x,%x,%x,%x", &ddr0_ctrl1, &ddr1_ctrl1, &ddr0_ctrl2, &ddr1_ctrl2);
	pr_info("wr ssc : 0x%x, 0x%x, 0x%x, 0x%x\n",
			ddr0_ctrl1, ddr1_ctrl1, ddr0_ctrl2, ddr1_ctrl2);

	mutex_lock(&ssc_lock);
	vaddr = ioremap((DDR0_CNTL1), 0x4);
	writel(ddr0_ctrl1, vaddr);
	iounmap(vaddr);
	vaddr = ioremap((DDR1_CNTL1), 0x4);
	writel(ddr1_ctrl1, vaddr);
	iounmap(vaddr);
	vaddr = ioremap((DDR0_CNTL2), 0x4);
	writel(ddr0_ctrl2, vaddr);
	iounmap(vaddr);
	vaddr = ioremap((DDR1_CNTL2), 0x4);
	writel(ddr1_ctrl2, vaddr);
	iounmap(vaddr);
	mutex_unlock(&ssc_lock);
	return strnlen(buf, count);
}

static CLASS_ATTR_RW(ddr_ssc);

static struct attribute *ssc_class_attrs[] = {
	&class_attr_ddr_ssc.attr,
	NULL
};

ATTRIBUTE_GROUPS(ssc_class);

static struct class ssc_class = {
	.name = "ssc",
	.owner = THIS_MODULE,
	.class_groups = ssc_class_groups,
};

static int __init aml_ssc_module_init(void)
{
	int ret = 0;

	ret = class_register(&ssc_class);
	if (ret < 0)
		pr_err("Create ssc class failed\n");
	return 0;
}

static void __exit aml_ssc_module_exit(void)
{
	class_unregister(&ssc_class);
}

module_init(aml_ssc_module_init);
module_exit(aml_ssc_module_exit);

MODULE_AUTHOR("AMLOGIC");
MODULE_DESCRIPTION("AMLOGIC SSC");
MODULE_LICENSE("GPL");

