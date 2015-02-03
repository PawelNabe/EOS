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

#ifdef	COMMENT
char *basename(name)	/* simple version */
register char *name;
{
	register char *p;

	for (p = name; *p++; )
		;
	while (--p >= name && *p != '/')
		;
	return(++p);
} /* basename */
#endif

char *basename(name)	/* sophisticated version */
register char *name;
{
	static char buf[1024];
	register char *bp;

	for (bp = buf; *bp = *name++; )
		++bp;
	while (--bp >= buf && (*bp == '/'
#ifdef	__MSDOS__
					|| *bp == '\\'
#endif
				))
		*bp = '\0';
	while (  bp >= buf && (*bp != '/'
#ifdef	__MSDOS__
					&& *bp != '\\'
#endif
				))
		--bp;
	return(++bp);
} /* basename */

#ifdef	MAIN
int main(argc, argv)
int argc;
char *argv[];
{
	do {
		printf("\"%s\"	dir:\"%s\"\nbas:\"%s\"\n", *argv, dirname(*argv), basename(*argv));
	} while (*++argv);
	return(0);
} /* main */
#endif

