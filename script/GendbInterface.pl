#!/usr/bin/perl -w

use Data::Dumper;

my $classname;
my %varString;
my %varTypes;
my @vars;
my $count=0;
my $featureStr;
my $hashStr;
my $assignStr;
my $constructParamStr;
my $constructAssignStr;
my $getMapStr;
my $setMapStr;
my $accessMethodStr;
my $codeStr;

while (my $line = <STDIN>) {
	chop($line);
	if( $line =~ m/^\s*CREATE\s+TABLE\s+IF\s+NOT\s+EXISTS\s+`(\w+)`\s*\(\s*$/ ){
		$classname=$1;
	}
	if ($line =~ m/^\s*`(\w+)`([\w\x{20}\.\+\-\)\(]+(?:\s".*")?[\w\x{20}\.\+\-\)\(]*)\,?\s*$/){
		my $var=$1;
		$vars[$count++]=$var;
		#print '1:'.$1;
		#print '=>2:'.$2;
		#print "--\n";
		$varString{$var}=$2;
		if($varString{$var} =~ m/INT/i ){
			# INT, INTEGER, TINYINT, SMALLINT, MEDIUMINT, BIGINT, UNSIGNED BIG INT, INT2, INT8 
			$varTypes{$var} = 'long';
		}elsif ($varString{$var} =~ m/CHAR|TEXT|CLOB/i ){
			# CHARACTER(20), VARCHAR(255), VARYING CHARACTER(255), NCHAR(55), NATIVE CHARACTER(70), NVARCHAR(100), TEXT, CLOB,
			$varTypes{$var} = 'std::string';
		}elsif ($varString{$var} =~ m/REAL|FLOA|DOUB/i ){
			#REAL, DOUBLE, DOUBLE PRECISION, FLOAT, 
			$varTypes{$var} = 'double';
		}else {
			# NUMERIC, DECIMAL(10,5), BOOLEAN, DATE, DATETIME,
			$varTypes{$var} = 'std::string';
		}

	}
};

#print Dumper $classname;
#print Dumper \%varString;
#my @vars= sort{$a cmp $b} keys %varTypes;

$featureStr="";
foreach my $var (@vars){
	if($var ne "id"){
		$featureStr.="".$varTypes{$var}." ".$var.";";
	}
}
#print Dumper $featureStr;

$hashStr="";
foreach my $var (@vars){
	$hashStr.="".'hashmap["'.$var.'"]= " '.$varString{$var}." \";";
}
#print Dumper $hashStr;

$assignStr="";
foreach my $var (@vars){
	if($var ne "id"){
		$assignStr.=""."$var=x.$var;";
	}
}
#print Dumper $assignStr;

$constructParamStr="";
$constructAssignStr="";
foreach my $var (@vars){
	if($constructParamStr){
		$constructParamStr.=",";
	}
	if($var ne "id"){
		$constructParamStr.=$varTypes{$var}." ".$var."_";
		$constructAssignStr.="$var=$var"."_;";
	}
}
$getMapStr="char str[128];";
foreach my $var (@vars){
	if($varTypes{$var} =~ m/std::string/){
		$getMapStr.="".'hashmap["'.$var.'"]='.$var.";";
	}elsif($varTypes{$var} =~ m/long/){
		$getMapStr.="".'sprintf(str,"%ld",'.$var.");";
		$getMapStr.="".'hashmap["'.$var."\"]=str;";
	}elsif($varTypes{$var} =~ m/double/){
		$getMapStr.="".'sprintf(str,"%lf",'.$var.");";
		$getMapStr.="".'hashmap["'.$var."\"]=str;";
	}
}
#print Dumper $getMapStr;


$setMapStr="";
foreach my $var (@vars){
	if($varTypes{$var} =~ m/std::string/){
		$setMapStr.="".$var.' = hashmap["'.$var.'"]'.";";
	}elsif($varTypes{$var} =~ m/long/){
		$setMapStr.="".$var.' = atol(hashmap["'.$var."\"].c_str());";
	}elsif($varTypes{$var} =~ m/double/){
		$setMapStr.="".$var.' = atof(hashmap["'.$var."\"].c_str());";
	}
}
#print Dumper $setMapStr;

$accessMethodStr="";
foreach my $var (@vars){
	if($var ne "id"){
		$accessMethodStr .= "\t\t"."$varTypes{$var} Get$var() const {return $var;}\n";
		$accessMethodStr .= "\t\t"."void Set$var($varTypes{$var} variable_) {$var=variable_;}\n";
	}
}
#print Dumper $accessMethodStr;

$codeStr = <<"END";
#ifndef _$classname\_H_
#define _$classname\_H_
#include <stdlib.h>
#include "inc/orm.h"
namespace PlusORM {
	class $classname :public ObjectMap {
		private:
		$featureStr
		public:
		inline static void Initialize(unsigned long maxid){ObjectMap::Initialize(GetTableName(),maxid);}
		inline static std::string GetTableName() { return "$classname";	};
		inline static std::string GetPrimaryKeyString() { return "id"; }
		inline static void CreateTable(std::map<std::string,std::string> &hashmap){$hashStr}
		$classname():ObjectMap(GetTableName()) {}
		$classname($constructParamStr):ObjectMap(GetTableName()){$constructAssignStr}
		$classname(std::map<std::string,std::string> &hashmap ):ObjectMap(GetTableName()){SetMap(hashmap);}
		$classname($classname& x):ObjectMap(x){$assignStr}
		$classname& operator= ($classname& x) {ObjectMap::operator=(x);$assignStr return *this;}
		inline void SetMap(std::map<std::string,std::string> &hashmap ){$setMapStr}
		inline void GetMap(std::map<std::string,std::string> &hashmap ) const {$getMapStr}
		unsigned long GetId() const {return id;}
		void SetId(unsigned long variable_) {id=variable_;}
$accessMethodStr
	};
};
#endif //_$classname\_H_

END

print $codeStr;
