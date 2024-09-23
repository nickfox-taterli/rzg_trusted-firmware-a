/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <arch_helpers.h>
#include <assert.h>
#include <common/bl_common.h>
#include <lib/xlat_tables/xlat_tables_compat.h>
#include <plat/common/common_def.h>

#include <scifa.h>
#include <plat_tzc_def.h>
#include <rz_private.h>
#include <rz_soc_def.h>


static console_t rzt2h_bl31_console;
static bl2_to_bl31_params_mem_t from_bl2;


void bl31_early_platform_setup2(u_register_t arg0,
								u_register_t arg1,
								u_register_t arg2,
								u_register_t arg3)
{
	int ret;

	/* initialize console driver */
	ret = console_rz_register(
							RZT2H_SCIF_BASE,
							RZT2H_UART_INCK_HZ,
							RZT2H_UART_BARDRATE,
							&rzt2h_bl31_console);
	if (!ret)
		panic();

	console_set_scope(&rzt2h_bl31_console,
			CONSOLE_FLAG_BOOT | CONSOLE_FLAG_RUNTIME | CONSOLE_FLAG_CRASH);

	/* copy bl2_to_bl31_params_mem_t*/
	memcpy(&from_bl2, (void *)arg0, sizeof(from_bl2));
}

void bl31_plat_arch_setup(void)
{
	const mmap_region_t bl31_regions[] = {
		MAP_REGION_FLAT(BL31_START, BL31_END - BL31_START,
				MT_MEMORY | MT_RW | MT_SECURE),
		MAP_REGION_FLAT(BL_CODE_BASE, BL_CODE_END - BL_CODE_BASE,
				MT_CODE | MT_SECURE),
#if SEPARATE_CODE_AND_RODATA
		MAP_REGION_FLAT(BL_RO_DATA_BASE, BL_RO_DATA_END - BL_RO_DATA_BASE,
				MT_RO_DATA | MT_SECURE),
#endif
		{0}
	};

	const mmap_region_t rzt2h_mmap[] = {
		MAP_REGION_FLAT(RZT2H_R_BUS_NON_SAFETY_DEVICE_BASE, RZT2H_R_BUS_NON_SAFETY_DEVICE_SIZE,
				MT_DEVICE | MT_RW | MT_SECURE),
		MAP_REGION_FLAT(RZT2H_R_BUS_SAFETY_DEVICE_BASE, RZT2H_R_BUS_SAFETY_DEVICE_SIZE,
				MT_DEVICE | MT_RW | MT_SECURE),
		MAP_REGION_FLAT(RZ_SOC_SYC_BASE, RZT2H_SYSC_SIZE,
				MT_DEVICE | MT_RW | MT_SECURE),
		MAP_REGION_FLAT(RZT2H_GICD_BASE, RZT2H_GICD_SIZE,
				MT_DEVICE | MT_RW | MT_SECURE),
		{0}
	};

	setup_page_tables(bl31_regions, rzt2h_mmap);
	enable_mmu_el3(0);
}

void bl31_platform_setup(void)
{
	/* initialize GIC-600 */
	plat_gic_driver_init();
	plat_gic_init();
}

entry_point_info_t *bl31_plat_get_next_image_ep_info(uint32_t type)
{
	entry_point_info_t *next_image_info = NULL;

	next_image_info = (type == NON_SECURE)
			? &from_bl2.bl33_ep_info : &from_bl2.bl32_ep_info;

	if (next_image_info->pc)
		return next_image_info;
	else
		return NULL;
}
