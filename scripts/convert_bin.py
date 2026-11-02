#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>

"""
Convert a binary file to a C array
Don't overuse this!
"""

from argparse import ArgumentParser, Namespace
import os
import sys

def dataToArray(
	data: list[int],
	name: str,
	cols: int
):
	print(f"static const uint8_t {name}[{len(data)}] = {{")
	for i in range(0, len(data), cols):
		chunk = data[i:i+cols]
		print("\t" + ", ".join(f"0x{b:02X}" for b in chunk) + ",")
	print("};")

## Main

def createParser() -> ArgumentParser:
	parser = ArgumentParser(
		description = \
			"Convert a binary file to a C array.",
		add_help    = False
	)

	group = parser.add_argument_group("Tool options")
	group.add_argument(
		"-h", "--help",
		action = "help",
		help   = "Show this help message and exit"
	)

	group = parser.add_argument_group("Input options")
	group.add_argument(
		"input",
		type  = str,
		help  = "Path to input file"
	)
	group.add_argument(
		"-n", "--name",
		type = str,
		help = "Array name"
	)
	group.add_argument(
		"-c", "--cols",
		type = int,
		default = 12,
		help = "Maximum number of hex columns in the data array"
	)

	return parser

def main():
	parser: ArgumentParser = createParser()
	args:   Namespace      = parser.parse_args()

	with open(args.input, 'rb') as f:
		data = f.read()

	if args.name is None:
		args.name = os.path.splitext(os.path.basename(args.input))[0]

	dataToArray(data, args.name, args.cols)

if __name__ == "__main__":
	main()
