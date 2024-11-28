/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stddef.h>
#include <pfc_regs.h>
#include <sys_regs.h>
#include <sys.h>
#include <lib/mmio.h>

#define SDO_PWR_EN_PR_PIN	P_P1
#define SDO_PWR_SEL_PR_PIN	P_P2
#define SD1_PWR_EN_PR_PIN	P_P3
#define SD1_PWR_SEL_PR_PIN	P_P2

static PFC_REGS pfc_mux_sd_reg_tbl[PFC_MUX_SD_TBL_NUM] = {
	/* P0(SD0/SD1) CD, WP*/
	{
		{ PFC_ON,	(uintptr_t)PFC_PMC20,	0x05 },					/* PMC */
		{ PFC_ON,	(uintptr_t)PFC_PFC20,	0x00000000 },			/* PFC */
		{ PFC_ON,	(uintptr_t)PFC_IOLH20,	0x0000000000030003 },	/* IOLH */
		{ PFC_ON,	(uintptr_t)PFC_PUPD20,	0x0000000000000000 },	/* PUPD */
		{ PFC_OFF,	(uintptr_t)NULL,		0 }						/* IEN */
	}
};

static PFC_REGS pfc_mux_scif_reg_tbl[PFC_MUX_SD_TBL_NUM] = {
	/* P6(scif0) - Tx, Rx*/
	{
		{ PFC_ON,	(uintptr_t)PFC_PMC22,	0x18 },					/* PMC */
		{ PFC_ON,	(uintptr_t)PFC_PFC22,	0x00000000 },			/* PFC */
		{ PFC_ON,	(uintptr_t)PFC_IOLH22,	0x0000000303000000 },	/* IOLH */
		{ PFC_ON,	(uintptr_t)PFC_PUPD22,	0x0000000000000000 },	/* PUPD */
		{ PFC_OFF,	(uintptr_t)NULL,		0 }						/* IEN */
	}
};

static PFC_REGS  pfc_xspi_reg_tbl[PFC_XSPI_TBL_NUM] = {
	/* XSPI  - CS1, CS0, DS, WP, RESET, SPCLK */
	{
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PMC */
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PFC */
		{ PFC_ON,	(uintptr_t)PFC_IOLH04,	0x0000030000000303 },	/* IOLH */
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PUPD */
		{ PFC_OFF,	(uintptr_t)NULL,		0 }						/* IEN */
	},
	/* XSPI - IO3 - IO0*/
	{
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PMC */
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PFC */
		{ PFC_ON,	(uintptr_t)PFC_IOLH05,	0x0000000003030303 },	/* IOLH */
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PUPD */
		{ PFC_OFF,	(uintptr_t)NULL,		0 }						/* IEN */
	}
};

static PFC_REGS  pfc_sd_reg_tbl[PFC_SD_TBL_NUM_SMARC] = {
	/* SD0 RST, CMD, CLK (eMMC) */
	{
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PMC */
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PFC */
		{ PFC_ON,	(uintptr_t)PFC_IOLH10,	0x0000000000030303 },	/* IOLH */
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PUPD */
		{ PFC_ON,	(uintptr_t)PFC_IEN10,	0x0000000000000100 }	/* IEN */
	},
	/* SD0 DATA7 - DATA0 (eMMC) */
	{
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PMC */
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PFC */
		{ PFC_ON,	(uintptr_t)PFC_IOLH11,	0x0303030303030303 },	/* IOLH */
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PUPD */
		{ PFC_ON,	(uintptr_t)PFC_IEN11,	0x0101010101010101 }	/* IEN */
	},
	/* SD1 CMD, CLK (SD Card) */
	{
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PMC */
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PFC */
		{ PFC_ON,	(uintptr_t)PFC_IOLH12,	0x0000000000000303 },	/* IOLH */
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PUPD */
		{ PFC_ON,	(uintptr_t)PFC_IEN12,	0x0000000000000100 }	/* IEN */
	},
	/* SD1 DATA3 - DATA0 (SD Card) */
	{
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PMC */
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PFC */
		{ PFC_ON,	(uintptr_t)PFC_IOLH13,	0x0000000003030303 },	/* IOLH */
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PUPD */
		{ PFC_ON,	(uintptr_t)PFC_IEN13,	0x0000000001010101 }	/* IEN */
	},
	/* SD0 Pwr En, SD0 Pwr Sel, SD1 Pwr En*/
	{
		{ PFC_ON,	(uintptr_t)PFC_PMC31,	0x00 },					/* PMC */
		{ PFC_ON,	(uintptr_t)PFC_PFC31,	0x00000000 },			/* PFC */
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* IOLH */
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PUPD */
		{ PFC_OFF,	(uintptr_t)NULL,		0 }						/* IEN */
	},
	/* SD1 Pwr Sel */
	{
		{ PFC_ON,	(uintptr_t)PFC_PMC33,	0x00 },					/* PMC */
		{ PFC_ON,	(uintptr_t)PFC_PFC33,	0x00000000 },			/* PFC */
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* IOLH */
		{ PFC_OFF,	(uintptr_t)NULL,		0 },					/* PUPD */
		{ PFC_OFF,	(uintptr_t)NULL,		0 }						/* IEN */
	}
};

static void pfc_write_registers(uint8_t tbl_size, PFC_REGS *pfc_reg_tbl)
{
	int cnt;

	for (cnt = 0; cnt < tbl_size; cnt++) {
		/* PMC */
		if (pfc_reg_tbl[cnt].pmc.flg == PFC_ON) {
			mmio_write_8(pfc_reg_tbl[cnt].pmc.reg, pfc_reg_tbl[cnt].pmc.val);
		}

		/* PFC */
		if (pfc_reg_tbl[cnt].pfc.flg == PFC_ON) {
			mmio_write_32(pfc_reg_tbl[cnt].pfc.reg, pfc_reg_tbl[cnt].pfc.val);
		}

		/* PUPD */
		if (pfc_reg_tbl[cnt].pupd.flg == PFC_ON) {
			mmio_write_64(pfc_reg_tbl[cnt].pupd.reg, pfc_reg_tbl[cnt].pupd.val);
		}

		/* IEN */
		if (pfc_reg_tbl[cnt].ien.flg == PFC_ON) {
			mmio_write_64(pfc_reg_tbl[cnt].ien.reg, pfc_reg_tbl[cnt].ien.val);
		}
	}
}

static void pfc_scif_setup(void)
{
	/* Write protection for PFC register */
	mmio_write_32(PFC_PWPR, 0x0);
	mmio_write_32(PFC_PWPR, PWPR_PFCWE);

	pfc_write_registers(PFC_MUX_SCIF_TBL_NUM, pfc_mux_scif_reg_tbl);

	mmio_write_32(PFC_PWPR, 0x0);
	mmio_write_32(PFC_PWPR, PWPR_B0WI);
}

static void pfc_xspi_setup(void)
{
	/* Set OEN of XSPI Multi/OctaRAM IO block. */
	mmio_write_32(PFC_XSPI_OEN, XSPI_OEN_SORST_N);

	pfc_write_registers(PFC_XSPI_TBL_NUM, pfc_xspi_reg_tbl);
}

static void pfc_sd_setup(void)
{
	/* Since SDx is 3.3V, the initial value will be set. */
	mmio_write_32(PFC_SD_ch0, 1);
	mmio_write_32(PFC_SD_ch1, 0);

	pfc_write_registers(PFC_SD_TBL_NUM_SMARC, pfc_sd_reg_tbl);

	/* Write protection for PFC register */
	mmio_write_32(PFC_PWPR, 0x0);
	mmio_write_32(PFC_PWPR, PWPR_PFCWE);

	pfc_write_registers(PFC_MUX_SD_TBL_NUM, pfc_mux_sd_reg_tbl);

	mmio_write_32(PFC_PWPR, 0x0);
	mmio_write_32(PFC_PWPR, PWPR_B0WI);
}

static const PFC_REGS *pfc_boot_mode_tbls[SYS_BOOT_MODE_MAX] = {
	pfc_sd_reg_tbl,
	pfc_sd_reg_tbl,
	pfc_sd_reg_tbl,
	pfc_xspi_reg_tbl,
	pfc_xspi_reg_tbl,
	pfc_mux_scif_reg_tbl
};

//different order here.
static const uint8_t pfc_boot_mode_tbl_len[SYS_BOOT_MODE_MAX] = {
	PFC_SD_TBL_NUM_SMARC,
	PFC_SD_TBL_NUM_SMARC,
	PFC_SD_TBL_NUM_SMARC,
	PFC_XSPI_TBL_NUM,
	PFC_XSPI_TBL_NUM,
	PFC_MUX_SCIF_TBL_NUM
};

static void pfc_drive_setup(void)
{
	/* Get the boot mode */
	int16_t boot_mode = sys_get_boot_mode();

	if (boot_mode < SYS_BOOT_MODE_MAX) {
		const PFC_REGS *p_pins_tbl = pfc_boot_mode_tbls[boot_mode];
		uint8_t tbl_len = pfc_boot_mode_tbl_len[boot_mode];

		int cnt;

		for (cnt = 0; cnt < tbl_len; cnt++) {
			if (p_pins_tbl[cnt].iolh.flg == PFC_ON) {
				/* Write IOLH value from pfc_sd_reg_tbl[] masked with value in pin table */
				mmio_write_64(p_pins_tbl[cnt].iolh.reg, p_pins_tbl[cnt].iolh.val);
			}
		}
	}

	uint8_t gpio_port4_out_val = mmio_read_8(PFC_P33);
	/* Set SD card 1 pullup voltage to 3v3 */
	gpio_port4_out_val = gpio_port4_out_val | SD1_PWR_SEL_PR_PIN;

	uint8_t gpio_port2_out_val = mmio_read_8(PFC_P31);
	/* Enable SD card 1 load switch */
	gpio_port2_out_val = gpio_port2_out_val | SD1_PWR_EN_PR_PIN;

	if (SYS_BOOT_MODE_ESD == boot_mode) {
		/* Set SD card 0 pullup voltage to 3v3*/
		gpio_port2_out_val = gpio_port2_out_val | SDO_PWR_SEL_PR_PIN;
		/* Enable SD card 0 load switch */
		gpio_port2_out_val = gpio_port2_out_val | SDO_PWR_EN_PR_PIN;
	}

	mmio_write_8(PFC_P33, gpio_port4_out_val);
	mmio_write_8(PFC_P31, gpio_port2_out_val);
}

void pfc_setup(void)
{
	pfc_scif_setup();
	pfc_xspi_setup();
	pfc_sd_setup();
	pfc_drive_setup();
}
