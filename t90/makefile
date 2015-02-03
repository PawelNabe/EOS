# (C) Copyright by PaN Super Sonic Software Service 2002
#                  expert for hi tec & low budget

# Makefile usable for DOS and UNIX
HISTFILE=
CDPATH=
ENV=

LDFLAGS=
CFLAGS=

S=		s
O=		-o $@
E=
OBJ=		.o

#ifdef __MSDOS__
# S=		asm
# O=		-o$@
# E=		.exe
# CC=		tcc
# OBJ=		.obj
# EXPFNOBJ=	expfn$(OBJ) stent$(OBJ)
# GETOPTOBJ=	getopt$(OBJ)

all:		rifut90$(E)

# asctodat:	asctodat$(E)
# dprint:		dprint$(E)
# esplit:		esplit$(E)
# genasc:		genasc$(E)
# epatch:		epatch$(E)
# ejoin:		ejoin$(E)
# esynt:		esynt$(E)
# rifu:		rifu$(E)
# mkvers:		mkvers$(E)
#endif # __MSDOS__

RIFUT90OBJ=rifut90$(OBJ) datestr$(OBJ) freopen$(OBJ)

rifut90$(E):	$(RIFUT90OBJ)
		$(CC) $(O) $(LDFLAGS) $(RIFUT90OBJ)

rifut90$(OBJ):	rifut90.c rifu.h pos.h

.c$(OBJ):
		$(CC) -c $(CFLAGS) $<

.c$(E):
		$(CC) -c $(CFLAGS) $<
		$(CC) $(O) $(LDFLAGS) $*$(OBJ)

.c.$(S):
		$(CC) -S $(CFLAGS) $<

%.dat:		%
		asctodat $@

TAGS:
		ctags -t *.[ch]

#ifndef __MSDOS__
.SUFFIXES:	.i .$(S) .dat

.c.i:
		$(CC) -P $(CFLAGS) $<
# Find hidden dependencies. Works only with SunMake's NSE.
# Generates .make.state & .nse_depinfo.
.KEEP_STATE:

#endif # !__MSDOS__

