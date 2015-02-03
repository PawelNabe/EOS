#!/bin/perl
# (C) Copyright by PaN Super Sonic Software Service 2002
#                  expert for hi tec & low budget

# use strict;

    eval 'exec p:       empperl?.00inMSWin32-x86-objectperl.exe -S $0 ${1+"$@"}'

        if $running_under_some_shell;

require "find.pl";
use Cwd;

# Traverse desired filesystems

my(@filenamearray);
@filenamearray = ();
$cwd = getcwd();
print "Analyse directory \"$cwd\"\n";

&find('.');

exit;
use File::Compare;
sub wanted {
    if (((undef,undef,$mode,undef,undef,undef,undef,$size) = stat($_)) &&
   	 -f _ && $size > 0) {
   	 	
   	 	# print("$cwd/$name $size\n");
   	 	# $name = $cwd."/".$name;
		
		if ($filenamearray[$size][0]) {
			# print "Found $name $filenamearray[$size][0] fs[$size] $#{$filenamearray[$size]}\n";
			for $i (0..$#{$filenamearray[$size]}) {
				# print "compare($name, $filenamearray[$size][$i])\n";
				if (compare($cwd."/".$name, $cwd."/".$filenamearray[$size][$i]) == 0) {
					print "\"$name\" == \"$filenamearray[$size][$i]\"\n";
					last;
				}	
    		}
    	}
    	push @{ $filenamearray[$size]}, $name;
    	# print "Push fs[$size] $#{$filenamearray[$size]}\n";

  	}
}

my(@fs);

@fs = ();

push @{ $LoL[0] }, "wilma", "betty";

push @{ $fs[123]}, qw(f1);
push @{ $fs[123]}, qw(huhu);

push @{ $fs[234]}, qw(f2);
push @{ $fs[567]}, qw(f3);
push @{ $fs[678]}, qw(f4);
push @{ $fs[789]}, qw(f5);

for $i ( 123 .. $#fs ) {
	$fs[$i][0] && print "\t elt $i is [ @{$fs[$i]} ], $fs[$i]\n";
};

for $i ( 0 .. $#fs ) {
        for $j ( 0 .. $#{$fs[$i]} ) {
            print "elt $i $j is $fs[$i][$j]\n";
        }
    }
