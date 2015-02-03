#!/bin/ksh
# (C) Copyright by PaN Super Sonic Software Service 1993, 1997, 1998, 2001, 2002, 2005
#                  expert for hi tech & low budget

umask 0
# BHS, BG-SYS

export ENSCRIPT='--ul-font=Times-Roman150 --ul-gray=.95 --ul-style=filled --ul-angle=90 --ul-position=+450+30 --setpagedevice=Duplex:true'
# ((
case "${OSTYPE}" in
	Windows_NT|cygwin)
		LOP=62
		TMPDIR=/temp/RIFU.$$
		RIFU=rifu32.exe
		;;
	*)
		LOP=100
		TMPDIR=/tmp/.RIFU.$$
		RIFU=rifu

		export ENSCRIPT="--ps-level=2 ${ENSCRIPT}"
		;;
esac

# ((
case "${OSTYPE}" in
	Windows_NT)
		PATH="C:/enscript;C:/rktools/bin;C:/bin;${PATH}"
		;;
	cygwin)
		PATH="/cygdrive/c/enscript:${PATH}"
		;;
esac

[ $# = 0 ] && ${RIFU}

mkdir -p ${TMPDIR} || exit

ORIGPWD="${PWD}"
export ORIGPWD

while [ $# -gt 0 ]
	do
		cd "${ORIGPWD}"

		# ((((
		case "$1" in
			-n)
				enscript () {
					echo enscript $@
				} # enscript

				;;
			[+-]x|[+-][xv][vx])
				set $1
				;;
			*.[dD][aA][tT]| \
			*[eE]60[1-9][0-9][0-9][0-9]| \
			*[eE][rR][tT][1-9][0-9][0-9][0-9]| \
			*[tT]90[1-9][0-9][0-9][0-9]| \
			*[eE]1[vV][1-9][0-9][0-9][0-9]| \
			*[dD][uU][mM][pP][0-9][0-9][0-9])
				# ((
				case "$1" in
					*.[dD][aA][tT])
						;;
					*)
						orig="$1"
						shift
						set -- "${orig}.dat" ${1+"$@"}
						;;
				esac

				cp -p "${1}" "${TMPDIR}"
				cd ${TMPDIR}
				bn="${1##*/}"

				typeset -u u=${bn%????}
				# (
				case "$u" in
					*[1-9][0-9][0-9][0-9])
						u="${u%???} ${u##????}"
						;;
				esac


				${RIFU} -LDe -pa "${bn}" >PA
				OLDDATE=
				exec <PA
				while read dat
					do
						dat=${dat%%'|'*}
						[ DATE = "${OLDDATE}" ] &&
							FROM="${dat}"
						OLDDATE="${dat}"
					done
				dat=${dat%%'|'*}
				if [ "${dat}" = "${FROM}" ]
					then ft="${FROM}"
					else ft="${FROM} ... ${dat}"
				fi

				${RIFU} -j -LDe ${OPT} "${bn}" >PB
				sed -n -e '/ bytes /,$p' <PB >PC
				tr '\202\216\231\232\204\224\201\341\205\267\265\266\240\203' '\351\304\326\334\344\366\374\337\340\300\301\302\341\342' <PC >"${ft}"
				touch -r "${bn}" -- "${ft}"

				GHOSTSPOOL=
				# ((
				case "${COMPUTERNAME}" in
					pan-*)
						GHOSTSPOOL="-pC:/GhostSpool/${u}.ps"
						LOP=100
						;;
					WEP04*)
						export ENSCRIPT="--printer=\\\\QEO00028\\DEO01106 ${ENSCRIPT}"
						;;
				esac

				enscript --non-printable-format=octal ${GHOSTSPOOL:+"${GHOSTSPOOL}"} -i 1 -B -L${LOP} -R1 -fCourier-BoldOblique8 --header="${u}" -G --underlay="${u}" -- "${ft}"
				;;
			*)
				OPT="${OPT} $1"
				;;
		esac
		shift
	done

cd ..
rm -rf "${TMPDIR}"

exit
rifu -j -LDe "${@:--h}" | enscript -B -L100 -R1 -fCourier-BoldOblique8
exit

# DreBa WM14
rifu -j -LDe "${@:--h}" | enscript -B -L100 -R1 -fCourier-BoldOblique8 -p/scm/user/pan/PRINTDIR/PRIFU$$
exit

# DreBa WL
set -a
PRINTER=tst_pr211k
LPDEST=${PRINTER}
set +a

rifu -j -LDe "$@" |
	a2ps -1 -8 -p -nL -nn -nu -B -nH -F7.35 -ns | lp
exit
		sed -e 's:/sw 8.27 inch def:/sw 8.67 inch def:' \
			-e 's:/margin 1.4 inch def:/margin 0.4 inch def:' |
			lp
exit
		sed -e 's:/margin 1.4 inch def:/margin 0.4 inch def:' \
			-e 's:m margin 3 div def:m margin 6 div def:' |
			lp
exit
		sed -e 's:/lm margin 2 mul 3 div def:/lm margin 3 div def:' \
			-e 's:m margin 3 div def:m margin 4 div def:' |
			lp
exit

# SEL Stuttgart
rifu -j -LDe "$@" | enscript -B -L100 -R1 -fCourier-BoldOblique8
exit
rifu -j $@ | enscript -r -B -fZapfChancery-MediumItalic7
exit

# Siemens MUC
rifu -j $@ | prf -orient land -top 1.0 -point 7

