#include <stdio.h>
#include <time.h>
#include <dos.h>
#include <process.h>

#define INTR	0x1C

static long calib()
{
	register long a;
	time_t t;

	for (t=time(NULL); t==time(NULL); )
		;
	for (a=0, ++t; t==time(NULL); ++a)
		;
	return(a);
} /* calib */

static void tspeed(msg)
char *msg;
{
	 long a, b, c;

	 a = calib();
	 b = calib();
	 c = calib();
	 printf("(%4ld + %4ld + %4ld )/3 = %4ld	%s\n", a, b, c, (a+b+c)/3, msg);
} /* tspeed */

static void interrupt (*oldintr)();

static volatile unsigned delayy;

static void interrupt tintr()
{
	volatile long a;

	(*oldintr)();
	for (a=0; a<delayy; ++a)
		;
} /* tintr */

int main(argc, argv)
int argc;
char *argv[];
{
	char *cmd;

	delayy = argc>1 ? atoi(argv[1]) : 8000;
	cmd = argc>2 ? argv[2] : "COMMAND.COM";
	oldintr = getvect(INTR);

	/* tspeed("before setvect(INTR, tintr)"); */
	setvect(INTR, tintr);
	/* tspeed("after setvect(INTR, tintr)"); */
	spawnlp(P_WAIT, cmd, cmd, NULL);
	/* tspeed("before setvect(INTR, oldintr)"); */
	setvect(INTR, oldintr);
	/* tspeed("after setvect(INTR, oldintr)"); */
	/* delay(1); */
	return(0);
} /* main */

