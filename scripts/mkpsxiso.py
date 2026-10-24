#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
# Copyright (c) 2023-2026 spicyjpeg
# Copyright (c) 2026 Alexey Laurentsyeu <alex@paidbsd.org>

"""
PlayStation 1 disc image creator

A simple script to create a raw 2352-byte sector disc image from a directory.
"""

__version__ = "0.0.1"
__author__ = "9vlc"

from argparse import ArgumentParser
from collections import deque
import datetime
import os
import struct
import sys
from typing import BinaryIO

SYNC_HEADER = b'\x00\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x00'

EDC_TABLE: list[int] = [0] * 256
GF_EXP:    list[int] = [0] * 512
GF_LOG:    list[int] = [0] * 256

class DirNode:
	def __init__(
		self,
		name:     str,
		hostPath: str,
		parent:   "DirNode | None" = None
	) -> None:
		self.name     = name
		self.hostPath = hostPath
		self.parent   = parent or self
		self.subdirs: list[DirNode]  = []
		self.files:   list[FileNode] = []
		self.index = 1
		self.lba   = 0
		self.size  = 0
		self.date  = datetime.datetime.fromtimestamp(
			os.path.getmtime(hostPath), datetime.timezone.utc
		)

class FileNode:
	def __init__(
		self,
		name:     str,
		hostPath: str
	) -> None:
		self.name     = name
		self.hostPath = hostPath
		self.lba      = 0
		self.size     = os.path.getsize(hostPath)
		self.date     = datetime.datetime.fromtimestamp(
			os.path.getmtime(hostPath), datetime.timezone.utc
		)

class DiscContext:
	def __init__(
		self,
		input:       str,
		output:      str,
		licensePath: str = None,
		volId:       str = "PSX" # PYTHOOON, why can't I place args without a default after one with a default?????
	) -> None:
		self.input       = input
		self.licensePath = licensePath
		self.volId       = volId

		if output.endswith(".bin"):
			self.binPath = output
			self.cuePath = output[:-4] + ".cue"
		else:
			self.binPath = output + ".bin"
			self.cuePath = output + ".cue"

		self.dirs:  list[DirNode]  = []
		self.files: list[FileNode] = []

		self.ptSize       = 0
		self.lPtLba       = 0
		self.mPtLba       = 0
		self.totalSectors = 0
		self.padSectors   = 0
		self.date         = datetime.datetime.now(datetime.timezone.utc)

def initTables() -> None:
	for i in range(256):
		edc = i
		for _ in range(8):
			if edc & 1:
				edc = (edc >> 1) ^ 0xD8018001
			else:
				edc >>= 1
		EDC_TABLE[i] = edc

	x = 1
	for i in range(255):
		GF_EXP[i] = x
		GF_EXP[i + 255] = x
		GF_LOG[x] = i
		x <<= 1
		if x & 0x100:
			x ^= 0x11D
	GF_EXP[510] = 1

def computeEdc(
	data: bytes
) -> bytes:
	edc = 0
	for b in data:
		edc = (edc >> 8) ^ EDC_TABLE[(edc ^ b) & 0xFF]
	return struct.pack('<I', edc)

def computeEcc(
	data: bytes
) -> bytes:
	block = bytearray(4) + data
	eccCol = bytearray(172)
	eccRow = bytearray(104)

	for col in range(43):
		p0_0 = p0_1 = p1_0 = p1_1 = 0
		idx = col * 2
		for row in range(24):
			v0 = block[idx]
			v1 = block[idx + 1]
			p0_0 ^= v0
			p0_1 ^= v1
			if v0:
				p1_0 ^= GF_EXP[GF_LOG[v0] + 24 - row]
			if v1:
				p1_1 ^= GF_EXP[GF_LOG[v1] + 24 - row]
			idx += 86

		a0 = p1_0 ^ (GF_EXP[GF_LOG[p0_0] + 25] if p0_0 else 0)
		b0 = GF_EXP[GF_LOG[a0] + 231] if a0 else 0
		c0 = p0_0 ^ b0

		a1 = p1_1 ^ (GF_EXP[GF_LOG[p0_1] + 25] if p0_1 else 0)
		b1 = GF_EXP[GF_LOG[a1] + 231] if a1 else 0
		c1 = p0_1 ^ b1

		eccCol[col * 2] = b0
		eccCol[col * 2 + 1] = b1
		eccCol[86 + col * 2] = c0
		eccCol[86 + col * 2 + 1] = c1

	block.extend(eccCol)

	for row in range(26):
		q0_0 = q0_1 = q1_0 = q1_1 = 0
		for col in range(43):
			idx = ((row * 86 + col * 88) % 2236)
			v0 = block[idx]
			v1 = block[idx + 1]
			q0_0 ^= v0
			q0_1 ^= v1
			if v0:
				q1_0 ^= GF_EXP[GF_LOG[v0] + 43 - col]
			if v1:
				q1_1 ^= GF_EXP[GF_LOG[v1] + 43 - col]

		a0 = q1_0 ^ (GF_EXP[GF_LOG[q0_0] + 44] if q0_0 else 0)
		b0 = GF_EXP[GF_LOG[a0] + 212] if a0 else 0
		c0 = q0_0 ^ b0

		a1 = q1_1 ^ (GF_EXP[GF_LOG[q0_1] + 44] if q0_1 else 0)
		b1 = GF_EXP[GF_LOG[a1] + 212] if a1 else 0
		c1 = q0_1 ^ b1

		eccRow[row * 2] = b0
		eccRow[row * 2 + 1] = b1
		eccRow[52 + row * 2] = c0
		eccRow[52 + row * 2 + 1] = c1

	return bytes(eccCol) + bytes(eccRow)

def toBcd(
	val: int
) -> int:
	return ((val // 10) << 4) | (val % 10)

def lbaToMsf(
	lba: int
) -> bytes:
	frames = lba + 150
	m = frames // (60 * 75)
	rem = frames % (60 * 75)
	s = rem // 75
	f = rem % 75

	return bytes([toBcd(m), toBcd(s), toBcd(f), 0x02])

def makeXASUA(
	isDir: bool = False
) -> bytes:
	attr = 0x8D55 if isDir else 0x0D55

	# gid, uid, attrs, "XA", filenum, reserved
	return struct.pack(">HHH2sB5s", 0, 0, attr, b"XA", 0, b"\x00" * 5)

def makeSector(
	lba:     int,
	data:    bytes,
	subMode: int  = 0x08,
	zeroEdc: bool = False
) -> bytes:
	msf    = lbaToMsf(lba)
	subHdr = bytes([0x00, 0x00, subMode, 0x00, 0x00, 0x00, subMode, 0x00])
	body   = subHdr + data

	if zeroEdc:
		edc = b'\x00\x00\x00\x00'
	else:
		edc = computeEdc(body)

	ecc = computeEcc(body + edc)

	return SYNC_HEADER + msf + body + edc + ecc

def biendU16(
	val: int
) -> bytes:
	return struct.pack('<H', val) + struct.pack('>H', val)

def biendU32(
	val: int
) -> bytes:
	return struct.pack('<I', val) + struct.pack('>I', val)

def padStr(
	string:  str,
	length:  int,
	padChar: bytes = b' '
) -> bytes:
	end = string.encode('ascii', errors='replace')[:length]
	return end + padChar * (length - len(end))

def padSectors(
	data: bytes
) -> bytes:
	rem = len(data) % 2048
	if rem != 0:
		return data + b'\x00' * (2048 - rem)
	return data

def cleanName(
	name:  str,
	isDir: bool = False
) -> str:
	name = name.upper()
	validChars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_'

	if isDir:
		cleaned = ''.join(c if c in validChars else '_' for c in name)[:8]
		return cleaned or 'DIR'

	if '.' in name:
		base, ext = name.split('.', 1)
	else:
		base, ext = name, ''

	base = ''.join(c if c in validChars else '_' for c in base)[:8] or 'FILE'
	ext = ''.join(c if c in validChars else '_' for c in ext)[:3]

	if ext:
		return f'{base}.{ext};1'
	else:
		return f'{base}.;1'

def convertDate(
	date: datetime.datetime
) -> bytes:
	return bytes([
		date.year - 1900,
		date.month,
		date.day,
		date.hour,
		date.minute,
		date.second,
		0
	])

def makePvdDate(
	date: datetime.datetime
) -> bytes:
	s = \
		f"{date.year:04d}{date.month:02d}{date.day:02d}" \
		f"{date.hour:02d}{date.minute:02d}{date.second:02d}00"

	return s.encode('ascii') + b'\x00'

def makeRecord(
	lba:   int,
	size:  int,
	date:  datetime.datetime,
	isDir: bool,
	ident: bytes
) -> bytes:
	flags    = 0x02 if isDir else 0x00
	identLen = len(ident)
	padLen   = 1 if (identLen % 2 == 0) else 0
	sua      = makeXASUA(isDir)
	recLen   = 33 + identLen + padLen + len(sua)

	rec = bytearray()
	rec.append(recLen)
	rec.append(0)
	rec.extend(biendU32(lba))
	rec.extend(biendU32(size))
	rec.extend(convertDate(date))
	rec.append(flags)
	rec.append(0)
	rec.append(0)
	rec.extend(biendU16(1))
	rec.append(identLen)
	rec.extend(ident)
	if padLen:
		rec.append(0)
	rec.extend(sua)

	return bytes(rec)

def scanTree(
	path: str
) -> list[DirNode]:
	root  = DirNode('', path)
	dirs  = [root]
	queue = deque([root])

	while queue:
		curr    = queue.popleft()
		entries = sorted(os.listdir(curr.hostPath), key = str.upper)

		for entry in entries:
			if entry.startswith('.'):
				continue

			full = os.path.join(curr.hostPath, entry)

			if os.path.isdir(full):
				node = DirNode(cleanName(entry, True), full, curr)
				curr.subdirs.append(node)
				dirs.append(node)
				queue.append(node)
			elif os.path.isfile(full):
				curr.files.append(FileNode(cleanName(entry), full))

	for index, node in enumerate(dirs):
		node.index = index + 1

	return dirs

def packDirRecords(
	node: DirNode
) -> bytes:
	recs = [
		makeRecord(node.lba, node.size, node.date, True, b'\x00'),
		makeRecord(
			node.parent.lba, node.parent.size, node.parent.date, True, b'\x01'
		)
	]

	entries = []
	for subdir in node.subdirs:
		entries.append((subdir.name, True, subdir))
	for file in node.files:
		entries.append((file.name, False, file))
	entries.sort(key = lambda entry: entry[0])

	for name, isDir, entry in entries:
		recs.append(
			makeRecord(entry.lba, entry.size, entry.date, isDir,
				name.encode('ascii'))
		)

	sectors = []
	sector  = bytearray()

	for rec in recs:
		if len(sector) + len(rec) > 2048:
			sector.extend(b'\x00' * (2048 - len(sector)))
			sectors.append(bytes(sector))
			sector = bytearray()
		sector.extend(rec)

	if sector or not sectors:
		sector.extend(b'\x00' * (2048 - len(sector)))
		sectors.append(bytes(sector))

	return b''.join(sectors)

def makePathTable(
	dirs:      list[DirNode],
	bigEndian: bool = False
) -> bytes:
	u16 = '>H' if bigEndian else '<H'
	u32 = '>I' if bigEndian else '<I'
	pt  = bytearray()

	for node in dirs:
		ident = b'\x00' if node.name == '' else node.name.encode('ascii')
		pt.append(len(ident))
		pt.append(0)
		pt.extend(struct.pack(u32, node.lba))
		pt.extend(struct.pack(u16, node.parent.index))
		pt.extend(ident)
		if len(ident) % 2 == 1:
			pt.append(0)

	return bytes(pt)

def makePvd(
	volId:        str,
	totalSectors: int,
	ptSize:       int,
	lPtLba:       int,
	mPtLba:       int,
	root:         DirNode,
	date:         datetime.datetime
) -> bytes:
	pvd = bytearray(2048)
	pvd[0]       = 1
	pvd[1:6]     = b'CD001'
	pvd[6]       = 1
	pvd[8:40]    = padStr('PLAYSTATION', 32)
	pvd[40:72]   = padStr(volId.upper(), 32)
	pvd[80:88]   = biendU32(totalSectors)
	pvd[120:124] = biendU16(1)
	pvd[124:128] = biendU16(1)
	pvd[128:132] = biendU16(2048)
	pvd[132:140] = biendU32(ptSize)
	pvd[140:144] = struct.pack('<I', lPtLba)
	pvd[144:148] = struct.pack('<I', lPtLba + 1)
	pvd[148:152] = struct.pack('>I', mPtLba)
	pvd[152:156] = struct.pack('>I', mPtLba + 1)

	rootRec = bytearray(makeRecord(
		root.lba, root.size, root.date, True, b'\x00'
	)[:34])
	rootRec[0]   = 34
	pvd[156:190] = rootRec

	pvd[190:318] = padStr('', 128)
	pvd[318:446] = padStr('', 128)
	pvd[446:574] = padStr('', 128)
	pvd[574:702] = padStr('PLAYSTATION', 128)
	pvd[702:739] = padStr('', 37)
	pvd[739:776] = padStr('', 37)
	pvd[776:813] = padStr('', 37)

	dateBytes    = makePvdDate(date)
	zeroDate     = b'0000000000000000\x00'
	pvd[813:830] = dateBytes
	pvd[830:847] = dateBytes
	pvd[847:864] = zeroDate
	pvd[864:881] = zeroDate
	pvd[881]     = 1

	pvd[1024:1032] = b'CD-XA001'

	return bytes(pvd)

def makeVdTerminator() -> bytes:
	vdt      = bytearray(2048)
	vdt[0]   = 255
	vdt[1:6] = b'CD001'
	vdt[6]   = 1

	return bytes(vdt)

def makeSystemArea(
	licensePath: str | None
) -> bytes:
	area    = bytearray()
	data    = b'\x00' * (16 * 2048)
	raw     = False
	zeroEdc = False

	if licensePath is not None:
		with open(licensePath, 'rb') as f:
			data = f.read()

		if len(data) == 24576:
			data    += b'\x00' * 8192
			zeroEdc = True
		elif len(data) == 32768:
			zeroEdc = True
		elif len(data) == 37632:
			raw = True
		else:
			sys.exit(
				f"error: {licensePath}: "
				"license must be 24576, 32768 or 37632 bytes"
			)

	for i in range(16):
		if raw:
			area.extend(data[i * 2352:(i + 1) * 2352])
		else:
			sector = data[i * 2048:(i + 1) * 2048]
			area.extend(makeSector(i, sector, 0x08, zeroEdc and 12 <= i <= 15))

	return bytes(area)

def layoutDisc(
	ctx: DiscContext
) -> None:
	ctx.dirs = scanTree(ctx.input)

	for node in ctx.dirs:
		node.size = len(packDirRecords(node))

	ctx.ptSize = len(makePathTable(ctx.dirs))
	ptSectors  = (ctx.ptSize + 2047) // 2048

	ctx.lPtLba = 18
	ctx.mPtLba = 18 + ptSectors * 2
	lba        = 18 + ptSectors * 4

	for node in ctx.dirs:
		node.lba = lba
		lba += node.size // 2048

	for node in ctx.dirs:
		for file in node.files:
			file.lba = lba
			lba += (file.size + 2047) // 2048 if file.size > 0 else 0
			ctx.files.append(file)

	# Pad
	ctx.totalSectors = max(300, lba + 150)
	ctx.padSectors   = ctx.totalSectors - lba

def writeExtent(
	out:  BinaryIO,
	lba:  int,
	blob: bytes
) -> int:
	for off in range(0, len(blob), 2048):
		isLast = (off + 2048 >= len(blob))

		# Submode 0x89 = EOF (0x80) | Data (0x08) | EOR (0x01)
		subMode = 0x89 if isLast else 0x08
		out.write(makeSector(lba, blob[off:off + 2048], subMode))
		lba += 1

	return lba

def writeDisc(
	ctx: DiscContext
) -> None:
	with open(ctx.binPath, 'wb') as out:
		out.write(makeSystemArea(ctx.licensePath))

		pvd = makePvd(
			ctx.volId, ctx.totalSectors, ctx.ptSize,
			ctx.lPtLba, ctx.mPtLba, ctx.dirs[0], ctx.date
		)
		out.write(makeSector(16, pvd, 0x08))
		out.write(makeSector(17, makeVdTerminator(), 0x88))

		lba = 18
		lPt = padSectors(makePathTable(ctx.dirs))
		mPt = padSectors(makePathTable(ctx.dirs, True))

		for blob in (lPt, lPt, mPt, mPt):
			lba = writeExtent(out, lba, blob)

		for node in ctx.dirs:
			lba = writeExtent(out, lba, packDirRecords(node))

		for file in ctx.files:
			if file.size == 0:
				continue

			sectors = (file.size + 2047) // 2048
			with open(file.hostPath, 'rb') as fp:
				for i in range(sectors):
					chunk = fp.read(2048)
					if len(chunk) < 2048:
						chunk += b'\x00' * (2048 - len(chunk))

					subMode = 0x89 if i == sectors - 1 else 0x08
					out.write(makeSector(lba, chunk, subMode))
					lba += 1

		for _ in range(ctx.padSectors):
			out.write(makeSector(lba, b'\x00' * 2048, 0x00))
			lba += 1

def writeCue(
	ctx: DiscContext
) -> None:
	binBase = os.path.basename(ctx.binPath)

	with open(ctx.cuePath, 'w') as cue:
		cue.write(f'FILE "{binBase}" BINARY\n')
		cue.write('  TRACK 01 MODE2/2352\n')
		cue.write('    INDEX 01 00:00:00\n')

## Main

def createParser() -> ArgumentParser:
	parser = ArgumentParser(
		description = \
			"Creates a PlayStation 1 .BIN+.CUE disc from a directory",
		add_help    = False
	)

	group = parser.add_argument_group("Tool options")
	group.add_argument(
		"-h", "--help",
		action = "help",
		help   = "Show this help message and exit"
	)

	group = parser.add_argument_group("Disc options")
	group.add_argument(
		"-l", "--license",
		type = str,
		help = "Disc license data (first 16 sectors)"
	)
	group.add_argument(
		"-v", "--volid",
		type = str,
		default = "PSX",
		help = "ISO 9660 Volume ID (default: PSX)"
	)

	group = parser.add_argument_group("File paths")
	group.add_argument(
		"input",
		type = str,
		help = "Path to input directory"
	)
	group.add_argument(
		"output",
		type = str,
		help = "Path to output bin"
	)

	return parser

def main() -> None:
	parser = createParser()
	args   = parser.parse_args()

	if not os.path.isdir(args.input):
		parser.error(f"not a directory: {args.input}")

	initTables()

	ctx = DiscContext(
		args.input, args.output, args.license, args.volid
	)
	layoutDisc(ctx)
	writeDisc(ctx)
	writeCue(ctx)

if __name__ == '__main__':
	main()
