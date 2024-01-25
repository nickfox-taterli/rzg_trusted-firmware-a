/*
 * Copyright (c) 2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PLAT_TZC_DEF_H
#define PLAT_TZC_DEF_H

/* Platform specific TrsutZone setup constants */

#define PLAT_TZC_REGION_ACCESS_ID_MASK	U(0x3)
#define PLAT_TZC_REGION_ACCESS_RDWR(nsaid)	\
	(TZC_REGION_ACCESS_RDWR(nsaid & PLAT_TZC_REGION_ACCESS_ID_MASK))

#define PLAT_TZC_REGION_ACCESS_S_PRIV		\
	(PLAT_TZC_REGION_ACCESS_RDWR(0x1))
#define PLAT_TZC_REGION_ACCESS_S_UNPRIV		\
	(PLAT_TZC_REGION_ACCESS_RDWR(0x0) | PLAT_TZC_REGION_ACCESS_S_PRIV)
#define PLAT_TZC_REGION_ACCESS_NS_PRIV		\
	(PLAT_TZC_REGION_ACCESS_RDWR(0x3) | PLAT_TZC_REGION_ACCESS_S_UNPRIV)
#define PLAT_TZC_REGION_ACCESS_NS_UNPRIV	\
	(PLAT_TZC_REGION_ACCESS_RDWR(0x2) | PLAT_TZC_REGION_ACCESS_NS_PRIV)

/* AP shared secure SRAM1 0x00010000 - 0x0001FFFF */
#define PLAT_AP_TZC_PROT_SRAM1_BASE		(0x00010000)
#define PLAT_AP_TZC_PROT_SRAM1_SIZE		(0x00010000)
#define PLAT_AP_TZC_PROT_SRAM1_END		\
	(PLAT_AP_TZC_PROT_SRAM1_BASE + PLAT_AP_TZC_PROT_SRAM1_SIZE - 1)

/* AP shared secure SRAM2 0x00020000 - 0x0002FFFF */
#define PLAT_AP_TZC_PROT_SRAM2_BASE		(0x00020000)
#define PLAT_AP_TZC_PROT_SRAM2_SIZE		(0x00010000)
#define PLAT_AP_TZC_PROT_SRAM2_END		\
	(PLAT_AP_TZC_PROT_SRAM2_BASE + PLAT_AP_TZC_PROT_SRAM2_SIZE - 1)

/* Trusted Firmware(BL31) secure DRAM 0x43F00000 - 0x440FFFFF */
#define PLAT_FW_TZC_PROT_DRAM1_BASE		(0x43F00000)
#define PLAT_FW_TZC_PROT_DRAM1_SIZE		(0x00200000)
#define PLAT_FW_TZC_PROT_DRAM1_END		\
	(PLAT_FW_TZC_PROT_DRAM1_BASE + PLAT_FW_TZC_PROT_DRAM1_SIZE - 1)

/* OP-TEE secure DRAM 0x44100000 - 0x47DFFFFF */
#define PLAT_TEE_TZC_PROT_DRAM1_BASE	(0x44100000)
#define PLAT_TEE_TZC_PROT_DRAM1_SIZE	(0x03D00000)
#define PLAT_TEE_TZC_PROT_DRAM1_END		\
	(PLAT_TEE_TZC_PROT_DRAM1_BASE + PLAT_TEE_TZC_PROT_DRAM1_SIZE - 1)

void plat_security_setup(void);

#endif /* PLAT_TZC_DEF_H */
