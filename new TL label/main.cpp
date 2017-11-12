#include "Header.h"
#include "Light.h"
#include "GlobalVar.h"
#include "sundry.h"

using namespace std;
using namespace cv;

// define LIGHT, if label units
//#define LIGHT

int main()
{
	string path = "D:\\[VI] Pedestrian crossing lights\\[PCL] new database 1703\\Testing set\\rainy\\00\\20170920_10-2-44_CWdetection.avi";
	int thresh = 100;
	if (!promtInputYN("continue?\ty/n?"))
	{
		getchar();
		return -1;
	}
#ifdef LIGHT
	ofstream txtout(path + "_unit.txt");
#else
	ofstream txtout(path + "sequence.txt");
#endif
	// 分批将数据存入缓存
	VideoCapture capture(path);
	if (!capture.isOpened())
	{
		cerr << "Video Read Fault"<<endl;
		getchar();
		return -1;
	}
	vector<Mat> buffer;
	bool ifVideoEnd = false;
	int bufferN = 0;
	while (!ifVideoEnd)
	{
		for (int i = 0; i < 1000; i++)
		{
			Mat color;
			if (!capture.read(color))
			{
				break;
				ifVideoEnd = true;
			}
			//imshow("watch", color);
			//waitKey(1);
			buffer.push_back(color);
		}
		if (promtInputYN("Whether skip?\ty/n"))
		{
			buffer.clear();
			bufferN++;
			continue;
		}
#ifdef LIGHT
		namedWindow("TL");
		Point p_click;	//最好加static? 避免有时数据赋值后被释放??
		setMouseCallback("TL", leftClick, &p_click);
		// 标记位置和灯
		GlobalVar g_variables;
		g_variables.init(640, 360, 960, 540);
		int frame_counter = 0;
		for (vector<Mat>::iterator it = buffer.begin(); it != buffer.end(); it++)
		{
			if (frame_counter<15)
			{
				frame_counter++;
				continue;
			}
			frame_counter = 0;
			frameProcess(thresh, g_variables, it, p_click, txtout, buffer, bufferN);
		}
#else

		int startNum = 1;
		int endNum;
		int sumNum = buffer.size();
		cout << sumNum;
		char color;
		while (true)
		{
			imshow("TL", buffer[startNum - 1]);
			waitKey(5);
			cout << "input color ? \t";
			cin >> color;
			while (true)
			{
				cout << "INPUT END NUMBER? ";
				int tmpNumm;
				cin >> tmpNumm;
				if (tmpNumm == -1)
				{
					break;
				}
				else
				{
					endNum = tmpNumm;
				}
				// 就是赋值不要更改
				if (endNum > startNum&&endNum <= sumNum)
				{
					stringstream tmpstream;
					tmpstream << "NOW" << endNum << "SUM" << sumNum;
					string tmp = tmpstream.str();
					putText(buffer[endNum - 1], tmp, Point(100, 100), FONT_HERSHEY_DUPLEX, 2, Scalar(255, 0, 0));
					imshow("TL", buffer[endNum - 1]);
					waitKey(1);
				}

			}
			for (size_t i = startNum; i <= endNum; i++)
			{
				txtout << i+bufferN*1000 << "\t" << color<<endl;
			}
			startNum = endNum + 1;
			if (startNum>=1000)
			{
				break;
			}
		}
#endif
		buffer.clear();
		bufferN++;
	}
	getchar();
	system("pause");
    return 0;
}