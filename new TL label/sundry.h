#pragma once

#include "Header.h"
#include "globalvar.h"

// Summary: display prompt information and compare input character with y or n
bool promtInputYN(std::string prompt, char choise1 = 'y', char choise2 = 'n');

// Summary: display prompt information and compare input character with y or n or e
int promtInputYNE(std::string prompt, char choise1 = 'y', char choise2 = 'n', char choise3 = 'e');

// Summary: what to do after left button is pressed
void leftClick(int event, int x, int y, int flag, void* param);

// Summary: what to do when processing one frame
void frameProcess(int th, GlobalVar g_variables, std::vector<cv::Mat>::iterator it, cv::Point& p_click,
	std::ofstream& txtout, std::vector<cv::Mat>& buffer, int bufferN);