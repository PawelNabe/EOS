#!/bin/ksh
# (C) Copyright by PaN Super Sonic Software Service 1993,1994,1997,1998,2002,2003,2005
#                  expert for hi tech & low budget

# On Windows
typeset -u u
LAST=
integer A B L LOP
enscript=enscript

export ENSCRIPT='--ul-font=Times-Roman200 --ul-gray=.95 --ul-style=filled --ul-angle=90 --ul-position=+450+30 --setpagedevice=Duplex:true'

case "${OSTYPE}" in
		Windows_NT|cygwin)
			ENSCRFUNC=enscrdos
			LOP=62
			;;
		*)
			export ENSCRIPT="--ps-level=2 ${ENSCRIPT}"
			ENSCRFUNC=enscrunix
			LOP=100
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

GHOSTSPOOL=
# ((
case "${COMPUTERNAME}" in
	pan-*)
		GHOSTSPOOL='-pC:/GhostSpool/${u}.ps'
		LOP=100
		;;
	WEP04*)
		export ENSCRIPT="--printer=\\\\QEO00028\\DEO01106 ${ENSCRIPT}"
		LOP=100
		;;
esac

enscrunix () {
	${enscript} --non-printable-format=octal ${LAST} -i 4 -L${LOP} -R1 -fCourier-BoldOblique10 -G --underlay="${u}" $f
} # enscrunix

enscrdos () {
	tr '\202\216\231\232\204\224\201\341\205\267\265\266\240\203' '\351\304\326\334\344\366\374\337\340\300\301\302\341\342' < ${f} > ${TMP}/${u}
	touch -r ${f} ${TMP}/${u}
	(
		cd ${TMP}
		eval ${enscript} --non-printable-format=octal ${GHOSTSPOOL} ${LAST} -i 4 -L${LOP} -R1 -fCourier-BoldOblique10 -G --underlay="${u}" -- "${u}"
		rm "${u}"
	)
} # enscrdos

for f in ${*-????[fF][iI][lL][mM]}
	do
		# ((((((
		case "$f" in
			-n)
				enscript='echo ${ENSCRFUNC}'
				;;
			-[vx]|-vx|-xv)
				set $f
				continue
				;;
			-l)
				LAST=T
				continue
				;;
			*[fF][iI][lL][mM].[tT][xX][tT])
				tbs=FILM.TXT
				;;
			*[fF][iI][lL][mM])
				tbs=FILM
				;;
			*)
				echo Internal error detected. Cannot match "'$f'" >&2
				exit 22
				;;
		esac
		if [ -n "${LAST}" ]
			then
				set -- `wc -l $f`
				L=$1
				let A=${L}/${LOP}
				let B=${L}%${LOP}
				[ "${B}" -gt 0 ] && let A=${A}+1
				LAST="--pages=$A"
				
		fi
		u=${f##*/}
		u=${u%${tbs}}

		${ENSCRFUNC}
	done

exit

# DreBa WM
for f in ${*-????film}
	do
		enscript -L100 -R1 -fCourier-BoldOblique8 -G -p/scm/user/pan/PRINTDIR/${f}.OUT $f
	done

exit

# DreBa WL
set -a
PRINTER=tst_pr211k
LPDEST=${PRINTER}
set +a

a2ps -1 -8 -p -nL -nn -nu -B -r -F9 -ns ${*-????film} | lp
exit

# SEL Stuttgart
for f in ${*-????film}
	do
		enscript -L100 -R1 -fCourier-BoldOblique8 -G $f
	done

