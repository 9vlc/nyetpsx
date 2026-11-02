![nyetpsx](resources/nyetpsx_logo.png)
------
A very opinionated fork of spicyjpeg's ps1-bare-metal.

# Status
Fully usable as a development library!

This project expands upon ps1-bare-metal by adding abstraction layers for various hardware while
still allowing you to directly interface with it without much trouble just like in the original.

Also, this project fully changes the C style to be what I consider a
nice in-between - a mix of FreeBSD's, Intel's, spicyjpeg's and mine C styles.

Currently, the only documentation is the [example program](src/example/).

# Example
The example program is a spinning cube with a mandelbrot texture generated on the fly.
![Spinning cube demo GIF](resources/nyetpsx_demo.gif)

# How to build
You need:
- bmake (BSD Make)
- mb (submodule, just run `git submodule init && git submodule update`)
- A MIPS cross-compiler toolchain:
  - GCC *(mipsel-\*-elf-\*, can be obtained using [this script](https://github.com/grumpycoders/pcsx-redux/blob/main/tools/linux-mips/spawn-compiler.sh))*
  - LLVM *(No support for GTE)*

To build the library and the example, you can just run `bmake`. nyetpsx will try to find a fitting GCC version and build itself with it.

You will find the output in `build/out`, library file named `libnyetpsx.a`.

Available build variables (can be passed over commandline or environment):
- `NYETPSX_TC(=GCC)` - Toolchain to use, set to "`LLVM`" to use LLVM
- `NYETPSX_USE_SDATA(=1)` - Rely on MIPS small data (`.sdata`), very slight performance increase
- `NYETPSX_BUILD_EXAMPLE(=1)` - Build the example EXE
- `NYETPSX_SOURCE_TC(=0)` - Find the toolchain and set toolchain-specific CFLAGS
- `NYETPSX_STDIO_USE_SIO1(=0)` - Use serial link port for stdio / printf
- `CFLAGS_ADD` - Appends to CFLAGS
- `LDFLAGS_ADD` - Appends to LDFLAGS

# About emulators
A lot of PlayStation 1 emulators are very inaccurate, so it's advised to use real hardware for testing programs built with this SDK.

If you can only use an emulator, the following emulators are mostly accurate:
- Duckstation
- Mednafen *(No UI by default)*
- PCSX-Redux *(Has recently become an AI slop project and lost in some accuracy together with becoming pretty slow)*

The following emulators are deemed not accurate enough by me and others:
- Xebra
- ePSXe
- psxfin / pSX *(Very inaccurate GPU and GTE)*
- PCSX and its derivatives other than PCSX-Redux
- Sony's official emulators
- Various RetroArch emulators such as Beetle PSX
- no$psx *(Has some GTE quirks, example program does not spin the cube on it)*

# TODO
- `graph` reference manual
- Comments pointing to useful psx-spx pages in code
- Styling documentation
- Better controller, SPU and CD-ROM abstraction layer
- Something for dealing with memory cards
- LLVM GTE / COP2 instructions

# Acknowledgements
This project would not have been possible without these amazing people:
- spicyjpeg - https://github.com/spicyjpeg/
- JonaB03
- Ekleipsis1502 - https://www.youtube.com/@ekleipsis1502
- The Humanbrew Discord Server - https://discord.gg/KB6uxMBaNq

And these projects:
- ps1-bare-metal - https://github.com/spicyjpeg/ps1-bare-metal/
- arith64 - https://github.com/glitchub/arith64/
- oldschool-pc-fonts - https://int10h.org/oldschool-pc-fonts/
- mandelbrot-c - https://github.com/jesper-olsen/mandelbrot-c/
