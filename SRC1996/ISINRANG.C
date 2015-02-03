/* (C) Copyright by PaN Super Sonic Software Service 1991	*/
/*                  expert for hi tec & low budget		*/

#include <stdio.h>

extern char *strtok(), *strchr();

static int rfact(sr)
register char *sr;
{
	int ret = 0;

	if (sr = strchr(sr, '*')) {
		ret = atoi(++sr);
	}
	return(ret <= 0 ? 1 : ret);
} /* rfact */

static int iisr(num, sr)
unsigned int num;
register char *sr;
{
	unsigned int lb = ~0, ub = 0;
	int ret;

	ret = rfact(sr);
#ifdef	MAIN
	printf("iisr(%d, \"%s\"):%d ", num, sr, ret);
#endif	/* MAIN */
	while (!strchr("-0123456789*", *sr))
		++sr;
	switch (*sr) {
		case '\0' :
			return(0);
		case  '*' :
			return(ret);
		case  '-' :
			lb = 0;
			break;
		default   :
			lb = atoi(sr);
			while (isdigit(*++sr))
				;

	}

	switch (*sr) {
		case  '*' :
		case '\0' :
			ub = lb;
			break;
		case  '-' :
			if (!(ub = atoi(++sr)))
				ub = ~0;
			break;
		default   :
			return(0);
	}
	return((num>=lb && num<=ub) ? ret : 0);
} /* iisr */

int IsInRange(num, range)
int num;
register char *range;
{
	register char *p;
	char rbuf[512];
	int ret;

	if (range) {
		strcpy(rbuf, range);	/* save range since strtok(3) destroys it */
		range = rbuf;
		while (p = strtok(range, ", \t")) {
			range = NULL;
			if (ret = iisr(num, p))
				return(ret);
		}
	}
	return(0);
} /* IsInRange */

#ifdef	MAIN
int main(argc, argv)
int argc;
register char *argv[];
{
	int num;

	num = atoi(*++argv);
	puts("------------------");
	while (*++argv) {
		printf("IsInRange(%d, \"%s\")=%d\n", num, *argv, IsInRange(num, *argv));
	}
	return(0);
} /* main */
#endif	/* MAIN */

