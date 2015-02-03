#include <stdio.h>
#include <string.h>
#include "stent.h"

APEENTRY *InitApe()
{
	return((APEENTRY *) calloc(1, sizeof(APEENTRY)));
} /* InitApe */

void StoreEnt(pent, pc, len)
APEENTRY *pent;
char *pc;
int len;
{
	if (pent->a_ind >= pent->a_maxind) {
		int numbytes;

		pent->a_maxind += 20;
		numbytes = pent->a_maxind*sizeof(char *);

		pent->a_arr = (char **) ((pent->a_ind <= 0) ? malloc(numbytes) : realloc((char *) pent->a_arr, numbytes));
	}
	pent->a_arr[pent->a_ind++] = pc ? memcpy(malloc(len), pc, len) : NULL;
} /* StoreEnt */

