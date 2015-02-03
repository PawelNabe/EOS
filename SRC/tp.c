/* (C) Copyright by PaN Super Sonic Software Service 1991	*/
/*                  expert for hi tec & low budget		*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(argc, argv)
int argc;
char *argv[];
{
	register int c, llen = 0;

	while ((c = getchar()) != EOF) {
		c &= 0xFF;
		if (c == '\n' || isprint(c)) {
			putchar(c);
			if (!(++llen % 78))
				putchar('\n');
		}
	}
	putchar('\n');
	return(0);
} /* main */
