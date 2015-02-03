@rem = '--*-Perl-*--
@echo off
if "%OS%" == "Windows_NT" goto WinNT
perl -x -S "%0" %1 %2 %3 %4 %5 %6 %7 %8 %9
goto endofperl
:WinNT
setlocal
set PERLLIB=
set PERL5LIB=
set CLEARCASE_PERLLIB=1
call perl -x "%~dp0%~nx0" %*
if NOT "%COMSPEC%" == "%SystemRoot%\system32\cmd.exe" goto endofperl
if %errorlevel% == 9009 echo You do not have Perl in your PATH.
if errorlevel 1 goto script_failed_so_exit_with_non_zero_val 2>nul
goto endofperl
@rem ';
#!perl
#line 19
# (C) Copyright by PaN Super Sonic Software Service 2005, 2006, 2007
#                  expert for hi tech & low budget

my $debug = undef;

# Use $_ instead of $0 directly, because ccperl in CC5 is broken.
$_ = $0;
s:\.bat$::io;
s:.*[\\/]::o;
$argv0 = $_;

my $MSWIN = !(-c '/dev/null');
my $nulldev = $MSWIN ? 'NUL' : '/dev/null';
$ENV{'PATH'} .= '';	# This enables /usr/atria/bin/Perl to pipe to cleartool(1).
$^O = 'MSWin32' if ($MSWIN && !$^O);

use Text::ParseWords;

while (defined($_ = <>)) {
	if (/^\s*$/) {
		next;

	}
	chomp;
	print "\n($_)\n";
	s/^\*//o;
	s/^,//o;
	s/,=(".*"),/,$1,/;

	my @fields;
	@fields = CSVsplit($_);
	$tv = $fields[3];
	$note = join(',', @fields[19 .. $#fields]);
	$tn1 = "$tv-$note";
	# print "($tv) ($note)\n";

	($fno, $focallen, $maxaper, $tv, $av, $isom, $expcomp, $fexpcomp, $fmode, $met, $prog, $fam, $afm, $bulb, $date, $time, $me, $bld, $blt, @note) = parse_csv($_);
	$tv = stripq($tv);
	$note = join(',', @note);
	$note = stripq($note);
	# print "($tv) ($note)\n";
	# @fields = parse_csv($_);
	$tv = $fields[3];
	$note = join(',', @fields[19 .. $#fields]);
	$tv = stripq($tv);
	$note = stripq($note);
	$tn2 = "$tv-$note";
	# print "($tv) ($note)\n";
	print (($tn1 eq $tn2) ? $tn1 : "$tn1 <> $tn2") . "\n";

	# foreach (CSVsplit($_)) {
	# 	print "<$_>\n";
	# }
	next;

	# ($a, $b, $c, $d, $e, $f, $g, $h) = parse_csv($_);
	# print "($a, $b, $c, $d, $e, $f, $g, $h)\n";
}
print "\n";

sub stripq { # string
	local $_ = $_[0];

	if (/^"(.*)"$/) {
		$_ = $1;
	}
	s/""/"/g;
	return $_;
} # stripq

sub parse_csv { # string
	return parse_line(',', 1, $_[0]);
} # parse_csv

# Cut off out of CSV.pm made by
# Christopher Rath <Christopher@Rath.ca> and Mark Mielke <Mark@Mielke.cc>
sub CSVsplit { # string
    local $_ = $_[0];
    my(@fields, $t);

    # According to the specs, a new-line is a field terminator not a field
    # delimiter. This only becomes an issue at the end of a string where
    # a new-line implies end of field, not a field delimiter. By stripping
    # the last optional new-line we allow all remaining new-lines to be
    # considered field delimiters.
    chomp;

    ###
    # Only build the array if at least one field exists. (if at least one
    # character exists, by definition, at least one field exists)
    #
    if (length($_) > 0) {
        ###
	# Break the record into fields.
	# Field Delimiter == /[,\n]/   (See comment above: According...)
        my $expecting;
        while (m`(?=.)((?:"((?:""|[^"]+)*)")?([^,]*))(,?)`sg) {
            $expecting = $4;
            if (defined($2) && !length($3)) {
                ($t = $2) =~ s/""/\"/g;
                push(@fields, $t);
            } else {
                push(@fields, $1);
            }
        }
        push(@fields, '') if $expecting;
    }

    @fields;
} # CSVsplit

__END__
:endofperl
if "%OS%" == "Windows_NT" ENDLOCAL

