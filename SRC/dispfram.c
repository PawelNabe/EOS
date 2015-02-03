/* (C) Copyright by PaN Super Sonic Software Service 1991, 1998, 2002, 2004, 2006	*/
/*                  expert for hi tec & low budget					*/

#include <stdio.h>
#include "rifu.h"
#include "pos.h"
#include "valstr.h"

#ifndef	__DATE__
#define	__DATE__	"Tue 17. Jul 2002"
#endif

#ifndef	__TIME__
#define	__TIME__	"17:20"
#endif
#ifdef __TIMESTAMP__
#define ___TS___ " " __TIMESTAMP__
#else
#define ___TS___ ""
#endif

void prtdVersion()
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
	fflush(NULL);
} /* prtdVersion */

static void formfield(formfunc, dest, val, sf, len, lc, rc)
void (*formfunc)();
char *dest;
int val, len, sf, lc, rc;
{
	if (sf)
		memset(dest, ' ', len);
	else (*formfunc)(dest, val, sf);
	if (lc)
		dest[-1] = lc;
	dest[len] = rc;
} /* formfield */

static void z2dtostr(buf, val)
char buf[];
int val;
{
	sprintf(buf, "%02d", val);
} /* z2dtostr */

static void metostr(buf, val)
char buf[];
int val;
{
	if (val)
		z2dtostr(buf, val);
} /* metostr */

void dispFrame(pfb, notime, nosecs, novalues, notesep)
FRAME *pfb;
int notime, nosecs, novalues, notesep;
{
	static int oldfno = -1, oldyy, oldmm, olddd;
	char buf[256];

	if ((oldfno != pfb->f_no) || olddd != pfb->p_day || oldmm != pfb->p_month || oldyy != pfb->p_year) {
		static char mmmm[] = "-----------------";

		DateStr(buf, pfb->p_day, pfb->p_month, pfb->p_year);
		printf("\n%4d %s %s %s\n", pfb->f_no, mmmm, buf, mmmm);
	}
	oldfno = pfb->f_no;

	olddd = pfb->p_day;
	oldmm = pfb->p_month;
	oldyy = pfb->p_year;

	memset(buf, ' ', sizeof buf);

	formfield(FrnoToStr,buf+FRNOPOS, pfb->fr_no,         0, FRNOLEN, '[', ']');

	formfield(metostr,  buf+  MEPOS, pfb->me,            0,   MELEN, pfb->me ? ':' : ' ', ' ');

	formfield(MetToStr, buf+ METPOS, pfb->metering, pfb->sf, METLEN,  ' ',  ' ');
	formfield(FlaToStr, buf+  FLPOS, pfb->flash&~0x02,     0, FLLEN,  ' ',  ' ');
	formfield(CurToStr, buf+ CURPOS, pfb->flash&0x02,     0, CURLEN, '\0',  ' ');
	if (!novalues) {
		formfield( TvToStr, buf+  TVPOS, pfb->tv,      0, TVLEN,  ' ',  ' ');
		formfield(TouToStr, buf+ TOUPOS, pfb->tv_our, 0, TOULEN, '\0',  ' ');
		formfield( AvToStr, buf+  AVPOS, pfb->av,      0, AVLEN,  ' ',  ' ');
		formfield(AouToStr, buf+ AOUPOS, pfb->av_our, 0, AOULEN, '\0',  ' ');
	}
	formfield(AemToStr, buf+ PRGPOS, pfb->ae_mode,        0, PRGLEN,  ' ',  ' ');
	formfield(AebToStr, buf+ AEBPOS, pfb->aeb,            0, AEBLEN,  ' ',  ' ');
	formfield(IsoToStr, buf+ ISOPOS, pfb->iso,      pfb->sf, ISOLEN,  ' ',  ' ');
	formfield(CorToStr, buf+CORRPOS, pfb->correct,       0, CORRLEN, '\0',  ' ');
	if (!notime && !pfb->sf) {
		TimToStr(buf+TIMEPOS, pfb->btime, nosecs);
		buf[TIMEPOS+TIMELEN] = ' ';
		if (nosecs)
			memset(buf+SECPOS-1, ' ', SECLEN+1);
	}
	if (pfb->lens_av || pfb->mm || pfb->lens_mm1) {
		formfield( AvToStr, buf+ AVLPOS, pfb->lens_av, pfb->sf, AVLLEN, ' ',  ' ');
		formfield( MmToStr, buf+ MMLPOS, pfb->mm,            0, MMLLEN, '/',  ' ');
	}
	if (pfb->lens_mm1) {
		formfield(MmToStr, buf+ MM1POS, pfb->lens_mm1, 0, MM1LEN, ' ',  ' ');
		formfield(MmToStr, buf+ MM2POS, pfb->lens_mm2, 0, MM2LEN, '-',  ' ');
	} else if (notesep!=' ' || !pfb->note[0])
		buf[MMLPOS+MMLLEN] = '\0';

	buf[MM2POS+MM2LEN] = '\0';

	fputs(buf, stdout);
	if (pfb->note[0])
		printf("%c%s", notesep, pfb->note);
	fputc('\n', stdout);
} /* dispFrame */

void DispFrame(pfb, notime, nosecs, novalues)
FRAME *pfb;
int notime, nosecs, novalues;
{
	dispFrame(pfb, notime, nosecs, novalues, '\n');
} /* DispFrame */

void DispFrameJ(pfb, notime, nosecs, novalues)
FRAME *pfb;
int notime, nosecs, novalues;
{
	dispFrame(pfb, notime, nosecs, novalues, ' ');
} /* DispFrameJ */

static void dispAtEofEG()
{
	fputc('\f', stdout);
} /* dispAtEofEG */

/* =================== CSV =================== */

static void prtCsvStr(str, sep)
char *str, sep;
{
	register char c;

	/* fputc('=', stdout); */
	fputc('"', stdout);
	for ( ; c = str[0]; ++str) {
		if (c == '"') {
			fputc('"', stdout);
		}
		fputc(c, stdout);
	}
	fputc('"', stdout);
	fputc(sep, stdout);
} /* prtCsvStr */

static void prtCsvField(formfunc, val, leaveempty)
void (*formfunc)();
int val, leaveempty;
{
	char buf[64];

	memset(buf, '\0', sizeof buf);
	if (!leaveempty)
		(*formfunc)(buf, val, leaveempty);
	prtCsvStr(buf, ',');
} /* prtCsvField */

static void p04d(buf, val)
char buf[];
int val;
{
	sprintf(buf, "%04d", val);
} /* p04d */

void trdtou(char *s)
{
	for ( ; ; ++s)
		switch (*s) {
			case '\000' : return;
			case '\202' : *s='\351'; break;	/* e' */
			case '\204' : *s='\344'; break;	/* ae */
			case '\224' : *s='\366'; break;	/* oe */
			case '\201' : *s='\374'; break;	/* ue */
			case '\216' : *s='\304'; break;	/* Ae */
			case '\231' : *s='\326'; break;	/* Oe */
			case '\232' : *s='\334'; break;	/* Ue */
			case '\341' : *s='\337'; break;	/* sz */
		}
} /* trdtou */

static void PrtFrameC(pfb, notime, nosecs, novalues, bn)
FRAME *pfb;
int notime, nosecs, novalues;
char *bn;
{
	static unsigned int numpics;
	char date[32], tim[16];

	if (!numpics++) {
		prtCsvStr("FILENAME", ',');
		prtCsvStr("FILMNUM", ',');
		prtCsvStr("FRAMENUM", ',');
		prtCsvStr("ME", ',');

		prtCsvStr("DATE", ',');
		prtCsvStr("TIME", ',');

		prtCsvStr("TV", ',');
		prtCsvStr("TVOU", ',');
		prtCsvStr("AV", ',');
		prtCsvStr("AVOU", ',');
		prtCsvStr("PROG", ',');
		prtCsvStr("AEB", ',');
		prtCsvStr("METERING", ',');
		prtCsvStr("ISO", ',');
		prtCsvStr("+/-", ',');
		prtCsvStr("FLASH", ',');
		prtCsvStr("CURTAIN", ',');
		prtCsvStr("FOCAL LENGTH", ',');
		prtCsvStr("LENS SPEED", ',');
		prtCsvStr("ZOOMMIN", ',');
		prtCsvStr("ZOOMMAX", ',');

		prtCsvStr("NOTE", '\n');
	}

	prtCsvStr(bn, ',');
	prtCsvField(p04d, pfb->f_no, 0);
	prtCsvField(FrnoToStr, pfb->fr_no, 0);
	prtCsvField(metostr, pfb->me, 0);

	DateStr(date, pfb->p_day, pfb->p_month, pfb->p_year);
	trdtou(date);
	if (notime) 
		memset(tim, '\0', sizeof tim);
	else {
		TimToStr(tim, pfb->btime, nosecs);
	}
	prtCsvStr(date, ',');
	prtCsvStr(tim, ',');

	prtCsvField( TvToStr, pfb->tv,     0);
	prtCsvField(TouToStr, pfb->tv_our, 0);
	prtCsvField( AvToStr, pfb->av,     0);
	prtCsvField(AouToStr, pfb->av_our, 0);

	prtCsvField(AemToStr, pfb->ae_mode,0);
	prtCsvField(AebToStr, pfb->aeb,    0);

	prtCsvField(MetToStr, pfb->metering, pfb->sf);

	prtCsvField(IsoToStr, pfb->iso,      pfb->sf);
	prtCsvField(CorToStr, pfb->correct, 0);

	prtCsvField(FlaToStr, pfb->flash&~0x02, 0);
	prtCsvField(CurToStr, pfb->flash&0x02, 0);

	prtCsvField( MmToStr, pfb->mm,      0);
	prtCsvField( AvToStr, pfb->lens_av, pfb->sf);
	prtCsvField(MmToStr, pfb->lens_mm1, 0);
	prtCsvField(MmToStr, pfb->lens_mm2, 0);

	trdtou(pfb->note);
	prtCsvStr(pfb->note, '\n');
} /* PrtFrameC */

static void nullFunc()
{
} /* nullFunc */

PRTFUNC DispFrameFunc = {
		nullFunc,
		DispFrame,
		dispAtEofEG,
		nullFunc,
	}, DispFrameJFunc = {
		nullFunc,
		DispFrameJ,
		dispAtEofEG,
		nullFunc,
	}, PrtFrameCFunc = {
		nullFunc,
		PrtFrameC,
		nullFunc,
		nullFunc,
	};

