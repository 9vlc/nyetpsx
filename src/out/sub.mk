EXTRAS:= nyetpsx.a
.include "${MBDIR}/extras.mk"

${SUBOBJDIR}/nyetpsx.a: ${OBJS_ALL}
	${AR} rcs $@ $>
	${RANLIB} $@
