#!/usr/bin/perl -w

while(my $line = <STDIN>){
	chomp($line);
	if($line =~ m/lines\.*: ([\d.]+)%\s+\((\d+)\s+of\s+(\d+)\s+lines\)/){ # lines
		print "##teamcity[buildStatisticValue key='CodeCoverageL' value='$1']\n";
		print "##teamcity[buildStatisticValue key='CodeCoverageAbsLCovered' value='$2']\n";
		print "##teamcity[buildStatisticValue key='CodeCoverageAbsLTotal' value='$3']\n";
	}
	if($line =~ m/functions\.*: ([\d.]+)%\s+\((\d+)\s+of\s+(\d+)\s+functions\)/){ # lines
		print "##teamcity[buildStatisticValue key='CodeCoverageM' value='$1']\n";
		print "##teamcity[buildStatisticValue key='CodeCoverageAbsMCovered' value='$2']\n";
		print "##teamcity[buildStatisticValue key='CodeCoverageAbsMTotal' value='$3']\n";
	}
}

