int StrToBcd(s)
char s[];
{
	return((((s[0]-'0')<<4)&0xF0)+((s[1]-'0')&0x0F));
} /* StrToBcd */

char *BcdToStr0(bcdc)
char bcdc;
{
	static char r[4];

	r[0] = '0'+((bcdc>>4)&0x0F);
	r[1] = '0'+( bcdc    &0x0F);
	r[2] = r[3] = '\0';
	return(r);
} /* BcdToStr0 */

char *BcdToStr(bcdc)
char bcdc;
{
	char *r;

	r = BcdToStr0(bcdc);
	if (*r == '0')
		*r = ' ';
	return(r);
} /* BcdToStr */

int main(argc, argv)
int argc;
char *argv[];
{
	register int i;

	for (i=0; i<=0xFF; ++i) {
		char *s;

		s = BcdToStr(i);
		i = StrToBcd(s);
		printf("0x%02x %03d 0%03o \"%s\"\n", i, i, i, s);
	}
	return(0);
} /* main */

