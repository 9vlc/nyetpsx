SRCS:= syscall.s arith64.c arith64_clz.s
CFLAGS.arith64.c:= -fno-lto
.include "${MBDIR}/gen.mk"
