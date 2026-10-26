# nyetpsx
A very opinionated C99 fork of spicyjpeg's ps1-bare-metal.

This was meant to be just a tiny libc and a base for my serial loader project, but it turned into probably my biggest project yet.

# Status
Fully usable as a development library, although currently without much documentation.

# How to build
You need:
- bmake (BSD Make)
- mb (submodule, just run `git submodule init && git submodule update`)
- A MIPS cross-compiler:
  - GCC *(mipsel-\*-elf-\*, can be obtained using [this script](https://github.com/grumpycoders/pcsx-redux/blob/main/tools/linux-mips/spawn-compiler.sh))*
  - LLVM *(No support for GTE / COP2 instructions as of now)*

To build the library and the example, you can just run `bmake`. nyetpsx will try to find a fitting GCC version and build itself with it.

You will find the output in `build/out`

Available build variables (can be passed over commandline or environment):
- `NYETPSX_TC(=GCC)` - Toolchain to use, set to "`LLVM`" to use LLVM
- `NYETPSX_USE_SDATA(=1)` - Rely on MIPS small data (`.sdata`), very slight performance increase
- `NYETPSX_BUILD_EXAMPLE(=1)` - Build the example EXE
- `NYETPSX_SOURCE_TC(=0)` - Find the toolchain and set toolchain-specific CFLAGS
- `NYETPSX_STDIO_USE_SIO1(=0)` - Use serial link port for stdio / printf
- `CFLAGS_ADD` - Appends to CFLAGS
- `LDFLAGS_ADD` - Appends to LDFLAGS

# How to use
TODO

# About emulators
A lot of PlayStation 1 emulators are very inaccurate, so it's advised to use real hardware for testing programs built with this SDK.

If you can only use an emulator, the following emulators are relatively accurate:
- Duckstation
- PCSX-Redux *(Very slow, recently switched to being vibe-coded, recompiler not as accurate)*

The following emulators are deemed not accurate enough by me and others:
- Xebra
- ePSXe
- psxfin / pSX *(Very inaccurate GPU and GTE)*
- PCSX and its derivatives other than PCSX-Redux
- Sony's official emulators
- Various RetroArch emulators such as Beetle PSX
- no$psx *(Has some GTE quirks, example program does not spin the cube on it)*

The following emulators are mostly accurate, but have a bad UX:
- Mednafen
