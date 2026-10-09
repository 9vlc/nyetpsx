/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _SYS__CTP_H
#define _SYS__CTP_H

#include <stdint.h>

#define CTP_ADDR_CONTROLLER		0x01 /* PS1 or PS2 controller */
#define CTP_ADDR_ACCESS_CARD		0x21 /* PS1 Net Yaroze Access Card */
#define CTP_ADDR_IMODE			0x41 /* PS1 i-Mode adapter cable */
#define CTP_ADDR_REMOTE			0x61 /* PS2 DVD remote receiver */
#define CTP_ADDR_MEMORY_CARD		0x81 /* PS1 memory card or PocketStation */
typedef uint32_t ctp_device_address_t;

/* Controller commands */
#define CTP_PAD_READ_DATA		0x42
#define CTP_PAD_ENTER_CONFIG_MODE	0x43

/* Controller commands in configuration mode */
#define CTP_CFG_VREF_PARAM		0x40 /* CTP 2.0 (DualShock 2) only */
#define CTP_CFG_QUERY_BUTTON_MASK	0x41 /* CTP 2.0 (DualShock 2) only */
#define CTP_CFG_SET_MAIN_MODE		0x44
#define CTP_CFG_QUERY_MODEL		0x45
#define CTP_CFG_QUERY_ACT		0x46
#define CTP_CFG_QUERY_COMB		0x47
#define CTP_CFG_UNKNOWN			0x48
#define CTP_CFG_QUERY_COMB2		0x4B
#define CTP_CFG_QUERY_MODE		0x4C
#define CTP_CFG_SET_ACT_ALIGN		0x4D
#define CTP_CFG_SET_BUTTON_INFO		0x4F /* CTP 2.0 (DualShock 2) only */

/* DVD remote receiver commands */
#define CTP_RM_POLL			0x04
#define CTP_RM_INIT			0x06
#define CTP_RM_FIND			0x0F

/* Memory card commands */
#define CTP_MC_READ			'R'
#define CTP_MC_SIZE			'S'
#define CTP_MC_WRITE			'W'

/* PocketStation commands */
#define CTP_MCX_GET_MCX_INFO		0x58
#define CTP_MCX_EXEC_APL		0x59
#define CTP_MCX_ALL_INFO		0x5A
#define CTP_MCX_READ_DEV		0x5B
#define CTP_MCX_WRITE_DEV		0x5C
#define CTP_MCX_SHOW_TRANS		0x5D
#define CTP_MCX_CURR_CTRL		0x5E
#define CTP_MCX_FLASH_ACS		0x5F
typedef uint32_t ctp_command_t;

#define CTP_TAP_NONE			0
#define CTP_TAP_A			1
#define CTP_TAP_B			2
#define CTP_TAP_C			3
#define CTP_TAP_D			4
typedef uint32_t ctp_multitap_port_t;

#define CTP_TYPE_NONE			0
#define CTP_TYPE_MOUSE			1
#define CTP_TYPE_NEGCON			2
#define CTP_TYPE_JUSTIFIER		3
#define CTP_TYPE_DIGITAL		4
#define CTP_TYPE_ANALOG_STICK		5
#define CTP_TYPE_GUNCON			6
#define CTP_TYPE_ANALOG			7
#define CTP_TYPE_MULTITAP		8
#define CTP_TYPE_KEYBOARD		9
#define CTP_TYPE_JOGCON			14
#define CTP_TYPE_CONFIG_MODE		15
typedef uint32_t ctp_controller_type_t;

/* Controller button definitions */

#define BTN_PAD_SELECT			(1 <<  0)
#define BTN_PAD_L3			(1 <<  1)
#define BTN_PAD_R3			(1 <<  2)
#define BTN_PAD_START			(1 <<  3)
#define BTN_PAD_UP			(1 <<  4)
#define BTN_PAD_RIGHT			(1 <<  5)
#define BTN_PAD_DOWN			(1 <<  6)
#define BTN_PAD_LEFT			(1 <<  7)
#define BTN_PAD_L2			(1 <<  8)
#define BTN_PAD_R2			(1 <<  9)
#define BTN_PAD_L1			(1 << 10)
#define BTN_PAD_R1			(1 << 11)
#define BTN_PAD_TRIANGLE		(1 << 12)
#define BTN_PAD_CIRCLE			(1 << 13)
#define BTN_PAD_CROSS			(1 << 14)
#define BTN_PAD_SQUARE			(1 << 15)
typedef uint32_t controller_button_flag_t;

#define BTN_MOUSE_RIGHT			BTN_PAD_L1
#define BTN_MOUSE_LEFT			BTN_PAD_R1
typedef uint32_t mouse_button_flag_t;

#define BTN_NEGCON_START		BTN_PAD_START
#define BTN_NEGCON_UP			BTN_PAD_UP
#define BTN_NEGCON_RIGHT		BTN_PAD_RIGHT
#define BTN_NEGCON_DOWN			BTN_PAD_DOWN
#define BTN_NEGCON_LEFT			BTN_PAD_LEFT
#define BTN_NEGCON_R			BTN_PAD_R1
#define BTN_NEGCON_B			BTN_PAD_TRIANGLE
#define BTN_NEGCON_A			BTN_PAD_CIRCLE
typedef uint32_t negcon_button_flag_t;

#define BTN_JUSTIFIER_START		BTN_PAD_START
#define BTN_JUSTIFIER_BACK		BTN_PAD_CROSS
#define BTN_JUSTIFIER_TRIGGER		BTN_PAD_SQUARE
typedef uint32_t justifier_button_flag_t;

#define BTN_GUNCON_A			BTN_PAD_START
#define BTN_GUNCON_TRIGGER		BTN_PAD_CIRCLE
#define BTN_GUNCON_B			BTN_PAD_CROSS
typedef uint32_t guncon_button_flag_t;

#define BTN_BM_SELECT			BTN_PAD_SELECT
#define BTN_BM_START			BTN_PAD_START
#define BTN_BM_TURNTABLE_UP		BTN_PAD_UP
#define BTN_BM_TURNTABLE_DOWN		BTN_PAD_DOWN
#define BTN_BM_WHITE4			BTN_PAD_LEFT /* PS2 IIDX controllers only */
#define BTN_BM_BLACK3			BTN_PAD_L2 /* PS2 IIDX controllers only */
#define BTN_BM_BLACK1			BTN_PAD_L1
#define BTN_BM_BLACK2			BTN_PAD_R1
#define BTN_BM_WHITE3			BTN_PAD_CIRCLE
#define BTN_BM_WHITE2			BTN_PAD_CROSS
#define BTN_BM_WHITE1			BTN_PAD_SQUARE
typedef uint32_t beatmania_button_flag_t;

#define BTN_POPN_SELECT			BTN_PAD_SELECT
#define BTN_POPN_START			BTN_PAD_START
#define BTN_POPN_YELLOW2		BTN_PAD_UP
#define BTN_POPN_ID_BITMASK		(BTN_PAD_RIGHT | BTN_PAD_DOWN | BTN_PAD_LEFT)
#define BTN_POPN_WHITE2			BTN_PAD_L2
#define BTN_POPN_GREEN2			BTN_PAD_R2
#define BTN_POPN_RED			BTN_PAD_L1
#define BTN_POPN_GREEN1			BTN_PAD_R1
#define BTN_POPN_WHITE1			BTN_PAD_TRIANGLE
#define BTN_POPN_YELLOW1		BTN_PAD_CIRCLE
#define BTN_POPN_BLUE1			BTN_PAD_CROSS
#define BTN_POPN_BLUE2			BTN_PAD_SQUARE
typedef uint32_t popn_button_flag_t;

#define BTN_TAIKO_SELECT		BTN_PAD_SELECT
#define BTN_TAIKO_START			BTN_PAD_START
#define BTN_TAIKO_DON1			BTN_PAD_LEFT
#define BTN_TAIKO_KA1			BTN_PAD_L1
#define BTN_TAIKO_KA2			BTN_PAD_R1
#define BTN_TAIKO_DON2			BTN_PAD_CIRCLE
typedef uint32_t taiko_button_flag_t;

#define BTN_DENSHA_SELECT		BTN_PAD_SELECT
#define BTN_DENSHA_START		BTN_PAD_START
#define BTN_DENSHA_ID_BITMASK		(BTN_PAD_UP | BTN_PAD_DOWN)
#define BTN_DENSHA_POWER2		BTN_PAD_RIGHT
#define BTN_DENSHA_POWER1		BTN_PAD_LEFT
#define BTN_DENSHA_BRAKE1		BTN_PAD_L2
#define BTN_DENSHA_BRAKE3		BTN_PAD_R2
#define BTN_DENSHA_BRAKE0		BTN_PAD_L1
#define BTN_DENSHA_BRAKE2		BTN_PAD_R1
#define BTN_DENSHA_POWER0		BTN_PAD_TRIANGLE
#define BTN_DENSHA_C			BTN_PAD_CIRCLE
#define BTN_DENSHA_B			BTN_PAD_CROSS
#define BTN_DENSHA_A			BTN_PAD_SQUARE
typedef uint32_t densha_button_flag_t;

#endif /* _SYS__CTP_H */
