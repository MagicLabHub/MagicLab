#ifndef _PLOTING_H
#define _PLOTING_H
#include"opencv2/opencv.hpp"
#include <cmath>
#include <cfloat>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 700
using namespace cv;
using namespace std;

struct FigureConfig{
	int				iRowNum;
	int				iColNum;
	int				iRegionIdx;
	int				iMergeRegion;
	int				iRegionHeight;
	int				iRegionWidth;

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
	int				iXOriPos;
	int				iYOriPos;
	int				iXLen;
	int				iYLen;
	int				iSleepTime;
	Scalar			color;
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

struct AxisPara{
	int				iGridStart;		// Start Idx of Grid Line 
	int				iGridNum;		// Number of Grid Line
	double			dAxisLenght;	// Pixel length of Axis
	double			dGridStep;		// Pixel length per Grid

	double			dValueStep;		// Data Value Scale per Grid 
	double			dMinValue;		// minValue of Data
	double			dMaxValue;		// maxValue of Data

	AxisPara()
		:dAxisLenght(0)
		,iGridStart(0)
		,iGridNum(0)
		,dGridStep(0.0)
		,dValueStep(0)
		,dMinValue(0.0)
		,dMaxValue(0.0){
	}
};

struct CoordPara{
	IplImage*		Figure;
	CvPoint2D64f	AxisOrigin;
	CvPoint2D64f	DataOrigin;
	int				iDataNum;
	int				iWidth;
	int				iHeight;
	int				iOffsetX;
	int				iOffsetY;
	AxisPara		XAxis;
	AxisPara		YAxis;
	CoordPara()
		:iDataNum(0)
		,iWidth(0)
		,iHeight(0)
		,iOffsetX(30)
		,iOffsetY(20)
	{}
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
		plotPara.iXLen		= m_figureConfig.iRegionWidth*m_figureConfig.iMergeRegion;
		plotPara.iYLen		= m_figureConfig.iRegionHeight;
		plotPara.iXOriPos	= (m_figureConfig.iRegionIdx-1)%m_figureConfig.iRowNum*m_figureConfig.iRegionWidth;
		plotPara.iYOriPos	= (m_figureConfig.iRegionIdx-1)/m_figureConfig.iRowNum*m_figureConfig.iRegionHeight;
		for (int i=0;i<Cnt;++i){
			plotPara.vPoint.push_back(cvPoint2D64f((double)i, (double)y[i]));
		}
		CvPoint2D64f OriPos = cvPoint2D64f(plotPara.iXOriPos,plotPara.iYOriPos);
		DrawCoord(Figure,plotPara.vPoint,OriPos,plotPara.iXLen,plotPara.iYLen);
		DrawFlow(plotPara.vPoint,plotPara.color);
	}

	template<class T>
	void plot(T *x, T *y, size_t Cnt, CvScalar color){
		PlotPara plotPara;
		plotPara.color		= color;
		plotPara.iXLen		= m_figureConfig.iRegionWidth*m_figureConfig.iMergeRegion;
		plotPara.iYLen		= m_figureConfig.iRegionHeight;
		plotPara.iXOriPos	= (m_figureConfig.iRegionIdx-1)%m_figureConfig.iRowNum*m_figureConfig.iRegionWidth;
		plotPara.iYOriPos	= (m_figureConfig.iRegionIdx-1)/m_figureConfig.iRowNum*m_figureConfig.iRegionHeight;
		for (int i=0;i<Cnt;++i){
			plotPara.vPoint.push_back(cvPoint2D64f((double)x[i], (double)y[i]));
		}
		CvPoint2D64f OriPos = cvPoint2D64f(plotPara.iXOriPos,plotPara.iYOriPos);
		DrawCoord(Figure,plotPara.vPoint,OriPos,plotPara.iXLen,plotPara.iYLen);
		DrawFlow(plotPara.vPoint,plotPara.color);
	}

	void subplot(int col,int row,int sIdx,int eIdx=-1);
	void xlabel(string xlabel_name, CvScalar label_color);
	void ylabel(string ylabel_name, CvScalar label_color);
	void title(string title_name,CvScalar title_color); 
	void clear();
private:
	void CreatFigure(int iWidth=WINDOW_WIDTH,int iHeight=WINDOW_HEIGHT);


	void creatAxis(IplImage *image); 

	void DrawFlow(PlotPara para); //画点

	void DrawFlow(std::vector<CvPoint2D64f>vPoint,Scalar color);

	void DrawCoord(IplImage* Figure,std::vector<CvPoint2D64f>vPoint,CvPoint2D64f OriginPos,int iWidth,int iHeight);

	void drawXYZone();

	void drawAxisX();

	void drawAxisY();

	void calminmax(std::vector<CvPoint2D64f>vPoint);

	CvPoint getRealPoint(CvPoint2D64f point);

	CvPoint2D64f getPlotPoint(CvPoint point);

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

	CoordPara m_coordPara;
	FigureConfig m_figureConfig;
};




#endif _PLOTING_H