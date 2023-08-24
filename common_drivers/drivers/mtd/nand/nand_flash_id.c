// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Copyright (c) 2019 Amlogic, Inc. All rights reserved.
 */

#include <linux/sizes.h>
#include "../../../../drivers/mtd/nand/raw/internals.h"
#include <linux/amlogic/aml_mtd_nand.h>

#define NAND_MFR_DOSILICON	0xf8
#define NAND_MFR_ZETTA		0xba
struct nand_flash_dev aml_nand_flash_ids[] = {
	{"A revision NAND 2GiB H27UAG8T2A",
		{ .id = {NAND_MFR_HYNIX, 0xd5, 0x94, 0x25, 0x44, 0x41}},
		4096,
		2048,
		0x80000,
		0,/*options*/
		6,/*id_length*/
		224,
		{0, 0},
		},

	{"A revision NAND 4GiB H27UBG8T2A",
		{ .id = {NAND_MFR_HYNIX, 0xd7, 0x94, 0x9a, 0x74, 0x42}},
		8192,
		4096,
		0x200000,
		0,
		6,
		448,
		{0, 0},
		},

	{"B revision NAND 2GiB H27UAG8T2B",
		{ .id = {NAND_MFR_HYNIX, 0xd5, 0x94, 0x9a, 0x74, 0x42}},
		8192,
		2048,
		0x200000,
		0,
		6,
		448,
		{0, 0},
		},

	{"B revision 26nm NAND 4GiB H27UBG8T2B",
		{ .id = {NAND_MFR_HYNIX, 0xd7, 0x94, 0xda, 0x74, 0xc3}},
		8192,
		4096,
		0x200000,
		0,
		6,
		640,
		{0, 0},
		},

	{"B revision 26nm NAND 8GiB H27UCG8T2M",
		{ .id = {NAND_MFR_HYNIX, 0xde, 0x94, 0xd2, 0x04, 0x43}},
		8192,
		8192,
		0x200000,
		0,
		6,
		448,
		{0, 0},
		},

	{"C revision 20nm NAND 4GiB H27UBG8T2C",
		{ .id = {NAND_MFR_HYNIX, 0xd7, 0x94, 0x91, 0x60, 0x44}},
		8192,
		4096,
		0x200000,
		0,
		6,
		640,
		{0, 0},
		},

	{"A revision 20nm NAND 8GiB H27UCG8T2A",
		{ .id = {NAND_MFR_HYNIX, 0xde, 0x94, 0xda, 0x74, 0xc4}},
		8192,
		8192,
		0x200000,
		0,
		6,
		640,
		{0, 0},
		},

	{"B revision 20nm NAND 8GiB H27UCG8T2B",
		{ .id = {NAND_MFR_HYNIX, 0xde, 0x94, 0xeb, 0x74, 0x44}},
		16384,
		8192,
		0x400000,
		0,
		6,
		1280,
		{0, 0},
		},

	{"E revision 1ynm NAND 8GiB H27UCG8T2E",
		{ .id = {NAND_MFR_HYNIX, 0xde, 0x14, 0xa7, 0x42, 0x4a}},
		16384,
		8192,
		0x400000,
		0,
		6,
		1664,
		{0, 0},
		},

	{"B revision NAND 8GiB MT29F64G08CBABA",
		{ .id = {NAND_MFR_MICRON, 0x64, 0x44, 0x4B, 0xA9}},
		8192,
		8192,
		0x200000,
		0,
		5,
		744,
		{0, 0},
		},

	{"D revision NAND 4GiB MT29F32G08CBADA",
		{ .id = {NAND_MFR_MICRON, 0x44, 0x44, 0x4B, 0xA9}},
		8192,
		4096,
		0x200000,
		0,
		5,
		744,
		{0, 0},
		},

	{"1 Generation NAND 8GiB JS29F64G08ACMF1",
		{ .id = {NAND_MFR_INTEL, 0x88, 0x24, 0x4b, 0xA9, 0x84}},
		8192,
		8192,
		0x200000,
		0,
		6,
		448,
		{0, 0},
		},

	{"SamSung NAND K9F4G08U0F 4Gb",
		{ .id = {NAND_MFR_SAMSUNG, 0xdc, 0x10, 0x95, 0x56}},
		2048,
		512,
		0x20000,
		0,
		5,
		64,
		{0, 0},
		},

	{"SamSung NAND k9f2g08u0d 2Gb",
		{ .id = {NAND_MFR_SAMSUNG, 0xda, 0x10, 0x95, 0x46}},
		2048,
		256,
		0x20000,
		0,
		5,
		64,
		{0, 0},
		},

	{"Dosilicon NAND FMND1GXXX3D 3.3V 1Gb",
		{ .id = {NAND_MFR_DOSILICON, 0xf1, 0x80, 0x95}},
		2048,
		128,
		0x20000,
		0,
		4,
		64,
		{0, 0},
		},

	{"Dosilicon NAND FMND1GXXX3D 1.8V 1Gb",
		{ .id = {NAND_MFR_DOSILICON, 0xa1, 0x80, 0x15}},
		2048,
		128,
		0x20000,
		0,
		4,
		64,
		{0, 0},
		},

	{"Dosilicon NAND FMND2GXXX3D 3.3V 2Gb",
		{ .id = {NAND_MFR_DOSILICON, 0xda, 0x90, 0x95, 0x46}},
		2048,
		256,
		0x20000,
		0,
		5,
		64,
		{0, 0},
		},

	{"Dosilicon NAND FMND2GXXX3D 1.8V 2Gb",
		{ .id = {NAND_MFR_DOSILICON, 0xaa, 0x90, 0x15, 0x46}},
		2048,
		256,
		0x20000,
		0,
		5,
		64,
		{0, 0},
		},

	{"ATO NAND AFND1G08U3 1Gb",
		{ .id = {NAND_MFR_ATO, 0xf1, 0x00, 0x1d}},
		2048,
		128,
		0x20000,
		0,
		4,
		64,
		{0, 0},
		},

	{"ATO NAND AFND2G08U3A 2Gb",
		{ .id = {NAND_MFR_HYNIX, 0xda, 0x90, 0x95, 0x46, 0xad}},
		2048,
		256,
		0x20000,
		0,
		6,
		64,
		{0, 0},
		},

	{"Zetta NAND ZDND2GXXXXX 2Gb",
		{ .id = {NAND_MFR_ZETTA, 0xda, 0x90, 0x95, 0x46}},
		2048,
		256,
		0x20000,
		0,
		5,
		64,
		{0, 0},
		},

	{"Zetta NAND ZDND1G08U3D-xx 1Gb",
		{ .id = {NAND_MFR_ZETTA, 0xf1, 0x80, 0x95}},
		2048,
		128,
		0x20000,
		0,
		4,
		64,
		{0, 0},
		},

	{"A revision NAND 1GiB sF1G-A",
		{ .id = {NAND_MFR_AMD, 0xf1, 0x80, 0x1d, 0x01, 0xf1}},
		2048,
		128,
		0x20000,
		0,
		6,
		64,
		{0, 0},
		},

	{"A revision NAND 1GiB sF1G-A",
		{ .id = {NAND_MFR_AMD, 0xf1, 0x0, 0x1d, 0x01, 0xf1}},
		2048,
		128,
		0x20000,
		0,
		6,
		64,
		{0, 0},
		},

	{"AMD/Spansion Slc NAND 2Gib S34ML02G1(MLO2G100BH1OO)",
		{ .id = {NAND_MFR_AMD, 0xda, 0x90, 0x95, 0x44, 0x01}},
		2048,
		256,
		0x20000,
		0,
		6,
		64,
		{0, 0},
		},

	{"AMD/Spansion Slc NAND 2Gib S34ML02G1(MLO2G200BH1OO)",
		{ .id = {NAND_MFR_AMD, 0xda, 0x90, 0x95, 0x46, 0x01}},
		2048,
		256,
		0x20000,
		0,
		6,
		64,
		{0, 0},
		},

	{"AMD/Spansion Slc NAND 8Gib S34ML08G1",
		{ .id = {NAND_MFR_AMD, 0xd3, 0xd1, 0x95, 0x58}},
		2048,
		1024,
		0x20000,
		0,
		5,
		64,
		{0, 0},
		},

	{"SkyHigh Slc NAND 4Gib S34ML04G3",
		{ .id = {NAND_MFR_AMD, 0xdc, 0x0, 0x1a, 0x0}},
		4096,
		512,
		0x40000,
		0,
		5,
		256,
		{0, 0},
		},

	{"A revision NAND 1Gib W29N01HV ",
		{ .id = {NAND_MFR_WINBOND, 0xf1, 0x00, 0x95, 0x00, 0x00}},
		2048,
		128,
		0x20000,
		0,
		6,
		64,
		{0, 0},
		},

	{"A revision NAND 4Gib W29N04GV ",
		{ .id = {NAND_MFR_WINBOND, 0xdc, 0x90, 0x95, 0x54, 0x00}},
		2048,
		512,
		0x20000,
		0,
		6,
		64,
		{0, 0},
		},

	{"A revision NAND 1Gib W29N01GV ",
		{ .id = {NAND_MFR_WINBOND, 0xf1, 0x80, 0x95, 0x00, 0x00}},
		2048,
		128,
		0x20000,
		0,
		6,
		64,
		{0, 0},
		},

	{"A revision NAND 2Gib W29N02GV ",
		{ .id = {NAND_MFR_WINBOND, 0xda, 0x90, 0x95, 0x04, 0x00}},
		2048,
		256,
		0x20000,
		0,
		6,
		64,
		{0, 0},
		},

	{"A revision NAND 1GiB H27U1G8F2CTR ",
		{ .id = {NAND_MFR_HYNIX, 0xf1, 0x80, 0x1d, 0xad, 0xf1}},
		2048,
		128,
		0x20000,
		0,
		6,
		64,
		{0, 0},
		},

	{"A revision NAND 4Gib EMST ",
		{ .id = {NAND_MFR_ESMT, 0xac, 0x90, 0x15, 0x54, 0x7f}},
		2048,
		512,
		0x20000,
		0,
		6,
		64,
		{0, 0},
		},

	{"A revision NAND 4Gib GD9FU1G8F2AMGI",
		{ .id = {NAND_MFR_ESMT, 0xf1, 0x80, 0x1d, 0x42, 0xc8}},
		2048,
		128,
		0x20000,
		0,
		6,
		128,
		{0, 0},
		},

	{"ESMT SLC 128MiB 3.3V 8-bit F59L1G81Lxxx",
		{ .id = {NAND_MFR_ESMT, 0xd1, 0x80, 0x95, 0x42, 0x7f}},
		2048,
		128,
		0x20000,
		0,
		6,
		64,
		{0, 0},
		},

	{"ESMT SLC 128MiB 3.3V 8-bit F59L1G81Mxxx",
		{ .id = {NAND_MFR_ESMT, 0xd1, 0x80, 0x95, 0x40, 0x7f}},
		2048,
		128,
		0x20000,
		0,
		6,
		64,
		{0, 0},
		},

	{"ESMT SLC 256MiB 3.3V 8-bit",
		{ .id = {NAND_MFR_ESMT, 0xda, 0x90, 0x95, 0x44, 0x7f}},
		2048,
		256,
		0x20000,
		0,
		6,
		64,
		{0, 0},
		},

	{"GigaDevice SLC 256MiB 8-bit GD9FU2G8F2A",
		{ .id = {NAND_MFR_ESMT, 0xda, 0x90, 0x95, 0x46}},
		2048,
		256,
		0x20000,
		0,
		5,
		128,
		{0, 0},
		},

	{"ESMT SLC 512MiB 3.3V 8-bit F59L4G81A",
		{ .id = {NAND_MFR_ESMT, 0xdc, 0x90, 0x95, 0x54}},
		2048,
		512,
		0x20000,
		0,
		5,
		64,
		{0, 0},
		},

	{"A revision NAND 2Gib TC58BVG1S3HTA00 ",
		{ .id = {NAND_MFR_TOSHIBA, 0xda, 0x90, 0x15, 0xF6}},
		2048,
		256,
		0x20000,
		0,
		5,
		64,
		{0, 0},
		},

	{"A revision NAND 2Gib TC58NVG1S3HBAI4 ",
		{ .id = {NAND_MFR_TOSHIBA, 0xda, 0x90, 0x15, 0x76}},
		2048,
		256,
		0x20000,
		0,
		5,
		64,
		{0, 0},
		},

	{"A revision NAND 2Gib TC58BVG0S3HTA00 ",
		{ .id = {NAND_MFR_TOSHIBA, 0xf1, 0x80, 0x15, 0xf2}},
		2048,
		128,
		0x20000,
		0,
		5,
		64,
		{0, 0},
		},

	{"A revision NAND 4Gib TH58NVG2S3HTA00 ",
		{ .id = {NAND_MFR_TOSHIBA, 0xdc, 0x91, 0x15, 0x76}},
		2048,
		512,
		0x20000,
		0,
		5,
		64,
		{0, 0},
		},

	{"A revision NAND 4Gib TC58NVG2S0HTA00/F59L4G81CA(2L) ",
		{ .id = {NAND_MFR_TOSHIBA, 0xdc, 0x90, 0x26, 0x76}},
		4096,
		512,
		0x40000,
		0,
		5,
		256,
		{0, 0},
		},

	{"A revision NAND 4Gib TC58BVG2S0HTA00 ",
		{ .id = {NAND_MFR_TOSHIBA, 0xdc, 0x90, 0x26, 0xF6}},
		4096,
		512,
		0x40000,
		0,
		5,
		128,
		{0, 0},
		},

	{"Slc NAND 1Gib MX30LF1G18AC ",
		{ .id = {NAND_MFR_MACRONIX, 0xf1, 0x80, 0x95, 0x02}},
		2048,
		128,
		0x20000,
		0,
		5,
		64,
		{0, 0},
		},

	{"Slc NAND 2Gib MX30LF2G18AC ",
		{ .id = {NAND_MFR_MACRONIX, 0xda, 0x90, 0x95, 0x06}},
		2048,
		256,
		0x20000,
		0,
		5,
		64,
		{0, 0},
		},

	{"Slc NAND 4Gib MX30LF4G18AC ",
		{ .id = {NAND_MFR_MACRONIX, 0xdc, 0x90, 0x95, 0x56}},
		2048,
		512,
		0x20000,
		0,
		5,
		64,
		{0, 0},
		},

	{"Slc NAND 4Gib MX30LF4G28AD",
		{ .id = {NAND_MFR_MACRONIX, 0xdc, 0x90, 0xA2, 0x57, 0x03}},
		4096,
		512,
		0x40000,
		0,
		6,
		256,
		{0, 0},
		},

	{"A revision NAND 128MB TC58NVG0S3HTA00 ",
		{ .id = {NAND_MFR_TOSHIBA, 0xf1, 0x80, 0x15, 0x72}},
		2048,
		128,
		0x20000,
		0,
		5,
		64,
		{0, 0},
		},

	{"4Gib MT29F4G08ABAEA",
		{ .id = {NAND_MFR_MICRON, 0xdc, 0x90, 0xA6, 0x54}},
		4096,
		512,
		0x40000,
		0,
		5,
		224,
		{0, 0},
		},

	{"4Gib MT29F4G08ABAFAWP",
		{ .id = {NAND_MFR_MICRON, 0xdc, 0x80, 0xA6, 0x62}},
		4096,
		512,
		0x40000,
		0,
		5,
		256,
		{0, 0},
		},

	{"4Gib MT29F4G08ABADA",
		{ .id = {NAND_MFR_MICRON, 0xdc, 0x90, 0x95, 0x56}},
		2048,
		512,
		0x20000,
		0,
		5,
		64,
		{0, 0},
		},

	{"A revision NAND 2Gib MT29F2G08-A",
		{ .id = {NAND_MFR_MICRON, 0xda, 0x90, 0x95, 0x06}},
		2048,
		256,
		0x20000,
		0,
		5,
		64,
		{0, 0},
		},

	{"A revision NAND 1Gib MT29F1G-A",
		{ .id = {NAND_MFR_MICRON, 0xf1, 0x80, 0x95, 0x04}},
		2048,
		128,
		0x20000,
		0,
		5,
		64,
		{0, 0},
		},

	{"A revision NAND 4GiB MT29F32G-A",
		{ .id = {NAND_MFR_MICRON, 0xd7, 0x94, 0x3e, 0x84}},
		4096,
		4096,
		0x80000,
		0,
		5,
		218,
		{0, 0},
		},

	{"A revision NAND 16GiB MT29F128G-A",
		{ .id = {NAND_MFR_MICRON, 0xd9, 0xd5, 0x3e, 0x88}},
		4096,
		16384,
		0x80000,
		0,
		5,
		218,
		{0, 0},
		},

	{"B revision NAND 4GiB MT29F32G-B",
		{ .id = {NAND_MFR_MICRON, 0x68, 0x04, 0x46, 0x89}},
		4096,
		4096,
		0x100000,
		0,
		5,
		224,
		{0, 0},
		},

	{"B revision NAND 8GiB MT29F64G-B",
		{ .id = {NAND_MFR_MICRON, 0x88, 0x05, 0xc6, 0x89}},
		4096,
		8192,
		0x100000,
		0,
		5,
		224,
		{0, 0},
		},

	{"C revision NAND 4GiB MT29F32G-C",
		{ .id = {NAND_MFR_MICRON, 0x68, 0x04, 0x4a, 0xa9}},
		4096,
		4096,
		0x100000,
		0,
		5,
		224,
		{0, 0},
		},

	{"C revision NAND 8GiB MT29F64G-C",
		{ .id = {NAND_MFR_MICRON, 0x88, 0x04, 0x4b, 0xa9}},
		8192,
		8192,
		0x200000,
		0,
		5,
		448,
		{0, 0},
		},

	{"C revision NAND 1GiB MT29F8G08ABABA",
		{ .id = {NAND_MFR_MICRON, 0x38, 0x00, 0x26, 0x85}},
		4096,
		1024,
		0x80000,
		0,
		5,
		224,
		{0, 0},
		},

	{"C revision NAND 32GiB MT29F256G-C",
		{ .id = {NAND_MFR_MICRON, 0xa8, 0x05, 0xcb, 0xa9}},
		8192,
		16384,
		0x200000,
		0,
		5,
		448,
		{0, 0},
		},

	{"1 Generation NAND 4GiB JS29F32G08AA-1",
		{ .id = {NAND_MFR_INTEL, 0x68, 0x04, 0x46, 0xA9}},
		4096,
		4096,
		0x100000,
		0,
		5,
		218,
		{0, 0},
		},

	{"1 Generation NAND 8GiB JS29F64G08AA-1",
		{ .id = {NAND_MFR_INTEL, 0x88, 0x24, 0x4b, 0xA9}},
		8192,
		8192,
		0x200000,
		0,
		5,
		448,
		{0, 0},
		},

	{"E serials NAND 2GiB TC58NVG4D2ETA00",
		{ .id = {NAND_MFR_TOSHIBA, 0xD5, 0x94, 0x32, 0x76, 0x54}},
		8192,
		2048,
		0x100000,
		0,
		6,
		376,
		{0, 0},
		},

	{"E serials NAND 4GiB TC58NVG5D2ETA00",
		{ .id = {NAND_MFR_TOSHIBA, 0xD7, 0x94, 0x32, 0x76, 0x54}},
		8192,
		4096,
		0x100000,
		0,
		6,
		376,
		{0, 0},
		},

	{"F serials NAND 2GiB TC58NVG4D2FTA00",
		{ .id = {NAND_MFR_TOSHIBA, 0xD5, 0x94, 0x32, 0x76, 0x55}},
		8192,
		2076,
		0x100000,
		0,
		6,
		448,
		{0, 0},
		},

	{"F serials NAND 4GiB TC58NVG5D2FTA00",
		{ .id = {NAND_MFR_TOSHIBA, 0xD7, 0x94, 0x32, 0x76, 0x55}},
		8192,
		4096,
		0x100000,
		0,
		6,
		448,
		{0, 0},
		},

	{"F serials NAND 8GiB TC58NVG6D2FTA00",
		{ .id = {NAND_MFR_TOSHIBA, 0xDE, 0x94, 0x32, 0x76, 0x55}},
		8192,
		8192,
		0x100000,
		0,
		6,
		448,
		{0, 0},
		},

	{"F serials NAND 8GiB TH58NVG7D2FTA20",
		{ .id = {NAND_MFR_TOSHIBA, 0xDE, 0x95, 0x32, 0x7a, 0x55}},
		8192,
		8200,
		0x100000,
		0,
		6,
		448,
		{0, 0},
		},

	{"F serials NAND 4GiB TC58NVG5D2HTA00",
		{ .id = {NAND_MFR_TOSHIBA, 0xD7, 0x94, 0x32, 0x76, 0x56}},
		8192,
		4096,
		0x100000,
		0,
		6,
		640,
		{0, 0},
		},

	{"F serials NAND 8GiB TC58NVG6D2GTA00",
		{ .id = {NAND_MFR_TOSHIBA, 0xDE, 0x94, 0x82, 0x76, 0x56}},
		8192,
		8192,
		0x200000,
		0,
		6,
		640,
		{0, 0},
		},

	{"F serials NAND 8GiB TC58TEG6DCJTA00",
		{ .id = {NAND_MFR_TOSHIBA, 0xDE, 0x84, 0x93, 0x72, 0x57}},
		16384,
		8192,
		0x400000,
		0,
		6,
		1280,
		{0, 0},
		},

	{"A serials NAND 4GiB TC58TEG5DCJTA00 ",
		{ .id = {NAND_MFR_TOSHIBA, 0xD7, 0x84, 0x93, 0x72, 0x57}},
		16384,
		4096,
		0x400000,
		0,
		6,
		1280,
		{0, 0},
		},

	{"A serials NAND 8GiB TC58TEG6DDKTA00 ",
		{ .id = {NAND_MFR_TOSHIBA, 0xDE, 0x94, 0x93, 0x76, 0x50}},
		16384,
		8192,
		0x400000,
		0,
		6,
		1280,
		{0, 0},
		},

	{"A serials NAND 16GiB TC58TEG7DCJTA00 ",
		{ .id = {NAND_MFR_TOSHIBA, 0xa3, 0x85, 0x93, 0x76, 0x57}},
		16384,
		16384,
		0x400000,
		0,
		6,
		1280,
		{0, 0},
		},

	{"A serials NAND 8GiB SDTNQGAMA-008G ",
		{ .id = {NAND_MFR_SANDISK, 0xDE, 0x94, 0x93, 0x76, 0x57}},
		16384,
		8192,
		0x400000,
		0,
		6,
		1280,
		{0, 0},
		},

	{"A serials NAND 4GiB SDTNQGAMA-004G ",
		{ .id = {NAND_MFR_SANDISK, 0xD7, 0x84, 0x93, 0x72, 0x57}},
		16384,
		4096,
		0x400000,
		0,
		6,
		1280,
		{0, 0},
		},

	{"A serials NAND 8GiB SDTNPMAHEM-008G ",
		{ .id = {NAND_MFR_SANDISK, 0xDE, 0xA4, 0x82, 0x76, 0x56}},
		8192,
		8192,
		0x200000,
		0,
		6,
		640,
		{0, 0},
		},

	{"A serials NAND 8GiB SDTNRGAMA-008G ",
		{ .id = {NAND_MFR_SANDISK, 0xDE, 0x94, 0x93, 0x76, 0x50}},
		16384,
		8192,
		0x400000,
		0,
		6,
		1280,
		{0, 0},
		},

	{"M Generation NAND 4Gib K9F4G08U0D",
		{ .id = {NAND_MFR_SAMSUNG, 0xDC, 0x10, 0x95, 0x54, 0XEC}},
		2048,
		512,
		0x20000,
		0,
		6,
		64,
		{0, 0},
		},

	{"M Generation NAND 1Gib FS33ND01GS108TFI0",
		{ .id = {NAND_MFR_SAMSUNG, 0xF1, 0x00, 0x95, 0x42}},
		2048,
		128,
		0x20000,
		0,
		5,
		64,
		{0, 0},
		},

	{"M Generation NAND 2GiB K9GAG08U0M",
		{ .id = {NAND_MFR_SAMSUNG, 0xD5, 0x14, 0xb6, 0x74}},
		4096,
		2048,
		0x80000,
		0,
		5,
		128,
		{0, 0},
		},

	{"5 Generation NAND 2GiB K9GAG08X0D",
		{ .id = {NAND_MFR_SAMSUNG, 0xD5, 0x94, 0x29, 0x34, 0x41}},
		4096,
		2048,
		0x80000,
		0,
		6,
		218,
		{0, 0},
		},

	{"6 Generation NAND 2GiB K9GAG08U0E",
		{ .id = {NAND_MFR_SAMSUNG, 0xD5, 0x84, 0x72, 0x50, 0x42}},
		8192,
		2048,
		0x100000,
		0,
		6,
		436,
		{0, 0},
		},

	{"7 Generation NAND 2GiB K9GAG08U0F",
		{ .id = {NAND_MFR_SAMSUNG, 0xD5, 0x94, 0x76, 0x54, 0x43}},
		8192,
		2048,
		0x100000,
		0,
		6,
		512,
		{0, 0},
		},

	{"6 Generation NAND 4GiB K9LBG08U0E",
		{ .id = {NAND_MFR_SAMSUNG, 0xD7, 0xC5, 0x72, 0x54, 0x42}},
		8192,
		4096,
		0x100000,
		0,
		6,
		436,
		{0, 0},
		},

	{"6 Generation NAND 8GiB K9HCG08U0E",
		{ .id = {NAND_MFR_SAMSUNG, 0xDE, 0xC5, 0x72, 0x54, 0x42}},
		8192,
		8192,
		0x100000,
		0,
		6,
		436,
		{0, 0},
		},

	{"2 Generation NAND 4GiB K9GBG08U0A",
		{ .id = {NAND_MFR_SAMSUNG, 0xD7, 0x94, 0x7a, 0x54, 0x43}},
		8192,
		4152,
		0x100000,
		0,
		6,
		640,
		{0, 0},
		},

	{"2 Generation NAND 8GiB K9LCG08U0A",
		{ .id = {NAND_MFR_SAMSUNG, 0xDE, 0xD5, 0x7a, 0x58, 0x43}},
		8192,
		8304,
		0x100000,
		0,
		6,
		640,
		{0, 0},
		},

	{"2 Generation NAND 4GiB K9GBG08U0B",
		{ .id = {NAND_MFR_SAMSUNG, 0xD7, 0x94, 0x7e, 0x64, 0x44}},
		8192,
		4096,
		0x100000,
		0,
		6,
		640,
		{0, 0},
		},

	{"2 Generation NAND 8GiB K9LCG08U0B",
		{ .id = {NAND_MFR_SAMSUNG, 0xDE, 0xD5, 0x7e, 0x68, 0x44}},
		8192,
		8192,
		0x100000,
		0,
		6,
		640,
		{0, 0},
		},

	{NULL,}
};
