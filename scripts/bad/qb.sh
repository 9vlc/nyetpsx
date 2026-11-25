#!/bin/sh
set -eu

i="$1"
o="$2"

CFLAGS="$(bmake -V CFLAGS)"
LDFLAGS="$(bmake -V LDFLAGS)"

CC="$(./scripts/find_tc.sh gcc gcc)"
OBJCOPY="$(./scripts/find_tc.sh gcc objcopy)"

c() {
	[ -f "$i"__.o ] && rm -f "$i"__.o
	[ -f "$i"__.e ] && rm -f "$i"__.e
}
trap c EXIT

"$CC" $CFLAGS  -c -o "$i"__.o "$i"
"$CC" $LDFLAGS    -o "$i"__.e "$i"__.o -T./psx_exe.ld -lnyetpsx
"$OBJCOPY" -O binary "$i"__.e "$o"
