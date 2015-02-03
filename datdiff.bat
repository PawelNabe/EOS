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
set CYGWIN=nodosfilewarning
call perl -x "%~dp0%~nx0" %*
if NOT "%COMSPEC%" == "%SystemRoot%\system32\cmd.exe" goto endofperl
if %errorlevel% == 9009 echo You do not have Perl in your PATH.
if errorlevel 1 goto script_failed_so_exit_with_non_zero_val 2>nul
goto endofperl
@rem ';
#!perl
#line 20
# (C) Copyright by PaN Super Sonic Software Service 2006, 2007, 2008, 2010
#                  expert for hi tech & low budget

sub usage {
	warn "$argv0(" . (caller(0))[2] . '): ' . join(' ', @_) . "\n" if (scalar(@_) > 0);
	warn "usage: $argv0 [-j] [-s] [-i] a.dat... b.dat\n";
	warn "usage: $argv0 [-j] [-s] [-i] a.dat... dir\n";
	warn "usage: $argv0 [-j] [-s] [-i] datdir1 datdir2\n";
	warn "usage: $argv0 [-j] [-s] [-i] files_or_dirs\n";
	warn "Function:\n";
	warn "	Diff command for *.dat files.\n";
	warn "Options:\n";
	warn "	-d		Debug output\n";
	warn "\n";
	warn "	-s		short output (no diff(1)).\n";
	warn "	-i		echo identical files.\n";
	warn "	-j		pass join flag to rifu(1).\n";
	warn "	-tw		use twmerge(1) instead of diff(1).\n";
	warn "	-cd		use xcleardiff(1) instead of diff(1).\n";
	warn "	-kd		use kdiff3(1) instead of diff(1).\n";
	warn "	-ed		use ExamDiff(1) instead of diff(1).\n";
	warn "	-wm		use WinMerge(1) instead of diff(1).\n";
	warn "	-pre		diff with predecessor version in ClearCase.\n";
	warn "	-bg		Run diffs in separate windows in parallel.\n";
	warn "	-stdin		Read options/parameters from stdin/pipes.\n";
	warn "Examples:\n";
	warn "	$argv0 a.dat b.dat\n";
	warn "	$argv0 C:/eos/e601dat M:/p1/pan/eos/e601dat\n" if ($MSWIN);
	warn "	$argv0 -j -LDe -pre ert1071\n";
	warn "	$argv0 -jde -pre -bg mjz1dat\n";

	exit 22;
} # usage

# Take a filename without .dat, copy to tmp and append .dat
sub arbfn2tmpdat { # pan/eos/ert1dat/ert10171.dat@@/main/3
	local $_ = shift;
	s:\\:/:g;
	my $vepn = $_;

	s-[:\\/@]-_-go;

	$tmpdir = GetTmpDir() unless ($tmpdir);
	my $tmpfn = "$tmpdir/" . $$ . "_$_.dat";

	if (open(VEPN, "<$vepn")) {
		binmode(VEPN);
		if (open(TMPFILE, ">$tmpfn")) {
			binmode(TMPFILE);
			while (defined($_ = <VEPN>)) {
				print TMPFILE "$_";
			}
			close TMPFILE;

		} else {
			warn "$argv0(" . __LINE__ . "): Cannot create($tmpfn): $!\n";
			$tmpfn = undef;
		}
		close VEPN;

	} else {
		warn "$argv0(" . __LINE__ . "): Cannot open($vepn): $!\n";
		$tmpfn = undef;
	}
	return $tmpfn;
} # arbfn2tmpdat

sub dat2txt { # a.dat
	local $_;
	$tmpdir = GetTmpDir() unless ($tmpdir);

	my $adat = shift;

	my @arifu = `$rifu $join $lang "${adat}"`;
	push @arifu, "\n";
	@arifu = map { tr/\202\204\224\201\216\231\232\341/\351\344\366\374\304\326\334\337/; $_ } @arifu;

	$_ = ${adat};
	s/\.dat$//oi;
	s-[:\\/]-_-go;
	my $atxt = "$tmpdir/$$" . "_$_.txt";

	open(TXT, ">$atxt") or die "$argv0(" . __LINE__ . "): Cannot open($atxt): $!\n";
	print TXT @arifu;
	close TXT;

	return $atxt;
} # dat2txt

sub datfn {
	local $_;

	$_ = shift;
	warn "$argv0(" . __LINE__ . "): datfn($_) xnsuff(${xnsuff})\n" if ($debug);
	return ((/\.dat$/oi) || (m:.${xnsuff}[/\\]main[/\\].:)) ? $_ : "$_.dat";
} # datfn

sub fn2dat {
	local $_ = shift;

	if (!m/\.dat$/oi) {
		$_ = arbfn2tmpdat($_);
		push @tmpfiles, $_;
	}
	return $_;
} # fn2dat

sub datdiff { # a.dat b.dat
	my $adat = shift;
	my $bdat = shift;


	if ($bg && $MSWIN) {
		my @cmd = ();
		$title = '"DatDiff"';
		push @cmd, "start", qq("$title");
		push @cmd, "/B" if ($diffopt);
		push @cmd, "perl", "-S", qq("$0" -xn "$xnsuff");
		push @cmd, $diffopt ? $diffopt : "-pause";
		push @cmd, qq($short $echosame $join $lang "$adat" "$bdat");
		system(@cmd);

	} else {
		local @tmpfiles = ();

		$adat = datfn($adat);
		$bdat = datfn($bdat);

		if (($adat = fn2dat($adat)) && ($bdat = fn2dat($bdat))) {
			system(qq(cmp -s "$adat" "$bdat"));
			my $ret = ($? >> 8);
			if ($ret == 0) {
				print "Files ${adat} and ${bdat} are identical\n" if ($echosame);

			} elsif ($ret == 1) {
				if ($short) {
					print "diff ${adat} ${bdat}\n";

				} else {
					$atxt = dat2txt($adat);
					$btxt = dat2txt($bdat);

					system(qq($diffcmd "$atxt" "$btxt"));
					unlink $atxt, $btxt;
				}

			} elsif ($ret == 2) {
				warn "$argv0(" . __LINE__ . "): '${adat}' not found.\n" unless (-f ${adat});
				warn "$argv0(" . __LINE__ . "): '${bdat}' not found.\n" unless (-f ${bdat});

			} else {
				die "$argv0(" . __LINE__ . "): Internal error detected: cmp returned $?\n";
			}
		}
		unlink @tmpfiles if (@tmpfiles);
	}
} # datdiff

sub wanted {
    my ($dev,$ino,$mode,$nlink,$uid,$gid);

    # /^.+\.dat\z/si &&
    /^.+\.dat$/oi &&
    (($dev,$ino,$mode,$nlink,$uid,$gid) = lstat($_)) &&
    	-f _ &&
    		push @adats, $name;
} # wanted

sub finddat { # dir
	require "File/Find.pm";

	# Set the variable $File::Find::dont_use_nlink if you're using AFS,
	# since AFS cheats.

	# for the convenience of &wanted calls, including -eval statements:
	use vars qw/*name *dir *prune/;
	*name   = *File::Find::name;
	*dir    = *File::Find::dir;
	*prune  = *File::Find::prune;

	local @adats;

	# Traverse desired filesystems
	File::Find::find({wanted => \&wanted}, shift);

	return @adats;
} # finddat

sub dirdatdiff { # dir1 dir2
	require "Cwd.pm";

	$cwd = Cwd::getcwd() unless $cwd;

	my $adir = shift;
	my $bdir = shift;

	if (chdir($adir)) {
		@adats = finddat('.');

		chdir($cwd) or die "$argv0(" . __LINE__ . "): Cannot chdir($cwd): $!\n";

		if (scalar(@adats) <= 0) {
			warn "$argv0(" . __LINE__ . "): No .dat files found in '$adir'\n";
		} else {
			local $_;

			foreach (@adats) {
				s:^\.[/\\]+::o;
				datdiff("$adir/$_", "$bdir/$_");
			}
		}
	} else {
		warn "$argv0(" . __LINE__ . "): Cannot chdir($adir): $!\n";
	}
} # dirdatdiff

sub cygpath_w { # UNIX-pname
	my $pname = shift;

	if ($^O eq 'cygwin') {
		chomp($pname = `cygpath -w "$pname"`);
		$pname =~ s/\r//g;
	}
	return $pname;
} # cygpath_w

sub GetTmpDir {
	foreach ($ENV{'TMPDIR'}, $ENV{'TEMP'}, $ENV{'TMP'}, $ENV{'TEMPDIR'}, $MSWIN ? ("C:\\temp", "C:\\tmp", "D:\\temp", "D:\\tmp") : ('/var/tmp', '/usr/tmp', '/tmp')) {
		if (defined($_) && -d && (($^O eq 'cygwin') or -w)) {	# cygwin has a bug with -w on dirs
			return cygpath_w($_);
		}
	}

	return '.';
} # GetTmpDir

sub System { # qq(params....)
	my $tmpf1;
	my $tmpdir = GetTmpDir();
	my $tmpfbn = sprintf("%0x%d", time, $$);
	$tmpf1 = "$tmpdir/$tmpfbn.txt";

	open(OLDOUT, ">&STDOUT");
	open(OLDERR, ">&STDERR");

	open(STDOUT, ">$tmpf1") || die "$argv0(" . __LINE__ . "): Cannot redirect stdout to '$tmpf1': $!\n";
	open(STDERR, ">&STDOUT") || die "$argv0(" . __LINE__ . "): Cannot dup stdout: $!\n";

	select(STDERR); $| = 1;	# make unbuffered
	select(STDOUT); $| = 1;	# make unbuffered

	system(@_);

	close(STDOUT);
	close(STDERR);

	open(STDOUT, ">&OLDOUT");
	open(STDERR, ">&OLDERR");

	my @ret;

	@ret = do { local( @ARGV ) = "$tmpf1" ; <> } ;
	unlink($tmpf1);
	return wantarray ? @ret : "@ret";
} # System

# Use $_ instead of $0 directly, because ccperl in CC5 is broken.
$_ = $0;
s:\.bat$::io;
s:.*[\\/]::o;
$argv0 = $_;

my $MSWIN = (($^O eq 'cygwin') or !(-c '/dev/null'));
my $nulldev = $MSWIN ? 'NUL' : '/dev/null';
$ENV{'PATH'} .= '';	# This enables /usr/atria/bin/Perl to pipe to cleartool(1).
$^O = 'MSWin32' if ($MSWIN && !$^O);

# Avoid Warning: locale not supported by Xlib, locale set to C
undef $ENV{'LC_CTYPE'};
undef $ENV{'LANG'};

$rifu = 'rifu';
$xcleardiff = $MSWIN ? 'cleartool diff -g' : 'xcleardiff';

$diffcmd = 'diff';

@ARGV = map { if (/^-stdin$/i) { my @lines = <STDIN>; chomp @lines; @lines = grep /^./, @lines; } else { $_ } } @ARGV;

$xnsuff = '@@';
$pred = 0;

%diffcmds = (
	'cd' => $xcleardiff,
	'ed' => 'ExamDiff',
	'kd' => 'kdiff3',
	'tw' => 'twmerge -r',
	'wm' => 'WinMerge',
);

$| = 1; # Autoflush on

while (defined($_ = shift(@ARGV))) {
	my $arg = $_;

	warn "$argv0(" . __LINE__ . "): ARG='$arg'" if ($debug);

	if (/^$/) {
		next;

	} elsif (/^-d$/i) {
		$debug = 1;
		next;

	} elsif (/^-bg$/i) {
		$bg = 1;
		next;

	} elsif (/^-de$/i) {
		$lang = '-LDe';
		next;

	} elsif (/^-L$/) {
		($lang = shift) or die "$argv0(" . __LINE__ . "): Need language with option $_.\n";
		$lang = "-L$lang";
		next;

	} elsif (/^-L.+$/) {
		$lang = $_;
		next;

	} elsif (/^-(cd|ed|kd|tw|wm)$/i) {
		$diffcmd = $diffcmds{lc($1)};
		$diffcmd .= '.bat' if ($^O eq 'cygwin');
		$diffopt = "-$1";
		next;

	} elsif (/^-jde/oi) {
		$join = '-j';
		$lang = '-LDe';
		next;

	} elsif (/^(-j)/oi) {
		$join = lc($1);
		next;

	} elsif (/^(-i)/oi) {
		$echosame = $1;
		next;

	} elsif (/^(-s)/oi) {
		$short = $1;
		next;

	} elsif (/^-pause/i) {
		$pause = 1;
		next;

	} elsif (/^-pre/i) {
		$pred = 1;
		next;

	} elsif (/^-xn/i) {
		($xnsuff = shift) or die "$argv0(" . __LINE__ . "): Need extended name suffix with option $_.\n";
		next;

	} elsif (/^--$/) {
		last;

	} elsif (/^-[hq?]/i) {
		usage();

	} else {
		unshift(@ARGV, $_);
		last;
	}
}

usage('Need something to diff.') if (($pred + scalar(@ARGV)) < 2);

if ($pred) {
	while (defined($_ = shift)) {
		if (-d $_) {
			unshift @ARGV, finddat($_);
		} else {
			my $fn = datfn($_);
			warn "$argv0(" . __LINE__ . "): datfn($_): '$fn'\n" if ($debug);

			$_ = System(qq(cleartool des -fmt "_____BASENAME(%ELn)_____PRED(%En@@%PSn)_____CURRENT(%Xn)" "$fn"));
			m/^_____BASENAME\((.*)\)_____PRED\((.*)\)_____CURRENT\((.*)\)$/;
			my $b = $1;
			my $p = $2;
			my $c = $3;
			foreach (($b, $p, $c)) {
				if (/^\<name-unknown>/ || /^$/) {
					warn "$argv0(" . __LINE__ . "): Non-mvfs file '$fn'\n";
					$p = $c = undef;
					last;
				}
			}
			if ($b && $p && $c) {
				if ($p =~ m/${xnsuff}$/) {
					warn "$argv0(" . __LINE__ . "): No predecessor for '$fn' found\n";

				} else {
					warn "$argv0(" . __LINE__ . "): $b $p $c\n" if ($debug);
					if ($MSWIN) {	# to avoid Cannot open(e602214.dat@@/main/CHECKEDOUT): Invalid argument
						if ($c =~ m:^(.*)${xnsuff}[\\/]main[\\/].*CHECKEDOUT$:) {
							$c = $1;
						}
					}
					datdiff($p, $c);
				}
			}
		}
	}
} else {
	my $lastarg = pop;
	push @ARGV, $lastarg;

	while ($#ARGV >= 1) {
		my $a = shift;

		if (-d $a) {
			dirdatdiff($a, $lastarg);
		} else {
			if (-d $lastarg) {
				datdiff($a, "$lastarg/$a");
			} else {
				datdiff($a, shift);
			}
		}
	}
}

if ($pause) {
	print 'Please press RETURN...';
	$_ = <>;
}
__END__
:endofperl
if "%OS%" == "Windows_NT" ENDLOCAL

