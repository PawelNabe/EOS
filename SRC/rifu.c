/* (C) Copyright by PaN Super Sonic Software Service 1990,1991,1992,1993,1994,1995,1998,2002,2006,2007	*/
/*                  expert for hi tech & low budget							*/

#include <stdio.h>

#ifdef _WIN32
#include <stdlib.h>
#include <string.h>
#endif /* _WIN32 */

#include <time.h>
#include <errno.h>
#include "rifu.h"
#include "pos.h"
#include "valstr.h"

extern char *strerror(), *optarg, *getenv(), **SetArgv();
extern int errno, opterr, optind, GlobCmdLn();
extern long atol();
extern PRTFUNC DispFrameFunc, DispFrameJFunc, PrtFrameAFunc,
	PrtFrameEFunc, PrtFrameGFunc, PrtFrameIFunc, PrtFrameRFunc,
	PrtFrameCFunc;
extern void PrtSetPgFmtI5(), SetLang();

static char *argv0, *framerange="0-100", *negframerange;
static unsigned int nosecs, notime, novalues;
static int fnoOff, aa, pr, negOff, debug;
static time_t corsec;

static FRAME fb;

#ifndef	__DATE__
#define	__DATE__	"Tue 20. Mar 2007"
#endif

#ifndef	__TIME__
#define	__TIME__	"13:08"
#endif
#ifdef __TIMESTAMP__
#define ___TS___ " " __TIMESTAMP__
#else
#define ___TS___ ""
#endif

static void rifuVersion()
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

	datestr_version();
#if	__MSDOS__ || _WIN32
	expfn_version();
#endif	/* __MSDOS__ || _WIN32 */
	valstr_version();
	prtdVersion();
	prtfVersion();
	setargv_version();

	exit(0);
} /* rifuVersion */

#if	__MSDOS__ || _WIN32
#define	TRDTOU(s)
#else
extern void trdtou(char *s);
#define	TRDTOU(s)	trdtou(s)
#endif	/* __MSDOS__ || _WIN32 */

static void tritod(char *s)
{
	for ( ; ; ++s)
		switch (*s) {
			case '\000' : return;
			case '\340' : *s='\216'; break;	/* Ae */
			case '\344' : *s='\231'; break;	/* Oe */
			case '\346' : *s='\232'; break;	/* Ue */
		}
} /* tritod */

#ifdef	COMMENT
char *basename(register char *name)	/* simple version */
{
	register char *p;

	for (p = name; *p++; )
		;
	while (--p >= name && *p != '/')
		;
	return(++p);
} /* basename */
#endif

static char *basename(register char *name)	/* sophisticated version */
{
	static char buf[1024];
	register char *bp;

	for (bp = buf; *bp = *name++; )
		++bp;
	while (--bp >= buf && (*bp == '/'
#if	__MSDOS__ || _WIN32
					|| *bp == '\\'
#endif
				))
		*bp = '\0';
	while (  bp >= buf && (*bp != '/'
#if	__MSDOS__ || _WIN32
					&& *bp != '\\'
#endif
				))
		--bp;
	return(++bp);
} /* basename */

static void corTime(pfb, sec)
FRAME *pfb;
time_t sec;
{
	static time_t t;
	struct tm *ptm;

	if (!t)
		ptm = localtime(&t);	/* get isdst */
	ptm->tm_year = pfb->p_year - 1900;
	ptm->tm_mon = pfb->p_month - 1;
	ptm->tm_mday = pfb->p_day;
	ptm->tm_hour = BcdToI(pfb->btime.hour);
	ptm->tm_min = BcdToI(pfb->btime.min);
	ptm->tm_sec = BcdToI(pfb->btime.sec);
	sec += mktime(ptm);
	ptm = localtime(&sec);

	pfb->p_year = ptm->tm_year + 1900;
	pfb->p_month = ptm->tm_mon + 1;
	pfb->p_day = ptm->tm_mday;
	pfb->btime.hour = IToBcd(ptm->tm_hour);
	pfb->btime.min = IToBcd(ptm->tm_min);
	pfb->btime.sec = IToBcd(ptm->tm_sec);
} /* corTime */

#define GETIFUC() fgetc(fpDat)

static int getFrame(fpDat)
register FILE *fpDat;
{
	register int ch;

	ch = GETIFUC();

	if ((ch == 0xFF) || (ch == EOF))
		return(0);
	memset((char *) &fb, '\0', sizeof(fb));
	fb.fr_no = BcdToI(ch);

	ch = GETIFUC();

	fb.f_no = (ch & 0x0F) * 100;
	ch = GETIFUC();
	fb.f_no += BcdToI(ch);
	fb.f_no += fnoOff;

	ch = GETIFUC();
	fb.tv = ch & 0xFC;
	fb.flash = ch & 0x01;

	ch = GETIFUC();
	fb.av = ch & 0x7C;
	fb.ae_mode = ch & 0x83; /* should be 0x03 AE_P, 0x80 AE_DEP */

	ch = (GETIFUC() << 8) ;
	ch += GETIFUC() & 0xFF;
	fb.mm = ch;

	ch = GETIFUC();
	if ((ch == 0xFF) || (ch == EOF)) {
		/* Short format */
		fb.sf = 1;
		return(1);
	}
	if (debug) {
		int c = ch;
		fprintf(stdout, "%3d:%02d 0x%02x 0x%02x\n", fb.f_no, fb.fr_no, c & 0xFF, (c  & 0x8E) | fb.flash);
	}

	fb.flash |= ch & 0x8E;
	fb.metering = (ch & 0x11);
	fb.av_our = ch & 0x20;
	fb.tv_our = ch & 0x40;

	ch = GETIFUC();
	fb.iso = ch & 0xFE;	/* what does the LSB mean? */

	if ((ch = BcdToI(GETIFUC()) + 1900) < 1980)
		ch += 100;
	fb.p_year = ch;
	fb.p_month = BcdToI(GETIFUC());
	fb.p_day = BcdToI(GETIFUC());
	fb.btime.hour = GETIFUC();
	fb.btime.min  = GETIFUC();
	fb.btime.sec  = GETIFUC();

	fb.lens_av = GETIFUC() & 0x7C;

	ch = GETIFUC();
	fb.aeb = ch & 0x01;
	if (ch & 0x20)
		fb.correct = GETIFUC() & 0xFF;
	if (ch & 0x10)
		fb.me = BcdToI(GETIFUC());
	if (ch & 0x04) {
		register int ch1;

		ch1 = (GETIFUC() << 8) ;
		ch1 += GETIFUC() & 0xFF;
		fb.lens_mm1 = ch1;

		ch1 = (GETIFUC() << 8) ;
		ch1 += GETIFUC() & 0xFF;
		fb.lens_mm2 = ch1;
	}
	if (ch & 0x02) {
		register int i;

		for (i=0; i < NOTELEN && ((ch = GETIFUC()) != 0xFF); ++i)
			fb.note[i] = ch;
		fb.note[i] = '\0';

		tritod(fb.note);
		TRDTOU(fb.note);
	}
	while ((ch != 0xFF) && (ch != EOF))
		ch = GETIFUC();
	return(1);
} /* getFrame */

static void getHeader(fnDat, fpDat)
char *fnDat;
register FILE *fpDat;
{
	int dat_len, df;
	unsigned char header[2+GAP+1];

	fread(header, sizeof header, 1, fpDat);
	dat_len = header[0];
	dat_len += header[1] << 8;
	df = header[2+GAP];			/* Date Format */
	if (!pr)
		printf("-------- %s -------- %d bytes %s\n", fnDat, dat_len, df & 0x08 ? "MMMDDYY" : df & 0x04 ? "DDMMYY" : "YYMMDD");
	if (header[2] == 0x12 && (header[3]==0x34 || header[3]=='A')) {
		register char *left, *right, *endb;

		if (header[3]=='A')
			aa = FRNOAA;
		left = (char *) header+4;

		tritod(left);
		TRDTOU(left);

		for (endb = left+strlen(left); left<endb; left=right) {
			for (right=left; *right && (*right!='\n'); ++right)
				;
			*right++ = '\0';
			if (!pr)
				printf("* %s\n", left);
		}
	}
} /* getHeader */

static char *toUpperS(char *str)
{
	register char *s = str;

	while (*s = toupper(*s))
		++s;
	return(str);
} /* toUpperS */

static int isinrange(frno)
int frno;
{
	return(IsInRange(frno, negframerange) ? 0 : IsInRange(frno, framerange));
} /* isinrange */

static void readDatFile(fnDat, prtfunc, prt2file)
char *fnDat;
register PRTFUNC *prtfunc;
int prt2file;
{
	register FILE *fpDat;
	char bn[256];

	strcpy(bn, basename(fnDat));
	bn[strlen(bn)-4] = '\0';
	if (fpDat = fopen(fnDat, "rb")) {
		if (prt2file)
			freopen(bn, "w", stdout);
		toUpperS(bn);
		getHeader(fnDat, fpDat);
		while (getFrame(fpDat)) {
			register int rfact;

			fb.fr_no += negOff;
			rfact = isinrange(fb.fr_no);
			fb.fr_no += aa;

			if (corsec)
				corTime(&fb, corsec);

			while (--rfact>=0)
				prtfunc->prtFrame(&fb, notime, nosecs, novalues, bn);
		}
		prtfunc->prtAtEof();
		fclose(fpDat);
	} else fprintf(stderr, "%s: cannot open file \"%s\": %s\n", argv0, fnDat, strerror(errno));
} /* readDatFile */

static int coffset(char *str)
{
	register char *p;

	for (p=str; *p && *p!='.'; ++p)
		;
	return(*p ? atoi(p+1)-atoi(str) : atoi(str));
} /* coffset */

#ifdef __MSDOS__

static char *initialcwd;

static void cdback()
{
	chdir(initialcwd);
} /* cdback */

#endif /* __MSDOS__ */

static void exiterrno()
{
	exit(errno);
} /* exiterrno */

static int chDir(path)
char *path;
{
#ifdef __MSDOS__
#include <dir.h>
#include <signal.h>

	if (initialcwd == NULL) {
		if (initialcwd = getcwd(NULL, 1024)) {
			atexit(cdback);
			signal(SIGINT, exiterrno);
		} else {
			fprintf(stderr, "%s: cannot get current directory: %s\n", argv0, strerror(errno));
			exiterrno();
		}
	}
#endif /* __MSDOS__ */
	return(chdir(path));
} /* chDir */

static int rifuusage()
{
	fprintf(stderr, "Usage: %s options_or_datfiles\n", argv0);
	fputs("\twhere options are:\n", stderr);
	fputs("\t-A (append 'A' to each negnumber on/off for one film)\n", stderr);
	fputs("\t-C directory (chdir to directory)\n", stderr);
	fputs("\t-F filmnooffset ([-999..999] or f.t)\n", stderr);
	fputs("\t-L lang (set date output language) (En,De,Fr,Pl,R)\n", stderr);
	fputs("\t-O (automatic setting of options depending on filename off/on)\n", stderr);
	fputs("\t-R no-print-framerange (like 5,13,20-)\n", stderr);
	fputs("\t-S seconds (correct time of exposure)\n", stderr);
	fputs("\t-V (print version and exit)\n", stderr);
	fputs("\t-f argfile\n", stderr);
	fputs("\t-j (join dates and notes on a single line)\n", stderr);
	fputs("\t-n negativoffset ([-7..+8] or f.t)\n", stderr);
	fputs("\t-p format (prepare output for printer)\n", stderr);
	fputs("\t   format is one of\n", stderr);
	fputs("\t      'a' AVERY for the stickers\n", stderr);
	fputs("\t      'c' CSV for Excel and others\n", stderr);
	fputs("\t      'f' print to file. Filename is basename of .dat file\n", stderr);
	fputs("\t-r framerange (like -5,8-10,13*2,20-)\n", stderr);
	fputs("\t-s (display seconds off/on (only valid with -O))\n", stderr);
	fputs("\t-t (display time of exposure off/on (only valid with -O))\n", stderr);
	fputs("\t-v (display aperature/time-values off/on (only valid with -O))\n", stderr);
#if __MSDOS__ || _WIN32
	fputs("Examples:\n", stderr);
	fprintf(stderr, "\tfor %%i in (*.dat) do @%s -pc %%i > %%~ni.csv\n", basename(argv0));
#endif /* _WIN32 */
	return(0);
} /* rifuusage */

int main(int argc, char *argv[])
{
	PRTFUNC *prtfunc = &DispFrameFunc;
	int noautoopt = 0, prt2file = 0;

	argv0 = argv[0];

	(argc<=1) && rifuusage();

#if	__MSDOS__ || _WIN32
	GlobCmdLn(&argc, &argv);
#endif

	SetLang(getenv("LANG"));

	opterr = 1;
	optind = 0;	/* must do this because of "++optind..." */

	while (++optind<argc) {
		register int c;

		while ((c = getopt(argc, argv, "?AC:F:L:OR:S:Vdf:hjn:p:qr:stv")) != EOF) {
			switch (c) {
				case 'A' :
					aa = aa ? 0 : FRNOAA;
					break;
				case 'C' :
					if (chDir(optarg)) {
						fprintf(stderr, "%s: cannot chdir(\"%s\"): %s\n", argv0, optarg, strerror(errno));
						exiterrno();
					}
					break;
				case 'F' :
					if ((fnoOff = coffset(optarg)) >=10000 || (-10000>=fnoOff)) {
						fprintf(stderr, "%s: Invalid film number offset \"%s\". Must be within +-9999.\n", argv0, optarg);
						exit(22);
					}
					break;
				case 'L' :
					SetLang(optarg);
					break;
				case 'O' :
					noautoopt ^= 1;
					break;
				case 'R' :
					negframerange = optarg;
					break;
				case 'S' :
					corsec = atol(optarg);
					break;
				case 'V' :
					rifuVersion();
				case 'd' :
					debug = 1;
					break;
				case 'f' :
					--optind;
					if (SetArgv(&argc, &argv, optarg, optind, optind))
						continue;
					++optind;
					break;
				case 'j' :
					prtfunc = &DispFrameJFunc;
					break;
				case 'n' :
					negOff = coffset(optarg);
					break;
				case 'p' :
					++pr;
					switch (*optarg) {
						case 'A' :
						case 'a' :
							prtfunc = &PrtFrameAFunc;
							break;
						case 'C' :
						case 'c' :
							prtfunc = &PrtFrameCFunc;
							break;
						case 'E' :
						case 'e' :
							prtfunc = &PrtFrameEFunc;
							break;
						case 'F' :
						case 'f' :
							prtfunc = &DispFrameFunc;
							prt2file = 1;
							pr = 0;
							break;
						case 'G' :
						case 'g' :
							prtfunc = &PrtFrameGFunc;
							break;
						case 'I' :
						case 'i' :
							prtfunc = &PrtFrameIFunc;
							PrtSetPgFmtI5(optarg+1);
							break;
						case 'R' :
						case 'r' :
							prtfunc = &PrtFrameRFunc;
							break;
						default  :
							fprintf(stderr, "%s: Invalid print format '%c'\n", argv0, *optarg);
							exit(22);
					}
					break;
				case 'r' :
					framerange = optarg;
					break;
				case 's' :
					nosecs ^= 1;
					break;
				case 't' :
					notime ^= 1;
					break;
				case 'v' :
					novalues ^= 1;
					break;
				default  :
					return(rifuusage());
			}
		}
		if (optind<argc) {
			static char _dat[] = ".dat";
			char buf[512];
			int slen;

			strcpy(buf, argv[optind]);
			if (!noautoopt) {
				nosecs = notime = novalues = 0;
				if (StrnCmp(buf, ==, "af1", 3) || StrnCmp(buf, ==, "AF1", 3))
					nosecs = notime = novalues = 1;
				if (StrnCmp(buf, ==, "aft", 3) || StrnCmp(buf, ==, "AFT", 3) || StrnCmp(buf, ==, "mjz", 3) || StrnCmp(buf, ==, "MJZ", 3))
					nosecs = novalues = 1;
				if (StrnCmp(buf, ==, "t90", 3) || StrnCmp(buf, ==, "T90", 3))
					nosecs = 1;
			}
			slen=strlen(buf)-4;
			if (slen<0 || (StrCmp(buf+slen, !=, _dat) && StrCmp(buf+slen, !=, ".DAT")))
				strcat(buf, _dat);
			readDatFile(buf, prtfunc, prt2file);
			aa = 0;	/* reset 'A' */
		}
	}
	prtfunc->prtAtExit();
	return(0);
} /* main */

