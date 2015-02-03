@rem = 'Perl, ccperl reads this as an array assignment & skip the goto
@echo off
goto endofperl
@rem ';

$base = $0;
$base =~ s:.*\\+::;

$dn = $0;
$dn =~ s:\\+[^\\]*$::;
$dn = "." if ($dn eq $0);

if ($^O =~ /^(MS(DOS|Win32)|Windows_NT)/i) {
	$os = "Win";
	$eosdir = "c:\\eos";
	$sep = "\\";
} else {
	$os = "Unix";
	$eosdir = "/home/lv25493/eos";
	$sep = "/";
}

print "\n$base: transfer EOS files on $os\n";

for (@ARGV) {
	$ddir = "";
	$fn = $_;
	print "\nTransfer: '$fn'\n";
	$_ = $fn;

	if (/[0-9]film$/) {
		if ($os eq "Unix") {
			system "WinAscii2UnixAscii $fn";
		}
		$ddir = $eosdir;
	}
	if (/[0-9][0-9][0-9]\.dat$/) {
		$_ =~ m/(.*)(\.dat)$/;
		$_ = $1;
	}
	if (/[0-9][0-9][0-9]$/) {
		$fn = $_;
		$_ =~ m/(.*)([0-9][0-9][0-9])$/;
		if ($1 eq "dump") {
			$ddir = "$eosdir$sep"."dat";
		} else {
			$ddir = "$eosdir$sep$1"."dat";
		
			if (($os eq "Win") && (-f $fn)) {
				system "asctodat32 $fn";
			}
		}

		$fn = $fn.".dat"
	}

	if ($ddir eq "") {
		print "$base: Don't know what to do with '$fn'\n";
		next;
	}

	if ($os eq "Win") {
		chmod 0777, "$ddir$sep$fn";
		unlink "$ddir$sep$fn";
		system "copy /y $fn $ddir";
		if ($ENV{'LOGONSERVER'} !~ /pan-00/) {
			system "copy /y $fn u:\\TRANS";
			system "rsh sls27g \"cd TRANS ; teos $fn\"";
		}
	} else {
		system "/bin/cp -f -p $fn $ddir\n";
	}
}

__END__
 
:endofperl
perl -e "$s = shift; $c = $s =~ /.bat$/ ? $s : $s.'.bat'; $p = (-x $c) ? '' :'-S '; system('perl '.$p.$c.' '.join(' ',@ARGV)); exit $?;" %0 %1 %2 %3 %4 %5 %6 %7 %8 %9

