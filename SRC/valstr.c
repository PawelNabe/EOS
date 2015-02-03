/* (C) Copyright by PaN Super Sonic Software Service 1991-2006	*/
/*                  expert for hi tec & low budget		*/

#ifdef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif /* _WIN32 */

#include "rifu.h"
#include "valstr.h"
#include "pos.h"

typedef struct {
	int val;
	char *str;
} valstr;

#ifndef	__DATE__
#define	__DATE__	"Tue 17 Jan 2006"
#endif

#ifndef	__TIME__
#define	__TIME__	"17:25"
#endif
#ifdef __TIMESTAMP__
#define ___TS___ " " __TIMESTAMP__
#else
#define ___TS___ ""
#endif

void valstr_version()
{
	puts(
		"@" "(#)"
#ifdef __CCXPN__
		__CCXPN__
#else
		__FILE__
		___TS___
#endif /* __CCXPN__ */
		" compiled: "
		__DATE__
		" "
		__TIME__
	);
} /* valstr_version */

static void valtostr(buf, val, valstrs, numvals)
char buf[];
int val;
valstr valstrs[];
int numvals;
{
	while (--numvals && valstrs[numvals].val != val)
		;
	strcpy(buf, valstrs[numvals].str);
} /* valtostr */

static int strtoval(str, valstrs, numvals)
char *str;
valstr valstrs[];
int numvals;
{
	while (--numvals && StrnCmp(valstrs[numvals].str, !=, str, strlen(valstrs[numvals].str)))
		;
	return(valstrs[numvals].val);
} /* strtoval */

#define STRTOVALFUNC(func, vs)\
int func(str)\
char *str;\
{\
	return(strtoval(str, vs, NUMELEMS(vs)));\
}

#define VALTOSTRFUNC(func, vs)\
void func(buf, val)\
char buf[];\
int val;\
{\
	valtostr(buf, val, vs, NUMELEMS(vs));\
}

static valstr aemvals[] = {
	{ 0x83, "???" },
	{ 0x00, "P  " },
	{ 0x01, "TV " },
	{ 0x02, "AV " },
	{ 0x03, "DEP" },
	{ 0x80, "M  " },
};

STRTOVALFUNC(StrToAem, aemvals)
VALTOSTRFUNC(AemToStr, aemvals)

/* TBE does not use combination 0x8D.
   We overload this with FEL for the T90.
*/
static valstr flavals[] = {
	{ 0x00, "    " },
	{ 0x81, "MAN." },
	{ 0x85, "ATTL" },
	{ 0x89, "TTL " },
	{ 0x8D, "FEL " },	/* Special hack for T90 */
	{ 0x01, "ON  " },
};

STRTOVALFUNC(StrToFla, flavals)
VALTOSTRFUNC(FlaToStr, flavals)

static valstr curvals[] = {
	{ 0x00, "   " },	/* 1st curtain */
	{ 0x02, "->>" },	/* 2nd curtain */
};

STRTOVALFUNC(StrToCur, curvals)
VALTOSTRFUNC(CurToStr, curvals)

static valstr avvals[] = {
	 { 0x00, "???" },
	 { 0x04, "---" },
	 { 0x08, "1.0" },
	 { 0x0C, "1.2" },
	 { 0x10, "1.4" },
	 { 0x14, "1.8" },
	 { 0x18, "2.0" },
	 { 0x1C, "2.5" },
	 { 0x20, "2.8" },
	 { 0x24, "3.5" },
	 { 0x28, "4.0" },
	 { 0x2C, "4.5" },
	 { 0x30, "5.6" },
	 { 0x34, "6.7" },
	 { 0x38, "8.0" },
	 { 0x3C, "9.5" },
	 { 0x40, " 11" },
	 { 0x44, " 13" },
	 { 0x48, " 16" },
	 { 0x4C, " 19" },
	 { 0x50, " 22" },
	 { 0x54, " 27" },
	 { 0x58, " 32" },
};

STRTOVALFUNC(StrToAv, avvals)
VALTOSTRFUNC(AvToStr, avvals)

static valstr tvvals[] = {
	{ 0x00, "????" },
	{ 0x0C, "bulb" },
	{ 0x10, " 30\"" },
	{ 0x14, " 20\"" },
	{ 0x18, " 15\"" },
	{ 0x1C, " 10\"" },
	{ 0x20, "  8\"" },
	{ 0x24, "  6\"" },
	{ 0x28, "  4\"" },
	{ 0x2C, "  3\"" },
	{ 0x30, "  2\"" },
	{ 0x34, " 1\"5" },
	{ 0x38, "  1\"" },
	{ 0x3C, " 0\"7" },
	{ 0x40, "   2" },
	{ 0x44, "   3" },
	{ 0x48, "   4" },
	{ 0x4C, "   6" },
	{ 0x50, "   8" },
	{ 0x54, "  10" },
	{ 0x58, "  15" },
	{ 0x5C, "  20" },
	{ 0x60, "  30" },
	{ 0x64, "  45" },
	{ 0x68, "  60" },
	{ 0x6C, "  90" },
	{ 0x70, " 125" },
	{ 0x74, " 180" },
	{ 0x78, " 250" },
	{ 0x7C, " 350" },
	{ 0x80, " 500" },
	{ 0x84, " 750" },
	{ 0x88, "1000" },
	{ 0x8C, "1500" },
	{ 0x90, "2000" },
	{ 0x94, "3000" },
	{ 0x98, "4000" },
	{ 0xEC, "bc  " },
	{ 0xF0, "EEE " },
	{ 0xF4, "dEP " },
	{ 0xF8, "HELP" },
	{ 0xFC, "----" },
};

STRTOVALFUNC(StrToTv, tvvals)
VALTOSTRFUNC(TvToStr, tvvals)

static valstr isovals[] = {
	{ 0x00, "????" },
	{ 0x28, "   6" },
	{ 0x2A, "   8" },
	{ 0x2C, "  10" },
	{ 0x30, "  12" },
	{ 0x32, "  16" },
	{ 0x34, "  20" },
	{ 0x38, "  25" },
	{ 0x3A, "  32" },
	{ 0x3C, "  40" },
	{ 0x40, "  50" },
	{ 0x42, "  64" },
	{ 0x44, "  80" },
	{ 0x48, " 100" },
	{ 0x4A, " 125" },
	{ 0x4C, " 160" },
	{ 0x50, " 200" },
	{ 0x52, " 250" },
	{ 0x54, " 320" },
	{ 0x58, " 400" },
	{ 0x5A, " 500" },
	{ 0x5C, " 640" },
	{ 0x60, " 800" },
	{ 0x62, "1000" },
	{ 0x64, "1250" },
	{ 0x68, "1600" },
	{ 0x6A, "2000" },
	{ 0x6C, "2500" },
	{ 0x70, "3200" },
	{ 0x72, "4000" },
	{ 0x74, "5000" },
	{ 0x78, "6400" },
};

STRTOVALFUNC(StrToIso, isovals)
VALTOSTRFUNC(IsoToStr, isovals)

/* TBE uses just the top 6 bits (0xFC).
   We overload the 2 lower bits with 1/3rd stop exposure compensation for the T90.
*/

static valstr corvals[] = {
	{ 0xB0,("+--+") },
	{ 0x28, "-5.0" },
	{ 0x24, "-4.5" },
	{ 0x20, "-4.0" },
	{ 0x1C, "-3.5" },
	{ 0x18, "-3.0" },
	{ 0x14, "-2.5" },
	{ 0x10, "-2.0" },
	{ 0x0E, "-5/3" },	/* Special hack for T90 */
	{ 0x0C, "-1.5" },
	{ 0x09, "-4/3" },	/* Special hack for T90 */
	{ 0x08, "-1.0" },
	{ 0x03, "-2/3" },	/* Special hack for T90 */
	{ 0x04, "-0.5" },
	{ 0x01, "-1/3" },	/* Special hack for T90 */
	{ 0x00, "-0.0" },
	{ 0x00, " 0.0" },
	{ 0x00, "+0.0" },
	{ 0x00, "----" },
	{ 0x00, "    " },
	{ 0xFD, "+1/3" },	/* Special hack for T90 */
	{ 0xFC, "+0.5" },
	{ 0xFA, "+2/3" },	/* Special hack for T90 */
	{ 0xF8, "+1.0" },
	{ 0xF5, "+4/3" },	/* Special hack for T90 */
	{ 0xF4, "+1.5" },
	{ 0xF2, "+5/3" },	/* Special hack for T90 */
	{ 0xF0, "+2.0" },
	{ 0xEC, "+2.5" },
	{ 0xE8, "+3.0" },
	{ 0xE4, "+3.5" },
	{ 0xE0, "+4.0" },
	{ 0xDC, "+4.5" },
	{ 0xD8, "+5.0" },
};

STRTOVALFUNC(StrToCor, corvals)
VALTOSTRFUNC(CorToStr, corvals)

static valstr metvals[] = {
	{ 0x00, "  " }, /* Hack for short format */
	{ 0xFF, "??" },
	{ 0x00, "# " }, /* EVAL */
	{ 0x01, "* " }, /* PART */
	{ 0x10, "[]" }, /* AVR  */
	{ 0x11, ". " }, /* SPOT */
};

STRTOVALFUNC(StrToMet, metvals)
VALTOSTRFUNC(MetToStr, metvals)

static valstr touvals[] = {
	{ 0x00, " " },
	{ 0x40, "*" },	/* Time value over/underflow */
};

STRTOVALFUNC(StrToTou, touvals)
VALTOSTRFUNC(TouToStr, touvals)

static valstr aouvals[] = {
	{ 0x00, " " },
	{ 0x20, "*" },	/* Aperature value over/underflow */
};

STRTOVALFUNC(StrToAou, aouvals)
VALTOSTRFUNC(AouToStr, aouvals)

static valstr aebvals[] = {
	{ 0x00, "   " },
	{ 0x01, "AEB" },
};

STRTOVALFUNC(StrToAeb, aebvals)
VALTOSTRFUNC(AebToStr, aebvals)

static valstr monvals[] = {
	{ 0x00, "XXX" },
	{ 0x01, "Jan" },
	{ 0x02, "Feb" },
	{ 0x03, "Mar" },
	{ 0x04, "Apr" },
	{ 0x05, "May" },
	{ 0x06, "Jun" },
	{ 0x07, "Jul" },
	{ 0x08, "Aug" },
	{ 0x09, "Sep" },
	{ 0x0A, "Oct" },
	{ 0x0B, "Nov" },
	{ 0x0C, "Dec" },
};

STRTOVALFUNC(StrToMon, monvals)
/* VALTOSTRFUNC(MonToStr, monvals) */

static char _5[] = ".5";

int StrToMm(str)
char *str;
{
	return(2*atoi(str)+StrCmp(str+MMLLEN-2, ==, _5));
} /* StrToMm */

void MmToStr(buf, val)
char buf[];
int val;
{
	sprintf(buf, val ? "%*d%s" : "------", MMLLEN-2, val / 2, val & 1 ? _5 : "mm");
} /* MmToStr */

int IToBcd(i)
int i;
{
	return(((i/10)<<4) | (i%10));
} /* IToBcd */

int BcdToI(nr)
bcd nr;
{
	return((((nr>>4)&0x0F) * 10 + (nr&0x0F))&0xFF);
} /* BcdToI */

int ValidBcd(b, upper)
bcd b;
int upper;
{
	return((b>=0) && (b<upper));
} /* ValidBcd */

static int strtobcd(s)
char s[];
{
	return(!*s || s[0]==' ' && s[1]==' ' ? NOTIME : ((((s[0]-'0')<<4)&0xF0)+((s[1]-'0')&0x0F))&0xFF);
} /* strtobcd */

bcdtime StrToTim(str)
char *str;
{
	bcdtime bt;
	char buf[10];

	strncpy(buf, str, sizeof buf);
	bt.hour = strtobcd(buf+0);
	bt.min  = strtobcd(buf+3);
	bt.sec  = strtobcd(buf+6);
	return(bt);
} /* StrToTim */

void TimToStr(buf, bt, nosecs)
char buf[];
bcdtime bt;
int nosecs;
{
	int h, m, s;

	h = BcdToI(bt.hour);
	m = BcdToI(bt.min);
	s = BcdToI(bt.sec);
	memset(buf, ' ', 8);
	buf[8] = '\0';
	if (ValidBcd(h, 24) && ValidBcd(m, 60)) {
		sprintf(buf, "%2d:%02d", h, m);
		if (ValidBcd(s, 60))
			sprintf(buf+5, ":%02d", s);
			else buf[5] = ' ';
	}

	if (nosecs)
		buf[5] = '\0';
} /* TimToStr */

void FrnoToStr(buf, val)
char buf[];
int val;
{
	int aa;

	if (aa = (val>1000)) {
		val -= FRNOAA;
	}
	sprintf(buf, "%2d%c", val, aa ? 'A' : ' ');
} /* FrnoToStr */

#ifdef MAIN

char NullStr[2];

enum { ANYCAM, A1, T90, AF1, AFT };

static valstr camtypes[] = {
	{ ANYCAM, NullStr },
	{ A1,     "a1" },
	{ A1,     "A1" },
	{ T90,    "t90" },
	{ T90,    "T90" },
	{ AF1,    "af1" },
	{ AF1,    "AF1" },
	{ AFT,    "aft" },
	{ AFT,    "AFT" },
};

STRTOVALFUNC(StrToCam, camtypes)
VALTOSTRFUNC(CamToStr, camtypes)

static valstr opts[] = {
	{ ANYCAM, NullStr },
	{ A1,     "-tv" },
	{ T90,    "-s" },
	{ AF1,    "-tv" },
	{ AFT,    "-sv" },
};

VALTOSTRFUNC(CamToOpt, opts)

char *camtoopt(cam)
char *cam;
{
	static char opt[8];

	CamToOpt(opt, StrToCam(cam));
	return(opt);
} /* camtoopt */

char *camtostr(cam)
char *cam;
{
	static char camname[8];

	CamToStr(camname, StrToCam(cam));
	return(camname);
} /* camtostr */

int main(argc, argv)
int argc;
char *argv[];
{
	while (*++argv)
		printf("StrToCam(\"%s\"): %s %s\n", *argv, camtostr(*argv), camtoopt(*argv));
	return(0);
} /* main */

#endif /* MAIN */

