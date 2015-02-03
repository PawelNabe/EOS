/* (C) Copyright by PaN Super Sonic Software Service 1991	*/
/*                  expert for hi tec & low budget		*/

#ifndef	POS_H
#define	POS_H

#define	FRNOPOS	1			/* FrameNumber */
#define	FRNOLEN	3			/* FrameNumber LENgth */

#define	MEPOS	(FRNOPOS+FRNOLEN+1+1)	/* Multi Exposure */
#define	MELEN	2

#define	METPOS	(MEPOS+MELEN+1)		/* METering */
#define	METLEN	2

#define	FLPOS	(METPOS+METLEN+1)	/* FLash */
#define	FLLEN	4

#define	CURPOS	(FLPOS+FLLEN+0)		/* 2nd CURtain */
#define	CURLEN	3

#define	TVPOS	(CURPOS+CURLEN+1)	/* Time Value */
#define	TVLEN	4

#define	TOUPOS	(TVPOS+TVLEN+0)		/* Time Over/Underflow */
#define	TOULEN	1

#define	AVPOS	(TOUPOS+TOULEN+1)	/* Aperature Value */
#define	AVLEN	3

#define	AOUPOS	(AVPOS+AVLEN+0)		/* Aperature Over/Underflow */
#define	AOULEN	1

#define	PRGPOS	(AOUPOS+AOULEN+1)	/* PRoGram */
#define	PRGLEN	3

#define	AEBPOS	(PRGPOS+PRGLEN+1)	/* Auto Exposure Bracketing */
#define	AEBLEN	3

#define	ISOPOS	(AEBPOS+AEBLEN+1)	/* ISO */
#define	ISOLEN	4

#define	CORRPOS	(ISOPOS+ISOLEN+0)	/* CORRection */
#define	CORRLEN	4

#define	HOURPOS	(CORRPOS+CORRLEN+1)	/* HOURs */
#define	HOURLEN	2

#define	MINPOS	(HOURPOS+HOURLEN+1)	/* MINutes */
#define	MINLEN	2

#define	SECPOS	(MINPOS+MINLEN+1)	/* SEConds */
#define	SECLEN	2

#define TIMEPOS	HOURPOS
#define	TIMELEN	(SECPOS-HOURPOS+SECLEN)

#define	AVLPOS	(SECPOS+SECLEN+1)	/* Aperature Value Lens */
#define	AVLLEN	3

#define	MMLPOS	(AVLPOS+AVLLEN+1)	/* MM Lens */
#define	MMLLEN	6

#define	MM1POS	(MMLPOS+MMLLEN+1)	/* MM 1 Lens */
#define	MM1LEN	4

#define	MM2POS	(MM1POS+MM1LEN+1)	/* MM 2 Lens */
#define	MM2LEN	6

#define	NOTEPOS	(MM2POS+MM2LEN+1)	/* NOTE if on same line */
#define	NOTELEN	30

#endif	/* POS_H */

