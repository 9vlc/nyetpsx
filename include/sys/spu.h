/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _SYS__SPU_H
#define _SYS__SPU_H

#include <sys/mmio.h>

/*
 * SPU register flag definitions
 */

enum spu_status_flag {
	SPU_STAT_I2SA_ENABLE		= 1u << 0,
	SPU_STAT_I2SB_ENABLE		= 1u << 1,
	SPU_STAT_I2SA_REVERB		= 1u << 2,
	SPU_STAT_I2SB_REVERB		= 1u << 3,
	SPU_STAT_XFER_BITMASK		= 3u << 4,
	SPU_STAT_XFER_NONE		= 0u << 4,
	SPU_STAT_XFER_WRITE		= 1u << 4,
	SPU_STAT_XFER_DMA_WRITE		= 2u << 4,
	SPU_STAT_XFER_DMA_READ		= 3u << 4,
	SPU_STAT_IRQ			= 1u << 6,
	SPU_STAT_DREQ			= 1u << 7,
	SPU_STAT_WRITE_REQ		= 1u << 8,
	SPU_STAT_READ_REQ		= 1u << 9,
	SPU_STAT_BUSY			= 1u << 10,
	SPU_STAT_CAPTURE_BUF		= 1u << 11
};
typedef enum spu_status_flag spu_status_flag_t;

enum spu_control_flag {
	SPU_CTRL_I2SA_ENABLE		=  1u << 0,
	SPU_CTRL_I2SB_ENABLE		=  1u << 1,
	SPU_CTRL_I2SA_REVERB		=  1u << 2,
	SPU_CTRL_I2SB_REVERB		=  1u << 3,
	SPU_CTRL_XFER_BITMASK		=  3u << 4,
	SPU_CTRL_XFER_NONE		=  0u << 4,
	SPU_CTRL_XFER_WRITE		=  1u << 4,
	SPU_CTRL_XFER_DMA_WRITE		=  2u << 4,
	SPU_CTRL_XFER_DMA_READ		=  3u << 4,
	SPU_CTRL_IRQ_ENABLE		=  1u << 6,
	SPU_CTRL_REVERB_ENABLE		=  1u << 7,
	SPU_CTRL_NOISE_STEP_BITMASK	=  3u << 8,
	SPU_CTRL_NOISE_SHIFT_BITMASK	= 15u << 10,
	SPU_CTRL_DAC_ENABLE		=  1u << 14,
	SPU_CTRL_ENABLE			=  1u << 15
};
typedef enum spu_control_flag spu_control_flag_t;

/*
 * SPU register addresses
 */

/* Per-voice registers */
#define SPU_CH_VOLL(N)			MMIO16((IO_BASE | 0xC00) + (16u * (N)))
#define SPU_CH_VOLR(N)			MMIO16((IO_BASE | 0xC02) + (16u * (N)))
#define SPU_CH_PITCH(N)			MMIO16((IO_BASE | 0xC04) + (16u * (N)))
#define SPU_CH_SSA(N)			MMIO16((IO_BASE | 0xC06) + (16u * (N)))
#define SPU_CH_ADSR1(N)			MMIO16((IO_BASE | 0xC08) + (16u * (N)))
#define SPU_CH_ADSR2(N)			MMIO16((IO_BASE | 0xC0A) + (16u * (N)))
#define SPU_CH_ENVX(N)			MMIO16((IO_BASE | 0xC0C) + (16u * (N)))
#define SPU_CH_LSAX(N)			MMIO16((IO_BASE | 0xC0E) + (16u * (N)))

/* Key on/off and voice control */
#define SPU_MVOLL			MMIO16(IO_BASE | 0xD80)
#define SPU_MVOLR			MMIO16(IO_BASE | 0xD82)
#define SPU_EVOLL			MMIO16(IO_BASE | 0xD84)
#define SPU_EVOLR			MMIO16(IO_BASE | 0xD86)
#define SPU_KON0			MMIO16(IO_BASE | 0xD88)
#define SPU_KON1			MMIO16(IO_BASE | 0xD8A)
#define SPU_KOFF0			MMIO16(IO_BASE | 0xD8C)
#define SPU_KOFF1			MMIO16(IO_BASE | 0xD8E)
#define SPU_PMON0			MMIO16(IO_BASE | 0xD90)
#define SPU_PMON1			MMIO16(IO_BASE | 0xD92)
#define SPU_NON0			MMIO16(IO_BASE | 0xD94)
#define SPU_NON1			MMIO16(IO_BASE | 0xD96)
#define SPU_EON0			MMIO16(IO_BASE | 0xD98)
#define SPU_EON1			MMIO16(IO_BASE | 0xD9A)
#define SPU_ENDX0			MMIO16(IO_BASE | 0xD9C)
#define SPU_ENDX1			MMIO16(IO_BASE | 0xD9E)

/* Sound RAM transfer, IRQ and control/status */
#define SPU_ESA				MMIO16(IO_BASE | 0xDA2)
#define SPU_IRQA			MMIO16(IO_BASE | 0xDA4)
#define SPU_TSA				MMIO16(IO_BASE | 0xDA6)
#define SPU_DATA			MMIO16(IO_BASE | 0xDA8)
#define SPU_CTRL			MMIO16(IO_BASE | 0xDAA)
#define SPU_RAM_CTRL			MMIO16(IO_BASE | 0xDAC)
#define SPU_STAT			MMIO16(IO_BASE | 0xDAE)

/* CD and external audio input volumes */
#define SPU_AVOLL			MMIO16(IO_BASE | 0xDB0)
#define SPU_AVOLR			MMIO16(IO_BASE | 0xDB2)
#define SPU_BVOLL			MMIO16(IO_BASE | 0xDB4)
#define SPU_BVOLR			MMIO16(IO_BASE | 0xDB6)
#define SPU_MVOLXL			MMIO16(IO_BASE | 0xDB8)
#define SPU_MVOLXR			MMIO16(IO_BASE | 0xDBA)

/* Reverb config */
#define SPU_REVERB_DAPF1		MMIO16(IO_BASE | 0xDC0)
#define SPU_REVERB_DAPF2		MMIO16(IO_BASE | 0xDC2)
#define SPU_REVERB_VIIR			MMIO16(IO_BASE | 0xDC4)
#define SPU_REVERB_VCOMB1		MMIO16(IO_BASE | 0xDC6)
#define SPU_REVERB_VCOMB2		MMIO16(IO_BASE | 0xDC8)
#define SPU_REVERB_VCOMB3		MMIO16(IO_BASE | 0xDCA)
#define SPU_REVERB_VCOMB4		MMIO16(IO_BASE | 0xDCC)
#define SPU_REVERB_VCOMB4		MMIO16(IO_BASE | 0xDCC)
#define SPU_REVERB_VWALL		MMIO16(IO_BASE | 0xDCE)
#define SPU_REVERB_VAPF1		MMIO16(IO_BASE | 0xDD0)
#define SPU_REVERB_VAPF2		MMIO16(IO_BASE | 0xDD2)
#define SPU_REVERB_MLSAME		MMIO16(IO_BASE | 0xDD4)
#define SPU_REVERB_MRSAME		MMIO16(IO_BASE | 0xDD6)
#define SPU_REVERB_MLCOMB1		MMIO16(IO_BASE | 0xDD8)
#define SPU_REVERB_MRCOMB1		MMIO16(IO_BASE | 0xDDA)
#define SPU_REVERB_MLCOMB2		MMIO16(IO_BASE | 0xDDC)
#define SPU_REVERB_MRCOMB2		MMIO16(IO_BASE | 0xDDE)
#define SPU_REVERB_DLSAME		MMIO16(IO_BASE | 0xDE0)
#define SPU_REVERB_DRSAME		MMIO16(IO_BASE | 0xDE2)
#define SPU_REVERB_MLDIFF		MMIO16(IO_BASE | 0xDE4)
#define SPU_REVERB_MRDIFF		MMIO16(IO_BASE | 0xDE6)
#define SPU_REVERB_MLCOMB3		MMIO16(IO_BASE | 0xDE8)
#define SPU_REVERB_MRCOMB3		MMIO16(IO_BASE | 0xDEA)
#define SPU_REVERB_MLCOMB4		MMIO16(IO_BASE | 0xDEC)
#define SPU_REVERB_MRCOMB4		MMIO16(IO_BASE | 0xDEE)
#define SPU_REVERB_DLDIFF		MMIO16(IO_BASE | 0xDF0)
#define SPU_REVERB_DRDIFF		MMIO16(IO_BASE | 0xDF2)
#define SPU_REVERB_MLAPF1		MMIO16(IO_BASE | 0xDF4)
#define SPU_REVERB_MRAPF1		MMIO16(IO_BASE | 0xDF6)
#define SPU_REVERB_MLAPF2		MMIO16(IO_BASE | 0xDF8)
#define SPU_REVERB_MRAPF2		MMIO16(IO_BASE | 0xDFA)
#define SPU_REVERB_VLIN			MMIO16(IO_BASE | 0xDFC)
#define SPU_REVERB_VRIN			MMIO16(IO_BASE | 0xDFE)
#define SPU_CH_VOLXL(N)			MMIO16((IO_BASE | 0xE00) + (4u * (N)))
#define SPU_CH_VOLXR(N)			MMIO16((IO_BASE | 0xE02) + (4u * (N)))

#endif /* _SYS__SPU_H */
