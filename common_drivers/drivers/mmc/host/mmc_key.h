/* SPDX-License-Identifier: (GPL-2.0+ OR MIT) */
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#ifndef _MMC_KEY_H_
#define _MMC_KEY_H_

#include <linux/mmc/core.h>
#include <linux/mmc/card.h>
#include <linux/mmc/host.h>
#include <linux/mmc/mmc.h>

#define EMMC_KEY_AREA_SIGNAL		"emmckeys"
#define EMMC_KEY_AREA_SIGNAL_LEN	16

#define EMMC_KEYAREA_SIZE		(256 * 1024)
#define EMMC_KEYAREA_COUNT		2

/* we store partition table in the previous 16KB space */
#define EMMCKEY_RESERVE_OFFSET          0x4000
#define EMMCKEY_AREA_PHY_SIZE           (EMMC_KEYAREA_COUNT * EMMC_KEYAREA_SIZE)
struct emmckey_valid_node_t {
	u64 phy_addr;
	u64 phy_size;
	struct emmckey_valid_node_t *next;
};

struct aml_emmckey_info_t {
	/* struct memory_card *card; */
	struct	emmckey_valid_node_t *key_valid_node;
	u64	keyarea_phy_addr;
	u64	keyarea_phy_size;
	u64	lba_start;
	u64	lba_end;
	u32	blk_size;
	u32	blk_shift;
	u8	key_init;
	u8	key_valid;
	u8	key_part_count;
};

struct aml_key_info {
	u64 checksum;
	u32 stamp;
	u32 magic;
};

#define EMMCKEY_DATA_VALID_LEN		\
	(EMMC_KEYAREA_SIZE - EMMC_KEY_AREA_SIGNAL_LEN - 4 - 4 - 4)

struct emmckey_data_t {
	u8	keyarea_mark[EMMC_KEY_AREA_SIGNAL_LEN];
	u32	keyarea_mark_checksum;
	u32	checksum;
	u32	reserve;
	u8	data[EMMCKEY_DATA_VALID_LEN];
};

int32_t emmc_key_read(u8 *buffer,
		      u32 length, u32 *actual_length);
int32_t emmc_key_write(u8 *buffer,
		       u32 length, u32 *actual_length);

void register_key_dtb(void);

void emmc_key_init(struct mmc_card *card, int *retp);

extern struct task_struct      *thread_dtb_key_task;

#endif

