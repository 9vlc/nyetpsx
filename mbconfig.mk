NYETPSX_TC?= GCC
NYETPSX_USE_SDATA?= 1
NYETPSX_BUILD_EXAMPLE?= 1

.if ${NYETPSX_TC:tu} == "GCC"
CC!= ${.PARSEDIR}/scripts/find_tc.sh gcc gcc
AS!= ${.PARSEDIR}/scripts/find_tc.sh gcc gcc
LD!= ${.PARSEDIR}/scripts/find_tc.sh gcc ld.bfd
AR!= ${.PARSEDIR}/scripts/find_tc.sh gcc ar
RANLIB!= ${.PARSEDIR}/scripts/find_tc.sh gcc ranlib
OBJCOPY!= ${.PARSEDIR}/scripts/find_tc.sh gcc objcopy

CFLAGS:= -march=r3000 -mabi=32 -mno-llsc -mfp32

.elif ${NYETPSX_TC:tu} == "LLVM"
CC!= ${.PARSEDIR}/scripts/find_tc.sh llvm clang
AS!= ${.PARSEDIR}/scripts/find_tc.sh llvm clang
LD!= ${.PARSEDIR}/scripts/find_tc.sh llvm ld.lld
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

ASFLAGS:= ${CFLAGS}

CFLAGS+= -ffreestanding -fno-builtin -nostdlib -fno-pic
CFLAGS+= -fdata-sections -ffunction-sections -fsigned-char -mno-extern-sdata
CFLAGS+= -mno-mt -mno-abicalls -mno-check-zero-division

.if ${NYETPSX_USE_SDATA} == 1
CFLAGS+= -mgpopt -G8
.else
CFLAGS+= -mno-gpopt -G0
.endif

CFLAGS+= -I${INCDIR} -I${INCDIR}/psx -Wall -pedantic -std=c23 -Os ${CFLAGS_ADD}
LDFLAGS:= -L${OBJDIR}/out -g -G8 -static -nostdlib ${LDFLAGS_ADD}
