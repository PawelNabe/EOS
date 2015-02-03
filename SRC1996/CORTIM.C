#include <time.h>
#include "rifu.h"

extern long atol();

void CorTime(pfb, sec)
FRAME *pfb;
time_t sec;
{
	static time_t t;
	struct tm *ptm;

	if (!t)
		ptm = localtime(&t);	/* get isdst */
	ptm->tm_year = pfb->p_year - 1900;
	ptm->tm_mon = pfb->p_month - 1;
	ptm->tm_mday = pfb->p_day;
	ptm->tm_hour = BcdToI(pfb->btime.hour);
	ptm->tm_min = BcdToI(pfb->btime.min);
	ptm->tm_sec = BcdToI(pfb->btime.sec);
	sec += mktime(ptm);
	ptm = localtime(&sec);

	pfb->p_year = ptm->tm_year + 1900;
	pfb->p_month = ptm->tm_mon + 1;
	pfb->p_day = ptm->tm_mday;
	pfb->btime.hour = IToBcd(ptm->tm_hour);
	pfb->btime.min = IToBcd(ptm->tm_min);
	pfb->btime.sec = IToBcd(ptm->tm_sec);
} /* CorTime */

#ifdef COMMENT
void CorTime(pfb, sec)
FRAME *pfb;
long sec;
{
	long plus, yr, mo, da, h, m, s;
	char buf[32];

	yr = pfb->p_year;
	mo = pfb->p_month;
	da = pfb->p_day;
	h = BcdToI(pfb->btime.hour);
	m = BcdToI(pfb->btime.min);
	s = BcdToI(pfb->btime.sec);

	sprintf(buf, "%ld--%02ld:%02ld:%02ld", sec, h, m, s);
	if (plus = (sec >= 0)) {
		if (!ValidBcd((bcd) (s+=sec), 60)) {
			s%=60;
			if (!ValidBcd((bcd) (m+=sec/60), 60)) {
				m%=60;
				if (!ValidBcd((bcd) (h+=sec/(60*60)), 24)) {
					h%=24;
					if (!ValidBcd((bcd) (da+=sec/(24*60*60)), 31)) {
						da=1;
						++mo;
					}
				}
			}

		}
	}

	sprintf(pfb->note, "%s--%02ld:%02ld:%02ld", buf, h, m, s);
	pfb->p_year = yr;
	pfb->p_month = mo;
	pfb->p_day = da;
	pfb->btime.hour = IToBcd(h);
	pfb->btime.min = IToBcd(m);
	pfb->btime.sec = IToBcd(s);
} /* CorTime */
#endif

int main(argc, argv)
int argc;
char *argv[];
{
	FRAME fb;
	long cor;

	memset((char *) &fb, '\0', sizeof fb);
	fb.tv = 0xFC;
	fb.av = 0x04;
	fb.p_year = 1992;
	fb.p_month = 4;
	fb.p_day = 13;
	fb.btime.hour = 1;
	fb.btime.min = 2;
	fb.btime.sec = 3;

	while (*++argv) {
		fb.note[0] = '\0';
		DispFrame(&fb, 0, 0, 0, '\n');
		cor = atol(*argv);
		CorTime(&fb, cor);
		DispFrame(&fb, 0, 0, 0, '\n');
		puts("---------");
	}
	return(0);
} /* main */

