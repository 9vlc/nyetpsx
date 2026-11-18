SRCS:= assert.c crt0.c stdio.c uprintf.c string.c ctype.c
CFLAGS.string.c:= -fno-lto
.include "${MBDIR}/gen.mk"
