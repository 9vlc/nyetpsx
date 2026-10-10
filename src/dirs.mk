SUBDIRS:= psx libc etc graph out
.if ${NYETPSX_BUILD_EXAMPLE} == 1
SUBDIRS+= example
.endif
.include "${MBDIR}/indirs.mk"
