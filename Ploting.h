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
	int				iPixelNum;
	int				iGridNum;		// GridNum  of XGrid Unit 
	int				iGridScale;	// PixelNum  of XGrid Unit 
	double			dGridScale;
	double			dMinGrid;
	double			dMaxGrid;
	double			dMinValue;
	double			dMaxValue;

	AxisPara()
		:iPixelNum(0)
		,iGridNum(0)
		,iGridScale(0)
		,dGridScale(0.0)
		,dMinGrid(0.0)
		,dMaxGrid(0.0)
		,dMinValue(0.0)
		,dMaxValue(0.0){
	}
};

struct CoordPara{
	IplImage*		Figure;
	CvPoint2D64f	AxisOrigin;
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
		,iOffsetX(10)
		,iOffsetY(10)
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
		DrawFlow(plotPara);
		CvPoint2D64f OriPos = cvPoint2D64f(plotPara.iXOriPos,plotPara.iYOriPos);
		DrawCoord(Figure,plotPara.vPoint,OriPos,plotPara.iXLen,plotPara.iYLen);
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
		DrawFlow(plotPara);
	}

	void subplot(int col,int row,int sIdx,int eIdx=-1);
	void xlabel(string xlabel_name, CvScalar label_color);
	void ylabel(string ylabel_name, CvScalar label_color);
	void title(string title_name,CvScalar title_color); 
	void clear();
private:
	void CreatFigure(int iWidth=WINDOW_WIDTH,int iHeight=WINDOW_HEIGHT);


	void creatAxis (IplImage *image); 

	void DrawFlow (PlotPara para); //画点


	void DrawCoord(IplImage* Figure,std::vector<CvPoint2D64f>vPoint,CvPoint2D64f OriginPos,int iWidth,int iHeight);

	void drawXYZone();

	void drawAxisX();

	void drawAxisY();

	void calminmax(std::vector<CvPoint2D64f>vPoint);

	CvPoint2D64f getRealPoint(CvPoint2D64f point);

	CvPoint2D64f getPlotPoint(CvPoint2D64f point);

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