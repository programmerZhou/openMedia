// WavDecode.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stdafx.h"

#include <windows.h>
#include <mmsystem.h>

#include <iostream>
using namespace std;
#pragma comment(lib, "winmm.lib")



/* 定时器
void WINAPI onTimeFunc(UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2)
{
    printf("time out\n");
    return;

}

int main(int argc, char* argv[])
{
	MMRESULT timer_id;
    int n = 0;
    timer_id = timeSetEvent(5000, 1, (LPTIMECALLBACK)onTimeFunc, DWORD(1), TIME_PERIODIC);
    if(NULL == timer_id)
    {
        printf("timeSetEvent() failed with error %d\n", GetLastError());
        return 0;
    }
    while(n<20)
    {
        printf("Hello World\n");
        Sleep(2000);
        n++;
    }
    timeKillEvent(timer_id);        //释放定时器
    return 1;
}

*/

#define MAX_BUFF_SOUNDSIZE	176400

DWORD PlayThread(DWORD wPara);

void init();

void clear();

DWORD CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, 
						   DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);

WAVEFORMATEX m_SoundFormat;
HWAVEOUT	 m_hWaveOut;
WAVEHDR		 m_pWaveHdrOut[3];
CHAR		 m_cBufferOut[3][MAX_BUFF_SOUNDSIZE];

int m_iPlayTimer = 0;
FILE* fp;
CHAR FileName[256] = {0};


struct WaveFormat{

	WORD	wFormatTag;		// format type
	WORD	nChannels;
	DWORD	nSamplesPerSec;
	DWORD	nAvgBytesPerSec;
	WORD	nBlockAlign;
	WORD	wBitsPerSample;
};

struct RiffHeader{
	
	char	szRiffID[4];	// 'R' 'I' 'F' 'F'
	DWORD	dwRiffSize;		// wav file length
	char	szRiffFormat[4];// 'W' 'A' 'V' 'E'
};


struct FormatBlock{
	
	char	szFormatID[4];	// 'f' 'm' 't' ' '
	DWORD	dwFormatSize;
	WaveFormat wavFormat;
};

struct DataBlock{

	char	szDataID[4];	// 'd' 'a' 't' 'a'
	DWORD	dwDataSize;
};




int main(int argc, char** argv){


	if(argc > 2){
		strcpy(FileName, argv[1]);
	}else{
		strcpy(FileName, "D:\\github\\openMedia\\openMedia\\projects\\windows\\wav\\WavDecode\\Debug\\test.wav");
	}

	init();

	HANDLE hd = CreateThread(NULL, NULL, 
		(LPTHREAD_START_ROUTINE)PlayThread, NULL, NULL, NULL);

	WaitForSingleObject(hd, INFINITE);

	clear();

	printf("play \n");
	system("PAUSE");

	return 0;
}


DWORD PlayThread(DWORD wParam){

	printf("play thread \n");
	Sleep((m_iPlayTimer+2)*1000);
	return 0;
}

DWORD CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, 
						   DWORD dwInstance, DWORD dwParam1, DWORD dwParam2){

	if( uMsg == WOM_DONE ){
	
		WAVEHDR* p = (WAVEHDR*)dwParam1;

		int i = p->dwUser;

		waveOutUnprepareHeader(m_hWaveOut, p, sizeof(WAVEHDR));

		int n = fread(&m_cBufferOut[i], 1, MAX_BUFF_SOUNDSIZE, fp);

		if( n < MAX_BUFF_SOUNDSIZE ){
			
			cout << "play over" << endl;
			return 0;
		}

		p->lpData = m_cBufferOut[i];
		p->dwBufferLength = n;
		p->dwBytesRecorded = 0;
		p->dwUser = i;
		p->dwFlags = 0;

		waveOutPrepareHeader(m_hWaveOut, p, sizeof(WAVEHDR));
		waveOutWrite(m_hWaveOut, p, sizeof(WAVEHDR));
	}

	if( uMsg == WOM_OPEN ){
		cout << "open" << endl;
	}

	if( uMsg == WOM_CLOSE ){
		cout << "close" << endl;
	}
	
	return 0;
}


void init(){

	fp = fopen(FileName, "rb");

	if(fp == NULL){
		cout << "open file error"  << endl;
		exit(0);
	}

	RiffHeader mRiff = {0};
	fread(&mRiff, 1, sizeof(RiffHeader), fp);

	cout << endl;
	cout << "riff id is " << mRiff.szRiffID << endl;
	cout << "riff len is " << mRiff.dwRiffSize << endl;
	cout << "riff format is " << mRiff.szRiffFormat << endl;

	FormatBlock mFmtBlock = {0};
	fread(&mFmtBlock, 1, sizeof(FormatBlock), fp);
 
	cout << endl;
	cout << "format block format id is " << mFmtBlock.szFormatID << endl;
	cout << "format block size is " << mFmtBlock.dwFormatSize << endl;

	if(mFmtBlock.dwFormatSize == 18){
		fseek(fp, 2, SEEK_CUR);
	}

	DataBlock mDataBlock = {0};
	fread(&mDataBlock, 1, sizeof(DataBlock), fp);

	cout << endl;
	cout << "data block id is " << mDataBlock.szDataID << endl;
	cout << "data block len is " << mDataBlock.dwDataSize << endl;

	WAVEFORMATEX mWaveFormat = {0};
	memset(&mWaveFormat, 0, sizeof(WaveFormat));
	mWaveFormat = *(WAVEFORMATEX*)&mFmtBlock.wavFormat;

	cout << endl;
	cout << "wave format tag " << mWaveFormat.wFormatTag << endl;
	cout << "wave format channel " << mWaveFormat.nChannels << endl;
	cout << "wave format sample rate " << mWaveFormat.nSamplesPerSec << endl;
	cout << "wave format bit " << mWaveFormat.wBitsPerSample << endl;
	cout << "wave format avg is " << mWaveFormat.nAvgBytesPerSec << endl;

	m_iPlayTimer = mDataBlock.dwDataSize / mWaveFormat.nAvgBytesPerSec;
	cout << "wave play timer is " << m_iPlayTimer << endl;


	MMRESULT res;

	res = waveOutOpen(&m_hWaveOut, 0, &mWaveFormat, (DWORD)(waveOutProc), 0, CALLBACK_FUNCTION); 
	if(res != 0){
		cout << "open wave out error " << endl;
		exit(0);
	}

	int i;

	for( i = 0; i < 3; i++){
		
		m_pWaveHdrOut[i].lpData = m_cBufferOut[i];
		m_pWaveHdrOut[i].dwBufferLength = MAX_BUFF_SOUNDSIZE;
		m_pWaveHdrOut[i].dwBytesRecorded = 0;
		m_pWaveHdrOut[i].dwUser = i;
		m_pWaveHdrOut[i].dwFlags = 0;
	
		fread(&m_cBufferOut[i], 1, MAX_BUFF_SOUNDSIZE, fp);

		waveOutPrepareHeader(m_hWaveOut, &m_pWaveHdrOut[i], sizeof(WAVEHDR));
		waveOutWrite(m_hWaveOut, &m_pWaveHdrOut[i], sizeof(WAVEHDR));
	
	}



	return;
}

void clear(){

	return;
}