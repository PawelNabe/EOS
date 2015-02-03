/* (C) Copyright by PaN Super Sonic Software Service 2002, 2003	*/
/*                  expert for hi tec & low budget		*/

#include <stdio.h>
#include "rifu.h"

extern FILE *FreOpen(fn, mode, oldfp);

#define	ff(x)	((x)&0xff)

typedef struct {
	int val;
	char *str;
} valstr;

char *argv0;

#ifndef	__DATE__
#define	__DATE__	"Thu 04. Apr 2002"
#endif

#ifndef	__TIME__
#define	__TIME__	"16:13"
#endif

static void prtWhatln(tab, buf)
int tab;
char *buf;
{
	int len;

	while (buf[0] == '\n')
		++buf;
	if (buf[0]=='@' && buf[1]=='(' && buf[2]=='#' && buf[3]==')')
		buf+=4;
	len = strlen(buf);
	if (tab)
		putchar('\t');
	if (buf[len-1] == '\n')
		fputs(buf, stdout);
		else puts(buf);
} /* prtWhatln */

static void Version()
{
	static char crstr0[] = "@(#)rifut90 V2.1";
	static char crstr1[] = "@(#)(C) Copyright by PaN Super Sonic Software Service 2002, 2003";
	static char crstr2[] = "@(#)    expert for hi tech & low budget";
	static char crstr3[] = "@(#)    email: Pawel.Nabe@web.de";

	prtWhatln(0, crstr0);
	prtWhatln(0, crstr1);
	prtWhatln(0, crstr2);
	prtWhatln(0, crstr3);

#ifdef __VERSION__
	{
		static char versionstr[] =
			"\n"
			__VERSION__
			"\n";

		prtWhatln(0, versionstr);
	}
#endif /* __VERSION__ */
	prtWhatln(0,
		"\n@"
		"(#)"
		__FILE__
		"\tcompiled "
		__DATE__
		" "
		__TIME__
		"\n"
	);
	exit(0);
} /* Version */

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
	{ 0x00, "PRG" },
	{ 0x01, "P  " },
	{ 0x02, "TV " },
	{ 0x03, "AV " },
};

STRTOVALFUNC(StrToAem, aemvals)
VALTOSTRFUNC(AemToStr, aemvals)

static valstr flavals[] = {
	{ 0x00, "    " },
	{ 0x01, "FL1 " },
	{ 0x02, "MAN." },
	{ 0x03, "ATTL" },
	{ 0x04, "CATS" },
	{ 0x05, "TTL " },
	{ 0x06, "FL6 " },
	{ 0x07, "FEL " },
};

STRTOVALFUNC(StrToFla, flavals)
VALTOSTRFUNC(FlaToStr, flavals)

static valstr avvals[] = {
	 { 0x00, "SDx" },
	 { 0x01, "SD1" },
	 { 0x02, "SD2" },
	 { 0x03, "1.2" },
	 { 0x04, "1.4" },
	 { 0x05, "1.8" },
	 { 0x06, "2.0" },
	 { 0x07, "2.5" },
	 { 0x08, "2.8" },
	 { 0x09, "3.5" },
	 { 0x0A, "4.0" },
	 { 0x0B, "4.5" },
	 { 0x0C, "5.6" },
	 { 0x0D, "6.7" },
	 { 0x0E, "8.0" },
	 { 0x0F, "9.5" },
	 { 0x10, " 11" },
	 { 0x11, " 13" },
	 { 0x12, " 16" },
	 { 0x13, " 19" },
	 { 0x14, " 22" },
	 { 0x15, " 27" },
	 { 0x16, " 32" },
};

STRTOVALFUNC(StrToAv, avvals)
VALTOSTRFUNC(AvToStr, avvals)

static valstr tvvals[] = {
	{ 0x00, "????" },
	{ 0x03, "bulb" },
	{ 0x04, " 30\"" },
	{ 0x05, " 20\"" },
	{ 0x06, " 15\"" },
	{ 0x07, " 10\"" },
	{ 0x08, "  8\"" },
	{ 0x09, "  6\"" },
	{ 0x0A, "  4\"" },
	{ 0x0B, "  3\"" },
	{ 0x0C, "  2\"" },
	{ 0x0D, " 1\"5" },
	{ 0x0E, "  1\"" },
	{ 0x0F, " 0\"7" },
	{ 0x10, "   2" },
	{ 0x11, "   3" },
	{ 0x12, "   4" },
	{ 0x13, "   6" },
	{ 0x14, "   8" },
	{ 0x15, "  10" },
	{ 0x16, "  15" },
	{ 0x17, "  20" },
	{ 0x18, "  30" },
	{ 0x19, "  45" },
	{ 0x1a, "  60" },
	{ 0x1b, "  90" },
	{ 0x1c, " 125" },
	{ 0x1d, " 180" },
	{ 0x1e, " 250" },
	{ 0x1f, " 350" },
	{ 0x20, " 500" },
	{ 0x21, " 750" },
	{ 0x22, "1000" },
	{ 0x23, "1500" },
	{ 0x24, "2000" },
	{ 0x25, "3000" },
	{ 0x26, "4000" },
	{ 0x2f, "----" },
};

static int expvalue4(av, tv)
char *av, *tv;
{
	static char outbuf[16];
	char inbuf[8];
	valstr *pvalstr;
	int sum = -1;

#ifdef EXPVALDEBUG
	printf("expvalue4(\"%s\", \"%s\") ", av, tv);
#endif /* EXPVALDEBUG */
	memset(outbuf, '\0', sizeof outbuf);

	memset(inbuf, ' ', sizeof inbuf);
	strcpy(inbuf+3-strlen(av), av);
	for (pvalstr = avvals; pvalstr < avvals+NUMELEMS(avvals); ++pvalstr) {
		if (StrCmp(inbuf, ==, pvalstr->str)) {
			sum = (pvalstr - avvals) - 2;
#ifdef EXPVALDEBUG
			printf("avvals[%s]='%s' sum=%d ", av, pvalstr->str, sum);
#endif /* EXPVALDEBUG */
			break;
		}
	}

	memset(inbuf, ' ', sizeof inbuf);
	strcpy(inbuf+4-strlen(tv), tv);
	for (pvalstr = tvvals; pvalstr < tvvals+NUMELEMS(tvvals); ++pvalstr) {
		if (StrCmp(inbuf, ==, pvalstr->str)) {
#ifdef EXPVALDEBUG
			printf("tvvals[%s]='%s' tvsum:%d ", tv, pvalstr->str, (pvalstr - tvvals) - 12);
#endif /* EXPVALDEBUG */
			sum += (pvalstr - tvvals) - 12;
			break;
		}
	}
#ifdef EXPVALDEBUG
	sprintf(outbuf, "%3d ", sum*2);
	outbuf[4] = ' ';
	if (sum<0) {
		outbuf[4] = '-';
		sum = -sum;
	}
	sprintf(outbuf+5, "%d.%c", sum/2, sum%2 ? '5' : '0');
	puts(outbuf);
#endif /* EXPVALDEBUG */

	return(sum*2);
} /* expvalue4 */

STRTOVALFUNC(StrToTv, tvvals)
VALTOSTRFUNC(TvToStr, tvvals)

static valstr isovals[] = {
	{ 0x00, "????" },

	{ 0x10, "   6" },
	{ 0x12, "   8" },
	{ 0x14, "  10" },

	{ 0x18, "  12" },
	{ 0x1a, "  16" },
	{ 0x1c, "  20" },

	{ 0x20, "  25" },
	{ 0x22, "  32" },
	{ 0x24, "  40" },

	{ 0x28, "  50" },
	{ 0x2a, "  64" },
	{ 0x2c, "  80" },

	{ 0x30, " 100" },
	{ 0x32, " 125" },
	{ 0x34, " 160" },

	{ 0x38, " 200" },
	{ 0x3a, " 250" },
	{ 0x3c, " 320" },

	{ 0x40, " 400" },
	{ 0x42, " 500" },
	{ 0x44, " 640" },

	{ 0x48, " 800" },
	{ 0x4a, "1000" },
	{ 0x4c, "1250" },

	{ 0x50, "1600" },
	{ 0x52, "2000" },
	{ 0x54, "2500" },

	{ 0x58, "3200" },
	{ 0x5a, "4000" },
	{ 0x5c, "5000" },

	{ 0x60, "6400" },
};

STRTOVALFUNC(StrToIso, isovals)
VALTOSTRFUNC(IsoToStr, isovals)

static valstr corvals[] = {
	{ 0x0a, "-2.0" },
	{ 0x0b, "-5/3" },
	{ 0x0c, "-4/3" },
	{ 0x0d, "-1.0" },
	{ 0x0e, "-2/3" },
	{ 0x0f, "-1/3" },
	{ 0x00, "-0.0" },
	{ 0x00, " 0.0" },
	{ 0x00, "+0.0" },
	{ 0x00, "----" },
	{ 0x00, "    " },
	{ 0x01, "+1/3" },
	{ 0x02, "+2/3" },
	{ 0x03, "+1.0" },
	{ 0x04, "+4/3" },
	{ 0x05, "+5/3" },
	{ 0x06, "+2.0" },
};

STRTOVALFUNC(StrToCor, corvals)
VALTOSTRFUNC(CorToStr, corvals)

static valstr metvals[] = {
	{ 0x00, "  " }, /* Hack for short format */
	{ 0x01, "[]" }, /* AVER  */
	{ 0x02, "* " }, /* PART */
	{ 0x03, ". " }, /* SPOT */
};

STRTOVALFUNC(StrToMet, metvals)
VALTOSTRFUNC(MetToStr, metvals)

static char User1[] = "User1\0\0", User2[] = "User2\0\0", User3[] = "User3\0\0", User4[] = "User4\0\0";

static valstr lensvals[] = { 
	{  0, "--\0\0\0\0\0" },
	{  1, "28\0\0\0\0\0" },
	{  2, "50\0\0\0\0\0" },
	{  3, "135\0\0\0\0" },
	{  4, "35-70\0\0" },
	{  5, "35-105\0" },
	{  6, "75-200\0" },
	{  7, "100-300" },
	{  8, User1 },
	{  9, User2 },
	{ 10, User3 },
	{ 11, User4 },
};

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

void TimToStr(buf, bt)
char buf[];
bcdtime bt;
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

static char *months[] = { "???", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dez" };
static char *lenses[] = { "3.5/  17", "2.8/  24", "1.4/  50", "1.2/  85", "35-70", "35-105", "2.8/80-200", "100-300", "4.0/   8", "2.8/  15", "3.5/  17", "User4", "L13", "L14", "L15", "L16" };

static char lensNibble2Byte(nib)
int nib;
{
	switch (nib &= 0x0F) {
		case 0x0e:
			return(' ');
		case 0x0c:
			return('-');
		case 0x00:
		case 0x01:
		case 0x02:
		case 0x03:
		case 0x04:
		case 0x05:
		case 0x06:
		case 0x07:
		case 0x08:
		case 0x09:
			return('0' + nib);
		default:
			fprintf(stderr, "Invalid lens byte %d\n", nib);
	}
	return('\0');
} /* lensNibble2Byte */

static void setLensData(dest, src)
char *dest, src[];
{
	register int i;
	register char *pdest = dest;

	for (i = 3; i >= 0; --i) {
		*pdest++ = lensNibble2Byte(src[i] >> 4);
		*pdest++ = i ? lensNibble2Byte(src[i]     ) : '\0';
	}
	for (pdest -= 2; (pdest >= dest) && (pdest[0] == ' '); --pdest)
			pdest[0] = '\0';
} /* setLensData */

static char *quarter2str(hexval, zero)
char hexval, zero;
{
	static char *fractions[] = { "0 ", "25", "5 ", "75" };
	static char outbuf[32];
	int val;

	val = ff(hexval) - ff(zero);
	sprintf(outbuf, "%s%d.%s", (val<0) && ((val/4) == 0) ? "-" : "", val/4, fractions[(val<0?-val:val)%4]);
	return(outbuf);
} /* quarter2str */

static char *spotval(hexbyte, avstr, tvstr)
char hexbyte, *avstr, *tvstr;
{
	return(quarter2str((ff(hexbyte)/2) - expvalue4(avstr, tvstr), 32));
} /* spotval */

int main(argc, argv)
int argc;
char *argv[];
{
	register int c;
	char buf[2*1024+32], lensbuf[4*4];
	register char *pbuf;
	int raw = 0, oldday = -1, oldmon = -1, oldyear = -1;

	argv0 = argv[0];

	if (argc<=1 || (argv[1] && (StrCmp(argv[1], ==, "-?") || StrCmp(argv[1], ==, "-h")))) {
		fprintf(stderr, "Usage: %s [-R] [-V] <filenames>\n", argv0);
		fputs("Options:\n", stderr);
		fputs("\t-R\tRaw output.\n", stderr);
		fputs("\t-V\tPrint version information and exit.\n", stderr);
		fputs("Examples:\n", stderr);
		fprintf(stderr, "\t%s -R t902054.rwd\n", argv0);
		return(0);
	}
	if (argv[1] && StrCmp(argv[1], ==, "-V"))
		Version();

	if (argv[1] && StrCmp(argv[1], ==, "-R")) {
		raw = 1;
		++argv;
	}

#ifdef LENSDATA
LENS DATA: e2 ee ee 8e e0 ee ee 15 e0 ee ee 17 ee ee ee 
LENS DATA: e2 ee ee 8e e0 ee ee 15 e0 ee ee 17 e0 00 c2 
LENS DATA: e2 ee ee 10 e0 ee ee 20 e0 ee ee 30 e0 ee ee 
LENS DATA: e2 ee ee 8e e0 ee ee 15 e0 ee ee 17 e0 ee 5e 
LENS DATA: 72 56 34 12 40 3e 2e 1e 40 3c 2c 1c c0 56 3e 
LENS DATA: e2 ee ee 8e e0 ee ee 15 e0 ee ee 17 e0 5e 7e 
LENS DATA: e2 ee ee 8e e0 ee ee 15 e0 ee ee 17 ee ee ee 
LENS DATA: e2 ee ee 8e e0 ee ee 15 e0 ee ee 17 ee ee ee 
LENS DATA: e2 ee ee 8e e0 ee ee 15 e0 ee ee 17 ee ee ee 
LENS DATA: e2 ee ee 8e e0 ee ee 15 e0 ee ee 17 ee ee ee 
#endif /* LENSDATA */

	while (*++argv) {
		int oldfrno = 99;

		if (!FreOpen(*argv, "rb", stdin))
			continue;

		for (pbuf = lensbuf; pbuf < (lensbuf+sizeof(lensbuf)); ++pbuf) {
			*pbuf = (char) getchar();
		}

		setLensData(lensvals[ 8].str, lensbuf+ 0);
		setLensData(lensvals[ 9].str, lensbuf+ 4);
		setLensData(lensvals[10].str, lensbuf+ 8);
		setLensData(lensvals[11].str, lensbuf+12);

		printf("LENS DATA of \"%s\":\n", *argv);
		{
			register int i;
		
			for (i = 0; i < NUMELEMS(lensvals); ++i) {
				printf("\t%2d:'%s'\n", i, lensvals[i].str);
			}
		}
	
		puts("");

		for (pbuf = buf+sizeof(buf)-1; (c = getchar()) != EOF; --pbuf)
			*pbuf = (char) c;

		++pbuf;
		/* printf("Begin %02x\n", (*pbuf) & 0xff); */
		if (StrnCmp("DNE", !=, pbuf+1, 3)) {
			puts("END not found");
			exit(1);
		}
		pbuf+=4;
		if ((*pbuf & 0xff) == 0xdd) {
			puts("* Has already been dumped before");
			*pbuf = 0;
		}
		while ((pbuf[0] & 0xff) != 0xdd) {
			int frno, tv, av, negno, lens, mon, year, day, hour, min, fmode, pmode, met, isocomp;
			char tvbuf[8], avbuf[8], fbuf[8], prgbuf[8], metbuf[8], isocompbuf[8], isobuf[8];

			frno = BcdToI((pbuf[0]>>1)&0x7f);
			tv = (pbuf[1]>>2)&0x3f;
			av = (pbuf[2]>>2)&0x1f;

			negno = BcdToI(pbuf[3])*100 + BcdToI(pbuf[4]);

			lens = (pbuf[5]>>4)&0x0f;

			mon = pbuf[5]&0x0f;
			year = BcdToI(pbuf[6]);
			if (year>=85)
				year += 1900;
				else year += 2000;
			day = BcdToI(pbuf[7]);

			hour = BcdToI(pbuf[8]);
			min = BcdToI(pbuf[9]);

			isocompbuf[0] = '\0';
			if (pbuf[10] & 0x80) {
				isocomp = (pbuf[12]>>4)&0x0f;
				CorToStr(isocompbuf, isocomp);
			}
			IsoToStr(isobuf, pbuf[10]&0x7e);

			fmode = (pbuf[11]>>5) & 0x07;
			FlaToStr(fbuf, fmode);

			pmode = (pbuf[11]>>3) & 0x03;
			AemToStr(prgbuf, pmode);

			TvToStr(tvbuf, ff(tv));
			AvToStr(avbuf, ff(av));
			switch (pbuf[2]&0x81) {
				case 0x01:
					avbuf[2]='A';
					break;
				case 0x80:
					strncpy(avbuf, "M  ", 3);
					break;
				case 0x81:
					avbuf[2]='M';
					break;
			}

			met = (pbuf[11]>>1)&0x03;
			MetToStr(metbuf, met);

/*
[34 ]    .  ATTL     250  4.0  AV       100     13:51    2.8/  24mm             FEL DATA:+/- 0 EVR:- 3 3/4
[15A]:01 [] ATTL->> 4000* 1.2* DEP AEB 6400+3.5 13:04:05 1.2/1234mm 1234-2345mm
Test the Pest,ÄÖÜ8901234567890
*/
			if (!raw && ((oldday != day) || (oldmon != mon) || (oldyear != year))) {
				char dbuf[32];

				DateStr(dbuf, day, mon, year);
				printf("\n%4d ----------------- %s -----------------\n", negno/36+1, dbuf);
			}
			oldday = day;
			oldmon = mon;
			oldyear = year;

			if (raw) {
				static char *rawmets[] = { "    ", "AVER", "PART", "SPOT" };

				if (frno < oldfrno) {
					fputs("\n\nNO.  FR   TV  AV             LENS        DATE  TIME  ISO  +/- AE-  FLASH METER", stdout);
					fputs(  "\n                                                              MODE       -ING",  stdout);
				}
				printf("\n%04d %2d %s %s %s %s %7smm %2d %s %4d %2d:%02d %s %-4s %s   %s%c %s      AM:%c ",
					negno, frno, tvbuf, avbuf, pbuf[2]&0x80 ? "MAN" : "ASD", pbuf[2]&0x01 ? "SD" : "AO", lensvals[lens].str,
					day, months[mon], year, hour, min,
					isobuf, isocompbuf, prgbuf, fbuf, pbuf[11]&0x01 ? 'M' : ' ',
					rawmets[met], pbuf[11]&0x01 ? 'M' : 'A', expvalue4(avbuf, tvbuf));
			} else {
				printf("[%2d ]    %s %s    %s  %s  %s     %s%4s %2d:%02d    %smm\n",
					frno, metbuf, fbuf, tvbuf, avbuf, prgbuf, isobuf, isocompbuf, hour, min, lenses[lens]);
			}
			oldfrno = frno;

			pbuf += 11;
			if (isocompbuf[0])
				++pbuf;;
			if (StrCmp(fbuf, ==, "FEL ")) {
				printf("FEL:%s ", quarter2str(pbuf[2], 0x14));
				printf("EVR:%s\n", spotval(pbuf[1], avbuf, tvbuf));
				pbuf+=2;
			}
			{
				char *sep = "SPOT DATA:";

				while (ff(*++pbuf) != 0xFF) {
					printf("%s %s", sep, spotval(pbuf[0], avbuf, tvbuf));
					sep = "";
				}
				if (!*sep)
					putchar('\n');
			}
			++pbuf;
			fflush(stdout);
		}

#ifdef TESTQ2S
		{
			int spotval;

			for (spotval=30; spotval<=190; spotval+=2) {
				printf("%3d: %02x %s\n", spotval, spotval, quarter2str(spotval/2, 0x74/2));
			}
			putchar('\n');
		}
#endif /* TESTQ2S */

#ifdef TESTEXPVALUE
		{
			register valstr *pavvalstr, *ptvvalstr;

			for (pavvalstr = avvals+3; pavvalstr < avvals+NUMELEMS(avvals); ++pavvalstr) {
				for (ptvvalstr = tvvals+2; ptvvalstr < tvvals+NUMELEMS(tvvals); ++ptvvalstr) {
					expvalue4(pavvalstr->str, ptvvalstr->str);
					puts("--------------------------------------------------");
				}
			}
		}
#endif /* TESTEXPVALUE */

		puts("");
	}

	return(0);
} /* main */

#ifdef COMMENT

-----------------------------------------------------------
DMB Data Format
-----------------------------------------------------------

data is read backwards starting from byte 2064
0xDD means "End of Data", previous data is not
deleted and can still be accessed. After battery
change, memory is initialised with 0xAA and 0x55
patterns.

-------------------
Record in Long Mode
-------------------

Record length (13 bytes + Exp.Comp + 2(FEL) + N * SpotValue)

Byte	76543210	Value
------------------------------------------------------------
0	FFFffff?	Frame	FFF * 10 + ffff * 1	

1	TTTTTT??	Tv	TTTTTT

				000011	bulb				
				000100	30"
				000101	20"
				...
				100101	3000
				100110	4000
				
2	mAAAAA?s	Av	AAAAA

				00000	stopped down
				00011	1.2
				00100	1.4
				...
				10101	27
				10110	32
		
				m	0 Auto/ stopped down
					1 Manual
				
				s	0 aperture open
					1 stopped down
										
3	NNNNnnnn	Number	NNNN * 1000 + nnnn * 100

4	NNNNnnnn	"	+ NNNN * 10 + nnnn * 1

5	LLLLMMMM	Lens	LLLL
				0	--
				1	28
				2	50
				3	135
				4	35-70
				5	35-105
				6	75-200
				7	100-300
				8	User1
				9	User2
				10	User3
				11	User4
			
			Month	MMMM				

6	YYYYyyyy	Year	YYYY * 10 + yyyy
			
7	DDDDdddd	Date	DDDD * 10 + dddd

8	HHHHhhhh	Hour	HHHH * 10 + hhhh

9	MMMMmmmm	Minute	MMMM * 10 + mmmm

10	EIIIIII?	ISO	IIIIII

				001000	6
				001001	8
				001010	10
				001011	?	(combinations with 
				001100	12	...011 and ...111 
				...		are not used)
				101110	5000
				101111	?	(see above)
				110000	6400
		
				E	0  	no compensation
					1	with exp. comp
						( Byte 12 has exp.
						comp. value)
										
11	FFFPPMMm	Mode	FFF	000	No Flash
					010	Manual Flash
					011	A-TTL
					101	TTL
					111	FEL (two more bytes follow)
					
				PP	00	Program
					01	P
					10	Tv
					11	Av

				MM	01	[   ] Average
					10	[( )] Partial
					11	[ o ] Spot (Values follow) 
				
				m	0	Auto
					1	Manual			

(12)	EEEE????	Exp. Comp.	E	Exp. Comp. Value
	    0100				
	    				A	-2
					B	-1 2/3
					C	-1 1/3
					D	-1
					E	-  2/3
					F	-  1/3
						
					1	+  1/3
					2	+  2/3
					3	+1
					4	+1 1/3
					5	+1 2/3
					6	+2
					
(12..)	SSSSSSS?	Spot	Spot values: Mean of all spot
				values gives 0 value, diff to
				0 value is in 1/4 steps					

(12	EEEEEEEE	EVR	EVR value in FEL mode

 13)	FFFFFFFF	FEL	FEL correction

12	11111111	End of record


--------------------
Record in Short Mode
--------------------

Record length is fixed at 6 bytes

Byte	76543210	Value
--------------------------------------------------------------
0	FFFffff?	Frame	FFF * 10 + ffff * 1	

1	TTTTTT??	Tv	TTTTTT

				000011	bulb				
				000100	30"
				000101	20"
				...
				100101	3000
				100110	4000
				
2	mAAAAA?s	Av	AAAAA

				00000	stopped down
				00011	1.2
				00100	1.4
				...
				10101	27
				10110	32
		
				m	0 Auto/ stopped down
					1 Manual
				
				s	0 aperture open
					1 stopped down
										
3	NNNNnnnn	Number	NNNN * 1000 + nnnn * 100

4	NNNNnnnn	"	+ NNNN * 10 + nnnn * 1

5	11111111	End of Record

#endif /* COMMENT */

