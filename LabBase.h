#pragma once
#ifndef LAB_BASE
#define LAB_BASE 
#include <stdio.h> 
#include <fstream>
#include <windows.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <math.h>
#include <string>

class LabBase
{
public:
	LabBase(void);
	~LabBase(void);
};


class AlgoLogPerf
{
public:
	AlgoLogPerf(void)
	{
		m_vBegCounter.clear();
		m_vEndCounter.clear();
		m_vBegCounter.reserve(1000000);
		m_vEndCounter.reserve(1000000);
		QueryPerformanceFrequency(&m_TickFreq);
	}
	~AlgoLogPerf(void)
	{
		log2File();
	}
	void setLogStart()
	{
		QueryPerformanceCounter(&m_BegCounter);
		m_vBegCounter.push_back(m_BegCounter);
	}

	void getLogEnd()
	{
		QueryPerformanceCounter(&m_EndCounter);
		m_vEndCounter.push_back(m_EndCounter);
	}

private:
	void log2File(){
		int DataNum = m_vEndCounter.size();
		if(DataNum){
			DWORD ThreadID = GetCurrentThreadId();
			std::string sLogFile = "D:\\logfile_"+std::to_string((_Longlong)ThreadID)+".log";
			std::ofstream outfile(sLogFile,std::ios::binary|std::ios::app);
			outfile<<"Frequence: "<<std::to_string((_Longlong)m_TickFreq.QuadPart)
				<<"	Process Num:	"<<std::to_string((_Longlong)DataNum)<<'\r'<<'\n';
			for(int i=0;i<DataNum;++i){
				double dTime = (double)(m_vEndCounter[i].QuadPart - m_vBegCounter[i].QuadPart)/(double)m_TickFreq.QuadPart;
				//std::cout<<"Time Assume "<<dTime<<std::endl;
				outfile<<std::to_string((_Longlong)i)<<"		"<<std::to_string((long double)dTime)<<'\r'<<'\n';
				outfile.flush();
			}
			outfile.close();
		}
	}
private:

	LARGE_INTEGER m_TickFreq; 
	LARGE_INTEGER m_BegCounter;
	LARGE_INTEGER m_EndCounter;
	std::vector<LARGE_INTEGER> m_vBegCounter;
	std::vector<LARGE_INTEGER> m_vEndCounter;

};

#define LOGPERF(_ins,m,...) { _ins.##m(##__VA_ARGS__);}

#endif LAB_BASE