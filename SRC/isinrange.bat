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
# (C) Copyright by PaN Super Sonic Software Service 2004
#                  expert for hi tech & low budget

while ($range = shift(@ARGV)) {
	print "\nRANGE: $range\n";
	for (0..36) {
		$ret = IsInRange($_, $range);
		print "IsInRange($_, $range): $ret\n";
	}
}

$range = "";
$ret = IsInRange($_, $range);
print "IsInRange($_, $range): $ret\n";

sub IsInRange { # num, range
	my $num = $_[0];
	my $range = $_[1];
	my $ret = 0;

	@fields = split(/[, \t]/, $range);
	foreach (@fields) {
		if ($ret = iisr($num, $_)) {
			return $ret;
		}
	}
	return 0;
} # IsInRange

sub iisr { # num, subrange
	my $num = $_[0];
	my $sr = $_[1];

	$sr =~ s/[ \t]*//go;
	return 0 if ($sr eq "");

	$_ = $sr;
	if (/-/) {
		if (/^-+$/) {
			return 0;
		} elsif (/^-/) {
			$_ = "0$_"
		} elsif (/-$/) {
			$_ .= "99";
		}
		($lb, $ub) = split /-/, $_;
		return(($num>=$lb) && ($num<=$ub));
	} else {
		return($num==$_);
	}
} # iisr

__END__
:endofperl
