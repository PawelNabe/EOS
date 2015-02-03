@rem = 'Perl, ccperl read this as an array assignment & skip the goto
@echo off
goto endofperl
@rem ';

$argv0 = $0;
$argv0 =~ s:.*\\+::;

$dn = $0;
$dn =~ s:\\+[^\\]*$::;
$dn = "." if ($dn eq $0);

if ($^O =~ /^(MS(DOS|Win32)|Windows_NT)/i) {
	$os = "Win";
	$sep = "\\";
} else {
	$os = "Unix";
	$sep = "/";
}

print "\n$argv0: Find new EOS *.DAT and *film files on $os\n";

chomp(@labels = sort { $b cmp $a } grep { /^PAN-/ } `cleartool lstype -kind lbtype -fmt "%Ln\\n"`);
$lastlbt = @labels[0];
print "LASTLBT:$lastlbt\n";

chomp(@nfiles = sort { $a cmp $b } `cleartool find -all  -version "version(/main/LATEST) && !lbtype($lastlbt)" -type f -nxname -print`);

if (@dats = grep { /\.dat$/ } @nfiles) {
	my $dats = undef;
	my $olddatdir = undef;

	foreach (@dats) {
		my $datdir = $_;
		$datdir =~ s:[/\\]\w+\.dat::o;
		$_ =~ s:.*[/\\]::o;
		my $bn = $_;

		if ($olddatdir && ($olddatdir ne $datdir)) {
			chdir $olddatdir;
			system("teos.bat $dats");
			$dats = undef;
		}
		$dats .= " $bn";
		$olddatdir = $datdir;
	}
	chdir $olddatdir;
	system("teos.bat $dats") if $dats; 
}

exit;

if (@films = grep { /\dfilm$/ } @nfiles) {
	my $films = undef;
	my $filmdir = @films[0];
	$filmdir =~ s:[/\\]\w+film::o;

	foreach (@films) {
		$_ =~ s:.*[/\\]::o;
		$films .= " $_";
	}
	chdir $filmdir && system("teos.bat $films");
}

__END__
 
:endofperl
perl -e "$s = shift; $c = $s =~ /.bat$/ ? $s : $s.'.bat'; $p = (-x $c) ? '' :'-S '; system('perl '.$p.$c.' '.join(' ',@ARGV)); exit $?;" %0 %1 %2 %3 %4 %5 %6 %7 %8 %9

