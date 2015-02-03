/* (C) Copyright by PaN Super Sonic Software Service 1991	*/
/*                  expert for hi tec & low budget		*/

#include <stdio.h>

extern int errno;

int btobcd(a)
int a;
{
	return(((a / 10) << 4) | (a % 10));
} /* btobcd */

#define	INCPUTC(c) if (++c>=255) c=0; fputc(c, stdout)
int main(argc, argv)
int argc;
char *argv[];
{
	register int t = 0x00, av=0, met=0, iso=0, cor=0, film, frame, suff=0;
	unsigned short len, mm = 960*2;
	unsigned char buf[192+2], ch=128;

	if ((argc>1) && !freopen(argv[1], "wb", stdout))
		exit(errno);
	memset(buf, 'X', sizeof buf);
	fwrite(buf, 1, sizeof buf, stdout);

	fputc(0x00, stdout);	/* dateform */
	for (film=1; film <= 20; ++film) {
		for (frame = 0; frame <= 15; ++frame) {
			fputc(btobcd(frame), stdout);
			fputc(0xF0 | (film / 100), stdout);
			fputc(btobcd(film % 100), stdout);
			INCPUTC(t);
			INCPUTC(av);
			fputc(mm/0xFF, stdout);	/* mm */
			fputc(mm%0xFF, stdout);	/* mm */
			INCPUTC(met);	/* flash, metering, {av,tv}ovr */
			INCPUTC(iso);	/* iso */
			fputc(0x13, stdout);	/* year */
			fputc('\001', stdout);	/* month */
			fputc('\002', stdout);	/* day */
			fputc(ch, stdout);	/* hour */
			fputc(ch, stdout);	/* min */
			fputc(ch, stdout);	/* sec */
			fputc(ch, stdout);	/* lens aperature */
			/* ++suff; */
			suff = 0x02;
			/* suff &= ~0x04; */
			fputc(suff, stdout);
			if (suff & 0x20)
				INCPUTC(cor);	/* correction */
			if (suff & 0x10)
				fputc(btobcd((""[0]++ % 99) + 1), stdout);
			if (suff & 0x04) {
				fputc(ch, stdout);	/* mm */
				fputc(ch, stdout);	/* mm */
			}
			if (suff & 0x02) {
				/* printf("cor:0x%02x av:0x%02x %dmm", cor, av, mm/2); */
				printf("0x%02x %3d 0%03o %c", ch, ch, ch, ch);
				if ((++ch & 0xFF) >= 255)
					ch = 128;
			}
			mm += 20;
			fputc(0xFF, stdout);
		}
	}
	fputs("\377\377\377\377", stdout);
	len = ftell(stdout);
	len -= 195;
	rewind(stdout);
	fwrite((char *) &len, 2, 1, stdout);
	return(0);
} /* main */
