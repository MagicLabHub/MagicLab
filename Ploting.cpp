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
			cvLine(Figure, prev_point, current_point, para.color, 1, CV_AA);
		prev_point = current_point;
		//cvWaitKey(para.iSleepTime);
	}
}

void Ploting::subplot(int col,int row,int sIdx,int eIdx){
	if (eIdx==-1) eIdx = sIdx;
	m_figConfig.iColNum = col;
	m_figConfig.iRowNum = row;
	m_figConfig.iRegionHeight	= m_iFigureHeight/m_figConfig.iColNum;
	m_figConfig.iRegionWidth	= m_iFigureWidth/m_figConfig.iRowNum;
	m_figConfig.iRegionIdx		= sIdx;
	m_figConfig.iMergeRegion	= eIdx-sIdx+1;
}
