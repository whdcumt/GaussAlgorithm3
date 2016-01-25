// Algorithm3.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "stdafx.h"
#include <cmath>
#include<opencv2/opencv.hpp>
#include<windows.h>  //������������ʱ�亯��
using namespace std;
using namespace cv;  
//ǰ������ΪOpenCVͼ��������������Ҫ�Ļ�����ͷ�ļ��������ռ�
#define M_PI 3.14159265358979323846
//�������뺯������
double round(double r);  
//���ݷ���;�ֵģ������ȷ��ģ���뾶��С��������
int *boxesForGauss(float sigma, int n);  // standard deviation, number of boxes 
//ģ���㷨2��������
void gaussBlur_3(IplImage *scl,IplImage *tcl,int w,int h,int r);
//��ֵģ����������
void boxBlur_3(IplImage *scl,IplImage *tcl,int w,int h,int r); 
//ˮƽһάģ����������
void boxBlurH_3 (IplImage *scl,IplImage *tcl,int w,int h,int r);
//��ֱһάģ����������
void boxBlurT_3(IplImage *scl,IplImage *tcl,int w,int h,int r);
int _tmain(int argc, _TCHAR* argv[])
{

	Mat InputImage=imread("test.jpg");
	imshow("ԭʼͼ��",InputImage);
	//���������ǲ��Գ����Ƿ����ú�OpenCV��򵥵ķ�ʽ��
	// @����Ĳ�����������OpenCV�������ͼ�����ͨ�����������ν��д���@
	//***********************************************************************//
	// @���Ϊ��ͨ���ģ�����ͨ������֮���ÿ��ͨ���ֱ���и�˹ģ������@
	// @���Ϊ��ͨ���ģ���ֱ�ӽ���ģ������@
	//��������ΪMat����ת��ΪIplImage���ͣ���ʵIplImage���;��ǵ�ַ��Mat���;��Ǳ�������������ת����ʱ����Ҫȡ��ַ
	//֮����ת����������ΪIplImage��ͨ������ο�����Ƚ϶�
	IplImage *InputImageConvertIp;
	InputImageConvertIp=&(IplImage(InputImage));  
	//@ͨ������
	//��������ͨ����ͼ��ͷ����������Ĵ洢
	IplImage* bImg=cvCreateImage(cvGetSize(InputImageConvertIp),IPL_DEPTH_8U,1);    
    IplImage* gImg=cvCreateImage(cvGetSize(InputImageConvertIp),IPL_DEPTH_8U,1);    
    IplImage* rImg=cvCreateImage(cvGetSize(InputImageConvertIp),IPL_DEPTH_8U,1);   
	cvSplit(InputImageConvertIp,bImg,gImg,rImg,0);
	//cvShowImage("red",rImg);
	//cvShowImage("green",gImg);
	//cvShowImage("blue",bImg);
	//@ͨ������
	IplImage* rImgBlur=cvCreateImage(cvGetSize(InputImageConvertIp),IPL_DEPTH_8U,1);  
	IplImage* gImgBlur=cvCreateImage(cvGetSize(InputImageConvertIp),IPL_DEPTH_8U,1);  
	IplImage* bImgBlur=cvCreateImage(cvGetSize(InputImageConvertIp),IPL_DEPTH_8U,1);  
	DWORD start_time=GetTickCount();
	gaussBlur_3(rImg, rImgBlur,rImgBlur->width ,rImgBlur->height,3);
	gaussBlur_3(gImg, gImgBlur,gImgBlur->width ,gImgBlur->height,3);
	gaussBlur_3(bImg, bImgBlur,bImgBlur->width ,bImgBlur->height,3);
	DWORD end_time=GetTickCount();
    cout<<"The run time is:"<<(end_time-start_time)<<"ms!"<<endl;//�������ʱ��
   // cvShowImage("reddeblur",rImgBlur);

	//ͨ���ϲ�
	 IplImage* mergeImgBlur=cvCreateImage(cvGetSize(InputImageConvertIp),IPL_DEPTH_8U,3);   
	 cvMerge(bImgBlur,gImgBlur,rImgBlur,0,mergeImgBlur);
	 cvShowImage("��ͨ��ģ����ͼ��",mergeImgBlur);

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