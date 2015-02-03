/* (C) Copyright by PaN Super Sonic Software Service 1991	*/
/*                  expert for hi tec & low budget		*/

static int doy(day, mon, year)
int day, mon, year;
{
	static int dpm[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	register int m, days;

	days=0;
	for (m = 1; m < mon; ++m) {
		days += dpm[m];
		if (m == 2 && (!(year % 4) && (year % 100) || !(year % 400)))
			++days;
	}
	days += day;
	return(days);
} /* doy */

static char
	*de[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" },
	*df[] = { "Dim", "Lun", "Mar", "Mer", "Jeu", "Ven", "Sam" },
	*dd[] = {  "So",  "Mo",  "Di",  "Mi",  "Do",  "Fr",  "Sa" },
	*dp[] = { "Nie", "Pon", "Wto", "Sro", "Czw", "Pia", "Sob" },
	**dw = de;
	
static char *dowk(day, mon, year)
int day, mon, year;
{
	register long yy = year-1;

	return(dw[(yy*365L+yy/4L-yy/100L+yy/400L+doy(day, mon, year))%7L]);
} /* dowk */

static char ___[] = "???",
	_Jan_[] = "Jan", _Feb_[] = "Feb", _Mar_[] = "Mar", _Apr_[] = "Apr",
	_Mai_[] = "Mai", _Jun_[] = "Jun", _Jul_[] = "Jul", _Aug_[] = "Aug",
	_Sep_[] = "Sep", _Oct_[] = "Oct", _Nov_[] = "Nov", _Dec_[] = "Dec",

	*me[] = { ___,	_Jan_, _Feb_, _Mar_, _Apr_,
			"May", _Jun_, _Jul_, _Aug_,
			_Sep_, _Oct_, _Nov_, _Dec_ },

	*mf[] = { ___,	_Jan_, "Fev", _Mar_, "Avr",
			_Mai_,"Juin","Juil", "Aou",
			_Sep_, _Oct_, _Nov_, _Dec_ },

#ifdef  __MSDOS__
#define	_Mar_ "M\204rz"
#else
#define _Mar_ "M\344rz"
#endif
	*md[] = { ___,	_Jan_, _Feb_, _Mar_, _Apr_,
			_Mai_, _Jun_, _Jul_, _Aug_,
			_Sep_, "Okt", _Nov_, "Dez" },
#undef	_Mar_

	*mp[] = { ___,	"Sty", "Lut", _Mar_, "Kwi",
			_Mai_, "Cze", "Lip", "Sie",
			"Wrz", "Paz", "Lis", "Gru" },

	*ml[] = { ___,	  "I",  "II", "III",  "IV",
			  "V",  "VI", "VII", "VIII",
			 "IX",   "X",  "XI", "XII" },

	**months = me;

void DateStr(buf, day, mon, year)
char buf[];
int day, mon, year;
{
	sprintf(buf, "%s %2d %s %4d", dowk(day, mon, year), day, months[mon], year);
} /* DateStr */

void SetLang(lang)
char *lang;
{
	dw = de;
	months = me;
	if (lang) {
		if (strcmp(lang, "De")) {
			if (strcmp(lang, "Fr")) {
				if (strcmp(lang, "Po")) {
					if (!strcmp(lang, "R")) {
						months = ml;
					}
				} else {
					dw = dp;
					months = mp;
				}
			} else {
				dw = df;
				months = mf;
			}
		} else {
			dw = dd;
			months = md;
		}
	}
} /* SetLang */

