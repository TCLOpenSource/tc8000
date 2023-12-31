/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef __LINUX_AMLOGIC_ION_H__
#define __LINUX_AMLOGIC_ION_H__

#include <linux/types.h>
#include <linux/amlogic/ion.h>

/**
 * CUSTOM IOCTL - CMD
 */

#define DION_ERROR(fmt, args ...)	pr_err("ion_dev: " fmt, ## args)
#define DION_INFO(fmt, args ...)	pr_info("ion_dev: " fmt, ## args)
#define DION_DEBUG(fmt, args ...)	pr_debug("ion_dev: " fmt, ## args)

#define ION_IOC_MESON_PHYS_ADDR             8
#define ION_FLAG_EXTEND_MESON_HEAP_SECURE   BIT(29)
#define ION_FLAG_EXTEND_MESON_HEAP          BIT(30)
#define ION_FLAG_EXTEND_PROTECTED           BIT(31)
#define SECURE_MEM_DELAY_PROTECT 1

struct ion_cma_heap {
	struct ion_heap heap;
	struct cma *cma;

	bool is_added;
	unsigned long max_can_alloc_size;
	unsigned long alloced_size;
	/* protect size account */
	struct mutex mutex;
};

struct meson_phys_data {
	int handle;
	unsigned int phys_addr;
	unsigned int size;
};

/**
 * meson_ion_share_fd_to_phys -
 * associate with a fd
 * @fd: passed from the user space
 * @addr point to the physical address
 * @size point to the size of this ion buffer
 */

int meson_ion_share_fd_to_phys(int fd, phys_addr_t *addr, size_t *len);
void meson_ion_buffer_to_phys(struct ion_buffer *buffer,
			      phys_addr_t *addr, size_t *len);
unsigned int meson_ion_codecmm_heap_id_get(void);
unsigned int meson_ion_cma_heap_id_get(void);
unsigned int meson_ion_fb_heap_id_get(void);

extern struct ion_heap_ops codec_mm_heap_ops;
extern struct ion_heap_ops ion_cma_ops;

#endif
