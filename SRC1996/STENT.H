/* (C) Copyright by PaN Super Sonic Software Service 1991, 1992	*/
/*                  expert for hi tec & low budget		*/

#ifndef	STENT_H
#define	STENT_H

extern char *malloc(), *realloc(), *calloc();

typedef struct apeent {	/* Array of Pointers to Elements ENTry */
	char **a_arr;
	int a_ind, a_maxind;
} APEENTRY;

extern APEENTRY *InitApe();
extern void StoreEnt();

#endif	/* STENT_H */

