/* (C) Copyright by PaN Super Sonic Software Service 1991	*/
/*                  expert for hi tec & low budget		*/

#include <stdio.h>
#include <time.h>
#include "rifu.h"
#include "pos.h"
#include "valstr.h"

#ifndef	__DATE__
#define	__DATE__	"5. Jan 1994"
#endif

#ifndef	__TIME__
#define	__TIME__	"15:44"
#endif

extern char *sys_errlist[], *strncpy();
extern int errno;

extern FILE *FreOpen();

char *argv0;

static char *prepln(out, in, pos, len)
char *out, *in;
int pos, len;
{
	memset(out, '\0', len+1);
	return(strncpy(out, in+pos, len));
} /* prepln */

static void lntofb(pfb, line)
FRAME *pfb;
char *line;
{
	char subln[32];

	pfb->fr_no    =     atoi(prepln(subln, line, FRNOPOS, FRNOLEN));
	pfb->me       =     atoi(prepln(subln, line,   MEPOS,   MELEN));
	pfb->metering = StrToMet(prepln(subln, line,  METPOS,  METLEN));
	pfb->flash    = StrToFla(prepln(subln, line,   FLPOS,   FLLEN));
	pfb->flash   |= StrToCur(prepln(subln, line,  CURPOS,  CURLEN));
	pfb->tv       = StrToTv (prepln(subln, line,   TVPOS,   TVLEN));
	pfb->tv_our   = StrToTou(prepln(subln, line,  TOUPOS,  TOULEN));
	pfb->av       = StrToAv (prepln(subln, line,   AVPOS,   AVLEN));
	pfb->av_our   = StrToAou(prepln(subln, line,  AOUPOS,  AOULEN));
	pfb->ae_mode  = StrToAem(prepln(subln, line,  PRGPOS,  PRGLEN));
	pfb->aeb      = StrToAeb(prepln(subln, line,  AEBPOS,  AEBLEN));
	pfb->iso      = StrToIso(prepln(subln, line,  ISOPOS,  ISOLEN));
	pfb->correct  = StrToCor(prepln(subln, line, CORRPOS, CORRLEN));
	pfb->btime    = StrToTim(prepln(subln, line, TIMEPOS, TIMELEN));
	pfb->lens_av  = StrToAv (prepln(subln, line,  AVLPOS,  AVLLEN));
	pfb->mm       = StrToMm (prepln(subln, line,  MMLPOS,  MMLLEN));
	pfb->lens_mm1 = StrToMm (prepln(subln, line,  MM1POS,  MM1LEN));
	pfb->lens_mm2 = StrToMm (prepln(subln, line,  MM2POS,  MM2LEN));
} /* lntofb */

static char mmmm[] = "-----------------";

static void dmytofb(pfb, line, len)	/* Day Month Year TO FrameBuf */
FRAME *pfb;
char *line;
int len;
{
	char subln[32];

	pfb->f_no = atoi(line);
	pfb->p_day = atoi(line+len-sizeof(mmmm)-4-1-3-1-2);
	pfb->p_month = StrToMon(prepln(subln, line, len-sizeof(mmmm)-4-1-3, 3));
	pfb->p_year= atoi(line+len-sizeof(mmmm)-4);
	if ((pfb->p_year-=1900) >= 100)
		pfb->p_year -= 100;
} /* dmytofb */

static void resetfb(pfb)
FRAME *pfb;
{
	pfb->fr_no = -1;
	pfb->mm = pfb->lens_mm1 = pfb->lens_mm2 = 0;
	pfb->note[0] = '\0';
} /* resetfb */

static void dumpfr(pfb)
FRAME *pfb;
{
	unsigned int mm, suff;

	if (pfb->fr_no>=0) {
		fputc(IToBcd(pfb->fr_no), stdout);
		fputc(IToBcd(pfb->f_no/100) | 0xF0, stdout);
		fputc(IToBcd(pfb->f_no%100), stdout);
		fputc((pfb->tv & 0xFC) | (pfb->flash & 0x01), stdout);
		fputc((pfb->av & 0x7C) | (pfb->ae_mode & 0x83), stdout);
		mm = pfb->mm;
		fputc((mm>>8) & 0xFF, stdout);
		fputc( mm     & 0xFF, stdout);
		fputc((pfb->flash&0x8E) | (pfb->metering&0x11) |
			pfb->av_our | pfb->tv_our, stdout);
		fputc(pfb->iso, stdout);
		fputc(IToBcd(pfb->p_year), stdout);
		fputc(IToBcd(pfb->p_month), stdout);
		fputc(IToBcd(pfb->p_day), stdout);
		fputc(pfb->btime.hour, stdout);
		fputc(pfb->btime.min, stdout);
		fputc(pfb->btime.sec, stdout);
		fputc(pfb->lens_av, stdout);
		suff = pfb->aeb;
		if (pfb->note[0])
			suff |= 0x02;
		if (pfb->lens_mm1)
			suff |= 0x04;
		if (pfb->me)
			suff |= 0x10;
		if (pfb->correct)
			suff |= 0x20;
		fputc(suff, stdout);
		if (pfb->correct)
			fputc(pfb->correct, stdout);
		if (pfb->me)
			fputc(IToBcd(pfb->me), stdout);
		if (pfb->lens_mm1) {
			fputc((pfb->lens_mm1>>8) & 0xFF, stdout);
			fputc( pfb->lens_mm1     & 0xFF, stdout);
			fputc((pfb->lens_mm2>>8) & 0xFF, stdout);
			fputc( pfb->lens_mm2     & 0xFF, stdout);
		}
		if (pfb->note[0])
			fputs(pfb->note, stdout);
		resetfb(pfb);
		fputc('\377', stdout);
	}
} /* dumpfr */

static void enddat(header, infn)
char header[], *infn;
{
	unsigned short len;

	fputs("\377\377\377\377", stdout);
	len = ftell(stdout);
	len -= 2+GAP+1;
	rewind(stdout);
	header[0] = len & 0xFF;
	header[1] = (len>>8) & 0xFF;
	if (!header[4]) {
		long t;

		time(&t);
		sprintf(header+4, "Generated %sfrom %s", asctime(localtime(&t)), infn);
	}
	header[2+GAP] = 0x00;	/* dateform */
	fwrite(header, 2+GAP+1, 1, stdout);
} /* enddat */

#ifndef __MSDOS__
static void trutod(s)
char *s;
{
	for ( ; ; ++s)
		switch (*s) {
			case '\000' : return;
			case '\351' : *s='\202'; break;	/* e' */
			case '\344' : *s='\204'; break;	/* ae */
			case '\366' : *s='\224'; break;	/* oe */
			case '\374' : *s='\201'; break;	/* ue */
			case '\304' : *s='\216'; break;	/* Ae */
			case '\326' : *s='\231'; break;	/* Oe */
			case '\334' : *s='\232'; break;	/* Ue */
			case '\337' : *s='\341'; break;	/* sz */
		}
} /* trutod */
#endif /* __MSDOS__ */

static void trdtoi(s)
char *s;
{
	for ( ; ; ++s)
		switch (*s) {
			case '\000' : return;
			case '\216' : *s='\340'; break;	/* Ae */
			case '\231' : *s='\344'; break;	/* Oe */
			case '\334' : *s='\346'; break;	/* Ue */
		}
} /* trdtoi */

static void asctodat(infn)
char *infn;
{
	char line[256], header[512];
	FRAME fb;

	memset(header, '\0', sizeof header);
	header[2] = 0x12;
	header[3] = 0x34;
	fwrite(header, 2+GAP+1, 1, stdout);

	resetfb(&fb);
	while (memset(line, '\0', sizeof line), gets(line)) {
		int len;

		if ((len = strlen(line)) && (line[0] != '\f')) {
#ifndef __MSDOS__
			trutod(line);
#endif /* __MSDOS__ */
			trdtoi(line);
			if (StrCmp(line+len-(sizeof(mmmm)-1), !=, mmmm)) {
				if ((line[0]=='*') && (line[1]==' ')) {
					sprintf(header+4, "%s\n", line+2);
				} else {
					if (len<=30) {
						strcpy(fb.note, line);
						dumpfr(&fb);
					} else {
						dumpfr(&fb);
						if ((line[FRNOPOS-1]=='[') && (line[FRNOPOS+FRNOLEN]==']')) {
							lntofb(&fb, line);
							if (line[FRNOPOS+2] == 'A')
								header[3] = 'A';
						}
					}
				}
			} else {
				dumpfr(&fb);
				dmytofb(&fb, line, len);
			}
		}
	}
	dumpfr(&fb);
	enddat(header, infn);
} /* asctodat */

int main(argc, argv)
int argc;
char *argv[];
{
	argv0 = argv[0];

#ifdef	__MSDOS__
	GlobCmdLn(&argc, &argv);
#endif

	if (argc<=1 || StrCmp(argv[1], ==, "-?")) {
		fprintf(stderr, "Usage: %s fn...\n", argv0);
		exit(0);
	}
	if (StrCmp(argv[1], ==, "-V")) {
		printf("%s: %s %s\n", argv0, __TIME__, __DATE__);
		exit(0);
	}

	if (argc>1) {
		while (*++argv) {
			static char _dat[] = ".dat", _DAT[] = ".DAT";
			char infn[256], *suff;
			int slen;

			strcpy(infn, *argv);
			slen = strlen(infn)-4;
			if ((slen>0) && (StrCmp(infn+slen, ==, (suff = _DAT)) || StrCmp(infn+slen, ==, (suff = _dat)))) {
				infn[slen] = '\0';
			} else suff = _dat;
			if (FreOpen(infn, "r", stdin)) {
				char outfn[sizeof infn];

				sprintf(outfn, "%s%s", infn, suff);
				if (FreOpen(outfn, "wb", stdout))
					asctodat(infn);
			}
		}
	} else asctodat("<stdin>");
	return(0);
} /* main */

