#include "Ploting.h"

Ploting::Ploting(void){
	CreatFigure(WINDOW_WIDTH,WINDOW_HEIGHT);
}

Ploting::~Ploting(void){
	cvReleaseImage( &(this->Figure) );
}

void Ploting::CreatFigure(int CanvaWidth,int CanvaHeight){
	this->m_iFigureHeight	= CanvaHeight;
	this->m_iFigureWidth	= CanvaWidth;
	this->Figure = cvCreateImage(cvSize(this->m_iFigureWidth,this->m_iFigureHeight),IPL_DEPTH_8U, 3);
	memset(Figure->imageData, 255, sizeof(unsigned char)*Figure->widthStep*Figure->height);
	//color
	this->backgroud_color = CV_RGB(255,255,255); //背景白色
}



void Ploting::DrawFlow (PlotPara para){
	int bs = 5;
	for(size_t i = 0; i < para.vPoint.size(); i++){
			if(para.vPoint[i].x < this->x_min){
				this->x_min = para.vPoint[i].x;
			}else if(para.vPoint[i].x > this->x_max){
				this->x_max = para.vPoint[i].x;
			}

			if(para.vPoint[i].y < this->y_min){
				this->y_min = para.vPoint[i].y;
			}else if(para.vPoint[i].y > this->y_max){
				this->y_max = para.vPoint[i].y;
			}

	}
	double x_range = this->x_max - this->x_min;
	double y_range = this->y_max - this->y_min;
	this->x_scale = (para.iXLen - bs*2)/ x_range;
	this->y_scale = (para.iYLen - bs*2)/ y_range;


	//清屏
	//memset(Figure->imageData, 255, sizeof(unsigned char)*Figure->widthStep*Figure->height);

	//printf("x_range: %f y_range: %f\n", x_range, y_range);
	//绘制点
	double tempX, tempY;
	CvPoint prev_point, current_point;
	std::vector<CvPoint>vTemp;
	//prev_point = cvPoint(para.iXOriPos + bs, para.iYOriPos + para.iYLen - bs);
	for(size_t i = 0; i < para.vPoint.size(); i++)
	{
		tempX = (int)((para.vPoint[i].x - this->x_min)*this->x_scale);
		tempY = (int)((para.vPoint[i].y - this->y_min)*this->y_scale);
		current_point = cvPoint(para.iXOriPos + bs + tempX, para.iYOriPos + para.iYLen - (tempY + bs));
		vTemp.push_back(current_point);
		// draw a line between two points
		if(i>0)
			cvLine(Figure, prev_point, current_point, para.color, 1, 8);
		prev_point = current_point;
		//cvWaitKey(para.iSleepTime);
	}
}

void Ploting::subplot(int col,int row,int sIdx,int eIdx){
	if (eIdx==-1) eIdx = sIdx;
	m_figureConfig.iColNum = col;
	m_figureConfig.iRowNum = row;
	m_figureConfig.iRegionHeight	= m_iFigureHeight/m_figureConfig.iColNum;
	m_figureConfig.iRegionWidth		= m_iFigureWidth/m_figureConfig.iRowNum;
	m_figureConfig.iRegionIdx		= sIdx;
	m_figureConfig.iMergeRegion		= eIdx-sIdx+1;
}


void Ploting::DrawCoord(
	IplImage* Figure,std::vector<CvPoint2D64f>vPoint,CvPoint2D64f OriginPos,int iWidth,int iHeight){
	m_coordPara.Figure			= Figure;
	m_coordPara.AxisOrigin		= OriginPos;
	m_coordPara.iWidth			= iWidth;
	m_coordPara.iHeight			= iHeight;
	m_coordPara.iDataNum		= (int)vPoint.size();
	if (m_coordPara.iDataNum > 0){
		calminmax(vPoint);
		drawXYZone();
		drawAxisX();
		drawAxisY();
	}
}

void Ploting::calminmax(std::vector<CvPoint2D64f>vPoint){
	// min max
	m_coordPara.XAxis.dMinValue = 10000000;
	m_coordPara.XAxis.dMaxValue = - m_coordPara.XAxis.dMinValue;
	m_coordPara.YAxis.dMinValue = 10000000;
	m_coordPara.YAxis.dMaxValue = - m_coordPara.YAxis.dMinValue;
	for(size_t i = 0; i <vPoint.size(); i++){
		if(vPoint[i].x < m_coordPara.XAxis.dMinValue){
			m_coordPara.XAxis.dMinValue = vPoint[i].x;
		}else if(vPoint[i].x > m_coordPara.XAxis.dMaxValue){
			m_coordPara.XAxis.dMaxValue = vPoint[i].x;
		}
		if(vPoint[i].y < m_coordPara.YAxis.dMinValue){
			m_coordPara.YAxis.dMinValue = vPoint[i].y;
		}else if(vPoint[i].y > m_coordPara.YAxis.dMaxValue){
			m_coordPara.YAxis.dMaxValue = vPoint[i].y;
		}
	}
}

void Ploting::drawXYZone()
{
	CvPoint prev_point, current_point;
	Scalar	color	= CV_RGB(0,0,0);
	double xmin		= m_coordPara.AxisOrigin.x + m_coordPara.iOffsetX;
	double xmax		= m_coordPara.AxisOrigin.x + m_coordPara.iWidth - m_coordPara.iOffsetX;
	double ymin		= m_coordPara.AxisOrigin.y + m_coordPara.iOffsetY;
	double ymax		= m_coordPara.AxisOrigin.y + m_coordPara.iHeight - m_coordPara.iOffsetY;

	prev_point		= cvPoint(xmin,ymin);
	current_point	= cvPoint(xmin,ymax);
	cvLine(m_coordPara.Figure, prev_point, current_point, color, 1, 4);
	prev_point		= cvPoint(xmin, ymin);
	current_point	= cvPoint(xmax, ymin);
	cvLine(m_coordPara.Figure, prev_point, current_point, color, 1, 4);
	prev_point		= cvPoint(xmin, ymax);
	current_point	= cvPoint(xmax, ymax);
	cvLine(m_coordPara.Figure, prev_point, current_point, color, 1, 4);
	prev_point		= cvPoint(xmax, ymin);
	current_point	= cvPoint(xmax, ymax);
	cvLine(m_coordPara.Figure, prev_point, current_point, color, 1, 4);
}

void Ploting::drawAxisX()
{
	int iGridXNum = m_coordPara.iDataNum;
	if (iGridXNum > 20)
	{
		iGridXNum = 10;
	}
	double fXDataLen        = m_coordPara.XAxis.dMaxValue - m_coordPara.XAxis.dMinValue;
	double fXscale          = 1.0;
	double fXScaleLen       = fXDataLen / fXscale;

	double fZoomfactor      = 10;
	while (fXScaleLen > 100 || fXScaleLen < 1)
	{
		fXscale = (fXScaleLen > 100) ? fXscale * fZoomfactor : fXscale / fZoomfactor;
		fXScaleLen = fXDataLen / fXscale;
	}
	double fZoomfactor2     = 2;
	double TextXScale       = 1.0;

	if (fXscale>100)
	{
		TextXScale = fXscale;
		//xContent = "x10" + fXscale.ToString("e").Substring(8);
	}


	while (fXScaleLen > 2 * iGridXNum || fXScaleLen < iGridXNum)
	{
		fXscale = (fXScaleLen > 2 * iGridXNum) ? fXscale * fZoomfactor2 : fXscale/fZoomfactor2;
		fXScaleLen = fXDataLen / fXscale;
	}
	double fGridXStep = fXscale;
	iGridXNum   = (int)(fXScaleLen + 1.5);

	int iGridStart  = 0;
	double fStartPos = 0;

	CvPoint prev_point, current_point;
	Scalar	color	= CV_RGB(192,192,192);
	double xmin		= m_coordPara.AxisOrigin.x + m_coordPara.iOffsetX;
	double xmax		= m_coordPara.AxisOrigin.x + m_coordPara.iWidth - m_coordPara.iOffsetX;
	double ymin		= m_coordPara.AxisOrigin.y + m_coordPara.iOffsetY;
	double ymax		= m_coordPara.AxisOrigin.y + m_coordPara.iHeight - m_coordPara.iOffsetY;
	double fXScaleStep = (xmax - xmin)/ iGridXNum;
	for (int i = iGridStart; i <= iGridXNum; i++)
	{
		double x = i * fXScaleStep + fStartPos;
		double TextData = i * fGridXStep / TextXScale;
		prev_point		= cvPoint(x,ymin);
		current_point	= cvPoint(x,ymax);
		cvLine(m_coordPara.Figure, prev_point, current_point, color, 1, 4);
	}
}

void Ploting::drawAxisY()
{
	int iGridYNum = m_coordPara.iDataNum;
	if (iGridYNum > 20)
	{
		iGridYNum = 10;
	}
	double fYDataLen = m_coordPara.YAxis.dMaxValue - m_coordPara.YAxis.dMinValue;
	int GridYrNum = (int)(-iGridYNum / 2);
	int GridYaNum = (int)(iGridYNum /2);
	double fGridYStep = 1.0;
	double TextYScale = 1.0;
	if (fYDataLen>0)
	{
		double fYscale = 1.0;
		double fYScaleLen = fYDataLen / fYscale;

		double fZoomfactor = 10;
		while (fYScaleLen > 100 || fYScaleLen < 1)
		{
			fYscale = (fYScaleLen > 100) ? fYscale * fZoomfactor : fYscale / fZoomfactor;
			fYScaleLen = fYDataLen / fYscale;
		}


		double fZoomfactor2 = 2;
		while (fYScaleLen > 2 * iGridYNum || fYScaleLen < iGridYNum)
		{
			fYscale = (fYScaleLen > 2 * iGridYNum) ? fYscale * fZoomfactor2 : fYscale / fZoomfactor2;
			fYScaleLen = fYDataLen / fYscale;
		}
		fGridYStep = fYscale;
		GridYrNum = (int)(m_coordPara.YAxis.dMinValue / fGridYStep - 1.5);
		GridYaNum = (int)(m_coordPara.YAxis.dMaxValue / fGridYStep + 1.0);
		iGridYNum = abs(GridYaNum - GridYrNum);
	}
	int iGridStart = GridYrNum;
	int iGridEnd = GridYaNum;

	CvPoint prev_point, current_point;
	Scalar	color	= CV_RGB(192,192,192);
	double xmin		= m_coordPara.AxisOrigin.x + m_coordPara.iOffsetX;
	double xmax		= m_coordPara.AxisOrigin.x + m_coordPara.iWidth - m_coordPara.iOffsetX;
	double ymin		= m_coordPara.AxisOrigin.y + m_coordPara.iOffsetY;
	double ymax		= m_coordPara.AxisOrigin.y + m_coordPara.iHeight - m_coordPara.iOffsetY;
	double fYScaleStep = (ymax - ymin)/ iGridYNum;
	for (int i = iGridStart; i <= iGridYNum; i++)
	{
		double y = i * fYScaleStep;
		double TextData = i * fGridYStep / TextYScale;
		prev_point		= cvPoint(xmin,y);
		current_point	= cvPoint(xmin,y);
		cvLine(m_coordPara.Figure, prev_point, current_point, color, 1, 4);
	}
}

//CvPoint2D64f GetRealPoint(CvPoint2D64f point)
//{
//	var realX = point.X * fXScaleStep / fGridXStep;
//	var realY = -point.Y * fYScaleStep / fGridYStep + this.MainCanvas.ActualHeight + origialFigure.Y;
//	if (double.IsNaN(realY) || double.IsInfinity(realY))
//	{
//		return new Point(0, 0);
//	}
//	return new Point(realX, realY);
//}
//
//CvPoint2D64f GetPlotPoint(CvPoint2D64f point)
//{
//	var realX = point.X * fGridXStep / fXScaleStep;
//	var realY = (this.MainCanvas.ActualHeight + origialFigure.Y - point.Y) * fGridYStep / fYScaleStep;
//	if (double.IsNaN(realY) || double.IsInfinity(realY))
//	{
//		return new Point(0, 0);
//	}
//	return new Point(realX, realY);
//}