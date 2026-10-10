/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _SYS__CDROM_H
#define _SYS__CDROM_H

#include <sys/mmio.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define DEF(T) static inline T __attribute__((always_inline))
#define PACKED __attribute__((packed))

/*
 * CD-ROM drive register flag definitions
 */

#define CDROM_HSTS_RA_BITMASK	(3u << 0)
#define CDROM_HSTS_ADPBUSY	(1u << 2)
#define CDROM_HSTS_PRMEMPT	(1u << 3)
#define CDROM_HSTS_PRMWRDY	(1u << 4)
#define CDROM_HSTS_RSLRRDY	(1u << 5)
#define CDROM_HSTS_DRQSTS	(1u << 6)
#define CDROM_HSTS_BUSYSTS	(1u << 7)
typedef uint32_t cdrom_hsts_flag_t;

#define CDROM_HINT_INT_BITMASK	(7u << 0)
#define CDROM_HINT_INT0		(1u << 0)
#define CDROM_HINT_INT1		(1u << 1)
#define CDROM_HINT_INT2		(1u << 2)
#define CDROM_HINT_BFEMPT	(1u << 3)
#define CDROM_HINT_BFWRDY	(1u << 4)
typedef uint32_t cdrom_hint_flag_t;

#define CDROM_HCHPCTL_SMEN	(1u << 5)
#define CDROM_HCHPCTL_BFWR	(1u << 6)
#define CDROM_HCHPCTL_BFRD	(1u << 7)
typedef uint32_t cdrom_hchpctl_flag_t;

#define CDROM_HCLRCTL_CLRINT_BITMASK	(7u << 0)
#define CDROM_HCLRCTL_CLRINT0		(1u << 0)
#define CDROM_HCLRCTL_CLRINT1		(1u << 1)
#define CDROM_HCLRCTL_CLRINT2		(1u << 2)
#define CDROM_HCLRCTL_CLRBFEMPT		(1u << 3)
#define CDROM_HCLRCTL_CLRBFWRDY		(1u << 4)
#define CDROM_HCLRCTL_SMADPCLR		(1u << 5)
#define CDROM_HCLRCTL_CLRPRM		(1u << 6)
#define CDROM_HCLRCTL_CHPRST		(1u << 7)
typedef uint32_t cdrom_hclrctl_flag_t;

#define CDROM_CI_SM		(1u << 0)
#define CDROM_CI_FS		(1u << 2)
#define CDROM_CI_BITLNGTH	(1u << 4)
#define CDROM_CI_EMPHASIS	(1u << 6)
typedef uint32_t cdrom_ci_flag_t;

#define CDROM_ADPCTL_ADPMUTE	(1u << 0)
#define CDROM_ADPCTL_CHNGATV	(1u << 5)
typedef uint32_t cdrom_adpctl_flag_t;

/*
 * CD-ROM drive register addresses
 */

#define CDROM_HSTS		MMIO8(IO_BASE | 0x800) /* All banks */
#define CDROM_RESULT		MMIO8(IO_BASE | 0x801) /* All banks */
#define CDROM_RDDATA		MMIO8(IO_BASE | 0x802) /* All banks */
#define CDROM_HINTMSK_R		MMIO8(IO_BASE | 0x803) /* Bank 0 */
#define CDROM_HINTSTS		MMIO8(IO_BASE | 0x803) /* Bank 1 */

#define CDROM_ADDRESS		MMIO8(IO_BASE | 0x800) /* All banks */
#define CDROM_COMMAND		MMIO8(IO_BASE | 0x801) /* Bank 0 */
#define CDROM_PARAMETER		MMIO8(IO_BASE | 0x802) /* Bank 0 */
#define CDROM_HCHPCTL		MMIO8(IO_BASE | 0x803) /* Bank 0 */
#define CDROM_WRDATA		MMIO8(IO_BASE | 0x801) /* Bank 1 */
#define CDROM_HINTMSK_W		MMIO8(IO_BASE | 0x802) /* Bank 1 */
#define CDROM_HCLRCTL		MMIO8(IO_BASE | 0x803) /* Bank 1 */
#define CDROM_CI		MMIO8(IO_BASE | 0x801) /* Bank 2 */
#define CDROM_ATV0		MMIO8(IO_BASE | 0x802) /* Bank 2 */
#define CDROM_ATV1		MMIO8(IO_BASE | 0x803) /* Bank 2 */
#define CDROM_ATV2		MMIO8(IO_BASE | 0x801) /* Bank 3 */
#define CDROM_ATV3		MMIO8(IO_BASE | 0x802) /* Bank 3 */
#define CDROM_ADPCTL		MMIO8(IO_BASE | 0x803) /* Bank 3 */

/*
 * CD-ROM XA sector header structure
 */

struct cdrom_xa_header {
	/* 0x00 */
	uint8_t file;

	/* 0x01 */
	uint8_t channel;

	/* 0x02; CDROM_XA_SM_* */
	uint8_t submode;

	/* 0x03; CDROM_XA_CI_* */
	uint8_t coding_info;

	/* 0x04 */
} PACKED;

#define CDROM_XA_SM_END_OF_RECORD	(1u << 0)
#define CDROM_XA_SM_TYPE_VIDEO		(1u << 1)
#define CDROM_XA_SM_TYPE_AUDIO		(1u << 2)
#define CDROM_XA_SM_TYPE_DATA		(1u << 3)
#define CDROM_XA_SM_TRIGGER		(1u << 4)
#define CDROM_XA_SM_FORM2		(1u << 5)
#define CDROM_XA_SM_REAL_TIME		(1u << 6)
#define CDROM_XA_SM_END_OF_FILE		(1u << 7)
typedef uint32_t cdrom_xa_submode_flag_t;

#define CDROM_XA_CI_CHANNELS_BITMASK	(3u << 0)
#define CDROM_XA_CI_CHANNELS_MONO	(0u << 0)
#define CDROM_XA_CI_CHANNELS_STEREO	(1u << 0)
#define CDROM_XA_CI_SAMPLE_RATE_BITMASK	(3u << 2)
#define CDROM_XA_CI_SAMPLE_RATE_37800	(0u << 2)
#define CDROM_XA_CI_SAMPLE_RATE_18900	(1u << 2)
#define CDROM_XA_CI_BITS_BITMASK	(3u << 4)
#define CDROM_XA_CI_BITS_4		(0u << 4)
#define CDROM_XA_CI_BITS_8		(1u << 4)
#define CDROM_XA_CI_EMPHASIS		(1u << 6)
typedef uint32_t cdrom_xa_coding_info_flag_t;

/*
 * CD-ROM drive data types
 */

struct cdrom_msf {
	uint8_t minute;
	uint8_t second;
	uint8_t frame;
};
typedef struct cdrom_msf cdrom_msf_t;

struct cdrom_getloc_l_result {
	/* 0x00 */
	struct cdrom_msf absolute_msf;

	/* 0x03 */
	uint8_t mode;

	/* 0x04 */
	struct cdrom_xa_header header;

	/* 0x08 */
} PACKED;
typedef struct cdrom_getloc_l_result cdrom_getloc_l_result_t;

struct cdrom_getloc_p_result {
	/* 0x00 */
	uint8_t track;

	/* 0x01 */
	uint8_t index;

	/* 0x02 */
	struct cdrom_msf relative_msf;

	/* 0x05 */
	struct cdrom_msf absolute_msf;

	/* 0x08 */
} PACKED;
typedef struct cdrom_getloc_p_result cdrom_getloc_p_result_t;

struct cdrom_get_id_result {
	/* 0x00 */
	uint8_t status;

	/* 0x01 */
	uint8_t flag;

	/* 0x02 */
	uint8_t type;

	/* 0x03 */
	uint8_t atip;

	/* 0x04 */
	uint8_t license[4];

	/* 0x08 */
} PACKED;
typedef struct cdrom_get_id_result cdrom_get_id_result_t;

struct cdrom_report_packet {
	/* 0x00 */
	uint8_t status;

	/* 0x01 */
	uint8_t track;

	/* 0x02 */
	uint8_t index;

	/* 0x03 */
	struct cdrom_msf msf;

	/* 0x06 */
	uint8_t peak_left;

	/* 0x07 */
	uint8_t peak_right;

	/* 0x08 */
} PACKED;
typedef struct cdrom_report_packet cdrom_report_packet_t;

DEF(uint8_t)
cdrom_encode_bcd(
	uint8_t val
	)
{
	/*
	 * output = units + tens * 16
	 *        = units + tens * 10 + tens * 6
	 *        = val               + tens * 6
	 */
	return (val + (val / 10) * 6);
}

DEF(uint8_t)
cdrom_decode_bcd(
	uint8_t val
	)
{
	/*
	 * output = low + high * 10
	 *        = low + high * 16 - high * 6
	 *        = val             - high * 6
	 */
	return (val - (val >> 4) * 6);
}

DEF(void)
cdrom_lba_to_msf(
	cdrom_msf_t *msf,
	uint32_t lba
	)
{
	/*
	 * The 2-second lead-in area at 00:00:00 can't hold any audio or data
	 *   and must be skipped.
	 */
	lba += 150;

	msf->minute = cdrom_encode_bcd(lba / (75 * 60));
	msf->second = cdrom_encode_bcd((lba / 75) % 60);
	msf->frame = cdrom_encode_bcd(lba % 75);
}

DEF(uint32_t)
cdrom_msf_to_lba(
	const cdrom_msf_t *msf
	)
{
	return (cdrom_decode_bcd(msf->minute) * (75 * 60) +
		cdrom_decode_bcd(msf->second) * 75 +
		cdrom_decode_bcd(msf->frame) - 150);
}

/*
 * CD-ROM drive command and status definitions
 */

#define CDROM_CMD_NOP		0x01
#define CDROM_CMD_SETLOC	0x02
#define CDROM_CMD_PLAY		0x03
#define CDROM_CMD_FORWARD	0x04
#define CDROM_CMD_BACKWARD	0x05
#define CDROM_CMD_READ_N	0x06
#define CDROM_CMD_STANDBY	0x07
#define CDROM_CMD_STOP		0x08
#define CDROM_CMD_PAUSE		0x09
#define CDROM_CMD_INIT		0x0A
#define CDROM_CMD_MUTE		0x0B
#define CDROM_CMD_DEMUTE	0x0C
#define CDROM_CMD_SETFILTER	0x0D
#define CDROM_CMD_SETMODE	0x0E
#define CDROM_CMD_GETPARAM	0x0F
#define CDROM_CMD_GETLOC_L	0x10
#define CDROM_CMD_GETLOC_P	0x11
#define CDROM_CMD_SETSESSION	0x12
#define CDROM_CMD_GET_TN	0x13
#define CDROM_CMD_GET_TD	0x14
#define CDROM_CMD_SEEK_L	0x15
#define CDROM_CMD_SEEK_P	0x16
#define CDROM_CMD_TEST		0x19
#define CDROM_CMD_GET_ID	0x1A
#define CDROM_CMD_READ_S	0x1B
#define CDROM_CMD_RESET		0x1C
#define CDROM_CMD_GET_Q		0x1D /* Versions 0xC1 and later only */
#define CDROM_CMD_READ_TOC	0x1E /* Versions 0xC1 and later only */
#define CDROM_CMD_UNLOCK0	0x50 /* Versions 0xC1 and later only */
#define CDROM_CMD_UNLOCK1	0x51 /* Versions 0xC1 and later only */
#define CDROM_CMD_UNLOCK2	0x52 /* Versions 0xC1 and later only */
#define CDROM_CMD_UNLOCK3	0x53 /* Versions 0xC1 and later only */
#define CDROM_CMD_UNLOCK4	0x54 /* Versions 0xC1 and later only */
#define CDROM_CMD_UNLOCK5	0x55 /* Versions 0xC1 and later only */
#define CDROM_CMD_UNLOCK6	0x56 /* Versions 0xC1 and later only */
#define CDROM_CMD_LOCK		0x57 /* Versions 0xC1 and later only */
typedef uint32_t cdrom_cmd_t;

#define CDROM_TEST_READ_ID		0x04
#define CDROM_TEST_GET_ID_COUNTERS	0x05
#define CDROM_TEST_GET_VERSION		0x20
#define CDROM_TEST_GET_SWITCHES		0x21
#define CDROM_TEST_GET_REGION		0x22 /* Versions 0xC1 and later only */
#define CDROM_TEST_GET_SERVO_TYPE	0x23 /* Versions 0xC1 and later only */
#define CDROM_TEST_GET_DSP_TYPE		0x24 /* Versions 0xC1 and later only */
#define CDROM_TEST_GET_DECODER_TYPE	0x25 /* Versions 0xC1 and later only */
#define CDROM_TEST_DSP_CMD		0x50
#define CDROM_TEST_DSP_CMD_RESP		0x51 /* Versions 0xC2 and later only */
#define CDROM_TEST_MCU_PEEK		0x60
#define CDROM_TEST_DECODER_GET_REG	0x71 /* Versions 0xC1 and later only */
#define CDROM_TEST_DECODER_SET_REG	0x72 /* Versions 0xC1 and later only */
#define CDROM_TEST_DECODER_GET_SRAM_PTR	0x75 /* Versions 0xC1 and later only */
#define CDROM_TEST_DECODER_SET_SRAM_PTR	0x76 /* Versions 0xC1 and later only */
typedef uint32_t cdrom_test_cmd_t;

#define CDROM_IRQ_NONE		0
#define CDROM_IRQ_DATA_READY	1
#define CDROM_IRQ_COMPLETE	2
#define CDROM_IRQ_ACKNOWLEDGE	3
#define CDROM_IRQ_DATA_END	4
#define CDROM_IRQ_ERROR		5
typedef uint32_t cdrom_irq_type_t;

#define CDROM_CMD_STAT_ERROR		(1u << 0)
#define CDROM_CMD_STAT_SPINDLE_ON	(1u << 1)
#define CDROM_CMD_STAT_SEEK_ERROR	(1u << 2)
#define CDROM_CMD_STAT_ID_ERROR		(1u << 3)
#define CDROM_CMD_STAT_LID_OPEN		(1u << 4)
#define CDROM_CMD_STAT_READING		(1u << 5)
#define CDROM_CMD_STAT_SEEKING		(1u << 6)
#define CDROM_CMD_STAT_PLAYING		(1u << 7)
typedef uint32_t cdrom_cmd_stat_flag_t;

#define CDROM_CMD_ERR_SEEK_FAILED		(1u << 2)
#define CDROM_CMD_ERR_LID_OPENED		(1u << 3)
#define CDROM_CMD_ERR_INVALID_PARAM_VALUE	(1u << 4)
#define CDROM_CMD_ERR_INVALID_PARAM_COUNT	(1u << 5)
#define CDROM_CMD_ERR_INVALID_COMMAND		(1u << 6)
#define CDROM_CMD_ERR_NO_DISC			(1u << 7)
typedef uint32_t cdrom_cmd_error_flag_t;

#define CDROM_MODE_CDDA			(1u << 0)
#define CDROM_MODE_AUTO_PAUSE		(1u << 1)
#define CDROM_MODE_CDDA_REPORT		(1u << 2)
#define CDROM_MODE_XA_FILTER		(1u << 3)
#define CDROM_MODE_SIZE_BITMASK		(3u << 4)
#define CDROM_MODE_SIZE_2048		(0u << 4)
#define CDROM_MODE_SIZE_2328		(1u << 4)
#define CDROM_MODE_SIZE_2340		(2u << 4)
#define CDROM_MODE_XA_ADPCM		(1u << 6)
#define CDROM_MODE_SPEED_1X		(0u << 7)
#define CDROM_MODE_SPEED_2X		(1u << 7)
typedef uint32_t cdrom_mode_flag_t;

DEF(size_t)
cdrom_sector_len(
	uint8_t mode
	)
{
	switch (mode & CDROM_MODE_SIZE_BITMASK) {
	case CDROM_MODE_SIZE_2048:
		return (2048);

	case CDROM_MODE_SIZE_2328:
		return (2328);

	case CDROM_MODE_SIZE_2340:
		return (2340);

	default:
		return (0);
	}
}

#undef DEF
#undef PACKED

/*
 * CD-ROM driver
 */

void cdrom_init(void);
void cdrom_setup_audio(bool mono);
void cdrom_await_dma(void);
bool cdrom_pause_read(void);

void
cdrom_cmd(
	cdrom_cmd_t cmd,
	const uint8_t *params,
	size_t param_len
	);

cdrom_irq_type_t
cdrom_check_irq(
	uint8_t *resp,
	size_t max_resp_len
	);

cdrom_irq_type_t
cdrom_await_irq(
	uint8_t *resp,
	size_t max_resp_len
	);

bool
cdrom_play_cdda(
	uint32_t track,
	uint8_t mode
	);

bool
cdrom_start_read(
	uint32_t lba,
	uint8_t mode,
	bool retry
	);

void
cdrom_read_sector(
	void *data,
	size_t len
	);

size_t
cdrom_read_sectors(
	void *data,
	uint32_t lba,
	size_t num_sectors,
	uint8_t mode
	);

#endif /* _SYS__CDROM_H */
