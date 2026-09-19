SRCS:= main.c
EXTRAS:= example.exe

.include "${MBDIR}/gen.mk"
.include "${MBDIR}/extras.mk"

${SUBOBJDIR}/example.elf: ${PROJDIR}/psx_exe.ld ${OBJS} ${EXTRAS.out}
	${LD} ${LDFLAGS} -o $@ -T $>

${SUBOBJDIR}/example.exe: ${SUBOBJDIR}/example.elf
	${OBJCOPY} -O binary $> $@
	cp $@ ${OBJDIR}/out/
