#
# Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

# Set the platform and SOC specific header files 1st
PLAT_INCLUDES		:=	-Iplat/renesas/rz/soc/g3s/include

include plat/renesas/rz/common/g3s_common.mk
include plat/renesas/rz/board/${PLAT}_${BOARD}/rz_board.mk

PLAT_INCLUDES			+=	-Iplat/renesas/rz/soc/g3s/drivers/ddr					\
							-Iplat/renesas/rz/soc/g3s/drivers/emmc

DDR_SOURCES				+=  plat/renesas/rz/soc/g3s/drivers/ddr/ddr.c				\
							plat/renesas/rz/soc/g3s/drivers/ddr/ddr_misc.c

EMMC_SOURCES			+=	plat/renesas/rz/soc/g3s/drivers/emmc/emmc_interrupt.c	\
							plat/renesas/rz/soc/g3s/drivers/emmc/emmc_utility.c		\
							plat/renesas/rz/soc/g3s/drivers/emmc/emmc_mount.c		\
							plat/renesas/rz/soc/g3s/drivers/emmc/emmc_init.c		\
							plat/renesas/rz/soc/g3s/drivers/emmc/emmc_read.c		\
							plat/renesas/rz/soc/g3s/drivers/emmc/emmc_cmd.c			\
							plat/renesas/rz/soc/g3s/drivers/emmc/emmc_write.c

SD_SOURCES				+=	plat/renesas/rz/soc/g3s/drivers/sd_dev_low.c

PLAT_BL_COMMON_SOURCES	+=	plat/renesas/rz/soc/g3s/drivers/riic.c					\
							plat/renesas/rz/soc/g3s/drivers/tzc/tzc400.c			\
							plat/renesas/rz/soc/g3s/drivers/pwrc/pwrc.c				\
							plat/renesas/rz/soc/g3s/drivers/pwrc/pwrc_stack.S		\
							${DDR_SOURCES}

BL2_SOURCES				+=	plat/renesas/rz/soc/g3s/bl2_plat_setup.c				\
							plat/renesas/rz/soc/g3s/plat_storage.c					\
							plat/renesas/rz/soc/g3s/plat_security.c					\
							plat/renesas/rz/soc/g3s/bl2_plat_mem_params_desc.c		\
							plat/renesas/rz/soc/g3s/drivers/sys.c					\
							plat/renesas/rz/soc/g3s/plat_ddr_setup.c

BL31_SOURCES			+=	plat/renesas/rz/soc/g3s/bl31_plat_setup.c				\
							plat/renesas/rz/soc/g3s/rz_plat_sip_handler.c			\
							plat/renesas/rz/soc/g3s/plat_pm.c

ifneq (${ENABLE_STACK_PROTECTOR},0)
PLAT_BL_COMMON_SOURCES	+=	plat/renesas/rz/soc/g3s/rz_stack_protector.c
endif

ifneq (${TRUSTED_BOARD_BOOT},0)
	PLAT_BL_COMMON_SOURCES	+=	plat/renesas/rz/soc/g3s/drivers/auth/cip/cip.c

	# Include the selected chain of trust sources.
	ifeq (${COT},tbbr)
		BL2_SOURCES	+=	plat/renesas/rz/soc/g3s/drivers/auth/tbbr/tbbr_cot.c
	else
		$(error Unknown chain of trust ${COT})
	endif
endif


.PHONY: bptool_make bptool_clean

bptool: bptool_make
distclean realclean clean: bptool_clean

BPTOOLPATH		?=	tools/renesas/rz_boot_param

bptool_make:
	${Q}${MAKE} TRUSTED_BOARD_BOOT=${TRUSTED_BOARD_BOOT} --no-print-directory -C ${BPTOOLPATH}

bptool_clean:
	${Q}${MAKE} --no-print-directory -C ${BPTOOLPATH} clean

pkg:
	./tools/renesas/bptool build/g3s/${BUILD_TYPE}/bl2.bin build/g3s/${BUILD_TYPE}/bp_spi.bin 0x000A3000 spi
	cat build/g3s/${BUILD_TYPE}/bp_spi.bin build/g3s/${BUILD_TYPE}/bl2.bin > build/g3s/${BUILD_TYPE}/bl2_bp_spi.bin
	objcopy -I binary -O srec --adjust-vma=0xA1E00 --srec-forceS3 build/g3s/${BUILD_TYPE}/bl2_bp_spi.bin  build/g3s/${BUILD_TYPE}/bl2_bp_spi.srec
	./tools/renesas/bptool build/g3s/${BUILD_TYPE}/bl2.bin build/g3s/${BUILD_TYPE}/bp_mmc.bin 0x000A3000 mmc
	cat build/g3s/${BUILD_TYPE}/bp_mmc.bin build/g3s/${BUILD_TYPE}/bl2.bin > build/g3s/${BUILD_TYPE}/bl2_bp_mmc.bin
	objcopy -I binary -O srec --adjust-vma=0xA1E00 --srec-forceS3 build/g3s/${BUILD_TYPE}/bl2_bp_mmc.bin  build/g3s/${BUILD_TYPE}/bl2_bp_mmc.srec
	./tools/renesas/bptool build/g3s/${BUILD_TYPE}/bl2.bin build/g3s/${BUILD_TYPE}/bp_esd.bin 0x000A3000 esd
	cat build/g3s/${BUILD_TYPE}/bp_esd.bin build/g3s/${BUILD_TYPE}/bl2.bin > build/g3s/${BUILD_TYPE}/bl2_bp_esd.bin
	objcopy -I binary -O srec --adjust-vma=0xA1E00 --srec-forceS3 build/g3s/${BUILD_TYPE}/bl2_bp_esd.bin  build/g3s/${BUILD_TYPE}/bl2_bp_esd.srec
	#Generate FIP S-Record if FIP binary is present
	if [ -f build/g3s/${BUILD_TYPE}/fip.bin ]; then  objcopy -I binary -O srec --adjust-vma=0x00000 --srec-forceS3 build/g3s/${BUILD_TYPE}/fip.bin build/g3s/${BUILD_TYPE}/fip.srec ; fi ;