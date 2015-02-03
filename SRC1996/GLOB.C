#include <stdio.h>
#include <dir.h>
#include <dos.h>

static void pdir(char *dir)
{
	struct ffblk ffblk;
	char buf[256];
	int ind = 0;

	sprintf(buf, "%s/%s", dir, "*.*");
	if (!findfirst(buf, &ffblk, 0 /* FA_ARCH */)) {
		do {
			printf("%2d: %s\n", ++ind, ffblk.ff_name);
		} while (!findnext(&ffblk));
	}
} /* pdir */

int main(int argc, char *argv[])
{
	long t;

	time(&t);
	puts(ctime(&t));
	while (*argv)
		puts(*argv++);
	pdir("/tmp");
	pdir("/etc");
	pdir("/pan/..");
	sleep(5);
	return(0);
} /* main */
