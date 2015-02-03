/* (C) Copyright by PaN Super Sonic Software Service 1991	*/
/*                  expert for hi tec & low budget		*/

#ifndef	VALSTR_H
#define	VALSTR_H

extern int
	StrToAem(),
	StrToFla(),
	StrToCur(),
	StrToAv(),
	StrToTv(),
	StrToIso(),
	StrToCor(),
	StrToMet(),
	StrToTou(),
	StrToAou(),
	StrToAeb(),
	StrToMon(),
	StrToMm();

extern void
	AemToStr(),
	FlaToStr(),
	CurToStr(),
	AvToStr(),
	TvToStr(),
	IsoToStr(),
	CorToStr(),
	MetToStr(),
	TouToStr(),
	AouToStr(),
	AebToStr(),
/*	MonToStr(), */
	MmToStr(),
	TimToStr(),
	FrnoToStr();

extern bcdtime
	StrToTim();

#endif	/* VALSTR_H */

