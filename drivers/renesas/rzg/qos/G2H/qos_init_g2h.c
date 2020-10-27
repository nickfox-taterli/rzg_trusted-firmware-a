/*
 * Copyright (c) 2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>

#include <common/debug.h>

#include "../qos_common.h"
#include "../qos_reg.h"
#include "qos_init_g2h.h"

#define	RZG_QOS_VERSION			"rev.0.07"

#define QOSWT_TIME_BANK0			20000000U	/* unit:ns */

#define	QOSWT_WTEN_ENABLE			0x1U

#define QOSCTRL_REF_ARS_ARBSTOPCYCLE_G2H	(SL_INIT_SSLOTCLK_G2H - 0x5U)

#define OSWT_WTREF_SLOT0_EN_REQ1_SLOT		3U
#define OSWT_WTREF_SLOT0_EN_REQ2_SLOT		9U
#define QOSWT_WTREF_SLOT0_EN				\
	((0x1U << OSWT_WTREF_SLOT0_EN_REQ1_SLOT) |	\
	(0x1U << OSWT_WTREF_SLOT0_EN_REQ2_SLOT))
#define QOSWT_WTREF_SLOT1_EN				\
	((0x1U << OSWT_WTREF_SLOT0_EN_REQ1_SLOT) |	\
	(0x1U << OSWT_WTREF_SLOT0_EN_REQ2_SLOT))

#define QOSWT_WTSET0_REQ_SSLOT0			5U
#define WT_BASE_SUB_SLOT_NUM0			12U
#define QOSWT_WTSET0_PERIOD0_G2H			\
	((QOSWT_TIME_BANK0 / QOSWT_WTSET0_CYCLE_G2H) - 1U)
#define QOSWT_WTSET0_SSLOT0			(QOSWT_WTSET0_REQ_SSLOT0 - 1U)
#define QOSWT_WTSET0_SLOTSLOT0			(WT_BASE_SUB_SLOT_NUM0 - 1U)

#define QOSWT_WTSET1_PERIOD1_G2H		(QOSWT_WTSET0_PERIOD0_G2H)
#define QOSWT_WTSET1_SSLOT1			(QOSWT_WTSET0_SSLOT0)
#define QOSWT_WTSET1_SLOTSLOT1			(QOSWT_WTSET0_SLOTSLOT0)

#if RZG_QOS_TYPE  == RZG_QOS_TYPE_DEFAULT

#if RZG_REF_INT == RZG_REF_DEFAULT
#include "qos_init_g2h_mstat195.h"
#else
#include "qos_init_g2h_mstat390.h"
#endif

#if RZG_REWT_TRAINING != RZG_REWT_TRAINING_DISABLE

#if RZG_REF_INT == RZG_REF_DEFAULT
#include "qos_init_g2h_qoswt195.h"
#else
#include "qos_init_g2h_qoswt390.h"
#endif

#endif /* RZG_REWT_TRAINING != RZG_REWT_TRAINING_DISABLE */

#endif

struct rzg_gen2_dbsc_qos_settings g2h_qos[] = {
	/* BUFCAM settings */
	{ DBSC_DBCAM0CNF1, 0x00043218U },
	{ DBSC_DBCAM0CNF2, 0x000000F4U },
	{ DBSC_DBCAM0CNF3, 0x00000000U },
	{ DBSC_DBSCHCNT0, 0x000F0037U },
	{ DBSC_DBSCHSZ0, 0x00000001U },
	{ DBSC_DBSCHRW0, 0x22421111U },

	/* DDR3 */
	{ DBSC_SCFCTST2, 0x012F1123U },

	/* QoS Settings */
	{ DBSC_DBSCHQOS00, 0x00000F00U },
	{ DBSC_DBSCHQOS01, 0x00000B00U },
	{ DBSC_DBSCHQOS02, 0x00000000U },
	{ DBSC_DBSCHQOS03, 0x00000000U },
	{ DBSC_DBSCHQOS40, 0x00000300U },
	{ DBSC_DBSCHQOS41, 0x000002F0U },
	{ DBSC_DBSCHQOS42, 0x00000200U },
	{ DBSC_DBSCHQOS43, 0x00000100U },
	{ DBSC_DBSCHQOS90, 0x00000100U },
	{ DBSC_DBSCHQOS91, 0x000000F0U },
	{ DBSC_DBSCHQOS92, 0x000000A0U },
	{ DBSC_DBSCHQOS93, 0x00000040U },
	{ DBSC_DBSCHQOS120, 0x00000040U },
	{ DBSC_DBSCHQOS121, 0x00000030U },
	{ DBSC_DBSCHQOS122, 0x00000020U },
	{ DBSC_DBSCHQOS123, 0x00000010U },
	{ DBSC_DBSCHQOS130, 0x00000100U },
	{ DBSC_DBSCHQOS131, 0x000000F0U },
	{ DBSC_DBSCHQOS132, 0x000000A0U },
	{ DBSC_DBSCHQOS133, 0x00000040U },
	{ DBSC_DBSCHQOS140, 0x000000C0U },
	{ DBSC_DBSCHQOS141, 0x000000B0U },
	{ DBSC_DBSCHQOS142, 0x00000080U },
	{ DBSC_DBSCHQOS143, 0x00000040U },
	{ DBSC_DBSCHQOS150, 0x00000040U },
	{ DBSC_DBSCHQOS151, 0x00000030U },
	{ DBSC_DBSCHQOS152, 0x00000020U },
	{ DBSC_DBSCHQOS153, 0x00000010U },
};

void qos_init_g2h(void)
{
	unsigned int split_area;

	rzg_qos_dbsc_setting(g2h_qos, ARRAY_SIZE(g2h_qos), true);

	/* use 1(2GB) for RZG_DRAM_LPDDR4_MEMCONF for G2H */
	split_area = 0x1CU;

	/* DRAM Split Address mapping */
#if (RZG_DRAM_SPLIT == RZG_DRAM_SPLIT_4CH)
#if RZG_LSI == RZG_G2H
#error "Don't set DRAM Split 4ch(G2H)"
#else
	ERROR("DRAM Split 4ch not supported.(G2H)");
	panic();
#endif
#elif (RZG_DRAM_SPLIT == RZG_DRAM_SPLIT_2CH) || \
    (RZG_DRAM_SPLIT == RZG_DRAM_SPLIT_AUTO)
	NOTICE("BL2: DRAM Split is 2ch(DDR %x)\n", (int)qos_init_ddr_phyvalid);

	io_write_32(AXI_ADSPLCR0, ADSPLCR0_AREA(split_area));
	io_write_32(AXI_ADSPLCR1, ADSPLCR0_ADRMODE_DEFAULT
		    | ADSPLCR0_SPLITSEL(0xFFU)
		    | ADSPLCR0_AREA(split_area)
		    | ADSPLCR0_SWP);
	io_write_32(AXI_ADSPLCR2, 0x00001004U);
	io_write_32(AXI_ADSPLCR3, 0x00000000U);
#else
	io_write_32(AXI_ADSPLCR0, ADSPLCR0_AREA(split_area));
	NOTICE("BL2: DRAM Split is OFF(DDR %x)\n", (int)qos_init_ddr_phyvalid);
#endif

#if !(RZG_QOS_TYPE == RZG_QOS_NONE)
#if RZG_QOS_TYPE  == RZG_QOS_TYPE_DEFAULT
	NOTICE("BL2: QoS is default setting(%s)\n", RZG_QOS_VERSION);
#endif

#if RZG_REF_INT == RZG_REF_DEFAULT
	NOTICE("BL2: DRAM refresh interval 1.95 usec\n");
#else
	NOTICE("BL2: DRAM refresh interval 3.9 usec\n");
#endif

#if RZG_REWT_TRAINING != RZG_REWT_TRAINING_DISABLE
	NOTICE("BL2: Periodic Write DQ Training\n");
#endif /* RZG_REWT_TRAINING != RZG_REWT_TRAINING_DISABLE */

	io_write_32(QOSCTRL_RAS, 0x00000044U);
	io_write_64(QOSCTRL_DANN, 0x0404020002020201UL);
	io_write_32(QOSCTRL_DANT, 0x0020100AU);
	io_write_32(QOSCTRL_FSS, 0x0000000AU);
	io_write_32(QOSCTRL_INSFC, 0x06330001U);
	io_write_32(QOSCTRL_RACNT0, 0x00010003U);

	/* GPU Boost Mode */
	io_write_32(QOSCTRL_STATGEN0, 0x00000001U);

	io_write_32(QOSCTRL_SL_INIT,
		    SL_INIT_REFFSSLOT | SL_INIT_SLOTSSLOT |
		    SL_INIT_SSLOTCLK_G2H);
	io_write_32(QOSCTRL_REF_ARS,
		    ((QOSCTRL_REF_ARS_ARBSTOPCYCLE_G2H << 16)));

	uint32_t i;

	for (i = 0U; i < ARRAY_SIZE(mstat_fix); i++) {
		io_write_64(QOSBW_FIX_QOS_BANK0 + i * 8, mstat_fix[i]);
		io_write_64(QOSBW_FIX_QOS_BANK1 + i * 8, mstat_fix[i]);
	}
	for (i = 0U; i < ARRAY_SIZE(mstat_be); i++) {
		io_write_64(QOSBW_BE_QOS_BANK0 + i * 8, mstat_be[i]);
		io_write_64(QOSBW_BE_QOS_BANK1 + i * 8, mstat_be[i]);
	}
#if RZG_REWT_TRAINING != RZG_REWT_TRAINING_DISABLE
	for (i = 0U; i < ARRAY_SIZE(qoswt_fix); i++) {
		io_write_64(QOSWT_FIX_WTQOS_BANK0 + i * 8,
			    qoswt_fix[i]);
		io_write_64(QOSWT_FIX_WTQOS_BANK1 + i * 8,
			    qoswt_fix[i]);
	}
	for (i = 0U; i < ARRAY_SIZE(qoswt_be); i++) {
		io_write_64(QOSWT_BE_WTQOS_BANK0 + i * 8, qoswt_be[i]);
		io_write_64(QOSWT_BE_WTQOS_BANK1 + i * 8, qoswt_be[i]);
	}
#endif /* RZG_REWT_TRAINING != RZG_REWT_TRAINING_DISABLE */

	/* AXI setting */
	io_write_32(AXI_MMCR, 0x00010008U);
	io_write_32(AXI_TR3CR, 0x00010000U);
	io_write_32(AXI_TR4CR, 0x00010000U);

	/* RT bus Leaf setting */
	io_write_32(RT_ACT0, 0x00000000U);
	io_write_32(RT_ACT1, 0x00000000U);

	/* CCI bus Leaf setting */
	io_write_32(CPU_ACT0, 0x00000003U);
	io_write_32(CPU_ACT1, 0x00000003U);
	io_write_32(CPU_ACT2, 0x00000003U);
	io_write_32(CPU_ACT3, 0x00000003U);

	io_write_32(QOSCTRL_RAEN, 0x00000001U);

#if RZG_REWT_TRAINING != RZG_REWT_TRAINING_DISABLE
	/*  re-write training setting */
	io_write_32(QOSWT_WTREF,
		    ((QOSWT_WTREF_SLOT1_EN << 16) | QOSWT_WTREF_SLOT0_EN));
	io_write_32(QOSWT_WTSET0,
		    ((QOSWT_WTSET0_PERIOD0_G2H << 16) |
		     (QOSWT_WTSET0_SSLOT0 << 8) | QOSWT_WTSET0_SLOTSLOT0));
	io_write_32(QOSWT_WTSET1,
		    ((QOSWT_WTSET1_PERIOD1_G2H << 16) |
		     (QOSWT_WTSET1_SSLOT1 << 8) | QOSWT_WTSET1_SLOTSLOT1));

	io_write_32(QOSWT_WTEN, QOSWT_WTEN_ENABLE);
#endif /* RZG_REWT_TRAINING != RZG_REWT_TRAINING_DISABLE */

	io_write_32(QOSCTRL_STATQC, 0x00000001U);
#else
	NOTICE("BL2: QoS is None\n");

	io_write_32(QOSCTRL_RAEN, 0x00000001U);
#endif /* !(RZG_QOS_TYPE == RZG_QOS_NONE) */
}
