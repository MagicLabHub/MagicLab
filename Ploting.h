#ifndef _PLOTING_H
#define _PLOTING_H
#include"opencv2/opencv.hpp"
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
using namespace cv;
using namespace std;

struct FigureConfig{
	int iRowNum;
	int iColNum;
	int iRegionIdx;
	int iMergeRegion;
	int iRegionHeight;
	int iRegionWidth;

	FigureConfig()
		:iRowNum(1)
		,iColNum(1)
		,iRegionIdx(1)
		,iMergeRegion(1)
		,iRegionHeight(WINDOW_HEIGHT)
		,iRegionWidth(WINDOW_WIDTH){
	}
};

struct PlotPara{
	int iXOriPos;
	int iYOriPos;
	int iXLen;
	int iYLen;
	int iSleepTime;
	Scalar color;
	std::vector<CvPoint2D64f>vPoint;
	PlotPara()
		:iXOriPos(0)
		,iYOriPos(0)
		,iXLen(0)
		,iYLen(0)
		,iSleepTime(0)
	{

	}
	
};

class Ploting
{
public: 
	Ploting();
	~Ploting();
   

	template<class T>
	void plot(T *y, size_t Cnt, CvScalar color){
		PlotPara plotPara;
		plotPara.color		= color;
		plotPara.iXLen		= m_figConfig.iRegionWidth*m_figConfig.iMergeRegion;
		plotPara.iYLen		= m_figConfig.iRegionHeight;
		plotPara.iXOriPos	= (m_figConfig.iRegionIdx-1)%m_figConfig.iRowNum*m_figConfig.iRegionWidth;
		plotPara.iYOriPos	= (m_figConfig.iRegionIdx-1)/m_figConfig.iRowNum*m_figConfig.iRegionHeight;
		for (int i=0;i<Cnt;++i){
			plotPara.vPoint.push_back(cvPoint2D64f((double)i, (double)y[i]));
		}
		DrawFlow(plotPara);
	}

	template<class T>
	void plot(T *x, T *y, size_t Cnt, CvScalar color){
		PlotPara plotPara;
		plotPara.color		= color;
		plotPara.iXLen		= m_figConfig.iRegionWidth*m_figConfig.iMergeRegion;
		plotPara.iYLen		= m_figConfig.iRegionHeight;
		plotPara.iXOriPos	= (m_figConfig.iRegionIdx-1)%m_figConfig.iRowNum*m_figConfig.iRegionWidth;
		plotPara.iYOriPos	= (m_figConfig.iRegionIdx-1)/m_figConfig.iRowNum*m_figConfig.iRegionHeight;
		for (int i=0;i<Cnt;++i){
			plotPara.vPoint.push_back(cvPoint2D64f((double)x[i], (double)y[i]));
		}
		DrawFlow(plotPara);
	}

	void subplot(int col,int row,int sIdx,int eIdx=-1);
	void xlabel(string xlabel_name, CvScalar label_color);
	void ylabel(string ylabel_name, CvScalar label_color);
	void title(string title_name,CvScalar title_color); 
	void clear();
private:
	void CreatFigure(int iWidth=WINDOW_WIDTH,int iHeight=WINDOW_HEIGHT);
	void DrawAxis (IplImage *image); //画坐标轴
	void DrawFlow (PlotPara para); //画点
public:
	IplImage* Figure;
private:
    int m_iFigureHeight; //窗口大小
    int m_iFigureWidth;

    double y_max;
    double y_min;
    double y_scale;
    double x_max;
    double x_min;
    double x_scale;

	//color
	CvScalar backgroud_color;
	CvScalar axis_color;
	CvScalar text_color;
    //边界大小
    int border_size;

	FigureConfig m_figConfig;
};




#endif _PLOTING_H