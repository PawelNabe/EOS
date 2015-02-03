/* (C) Copyright by PaN Super Sonic Software Service 1991, 1993, 2006	*/
/*                  expert for hi tec & low budget			*/

#include <stdio.h>
#include <errno.h>

extern char *strerror(), *strncpy();
extern int errno;

extern char *argv0;

#ifndef	__DATE__
#define	__DATE__	"Tue 17 Jan 2006"
#endif

#ifndef	__TIME__
#define	__TIME__	"18:08"
#endif
#ifdef __TIMESTAMP__
#define ___TS___ " " __TIMESTAMP__
#else
#define ___TS___ ""
#endif

void freopen_version()
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
} /* freopen_version */

FILE *FreOpen(fn, mode, oldfp)
char *fn, *mode;
register FILE *oldfp;
{
	FILE *fp;

	if (!(fp = freopen(fn, mode, oldfp)))
		fprintf(stderr, "%s: cannot freopen(\"%s\", \"%s\", %s): %s\n",
			argv0, fn, mode,
			oldfp==stdin  ? "stdin"  :
			oldfp==stdout ? "stdout" :
#ifdef stdprn
			oldfp==stdprn ? "stdprn" :
#endif /* stdprn */
#ifdef stdaux
			oldfp==stdaux ? "stdaux" :
#endif /* stdaux */
			"...", strerror(errno));
	return(fp);
} /* FreOpen */

