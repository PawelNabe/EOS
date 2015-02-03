#include <stdio.h>

static void putnas(str)
register char *str;
{
	register int c;

	while (c = 0xFF & (*str++))
		printf(c<='~' ? "%c" : "\\%03o", c);
} /* putnas */

int main(argc, argv)
int argc;
char *argv[];
{
	register int c;
	char buf[BUFSIZ];

	while (gets(buf)) {
		putchar('"');
		putnas(buf);
		puts("\",");
	}
	return(0);
} /* main */

#ifdef COMMENT

"====== danish ======",
"jan.", "feb.", "marts", "apr.",
"maj", "juni", "juli", "aug.",
"sept.", "okt.", "nov.", "dec.",
"januar", "februar", "marts", "april",
"maj", "juni", "juli", "august",
"september", "oktober", "november", "december",
"s\370n", "man", "tirs", "ons", "tors", "fre", "l\370r",
"s\370ndag", "mandag", "tirsdag", "onsdag", "torsdag", "fredag", "l\370rdag",

"====== finnish ======",
"tam", "hel", "maal", "huhti",
"tuo", "kes\344", "hein\344", "elo",
"syys", "loka", "mar", "jou",
"tammi", "helmi", "maalis", "huhti",
"tuoko", "kes\344", "hein\344", "elo",
"syys", "loka", "marras", "joulu",
"sunnun", "maanan", "tiis", "keski", "tors", "perjan", "lauan",
"sunnuntai", "maanantai", "tiistai", "keski", "torstai", "perjantai", "lauantai",

"====== french ======",
"janv.", "f\351vr.", "mars", "avr.",
"mai", "juin", "juil.", "a\364ut",
"sept.", "oct.", "nov.", "d\351c.",
"janvier", "f\351vrier", "mars", "avril",
"mai", "juin", "juillet", "ao\373t",
"septembre", "octobre", "novembre", "d\351cembre",
"dim", "lun", "mar", "mer", "jeu", "ven", "sam",
"dimanche", "lundi", "mardi", "mercredi", "jeudi", "vendredi", "samedi",

"====== german ======",
"Jan.", "Feb.", "M\344rz", "Apr.",
"Mai", "Jun.", "Jul.", "Aug.",
"Sept.", "Okt.", "Nov.", "Dez.",
"Januar", "Februar", "M\344rz", "April",
"Mai", "Juni", "Juli", "August",
"September", "Oktober", "November", "Dezember",
"So", "Mo", "Di", "Mi", "Do", "Fr", "Sa",
"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag",

"====== italian ======",
"genn.", "febbr.", "mar.", "apr.",
"magg.", "giugno", "luglio", "ag.",
"sett.", "ott.", "nov.", "dic.",
"gennaio", "febbraio", "marzo", "aprile",
"maggio", "giugno", "luglio", "agosto",
"settembre", "ottobre", "novembre", "dicembre",
"dom", "lun", "mar", "mer", "gio", "ven", "sab",
"domenica", "lunedi", "martedi", "mercoledi", "giovedi", "venerdi", "sabato",

"====== norwegian ======",
"jan.", "febr.", "mars", "apr.",
"mai", "juni", "juli", "aug.",
"sept.", "okt.", "nov.", "des.",
"januar", "februar", "mars", "april",
"mai", "juni", "juli", "august",
"september", "oktober", "november", "desember",
"s\370n", "man", "tirs", "ons", "tors", "fre", "l\370r",
"s\370ndag", "mandag", "tirsdag", "onsdag", "torsdag", "fredag", "l\370rdag",

"====== spanish ======",
"Ene.", "Feb.", "Mar.", "Abr.",
"Mayo", "Jun.", "Jul.", "Ago.",
"Sept.", "Oct.", "Nov.", "Dic.",
"Enero", "Febrero", "Marzo", "Abril",
"Mayo", "Junio", "Julio", "Agosto",
"Septiembre", "Octubre", "Noviembre", "Diciembre",
"Dom", "Lun", "Mar", "Mie", "Jue", "Vie", "Sab",
"Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado",

"====== swedish ======",
"jan.", "febr.", "mars", "april",
"maj", "juni", "juli", "aug.",
"sept.", "okt.", "nov.", "dec.",
"januari", "februari", "mars", "april",
"maj", "juni", "juli", "augusti",
"september", "oktober", "november", "december",
"s\366n", "m\345n", "tis", "ons", "tor", "fre", "l\366r",
"s\366ndag", "m\345ndag", "tisdag", "onsdag", "torsdag", "fredag", "l\366rdag",

"====== usa_english ======",
"Jan", "Feb", "Mar", "Apr",
"May", "Jun", "Jul", "Aug",
"Sep", "Oct", "Nov", "Dec",
"January", "February", "March", "April",
"May", "June", "July", "August",
"September", "October", "November", "December",
"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat",
"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday",

#endif /* COMMENT */
