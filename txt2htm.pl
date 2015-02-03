#!/bin/perl
# (C) Copyright by PaN Super Sonic Software Service 2002
#                  expert for hi tec & low budget

while (<>) {
		if ($ARGV ne $oldargv) {
			if ($oldargv ne "") {
				print "</PRE>\n";
				print "</BODY>\n";
				print "</HTML>\n";
			}
			print "<HTML>\n";
			print "<HEAD>".$ARGV."\n";
			print "<TITLE>".$ARGV."</TITLE>\n";
			print "</HEAD>\n";
			print "<BODY>\n";
			print "<PRE WIDTH=\"132\">\n";
			$oldargv = $ARGV;
		} 

		s/\202/&eacute;/go;

		s/\216/&Auml;/go;
		s/\231/&Ouml;/go;
		s/\232/&Uuml;/go;
		s/\204/&auml;/go;
		s/\224/&ouml;/go;
		s/\201/&uuml;/go;
		s/\341/&szlig;/go;

		s/"/&quot;/go;
		s/</&lt;/go;
		s/>/&gt;/go;
		s/Ä/&Auml;/go;
		s/Ö/&Ouml;/go;
		s/Ü/&Uuml;/go;
		s/ä/&auml;/go;
		s/ö/&ouml;/go;
		s/ü/&uuml;/go;
		s/ß/&szlig;/go;
		s/\f//go;

		print $_ ;
}

END {
	print "\n";
	print "</PRE>\n";
	print "</BODY>\n";
	print "</HTML>\n";
}