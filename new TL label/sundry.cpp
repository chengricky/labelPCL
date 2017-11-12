#include "sundry.h"
#include "light.h"

using namespace std;
using namespace cv;

bool promtInputYN(string prompt, char choise1, char choise2)
{
	cout << prompt << endl;
	char ch;
	do
	{
		cin >> ch;
	} while (ch != choise1 && ch != choise2);
	if (ch == choise1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int promtInputYNE(std::string prompt, char choise1, char choise2, char choise3)
{
	cout << prompt << endl;
	char ch;
	do
	{
		cin >> ch;
	} while (ch != choise1 && ch != choise2 && ch != choise3);
	if (ch == choise1)
	{
		return 1;
	}
	else if (ch == choise2)
	{
		return 2;
	}
	else
	{
		return 3;
	}
}

void leftClick(int event, int x, int y, int flag, void* param)
{
	if (event != EVENT_LBUTTONDOWN)
	{
		return;
	}

	Point *ptr = (Point*)param;
	ptr->x = x;
	ptr->y = y;
}

void frameProcess(int th, GlobalVar g_variables, vector<Mat>::iterator it, Point& p_click, ofstream& txtout, vector<Mat>& buffer, int bufferN)
{
	Mat output_tl(g_variables.width_tl, g_variables.height_tl, CV_8UC3);
	Light traffic_light;
	int resultTL;	//green-red-yellow
	traffic_light.detect_trafficlight(th, *it, output_tl, SVM_HOG, g_variables, resultTL);
	imshow("TL", output_tl);
	waitKey(1);
	waitKey(0);
	bool flag = false;
	//while (true)
	//{
		size_t i;
		int chosen  = -1;
		for (i = 1; i < traffic_light.connectedNum; i++)
		{
			if (!traffic_light.valid[i])
			{
				continue;
			}
			Rect ri = traffic_light.expandedBox[i];
			if (p_click.inside(ri))
			{
				rectangle(output_tl, ri, Scalar(0, 0, 255));
				imshow("TL", output_tl);
				waitKey(1);
				int inChar = promtInputYNE("Is this One, y/n or escape (e)?\t");
				if (inChar == 1)
				{
					chosen = i;
					flag = true;
				}
				else if (inChar == 3)
				{
					flag = true;
				}
				rectangle(output_tl, ri, Scalar(0, 0, 0));
				imshow("TL", output_tl);
				waitKey(1);
			}
			else
			{
				continue;
			}
			if (flag)
			{
				break;
			}
		}
		// 没有中的或者跳过了
		if ((flag&&chosen==-1) || i == traffic_light.connectedNum)
		{
			//break;
			return;
		}
		else
		{
			for (int ii = 1; ii < traffic_light.connectedNum; ii++)
			{
				if (!(traffic_light.valid[ii]))
				{
					continue;
				}
				Rect ri = traffic_light.expandedBox[ii];
				if (ii==chosen)
				{
					cout << "set color\t";
					string cl;
					cin >> cl;
					txtout << it - buffer.begin() + 1 + bufferN * 1000 << "\t" << cl << "\t"
						<< (double)ri.tl().x / g_variables.width_tl << "\t" << (double)ri.tl().y / g_variables.height_tl << "\t"
						<< (double)ri.br().x / g_variables.width_tl << "\t" << (double)ri.br().y / g_variables.height_tl << endl;
				}
				else
				{
					txtout << it - buffer.begin() + 1 + bufferN * 1000 << "\t" << 'n' << "\t"
						<< (double)ri.tl().x / g_variables.width_tl << "\t" << (double)ri.tl().y / g_variables.height_tl << "\t"
						<< (double)ri.br().x / g_variables.width_tl << "\t" << (double)ri.br().y / g_variables.height_tl << endl;

				}

			}
		}

	//}
}

