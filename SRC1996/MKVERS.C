#include <stdio.h>
#include <time.h>

extern FILE *FreOpen();
char *argv0;

int main(argc, argv)
int argc;
char *argv[];
{
	char *ct;
	long t;

	argv0 = argv[0];
	FreOpen("version.c", "w", stdout);

	fputs("char version[] = \"", stdout);
	while (*++argv)
		printf("%s ", *argv);
	time(&t);
	ct = ctime(&t);
	ct[24] = '\0';
	printf("%s\";\n", ct);

	return(0);
} /* main */

