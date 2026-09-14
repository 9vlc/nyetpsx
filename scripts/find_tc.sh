#!/bin/sh

usage()
{
	>&2 echo "usage: find_tc.sh <gcc/llvm> <tool>"
	exit 1
}

if [ -z "${1:-}" ] || [ -z "${2:-}" ]
then
	usage
fi

a_tcpart="$2"

# GCC: Probe various toolchain prefixes
if [ "$1" = gcc ]
then
	for f_tc in \
		mipsel-none-elf- \
		mipsel-unknown-elf- \
		mipsel-linux-gnu- \
		mipsel-linux-elf-
	do
		w_ex="$f_tc$a_tcpart"

		if command -v "$w_ex" >/dev/null
		then
			echo "$w_ex"
			exit 0
		fi
	done

# LLVM: Pick the latest version on the machine
elif [ "$1" = llvm ]
then
	w_matches="$(
		echo "$PATH" | tr ':' '\n' | while IFS= read -r d
		do
			if [ -d "$d" ]
			then
				ls "$d" | grep -E '^'"$a_tcpart"'([0-9]+|-[0-9]+)$'
			fi
		done | sort -r
		command -v "$a_tcpart"
	)"

	if [ "$w_matches" ]
	then
		echo "$w_matches" | sed 1q
		exit 0
	fi
else
	usage
fi

exit 1
#exit 0
