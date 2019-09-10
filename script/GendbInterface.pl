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
	if($var ne "Id"){
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
	if($var ne "Id"){
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
	if($var ne "Id"){
		$constructParamStr.=$varTypes{$var}." ".$var."_";
		$constructAssignStr.="Set$var($var"."_);";
	}
}
$getMapStr="";
foreach my $var (@vars){
	$getMapStr.="".'hashmap["'.$var.'"]=Get(GenKey("'.$var.'"));';
}
#print Dumper $getMapStr;


$setMapStr="";
foreach my $var (@vars){
	$setMapStr.='Set(GenKey("'.$var.'"), hashmap["'.$var.'"]'.");";
}
#print Dumper $setMapStr;

$accessMethodStr="";
foreach my $var (@vars){
	if($varTypes{$var} =~ m/std::string/){
		$accessMethodStr .= "\t\t"."$varTypes{$var} Get$var() const {return Get(GenKey(\"$var\"));}\n";
		$accessMethodStr .= "\t\t"."void Set$var($varTypes{$var} variable_) {Set(GenKey(\"$var\"),variable_);}\n";
	}elsif($varTypes{$var} =~ m/long/){
		$accessMethodStr .= "\t\t"."$varTypes{$var} Get$var() const {return atol(Get(GenKey(\"$var\")).c_str());}\n";
		$accessMethodStr .= "\t\t"."void Set$var($varTypes{$var} variable_) {Set(GenKey(\"$var\"),toString(variable_));}\n";
	}elsif($varTypes{$var} =~ m/double/){
		$accessMethodStr .= "\t\t"."$varTypes{$var} Get$var() const {return atof(Get(GenKey(\"$var\")).c_str());}\n";
		$accessMethodStr .= "\t\t"."void Set$var($varTypes{$var} variable_) {Set(GenKey(\"$var\"),toString(variable_));}\n";
	}
}
#print Dumper $accessMethodStr;
$accessMethodStaticStr="";
foreach my $var (@vars){
		if($varTypes{$var} =~ m/std::string/){
			$accessMethodStaticStr .= "\t\t"."static $varTypes{$var} Get$var(ObjectMap& x) {return x.Get(GenKey(\"$var\"));}\n";
			$accessMethodStaticStr .= "\t\t"."static $varTypes{$var} Get$var(ObjectMap* x) {return x->Get(GenKey(\"$var\"));}\n";
		}elsif($varTypes{$var} =~ m/long/){
			$accessMethodStaticStr .= "\t\t"."static $varTypes{$var} Get$var(ObjectMap& x) {return atol(x.Get(GenKey(\"$var\")).c_str());}\n";
			$accessMethodStaticStr .= "\t\t"."static $varTypes{$var} Get$var(ObjectMap* x) {return atol(x->Get(GenKey(\"$var\")).c_str());}\n";
		}elsif($varTypes{$var} =~ m/double/){
			$accessMethodStaticStr .= "\t\t"."static $varTypes{$var} Get$var(ObjectMap& x) {return atof(x.Get(GenKey(\"$var\")).c_str());}\n";
			$accessMethodStaticStr .= "\t\t"."static $varTypes{$var} Get$var(ObjectMap* x) {return atof(x->Get(GenKey(\"$var\")).c_str());}\n";
		}
}

$codeStr = <<"END";
#ifndef _$classname\_H_
#define _$classname\_H_
#include <stdlib.h>
#include "inc/orm.h"
namespace PlusORM {
	class $classname :public ObjectMap {
		private:
		public:
		static void Initialize(){ORM* model=ORM::GetInstance(); std::map<std::string,std::string> hashmap; CreateTable(hashmap); model->Create(GetTableName(), hashmap); unsigned long maxid = model->MaxPrimaryKey(GetTableName(),GetPrimaryKey()); ObjectMap::Initialize(GetTableName(),maxid);}
		static std::string GetTableName() { return "$classname";	};
		static std::string GetPrimaryKey() { return "Id"; }
		static std::string GenKey(const std::string& key) {return (GetTableName()+"#"+key);}
		static std::string ExtKey(const std::string& key) {std::size_t found = key.find(GetTableName()+"#"); if(found!=std::string::npos){ return key.substr(found+GetTableName().size()+1);}	return key; }		

		static void CreateTable(std::map<std::string,std::string> &hashmap){$hashStr}
$accessMethodStaticStr

		$classname():ObjectMap(GetTableName()) {SetId(GetMaxId(GetTableName()));}
		$classname($constructParamStr):ObjectMap(GetTableName()){SetId(GetMaxId(GetTableName()));$constructAssignStr}
		$classname(std::map<std::string,std::string> &hashmap ):ObjectMap(hashmap){}
		$classname($classname& x):ObjectMap(x){}
		$classname(ObjectMap& x):ObjectMap(x){}
		$classname& operator= ($classname& x) {ObjectMap::operator=(x); return *this;}
		std::string GetTableNameString() const { return "$classname";	};
		std::string GetPrimaryKeyString() const { return "Id"; }
		std::string GetPrimaryValueString() const { return Get(GenKey("Id"));}
		void SetMap(std::map<std::string,std::string> &hashmap ){$setMapStr}
		void GetMap(std::map<std::string,std::string> &hashmap ) const {$getMapStr}
$accessMethodStr
	};
};
#endif //_$classname\_H_

END

print $codeStr;
