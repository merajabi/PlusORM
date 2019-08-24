#include <iostream>
#include <iomanip>
#include <sstream>

#include <cstdio>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "tools.h"
#include "posdebug.h"

void MyPosDebug(const char* format, ...){
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

std::string HexToStr( const std::vector<uint8_t>& in) {
	std::stringstream str;
	for(size_t i=0;i<in.size();i++)	{
		str << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(in[i]);
	}
	return str.str();
}

std::vector<uint8_t> StrToHex( const std::string& in) {
	std::vector<uint8_t> hex;
	std::string val;

	if(in.length()%2==0) {
		val=in;
	} else {
		val="0"+in;
	}

	for(size_t i=0; i < val.length(); i+=2){
		int num;
		std::stringstream(val.substr(i,2)) >> std::hex >> num;
		//std::cout << std::hex << "i: " << num << std::endl;
		hex.push_back(static_cast<uint8_t>(num));
	}
	return hex;
}
std::string HexString(const std::string& val){
	std::stringstream ss;
	for (size_t i=0; i< val.size(); i++){
		ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(static_cast<uint8_t>(val[i]));
	}
	return ss.str();
}

std::string FixedLenPadString(const std::string& val,int len){
	std::string str=std::string(len-val.size(),'0')+val;
	return str;
}

// Fixed len BCD
std::string PaddedFixedLenString(const std::string& val,int len, char ch){
	std::string str=std::string(len-val.size(),ch)+val;
	return str;
}
std::string PaddedFixedLenDecString(uint32_t val,int len){
	std::stringstream ss;
	ss << std::setfill('0') << std::setw(len) << val ;
	return ss.str();
}

std::string FixedLenDecString(uint32_t val,int len){
	std::stringstream ss;
	ss << std::setfill('0') << std::setw(len) << val ;
	return ss.str();
}
std::string AsciiToHex(const std::string& str){
	return HexString(str);
}
std::string HexToAscii(const std::string& str){
	std::stringstream ss;
	std::vector<uint8_t> binary=StrToHex(str);
	for(size_t i=0; i<binary.size();i++){
		ss << binary[i] ;
	}
	return ss.str();
}
std::string FixedLenHexString(uint32_t val,int len){
	std::string ss = FixedLenDecString(val,len/2);
	return HexString(ss);
}

