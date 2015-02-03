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
# (C) Copyright by PaN Super Sonic Software Service 2008
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

# CReate GenAsc start script 

while (defined($_ = shift)) {
	s/film$//i;
	$fn = $cn = $_;
	$fn .= 'film';
	/^(.)/;
	$fc = $1;

	if (open(FILM, "<$fn")) {
		while (defined($_ = <FILM>)) {
			if (/^(\d\d\d?)\s+(.+)$/) {
				$fo = 0 + $1;
				$comment = $2;

				$dat = sprintf("%sdat/%s%03d.dat", $cn, $cn, $fo);
				if (-s "$dat") {
					print "Have $dat\n";

				} elsif ( -s "$fc$fo") {
					print "Have $fc$fo\n";

				} elsif (open(FO, ">$fc$fo")) {
					print "Created $fc$fo\n";

					print FO "-F$fo\n-h$comment\n";
					print FO "-l35\n-a2.8\n" if ($cn =~ /^af1/i);
					print FO "-i$2\n" if ($comment =~ /(Gold|XRG|SRV|Elite|Tmax)\s+([123]\d+)\s/);
					print FO "-Y$1\n" if ($comment =~ /([12][90]\d\d)$/);
					print FO "-M\n\n";
					close FO;

				} else {
					warn "$argv0(", __LINE__, "): Cannot create($fc$fo): $!\n";
				}
			}
		}
		close FILM;
	} else {
		warn "$argv0(", __LINE__, "): Cannot open($fn): $!\n";
	}
}

__END__
:endofperl
if "%OS%" == "Windows_NT" ENDLOCAL

