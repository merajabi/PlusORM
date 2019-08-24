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

#if defined(HWX86QT)
	#include <QDebug>
#elif defined(HWAMP7000)
#elif defined(HWPAXS800)
#endif

#undef LMT
//#define LMT	"444"

const long debugBufferSize = 2048;
#ifdef POS_DEBUG
static int pipeFD = -1;
#endif //POS_DEBUG

int OpenLog(){
#ifdef POS_DEBUG
	#ifdef HWPAXS800
		char* posfifo = "poslog";
		mkfifo(posfifo, 0666);
		pipeFD = open(posfifo,O_WRONLY);
	#endif //PIPEDEBUG
#endif //POS_DEBUG
	return 0;
}

int CloseLog(){
#ifdef POS_DEBUG
	#ifdef HWPAXS800
		close(pipeFD);
		pipeFD = -1;
	#endif //PIPEDEBUG
#endif //POS_DEBUG
    return 0;
}

void MyPosDebug(const char* format, ...){
#ifdef POS_DEBUG
	va_list ap;
	char buffer[debugBufferSize] = {0};

	memset( buffer, 0, sizeof(buffer));
	buffer[0] = '\r';
	va_start(ap, format);
	vsnprintf (buffer + 1, debugBufferSize, format, ap);
	va_end(ap);
	int len = strlen(buffer);
	if(buffer[len-1] != '\n'){
		buffer[len] = '\n';
	}

	#ifdef LMT
		char lmtStr[100] = {0};
		char lmtBuffer[10] = {0};

		char* sStr = 0;
		int flag = 0;
		int i = 0;

		strcpy(lmtStr, LMT);

		if( (buffer+1)[i++] == '#' ){
			int j = 0;
			while( isdigit( (buffer+1)[i] ) ){
				lmtBuffer[j++] = (buffer+1)[i++] ;
			}
		}
		sStr = strtok(lmtStr, ",");
		while (sStr != NULL) {
			if( strcmp(sStr, lmtBuffer) == 0 ){
				flag = 1;
				break;
			}
			sStr = strtok (NULL, ",");
		}

		if(flag == 1){
	#endif

			#if defined(HWX86QT)
				qDebug("%s", buffer);
			#elif defined(HWAMP7000)
				fprintf(stderr,"%s", buffer);
				fflush(stderr);
			#elif defined(HWPAXS800)
				write(pipeFD, buffer, len+1);
			#else
				std::cerr<<buffer;//<<std::endl;
			#endif

	#ifdef LMT
		}
	#endif//LMT

#endif //POS_DEBUG
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
std::string HexToStr( const uint8_t* in, size_t len) {
    std::stringstream str;
    for(size_t i=0;i<len;i++)	{
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
std::string HexString(const std::string& val){
	std::stringstream ss;
	for (size_t i=0; i< val.size(); i++){
		ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(static_cast<uint8_t>(val[i]));
	}
	return ss.str();
}
std::string FixedLenHexString(uint32_t val,int len){
	std::string ss = FixedLenDecString(val,len/2);
	return HexString(ss);
}

std::vector<uint8_t> GenerateTLVVector(const uint8_t tag, int len,  const std::string& value, int type) {
	std::vector<uint8_t> tlv;
	intUnion lenstr;//lenstr.resize(2*len+1);
	if(type == 0 ){
		std::vector<uint8_t> tlv2 = StrToHex(value);
		lenstr.i16[0] = tlv2.size();
		tlv.push_back(tag);
		for(int i=len-1; i>=0 ; i--){
			tlv.push_back(lenstr.i8[i]);
		}
		tlv.insert(tlv.end(),tlv2.begin(),tlv2.end());
	}else if( type == 1 ){
		lenstr.i16[0] = value.size();
		tlv.push_back(tag);
		for(int i=len-1; i>=0 ; i--){
			tlv.push_back(lenstr.i8[i]);
		}
		tlv.insert(tlv.end(),value.begin(),value.end());
	}
	return tlv;
}


std::string GenerateTLVBCD(const std::string& tag, int len,  const std::string& value){
	std::string paddedValue;
	char *lenstr = new char[2*len+1];//2*len+1
	if(value.length()%2==0) {
		paddedValue=value;
	} else {
		paddedValue="0"+value;
	}
	sprintf(lenstr,"%0*lX",2*len,paddedValue.length()/2);
	std::string tlv = tag+std::string(lenstr)+paddedValue;
	delete[] lenstr;
	return tlv;
}

/*
std::string toString(long i){
	char str[128];
	posDebug("********            toString(long) \n");
	sprintf(str,"%ld",i);
	return str;
}
std::string toString(double i){
	char str[128];
	posDebug("********           toString(double) \n");
	sprintf(str,"%.lf",i);
	return str;
}
*/


int strBcdToAsc(int ilen, unsigned char *Sbcd, unsigned char *_DStr)
{
	unsigned char tmp;
	unsigned char mask = 0x30;
	unsigned char mask2 = 0x0f;
	int i;

	unsigned char DStr[100];
	// ilen is number of bcd digits (in a byte we have 2 bcd digits)

	for (i = 0; i < (ilen / 2 + ilen % 2); i++)
	{
		tmp = Sbcd[i];
		DStr[i * 2] = Sbcd[i];
		DStr[i * 2] >>= 4;
		DStr[i * 2] = DStr[i * 2] | mask;
		tmp = tmp & mask2;
		DStr[i * 2 + 1] = tmp | mask;

	}
	DStr[i * 2] = '\0';
	if (ilen % 2 == 0)
		memcpy((char *)_DStr, DStr, strlen((char *)DStr));
	else
		memcpy((char *)_DStr, DStr + 1, (strlen((char *)DStr) - 1));

	return strlen((char *)DStr);
}

void HexToStrChar( const uint8_t *in, int len, char *out)	{
	for(int i=0;i<len;i++)	{
		sprintf(out+i*2,"%02X",in[i]);
	}
}
int StrToHex( const char *in, int len, uint8_t *out)	{
	int i,j;
	char temp[3];
	if(len%2!=0){
		return 0;
	}
	for (i = 0, j=0; i < len; i+=2, j++){
		temp[0]=in[i];
		temp[1]=in[i+1];
		temp[2]=0;
		out[j] = strtol(temp, NULL, 16);
	}
	return j;
}

std::string CenterString(std::string input, int lengthChar, bool allignLeft){
    int paddLen = lengthChar - input.length();
    if (paddLen < 0){
        return std::string(input);
    }
    int rightPaddLen = paddLen / 2;
    int leftPaddLen = paddLen / 2;

    if (paddLen % 2 != 0){
        if (allignLeft){
            rightPaddLen += 1;
        }
        else{
            leftPaddLen += 1;
        }
    }
    return std::string(leftPaddLen, ' ') + input + std::string(rightPaddLen, ' ');
}

std::string GetSeparatedNumbers(const std::string& text, int sepSize, const std::string &sepText ){
	std::string str;
    size_t i=text.length() % sepSize;
     i=i ? i:sepSize;
	str=text.substr(0,i);
    for( ; i<text.length() ; i+=sepSize) {
        str+= sepText + text.substr(i,sepSize);
	}
	return str;
}

std::string GetMaskedPAN(const std::string& pan ){
    return pan.substr(0,6) + "***" + pan.substr(pan.size() - 4,4);
}

std::string MiladiToShamsi(const std::string& miladiDate) {
	std::string shamsiDate;
	int sumDayMiladiMonth[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
	int sumDayMiladiMonthLeap[] = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 };

	int iMiladiMonth, iMiladiDay, iMiladiYear;
	int shamsiDay, shamsiMonth, shamsiYear;
	int dayCount, farvardinDayDiff, deyDayDiff;

	iMiladiYear = atoi(miladiDate.substr(0,4).c_str());
	iMiladiMonth = atoi(miladiDate.substr(4,2).c_str());
	iMiladiDay = atoi(miladiDate.substr(6,2).c_str());

	farvardinDayDiff = 79;

	if (MiladiIsLeap(iMiladiYear)) {
		dayCount = sumDayMiladiMonthLeap[iMiladiMonth - 1] + iMiladiDay;
	}
	else {
		dayCount = sumDayMiladiMonth[iMiladiMonth - 1] + iMiladiDay;
	}

	if (MiladiIsLeap(iMiladiYear - 1)) {
		deyDayDiff = 11;
	}
	else {
		deyDayDiff = 10;
	}

	if (dayCount > farvardinDayDiff) {
		dayCount = dayCount - farvardinDayDiff;
		if (dayCount <= 186) {
			switch (dayCount % 31)
			{
			case 0:
				shamsiMonth = dayCount / 31;
				shamsiDay = 31;
				break;

			default:
				shamsiMonth = (dayCount / 31) + 1;
				shamsiDay = (dayCount % 31);
				break;
			}
			shamsiYear = iMiladiYear - 621;
		}
		else {
			dayCount = dayCount - 186;
			switch (dayCount % 30)
			{
			case 0:
				shamsiMonth = (dayCount / 30) + 6;
				shamsiDay = 30;
				break;

			default:
				shamsiMonth = (dayCount / 30) + 7;
				shamsiDay = (dayCount % 30);
				break;
			}
			shamsiYear = iMiladiYear - 621;
		}
	}
	else {
		dayCount = dayCount + deyDayDiff;

		switch (dayCount % 30)
		{
		case 0:
			shamsiMonth = (dayCount / 30) + 9;
			shamsiDay = 30;
			break;

		default:
			shamsiMonth = (dayCount / 30) + 10;
			shamsiDay = (dayCount % 30);
			break;
		}
		shamsiYear = iMiladiYear - 622;

	}
	shamsiDate = toString(shamsiYear)+ PaddedFixedLenDecString(shamsiMonth,2) + PaddedFixedLenDecString(shamsiDay,2);
	return shamsiDate;
}

bool MiladiIsLeap(int miladiYear) {
	if (((miladiYear % 100) != 0 && (miladiYear % 4) == 0) || ((miladiYear % 100) == 0 && (miladiYear % 400) == 0)) {
		return true;
	} else {
		return false;
	}
}
std::string GetPanFromTr2(const std::string tr2) {
    return tr2.substr(0,tr2.find('='));
}
std::string ConvertMiliLiterToLiter(const std::string ml) {
	char temp[64];
	sprintf(temp,"%0.2f",atof(ml.c_str())/100.0);
    return std::string(temp);
}
