
class PackedBCD {
	char* asciiStr;
	unsigned char* pbcdStr;
	int asclen,bcdlen;

	// assume source string contains chars (numbers) in range 0 ~ 9 - len is srclen
	int ASCII2PackedBCD(char *src, unsigned char *dst, int len);

	// Implicitly assumed Packed BCd contains number in range 0 ~ 9 - len is srclen
	int PackedBCD2ASCII(unsigned char *src, char *dst,int len);

	void Delete();
public:
	PackedBCD(char *src);
	PackedBCD(unsigned char *src);
	~PackedBCD(){Delete();};
	char* operator=(char* src);
	unsigned char* operator=(unsigned char* src);
	char* toAscii();
	unsigned char* toPackedBCD();
	void Print();
};

	// **********************************************************************************
	//						PackedBCD Class
	// **********************************************************************************
	// assume source string contains chars (numbers) in range 0 ~ 9 - len is srclen
int PackedBCD::ASCII2PackedBCD(char *src, unsigned char *dst, int len) {
		char mask = 0x0f; // 0b00001111
		char* bcdStr;
		unsigned char* packedBcdStr;

		if ((len % 2) == 0){
			bcdStr=new char[len];
			memset(bcdStr, 0, len * sizeof(char));
			memcpy(bcdStr, src, len);
		} else {
			len++;
			bcdStr=new char[len];
			memset(bcdStr, 0, len * sizeof(char));
			//because it has even index higher order bits 0011 will be romoved
//			bcdStr[0] = 0x30; // 0b00110000 its ASCII code for char '0'
			bcdStr[0] = 0x3A; // 0b00111010 its ASCII code for char ':'
			memcpy(bcdStr + 1, src, len-1);
		}

		packedBcdStr=new unsigned char[len];
		memset(packedBcdStr, 0, len * sizeof(unsigned char));

		for (int i = 0; i <= len; i++) { //generate BCD
			packedBcdStr[i] = bcdStr[i] & mask; //0b00001111
			if ((i % 2) == 0) // generate low nible high nible
				packedBcdStr[i]= packedBcdStr[i] << 4;
		}

		for (int i = 0; i < (len / 2); i++)	{ // pack two parts
			dst[i] = (packedBcdStr[i * 2] | packedBcdStr[i * 2 + 1]);
		}
		delete bcdStr;
		delete packedBcdStr;
		return len / 2;
	}

	// Implicitly assumed Packed BCd contains number in range 0 ~ 9 - len is srclen
int PackedBCD::PackedBCD2ASCII(unsigned char *src, char *dst,int len) {
		char zero = 0x30;  // 0b00110000 its ASCII code for char '0'
		char mask = 0x0f; // 0b00001111

		char* str = new char [2*len];
		memset(str, 0, 2*len * sizeof(char));

		// len is number of bcd digits (in a byte we have 2 bcd digits)
		for (int i = 0; i < len; i++)	{
			str[i * 2]     = ( src[i] >> 4   ) | zero;
			str[i * 2 + 1] = ( src[i] & mask ) | zero;
		}

		if(str[0] != 0x3A) { // 0b00111010 its ASCII code for char ':'
//		if (len % 2 == 0){
			len=2*len;
			memcpy(dst, str, len * sizeof(char) );

		} else {
			len=2*len-1;
			memcpy(dst, str + 1, len * sizeof(char) );
		}

		dst[len]=0;
		delete str;
		return len;
	}
void PackedBCD::Delete(){
	if(pbcdStr)
		delete pbcdStr;
	if(asciiStr)
		delete asciiStr;
	pbcdStr=NULL;
	asciiStr=NULL;
	asclen=0;
	bcdlen=0;
}

PackedBCD::PackedBCD(char *src){
	pbcdStr=NULL;
	bcdlen=0;
	asclen=strlen(src);
	asciiStr = new char[asclen];
	memcpy(asciiStr,src,asclen* sizeof(char));
}
PackedBCD::PackedBCD(unsigned char *src){
	asciiStr=NULL;
	asclen=0;
	bcdlen=strlen((char*)src);
	pbcdStr = new unsigned char[bcdlen];
	memcpy(pbcdStr,src,bcdlen* sizeof(unsigned char));
}
char* PackedBCD::operator=(char* src){
	Delete();
	asclen=strlen(src);
	asciiStr = new char[asclen];
	memcpy(asciiStr,src,asclen* sizeof(char));
}
unsigned char* PackedBCD::operator=(unsigned char* src){
	Delete();
	bcdlen=strlen((char*)src);
	pbcdStr = new unsigned char[bcdlen];
	memcpy(pbcdStr,src,bcdlen* sizeof(unsigned char));
}

char* PackedBCD::toAscii(){
	if(!asciiStr && pbcdStr){
		asciiStr = new char[bcdlen*2];
		asclen=PackedBCD2ASCII(pbcdStr,asciiStr,bcdlen);
	}
	return asciiStr;
}
unsigned char* PackedBCD::toPackedBCD(){
	if(!pbcdStr && asciiStr){
		pbcdStr = new unsigned char[asclen];
		bcdlen=ASCII2PackedBCD(asciiStr,pbcdStr,asclen);
	}
	return pbcdStr;
}
void PackedBCD::Print(){
	fprintf(stderr,"ASCII: %s len: %d -> BCD: ",asciiStr,asclen);
	for(int i=0; i<bcdlen;i++){
		fprintf(stderr,"%X ",pbcdStr[i]);
	}
	fprintf(stderr,"len: %d\n",bcdlen);
}

