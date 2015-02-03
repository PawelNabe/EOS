@rem = '--*-Perl-*--
@echo off
if "%OS%" == "Windows_NT" goto WinNT
perl -x -S "%0" %1 %2 %3 %4 %5 %6 %7 %8 %9
goto endofperl
:WinNT
call perl -x "-I%~dp0..\perllib" "%~dp0%~nx0" %*
if NOT "%COMSPEC%" == "%SystemRoot%\system32\cmd.exe" goto endofperl
if %errorlevel% == 9009 echo You do not have Perl in your PATH.
if errorlevel 1 goto script_failed_so_exit_with_non_zero_val 2>nul
goto endofperl
@rem ';
#!perl
#line 15
# (C) Copyright by PaN Super Sonic Software Service 2005
#                  expert for hi tech & low budget

my $debug = undef;

# Use $_ instead of $0 directly, because ccperl in CC5 is broken.
$_ = $0;
s:\.bat$::io;
s:.*[\\/]::o;
$0 = $argv0 = $_;

usage("Need at least one DAT file name") if ($#ARGV < 0);

$OS = ($ENV{OS} =~ /Windows/i) ? "Windows" : "UNIX";

my $eosbase;
if ($OS eq "UNIX") {
	$enscript = "enscript";
	$eosbase = $ENV{'HOME'};
	$rifuprg = "rifu";
} else {
	$enscript = "C:/enscript/enscript.exe";
	$eosbase = "C:";
	$rifuprg = "rifu32.exe";
}
$eosbase .= "/eos/";

$ENV{"ENSCRIPT"} = "-R1 -fCourier-BoldOblique8 --non-printable-format=octal --ul-gray=.95 --ul-style=filled --ul-angle=90 --ul-position=+450+30 --setpagedevice=Duplex:true";

$ulsize = 150;

while ($_ = shift(@ARGV)) {
	my $dat = $_;
	
	my $bn = $_;
	$bn =~ s:^.*[\\/]::o;
	$bn =~ s:\.dat$::oi;
	$bn = uc $bn;
	$_ = $bn;
	m/(.*)(...)$/o;
	$bn = "$1 $2";
		
	$_ = $dat;
	if (/^-[h?]/i) {
		usage();
	} elsif (/^-d/) {
		$debug = "T";
		next;
	} elsif (/^-u/) {
		$umlautmapping="";
		next;
	} elsif (/^-eosbase$/) {
		$eosbase = shift;
		$eosbase .= "/";
		next;
	} elsif (/^-/) {
		usage("Unknown option '$_'");
	}

	if (m:^[\\/]:) {
		$dat = $_;
	} elsif (m:.[\\/].:) {
		$dat = $eosbase . $_;
	} elsif (m/dump\d\d\d/i) {
		$dat = $_;
		$dat =~ s/\.dat$//i;

		$dat = $eosbase . "dat/" . $dat;

		$ulsize = 140;
	} elsif (/^[AETM][69FRJ][01TZ][1-9][0-9][0-9][0-9]/i || /^[AETM][69FRJ][01TZ][1-9][0-9][0-9][0-9]\.dat/i) {
		$dat = $_;
		$dat =~ s/\.dat$//i;
		
		$dat =~ m/(....)(...)$/;
		
		$dat = $eosbase . $1 . "dat/" . $dat;
	}

	print "\n$0: $dat $_\n" if ($debug);

	open RIFU, "$rifuprg -pa -LDe $rangeincl $dat |" or die "$0: Cannot open '$dat': $!";
	$from = "";
	$to = "";
	while (<RIFU>) {
		($fd, undef) = split /\|/;
		if ($olddate eq "DATE") {
			$from = $fd;
		}
		$olddate = $fd;
		print if $debug;
	}
	if ($from eq $olddate) {
		$ft = $from;
	} else {
		$ft = "$from ... $olddate";
	}
	close RIFU;
	# More than a single consecutive space confuses enscript(1) with perl 5.001.
	$ft =~ s/  / /go;
	print "\n$bn $ft\n" if $debug;

	open ESF, "> $ft" or die "$0: Cannot open output file '$ft': $!";
	open RIFU, "$rifuprg -j -LDe $rangeincl $dat |" or die "$0: Cannot run $rifuprg on '$dat': $!";
	while (<RIFU>) {
		s:^(-------- ).*[\\/](.+ -------- \d+ bytes .*):$1$2:;
		tr /\202\216\231\232\204\224\201\341\205\267\265\266\240\203/\351\304\326\334\344\366\374\337\340\300\301\302\341\342/;
		print ESF $_ or die;
		print if $debug;
	}
	close RIFU;
	close ESF;
	$lop="100";

	$dat .= ".dat";
	($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,
	       $atime,$mtime,$ctime,$blksize,$blocks)
	                  = stat($dat);

	print "$dat " . localtime($mtime). "\n" if $debug;
	utime $mtime, $mtime, $ft;


	$_ = $ENV{"COMPUTERNAME"};
	if (/^WEP0/) {
		$printer = quotemeta "--printer=\\\\QEO00028\\DEO01106";
	} elsif (/^pan-/) {
		$printer = "\"-pC:/GhostSpool/$bn.ps\"";
	}

	system("$enscript $printer --ul-font=Times-Roman$ulsize -i 1 -B -L$lop --header=\"$bn\" -G --underlay=\"$bn\" -- \"$ft\"");
	unlink $ft;
}

sub usage {
	warn "$0: " . join(" ", @_) . "\n" if (scalar(@_) > 0);
	warn "usage: $0 options... datfiles...\n";
	warn "Options:\n";
	warn "	-d		Debug\n";
	warn "\n";
	warn "	-r <range>	Set range of frames t print to <range>\n";
	warn "	-u		No umlautmapping\n";
	warn "	-eosbase <eb>	Set base of *.DAT files to <eb>\n";
	warn "\n";
	warn "Examples:\n";
	warn "\n";
	warn "	$0 -r 1,4-7 e602123\n";
	warn "	$0 -r 1 e602085\n";

	exit 22;
} # usage

__END__
:endofperl

