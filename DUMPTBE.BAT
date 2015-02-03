#!/bin/ksh
# (C) Copyright by PaN Super Sonic Software Service 1997
#                  expert for hi tech & low budget

# Usage: enter sh and then type in ". ./dumptbe.bat"

IFS=' 	
'
export IFS

stderr () {
	print -u2 "${ARGV0}:" ${1+"$@"}
} # stderr

Exit () {	# exitcode messages....
	ec="$1"
	shift
	stderr ${1+"$@"}
	exit "${ec}"
} # Exit

usage () {
	Exit 22 "$*
Usage: ${ARGV0:=$0} [-n]
Options:
	-n	Noexecute mode.
Examples:
	${ARGV0}
"
} # usage

tbe () { # file
	echo tbe 2 i "$1"
	if [ -z "${NOEXECUTE}" ]
		then
			>"${1}" || Exit 13 Cannot create new dumpfile \"$1\"
			for i in 1 2 3 4 5 6 7 8 9 0
				do
					for j in 1 2 3 4 5 6 7 8 9 0
						do
							echo 01234567890123456789012345678901234567890123456789012345678901234567890123456789
						done
				done >"${1}" || Exit 28 Cannot fill new dumpfile \"$1\"
			>"${1}" || Exit 1 Cannot truncate new dumpfile \"$1\"
			# (
			case "${PATH}" in
				*';'*)
					PATH="${PATH};.;.."
					export PATH
					;;
			esac
			TBE 2 i "$1"
	fi
} # tbe

ARGV0="${0##*/}"
NOEXECUTE=

set +f

while [ $# -gt 0 ]
	do
		# ((((
		case "$1" in
			[+-]x|[+-][xv][vx])
				set $1
				;;
			-n)
				NOEXECUTE=T
				;;
			-*)
				usage "Invalid option '$1'"
				;;
			*)
				;;
		esac
		shift
	done

dump=
for d in ../dump*.dat dump*dat dat/dump*dat
	do
		if [ -f "$d" -a -r "${d}" ]
			then
				d="${d##*/}"
				d="${d%.dat}"
				d="${d#dump}"
				# (((((
				case "$d" in
					????*)	;;
					???)	d="0$d";;
					??)	d="00$d";;
					?)	d="000$d";;
					*)	Exit 22 "Internal error detected: dumpfilename is '$d'";;
				esac
				dump="${dump} ${d}"
		fi
	done
if [ -n "${dump}" ]
	then
		set -s -- ${dump}
		for dump
			do
				:
			done
		integer next="${dump}"
		let next=next+1
		stderr Highest dumpnumber is "${dump} next dump is 'dump${next}.dat'"
		tbe "dump${next}.dat"
	else
		stderr No dump file found
fi

