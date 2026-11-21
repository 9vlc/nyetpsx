NYETPSX_TC?= GCC
NYETPSX_USE_SDATA?= 1
NYETPSX_BUILD_EXAMPLE?= 1
NYETPSX_STDIO_USE_SIO1?= 0
NYETPSX_SOURCE_TC?= 0 # Set to 1 to just get the toolchain from a different project

.if ${NYETPSX_TC:tu} == "GCC"
CC!= ${.PARSEDIR}/scripts/find_tc.sh gcc gcc
AS!= ${.PARSEDIR}/scripts/find_tc.sh gcc gcc
LD!= ${.PARSEDIR}/scripts/find_tc.sh gcc gcc # Link through driver
AR!= ${.PARSEDIR}/scripts/find_tc.sh gcc gcc-ar
RANLIB!= ${.PARSEDIR}/scripts/find_tc.sh gcc gcc-ranlib
OBJCOPY!= ${.PARSEDIR}/scripts/find_tc.sh gcc objcopy

CFLAGS:= -march=r3000 -mabi=32 -mno-llsc -mfp32

.elif ${NYETPSX_TC:tu} == "LLVM"
.warning "LLVM does not support some COP2 instructions, GTE will not work!"
CC!= ${.PARSEDIR}/scripts/find_tc.sh llvm clang
AS!= ${.PARSEDIR}/scripts/find_tc.sh llvm clang
LD!= ${.PARSEDIR}/scripts/find_tc.sh llvm clang
AR!= ${.PARSEDIR}/scripts/find_tc.sh llvm llvm-ar
RANLIB!= ${.PARSEDIR}/scripts/find_tc.sh llvm llvm-ranlib
OBJCOPY!= ${.PARSEDIR}/scripts/find_tc.sh llvm llvm-objcopy

CFLAGS:= -target mipsel-none-elf -march=mips1 -mabi=32 -mfp32 -Wextra

.else
.  error "Unknown toolchain: '${NYETPSX_TC:tu}', try 'GCC' or 'LLVM'"
.endif

.for _t in CC LD
.  if empty(${_t})
.    error "Could not find a fitting MIPS ${NYETPSX_TC:tu} ${_t} executable"
.  endif
.endfor

ASFLAGS:= ${CFLAGS} -mno-abicalls

CFLAGS+= -ffreestanding -fno-builtin -nostdlib -fno-pic
CFLAGS+= -fdata-sections -ffunction-sections -fsigned-char -mno-extern-sdata
CFLAGS+= -mno-mt -mno-abicalls -mno-check-zero-division

.if ${NYETPSX_USE_SDATA} == 1
CFLAGS+= -mgpopt -G8
LDFLAGS:= -G8
.else
CFLAGS+= -mno-gpopt -G0
LDFLAGS:= -G0
.endif

CFLAGS+= -I${INCDIR} -Os # Default (-O0) fails to compile GTE code
LDFLAGS+= -L${OBJDIR}/out -nostdlib

.if ${NYETPSX_SOURCE_TC} != 1
CFLAGS+= -Wall -pedantic -std=c11 -flto ${CFLAGS_ADD}
LDFLAGS+= -s -static ${CFLAGS} ${LDFLAGS_ADD}
.endif

.if ${NYETPSX_STDIO_USE_SIO1} == 1
CFLAGS+= -DSTDIO_USE_SIO1=1
.endif
