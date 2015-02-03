/* (C) Copyright by PaN Super Sonic Software Service 1991,1992,1993,1994,2002,2006	*/
/*                  expert for hi tec & low budget					*/

#include <stdio.h>
#include <string.h>
#if _WIN32
#include <windows.h>
#endif

#ifndef	__DATE__
#define	__DATE__	"Tue 17 Jan 2002"
#endif

#ifndef	__TIME__
#define	__TIME__	"18:15"
#endif
#ifdef __TIMESTAMP__
#define ___TS___ " " __TIMESTAMP__
#else
#define ___TS___ ""
#endif

void expfn_version()
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
} /* expfn_version */

#ifdef	__MSDOS__
#include <dir.h>
#include <dos.h>
#else

struct ffblk {
	char ff_name[1024];
};

int _argc;
char **_argv;

#ifdef _WIN32
static HANDLE hFind;

int findfirst(pat, pffblk, i)
char *pat;
struct ffblk *pffblk;
int i;
{
	WIN32_FIND_DATA FindFileData;

	hFind = FindFirstFile(pat, &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE) {
		strcpy(pffblk->ff_name, FindFileData.cFileName);
		return(0);	
	}
	return(1);
} /* findfirst */

int findnext(pffblk)
struct ffblk *pffblk;
{
	WIN32_FIND_DATA FindFileData;

	if (FindNextFile(hFind, &FindFileData)) {
		strcpy(pffblk->ff_name, FindFileData.cFileName);
		return(0);
	} else {
		FindClose(hFind);
		return(1);
	}
} /* findnext */

#else

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
	char buf[2*1024];

	sprintf(buf, "%s:%d", pffblk->ff_name, ++cnt);
	strcpy(pffblk->ff_name, buf);
	if (cnt > 1)
		cnt = 0;

	return(cnt);
} /* findnext */
#endif /* !__MSDOS__ */
#endif /* !_WIN32 */

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
#if __MSDOS__ || Win32
					|| *p == '\\'
#endif
				); --p)
		;
	for (   ; (p >= dn) && (*p != '/'
#if	__MSDOS__ || _WIN32
					&& *p != '\\'
#endif
				); --p)
		;
	for (   ; (p >= dn) && (*p == '/'
#if	__MSDOS__ || _WIN32
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

