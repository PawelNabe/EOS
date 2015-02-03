/* (C) Copyright by PaN Super Sonic Software Service 1991,1992,1993,1994	*/
/*                  expert for hi tec & low budget				*/

#include <stdio.h>
#include <string.h>
#ifdef	__MSDOS__
#include <dir.h>
#include <dos.h>
#else
struct ffblk {
	char ff_name[256];
};
int _argc;
char **_argv;
int findfirst(pat, pffblk, i)
char *pat;
struct ffblk *pffblk;
int i;
{
	strcpy(pffblk->ff_name, pat);
	return(0);
} /* findfirst */
int findnext(pffblk)
struct ffblk *pffblk;
{
	static int cnt;
	char buf[512];

	sprintf(buf, "%s:%d", pffblk->ff_name, ++cnt);
	strcpy(pffblk->ff_name, buf);
	if (cnt > 1)
		cnt = 0;

	return(cnt);
} /* findnext */
#endif
#include "stent.h"

char *dirname(path)
char *path;
{
	static char dn[256];
	register char *p;

	strcpy(dn, path);
	for (p = dn; *p; ++p)
		;
	for (--p; (p >= dn) && (*p == '/'
#ifdef	__MSDOS__
					|| *p == '\\'
#endif
				); --p)
		;
	for (   ; (p >= dn) && (*p != '/'
#ifdef	__MSDOS__
					&& *p != '\\'
#endif
				); --p)
		;
	for (   ; (p >= dn) && (*p == '/'
#ifdef	__MSDOS__
					|| *p == '\\'
#endif
				); --p)
		*p = '\0';
	if (p < dn)
		dn[0] = '\0';
	/* fprintf(stderr, "dirname(\"%s\"):\"%s\"\n", path, dn); */
	return(dn);
} /* dirname */

static void apcstore(pent, name)
APEENTRY *pent;
char *name;
{
	StoreEnt(pent, name, name ? strlen(name)+1 : 0);
} /* apcstore */

static int apccmp(p1, p2)
void *p1, *p2;
{
#ifdef COMMENT
	printf("apccmp(\"%s\", \"%s\")\n", *(char **)p1, *(char **)p2);
#endif /* COMMENT */
	return(strcmp(*(char **)p1, *(char **)p2));
} /* apccmp */

#define SORT(buf, num, cmp)	qsort((char *) (buf), num, sizeof((buf)[0]), cmp)
#define APCSORT(pent, offs)	SORT((pent)->a_arr+offs, (pent)->a_ind-offs, apccmp)

static int apetoargcv(pargc, pargv, pent)
int *pargc;
char ***pargv;
APEENTRY *pent;
{
	*pargc = pent->a_ind;
	*pargv = pent->a_arr;
	apcstore(pent, NULL);
	return(*pargc);
} /* apetoargcv */

#ifdef COMMENT
static void prffblk(pffblk)
struct ffblk *pffblk;
{
	printf("\"%s\"%*s0x%02x ", pffblk->ff_name, 20-strlen(pffblk->ff_name), "", pffblk->ff_attrib&0xFF);
	printf("time:%6u date:%6u size:%6ld\n", pffblk->ff_ftime, pffblk->ff_fdate, pffblk->ff_fsize);
} /* prffblk */
#endif COMMENT

static char *mkfn(dest, pat, fn)
char *dest, *pat, *fn;
{
	char *d = dirname(pat);

	if (*d)
		sprintf(dest, "%s/%s", d, fn);
		else strcpy(dest, fn);
	return(dest);
} /* mkfn */

static void expfn(pent, pat)
APEENTRY *pent;
char *pat;
{
	struct ffblk ffblk;

	if (*pat && strpbrk(pat, "*?") && !findfirst(pat, &ffblk, 0)) {
		int offset = pent->a_ind;

		do {
			char fn[256];

			apcstore(pent, mkfn(fn, pat, ffblk.ff_name));
			/* prffblk(&ffblk); */
		} while (!findnext(&ffblk));
		APCSORT(pent, offset);
	} else apcstore(pent, pat);
} /* expfn */

int GlobCmdLn(pargc, pargv)
int *pargc;
char ***pargv;
{
	APEENTRY *pent;
	char **argv;

	argv = *pargv;
	pent = InitApe();
	apcstore(pent, argv[0]);
	while (*++argv) {
		expfn(pent, *argv);
	}
	apetoargcv(pargc, pargv, pent);
	_argv = *pargv;
	return(_argc = *pargc);
} /* GlobCmdLn */

#ifdef MAIN
int main(argc, argv)
int argc;
char *argv[];
{
	GlobCmdLn(&argc, &argv);
	while (*argv) {
		printf("%2d: %s\n", argc--, *argv++);
	}
	return(0);
} /* main */
#endif /* MAIN */
