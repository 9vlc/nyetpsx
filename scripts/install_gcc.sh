#!/bin/sh
# Original script by malucart on the humanbrew Discord server
# Modified by 9vlc to be a bit more cross-platform and work on BSD
#
# HOW TO USE:
# 1. Install the following packages:
#    gcc / clang, make, tar, zstd, libelf, bison, flex, automake, autoconf, libtool, m4
#    (note: make must be GNU make, check with "make --version")
# 2. Run the script, it should install GCC into ".local/share/gcc-mipsel32"
# 3. Add "$HOME/.local/share/gcc-mipsel32/bin" into PATH.
#    At the end of your shell rc file (.shrc, .bashrc, .zshrc, ...) add this:
#    export PATH="$HOME/.local/share/gcc-mipsel32/bin:$PATH"
#    And restart your terminal
#

#
# Defaults, you can change these through environment!
#
: ${BINUTILS_VERSION="2.47"}
: ${GCC_VERSION="16.2.0"}

: ${INSTALL_PREFIX="$HOME/.local/share/gcc-mipsel32"}

: ${CC="cc"}
: ${CXX="c++"}
: ${CFLAGS="-Os"}
: ${CXXFLAGS="-Os"}
: ${CFLAGS_FOR_TARGET="-G0 -Os"}
: ${CXXFLAGS_FOR_TARGET="-G0 -Os"}

: ${BUILD_TARGET="mipsel-none-elf"}

#set -ex
set -e

BUILD_HOST=$("$CC" -dumpmachine)
BUILD_THREADS_FALLBACK="8"

BUILD_DIR="$(mktemp -u -d "/tmp/gcc-$GCC_VERSION-$BUILD_TARGET.XXXXXXXXXXXXXXXXXXX")"

BUILD_THREADS="$BUILD_THREADS_FALLBACK"
if command -v nproc >/dev/null; then
	BUILD_THREADS="$(nproc)"
fi

# Binutils are going to be installed mid-way and are required for GCC to build
PATH="$INSTALL_PREFIX/bin:$PATH"

# These need to be in env to be picked up by configure and make
export \
	CC CXX \
	CFLAGS CXXFLAGS \
	CFLAGS_FOR_TARGET CXXFLAGS_FOR_TARGET \
	PATH

#
# Error!
#
f_error() {
	>&2 printf 'Error: %s\n' "$*"
	exit 1
}

#
# Download a file
#
f_download() {
	local url="$1"
	local out="$2"

	if command -v curl >/dev/null; then
		curl -fsSL --proto '=https' --tlsv1.2 -o "$out" "$url"
	elif command -v fetch >/dev/null; then
		SSL_NO_TLS1=1 SSL_NO_TLS1_1=1 fetch -q -o "$out" "$url"
	elif command -v wget >/dev/null; then
		wget -nv --tries=1 -O "$out" "$url"
	else
		f_error "no tools to download a file"
	fi
}

#
# GCC requires GNU make, on some systems it's named "gmake" and the default "make" is "bmake"
#
f_make() {
	if command -v gmake; then
		gmake "$@"
	elif make --version | grep -q 'GNU Make'; then
		make "$@"
	else
		f_error "could not find GNU make, try installing gmake"
	fi
}

#
# Cleanup trap
#
f_cleanup() {
	if [ -d "$BUILD_DIR" ]; then
		rm -rf "$BUILD_DIR"
	fi
}
trap f_cleanup INT EXIT

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

#
# Download
#

f_download "https://ftpmirror.gnu.org/gnu/gcc/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.gz" - | tar xzvpf - &
f_download "https://ftpmirror.gnu.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.gz" - | tar xzvpf - &
wait

#
# Build
#

cd "gcc-${GCC_VERSION}"
sh ./contrib/download_prerequisites
cd ..

mkdir build-binutils
cd build-binutils
"../binutils-$BINUTILS_VERSION/configure" \
	--build="$BUILD_HOST" \
	--host="$BUILD_HOST" \
	--target="$BUILD_TARGET" \
	--prefix="$INSTALL_PREFIX" \
	--with-float=soft \
	--disable-nls \
	--disable-werror

f_make -j$BUILD_THREADS
f_make install-strip
cd ..

mkdir build-gcc
cd build-gcc
"../gcc-${GCC_VERSION}/configure" \
	--build="$BUILD_HOST" \
	--host="$BUILD_HOST" \
	--target="$BUILD_TARGET" \
	--prefix="$INSTALL_PREFIX" \
	--with-arch=r3000 \
	--with-tune=r3000 \
	--with-abi=32 \
	--with-float=soft \
	--with-gnu-as \
	--with-gnu-ld \
	--without-headers \
	--without-newlib \
	--enable-languages=c,c++ \
	--enable-lto \
	--enable-fixed-point \
	--enable-initfini-array \
	--disable-bootstrap \
	--disable-hosted-libstdcxx \
	--disable-decimal-float \
	--disable-gcov \
	--disable-libatomic \
	--disable-libgomp \
	--disable-libquadmath \
	--disable-libssp \
	--disable-libvtv \
	--disable-multilib \
	--disable-nls \
	--disable-shared \
	--disable-threads \
	--disable-werror

f_make -j$BUILD_THREADS all
f_make install-strip

echo "Successfully installed!"
