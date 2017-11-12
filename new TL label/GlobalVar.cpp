#include "GlobalVar.h"


GlobalVar::GlobalVar(int w_cw, int h_cw, int w_tl, int h_tl)
{
	    width_cw = w_cw;
        height_cw = h_cw;
		width_tl = w_tl;
		height_tl = h_tl;
        frames = 0;
        thresh_last = new float[3];
        thresh_last[0] = 0;
        thresh_last[1] = 0;
        thresh_last[2] = 0;
}

GlobalVar::GlobalVar()
{
}

void GlobalVar::init(int w_cw, int h_cw, int w_tl, int h_tl)
{
	    width_cw = w_cw;
        height_cw = h_cw;
		width_tl = w_tl;
		height_tl = h_tl;
        frames = 0;
        thresh_last = new float[3];
        thresh_last[0] = 0;
        thresh_last[1] = 0;
        thresh_last[2] = 0;
}
