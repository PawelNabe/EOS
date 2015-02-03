#!/bin/sh
# (C) Copyright by PaN Super Sonic Software Service 2001, 2002, 2003
#                  expert for hi tech & low budget

# (
case "`type setopt`" in
	'setopt is a shell builtin')
		setopt SH_WORD_SPLIT
		setopt GLOB_SUBST
		;;
esac

IFS=' 	
'
export IFS

LC_COLLATE='' LC_CTYPE='' LANG='' ENV='' HISTFILE='' HISTSIZE=''
unset LC_COLLATE LC_CTYPE LANG ENV HISTFILE HISTSIZE

stderr () {
	: ${ARGV0:=`/bin/basename $0`}
	echo "${ARGV0}:" ${1+"$@"} >&2
} # stderr

Exit () {	# exitcode messages....
	: ${ARGV0:=`/bin/basename $0`}
	ec="$1"
	shift
	stderr ${1+"$@"}
	exit "${ec}"
} # Exit

if [ -x /bin/posix/sh -a -z "${__RUNNING_UNDER_POSIX}" ]
	then
		__RUNNING_UNDER_POSIX=T
		export __RUNNING_UNDER_POSIX
		exec /bin/posix/sh $0 ${1+"$@"}
		Exit 8 Cannot exec /bin/posix/sh
fi

unset __RUNNING_UNDER_POSIX

ARGV0="$0"

usage () { # msg....
	: ${ARGV0:=`/bin/basename $0`}
	Exit 1 "${*:+$*
}Usage: ${ARGV0} [-A] [-F filmnooffset] txtfile
Function:
	Convert text file from a scanner into EOS type DAT file.
Options:
	-F filmnooffset		
	-A	append 'A' to each negnumber
Example:
	${ARGV0} t901dump.txt
	${ARGV0} -F 173 t901dump.txt
"
} # usage

set -f

incr () { # &var
	eval set -- "\"\$1\"" \${$1_points}
	eval "${1}"=$#
	eval "${1}_points"="\"\${${1}_points} .\""
} # incr

initvar () { # &vars
	while [ $# -gt 0 ]
		do
			eval "${1}"=0
			eval "${1}_points"=
			shift
		done
} # initvar

prepend () { # &inoutvar char length
	eval _ioval="\"\${$1}\""
	while :
		do
			# (
			case "${_ioval}" in
				$3*)	break;;
			esac
			_ioval="$2${_ioval}"
		done
	eval $1="\"\${_ioval}\""
} # prepend

remslash () { # &var
	_var="$1"
	eval _val="\"\${$1}\""
	_ifs="${IFS}"
	IFS=/
	set -- x ${_val}
	IFS="${_ifs}"
	shift

	# ( The following case is performed for zsh(1) on DOS only. Remove trailing space.
	case "$*" in
		?*' ')
			set -- x ${*}
			shift
			;;
	esac

	eval ${_var}="'$*'"
} # remslash

unmatched () { # var val
	Exit 11 Unmatched $1: \"$2\" in \"${INPUTFILE}\" line ${LINENUM}
} # unmatched

splittime () { # &hour &min time
	_ifs="${IFS}"
	IFS=': 	
'
	set -- "${1}" "${2}" $3
	IFS="${_ifs}"
	eval "$1"="'$3'"
	eval "$2"="'$4'"
	prepend "$1" '0' '??'
} # splittime

MA=' '
OLDFILMOFFSET=777
FILMOFFSET=888
while [ $# -gt 0 ]
	do
		# ((((((
		case "$1" in
			[+-]x|[+-][xv][vx])
				set $1
				;;
			-n|-no|-noe|-noex|-noexe|-noexec|-noexecu|-noexecut|-noexecute)
				NOEXECUTE=-n
				;;
			-A)
				MA=A
				;;
			-F)
				FILMOFFSET="$2"
				prepend FILMOFFSET 0 '???'
				shift
				;;
			-*)
				usage "Unknown option \"$1\"."
				;;
			*)
				break
				;;
		esac
		shift
	done

[ $# = 1 ] || usage

INPUTFILE="$1"
exec < "${INPUTFILE}" || exit

OLDDAY=
OLDMONTH=
OLDYEAR=
OLDHOUR=
OLDMIN=
initvar LINENUM
while read line
	do
		incr LINENUM
		COMMENT= FRNO= TV= AV= DAY= MONTH= YEAR= TIME= ISO= FLASH= PROG= MET= LENS= CORR=
		# (((((((
		case "${line}" in
			[0-9][0-9][0-9][0-9]*)
				set x ${line}
				shift
				# ((
				case "$1" in
					[0-9][0-9][0-9][0-9])
						;;
					*)
						unmatched COMMENT "$1"
						;;
				esac
				COMMENT="Bild $1"
				shift

				# ((
				case "$1" in
					[1-9]|[1-3][0-9])
						FRNO="$1"
						;;
					*)
						unmatched FRNO "$1"
						;;
				esac
				prepend FRNO ' ' '??'
				shift 

				# ((
				case "$1" in
					[1-3]0\"|15\"|[123468]\"|1\"5|0\"7|[23468]|[12369]0|15|45|125|180|250|350|500|750|[1-4]000|1500)
						TV="$1"
						;;
					*)
						unmatched TV "$1"
						;;
				esac
				prepend TV ' ' '????'
				shift

				# (((
				case "$1" in
					1.[248]|2.[058]|3.5|4.0|4.5|5.6|6.7|8.0|9.5|1[1369]|22|27|32)
						AV="$1"
						;;
					M|SD[AM])
						AV='???'
						COMMENT="AV=$1"
						;;
					*)
						unmatched AV "$1"
						;;
				esac
				prepend AV ' ' '???'
				shift

				# (((
				case "$1" in
					[1-9]/??/??|[1-3][0-9]/??/??)
						DAY="$1"
						remslash DAY
						shift
						set ${DAY} "$@"
						DAY="$1"
						MONTH="$2"
						YEAR="$3"
						shift 3
						set "${DAY}" "${MONTH}/${YEAR}" "$@"
						;;
					[1-9]/|[1-3][0-9]/)
						DAY="$1"
						remslash DAY
						;;
					*)
						unmatched DAY "$1"
						;;
				esac
				prepend DAY ' ' '??'
				shift

				# ((
				case "$1" in
					[1-9]/[0-9][0-9]|1[0-9]/[0-9][0-9])
						MONTH="$1"
						remslash MONTH
						set -- ${MONTH} "$@"
						MONTH="$1"
						# (((((((((((((
						case "$1" in
							 1) MONTH=Jan;;
							 2) MONTH=Feb;;
							 3) MONTH=Mar;;
							 4) MONTH=Apr;;
							 5) MONTH=May;;
							 6) MONTH=Jun;;
							 7) MONTH=Jul;;
							 8) MONTH=Aug;;
							 9) MONTH=Sep;;
							10) MONTH=Oct;;
							11) MONTH=Nov;;
							12) MONTH=Dec;;
							*)
								unmatched MONTH "$1"
						esac
						YEAR="$2"
						shift 2
						# (((
						case "${YEAR}" in
							8[67])
								Exit 11 Cannot match these years yet \"${YEAR}\"
								;;
							[0-9][0-9])
								[ 87 -lt "${YEAR}" ] && YEAR=19"${YEAR}"
								[ 85 -gt "${YEAR}" ] && YEAR=20"${YEAR}"
								;;
							*)
								unmatched YEAR "$1"
								;;
						esac
						;;
					*)
						unmatched MONTH/YEAR "$1"
						;;
				esac
				shift

				# (((
				case "$1" in
					[0-9]|[1-2][0-9])
						TIME="$1:$2"
						shift
						;;
					[0-9]:[0-5][0-9]|[1-2][0-9]:[0-5][0-9])
						TIME="$1"
						;;
					*)
						unmatched TIME "$1"
						;;
				esac
				prepend TIME ' ' '?????'
				shift

				# ((
				case "$1" in
					[68]|[1-8][0-9]|[1-8][0-9][0-9]|[1-6][0-9][0-9][0-9])
						ISO="$1"
						;;
					*)
						unmatched ISO "$1"
						;;
				esac
				prepend ISO ' ' '????'
				shift

				# ((((((((
				case "$1:$2" in
					[12]/3:*)
						CORR="+${1}"
						shift
						;;
					-[12]/3:*)
						CORR="${1}"
						shift
						;;
					1:1/3)
						CORR=+4/3
						shift 2
						;;
					1:2/3)
						CORR=+5/3
						shift 2
						;;
					-1:1/3)
						CORR=-4/3
						shift 2
						;;
					-1:2/3)
						CORR=-5/3
						shift 2
						;;
					[1-2]:*)
						CORR="+${1}.0"
						shift
						;;
					-[1-2]:*)
						CORR="${1}.0"
						shift
						;;
				esac

				# ((((((
				case "$1" in
					OFF)
						FLASH='    '
						;;
					FEL)
						FLASH="FEL "
						COMMENT=
						;;
					M)
						FLASH="MAN."
						;;
					TTL)
						FLASH="TTL "
						;;
					ATTL)
						FLASH="$1"
						;;
					*)
						unmatched FLASH "$1"
						;;
				esac
				shift

				# (((
				case "$1" in
					[AT]V|M)
						PROG="$1"
						;;
					PROG)
						PROG='P '
						;;
					*)
						unmatched PROG "$1"
						;;
				esac
				shift

				# ((((
				case "$1" in
					PART)
						MET='* '
						;;
					SPOT)
						MET='. '
						;;
					AVER)
						MET='[]'
						;;
					*)
						unmatched MET "$1"
						;;
				esac
				shift

				# (((((((
				case "$1" in
					 8)
						LENS="4.0/   8mm"
						;;
					15)
						LENS="2.8/  15mm"
						;;
					17|--)
						LENS="3.5/  17mm"
						;;
					2[48])
						LENS="2.8/  24mm"
						;;
					50)
						LENS="1.4/  50mm"
						;;
					135)
						LENS="1.2/  85mm"
						;;
					*)
						unmatched LENS "$1"
						;;
				esac
				shift

				[ $# = 0 ] || Exit 11 Items left in ${INPUTFILE} line ${LINENUM}: \"$*\"
				;;
			'SPOT DATA'*|FEL?DATA:*|5:*)
				set -- ${line}
				COMMENT="${*}"
				;;
			FILMOFFSET*)
				set -- ${line}
				FILMOFFSET="$2"
				prepend FILMOFFSET 0 '???'
				;;
			APPENDA)
				MA=A
				;;
			''|''|N[O0].*FR*TV*AV*DATE*TIME*ISO*LENS|MODE*-ING)
				;;
			'* '*)
				echo
				COMMENT="${line}"
				;;
			*)
				unmatched line "${line}"
				;;
		esac
		if [ -n "${FRNO}" ]
			then
				splittime HOUR MIN ${TIME}
				if [ '(' "${OLDYEAR}${OLDMONTH}${OLDDAY}" = "${YEAR}${MONTH}${DAY}" ')' -a '(' "${HOUR}${MIN}" -lt "${OLDHOUR}${OLDMIN}" ')' ]
					then
						Exit 11 Wrong time stamp in \"${INPUTFILE}\" line ${LINENUM}: "${HOUR}:${MIN}" '<' "${OLDHOUR}:${OLDMIN}"
				fi
				OLDHOUR=${HOUR}
				OLDMIN=${MIN}

				if [ "${OLDDAY}${OLDMONTH}${OLDYEAR}${OLDFILMOFFSET}" != "${DAY}${MONTH}${YEAR}${FILMOFFSET}" ]
					then
						echo
						echo " ${FILMOFFSET} ----------------- XXX ${DAY} ${MONTH} ${YEAR} -----------------"
						OLDDAY="${DAY}"
						OLDMONTH="${MONTH}"
						OLDYEAR="${YEAR}"
						OLDFILMOFFSET="${FILMOFFSET}"
				fi
				echo "[${FRNO}${MA}]    ${MET} ${FLASH}    ${TV}  ${AV}  ${PROG}      ${ISO}${CORR:-    } ${TIME}    ${LENS}"
		fi
		[ -n "${COMMENT}" ] && echo "${COMMENT}"
	done
echo

exit 0
				FEL	DATA:-1	EVR:- 3 3/4

   1 ----------------- Wed  2 Jan 2013 -----------------
[15A]:01 [] ATTL->> 4000  1.2  DEP AEB 6400+3.5  3:04:05 1.2/1234mm 1234-2345mm

* Kodak	Kodachrome 64 09/2001 134 112		19.04..27.05.2001
FILMOFFSET 178
6227	1	20	2.8	19/	4/01	18	21	64	OFF	AV	PART 15
6228	2	125	13	20/	4/01	13	33	64	OFF	AV	PART 15
6229	3	90	13	20/	4/01	13	35	64	OFF	AV	PART 15
6230	4	20	3.5	26/	4/01	20	11	64	OFF	AV	PART 15
6231	5	30	3.5	26/	4/01	20	12	64	OFF	AV	PART 15
6232	6	15	2.8	26/	4/01	20	12	64	OFF	AV	PART 15
6233 	7	1500	2.0	26/	4/01	20	13	64	OFF 	AV  	PART 135
6234 	8	180	1.8	26/	4/01	20:13		64	OFF 	AV  	PART 135
6235 	9	60	4.5	27/	5/01	22:49		64	ATTL	PROG	PART 28
6236	10	4000	1.4	27/	5/01	23:00		64	OFF 	AV  	PART	50
6237	11	4	32	27/	5/01	23:00		64	OFF 	AV  	PART	50

FILMOFFSET 48
1685	1	30	8.0	21/	6/01	7:47	100	OFF	AV	PART	15
1686	2	30	8.0	21/	6/01	7:47	100	OFF	AV	PART	15
1687	3	45	8.0	21/	6/01	7:47	100	OFF	AV	PART	15
1688	4	90	8.0	21/	6/01	7:45	100	OFF	AV	PART	15
1689	5	45	9.5	21/	6/01	7:49	100	OFF	AV	PART	15
1690	6	45	9.5	21/	6/01	7:50	100	OFF	AV	PART	15
1691	7	45	9.5	21/	6/01	10:24	100	OFF	AV	PART	15
1692	8	90	9.5	21/	6/01	10:25	100	OFF	AV	PART	15
1693	9	90	9.5	21/	6/01	10:34	100	OFF	AV	PART	15
1694	10	250	9.5	21/	6/01	10:47	100	OFF	AV	PART	15
1695	11	250	9.5	21/	6/01	11:00	100	OFF	AV	PART	15
1696	12	1000	4.5	21/	6/01	11:19	100	OFF	AV	PART	15
1697	13	60	8.0	21/	6/01	11:23	100	OFF	AV	PART	15
1698	14	30	4.0	21/	6/01	17:25	100	OFF	PROG	PART	15
1699	15	20	3.5	22/	6/01	12:32	100	OFF	PROG	PART	15
1700	16	1"	2.8	25/	6/01	17:23	100	OFF	PROG	AVER	15
1701	17	0"7	2.8	25/	6/01	17:23	100	OFF	PROG	AVER	15
1702	18	0"7	2.8	25/	6/01	17:23	100	OFF	PROG	AVER	15
1703	19	0"7	2.8	25/	6/01	17:23	100	OFF	PROG	AVER	15
1704	20	90	3.5	30/	6/01	17:46	100	OFF	PROG	PART	28
1705	21	90	3.5	30/	6/01	17:46	100 1	OFF	PROG	PART	28
1706	22	180	4.5	30/	6/01	17:48	100	OFF	PROG	PART	28

1707	23	500	8.0	30/ 6/01	17	48	100	OFF	PROG	PART	28
1708	24	125	4.0	30/ 6/01	17	48	100	OFF	PROG	PART	28
1709	25	15	4.5	2/ 7/01	17	31	100	FEL	AV	SPOT	28
					FEL DATA:	-1/2		EVR:- 2				
1710	26	20	3.5	2/ 7/01	17	31	100	FEL	TV	SPOT	28
					FEL DATA:	-1/2		EVR:- 2	1/4			
1711	27	60	4.0	2/ 7/01	17	32	100	FEL	PROG	SPOT	28
					FEL DATA:	+/-	0	EVR:- 3				
1712	28	60	5.6	2/ 7/01	17	32	100	ATTL	PROG	SPOT	28
					SPOT DATA							
1713	29	10	2.8	2/ 7/01	17	32	100	OFF	PROG	SPOT	28
					SPOT DATA	1:-	5	3/4 2:-	5 1/4 3:	1 1/2	4: 7	3/4
						5:	1	6:-	1/2 7:-	1/2	8:-	3/4
1714	30	30	4.0	2/ 7/01	17	33	100	OFF	TV	SPOT	28
					SPOT DATA	1:-	1	2:-	1 1/4 3:-	3		
1715	31	250	5.6	2/ 7/01	17	33	100	M	AV	SPOT	28
					SPOT DATA							
1716	32	250	5.6	2/ 7/01	17	34	100	M	AV	SPOT	28
					SPOT DATA							
1717	33	125	2.8	2/ 7/01	17	55	100	ATTL	TV	SPOT	28
					SPOT DATA							
1718	34	4	2.8	2/ 7/01	17	56	100	OFF	TV	SPOT	28
					SPOT DATA	1:-		1/4				
1719	35	8	2.8	2/ 7/01	17	57	100	ATTL	TV	AVER	28
1720	36	30	2.8	2/ 7/01	17	58	100	FEL	TV	AVER	28
					FEL DATA:	+/-	0	EVR:- 2	1/2			
FILMOFFSET 054
1932	34	250	M	1/ 1/02	14:49	100	ATTL	PROG	PART		28
1933	35	60	SDM	1/ 1/02	14:49	100	ATTL	PROG	PART		28

FILMOFFSET 055
1945	9	60	5.6	5/	1/02	19:22	32	-1 2/3	ATTL	PROG	PART	28

