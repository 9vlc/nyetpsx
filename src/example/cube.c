/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2023-2026 spicyjpeg
 * Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>
 */

#include <psx/gpu.h>

#include <graph/gpu.h>
#include <graph/gte.h>
#include <graph/geom.h>
#include <graph/dmadraw.h>

#include <stdint.h>

#include "example.h"

/* Textured quad face with a normal */
typedef struct {
	uint8_t vert[4];
	graph_texture_info_t *tex;
	uint8_t norm;
} qface_t;

static const gte_vec16_t cube_verts[] = {
	{ .x = -CUBE_SIZE, .y = -CUBE_SIZE, .z = -CUBE_SIZE },
	{ .x =  CUBE_SIZE, .y = -CUBE_SIZE, .z = -CUBE_SIZE },
	{ .x = -CUBE_SIZE, .y =  CUBE_SIZE, .z = -CUBE_SIZE },
	{ .x =  CUBE_SIZE, .y =  CUBE_SIZE, .z = -CUBE_SIZE },
	{ .x = -CUBE_SIZE, .y = -CUBE_SIZE, .z =  CUBE_SIZE },
	{ .x =  CUBE_SIZE, .y = -CUBE_SIZE, .z =  CUBE_SIZE },
	{ .x = -CUBE_SIZE, .y =  CUBE_SIZE, .z =  CUBE_SIZE },
	{ .x =  CUBE_SIZE, .y =  CUBE_SIZE, .z =  CUBE_SIZE }
};

static const gte_vec16_t cube_normals[] = {
	{ .x =         0, .y =         0, .z = -GTE_UNIT },
	{ .x =         0, .y =         0, .z =  GTE_UNIT },
	{ .x =         0, .y = -GTE_UNIT, .z =         0 },
	{ .x =         0, .y =  GTE_UNIT, .z =         0 },
	{ .x = -GTE_UNIT, .y =         0, .z =         0 },
	{ .x =  GTE_UNIT, .y =         0, .z =         0 }
};

static qface_t cube_faces[] = {
	{ .vert = { 0, 1, 2, 3 }, .norm = 0 },
	{ .vert = { 6, 7, 4, 5 }, .norm = 1 },
	{ .vert = { 4, 5, 0, 1 }, .norm = 2 },
	{ .vert = { 7, 6, 3, 2 }, .norm = 3 },
	{ .vert = { 6, 4, 2, 0 }, .norm = 4 },
	{ .vert = { 5, 7, 1, 3 }, .norm = 5 }
};
#define NUM_CUBE_FACES (sizeof(cube_faces) / sizeof(qface_t))

static uint32_t cube_current_frame = 0;

/*
 * Initialize some GTE vars for the spinning cube.
 * Also call graph_gte_init() before this!
 */
void
cube_init(
	graph_texture_info_t *tex
	)
{
	/* Camera position */
	gte_set_ctrl_reg(GTE_TRX, CUBE_CAMERA_X);
	gte_set_ctrl_reg(GTE_TRY, CUBE_CAMERA_Y);
	gte_set_ctrl_reg(GTE_TRZ, CUBE_CAMERA_Z);

	/* Light sources */
	gte_set_light_col_mtx(
		GTE_UNIT * 2, GTE_UNIT / 2, 0,
		GTE_UNIT * 2, GTE_UNIT / 2, 0,
		GTE_UNIT * 2, GTE_UNIT / 2, 0
	);

	/* Ambient light color */
	gte_set_ctrl_reg(GTE_RBK, GTE_UNIT / 2);
	gte_set_ctrl_reg(GTE_GBK, GTE_UNIT / 2);
	gte_set_ctrl_reg(GTE_BBK, GTE_UNIT / 2);

	for (int i = 0; i < NUM_CUBE_FACES; i++) {
		cube_faces[i].tex = tex;
	}
}

/*
 * Put a single frame of the cube to screen and increment the frame counter
 */
void
cube_render(
	graph_dma_ord_chain_t *chain
	)
{
	gte_set_rot_mtx(
		GTE_UNIT, 0,        0,
		0,        GTE_UNIT, 0,
		0,        0,        GTE_UNIT
	);
	gte_rot_mtx_x(cube_current_frame * 12);
	gte_rot_mtx_y(cube_current_frame * 16);

	gte_set_light_mtx(
		0,          -GTE_UNIT,  0,
		GTE_RSQRT3, GTE_RSQRT3, -GTE_RSQRT3,
		0,          0,          0
	);

	gte_tf_light_mtx();

	for (int i = 0; i < NUM_CUBE_FACES; i++) {
		const qface_t *face = &cube_faces[i];

		/*
		 * Apply perspective projection to three vertices.
		 * The GTE can only process up to three vertices at once.
		 *
		 * Since we're using quads rather than triangles, we'll have to
		 *  transform the last one separately.
		 * The SF flag indicates we're working in the Q4.12 number format.
		 */
		gte_load_v0(&cube_verts[face->vert[0]]);
		gte_load_v1(&cube_verts[face->vert[1]]);
		gte_load_v2(&cube_verts[face->vert[2]]);
		gte_cmd(GTE_CMD_RTPT | GTE_SF);

		/*
		 * Get the winding of the projected vertices and area of their
		 *   respective triangle using the shoelace formula.
		 * If they are ordered clockwise then the face is visible,
		 *   otherwise it can be culled as it's not facing the camera.
		 *
		 * Note that gte_get_data_reg() returns a uint32_t, while some
		 *   registers contain a signed value, so it is required to
		 *   do a cast to int32_t for some data registers.
		 */
		gte_cmd(GTE_CMD_NCLIP);
		int32_t area = (int32_t)gte_get_data_reg(GTE_MAC0);

		if (area <= 0) {
			continue;
		}

		/*
		 * Save the first vertex and project the last one in its place
		 */
		uint32_t xy0 = gte_get_data_reg(GTE_SXY0);
		gte_load_v0(&cube_verts[face->vert[3]]);
		gte_cmd(GTE_CMD_RTPS | GTE_SF);

		/*
		 * Compute the average Z coordinate of all four verices and use
		 *   it to determine the ordering table bucket index for this face.
		 */
		gte_cmd(GTE_CMD_AVSZ4 | GTE_SF);
		int32_t z = (int32_t)gte_get_data_reg(GTE_OTZ);

		if (z < 0 || z > Z_MAX) {
			continue;
		}

		/*
		 * Feed the first GP0 command word containing the face into the
		 *   GTE alongside the face's normal, allowing it to apply light
		 *   sources and patch the resulting color back into the command.
		 *
		 * The LM flag saturates negative lights to zero, preventing the
		 *   GTE from darkening the face below the ambient color.
		 */
		gte_set_data_reg(GTE_RGBC, COL_CUBE.raw | gp0_shad_quad(false, true, false));
		gte_load_v0(&cube_normals[face->norm]);
		gte_cmd(GTE_CMD_NCCS | GTE_SF | GTE_LM);

		/*
		 * Manually add a GP0 command with colors patched by the GTE to the chain
		 */
		uint32_t *ptr = graph_alloc_ord_chain_packet(chain, z, 9);
		int	u0 = face->tex->u,
			v0 = face->tex->v,
			u1 = face->tex->u + face->tex->w - 1,
			v1 = face->tex->v + face->tex->h - 1;

		gte_store_data_reg(GTE_RGB2, &ptr[0]);
		ptr[1] = xy0;
		ptr[2] = gp0_uv(u0, v0, face->tex->clut);

		gte_store_data_reg(GTE_SXY0, &ptr[3]);
		ptr[4] = gp0_uv(u1, v0, face->tex->page);

		gte_store_data_reg(GTE_SXY1, &ptr[5]);
		ptr[6] = gp0_uv(u0, v1, 0);

		gte_store_data_reg(GTE_SXY2, &ptr[7]);
		ptr[8] = gp0_uv(u1, v1, 0);
	}

	cube_current_frame += 3;
}
