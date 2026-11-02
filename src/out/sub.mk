EXTRAS:= libnyetpsx.a
.include "${MBDIR}/extras.mk"

${SUBOBJDIR}/libnyetpsx.a: ${OBJS_ALL}
	${AR} rcs $@ $>
	${RANLIB} $@
