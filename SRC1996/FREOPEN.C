/* (C) Copyright by PaN Super Sonic Software Service 1991	*/
/*                  expert for hi tec & low budget		*/

#include <stdio.h>

extern char *sys_errlist[], *strncpy();
extern int errno;

extern char *argv0;

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
			"...", sys_errlist[errno]);
	return(fp);
} /* FreOpen */

