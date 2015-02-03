#include <stdio.h>

int main(int argc, char *argv[], char *envp[])
{
	char buf[256];
	FILE *fp;

	if (envp && *envp) {
		sprintf(buf, "/tmp/%s", *envp);
		if (fp=fopen(buf, "r")) {
			while (fgets(buf, sizeof buf, fp)) {
				fputs(buf, stdout);
			}
		}
	} else fputs("No environment\n", stdout);
	do {
		fprintf(stdout, "\"%s\" 0x%08lx\n", *envp, *envp);
	} while (*++envp);
	sleep(5);
} /* main */
