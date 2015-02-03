/* (C) Copyright by PaN Super Sonic Software Service 1991,1992,1993,1994,1995,1998,2002,2006	*/
/*                  expert for hi tech & low budget						*/

#include <stdio.h>
#ifdef _WIN32
#include <stdlib.h>
#include <string.h>
#endif /* _WIN32 */

#include "rifu.h"
#include "valstr.h"

static unsigned int numpics;

#ifndef	__DATE__
#define	__DATE__	"Tue 17 Jan 2006"
#endif

#ifndef	__TIME__
#define	__TIME__	"17:23"
#endif
#ifdef __TIMESTAMP__
#define ___TS___ " " __TIMESTAMP__
#else
#define ___TS___ ""
#endif

void prtfVersion()
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
} /* prtfVersion */

static void frmni(char *frm)
{
	fprintf(stderr, "%s format is not compiled into this binary.\n", frm);
	exit(22);
} /* frmni */

#ifdef EPSONSQ2550

/* =================== EPSON SQ-2550 =================== */

#define	ESC	'\033'

static char psb[] = "%s ";

static void p1(int c)
{
	fputc(c, stdout);
} /* p1 */

static void p3(int a, b, c)
{
	int i=3;

	while (--i>=0) {
		p1(a);
		p1(b);
		p1(c);
	}
} /* p3 */

static void defCharProlog(int c)
{
	p1(ESC);
	p1('&');
	p1(0);
	p1(c);
	p1(c);
	p1(2);
	p1(3*11);
	p1(2);
} /* defCharProlog */

static void usrCharOnOff(on_off)
int on_off;
{
	p1(ESC);
	p1('%');
	p1(!!on_off);
} /* usrCharOnOff */

static void dWidOnOff(on_off)
int on_off;
{
	p1(ESC);
	p1('W');
	p1(!!on_off);
} /* dWidOnOff */

static void dHiOnOff(on_off)
int on_off;
{
	p1(ESC);
	p1('w');
	p1(!!on_off);
} /* dHiOnOff */

static int userCharsDefined;

static void defUsrChrs()
{
	++userCharsDefined;
	defCharProlog('#');
	p3(0377, 0377, 0377);
	p3(0340, 0016, 0007);
	p3(0340, 0016, 0007);
	p3(0340, 0176, 0007);
	p3(0343, 0201, 0307);
	p3(0377, 0201, 0377);
	p3(0343, 0201, 0307);
	p3(0340, 0176, 0007);
	p3(0340, 0016, 0007);
	p3(0340, 0016, 0007);
	p3(0377, 0377, 0377);

	defCharProlog('[');	/* ] */
	p3(0377, 0377, 0377);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0377, 0377, 0377);

	defCharProlog('*');
	p3(0377, 0377, 0377);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0176, 0007);
	p3(0343, 0201, 0307);
	p3(0343, 0201, 0307);
	p3(0343, 0201, 0307);
	p3(0340, 0176, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0377, 0377, 0377);

	defCharProlog('.');
	p3(0377, 0377, 0377);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0036, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0377, 0377, 0377);

	defCharProlog(' ');
	p3(0000, 0000, 0000);
	p3(0000, 0000, 0000);
	p3(0000, 0000, 0000);
	p3(0000, 0000, 0000);
	p3(0000, 0000, 0000);
	p3(0000, 0000, 0000);
	p3(0000, 0000, 0000);
	p3(0000, 0000, 0000);
	p3(0000, 0000, 0000);
	p3(0000, 0000, 0000);
	p3(0000, 0000, 0000);
} /* defUsrChrs */

static void printMetering(char *metbuf)
{
	if (userCharsDefined) {
		dWidOnOff(1);
		usrCharOnOff(1);
		p1(*metbuf);
		dWidOnOff(0);
		usrCharOnOff(0);
	} else fputs(metbuf, stdout);
	fputc(' ', stdout);
} /* printMetering */

void PrtFrameE(pfb, notime, nosecs, novalues, bn)
FRAME *pfb;
int notime, nosecs, novalues;
char *bn;
{
	char buf[256];

	puts(bn);
	DateStr(buf, pfb->p_day, pfb->p_month, pfb->p_year);
	printf("       %s", buf);
	if (!notime) {
		TimToStr(buf, pfb->btime, nosecs);
		printf(" %s", buf);
	}
	fputs("\n       ", stdout);

	memset(buf, ' ', sizeof buf);

	if (!novalues) {
		TvToStr  (buf, pfb->tv);
		fputs(buf, stdout);
		TouToStr (buf, pfb->tv_our);
		printf(psb, buf);

		AvToStr  (buf, pfb->av);
		fputs(buf, stdout);
		AouToStr (buf, pfb->av_our);
		printf(psb, buf);
	}

	AemToStr (buf, pfb->ae_mode);
	printf(psb, buf);

	MetToStr(buf, pfb->metering);
	printMetering(buf);

	AebToStr(buf, pfb->aeb);
	puts(buf);

	memset(buf, ' ', sizeof buf);
	FrnoToStr(buf, pfb->fr_no);
	dHiOnOff(1);
	dWidOnOff(1);
	fputs(buf, stdout);
	dWidOnOff(0);
	dHiOnOff(0);

	IsoToStr(buf, pfb->iso);
	printf(" ISO%s", buf);
	CorToStr(buf, pfb->correct);
	printf(psb, buf);
	FlaToStr(buf, pfb->flash&~0x02);
	fputs(buf, stdout);
	CurToStr(buf, pfb->flash&0x02);
	puts(buf);
	if (pfb->lens_av || pfb->mm || pfb->lens_mm1) {
		memset(buf, ' ', sizeof buf);
		AvToStr(buf+7, pfb->lens_av);
		fputs(buf, stdout);
		MmToStr(buf, pfb->mm);
		printf("/%s ", buf);
	}
	if (pfb->lens_mm1) {
		MmToStr(buf, pfb->lens_mm1);
		fputs(buf, stdout);
		MmToStr(buf, pfb->lens_mm2);
		printf("-%s", buf);
	}
	printf("\n%s\n\n\n\n", pfb->note);
} /* PrtFrameE */

void PrtFrameG(pfb, notime, nosecs, novalues, bn)
FRAME *pfb;
int notime, nosecs, novalues;
char *bn;
{
	if (!userCharsDefined)
		defUsrChrs();
	PrtFrameE(pfb, notime, nosecs, novalues, bn);
} /* PrtFrameG */

#else

static void notImplementedEpson()
{
	frmni("EPSON SQ-2550");
} /* notImplementedEpson */

void PrtFrameE()
{
	notImplementedEpson();
} /* PrtFrameE */

void PrtFrameG()
{
	notImplementedEpson();
} /* PrtFrameG */

#endif /* EPSONSQ2550 */

#ifdef INTERLEAF5

/* =================== Interleaf5 =================== */

static int pOP = 16;		/* pictures on a page */
static char
	Rowrow[] = "<Row, \"row\">",
	fpn0[] = "0",
	fpn0_20[] = "0.20",
	fpn0_0266667[] = "0.0266667",
	fpn0_0270003[] = "0.0270003";

static char *ascii2i5(register unsigned char *str)
{
	static char unsigned buf[256];
	register unsigned char c, *pb = buf;

	while (*pb = c = *str++) {
		if (c == '<')
			*++pb = c;
		if ((c >= ' ') && (c <= '~') || c == '\n') {
			++pb;
		} else {
			sprintf(pb, "<#%02x>", c);
			pb += 5;
		}
	}
	return((char *) buf);
} /* ascii2i5 */

static void prHidden(endc)
char endc;
{
	printf("	Hidden =		yes%c\n", endc);
} /* prHidden */

static void prMargin(char *tb, *inch, endc)
{
	printf("	%s Margin =		%s Inches%c\n", tb, inch, endc);
} /* prMargin */

static void prTopMargin(char *inch)
{
	prMargin("Top", inch, ',');
} /* prTopMargin */

static void prBottomMargin(inch, endc)
char *inch, endc;
{
	prMargin("Bottom", inch, endc);
} /* prBottomMargin */

static void prMasterRow(inch21)
char *inch21;
{
	puts("<Master Row, \"row\",");
	if (pOP == 21) {
		prTopMargin(inch21);
		prBottomMargin(inch21, '>');
	} else {
		prTopMargin(fpn0);
		prBottomMargin(fpn0, ',');
		puts("	@\"<<+->\" =		\"+\",");
		puts("	@\"<<E>\" =		\"Base-U2\">\n");
	}
} /* prMasterRow */

static void prPageFrame(headfoot)
char *headfoot;
{
	printf("<Page %s, Frame =\n", headfoot);
	puts("V11,");
	puts("(g9,1,0,");
	puts(" (t14,1,0,,0,0.4299406,0,7,0,0,,wst:timsps10,)");
	puts(" (t14,2,0,,3.1496061,0.4299406,1,7,0,0,,wst:timsps10,)");
	puts(" (t14,3,0,,6.2992122,0.4299406,2,7,0,0,,wst:timsps10,))>");
} /* prPageFrame */

static void prI5Header()
{
	puts("<!OPS, Version = 8.0>");

	puts("<!Page Number Stream,");
	puts(" 	Name =			\"page\",");
	puts("	Starting Page # = 	Inherit>");

	puts("<!Document,");
	puts("	Header Page =		no,");
	puts("	Final Output Device =	\"ileaf\",");
	puts("	Default Printer =	\"nearest-ileaf\",");
	puts("	Measurement Unit =	MM,");
	puts("	Default Page Stream Name = \"page\">");

	puts("<!Font Definitions,");
	if (pOP == 21)
		puts("	F3 = Courier 9 Bold,");
	puts("	F4 = Courier 12 Bold,");
	puts("	F6 = Courier 24 Bold>");

	puts("<!Page,");
	printf("	Columns =		%d,\n", (pOP == 21) ? 3 : 2);
	printf("	Gutter =		0.1660441 Inches,\n", (pOP == 21) ? "0.1660441" : "0.3320882");
	puts("	Height =		11.6929134 Inches,");
	puts("	Width =			8.2677165 Inches,");

	if (pOP == 21) {
		prTopMargin("0.30");
		prBottomMargin(fpn0_20, ',');
		prMargin("Left", fpn0_20, ',');
		prMargin("Right", fpn0_20, ',');
	} else {
		prTopMargin(fpn0_20);
		prBottomMargin(fpn0_20, ',');
		prMargin("Left", "0.51", ',');
		prMargin("Right", "0.5118107", ',');
	}

	puts("	Hyphenation =		on,");
	puts("	Consecutive Hyphens =	3,");
	puts("	Revision Bar Placement = Left,");
	puts("	Feathering =		off>");

	puts("<!Class, \"1x1:cell\",");
	prTopMargin(fpn0_0266667);
	prBottomMargin(fpn0_0266667, ',');
	puts("	Alignment =		Center,");

	printf("	Font =			F%d@Z7@Lam,\n", (pOP == 21) ? 3 : 4);

	puts("	Line Spacing =		1.114 lines,");
	puts("	Left Tab =		0/0.5*3 Inches,");
	puts("	Composition =		Optimum>");

	printf("<!Class, \"%s:cell\",\n", (pOP == 21) ? "4x1" : "1x2");

	prTopMargin(fpn0_0266667);
	prBottomMargin(fpn0_0266667, ',');

	printf("	Alignment =		%s,\n", (pOP == 21) ? "Center" : "Left");

	puts("	Font =			F4@Z7@Lam,");
	puts("	Line Spacing =		1.114 lines,");
	puts("	Left Tab =		0/0.5*3 Inches,");
	puts("	Composition =		Optimum>");

	puts("<!Master Frame,");
	puts("	Name =			\"floating\",");
	puts("	Placement =		Following Anchor,");
	puts("	Horizontal Alignment =	Center,");
	puts("	Width =			0 Inches,");
	puts("	Width =			Page Without Margins,");
	puts("	Height =		0 Inches,");
	puts("	Height =		Page Without Margins * 0.33,");
	puts("	Diagram =");
	puts("V11,");
	puts("(g9,0,0,)>");

	puts("<!Master Table, \"1x1\",");
	puts("	Columns =		1,");
	prTopMargin(fpn0);
	prBottomMargin(fpn0, ',');	/* Not with pOP == 21 */
	puts("	Column 1 Width =	1 units,");
	puts("	Row =			\"row\">");

	prMasterRow(fpn0_0270003);

	puts("<Cell><\"1x1:cell\",");
	prHidden('>');

	puts("<End Table>");

	if (pOP == 21) {
		puts("<!Master Table, \"4x1\",");
		puts("	Columns =		1,");
		prBottomMargin(fpn0, ',');
		puts("	Column 1 Width =	1 units,");
		puts("	Row =			\"row\"");
		puts("	Row =			\"row\"");
		puts("	Row =			\"row\"");
		puts("	Row =			\"row\">");

		puts("<Master Row, \"row\",");
		prTopMargin(fpn0_0270003);
		prBottomMargin(fpn0_0270003, '>');

		puts("<Cell><\"4x1:cell\",");
	} else {
		puts("<!Master Table, \"1x2\",");
		puts("	Columns =		2,");
		puts("	Column 1 Width =	0.83 Inches,");
		puts("	Column 2 Width =	1 units,");
		puts("	Row =			\"row\">");

		puts("<Master Row, \"row\",");
		prTopMargin(fpn0);
		prBottomMargin(fpn0, ',');
		puts("	@\"<<+->\" =		\"+\">");

		puts("<Cell><\"1x2:cell\",");
		prHidden('>');

		puts("<Cell><\"1x2:cell\",");
	}

	prHidden('>');
	puts("<End Table>");

	prPageFrame("Header");
	prPageFrame("Footer");

	puts("<!End Declarations>");
} /* prI5Header */

void PrtFrameI5(pfb, notime, nosecs, novalues, bn)
FRAME *pfb;
int notime, nosecs, novalues;
char *bn;
{
	char buf[256];

	if (!numpics++)
		prI5Header();

	if (pOP == 21) {
		puts("<Table, \"4x1\",");
		puts("	Columns = 1,");
	} else {
		puts("<Table, \"1x1\",");
		switch (pOP) {
			case 14: prTopMargin("0.7000000"); break;
			case 16: prTopMargin("0.4500000"); break;
			case 18: prTopMargin("0.2500000"); break;
			default: fprintf(stderr, "%s: Invalid pOP:%d\n", "PrtFrameI5", pOP);
				 exit(22);
		}
	}

	if ((numpics > pOP) && ((numpics % pOP) == 1))
		puts("Begin New Page =		yes,");
	if (pOP != 21)
		puts("	Bottom Border Visible =	no,");
	puts("	Column 1 Width =	1 units>\n");

	prMasterRow("0.033");

	printf("<Cell><\"%dx1:cell\",\n", (pOP == 21) ? 4 : 1);
	prHidden('>');

	puts(Rowrow);

	printf("<Cell><\"%dx1:cell\">\n\n", (pOP == 21) ? 4 : 1);

	DateStr(buf, pfb->p_day, pfb->p_month, pfb->p_year);
	fputs(ascii2i5(buf), stdout);

	if (pOP == 21) {
		puts(Rowrow);
		puts("<Cell,");
		puts("	Top Ruling Visible =    no><\"4x1:cell\">");
	}

	if (!notime) {
		TimToStr(buf, pfb->btime, nosecs);
		if (pOP == 21) {
			fputs(buf[0] == ' ' ? buf+1 : buf, stdout);
		} else {
			printf("   %s", buf);
		}
	}

	if (pOP == 21) {
		puts("\n");
		puts(Rowrow);
		puts("<Cell><\"4x1:cell\">");
		printf("\n%s\n", bn);
	} else {
		puts("\n<End Table>\n");

		puts("<Table, \"1x2\",");
		prTopMargin(fpn0);
		prBottomMargin(fpn0, '>');

		puts("<Master Row, \"row\",");
		prTopMargin(fpn0);
		prBottomMargin(fpn0, ',');
		puts("	@\"<<+->\" =		\"+\">");

		puts("<Cell, Vertical Alignment = Center><\"1x2:cell\",");
		prHidden(',');
		puts("	Alignment =		Center>");

		puts("<Cell><\"1x2:cell\",");
		prHidden('>');

		puts(Rowrow);
		puts("<Cell, Vertical Alignment = Center><\"1x2:cell\",");
		puts("	Alignment =		Center>");

		printf("%s<HR>\n", bn);
	}
	memset(buf, ' ', sizeof buf);
	FrnoToStr(buf, pfb->fr_no);
	if (buf[2] == ' ')
		buf[2] = '\0';
	if (buf[0] == ' ')
		strcpy(buf, buf+1);
	if (pOP == 21) {
		puts(Rowrow);
		puts("<Cell,");
		puts("	Top Ruling Visible =    no><\"4x1:cell\",");
		puts("	Font = F6@Z7@Lam>");
		puts(buf);
	} else {
		printf("<F6@Z7@Lam>%s\n", buf);

		puts("<Cell><\"1x2:cell\">");

		memset(buf, ' ', sizeof buf);

		if (!novalues) {
			TvToStr  (buf, pfb->tv);
			fputs(buf, stdout);
			TouToStr (buf, pfb->tv_our);
			printf(psb, buf);

			AvToStr  (buf, pfb->av);
			fputs(buf, stdout);
			AouToStr (buf, pfb->av_our);
			printf(psb, buf);
		}

		AemToStr (buf, pfb->ae_mode);
		printf(psb, buf);

		MetToStr(buf, pfb->metering);
		printf(psb, buf);

		AebToStr(buf, pfb->aeb);
		printf("%s<HR>\n", buf);

		memset(buf, ' ', sizeof buf);
		IsoToStr(buf, pfb->iso);
		printf("ISO%s", buf);
		CorToStr(buf, pfb->correct);
		printf(psb, buf);
		FlaToStr(buf, pfb->flash&~0x02);
		fputs(buf, stdout);
		CurToStr(buf, pfb->flash&0x02);
		printf("%s<HR>\n", buf);

		if (pfb->lens_av || pfb->mm || pfb->lens_mm1) {
			memset(buf, ' ', sizeof buf);
			AvToStr(buf, pfb->lens_av);
			fputs(buf, stdout);
			MmToStr(buf, pfb->mm);
			printf("/%s ", buf);
		}
		if (pfb->lens_mm1) {
			MmToStr(buf, pfb->lens_mm1);
			fputs(buf, stdout);
			MmToStr(buf, pfb->lens_mm2);
			printf("-%s", buf);
		}
	}

	puts("<End Table>\n");


	puts("<Table, \"1x1\",");
	if (pfb->note[0] == '\0') {
		puts("Bottom Border Visible =	no,");
		puts("Left Border Visible =	no,");
		puts("Right Border Visible =	no,");
	}
	puts("	Top Border Visible =	no>");

	prMasterRow(fpn0_0270003);

	puts("<Cell><\"1x1:cell\",");
	prHidden('>');

	puts(Rowrow);
	puts("<Cell><\"1x1:cell\">");

	puts(ascii2i5(pfb->note));

	puts("\n<End Table>\n");
} /* PrtFrameI5 */

static void prtAtExitI5()
{
	FRAME fb;

	memset((char *) &fb, '\0', sizeof fb);
	fb.fr_no = 88 + FRNOAA;
	fb.p_year = 1961;
	fb.p_month = 4;
	fb.p_day = 13;
	strcpy(fb.note, "Comment");
	while (numpics % pOP)
		PrtFrameI5(&fb, 1, 1, 1, "FILLER");
} /* prtAtExitI5 */

void PrtSetPgFmtI5(fmt)
char *fmt;
{
	static char *fmts[] = {
			"7x2", "2x7", "14",
			"8x2", "2x8", "16",
			"9x2", "2x9", "18",
			"7x3", "3x7", "21",
			NULL
	};
	register char **pf;

	for (pf = fmts ; *pf && StrCmp(*pf, !=, fmt); ++pf)
		;
	pOP = 16;
	if (*pf) {
		pOP = atoi(fmts[((pf - fmts) / 3) * 3 + 2]);
	}
#ifdef DEBUG
	fprintf(stderr, "PrtSetPgFmtI5(\"%s\"): %d\n", fmt, pOP);
#endif /* DEBUG */
} /* PrtSetPgFmtI5 */

#else
static void notImplementedI5()
{
	frmni("Interleaf5");
} /* notImplementedI5 */

void PrtFrameI5()
{
	notImplementedI5();
} /* PrtFrameI5 */

void prtAtExitI5()
{
	notImplementedI5();
} /* prtAtExitI5 */

void PrtSetPgFmtI5()
{
	notImplementedI5();
} /* PrtSetPgFmtI5 */

#endif /* INTERLEAF5 */

#ifdef RTF

/* =================== RTF Rich TExt Format =================== */

static void trowd(int trrh)
{
	printf("\\trowd \\trgaph70\\trrh%d\\trleft-284\\trbrdrt\n", trrh);
} /* trowd */

static void prRHeader()
{
	puts("{\\rtf1\\ansi\\ansicpg1252\\uc1 \\deff6\\deflang1033\\deflangfe1031");
	puts("{\\fonttbl");
	puts("{\\f6\\fmodern\\fcharset0\\fprq1{\\*\\panose 00000000000000000000}Courier;}");
	puts("}");
	puts("{\\stylesheet{\\widctlpar\\adjustright \\fs20\\lang1031\\cgrid \\snext0 Normal;}");
	puts("{\\s1\\keepn\\widctlpar\\outlinelevel0\\adjustright \\fs40\\lang1031");
	puts("\\cgrid \\sbasedon0 \\snext0 heading 1;}");
	puts("{\\*\\cs10 \\additive Default Paragraph Font;}");
	puts("{\\s15\\widctlpar\\tqc\\tx4536\\tqr\\tx9072\\adjustright \\fs20\\lang1031");
	puts("\\cgrid \\sbasedon0 \\snext15 header;}");
	puts("{\\s16\\widctlpar\\tqc\\tx4536\\tqr\\tx9072\\adjustright \\fs20\\lang1031");
	puts("\\cgrid \\sbasedon0 \\snext16 footer;}");
	puts("}");
	puts("{\\info");
	puts("{\\title RIFU.OUT}");
	puts("{\\author Pawel Nabe}");
	puts("{\\creatim\\yr1998\\mo2\\dy11\\hr17\\min50}");
	puts("{\\version1}");
	puts("{\\edmins1}");
	puts("{\\*\\company PaN Super Sonic Software Service}");
	puts("}");
	puts("\\paperw11906\\paperh16838\\margl1134\\margr1134\\margt1134\\margb1021");
	puts("\\deftab708\\widowctrl\\ftnbj\\aenddoc\\hyphhotz425\\hyphcaps0\\formshade");
	puts("\\viewkind1\\viewscale100\\pgbrdrhead\\pgbrdrfoot \\fet0\\sectd \\linex0");
	puts("\\headery709\\footery709");
	puts("\\colsx709\\endnhere\\sectdefaultcl {\\*\\pnseclvl1\\pnucrm\\pnstart1");
	puts("\\pnindent720\\pnhang{\\pntxta .}}");
	puts("{\\*\\pnseclvl2\\pnucltr\\pnstart1\\pnindent720\\pnhang{\\pntxta .}}");
	puts("{\\*\\pnseclvl3\\pndec\\pnstart1\\pnindent720\\pnhang{\\pntxta .}}");
	puts("{\\*\\pnseclvl4\\pnlcltr\\pnstart1\\pnindent720\\pnhang{\\pntxta )}}");
	puts("{\\*\\pnseclvl5\\pndec\\pnstart1\\pnindent720\\pnhang{\\pntxtb (}{\\pntxta )}}");
	puts("{\\*\\pnseclvl6\\pnlcltr\\pnstart1\\pnindent720\\pnhang{\\pntxtb (}{\\pntxta )}}");
	puts("{\\*\\pnseclvl7\\pnlcrm\\pnstart1\\pnindent720\\pnhang{\\pntxtb (}{\\pntxta )}}");
	puts("{\\*\\pnseclvl8\\pnlcltr\\pnstart1\\pnindent720\\pnhang{\\pntxtb (}{\\pntxta )}}");
	puts("{\\*\\pnseclvl9\\pnlcrm\\pnstart1\\pnindent720\\pnhang{\\pntxtb (}{\\pntxta )}}");
	/* puts("\\trowd \\trgaph70\\trrh480\\trleft-284\\trbrdrt"); */
	trowd(480);
	puts("\\brdrs\\brdrw30 \\trbrdrl");
	puts("\\brdrs\\brdrw30 \\trbrdrb");
	puts("\\brdrs\\brdrw30 \\trbrdrr");
	puts("\\brdrs\\brdrw30 \\trbrdrh");
	puts("\\brdrs\\brdrw30 \\trbrdrv");
	puts("\\brdrs\\brdrw30 \\clvertalt\\clbrdrt");
	puts("\\brdrs\\brdrw30 \\clbrdrl");
	puts("\\brdrs\\brdrw30 \\clbrdrb");
	puts("\\brdrs\\brdrw30 \\clbrdrr");
	puts("\\brdrs\\brdrw30 \\cltxlrtb \\cellx2941\\clvertalt\\clbrdrl");
	puts("\\brdrs\\brdrw30 \\clbrdrr");
	puts("\\brdrs\\brdrw30 \\cltxlrtb \\cellx3242\\clvertalt\\clbrdrt");
	puts("\\brdrs\\brdrw30 \\clbrdrl");
	puts("\\brdrs\\brdrw30 \\clbrdrb");
	puts("\\brdrs\\brdrw30 \\clbrdrr");
	puts("\\brdrs\\brdrw30 \\cltxlrtb \\cellx6468\\clvertalt\\clbrdrl");
	puts("\\brdrs\\brdrw30 \\clbrdrr");
	puts("\\brdrs\\brdrw30 \\cltxlrtb \\cellx6770\\clvertalt\\clbrdrt");
	puts("\\brdrs\\brdrw30 \\clbrdrl");
	puts("\\brdrs\\brdrw30 \\clbrdrb");
	puts("\\brdrs\\brdrw30 \\clbrdrr");
	puts("\\brdrs\\brdrw30 \\cltxlrtb");
	puts("\\cellx9996\\pard\\plain \\s15\\qc\\widctlpar");
	puts("\\intbl\\adjustright \\fs20\\lang1031");
	puts("\\cgrid {\\b\\f6 Mon 11 HHH 8888");
	puts("\\par 88:88:88\\cell \\cell Di 12 BBB 2222");
	puts("\\par 22:22:22\\cell \\cell Mit 13 3333");
	puts("\\par 33:33:33\\cell }");
	puts("\\pard\\plain \\widctlpar");
	puts("\\intbl\\adjustright \\fs20\\lang1031\\cgrid {\\f6 \\row }");
	/* puts("\\trowd \\trgaph70\\trrh605\\trleft-284\\trbrdrt"); */
	trowd(605);
	puts("\\brdrs\\brdrw30 \\trbrdrl");
	puts("\\brdrs\\brdrw30 \\trbrdrb");
	puts("\\brdrs\\brdrw30 \\trbrdrr");
	puts("\\brdrs\\brdrw30 \\trbrdrh");
	puts("\\brdrs\\brdrw30 \\trbrdrv");
	puts("\\brdrs\\brdrw30 \\clvertalt\\clbrdrt");
	puts("\\brdrs\\brdrw30 \\clbrdrl");
	puts("\\brdrs\\brdrw30 \\clbrdrb");
	puts("\\brdrs\\brdrw30 \\clbrdrr");
	puts("\\brdrs\\brdrw30 \\cltxlrtb \\cellx2941\\clvertalt\\clbrdrl");
	puts("\\brdrs\\brdrw30 \\clbrdrr");
	puts("\\brdrs\\brdrw30 \\cltxlrtb \\cellx3242\\clvertalt\\clbrdrt");
	puts("\\brdrs\\brdrw30 \\clbrdrl");
	puts("\\brdrs\\brdrw30 \\clbrdrb");
	puts("\\brdrs\\brdrw30 \\clbrdrr");
	puts("\\brdrs\\brdrw30 \\cltxlrtb \\cellx6468\\clvertalt\\clbrdrl");
	puts("\\brdrs\\brdrw30 \\clbrdrr");
	puts("\\brdrs\\brdrw30 \\cltxlrtb \\cellx6770\\clvertalt\\clbrdrt");
	puts("\\brdrs\\brdrw30 \\clbrdrl");
	puts("\\brdrs\\brdrw30 \\clbrdrb");
	puts("\\brdrs\\brdrw30 \\clbrdrr");
	puts("\\brdrs\\brdrw30 \\cltxlrtb \\cellx9996");
	puts("\\pard\\plain \\s15\\qc\\widctlpar");
	puts("\\intbl\\adjustright \\fs20\\lang1031");
	puts("\\cgrid {\\b\\f6 E608888\\par }");
	puts("{\\b\\f6\\fs40 11A\\cell }{\\b\\f6 \\cell E6222222\\par }");
	puts("{\\b\\f6\\fs40 12A\\cell }{\\b\\f6 \\cell E333333\\par }");
	puts("{\\b\\f6\\fs40 13A\\cell }");
	puts("\\pard\\plain \\widctlpar");
	puts("\\intbl\\adjustright \\fs20\\lang1031\\cgrid {\\f6 \\row }");
	/* puts("\\trowd \\trgaph70\\trrh156\\trleft-284\\trbrdrt"); */
	trowd(156);
	puts("\\brdrs\\brdrw30 \\trbrdrl");
	puts("\\brdrs\\brdrw30 \\trbrdrb");
	puts("\\brdrs\\brdrw30 \\trbrdrr");
	puts("\\brdrs\\brdrw30 \\trbrdrh");
	puts("\\brdrs\\brdrw30 \\trbrdrv");
	puts("\\brdrs\\brdrw30 \\clvertalt\\clbrdrt");
	puts("\\brdrs\\brdrw30 \\clbrdrl");
	puts("\\brdrs\\brdrw30 \\clbrdrb");
	puts("\\brdrs\\brdrw30 \\clbrdrr");
	puts("\\brdrs\\brdrw30 \\cltxlrtb \\cellx2941\\clvertalt\\clbrdrl");
	puts("\\brdrs\\brdrw30 \\clbrdrr");
	puts("\\brdrs\\brdrw30 \\cltxlrtb \\cellx3242\\clvertalt\\clbrdrt");
	puts("\\brdrs\\brdrw30 \\clbrdrl");
	puts("\\brdrs\\brdrw30 \\clbrdrb");
	puts("\\brdrs\\brdrw30 \\clbrdrr");
	puts("\\brdrs\\brdrw30 \\cltxlrtb \\cellx6468\\clvertalt\\clbrdrl");
	puts("\\brdrs\\brdrw30 \\clbrdrr");
	puts("\\brdrs\\brdrw30 \\cltxlrtb \\cellx6770\\clvertalt\\clbrdrt");
	puts("\\brdrs\\brdrw30 \\clbrdrl");
	puts("\\brdrs\\brdrw30 \\clbrdrb");
	puts("\\brdrs\\brdrw30 \\clbrdrr");
	puts("\\brdrs\\brdrw30 \\cltxlrtb \\cellx9996");
	puts("\\pard\\plain \\s15\\qc\\widctlpar");
	puts("\\intbl\\adjustright \\fs20\\lang1031");
	puts("\\cgrid {\\b\\f6\\fs16 123456789012345678901234567890");
	puts("\\cell \\cell 123456789012345678901234567890");
	puts("\\cell \\cell 123456789012345678901234567890\\cell }");
	puts("\\pard\\plain\\widctlpar");
	puts("\\intbl\\adjustright \\fs20\\lang1031\\cgrid {\\f6\\fs16 \\row }");
	puts("\\pard\\plain \\s15\\widctlpar\\adjustright \\fs20\\lang1031\\cgrid {\\f6 \\par }");
	/* } */
} /* prRHeader */

static void PrtFrameR(pfb, notime, nosecs, novalues, bn)
FRAME *pfb;
int notime, nosecs, novalues;
char *bn;
{
	if (!numpics++)
		prRHeader();

} /* PrtFrameR */

static void prtAtExitR()
{
	/* { */
	puts("}");
} /* prtAtExitR */

#else
static void notImplementedRTF()
{
	frmni("RTF");
} /* notImplementedRTF */

static void PrtFrameR()
{
	notImplementedRTF();
} /* PrtFrameR */

static void prtAtExitR()
{
	notImplementedRTF();
} /* prtAtExitR */

#endif /* RTF */

/* =================== AVERY =================== */

static void PrtFrameA(pfb, notime, nosecs, novalues, bn)
FRAME *pfb;
int notime, nosecs, novalues;
char *bn;
{
	char date[32], tim[16], frno[8];

	if (!numpics++)
		fputs("DATE|TIME|FILMNAME|FRAMENUM|COMMENT", stdout);
	DateStr(date, pfb->p_day, pfb->p_month, pfb->p_year);
	trdtou(date);
	if (notime) 
		memset(tim, '\0', sizeof tim);
	else {
		TimToStr(tim, pfb->btime, nosecs);
	}
	FrnoToStr(frno, pfb->fr_no);
	trdtou(pfb->note);
	printf("\n%s|%s|%s|%s|%s", date, tim, bn, frno, pfb->note);
} /* PrtFrameA */

static void nullFunc()
{
} /* nullFunc */

PRTFUNC PrtFrameEFunc = {
		nullFunc,
		PrtFrameE,
		nullFunc,
		nullFunc,
	}, PrtFrameGFunc = {
		nullFunc,
		PrtFrameG,
		nullFunc,
		nullFunc,
	}, PrtFrameIFunc = {
		nullFunc,
		PrtFrameI5,
		nullFunc,
		prtAtExitI5,
	}, PrtFrameRFunc = {
		nullFunc,
		PrtFrameR,
		nullFunc,
		prtAtExitR,
	}, PrtFrameAFunc = {
		nullFunc,
		PrtFrameA,
		nullFunc,
		nullFunc,
	};

