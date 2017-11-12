/*====================================
version	：	2.0
update	：	2016.1.21
author	：	chengruiqi
detail  ：  去除斑马线检测，light设置构造和析构函数（取消Light的参数初始化函数，代之以构造函数），增加红绿灯的距离
======================================*/

#include "light.h"
#include <numeric>

using namespace cv;
using namespace std;


Light::Light()
{
}

Light::~Light()
{
	delete[]valid;
}

void Light::detect_trafficlight(int threshold4V, Mat color, Mat& candidate_color, int extract, GlobalVar g_variables,  int& resultTL)
{
	int processWidth = g_variables.width_tl;
	int processHeight = g_variables.height_tl;
	Mat detect_color;
	resize(color,detect_color,Size(processWidth, processHeight));
	open(detect_color);

	////Filter HSV image by threshold, save the results to Mat mask, meanwhile label them	
	maskWithLabel = Mat (processHeight, processWidth, CV_32SC1);
	// Extract candidates by color, get mask(with label)
	colorExtract (threshold4V);
	// Filter candidates by size and ratio 
	shapeFilter(processHeight, processWidth);

	// Filter candidates by spot light feature	
	brightFilter(processHeight, processWidth);

	color_img.copyTo(candidate_color);
	for (size_t i = 1; i < connectedNum; i++)
	{
		//ROI
		rectangle(candidate_color, expandedBox[i], Scalar(255, 0, 0));
		i++;
	}
}

void Light::open(Mat filename)
{
	filename.copyTo(color_img);
	cv::cvtColor(color_img, hsv_img, CV_BGR2HSV_FULL);
	cv::split(hsv_img, channels);
	cvtColor(color_img, gray_img, CV_BGR2GRAY);
}
void Light::colorExtract(int threshold4V)
{
	Mat mask, maskV, mask1, mask2, mask3, mask4;

	// remain V > threshold4V
	threshold(channels[2], maskV, threshold4V, 255, THRESH_BINARY);
	// remain H > 140
	threshold(channels[0], mask3, 140, 255, THRESH_BINARY_INV);
	// remain H > 225
	threshold(channels[0], mask4, 225, 255, THRESH_BINARY);

	mask = maskV &((mask3) | mask4);
	// dilate-erode
	morphologyEx(mask, mask, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(3, 3)));
	cv::imshow("mask", mask);
	connectedNum = connectedComponentsWithStats(mask, maskWithLabel, stats, centroids,8,CV_32S);
	// 确定连通域的个数，减去标号为0的背景
	valid = new bool[connectedNum];
	// 其他元素的初始化在shapeFilter中得到必然保证
	valid[0] = false;
	expandedBox = new Rect[connectedNum];
	// 初始化
	for (size_t i = 1; i < connectedNum; i++)
	{
		expandedBox[i] = Rect(0, 0, 0, 0);
	}

}

void Light::shapeFilter(int rows, int cols)
{
	double max_area = 0.01*rows*cols;
	double min_area = 5e-5*rows*cols;

	for (size_t i = 1; i < connectedNum; i++)
	{
		int* pStats = stats.ptr<int>(i);
		int area = pStats[CC_STAT_AREA];
		if (area > min_area && area < max_area)
		{
			valid[i] = true;
		}
		else
			valid[i] = false;
	}
	// filter by aspect and filling ratio
	for (size_t i = 1; i < connectedNum; i++)
	{
		if (!valid[i])
		{
			continue;
		}
		int* pStats = stats.ptr<int>(i);
		double aspectRatio = double(pStats[CC_STAT_HEIGHT]) / double(pStats[CC_STAT_WIDTH]);
		if (aspectRatio>2.5 || aspectRatio<0.5)
		{
			valid[i] = false;
			continue;
		}
		double tmpfill = double(pStats[CC_STAT_AREA]) / double(pStats[CC_STAT_HEIGHT] * pStats[CC_STAT_WIDTH]);
		if (tmpfill<0.5)
		{
			valid[i] = false;
			//continue;
		}
	}
}

void Light::brightFilter(int rows, int cols)
{
	Mat channel_v = channels[2];
	for (size_t i = 1; i < connectedNum; i++)
	{
		if (!valid[i])
		{
			continue;
		}
		int* pStats = stats.ptr<int>(i);
		int offset_i = (pStats[CC_STAT_HEIGHT]) / 2;
		int offset_j = (pStats[CC_STAT_WIDTH]) / 2;
		//float averV_light = 0;
		//float averV_bg = 0;
		//int num_bg = 0;
		// get expanded box
		int topleft_i = max(pStats[CC_STAT_TOP] - offset_i, 0);
		int bottomright_i = min(pStats[CC_STAT_TOP] + pStats[CC_STAT_HEIGHT] + offset_i, rows - 1);
		int topleft_j = max(pStats[CC_STAT_LEFT] - offset_j, 0);
		int bottomright_j = min(pStats[CC_STAT_LEFT] + pStats[CC_STAT_WIDTH] + offset_j, cols - 1);
		//for (int ii = topleft_i; ii <= bottomright_i; ii++)
		//{
		//	int* pmask = maskWithLabel.ptr<int>(ii);
		//	uchar* pv = channel_v.ptr<uchar>(ii);
		//	for (int jj = topleft_j; jj <= bottomright_j; jj++)
		//	{
		//		if (pmask[jj] == i)
		//		{
		//			averV_light += pv[jj];
		//		}
		//		else
		//		{
		//			averV_bg += pv[jj];
		//			num_bg++;
		//		}
		//	}
		//}
		//averV_light = averV_light / pStats[CC_STAT_AREA];
		//averV_bg = averV_bg / (num_bg);
		//if ((averV_light - averV_bg) / averV_light<0)
		//{
		//	valid[i] = false;
		//	//continue;
		//}
		//else
		//{
			expandedBox[i] = Rect(Point(topleft_j, topleft_i), Point(bottomright_j, bottomright_i));
		//}
	}
}
