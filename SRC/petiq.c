/* (C) Copyright by PaN Super Sonic Software Service 1991	*/
/*                  expert for hi tec & low budget		*/

#include <stdio.h>
#include "rifu.h"

static FRAME eb[16];
static int eind = 0;

static void pr_date(pfb)
FRAME *pfb;
{
	char buf[64];

	sprintf(buf, "%s %2d %s %4d", "Sat", 13, "Apr", 1991);
	printf(">       %-26s<", buf);
} /* pr_date */

static void pr_neg_a_time(pfb)
FRAME *pfb;
{
	char buf[64];

	sprintf(buf, "%2d:%02d:%02d", 13, 19, 31);
	printf(">%s%2d%s%s %-26s<", "\033w1\033W1", 15, "A", "\033W0\033w0", buf);
} /* pr_neg_a_time */

static void pr_note(pfb)
FRAME *pfb;
{
	printf(">%c4%-33s%c5<", '\033', "NOTE56789012345678901234567890", '\033');
} /* pr_note */

static void pretiqline()
{
	register int fi;

	for (fi = 0; fi <= eind; ++fi)
		pr_date(eb[fi]);
	fputc('\n', stdout);
	for (fi = 0; fi <= eind; ++fi)
		pr_neg_a_time(eb[fi]);
	fputc('\n', stdout);
	for (fi = 0; fi <= eind; ++fi)
		pr_note(eb[fi]);
	fputc('\n', stdout);

	eind = 0;
} /* pretiqline */

int main(argc, argv)
int argc;
char *argv[];
{
	eind = 1;
	pretiqline();
	return(0);
} /* main */

