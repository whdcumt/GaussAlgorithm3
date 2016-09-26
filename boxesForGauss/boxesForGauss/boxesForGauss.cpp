// boxesForGauss.cpp : �������̨Ӧ�ó������ڵ㡣
/// <summary>  
/// ʵ���þ�ֵģ��ģ���˹ģ��ʱ��������Ҫ��ֵģ���Ĵ�������������Ҫ�ľ�ֵģ���İ뾶���δ�С.  
/// �ο����ģ�http://blog.ivank.net/fastest-gaussian-blur.html  
///modify:whd qq:710433980  ��������ʱ�� 2016.9.26��
#include "stdafx.h"
#include <cmath>
//�������ܣ��������뺯����
/// <param name="r"> Ҫ�����������ֵ</param>  
/// <remarks> ����ֵΪ���������Ľ��</remarks>  
double round(double r)  
{  
    return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);  
}
//�������ܣ������ֵģ��ģ���˹ģ���ĸ��뾶��ֵ��
/// <param name="sigma"> ��˹ģ������sigma</param>  
/// <param name="n"> ��Ҫ�õ���ֵģ����������Ĭ��ֵ��3</param>  
/// <remarks> ����ֵΪ���ν��о�ֵģ���İ뾶</remarks>  
int *boxesForGauss(float sigma, int n)  // standard deviation, number of boxes
{
	 float wIdeal = sqrt((12*sigma*sigma/n)+1);  // Ideal averaging filter width 
     int wl = floor(wIdeal);  if(wl%2==0) wl--;
     int wu = wl+2;		
     float mIdeal = (12*sigma*sigma - n*wl*wl - 4*n*wl - 3*n)/(-4*wl - 4);
     int m = round(mIdeal);

     int *sizes=new int[3];  
	 for(int i=0; i<n; i++)
	 {
		 sizes[i]=i<m?wl:wu;
	 }
     return sizes;
}
int _tmain(int argc, _TCHAR* argv[])
{  
	double sigma=1;
	int *sizes=boxesForGauss(sigma,3);
	printf("��˹ģ���Ĳ���sigma=%f\n",sigma);
	printf("���ξ�ֵģ���İ뾶�ֱ�Ϊ��%d,%d,%d\n",sizes[0],sizes[1],sizes[2]);
	return 0;
}

