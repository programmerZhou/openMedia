// FlvDecode.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

#define HTON16(x)	((x>>8&0xff)|(x<<8&0xff00))
#define HTON24(x)	((x>>16&0xff)|(x<<16&0xff0000)|(x&0xff00))
#define HTON32(x)	((x>>24&0xff)|(x>>8&0xff00)| \
					 (x<<8&0xff0000)|(x<<24&0xff000000))


bool Read8(int &i8, FILE* fp){

	if(fread(&i8, 1, 1, fp) != 1)
		return false;
	
	return true;
}

bool Read16(int &i16, FILE* fp){

	if(fread(&i16, 2, 1, fp) != 1)
		return false;

	i16 = HTON16(i16);

	return true;
}

bool Read24(int &i24, FILE* fp){

	if(fread(&i24, 3, 1, fp) != 1)
		return false;

	i24 = HTON24(i24);
	
	return true;
}
bool Read32(int &i32, FILE* fp){
	
	if(fread(&i32, 4, 1, fp) != 1)
		return false;

	i32 = HTON32(i32);

	return true;
}
bool ReadTime(int &itime, FILE* fp){
	
	int tmp = 0;

	if(fread(&tmp, 4, 1, fp) != 1)
		return false;

	itime = HTON24(tmp);

	itime |= (tmp&0xff000000);

	return true;
}
bool Peek8(int &i8, FILE* fp);


struct FlvAdts{

	unsigned char SampleIndex1:3;
	unsigned char ObjectType:5;
	unsigned char other:3;
	unsigned char channel:4;
	unsigned char SampleIndex2:1;
};

struct AacAdts{

	unsigned char check1;
	
	unsigned char protection:1;
	unsigned char layer:2;
	unsigned char ver:1;
	unsigned char check2:4;

	unsigned char channel1:1;
	unsigned char privateStream:1;
	unsigned char SampleIndex:4;
	unsigned char ObjectType:2;

	unsigned char length1:2;
	unsigned char copyrightstart:1;
	unsigned char copyrightstream:1;
	unsigned char home:1;
	unsigned char originality:1;
	unsigned char channel2:2;

	unsigned char length2;

	unsigned char check3:5;
	unsigned char length3:3;

	unsigned char frames:2;
	unsigned char check4:6;
};


bool init();
void clear();
bool ReadHeader();
bool ReadBody();

int nClean = 0;
int nSampleIndex = 0;
AacAdts m_AacAdts = {0};
FlvAdts m_FlvAdts = {0};
void AacDecode(int len);

char FlvName[256] = {0};
char AacName[256] = {0};
FILE* flvFp;
FILE* aacFp;


int main(int argc, char* argv[])
{
	printf("Hello World!\n");

	if(argc > 2){
		sprintf(FlvName, "%s.flv", argv[1]);
		sprintf(AacName, "%s.aac", argv[1]);
	}else{
		strcpy(FlvName, "D:\\github\\openMedia\\projects\\windows\\flv\\FlvDecode\\test.flv");
		strcpy(AacName, "test.aac");
	}

	init();

	ReadHeader();

	ReadBody();

	cout << "read over" << endl;

	clear();

	return 0;
}

bool init(){

	flvFp = fopen(FlvName, "rb");
	if(flvFp == NULL){
		cout << "open flv file error" << endl;
		return false;
	}
	aacFp = fopen(AacName, "wb");

	fseek(flvFp, 0, SEEK_SET);

	return true;
}

void clear(){

	if(flvFp){
		fclose(flvFp);
	}

	if(aacFp){
		fclose(aacFp);
	}
}

bool ReadHeader(){

	int length = 0;
	int type = 0;

	if(Read24(type, flvFp) == false)
		return false;

	if(type != 'FLV'){
		cout << "it is not flv file" << endl;
		return false;
	}
	
	cout << "it is flv file." << endl;

	fseek(flvFp, 2, SEEK_CUR);

	if(Read32(length, flvFp) == false )
		return false;

	cout << "the length of flv header is " << length << endl;

	// jump header
	fseek(flvFp, length + 4, SEEK_SET);

	return true;
}

bool ReadBody(){

	int count = 0;

	while(true){
	
		int type = 0;
		int time = 0;
		int htime = 0;
		int dateLen = 0;
		int info = 0;
		char buffer[256] = {0};

		if(!Read8(type, flvFp))
			break;

		if(!Read24(dateLen, flvFp))
			break;

		if(!ReadTime(time, flvFp))
			break;

		// jump stream id
		fseek(flvFp, 3, SEEK_CUR);
		int pos = ftell(flvFp);

		if(type == 8){
			count++;
			//cout << "it is audio " << endl;
			AacDecode(dateLen);
		}else if(type == 9){
			count++;
		}

		fseek(flvFp, pos + dateLen + 4, SEEK_SET);

	}

	cout << "have media count is " << count << endl;
	return true;
}

bool hasSpecificConfig = false;

void AacDecode(int len){

	int info = 0;
	int type = 0;

	Read8(info, flvFp);
	Read8(type, flvFp);

	if(type == 0x00){
	
		hasSpecificConfig = true;
		fread(&m_FlvAdts, 1, sizeof(AacAdts), flvFp);

		m_AacAdts.check1 = 0xff;
		m_AacAdts.check2 = 0xff;
		m_AacAdts.check3 = 0xff;
		m_AacAdts.check4 = 0xff;
		m_AacAdts.protection = 1;
		m_AacAdts.ObjectType = 0;
		m_AacAdts.SampleIndex = m_FlvAdts.SampleIndex2|m_FlvAdts.SampleIndex1<<1;
		m_AacAdts.channel2 = m_FlvAdts.channel;
		return;
	}else{
	
		if(hasSpecificConfig){
		
			
		}
	}
}