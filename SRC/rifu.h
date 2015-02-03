/* (C) Copyright by PaN Super Sonic Software Service 1990,1991,1998	*/
/*                  expert for hi tec & low budget			*/

#ifndef	RIFU_H
#define	RIFU_H

#define NUMELEMS(a)		(sizeof(a)/sizeof(a[0]))

#define	GAP	192	/* unused gap between length and dateform */
#define	FRNOAA	10000	/* Add this in case of 'A' to be appended to fr_no */
#define	NOTIME	0xAA	/* Do not display these time values */

#define StrCmp(a, op, b)	(strcmp(a, b) op 0)
#define StrnCmp(a, op, b, n)	(strncmp(a, b, n) op 0)

typedef unsigned char bcd;

typedef struct { bcd hour, min, sec; } bcdtime;

typedef struct {
	int sf;				/* Is it the Short Format? */
	int f_no;			/* film number [0,...,1599] */
	int fr_no;			/* frame number [0,...,99] */
	int av;				/* aperature value [1.0,...,32] */
	int av_our;			/* aperature out of range [ ,*] */
	int tv;				/* time value [bc,30",...,4000] */
	int tv_our;			/* time out of range [ ,*] */
	int ae_mode;			/* auto exposure mode [P,TV,AV,DEP,M] */
	int metering;			/* [EVAL,PART,AVR,SPOT] */
	int iso;			/* film speed [6,...,6400] */
	int correct;			/* correction [-5.0,...,+5.0] */
	int flash;			/* [OFF,MAN,ATTL,TTL,ON,->>] */
	int aeb;			/* auto exposure bracketing [ ,AEB] */
	int me;				/* multi exposures [0,...,99] */
	int lens_av;			/* aperature value [1.0,...,32] */
	int mm, lens_mm1, lens_mm2;	/* focal length [0.5,...,9999] */
	bcdtime btime;
	int p_year, p_month, p_day;
	char note[32];
} FRAME;

typedef struct {
		void (*prtHead)();
		void (*prtFrame)();
		void (*prtAtEof)();
		void (*prtAtExit)();
} PRTFUNC;

#endif	/* RIFU_H */

