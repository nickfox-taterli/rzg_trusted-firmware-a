/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <common/tbbr/tbbr_img_def.h>
#include <drivers/io/io_driver.h>
#include <drivers/io/io_storage.h>
#include <drivers/io/io_fip.h>
#include <drivers/io/io_memmap.h>
#include <io_common.h>
#include <io_emmcdrv.h>
#include <lib/mmio.h>
#include <tools_share/firmware_image_package.h>
#include <rz_soc_def.h>
#include <sys.h>
#include <xspi.h>
#include <emmc_def.h>
#include <sys_regs.h>
#include <io_sddrv.h>


static uintptr_t memdrv_dev_handle;
static uintptr_t fip_dev_handle;
static uintptr_t emmcdrv_dev_handle;
static uintptr_t sddrv_dev_handle;


static uintptr_t boot_io_drv_id;

static const io_block_spec_t xspi0_block_spec = {
	.offset = RZ_SOC_XSPI0_FIP_BASE,
	.length = RZ_SOC_XSPI0_FIP_SIZE,
};

static const io_block_spec_t xspi1_block_spec = {
	.offset = RZ_SOC_XSPI1_FIP_BASE,
	.length = RZ_SOC_XSPI1_FIP_SIZE,
};

static const io_drv_spec_t emmc_block_spec = {
	.offset = RZ_SOC_EMMC_FIP_BASE,
	.length = RZ_SOC_EMMC_FIP_SIZE,
};

static const io_drv_spec_t sd_block_spec = {
	.offset = RZ_SOC_SD_FIP_BASE,
	.length = RZ_SOC_SD_FIP_SIZE,
};

static const io_uuid_spec_t bl31_file_spec = {
	.uuid = UUID_EL3_RUNTIME_FIRMWARE_BL31,
};

static const io_uuid_spec_t bl32_file_spec = {
	.uuid = UUID_SECURE_PAYLOAD_BL32,
};

static const io_uuid_spec_t bl33_file_spec = {
	.uuid = UUID_NON_TRUSTED_FIRMWARE_BL33,
};

#if TRUSTED_BOARD_BOOT
static const io_uuid_spec_t soc_fw_key_cert_file_spec = {
	.uuid = UUID_SOC_FW_KEY_CERT,
};

static const io_uuid_spec_t soc_fw_content_cert_file_spec = {
	.uuid = UUID_SOC_FW_CONTENT_CERT,
};

static const io_uuid_spec_t tos_fw_key_cert_file_spec = {
	.uuid = UUID_TRUSTED_OS_FW_KEY_CERT,
};

static const io_uuid_spec_t tos_fw_content_cert_file_spec = {
	.uuid = UUID_TRUSTED_OS_FW_CONTENT_CERT,
};

static const io_uuid_spec_t nt_fw_key_cert_file_spec = {
	.uuid = UUID_NON_TRUSTED_FW_KEY_CERT,
};

static const io_uuid_spec_t nt_fw_content_cert_file_spec = {
	.uuid = UUID_NON_TRUSTED_FW_CONTENT_CERT,
};
#endif

static int32_t open_emmcdrv(const uintptr_t spec);
static int32_t open_memmap(const uintptr_t spec);
static int32_t open_fipdrv(const uintptr_t spec);
static int32_t open_sddrv(const uintptr_t spec);

struct plat_io_policy {
	uintptr_t *dev_handle;
	uintptr_t image_spec;
	int32_t (*check)(const uintptr_t spec);
};

static const struct plat_io_policy sd_fip_policy = {
	&sddrv_dev_handle,
	(uintptr_t) &sd_block_spec,
	&open_sddrv
};

static const struct plat_io_policy emmc_fip_policy = {
	&emmcdrv_dev_handle,
	(uintptr_t) &emmc_block_spec,
	&open_emmcdrv
};

static const struct plat_io_policy xspi0_fip_policy = {
	&memdrv_dev_handle,
	(uintptr_t) &xspi0_block_spec,
	&open_memmap
};

static const struct plat_io_policy xspi1_fip_policy = {
	&memdrv_dev_handle,
	(uintptr_t) &xspi1_block_spec,
	&open_memmap
};
static struct plat_io_policy policies[] = {
	/* FIP_IMAGE_ID structure is added to this array on a bootmode basis */

	[BL31_IMAGE_ID] = {
				&fip_dev_handle,
				(uintptr_t) &bl31_file_spec,
				&open_fipdrv},
	[BL32_IMAGE_ID] = {
				&fip_dev_handle,
				(uintptr_t) &bl32_file_spec,
				&open_fipdrv},
	[BL33_IMAGE_ID] = {
				&fip_dev_handle,
				(uintptr_t) &bl33_file_spec,
				&open_fipdrv},
#if TRUSTED_BOARD_BOOT
	[SOC_FW_KEY_CERT_ID] = {
				&fip_dev_handle,
				(uintptr_t) &soc_fw_key_cert_file_spec,
				&open_fipdrv},
	[SOC_FW_CONTENT_CERT_ID] = {
				&fip_dev_handle,
				(uintptr_t) &soc_fw_content_cert_file_spec,
				&open_fipdrv},
	[TRUSTED_OS_FW_KEY_CERT_ID] = {
				&fip_dev_handle,
				(uintptr_t) &tos_fw_key_cert_file_spec,
				&open_fipdrv},
	[TRUSTED_OS_FW_CONTENT_CERT_ID] = {
				&fip_dev_handle,
				(uintptr_t) &tos_fw_content_cert_file_spec,
				&open_fipdrv},
	[NON_TRUSTED_FW_KEY_CERT_ID] = {
				&fip_dev_handle,
				(uintptr_t) &nt_fw_key_cert_file_spec,
				&open_fipdrv},
	[NON_TRUSTED_FW_CONTENT_CERT_ID] = {
				&fip_dev_handle,
				(uintptr_t) &nt_fw_content_cert_file_spec,
				&open_fipdrv},
#endif
	{ 0, 0, 0}
};

static int32_t open_fipdrv(const uintptr_t spec)
{
	int32_t result;

	result = io_dev_init(fip_dev_handle, boot_io_drv_id);
	if (result != 0)
		return result;

	return result;
}

static int32_t open_memmap(const uintptr_t spec)
{
	uintptr_t handle;
	int32_t result;

	result = io_dev_init(memdrv_dev_handle, 0);
	if (result != 0)
		return result;

	result = io_open(memdrv_dev_handle, spec, &handle);
	if (result == 0)
		io_close(handle);

	return result;
}

static int32_t open_emmcdrv(const uintptr_t spec)
{
	return io_dev_init(emmcdrv_dev_handle, 0);
}

static int32_t open_sddrv(const uintptr_t spec)
{
	return io_dev_init(sddrv_dev_handle, 0);
}

static void update_dev_policies(uint16_t boot_mode)
{
	switch (boot_mode) {
	case SYS_BOOT_MODE_ESD:
		policies[FIP_IMAGE_ID] = sd_fip_policy;
		break;

	case SYS_BOOT_MODE_XSPI0_x1:
	case SYS_BOOT_MODE_XSPI0_x8:
		policies[FIP_IMAGE_ID] = xspi0_fip_policy;
		break;

	case SYS_BOOT_MODE_XSPI1_x1:
		policies[FIP_IMAGE_ID] = xspi1_fip_policy;
		break;

	case SYS_BOOT_MODE_EMMC:
		policies[FIP_IMAGE_ID] = emmc_fip_policy;
		break;

	default:
		panic();
	}
}

void rz_io_setup(void)
{
	const io_dev_connector_t *memmap;
	const io_dev_connector_t *emmc;
	const io_dev_connector_t *rzsoc;
	const io_dev_connector_t *sd;
	boot_mode_t boot_mode;

	boot_mode = sys_get_boot_mode();

	boot_io_drv_id = FIP_IMAGE_ID;

	register_io_dev_fip(&rzsoc);

	io_dev_open(rzsoc, 0, &fip_dev_handle);

	if (boot_mode == SYS_BOOT_MODE_ESD) {
		register_io_dev_sddrv(&sd);
		io_dev_open(sd, 0, &sddrv_dev_handle);
	} else if ((boot_mode == SYS_BOOT_MODE_XSPI0_x1)
			|| (boot_mode == SYS_BOOT_MODE_XSPI0_x8)
			|| (boot_mode == SYS_BOOT_MODE_XSPI1_x1)) {
		xspi_setup();
		register_io_dev_memmap(&memmap);
		io_dev_open(memmap, 0, &memdrv_dev_handle);
	} else if (boot_mode == SYS_BOOT_MODE_EMMC) {
		if (emmc_init() != EMMC_SUCCESS) {
			NOTICE("BL2: Failed to eMMC driver initialize.\n");
			panic();
		}
		emmc_memcard_power(EMMC_POWER_ON);
		if (emmc_mount() != EMMC_SUCCESS) {
			NOTICE("BL2: Failed to eMMC mount operation.\n");
			panic();
		}

		register_io_dev_emmcdrv(&emmc);
		io_dev_open(emmc, 0, &emmcdrv_dev_handle);
	} else {
		ERROR("Unsupported IO device %d.\n", boot_mode);
		panic();
	}

	update_dev_policies(boot_mode);
}

int plat_get_image_source(unsigned int image_id, uintptr_t *dev_handle,
				uintptr_t *image_spec)
{
	const struct plat_io_policy *policy;
	int result;

	policy = &policies[image_id];

	result = policy->check(policy->image_spec);
	if (result != 0)
		return result;

	*image_spec = policy->image_spec;
	*dev_handle = *(policy->dev_handle);

	return 0;
}

