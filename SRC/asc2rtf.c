/* (C) Copyright by PaN Super Sonic Software Service 1998	*/
/*                  expert for hi tech & low budget		*/

#include <stdio.h>
#include "rifu.h"
#include "valstr.h"

#define	ESC	'\033'

static char psb[] = "%s ";

static char *ascii2rtf(str)
register unsigned char *str;
{
	static char unsigned buf[512];
	register unsigned char c, *pb = buf;

	while (*pb = c = *str++) {
		switch (c) {
			case '"' : 
				strcpy(pb, "\\ldblquote");
				pb += 10;
				break;
			case '\'':
				strcpy(pb, "\\lquote");
				pb += 7;
				break;
			case '\\':
				strcpy(pb, "\\\\");
				pb += 2;
				break;
			case '{':
				strcpy(pb, "\\{");
				pb += 2;
				break;
			default  :
				if ((c < ' ') || (c > '~')) {
					sprintf(pb, "\\'%02x", c);
					pb += 3;
				}
			case '\n':
				++pb;
		}
	}
	return((char *) buf);
} /* ascii2rtf */

