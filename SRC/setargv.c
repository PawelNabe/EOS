/* (C) Copyright by PaN Super Sonic Software Service 1991, 1993, 2006	*/
/*                  expert for hi tec & low budget			*/

#include <stdio.h>
#include <errno.h>

extern char *strerror(), *strcpy(), *malloc(), *realloc(), *calloc();
extern int errno;

char **SetArgv(pargc, pargv, fn, indfrom, indto)
int *pargc;
char ***pargv, *fn;
int indfrom, indto;
{
	register int srcind, dstind;
	register char **argv;
	register FILE *fp;

	argv = (char **) calloc(256, sizeof(char *));
	if (fp = fopen(fn, "r")) {
		char buf[BUFSIZ];

		for (srcind = dstind = 0; (srcind<indfrom) && (srcind<*pargc); ++dstind, ++srcind)
			argv[dstind] = (*pargv)[srcind];
		while (fgets(buf, sizeof buf, fp)) {
			if (buf[0] != '#') {	/* Comment */
				register int l;

				l = strlen(buf);
				if ((--l>=0) && (buf[l] == '\n')) {
					buf[l] = '\0';
				}
				argv[dstind++] = strcpy(malloc(l+2), buf);
			}
		}
		for (srcind = indto; ++srcind<*pargc; ++dstind)
			argv[dstind] = (*pargv)[srcind];
		fclose(fp);
		argv[*pargc = dstind] = NULL;
		return(*pargv = argv);
	} else fprintf(stderr, "%s: cannot open file \"%s\": %s\n", *pargv[0], fn, strerror(errno));
	return(NULL);
} /* SetArgv */

#ifndef	__DATE__
#define	__DATE__	"Tue 17 Jan 2006"
#endif

#ifndef	__TIME__
#define	__TIME__	"17:34"
#endif
#ifdef __TIMESTAMP__
#define ___TS___ " " __TIMESTAMP__
#else
#define ___TS___ ""
#endif

void setargv_version()
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
} /* setargv_version */

#ifdef MAIN

extern char *optarg;
extern int opterr, optind;

static char *argv0;

int main(argc, argv)
int argc;
char *argv[];
{
	argv0 = argv[0];

	opterr = 1;
	optind = 0;	/* must do this because of "++optind..." */

	while (++optind<argc) {
		register int c;

		while ((c = getopt(argc, argv, "AF:L:OPRS:Vf:jn:pr:stv")) != EOF) {
			switch (c) {
				case 'A' :
				case 'F' :
				case 'L' :
				case 'O' :
				case 'P' :
				case 'R' :
				case 'S' :
				case 'j' :
				case 'n' :
				case 'p' :
				case 'r' :
				case 's' :
				case 't' :
				case 'v' :
					printf("argc:%d optind:%d option:\"-%c\"\n", argc, optind, c);
					break;
				case 'V' :
					printf("%s: %s %s\n", argv0, __TIME__, __DATE__);
					setargv_version();
					return(0);
				case 'f' :
					--optind;
					if (SetArgv(&argc, &argv, optarg, optind, optind))
						continue;
					++optind;
					break;
			}
		}
		if (optind<argc) {
			printf("argc:%d optind:%d argv[optind]:\"%s\"\n", argc, optind, argv[optind]);
		}
	}
	SetArgv(&argc, &argv, "huhu", -1, -1);
	puts("------------------------------------------------------------------------------");
	while (--argc>=0)
		printf("\"%s\"\n", argv[argc]);
	puts("------------------------------------------------------------------------------");
	while (*argv) {
		printf("\"%s\"\n", *argv);
		++argv;
	}
	return(0);
} /* main */

#endif /* MAIN */

