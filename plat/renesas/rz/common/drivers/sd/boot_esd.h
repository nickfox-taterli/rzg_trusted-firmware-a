/*
 * Copyright (c) 2022, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**********************************************************************************************************************
 * File Name    : boot_esd.h
 * Version      : 1.0
 * Description  : eSD boot module dependent header file.
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 01.09.2020 1.00     First Release
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 *********************************************************************************************************************/

/**********************************************************************************************************************
 Macro definitions
 *********************************************************************************************************************/
#ifndef BOOT_ESD_H
#define BOOT_ESD_H

#define        NULL                               (0)                      /* NULL */

#define        DEF_FLAG_ON                        (1)                      /* Flag "ON" definition */
#define        DEF_FLAG_OFF                       (0)                      /* Flag "OFF" definition */

/* Borrowed from eMMC*/
#define D_STAT_CHECK_RESULT_PASS  (0x00000001)
#define D_STAT_EXTENDED_BOOT_MODE (0x00000002)
#define D_STAT_CHECK_RESULT_FAIL  (0x00001000)

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 External global variables
 *********************************************************************************************************************/
/* Buffer area */
/* Work area dedicated to eSD driver library (Size specified by library) */
extern uint64_t     gl_uwWorkBuffer[ ESD_SIZE_OF_INIT / sizeof(uint64_t)];

/* Struct pointer */
extern stESD_BOOT_OUTPUT *       gl_pstEsdBootOutput;                       /* Output structure pointer */

/**********************************************************************************************************************
 Exported global functions
 *********************************************************************************************************************/

#endif  /* BOOT_ESD_H */
/*---------------------------------------------------------------------------*/
/*      End Of File                                                          */
/*---------------------------------------------------------------------------*/