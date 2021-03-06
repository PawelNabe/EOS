/* (C) Copyright by PaN Super Sonic Software Service 1990, 1993, 2006	*/
/*                  expert for hi tec & low budget			*/

#include <stdio.h>
/* #include <unistd.h> */
#include <errno.h>

#ifndef SEEK_SET
#define	SEEK_SET	0	/* Set file pointer to "offset" */
#define	SEEK_CUR	1	/* Set file pointer to current plus "offset" */
#define	SEEK_END	2	/* Set file pointer to EOF plus "offset" */
#endif /* SEEK_SET */

extern int errno;
extern char *strerror();

#ifndef	__DATE__
#define	__DATE__	"Tue 17 Jan 2006"
#endif

#ifndef	__TIME__
#define	__TIME__	"18:09"
#endif
#ifdef __TIMESTAMP__
#define ___TS___ " " __TIMESTAMP__
#else
#define ___TS___ ""
#endif

void ejoin_version()
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
} /* ejoin_version */

char *argv0;

int main(argc, argv)
int argc;
char *argv[];
{
	argv0 = argv[0];

	if (argc != 4) {
		fprintf(stderr, "Usage: %s f1.dat f2.dat dest.dat\n", argv[0]);
		ejoin_version();
		freopen_version();
	} else {
			FILE *fp1, *fp2;
			long len1, len2;
			short len;

			if (fp1 = fopen(argv[1], "rb")) {
				if (fp2 = fopen(argv[2], "rb")) {
					if (FreOpen(argv[3], "wb", stdout)) {
						fseek(fp1, 0L, SEEK_END);
						len1 = ftell(fp1);
						fseek(fp1, 2L, SEEK_SET);

						fseek(fp2, 0L, SEEK_END);
						len2 = ftell(fp2);
						len = len1 - 195 - 4 + len2 - 195;

						fprintf(stderr, "%s: len1:%ld len2:%ld len:%d\n", argv[0], len1, len2, len);
						fwrite((char *) &len, 2, 1, stdout); /* !!!! UNPORTABLE */
						for (len1 -= 2L; --len1 >= 4L; )
							fputc(fgetc(fp1), stdout);
						fseek(fp2, 195L, SEEK_SET);
						for (len2 -= 195L; --len2 >= 0L; )
							fputc(fgetc(fp2), stdout);
					}
				} else fprintf(stderr, "%s: cannot open \"%s\": %s\n", argv[0], argv[2], strerror(errno));
			} else fprintf(stderr, "%s: cannot open \"%s\": %s\n", argv[0], argv[1], strerror(errno));
		}
	return(0);
} /* main */

