/* (C) Copyright by PaN Super Sonic Software Service 1993 */
/*                  expert for hi tec & low budget	  */

extern char *strchr();

static char *trstr(str, a, b)
char *str, *a, *b;
{
	register char *s = str;

	for ( ; ; ++s) {
		register char *p;

		if (p = strchr(a, *s)) {
			if ('\0' == (*s = b[p-a]))
				return(str);
		}
	}
} /* trstr */

/*                 (c) para C~  Ue  e'  a^  ae  a`  a0  c~  e^  e:  e`  i:  i^  i`  Ae  A0  E`  x   AE  o^  o:  o`  u^  u`  y'  Oe  Ue  o/  pd  Y=  a'  i'  o'  u'  n~  N~  1/2 1/4 i   <<  >>  sz  pi  mu  +-  0   .   2 */
static char dul[]="\001\025\200\201\202\203\204\205\206\207\210\211\212\213\214\215\216\217\220\221\222\223\224\225\226\227\230\231\232\233\234\235\240\241\242\243\244\245\253\254\255\256\257\341\343\346\361\370\371\375";

/*                  ©   §   Ç   ü   é   â   ä   à   å   ç   ê   ë   è   ï   î   ì   Ä   Å   È   æ   Æ   ô   ö   ò   û   ù   ý   Ö   Ü   ø   £   ¥   á   í   ó   ú   ñ   Ñ   ½   ¼   ¡   «   »   ß   ¶   µ   ±   °   ·   ² */
static char uul[]="\251\247\307\374\351\342\344\340\345\347\352\353\350\357\356\354\304\305\310\346\306\364\366\362\373\371\375\326\334\370\243\245\341\355\363\372\361\321\275\274\241\253\273\337\266\265\261\260\267\262";

char *dos2unix(str)
char *str;
{
	return(trstr(str, dul, uul));
} /* dos2unix */

char *unix2dos(str)
char *str;
{
	return(trstr(str, uul, dul));
} /* unix2dos */

static char dum[] = "\216\231\334";
static char ium[] = "\340\344\346";
/*                    Ae  Oe  Ue */

char *dos2ifu(str)
char *str;
{
	return(trstr(str, dum, ium));
} /* dos2ifu */

char *ifu2dos(str)
char *str;
{
	return(trstr(str, ium, dum));
} /* ifu2dos */

char *ToUpper(str)
char *str;
{
	return(trstr(str, "abcdefghijklmnopqrstuvwxyz", "ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
} /* ToUpper */

int main(argc, argv)
int argc;
char *argv[];
{
	register char *(*func)();

	func = ToUpper;
	if (!strcmp(argv[0], "dos2unix"))
		func = dos2unix;
	if (!strcmp(argv[0], "unix2dos"))
		func = unix2dos;
	while (*++argv) {
		puts((*func)(*argv));
	}
	return(0);
} /* main */

