/* (C) Copyright by PaN Super Sonic Software Service 1990, 1993, 2006	*/
/*                  expert for hi tec & low budget			*/

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#ifndef	__DATE__
#define	__DATE__	"Tue 17 Jan 2006"
#endif

#ifndef	__TIME__
#define	__TIME__	"18:10"
#endif
#ifdef __TIMESTAMP__
#define ___TS___ " " __TIMESTAMP__
#else
#define ___TS___ ""
#endif

void epatch_version()
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
} /* epatch_version */

#ifndef O_BINARY
#define	O_BINARY	0
#endif

#define GAP 192

extern char *strerror(), *optarg, *getenv(), **SetArgv();
extern int errno, opterr, optind;

char *argv0;

static int opdf(fn)
char *fn;
{
	static char _dat[] = ".dat";
	register int ifufd;
	char buf[256];

	strcpy(buf, fn);
	if (strcmp(buf+strlen(buf)-4, _dat))
		strcat(buf, _dat);
	if ((ifufd = open(buf, (O_RDWR|O_BINARY))) < 0)
		fprintf(stderr, "%s: cannot open \"%s\": %s\n", argv0, buf, strerror(errno));
	return(ifufd);
} /* opdf */

static int edel(fn)
char *fn;
{
	register int ifufd;
	char buf[GAP + 4];

	if ((ifufd = opdf(fn)) >= 0) {
		memset(buf, '\0', sizeof buf);
		lseek(ifufd, 2L, 0);
		write(ifufd, buf, GAP);
		close(ifufd);
	}
	return(errno);
} /* edel */

static int epatch(apnd, aa, argv)
int apnd;
char aa, *argv[];
{
	register int len, ifufd;
	register char *p, *pbuf;
	char buf[GAP + 4];

	errno = 0;
	if ((ifufd = opdf(*argv)) >= 0) {
		memset(buf, '\0', sizeof buf);
		if (apnd) {
			lseek(ifufd, 2L, 0);
			read(ifufd, buf, sizeof buf);
			buf[GAP-1] = '\0';
			if (buf[2])
				strcat(buf, "\n");
		}
		buf[0] = 0x12;
		buf[1] = aa;
		len = 2 + strlen(buf+2);
		pbuf = buf + len;
		while (p = *++argv) {
			while ((*pbuf = *p++) && (++len <= GAP-2))
				++pbuf;
			if (len < GAP-2 && argv[1]) {
				*pbuf++='\n';
				++len;
			}
		}
		buf[GAP-1] = '\0';
		if (len > 2) {
			lseek(ifufd, 2L, 0);
			write(ifufd, buf, GAP);
		}
		close(ifufd);
	}
	return(errno);
} /* epatch */

int main(argc, argv)
int argc;
char *argv[];
{
	register int c, apnd = 0;
	char aa = 0x34;

	argv0 = argv[0];

	opterr = 0;
	optind = 1;	/* No need to do this */

	while ((c = getopt(argc, argv, "AVadf:h?")) != EOF) {
		switch (c) {
			case 'A' :
				aa = 'A';
				break;
			case 'V' :
				epatch_version();
				setargv_version();
				exit(0);
			case 'a' :
				apnd ^= 1;
				break;
			case 'd' :
				exit(edel(optarg));
				break;
			case 'f' :
				--optind;
				if (SetArgv(&argc, &argv, optarg, optind, optind))
					continue;
				++optind;
				break;
			default :
				fprintf(stderr, "Usage: %s [-A] [-V] [-a|-d] [-f argfile]|[fn args..]\n", argv0);
				fputs("\twhere options are:\n", stderr);
				fputs("\t-A (append 'A' to each negnumber)\n", stderr);
				fputs("\t-V (print version)\n", stderr);
				fputs("\t-a (append)\n", stderr);
				fputs("\t-d (delete)\n", stderr);
				fputs("\t-f argfile\n", stderr);
				exit(0);
		}
	}
	return(epatch(apnd, aa, argv + optind));
} /* main */

