/* (C) Copyright by PaN Super Sonic Software Service 1993	*/
/*                  expert for hi tec & low budget		*/

#include <stdio.h>

extern char *sys_errlist[], *optarg, *calloc();
extern int errno, opterr, optind;

#define NUMLINES	9
#define STRLEN		40
#define SWIDTH		38
#define ESC		'\033'

#ifndef __DATE__
#define	__DATE__ "11. Jul 1993"
#endif /* __DATE__ */

#ifndef __TIME__
#define	__TIME__ "01:02:03"
#endif /* __TIME__ */

typedef struct datline {
	char dline[STRLEN];
	int slen, width;
} datlines[NUMLINES];

static char *argv0;

int getline(pdl, fp)
struct datline *pdl;
FILE *fp;
{
	char *p;
	int c;

	memset(pdl->dline, '\0', sizeof pdl->dline);

	p = pdl->dline;
	while ((c = fgetc(fp)) != EOF && c!='\n') {
		*p++ = c;
	}
	*p = '\0';
	pdl->slen = p - pdl->dline;
	pdl->width = (pdl->dline[0] == ESC) ? (6 + strlen(pdl->dline+15)) : strlen(pdl->dline);

	return(!feof(fp));
} /* getline */

static void prline(pdl, len)
struct datline *pdl;
int len;
{
	int slen, width;

	slen = pdl->slen;
	width = pdl->width;
	fwrite(pdl->dline, 1, slen, stdout);
	if (len)
		for (len-=width; --len>=0; )
			fputc(' ', stdout);
	else fputc('\n', stdout);
} /* prline */

static void prdatbufs(datbufs, numcols, swidth)
int numcols, swidth;
datlines *datbufs;
{
	int ncol, line;

	for (line=0; line<NUMLINES; ++line)
		for (ncol=0; ncol<numcols; ++ncol)
			prline(datbufs[ncol]+line, ncol<numcols-1 ? swidth : 0);
} /* prdatbufs */

int main(argc, argv)
int argc;
char *argv[];
{
	int c, swidth = SWIDTH, ncol = 0, line = 0, numcols=2;
	datlines *datbufs;

	argv0 = argv[0];

	opterr = 0;
	while ((c = getopt(argc, argv, "Vb:c:w:hq?")) != EOF) {
		switch (c) {
			case 'V' :
				printf("%s: Version 1.0 %s %s\n", argv0, __TIME__, __DATE__);
				return(0);
			case 'b' :
				ncol = atoi(optarg)-1;
				break;
			case 'c' :
				if ((numcols = atoi(optarg)) <= 0) {
					fprintf(stderr, "%s: invalid number of columns \"%s\", must be >0.\n", argv0, optarg);
					return(0);
				}
				break;
			case 'w' :
				swidth = atoi(optarg);
				break;
			default  :
				fprintf(stderr, "Usage: %s [-b begincol] [-c numcols] [-w stickerwidth] [files..]\n", argv0);
				return(0);
		}
	}
	if (ncol<0 || ncol>=numcols) {
		fprintf(stderr, "%s: begincol must be within 1 and the number of columns (%d).\n", argv0, numcols);
		exit(0);
	}

	datbufs = (datlines *) calloc(numcols, sizeof *datbufs);

	argc -= optind;
	argv += optind;
	if (argc <= 0) {
		static char *margv[] = { "-", NULL };

		argv = margv;
		argc = 1;
	}
	while (*argv) {
		FILE *fp;

		if (fp = (strcmp(*argv, "-") ? fopen(*argv, "r") : stdin)) {
			while (getline(datbufs[ncol]+line, fp)) {
				if (++line>=NUMLINES) {
					if (++ncol>=numcols) {
						prdatbufs(datbufs, numcols, swidth);
						memset((char *) datbufs, '\0', numcols*sizeof datbufs[0]);
						ncol=0;
					}
					line=0;
				}
			}
			fclose(fp);
		} else fprintf(stderr, "%s: cannot open \"%s\": %s\n", argv0, *argv, sys_errlist[errno]);

		++argv;
	}
	if (line>0 || ncol>0)
		prdatbufs(datbufs, numcols, swidth);
	return(0);
} /* main */

