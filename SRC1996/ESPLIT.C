/* (C) Copyright by PaN Super Sonic Software Service 1991	*/
/*                  expert for hi tec & low budget		*/

#include <stdio.h>

extern int errno;

#define GAP	195

static char *argv0;

static int bcd(nr)
int nr;
{
	return(((nr & 0xF0) >> 4) * 10 + (nr & 0x0F));
} /* bcd */

static int btobcd(i)
{
	return(((i/10)<<4) | (i%10));
} /* btobcd */

int main(argc, argv)
int argc;
char *argv[];
{
	register int c, len, fno, frno = 0;
	register char *pbuf;
	char buf[4*1024], *fnform;
	int fnoffset;

	argv0 = argv[0];

	if (argc<=1 || !strcmp(argv[1], "-?")) {
		fprintf(stderr, "Usage: %s [[[infn] offset] format]\n", argv0);
		exit(0);
	}
	if ((argc>1) && !freopen(argv[1], "rb", stdin))
		exit(errno);
	fnoffset = argc > 2 ? atoi(argv[2]) : 0;
	fnform = argc > 3 ? argv[3] : "f%03d.dat";
	fread(buf, 1, GAP, stdin);
	memset(buf, '\0', GAP - 1);	/* leave datedescriptor */
	pbuf = buf + GAP;
	fno = 0;
	while ((c = getchar()) != EOF) {
		if (c == 0xFF || fno > c) {
			char datname[80];
			FILE *fp;

			strcpy(pbuf, "\377\377\377\377");
			len = (pbuf+4) - (buf+GAP);
			buf[0] = len & 0xFF;
			buf[1] = (len >> 8) & 0xFF;
			sprintf(datname, fnform, frno);
			fp = fopen(datname, "wb");
			fwrite(buf, 1, len+GAP, fp);
			fclose(fp);
			fp = NULL;
			pbuf = buf + GAP;
			if (c == 0xFF)
				return(0);
		}
		fno = c;
		*pbuf++ = c;	/* frame number */
		c = getchar();
		frno = (c & 0x0F) * 100;
		c = getchar();
		frno += bcd(c);

		frno += fnoffset;

		c = btobcd(frno/100) | 0xF0;
		*pbuf++ = c;
		c = btobcd(frno%100);

		do {
			*pbuf++ = c;
		} while ((c = getchar()) != 0xFF);
		*pbuf++ = 0xFF;
	}
	return(0);
} /* main */

