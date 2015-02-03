# (C) Copyright by PaN Super Sonic Software Service 1990,1991,1992,1993
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

all:		genasc rifu epatch ejoin asctodat esplit dprint

#ifdef __MSDOS__
# S=		asm
# O=		-o$@
# E=		.exe
# CC=		tcc
# OBJ=		.obj
# EXPFNOBJ=	expfn$(OBJ) stent$(OBJ)
# GETOPTOBJ=	getopt$(OBJ)

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

GENASCOBJ=genasc$(OBJ) $(GETOPTOBJ) valstr$(OBJ) dispfram$(OBJ) datestr$(OBJ) setargv$(OBJ)

genasc$(E):	$(GENASCOBJ)
		$(CC) $(O) $(LDFLAGS) $(GENASCOBJ)

RIFUOBJ=rifu$(OBJ) $(GETOPTOBJ) $(EXPFNOBJ) valstr$(OBJ) isinrang$(OBJ) prtframe$(OBJ) dispfram$(OBJ) datestr$(OBJ) setargv$(OBJ)

rifu$(E):	$(RIFUOBJ)
		$(CC) $(O) $(LDFLAGS) $(RIFUOBJ)

rifu$(OBJ):	rifu.c rifu.h valstr.h pos.h

EPOBJ=epatch$(OBJ) $(GETOPTOBJ) setargv$(OBJ)

epatch$(E):	$(EPOBJ)
		$(CC) $(O) $(LDFLAGS) $(EPOBJ)

EJOBJ=ejoin$(OBJ) freopen$(OBJ)

ejoin$(E):	$(EJOBJ)
		$(CC) $(O) $(LDFLAGS) $(EJOBJ)

ATDOBJ=asctodat$(OBJ) $(EXPFNOBJ) valstr$(OBJ) freopen$(OBJ)

asctodat$(E):	$(ATDOBJ)
		$(CC) $(O) $(LDFLAGS) $(ATDOBJ)

asctodat$(OBJ):	rifu.h valstr.h pos.h

DPOBJ=dprint$(OBJ) $(GETOPTOBJ)

dprint$(E):	$(DPOBJ)
		$(CC) $(O) $(LDFLAGS) $(DPOBJ)

MKVOBJ=		mkvers$(OBJ) freopen$(OBJ)

mkvers$(E):	$(MKVOBJ)
		$(CC) $(O) $(LDFLAGS) $(MKVOBJ)

dispfram$(OBJ):	valstr.h pos.h rifu.h
prtframe$(OBJ):	valstr.h rifu.h
genasc$(OBJ):	valstr.h pos.h rifu.h
valstr$(OBJ):	valstr.h pos.h rifu.h
expfn$(OBJ):	stent.h
stent$(OBJ):    stent.h

pcc$(E):	pcc$(OBJ) freopen$(OBJ)
		$(CC) $(O) $(LDFLAGS) pcc$(OBJ) freopen$(OBJ)

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

