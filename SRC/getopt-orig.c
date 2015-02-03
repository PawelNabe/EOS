/*
	getopt.c -- Turbo C
 
	Copyright (c) 1986,1990 by Borland International Inc.
	Alle Rechte vorbehalten.
*/
 
#include <errno.h>
#include <string.h>
#include <dos.h>
#include <stdio.h>
 
int	optind	= 1;	/* Index: welches Argument ist das n„chste */
char   *optarg;		/* Zeiger auf das Argument der akt. Option */
int	opterr	= 1;	/* erlaubt Fehlermeldungen */
 
static	char   *letP = NULL;	/* Speichert den Ort des Zeichens der
				/* n„chsten Option */
static	char	SW = '-';		/* DOS-Schalter, entweder '-' oder '/' */

/*
  Analysiert die Kommandozeilen-Option, System V-Stil.

  Standard-Syntax der Optionen ist:

    option ::= SW [optLetter]* [argLetter space* argument]
 
  wobei
    - SW ist entweder '/' oder '-', je nach dem aktuellen Wert des
      MSDOS-Schalters (int 21h function 37h).
    - vor optLetter oder argLetter steht niemals ein Leerzeichen.
    - opt/argLetter sind ausschlieálich alphabetische Zeichen.
    - optLetters mssen zu Werten in optionS passen.
    - argLetters stehen, gefolgt von ':', in optionS.
    - Argument ist jeder mit Whitespaces begrenzter String. Beachten Sie,
      daá das auch das SW-Zeichen einschlieáen kann.
    - Groá- und Kleinschreibung wird unterschieden.

  In der Kommandozeile k”nnen mehrere mit SW beginnende Gruppen von Optionen
  stehen; sie mssen aber vor allen Argmenten erscheinen, die keine SW-
  Optionen sind. Options- und Argumentbuchstaben k”nnen sich wiederholen, es
  ist Sache des Aufrufenden zu entscheiden, ob dies ein Fehler ist.

  Wenn SW alleine (als letztes Argument) auftritt, dann ist das ein Fehler.
  Die Zeichenfolge SWSW ("--" oder "//") bewirkt, daá sie selbst und alle fol-
  genden Zeichen in der Zeile ignoriert werden (erlaubt Argumente mit einem
  Schalterzeichen beginnend, die nicht Optionen sind).

  šber den String *optionS werden gltige Optionen/Argumente erkannt. Den
  argLetters folgt ein ':'. Getopt () gibt den Wert des Buchstabens der
  Option zurck oder EOF, wenn keine weiteren Optionen in der Kommandozeile
  stehen.Wenn die Option ein argLetter ist, dann zeigt optarg (global) auf
  den Argumentstring (Whitespaces werden bergangen).

  Optind wird mit 1 initialisiert und zeigt immer als Index auf das n„chste
  Argument von argv[], das getopt noch nicht verarbeitet hat. Beachten Sie,
  daá optind auf das n„chste Argument zeigt, bevor getopt() EOF zurckgibt.

  Wenn ein Fehler auftritt (wenn SW einem unbekannten Buchstaben vorausgeht),
  gibt getopt() ein '?' zurck. Normalerweise wird mit perror() eine Fehler-
  meldung ausgegeben. Die Fehlermeldung wird nicht gedruckt, wenn die
  globale Variable opterror vor dem Aufruf von getopt() mit False (Null)
  initialisiert ist.
 
  Beispiel: Wenn der MSDOS-Schalter '/' verwendet wird (das normale MSDOS-
  Zeichen) und folgende Parameter bergeben werden

    *optionS == "A:F:PuU:wXZ:"
 
  dann sind 'P', 'u', 'w' und 'X' Optionen, und  'F', 'U', 'Z' folgen
  Argumente. Hier eine gltige Kommandozeile:

    aCommand  /uPFPi /X /A L someFile
 
  wobei:
    - 'u' und 'P' sind Optionen ohne Argumente.
    - 'F' hat das Argument "Pi".
    - 'X' ist eine isolierte Option.
    - 'A' hat das Argument "L".
    - "someFile" ist keine Option und beendet getOpt. Der Aufrufende kann
      briggebliebene Argumente mit argv-Zeigern abfragen.
*/

int	getopt(int argc, char *argv[], char *optionS)
{
	unsigned char ch;
	char *optP;
 
	if (SW == 0) {
		/* SW mit DOS-Aufruf 0x37 lesen */
		_AX = 0x3700;
		geninterrupt(0x21);
		SW = _DL;
	}
 
	if (argc > optind) {
		if (letP == NULL) {
			if ((letP = argv[optind]) == NULL || 
				*(letP++) != SW)  goto gopEOF;
			if (*letP == SW) {
				optind++;  goto gopEOF;
			}
		}
		if (0 == (ch = *(letP++))) {
			optind++;  goto gopEOF;
		}
		if (':' == ch  ||  (optP = strchr(optionS, ch)) == NULL)  
			goto gopError;
		if (':' == *(++optP)) {
			optind++;
			if (0 == *letP) {
				if (argc <= optind)  goto  gopError;
				letP = argv[optind++];
			}
			optarg = letP;
			letP = NULL;
		} else {
			if (0 == *letP) {
				optind++;
				letP = NULL;
			}
			optarg = NULL;
		}
		return ch;
	}
gopEOF:
	optarg = letP = NULL;  
	return EOF;
 
gopError:
	optarg = NULL;
	errno  = EINVAL;
	if (opterr)
		perror ("Kommandozeilen-Option lesen");
	return ('?');
}

