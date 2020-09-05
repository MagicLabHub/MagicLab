//************头文件包含*************
#include<iostream>
#include<opencv2/opencv.hpp>//包含opencv的头文件
#include<Windows.h>
#include<Windows.h>
#include"Ploting.h"


void ShowImage(){
	cv::Mat img = cv::imread("D:\\1.jpg",0);
	cvNamedWindow("figure",1);
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 0.5f, 0.5f, 0, 2, CV_AA);//设置显示的字体
	int propID = 0;
	double WinState = 0;
	cv::Mat img1;
	while (WinState >= 0)
	{
		img1 = img.clone();
		POINT pt;
		GetCursorPos(&pt);
		cv::waitKey(100);
		float fScale = pt.x/(float)pt.y;
		img1 = fScale*img1;
		std::cout << "当前du的鼠标坐zhi标为dao：" << pt.x << "," << pt.y << std::endl;
		std::string strText = "fScale: "+std::to_string((long double)fScale)+","+"mouse point:["+std::to_string((_Longlong)pt.x)+","+std::to_string((_Longlong)pt.y)+"]";
		cv::putText(img1, strText, cvPoint(10, 50), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(0, 255, 255), 1, 8, 0);

		WinState = cv::getWindowProperty("figure",propID);
		cv::imshow("figure", img1);//显示图像
	}
	img.release();
	cv::destroyAllWindows();
}

void PlotData(){
	const int Cnt = 2000;
	std::shared_ptr<double>X(new double[Cnt]);
	std::shared_ptr<double>Y(new double[Cnt]);
	std::shared_ptr<double>Z(new double[Cnt]);
	double*pX = X.get();
	double*pY = Y.get();
	for (int i=0;i<2000;++i)
	{
		double theta=3.1415926/1000.0*(i-1000);
		pX[i]=std::cos(theta);
		pY[i]=std::sin(theta)+std::pow(std::pow(pX[i],2.0),0.33333);
	}
	cvNamedWindow("hello",0);
	Ploting plot;
	plot.plot(X.get(),Y.get(), Cnt, CV_RGB(0,0,255)); //可以只传入Y值 X默认从0开始 ,
	//plot.subplot(3,1,1);plot.plot(X.get(), Cnt, CV_RGB(0,0,255)); //可以只传入Y值 X默认从0开始 
	//plot.subplot(3,1,2);plot.plot(X.get(),Y.get(), Cnt, CV_RGB(255,0,0)); //可以只传入Y值 X默认从0开始 
	//plot.subplot(3,1,3);plot.plot(Y.get(), Cnt, CV_RGB(255,255,0)); //可以只传入Y值 X默认从0开始 

	double WinState = 0;
	int propID = 0;

	while (WinState >= 0){
		cvShowImage("hello", plot.Figure);
		cvWaitKey(0);
		WinState = cv::getWindowProperty("hello",propID);
	}
}

int main()
{
	
	PlotData();
    //ShowImage();
	
    
	
    return 0;
}