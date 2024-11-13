/*
 * Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CPG_H__
#define __CPG_H__

void cpg_early_setup(void);
void cpg_setup(void);
void cpg_ddr_part1(void);
void cpg_ddr_part2(void);

#endif /* __CPG_H__ */
