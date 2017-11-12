/*====================================
update : 2016.12.07
author : Cheng Ruiqi
detail : 
======================================*/
#pragma once

#include "Header.h"
#include "GlobalVar.h"

#define SVM_HOG 1

// 红绿灯检测的类
class Light
{
public:
	// some images Mat
	cv::Mat color_img;	// color image in RGB space
	cv::Mat gray_img;	// gray-scale image
	cv::Mat hsv_img;	// color image in HSV space
	std::vector<cv::Mat> channels;  // hsv split

	// candidates of connected area
	cv::Mat maskWithLabel; //CV_32S or 16U
	cv::Mat stats; //CV_32S
	cv::Mat centroids; //CV_64F
	cv::Rect* expandedBox;
	bool* valid;
	int connectedNum;

	// HOG vector of every candidate
	//std::vector<std::vector<float>> hog_set; 

	// 对图像的处理
	void open(cv::Mat);
	void detect_trafficlight(int th, cv::Mat color, cv::Mat&, int extract, GlobalVar g_variables,  int& resultTL);
	// extract color, get mask & stats of connected domains
	void colorExtract (int threshold4V);
	void shapeFilter(int, int);
	void brightFilter(int, int);

	// 构造函数
	Light();
	// 析构函数
	~Light();

};



