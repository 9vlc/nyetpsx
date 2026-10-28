#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
# Copyright (c) 2023-2026 spicyjpeg
# Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>

"""
Modified convert_image.py for fonts

Takes a sheet of 96 ASCII characters starting from space and converts them to a
font array for nyetpsx.
"""

from argparse import ArgumentParser, Namespace
import shlex
import sys

import numpy as np
from numpy.typing import NDArray
from PIL import Image

def dataToArray(
	data: list[int],
	name: str,
):
	print(f"static const uint8_t {name}[{len(data)}] = {{")
	for i in range(0, len(data), 12):
		chunk = data[i:i+12]
		print("\t" + ", ".join(f"0x{b:02X}" for b in chunk) + ",")
	print("};")

## Input image handling

# Pillow's built-in quantize() method will use different algorithms, some of
# which are broken, depending on whether the input image has an alpha channel.
# As a workaround, all images are "quantized" (with no dithering - images with
# more colors than allowed are rejected) manually instead. This conversion is
# performed on indexed color images as well in order to normalize their
# palettes.
def quantizeImage(
	imageObj:  Image.Image,
	numColors: int
) -> Image.Image:
	#if imageObj.mode == "P":
		#return imageObj
	if imageObj.mode not in ( "RGB", "RGBA" ):
		imageObj = imageObj.convert("RGBA")

	image: NDArray[np.uint8] = np.asarray(imageObj, "B")
	clut, image              = np.unique(
		image.reshape(( -1, image.shape[2] )),
		return_inverse = True,
		axis           = 0
	)

	if clut.shape[0] > numColors:
		raise RuntimeError(
			f"source image contains {clut.shape[0]} unique colors (must be "
			f"{numColors} or less)"
		)

	image = image.astype("B").reshape((
		imageObj.height,
		imageObj.width
	))
	newObj: Image.Image = Image.fromarray(image, "P")

	newObj.putpalette(clut.tobytes(), imageObj.mode)
	return newObj

def getImagePalette(
	imageObj: Image.Image
) -> NDArray[np.uint8]:
	clut: NDArray[np.uint8] = np.array(imageObj.getpalette("RGBA"), "B")
	clut                    = clut.reshape(( -1, 4 ))

	# Pillow's PNG decoder does not handle indexed color images with alpha
	# correctly, so a workaround is needed here to manually integrate the
	# contents of the image's "tRNs" chunk into the palette.
	if "transparency" in imageObj.info:
		alpha: bytes = imageObj.info["transparency"]
		clut[:, 3]   = np.frombuffer(alpha.ljust(clut.shape[0], b"\xff"))

	return clut

## Image data conversion

LOWER_ALPHA_BOUND: int =  32
UPPER_ALPHA_BOUND: int = 224

# Color 0x0000 is interpreted by the PS1 GPU as fully transparent, so solid
# black pixels must be changed to either solid dark gray or semitransparent
# black.
TRANSPARENT_COLOR: int = 0x0000
BLACK_COLOR:       int = 0x0421
STP_BLACK_COLOR:   int = 0x8000

def to16bpp(
	inputData:   NDArray[np.uint8],
	forceSTP:    bool = False,
	useSTPBlack: bool = False
) -> NDArray[np.uint16]:
	source: NDArray[np.uint16] = inputData.astype("<H")
	r:      NDArray[np.uint16] = ((source[:, :, 0] * 31) + 127) // 255
	g:      NDArray[np.uint16] = ((source[:, :, 1] * 31) + 127) // 255
	b:      NDArray[np.uint16] = ((source[:, :, 2] * 31) + 127) // 255

	solid:           NDArray[np.uint16] = r | (g << 5) | (b << 10)
	semitransparent: NDArray[np.uint16] = solid | (1 << 15)

	data: NDArray[np.uint16] = np.full_like(solid, TRANSPARENT_COLOR)

	if inputData.shape[2] == 4:
		alpha: NDArray[np.uint8] = inputData[:, :, 3]
	else:
		alpha: NDArray[np.uint8] = np.full(inputData.shape[:-1], 0xff, "B")

	np.copyto(data, semitransparent, where = (alpha >= LOWER_ALPHA_BOUND))

	if not forceSTP:
		np.copyto(data, solid, where = (alpha >= UPPER_ALPHA_BOUND))
		np.copyto(
			data,
			STP_BLACK_COLOR if useSTPBlack else BLACK_COLOR,
			where = (
				(alpha >= UPPER_ALPHA_BOUND) &
				(solid == TRANSPARENT_COLOR)
			)
		)

	return data

def convertIndexedImage(
	imageObj:    Image.Image,
	forceSTP:    bool = False,
	useSTPBlack: bool = False
) -> tuple[NDArray[np.uint8], NDArray[np.uint16]]:
	clut: NDArray[np.uint8] = getImagePalette(imageObj).reshape(( 1, -1, 4 ))
	clut                    = to16bpp(clut, forceSTP, useSTPBlack)

	# Pad the palette to 16 or 256 colors after converting it to 16bpp.
	numColors: int = clut.shape[1]
	padAmount: int = (16 if (numColors <= 16) else 256) - numColors

	if padAmount:
		clut = np.c_[
			clut,
			np.zeros(( 1, padAmount ), "<H")
		]

	image: NDArray[np.uint8] = np.asarray(imageObj, "B")

	if image.shape[1] % 2:
		image = np.c_[
			image,
			np.zeros(( imageObj.height, 1 ), "B")
		]

	# Pack two pixels into each byte for 4bpp images.
	if numColors <= 16:
		image = image[:, 0::2] | (image[:, 1::2] << 4)

		if image.shape[1] % 2:
			image = np.c_[
				image,
				np.zeros(( imageObj.height, 1 ), "B")
			]

	return image, clut

## Main

def createParser() -> ArgumentParser:
	parser = ArgumentParser(
		description = \
			"Convert a 96 symbol ASCII table image to a nyetpsx font.",
		add_help    = False
	)

	group = parser.add_argument_group("Tool options")
	group.add_argument(
		"-h", "--help",
		action = "help",
		help   = "Show this help message and exit"
	)

	group = parser.add_argument_group("Conversion options")
	group.add_argument(
		"-s", "--force-stp",
		action = "store_true",
		help   = \
			"Set the semitransparency/blending flag on all pixels in the "
			"output image (useful when using additive or subtractive blending)"
	)
	group.add_argument(
		"-S", "--stp-black",
		action = "store_true",
		help   = \
			"Use semitransparent black instead of solid dark gray for black "
			"pixels in the image (useful when drawing the image with blending "
			"disabled)"
	)

	group = parser.add_argument_group("Input options")
	group.add_argument(
		"input",
		type  = Image.open,
		help  = "Path to input image file"
	)
	group.add_argument(
		"-c", "--cols",
		type = int,
		default = 16,
		help = "Number of glyph columns in the image"
	)
	group.add_argument(
		"-r", "--rows",
		type = int,
		default = 6,
		help = "Number of glyph rows in the image"
	)
	group.add_argument(
		"-n", "--name",
		type = str,
		default = "default",
		help = "Font name"
	)

	return parser

def main():
	parser: ArgumentParser = createParser()
	args:   Namespace      = parser.parse_args()

	try:
		image: Image.Image = quantizeImage(args.input, 2 ** 4)
	except RuntimeError as err:
		parser.error(err.args[0])

	if image.width % args.cols or image.height % args.rows:
		parser.error(
			f"image size {image.width}x{image.height} is not a whole "
			f"number of {args.cols}x{args.rows} glyphs"
		)
	elif args.cols * args.rows != 96:
		parser.error(
			f"font sheet rows*columns do not equal in 96 characters"
		)

	charWidth:  int = image.width  // args.cols
	charHeight: int = image.height // args.rows

	imageData, clutData = \
		convertIndexedImage(image, args.force_stp, args.stp_black)

	nameLower = args.name.lower()
	nameUpper = args.name.upper()

	print( "/*")
	print( " * Automatically generated with mkfont.py, DO NOT EDIT")
	print( " */")
	print()
	print(f"#ifndef _GRAPH__FONT_{nameUpper}")
	print(f"#define _GRAPH__FONT_{nameUpper}")
	print()
	print( "#include <psx/gpu.h>")
	print( "#include <graph/font.h>")
	print()
	print( "#include <stdint.h>")
	print()
	dataToArray(imageData.tobytes(), f"font_{nameLower}_sheet")
	print()
	dataToArray(clutData.tobytes(), f"font_{nameLower}_clut")
	print()
	print(f"const graph_font_t font_{nameLower} {{")
	print( "\t.data = {")
	print(f"\t\t.img   = font_{nameLower}_sheet,")
	print( "\t\t.x     = 0,")
	print( "\t\t.y     = 0,")
	print(f"\t\t.w     = {image.width},")
	print(f"\t\t.h     = {image.height},")
	print( "")
	print(f"\t\t.clut  = font_{nameLower}_clut,")
	print( "\t\t.cy    = 0,")
	print( "\t\t.cx    = 0,")
	print( "\t\t.depth = GP0_COLOR_4BPP")
	print( "\t},")
	print()
	print(f"\t.cw   = {charWidth},")
	print(f"\t.ch   = {charHeight},")
	print(f"\t.rows = {args.rows},")
	print(f"\t.cols = {args.cols}")
	print( "};")
	print()
	print(f"#endif /* _GRAPH__FONT_{nameUpper}_H */")

if __name__ == "__main__":
	main()
