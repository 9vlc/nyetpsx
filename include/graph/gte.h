/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#ifndef _GRAPH__GTE_H
#define _GRAPH__GTE_H

#include <sys/gpu.h>

#include <stddef.h>
#include <stdint.h>

void
graph_init_gte(
	uint32_t width,
	uint32_t height
	);

#endif /* _GRAPH__GTE_H */
