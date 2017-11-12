/*====================================
update	£º	2016.9.17
author	£º	cheng ruiqi
detail   :  
======================================*/
#pragma once

class GlobalVar
{
public:
    // results
    int width_cw;			// resolution
    int height_cw;			// resolution
	int width_tl;
	int height_tl;
    float* thresh_last;
    int frames;

	GlobalVar(int w_cw, int h_cw, int w_tl, int h_tl);
	GlobalVar();
	~GlobalVar(void){};

	void init(int w_cw, int h_cw, int w_tl, int h_tl);
};

