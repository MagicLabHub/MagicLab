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
	this->backgroud_color = CV_RGB(255,255,255); //±³¾°°×É«
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
		}
		if(vPoint[i].x > m_coordPara.XAxis.dMaxValue){
			m_coordPara.XAxis.dMaxValue = vPoint[i].x;
		}
		if(vPoint[i].y < m_coordPara.YAxis.dMinValue){
			m_coordPara.YAxis.dMinValue = vPoint[i].y;
		}
		if(vPoint[i].y > m_coordPara.YAxis.dMaxValue){
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
	int iGridNum = m_coordPara.iDataNum;
	if (iGridNum > 10)
	{
		iGridNum = 10;
	}
	double fDataLen        = m_coordPara.XAxis.dMaxValue - m_coordPara.XAxis.dMinValue;
	double dValueStep       = 1.0;
	double fScaleLen       = int(fDataLen / dValueStep+0.5);

	double fZoomfactor      = 10;
	while (fScaleLen > 100 || fScaleLen < 1)
	{
		dValueStep = (fScaleLen > 100) ? dValueStep * fZoomfactor : dValueStep / fZoomfactor;
		fScaleLen = int(fDataLen / dValueStep+0.5);
	}
	double fZoomfactor2     = 2;

	while ((fScaleLen > 2 * iGridNum || fScaleLen < iGridNum)&&(iGridNum>10))
	{
		dValueStep = (fScaleLen > 2 * iGridNum) ? dValueStep * fZoomfactor2 : dValueStep/fZoomfactor2;
		fScaleLen = int(fDataLen / dValueStep+0.5);
	}
	int iGridStart  = 0;
	if (m_coordPara.XAxis.dMinValue<0)
	{
		iGridStart = (int)(m_coordPara.XAxis.dMinValue / dValueStep - 0.5);
	}
	iGridNum   = (int)(fScaleLen + 1.5);

	

	CvPoint prev_point, current_point;
	Scalar	color	= CV_RGB(192,192,192);
	double xmin		= m_coordPara.AxisOrigin.x + m_coordPara.iOffsetX;
	double xmax		= m_coordPara.AxisOrigin.x + m_coordPara.iWidth - m_coordPara.iOffsetX;
	double ymin		= m_coordPara.AxisOrigin.y + m_coordPara.iOffsetY;
	double ymax		= m_coordPara.AxisOrigin.y + m_coordPara.iHeight - m_coordPara.iOffsetY;
	double dGridStep = (xmax - xmin)/ iGridNum;

	double TextScale       = 1.0;
	if (dValueStep>100)
	{
		TextScale = dValueStep;
		//xContent = "x10" + fXscale.ToString("e").Substring(8);
	}

	CvFont font;  
	cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, 0.3, 0.3, 0, 1, 4);  
	cv::Point origin; 
	origin.y = m_coordPara.AxisOrigin.y + m_coordPara.iHeight;
	int dec, sign;
	int ndig = 1;
	for (int i = iGridStart; i <= iGridNum; i++)
	{
		double x = (i-iGridStart) * dGridStep + xmin;
		double TextData = i * dValueStep / TextScale;

		prev_point		= cvPoint(x,ymin);
		current_point	= cvPoint(x,ymax);
		cvLine(m_coordPara.Figure, prev_point, current_point, color, 1, 4);
		char dstr[20];
		if (dGridStep>1000||dGridStep<0.001){
			sprintf(dstr,"%.2e",TextData);
		}else{
			sprintf(dstr,"%.0f",TextData);
		}
		


		origin.x = x-10;
		cvPutText(m_coordPara.Figure, dstr, origin, &font, CV_RGB(0,0,0)); 
	}
	m_coordPara.XAxis.dAxisLenght	= xmax - xmin;
	m_coordPara.XAxis.iGridNum		= iGridNum;
	m_coordPara.XAxis.iGridStart	= iGridStart;
	m_coordPara.XAxis.dValueStep	= dValueStep;
	m_coordPara.XAxis.dGridStep		= dGridStep;
	m_coordPara.DataOrigin.x		= xmin - dGridStep * iGridStart;

}

void Ploting::drawAxisY()
{
	int iGridNum = m_coordPara.iDataNum;
	if (iGridNum > 5)
	{
		iGridNum = 5;
	}
	double fYDataLen = m_coordPara.YAxis.dMaxValue - m_coordPara.YAxis.dMinValue;
	int iGridStart	= (int)(-iGridNum / 2);
	int iGridEnd	= (int)(iGridNum /2);
	double dValueStep = 1.0;
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
		while (fYScaleLen > 2 * iGridNum || fYScaleLen < iGridNum)
		{
			fYscale = (fYScaleLen > 2 * iGridNum) ? fYscale * fZoomfactor2 : fYscale / fZoomfactor2;
			fYScaleLen = fYDataLen / fYscale;
		}
		dValueStep = fYscale;
		iGridStart = (int)(m_coordPara.YAxis.dMinValue / dValueStep - 1.5);
		iGridEnd = (int)(m_coordPara.YAxis.dMaxValue / dValueStep + 1.5);
		iGridNum = abs(iGridEnd - iGridStart);
	}

	CvPoint prev_point, current_point;
	Scalar	color	= CV_RGB(192,192,192);
	double xmin		= m_coordPara.AxisOrigin.x + m_coordPara.iOffsetX;
	double xmax		= m_coordPara.AxisOrigin.x + m_coordPara.iWidth - m_coordPara.iOffsetX;
	double ymin		= m_coordPara.AxisOrigin.y + m_coordPara.iOffsetY;
	double ymax		= m_coordPara.AxisOrigin.y + m_coordPara.iHeight - m_coordPara.iOffsetY;
	double dGridStep = (ymax - ymin)/ iGridNum;
	double TextScale = 1.0;

	CvFont font;  
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.3, 0.3, 0, 1, 4);  
	cv::Point origin; 
	origin.y = m_coordPara.AxisOrigin.x;
	int dec, sign;
	int ndig = 1;
	for (int i = iGridStart; i <= iGridEnd; i++)
	{
		double y = (i-iGridStart) * dGridStep + ymin;
		double TextData = i * dValueStep / TextScale;
		prev_point		= cvPoint(xmin,y);
		current_point	= cvPoint(xmax,y);
		cvLine(m_coordPara.Figure, prev_point, current_point, color, 1, 4);
		char dstr[20];
		if (dGridStep>1000||dGridStep<0.001){
			sprintf(dstr,"%.2e",TextData);
		}else{
			sprintf(dstr,"%.3f",TextData);
		}
		origin.y = y;
		cvPutText(m_coordPara.Figure, dstr, origin, &font, CV_RGB(0,0,0)); 
	}
	m_coordPara.YAxis.dAxisLenght	= ymax - ymin;
	m_coordPara.YAxis.iGridNum		= iGridNum;
	m_coordPara.YAxis.iGridStart	= iGridStart;
	m_coordPara.YAxis.dValueStep	= dValueStep;
	m_coordPara.YAxis.dGridStep		= dGridStep;
	m_coordPara.DataOrigin.y		= ymax + dGridStep * iGridStart;
}

void Ploting::DrawFlow(std::vector<CvPoint2D64f>vPoint,Scalar color){
	CvPoint prev_point, current_point;
	for(size_t i = 0; i < vPoint.size(); i++)
	{
		current_point = getRealPoint(vPoint[i]);
		if(i>0)
			cvLine(m_coordPara.Figure, prev_point, current_point, color, 1, 8);
		prev_point = current_point;
	}
}

CvPoint Ploting::getRealPoint(CvPoint2D64f point)
{
	int realX = point.x* m_coordPara.XAxis.dGridStep / m_coordPara.XAxis.dValueStep+ m_coordPara.DataOrigin.x;
	int realY = m_coordPara.DataOrigin.y  - point.y * m_coordPara.YAxis.dGridStep / m_coordPara.YAxis.dValueStep;
	if (_finite(point.y)==0)
	{
		return cvPoint(m_coordPara.AxisOrigin.x, m_coordPara.AxisOrigin.y);
	}
	return cvPoint(realX, realY);
}

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