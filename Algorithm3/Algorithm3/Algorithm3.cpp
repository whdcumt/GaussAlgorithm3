// Algorithm3.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <cmath>
#include<opencv2/opencv.hpp>
#include<windows.h>  //包含程序运行时间函数
using namespace std;
using namespace cv;  
//前面三行为OpenCV图像输入和输出所需要的基本的头文件和命名空间
#define M_PI 3.14159265358979323846
//四舍五入函数声明
double round(double r);  
//根据方差和均值模糊个数确定模糊半径大小函数声明
int *boxesForGauss(float sigma, int n);  // standard deviation, number of boxes 
//模糊算法2函数声明
void gaussBlur_3(IplImage *scl,IplImage *tcl,int w,int h,int r);
//均值模糊函数声明
void boxBlur_3(IplImage *scl,IplImage *tcl,int w,int h,int r); 
//水平一维模糊函数声明
void boxBlurH_3 (IplImage *scl,IplImage *tcl,int w,int h,int r);
//竖直一维模糊函数声明
void boxBlurT_3(IplImage *scl,IplImage *tcl,int w,int h,int r);
int _tmain(int argc, _TCHAR* argv[])
{

	Mat InputImage=imread("test.jpg");
	imshow("原始图像",InputImage);
	//以上两句是测试程序是否配置好OpenCV最简单的方式。
	// @下面的操作就是利用OpenCV将输入的图像根据通道的数量依次进行处理，@
	//***********************************************************************//
	// @如果为三通道的，进行通道分离之后对每个通道分别进行高斯模糊处理，@
	// @如果为单通道的，则直接进行模糊处理。@
	//下面两句为Mat类型转换为IplImage类型，其实IplImage类型就是地址，Mat类型就是变量，所以类型转换的时候需要取地址
	//之所以转换类型是因为IplImage的通道分离参考程序比较多
	IplImage *InputImageConvertIp;
	InputImageConvertIp=&(IplImage(InputImage));  
	//@通道分离
	//定义三个通道的图像头，方便分离后的存储
	IplImage* bImg=cvCreateImage(cvGetSize(InputImageConvertIp),IPL_DEPTH_8U,1);    
    IplImage* gImg=cvCreateImage(cvGetSize(InputImageConvertIp),IPL_DEPTH_8U,1);    
    IplImage* rImg=cvCreateImage(cvGetSize(InputImageConvertIp),IPL_DEPTH_8U,1);   
	cvSplit(InputImageConvertIp,bImg,gImg,rImg,0);
	//cvShowImage("red",rImg);
	//cvShowImage("green",gImg);
	//cvShowImage("blue",bImg);
	//@通道分离
	IplImage* rImgBlur=cvCreateImage(cvGetSize(InputImageConvertIp),IPL_DEPTH_8U,1);  
	IplImage* gImgBlur=cvCreateImage(cvGetSize(InputImageConvertIp),IPL_DEPTH_8U,1);  
	IplImage* bImgBlur=cvCreateImage(cvGetSize(InputImageConvertIp),IPL_DEPTH_8U,1);  
	DWORD start_time=GetTickCount();
	gaussBlur_3(rImg, rImgBlur,rImgBlur->width ,rImgBlur->height,3);
	gaussBlur_3(gImg, gImgBlur,gImgBlur->width ,gImgBlur->height,3);
	gaussBlur_3(bImg, bImgBlur,bImgBlur->width ,bImgBlur->height,3);
	DWORD end_time=GetTickCount();
    cout<<"The run time is:"<<(end_time-start_time)<<"ms!"<<endl;//输出运行时间
   // cvShowImage("reddeblur",rImgBlur);

	//通道合并
	 IplImage* mergeImgBlur=cvCreateImage(cvGetSize(InputImageConvertIp),IPL_DEPTH_8U,3);   
	 cvMerge(bImgBlur,gImgBlur,rImgBlur,0,mergeImgBlur);
	 cvShowImage("三通道模糊后图像",mergeImgBlur);

	cvReleaseImage(&rImg);
	cvReleaseImage(&gImg);
	cvReleaseImage(&bImg);
	cvReleaseImage(&rImgBlur);
	cvReleaseImage(&gImgBlur);
	cvReleaseImage(&bImgBlur);
    cvReleaseImage(&mergeImgBlur);
	//**************************************************************//
	waitKey();
	return 0;
}
int *boxesForGauss(float sigma, int n)  // standard deviation, number of boxes
{
	 float wIdeal = sqrt((12*sigma*sigma/n)+1);  // Ideal averaging filter width 
     int wl = floor(wIdeal);  if(wl%2==0) wl--;
     int wu = wl+2;		
     float mIdeal = (12*sigma*sigma - n*wl*wl - 4*n*wl - 3*n)/(-4*wl - 4);
     int m = round(mIdeal);
    // var sigmaActual = Math.sqrt( (m*wl*wl + (n-m)*wu*wu - n)/12 );		
     int *sizes=new int[3];  
	 for(int i=0; i<n; i++)
	 {
		 sizes[i]=i<m?wl:wu;
	 }
     return sizes;
}
double round(double r)  
{  
    return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);  
}  
void gaussBlur_3(IplImage *scl,IplImage *tcl,int w,int h,int r)
{
    int *bxs = boxesForGauss(r, 3);
    boxBlur_3 (scl, tcl, w, h, (bxs[0]-1)/2);
    boxBlur_3 (tcl, scl, w, h, (bxs[1]-1)/2);
    boxBlur_3 (scl, tcl, w, h, (bxs[2]-1)/2);
}
void boxBlur_3 (IplImage *scl,IplImage *tcl,int w,int h,int r)
{
    boxBlurH_3(scl, tcl, w, h, r);
    boxBlurT_3(tcl, scl, w, h, r);
}
void boxBlurH_3 (IplImage *scl,IplImage *tcl,int w,int h,int r) 
{
    CvScalar s;
	for(int i=0; i<h; i++)
        for(int j=0; j<w; j++)  
		{
            double  val = 0;
            for(int ix=j-r; ix<j+r+1; ix++) 
			{
                 int x = MIN(w-1, MAX(0, ix));
                 val += cvGet2D(scl,i,x).val[0];
            }
			s.val[0]=val/(r+r+1);
            cvSet2D(tcl,i,j, s);
        }
}   
void boxBlurT_3(IplImage *scl,IplImage *tcl,int w,int h,int r) 
{
  CvScalar s;
  for(int i=0; i<h; i++)
        for(int j=0; j<w; j++) 
		{
            double val = 0;
            for(int iy=i-r; iy<i+r+1; iy++) 
			{
                int y = MIN(h-1, MAX(0, iy));
                val += cvGet2D(scl,y,i).val[0];;
            }
			s.val[0]=val/(r+r+1);
            cvSet2D(tcl,i,j, s);
        }
}