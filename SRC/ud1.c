/* (C) Copyright by PaN Super Sonic Software Service 1992	*/
/*                  expert for hi tec & low budget		*/

#include <stdio.h>

#define	ESC	'\033'
#define NC	3

static void p1(c)
unsigned char c;
{
	putc(c, stdout);
} /* p1 */

static p3(a, b, c)
unsigned char a, b, c;
{
	int i=NC;

	while (--i>=0) {
		p1(a);
		p1(b);
		p1(c);
	}
} /* p3 */

static void defCharProlog(c)
unsigned char c;
{
	p1(ESC);
	p1('&');
	p1(0);
	p1(c);
	p1(c);
	p1(2);
	p1(NC*11);
	p1(2);
} /* defCharProlog */

static usrCharOnOff(on_off)
int on_off;
{
	p1(ESC);
	p1('%');
	p1(!!on_off);
} /* usrCharOnOff */

static void prUsrStr(str)
char *str;
{
	usrCharOnOff(1);
	fputs(str, stdout);
	usrCharOnOff(0);
} /* prUsrStr */

static void dWidOnOff(on_off)
int on_off;
{
	p1(ESC);
	p1('W');
	p1(!!on_off);
} /* dWidOnOff */

void defUsrChrs()
{
	defCharProlog('#');
	p3(0377, 0377, 0377);
	p3(0340, 0016, 0007);
	p3(0340, 0016, 0007);
	p3(0340, 0176, 0007);
	p3(0343, 0201, 0307);
	p3(0377, 0201, 0377);
	p3(0343, 0201, 0307);
	p3(0340, 0176, 0007);
	p3(0340, 0016, 0007);
	p3(0340, 0016, 0007);
	p3(0377, 0377, 0377);

	defCharProlog('[');	/* ] */
	p3(0377, 0377, 0377);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0377, 0377, 0377);

	defCharProlog('*');
	p3(0377, 0377, 0377);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0176, 0007);
	p3(0343, 0201, 0307);
	p3(0343, 0201, 0307);
	p3(0343, 0201, 0307);
	p3(0340, 0176, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0377, 0377, 0377);

	defCharProlog('.');
	p3(0377, 0377, 0377);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0036, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0340, 0000, 0007);
	p3(0377, 0377, 0377);

	defCharProlog(' ');
	p3(0000, 0000, 0000);
	p3(0000, 0000, 0000);
	p3(0000, 0000, 0000);
	p3(0000, 0000, 0000);
	p3(0000, 0000, 0000);
	p3(0000, 0000, 0000);
	p3(0000, 0000, 0000);
	p3(0000, 0000, 0000);
	p3(0000, 0000, 0000);
	p3(0000, 0000, 0000);
	p3(0000, 0000, 0000);
} /* defUsrChrs */

int main(argc, argv)
int argc;
char *argv[];
{
	/* freopen("PRN", "wb", stdout); */

	defUsrChrs();
	fputs("abcdefghijklmnopqrstuvwxyz", stdout);
	dWidOnOff(1);
	prUsrStr("[] . * # [] . * #");
	dWidOnOff(0);
	puts("ABCDEFGHIJKLMNOPQRSTUVWXYZ\n\n\n\n");

	return(0);
} /* main */

