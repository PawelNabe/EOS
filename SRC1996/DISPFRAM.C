/* (C) Copyright by PaN Super Sonic Software Service 1991	*/
/*                  expert for hi tec & low budget		*/

#include <stdio.h>
#include "rifu.h"
#include "pos.h"
#include "valstr.h"

static void formfield(formfunc, dest, val, len, lc, rc)
void (*formfunc)();
char *dest;
int val, len, lc, rc;
{
	(*formfunc)(dest, val);
	if (lc)
		dest[-1] = lc;
	if (rc)
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
	static int oldfno, oldyy, oldmm, olddd;
	char buf[256];

	if ((oldfno && (oldfno != pfb->f_no)) || olddd != pfb->p_day || oldmm != pfb->p_month || oldyy != pfb->p_year) {
		static char mmmm[] = "-----------------";

		DateStr(buf, pfb->p_day, pfb->p_month, pfb->p_year);
		printf("\n%4d %s %s %s\n", pfb->f_no, mmmm, buf, mmmm);
	}
	oldfno = pfb->f_no;

	olddd = pfb->p_day;
	oldmm = pfb->p_month;
	oldyy = pfb->p_year;

	memset(buf, ' ', sizeof buf);

	formfield(FrnoToStr,buf+FRNOPOS, pfb->fr_no,   FRNOLEN, '[', ']');

	formfield(metostr,  buf+  MEPOS, pfb->me,        MELEN, pfb->me ? ':' : ' ', ' ');

	formfield(MetToStr, buf+ METPOS, pfb->metering,  METLEN,  ' ',  ' ');
	formfield(FlaToStr, buf+  FLPOS, pfb->flash&~0x02,FLLEN,  ' ',  ' ');
	formfield(CurToStr, buf+ CURPOS, pfb->flash&0x02,CURLEN, '\0',  ' ');
	if (!novalues) {
		formfield( TvToStr, buf+  TVPOS, pfb->tv,         TVLEN,  ' ',  ' ');
		formfield(TouToStr, buf+ TOUPOS, pfb->tv_our,    TOULEN, '\0',  ' ');
		formfield( AvToStr, buf+  AVPOS, pfb->av,         AVLEN,  ' ',  ' ');
		formfield(AouToStr, buf+ AOUPOS, pfb->av_our,    AOULEN, '\0',  ' ');
	}
	formfield(AemToStr, buf+ PRGPOS, pfb->ae_mode,   PRGLEN,  ' ',  ' ');
	formfield(AebToStr, buf+ AEBPOS, pfb->aeb,       AEBLEN,  ' ',  ' ');
	formfield(IsoToStr, buf+ ISOPOS, pfb->iso,       ISOLEN,  ' ',  ' ');
	formfield(CorToStr, buf+CORRPOS, pfb->correct,  CORRLEN, '\0',  ' ');
	if (!notime) {
		TimToStr(buf+TIMEPOS, pfb->btime);
		buf[TIMEPOS+TIMELEN] = ' ';
		if (nosecs)
			memset(buf+SECPOS-1, ' ', SECLEN+1);
	}
	if (pfb->lens_av || pfb->mm || pfb->lens_mm1) {
		formfield( AvToStr, buf+ AVLPOS, pfb->lens_av, AVLLEN, ' ',  ' ');
		formfield( MmToStr, buf+ MMLPOS, pfb->mm,      MMLLEN, '/',  ' ');
	}
	if (pfb->lens_mm1) {
		formfield(MmToStr, buf+ MM1POS, pfb->lens_mm1, MM1LEN, ' ',  ' ');
		formfield(MmToStr, buf+ MM2POS, pfb->lens_mm2, MM2LEN, '-',  ' ');
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
	};

