/* (C) Copyright by PaN Super Sonic Software Service 1991,1992,1993,1994,1995	*/
/*                  expert for hi tec & low budget				*/

#include <stdio.h>
#include "rifu.h"
#include "valstr.h"
#include "pos.h"

extern char *sys_errlist[], *optarg, *getenv(), **SetArgv();
extern int errno, opterr, optind;

#define	LA	0	/* LeftAligned */
#define	RA	1	/* RightAligned */

#ifndef	__DATE__
#define	__DATE__	"9. Feb 1995"
#endif

#ifndef	__TIME__
#define	__TIME__	"09:06"
#endif

static char *argv0, f5_6[] = "5.6";

static char *filblanks(buf, lr, len)
char *buf;
int lr, len;
{
	static char retbuf[32];
	int l;

	memset(retbuf, ' ', sizeof retbuf);
	if ((l = strlen(buf)) > len)
		l = len;
	strncpy(retbuf + ((lr==RA) ? (len - l) : 0), buf, l);
	retbuf[len] = '\0';
	return(retbuf);
} /* filblanks */

static void gausage()
{
	fprintf(stderr, "Usage: %s options\n", argv0);
	fputs("\twhere options are:\n", stderr);
	fputs("\t-0 (start with negative number 0)\n", stderr);
	fputs("\t-A (append 'A' to each negnumber on/off)\n", stderr);
	fputs("\t-C count (autocomment noncommented pictures)\n", stderr);
	fputs("\t-D day [1,...,31]\n", stderr);
	fputs("\t-F filmnooffset [0,...,9999]\n", stderr);
	fputs("\t-M month [Jan,...,Dec]\n", stderr);
	fputs("\t-V (print version)\n", stderr);
	fputs("\t-Y year [1980,...,2079]\n", stderr);
	fputs("\t-a aperature [1.0,...,32]\n", stderr);
	fputs("\t-c comment\n", stderr);
	fputs("\t-f argfile\n", stderr);
	fputs("\t-h headermsg\n", stderr);
	fputs("\t-i iso [6,...,6400]\n", stderr);
	fputs("\t-l focal length [0.5,...,9999]\n", stderr);
	fputs("\t-m metering [#,*,.,[]]\n", stderr);
	fputs("\t-n negativoffset [-7,...,+8]\n", stderr);
	fputs("\t-p program [P,TV,AV,DEP,M]\n", stderr);
	fputs("\t-s flash [OFF,MAN,ATTL,TTL,ON]\n", stderr);
	fputs("\t-t (time of exposure off/on)\n", stderr);
	fputs("\t-v (display aperature/time-values off/on)\n", stderr);
	fputs("\t-z (assume zoom lens on/off)\n", stderr);
	exit(0);
} /* gausage */

static void gisoacorr(iso, corr, str)
char iso[], corr[], *str;
{
	register char *pc;

	for (pc = str; *pc && *pc!='+' && *pc!='-'; ++pc)
		;
	if (*pc) {
		strncpy(corr, pc, CORRLEN+1);	/* where do we know how long corr is ? */
		*pc = '\0';
	}
	if (pc>str)
		strncpy(iso, str, ISOLEN+1);	/* where do we know how long iso is ? */
} /* gisoacorr */

static char *fl2ap(mm)
char *mm;
{
	switch (atoi(mm)) {
		case   0: return("???");
		case   7:			/* Canon 5.6/7.5mm Fisheye */
		case 300:			/* Sigma 4.5-5.6/75-300mm */
		case 400: return(f5_6);		/* Sigma 5.6/400mm */
		case  17:			/* Vivitar 3.5/17mm */
		case  35: return("3.5");	/* Olympus AF-1 and AF-1 Twin */
		case  15:			/* Sigma 2.8/15mm Fisheye */
		case  16:			/* Zenita 2.8/16mm Fisheye */
		case  24:			/* Vivitar 2.8/24mm */
		case  28:			/* Tokina 2.8/28-70mm */
		case  80:			/* Tokina 2.8/80-200mm */
		case 200: return("2.8");	/* Tokina 2.8/80-200mm */
		case  50: return("1.4");	/* Canon FD 1.4/50mm */
		case  85: return("1.2");	/* Canon FD 1.2/85mm */
		case 135: return("2.0");	/* Canon FD 2.0/135mm */
		case  75: return("4.5");	/* Sigma 4.5-5.6/75-300mm */
		case  70: return("6.7");	/* Olympus AF-1 Twin */
		case   8:			/* Sigma 4.0/8mm Fisheye */
		case  78:			/* Soligor 3.5/78-210mm */
		case 210: return("4.0");	/* Soligor 3.5/78-210mm */
	}
	return("----");
} /* fl2ap */

#define CPOPTARG(op, oa) if StrCmp(oa, ==, optarg) \
				fprintf(stderr, sastr, argv0, op, optarg); \
			strncpy(oa, optarg, sizeof oa)

int main(argc, argv)
int argc;
char *argv[];
{
	static char sastr[] = "%s: warning: %s set again to \"%s\"\n";
	char avl[AVLLEN+1], comment[256], prg[PRGLEN+1], met[METLEN+1], mm[MMLLEN+1], iso[ISOLEN+1], flash[FLLEN+1], corr[CORRLEN+1];
	unsigned int fnooff = 0, day = 1, mon = 1, year = 1980;
	int acomment = 0, aa = 0, negoff = 0, lower = 1, upper = 36, zoom = 0, notime = 0, novalues = 0;
	register int neg;

	argv0 = argv[0];

	strcpy(prg,   "AV");
	strcpy(met,   "*");
	strcpy(mm,    "24");
	strcpy(avl, fl2ap(mm));
	strcpy(iso,   "400");
	strcpy(flash, "OFF");
	corr[0] = '\0';

	opterr = 1;	/* enable messages */
	optind = 0;	/* must do this because of "++optind..." */

	while (++optind<argc) {
		register int c;

		memset(comment, '\0', sizeof comment);
		while ((c = getopt(argc, argv, "0AC:D:F:M:VY:a:c:f:h:i:l:m:n:p:s:tvz")) != EOF) {
			switch (c) {
				case '0' :
					lower = 0;
					break;
				case 'A' :
					aa = aa ? 0 : FRNOAA;
					break;
				case 'C' :
					acomment = atoi(optarg);
					break;
				case 'D' :
					day = atoi(optarg);
					if (--day  >= 31)
						gausage();
					++day;
					break;
				case 'F' :
					fnooff = atoi(optarg);
					lower = 1;	/* reset if new film */
					upper = 36;
					negoff = 0;
					break;
				case 'M' :
					if (!(mon = StrToMon(optarg)))
						mon = atoi(optarg);
					if (--mon  >= 12)
						gausage();
					++mon;
					break;
				case 'V' :
					printf("%s: %s %s\n", argv0, __TIME__, __DATE__);
					exit(0);
				case 'Y' :
					year = atoi(optarg);
					if (year < 1980)
						gausage();
					break;
				case 'a' :
					CPOPTARG("aperature of lens", avl);
					break;
				case 'c' :
					CPOPTARG("comment", comment);
					break;
				case 'f' :
					--optind;
					if (SetArgv(&argc, &argv, optarg, optind, optind))
						continue;
					++optind;
					break;
				case 'h' :
					printf("* %s\n", optarg);
					break;
				case 'i' :
					gisoacorr(iso, corr, optarg);
					break;
				case 'l' :
					CPOPTARG("length of lens", mm);
					strcpy(avl, fl2ap(mm));
					break;
				case 'm' :
					CPOPTARG("metering", met);
					break;
				case 'n' :
					negoff = atoi(optarg);
					break;
				case 'p' :
					CPOPTARG("program", prg);
					break;
				case 's' :
					CPOPTARG("flash function", flash);
					break;
				case 't' :
					notime ^= 1;
					break;
				case 'v' :
					novalues ^= 1;
					break;
				case 'z' :
					zoom ^= 1;
					break;
				default  :
					gausage();
			}
		}
		if (optind<argc) {
			upper = atoi(argv[optind]);
		}
		for (neg=lower; neg<=upper; ++neg) {
			static char mmmmmm[] = "------";
			FRAME fb;

			memset((char *) &fb, '\0', sizeof fb);
			fb.f_no     = fnooff;
			fb.fr_no    = neg+negoff+aa;
			fb.av       = StrToAv (filblanks(mmmmmm, RA, AVLEN ));
			fb.tv       = StrToTv (filblanks(mmmmmm, RA, TVLEN ));
			fb.metering = StrToMet(filblanks(met,    LA, METLEN));
			fb.iso      = StrToIso(filblanks(iso,    RA, ISOLEN));
			fb.correct  = StrToCor(filblanks(corr,   LA, CORRLEN));
			fb.flash    = StrToFla(filblanks(flash,  LA, FLLEN ));
			fb.ae_mode  = StrToAem(filblanks(prg,    LA, PRGLEN));
			fb.lens_av  = StrToAv (filblanks(avl,    RA, AVLLEN));
			fb.mm       = StrToMm (filblanks(mm,     RA, MMLLEN));
			if (zoom) {
				fb.lens_mm1 = StrToMm(filblanks( "80", RA, MMLLEN));
				fb.lens_mm2 = StrToMm(filblanks("200", RA, MMLLEN));
			}
			if (!novalues && StrCmp(flash, ==, "ATTL") && StrCmp(prg, ==, "P")) {
				fb.av       = StrToAv (filblanks(f5_6, RA, AVLEN ));
				fb.tv       = StrToTv (filblanks( "60", RA, TVLEN ));
			}
			if (!*comment && acomment)
				sprintf(comment, "Bild %04d", acomment);
			strncpy(fb.note, comment, sizeof(fb.note));
			memset(comment, '\0', sizeof comment);
			if (acomment)
				++acomment;
			if (notime)
				fb.btime.hour = fb.btime.min = NOTIME;
			fb.btime.sec = NOTIME;
			fb.p_day = day;
			fb.p_month = mon;
			fb.p_year = year;
			DispFrame(&fb, notime, 0, novalues);
		}
		lower = upper + 1;
	}
	return(0);
} /* main */

