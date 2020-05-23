#include "stdio.h"
#include "Interfaces.h"
#include "windows.h"
#include <process.h>
#include <TCHAR.H>

#include <iostream>

#ifdef _DEBUG
#else
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
//#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;
using namespace cv::cuda;

#define WIN_NAME "video"

//#define CreateWins
//#define OPENCV_SHOW_IMG
#define OPENCV_CAMERA_OPE
	#define MOUSE_MOVE_ON_PAPER
	//#define MOUSE_MOVE_WITH_RED
//#define OPENCV_OPENGL 

#ifdef OPENCV_OPENGL
#include <GL/gl.h>
#include <GL/glu.h>

#include "opencv2/core.hpp"
#include "opencv2/core/opengl.hpp"
#include "opencv2/core/cuda.hpp"

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib, "glu32.lib")
#endif

#ifdef CreateWins
#define WM_TEST	WM_USER+1

static TCHAR szClassName[] = _T("Win For Test");

void UserPaint(HWND hwnd)
{
	static TCHAR szstr[1024] = TEXT("Hello World!");

	HDC dc;
	RECT rect;
	PAINTSTRUCT ps;
	HBRUSH m_brush = CreateHatchBrush(HS_BDIAGONAL, RGB(0, 0, 0));

	dc = BeginPaint(hwnd, &ps);

	GetClientRect(hwnd, &rect);

	//color setting
	SetBkMode(dc, TRANSPARENT);
	SetBkColor(dc, RGB(0, 0, 0));
	SetTextColor(dc, RGB(255, 0, 0));

	FillRect(dc, &rect, m_brush);

	//paint context
	TextOut(dc, 0, 0, szstr, _tcsclen(szstr));

	EndPaint(hwnd, &ps);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
	case WM_TEST:
		printf("WM_TEST\r\n");

	case WM_PAINT:
		UserPaint(hwnd);
		//printf("WM_PAINT[%ld][%ld]\r\n", (LONG)wParam, (LONG)lParam);

		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		printf("WM_DESTROY\r\n");

		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

DWORD WINAPI WinThread(LPVOID pConfig)
{
	WNDCLASS wc;
	MSG msg;
	HWND g_hMain = NULL;
	Threads_Control_t* p = (Threads_Control_t*)pConfig;
	int win_x = 100, win_y = 200, win_width = 300, win_hegith = 400;

	memset(&wc, 0, sizeof(WNDCLASS));

	wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wc.hCursor = (HCURSOR)LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = (HICON)LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = szClassName;

	if (!RegisterClass(&wc))
	{
		int n = GetLastError();
		printf("error in RegisterClass\r\n");
	}
	else
	{
		if ((g_hMain = CreateWindow(	szClassName, szClassName, WS_OVERLAPPEDWINDOW, \
										win_x, win_y, win_width, win_hegith, \
										NULL, NULL, wc.hInstance, NULL)) != NULL)
		{
			ShowWindow(g_hMain, SW_SHOW);
			UpdateWindow(g_hMain);

			while (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				if (FUNC_ENABLE == p->exit_flag)
				{
					break;
				}
			}

			g_hMain = NULL;
		}
		else
		{
			printf("error in CreateWindow\r\n");
		}
	}

	return 0;
}
#endif

#ifdef OPENCV_CAMERA_OPE

#define MOUSE_POS_BK_NUM	10

typedef enum
{
	INIT_STATUS,
	CONFIG_STATUS,
	WORK_STATUS,
	PAUSE_STATUS,
	STOP_STATUS,
}Status_t;

typedef struct
{
	Mat *phh;
	Threads_Control_t* gpConfig;

	Status_t flag;
	uint8_t count;
	Point_Location_t ValidArea[2];
	float A, B;
}UserData_t;

const int win_width = 800;
const int win_height = 640;
const int rgb_channel = 3;
uint8_t InitData[win_height][win_width * rgb_channel];

static Point_Location_t Mouse_Pos[MOUSE_POS_BK_NUM];

int CalSum[FUNC_NUM_IN_TOTAL];
int FucPos[FUNC_NUM_IN_TOTAL];

void on_mouse(int EVENT, int x, int y, int flags, void* userdata)
{
	Mat hh;
	UserData_t* pInfo = (UserData_t*)userdata;
	hh = *(pInfo->phh);
	Point p(x, y);

	switch (EVENT)
	{
		case EVENT_LBUTTONDOWN:
		{
			printf("x = %d,y = %d\r\n", x, y);
			printf("b=%d\t", hh.at<Vec3b>(p)[0]);
			printf("g=%d\t", hh.at<Vec3b>(p)[1]);
			printf("r=%d\n", hh.at<Vec3b>(p)[2]);
			circle(hh, p, 2, Scalar(255), 3);

			switch (pInfo->flag)
			{
			case INIT_STATUS:
				pInfo->count = 0;
				pInfo->flag = CONFIG_STATUS;
			case CONFIG_STATUS:
				pInfo->ValidArea[pInfo->count].x = x;
				pInfo->ValidArea[pInfo->count].y = y;
				pInfo->count++;
				break;
			}
		}
		break;

	}

}

void adjust_scene_left_right(uchar* pBuffer,int row,int col,int channel,int step)
{
	int i,j;
	uchar temp;
	uchar* data = pBuffer;
	for (i = 0; i < row; i++)
	{
		for (j = 0; j <= col / 2; j++)
		{
			temp = data[j * channel];
			data[j * channel] = data[(col - 1 - j) * channel];
			data[(col - 1 - j) * channel] = temp;

			temp = data[j * channel + 1];
			data[j * channel + 1] = data[(col - 1 - j) * channel + 1];
			data[(col - 1 - j) * channel + 1] = temp;

			temp = data[j * channel + 2];
			data[j * channel + 2] = data[(col - 1 - j) * channel + 2];
			data[(col - 1 - j) * channel + 2] = temp;
		}

		data += step;
	}
}
#endif

#ifdef OPENCV_OPENGL
void on_opengl(void* param)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	glTranslated(0.0, 0.0, -1.0);
	glRotatef(55, 1, 0, 0);
	glRotatef(45, 0, 1, 0);
	glRotatef(0, 0, 0, 1);
	static const int coords[6][4][3] = {
		{ { +1, -1, -1 }, { -1, -1, -1 }, { -1, +1, -1 }, { +1, +1, -1 } },
		{ { +1, +1, -1 }, { -1, +1, -1 }, { -1, +1, +1 }, { +1, +1, +1 } },
		{ { +1, -1, +1 }, { +1, -1, -1 }, { +1, +1, -1 }, { +1, +1, +1 } },
		{ { -1, -1, -1 }, { -1, -1, +1 }, { -1, +1, +1 }, { -1, +1, -1 } },
		{ { +1, -1, +1 }, { -1, -1, +1 }, { -1, -1, -1 }, { +1, -1, -1 } },
		{ { -1, -1, +1 }, { +1, -1, +1 }, { +1, +1, +1 }, { -1, +1, +1 } }
	};
	for (int i = 0; i < 6; ++i) 
	{
		glColor3ub(i * 20, 100 + i * 10, i * 42);
		glBegin(GL_QUADS);
		for (int j = 0; j < 4; ++j) {
			glVertex3d(0.2 * coords[i][j][0], 0.2 * coords[i][j][1], 0.2 * coords[i][j][2]);
		}
		glEnd();
	}
	
}
#endif

void Mytest1(Threads_Control_t* gpConfig)
{
#ifdef OPENCV_SHOW_IMG
	Mat image = imread("E:/1.jpg");

	if (image.empty()) {

		printf("could not load image...\n");

		return;

	}
	else
	{
		namedWindow("test_opencv_setup", 0);

		imshow("test_opencv_srtup", image);

		waitKey(0);
	}

#endif

#ifdef CreateWins
	HANDLE hThread;
	DWORD  threadId;

	hThread = CreateThread(NULL, 0, WinThread, gpConfig, 0, &threadId);

	while (FUNC_DISABLE == gpConfig->exit_flag)
	{
		Sleep(1000);
	}
#endif

#ifdef OPENCV_CAMERA_OPE
	uchar *pImage,*data, *pInitData = &InitData[0][0];
	uchar val1, val2, val3;
	UserData_t info;
	VideoCapture cap;
	int i,j,k,channel,count,temp1;
	Mat original_frame,new_frame;
	new_frame = Mat::zeros(256, 256, CV_8UC3);

	cap.open(1);

	if (!cap.isOpened())
	{
		printf("open camera error\r\n");
		return;
	}

	cap.read(original_frame);

	printf("row = %d,col = %d,channels = %d\r\n", original_frame.rows, original_frame.cols, original_frame.channels());

#if 0
	printf("row0 = %d,col = %d,channels = %d\r\n", new_frame.rows, new_frame.cols, new_frame.channels());
	cv::cvtColor(original_frame,original_frame,cv::COLOR_BGR2GRAY);
	resize(original_frame, new_frame, new_frame.size());
	printf("row1 = %d,col = %d,channels = %d\r\n", new_frame.rows, new_frame.cols, new_frame.channels());
#endif

	namedWindow(WIN_NAME, 0);

	resizeWindow(WIN_NAME, win_width, win_height);
#define ORIGINAL_IMG

#ifdef ORIGINAL_IMG
	info.gpConfig = gpConfig;
	info.phh = &original_frame;
	info.flag = INIT_STATUS;
	setMouseCallback(WIN_NAME, on_mouse, &info);
#else
	setMouseCallback(WIN_NAME, on_mouse, &new_frame);
#endif

	cap.read(original_frame);

	channel = original_frame.channels();

	if (rgb_channel != channel)
	{
		printf("channel != 3\r\n");
		return;
	}

	while (1)
	{
		/*get original img*/
		cap.read(original_frame);

#ifdef ORIGINAL_IMG

		//adjust scene
		adjust_scene_left_right(original_frame.data, \
			original_frame.rows, original_frame.cols,\
			channel, original_frame.step);

		if (CONFIG_STATUS == info.flag)
		{
			if (2 == info.count)
			{
				printf("----------->x0= %d,y0= %d\r\n", info.ValidArea[0].x, info.ValidArea[0].y);

				printf("----------->x1= %d,y1= %d\r\n", info.ValidArea[1].x, info.ValidArea[1].y);

				printf("total points = %d,div into %d pieces\r\n", \
					20 * (info.ValidArea[1].x - info.ValidArea[0].x) , FUNC_NUM_IN_TOTAL);
				//y = A*x+B
				info.A = (float)(info.ValidArea[0].y - info.ValidArea[1].y) / \
					(info.ValidArea[0].x - info.ValidArea[1].x);

				info.B = info.ValidArea[0].y - info.A * (info.ValidArea[0].x);

				printf("----------->A= %f,B= %f\r\n", info.A, info.B);

#if 1
				data = original_frame.data;
				for (i = 0; i < original_frame.rows; i++)
				{
					memcpy(&InitData[i][0], data, win_width * rgb_channel);

					data += original_frame.step;
				}
#endif
				temp1 = (info.ValidArea[1].x - info.ValidArea[0].x) / FUNC_NUM_IN_TOTAL;
				for (i=0;i< FUNC_NUM_IN_TOTAL;i++)
				{
					FucPos[i] = info.ValidArea[0].x + i * temp1;
					printf("----------->Func[%d],pos = %d\r\n", i, FucPos[i]);
				}

				info.flag = WORK_STATUS;
			}
		}
		else if (WORK_STATUS == info.flag)
		{
			Mouse_Pos[0].x = 0;
			Mouse_Pos[0].y = 0;
			count = 0;

			data = original_frame.data;
			for (i = 0; i < FUNC_NUM_IN_TOTAL; i++)
			{
				CalSum[i] = 0;
			}

			for (i = 0; i < original_frame.rows; i++)
			{
				for (j = 0; j < original_frame.cols * channel; )
				{
#ifdef MOUSE_MOVE_ON_PAPER

//test for paint the hit point on the original image
#if 0
					if (((j / channel) < info.ValidArea[0].x + 10) && \
						((j / channel) > info.ValidArea[0].x - 10) && \
						(i < info.ValidArea[0].y + 10) && \
						(i > info.ValidArea[0].y - 10)
						)
					{
						data[j] = 0;
						data[j + 1] = 0;
						data[j + 2] = 255;
					}
					else if (((j / channel) < info.ValidArea[1].x + 10) && \
						((j / channel) > info.ValidArea[1].x - 10) && \
						(i < info.ValidArea[1].y + 10) && \
						(i > info.ValidArea[1].y - 10)
						)
					{
						data[j] = 0;
						data[j + 1] = 0;
						data[j + 2] = 255;
					}
#endif

//working on the selected area
#if 1
					temp1 = info.A * (j / channel) + info.B;

//paint black on the no-select area
#if 0
					if (!((i > temp1 - 10) && (i < temp1 + 10)))
					{
						//data not in the area
						data[j] = 0;
						data[j + 1] = 0;
						data[j + 2] = 0;
					}
#endif

//paint the green edge of selected area
#if 1
					//paint the edge of select area
					//horizon
					if (((i == ( temp1 - 10)) || \
						(i == ( temp1 + 10))) && \
						(j / channel <= info.ValidArea[1].x) && \
						(j / channel >= info.ValidArea[0].x))
					{
						data[j] = 0;
						data[j + 1] = 255;
						data[j + 2] = 0;
					}
					else if ((i>=(temp1-10)) && (i<=(temp1+10)))		//vertical
					{
						if ((j / channel == info.ValidArea[1].x) || \
							(j / channel == info.ValidArea[0].x))
						{
							data[j] = 0;
							data[j + 1] = 255;
							data[j + 2] = 0;
						}
						else
						{
							for (k = 0; k < FUNC_NUM_IN_TOTAL; k++)
							{
								if (FucPos[k] == j / channel)
								{
									data[j] = 0;
									data[j + 1] = 255;
									data[j + 2] = 0;
								}
							}
						}
					}

#endif

//select area
#if 1
					if ((i >= (temp1 - 10)) && \
						(i <= (temp1 + 10)) && \
						(j / channel <= info.ValidArea[1].x) && \
						(j / channel >= info.ValidArea[0].x))
					{
						val1 = (data[j] > pInitData[i * original_frame.step + j])? \
								(data[j] - pInitData[i * original_frame.step + j]): \
								(pInitData[i * original_frame.step + j] - data[j]);

						val2 = data[j + 1] > pInitData[i * original_frame.step + j + 1] ? \
								data[j + 1] - pInitData[i * original_frame.step + j + 1]:\
								pInitData[i * original_frame.step + j + 1] - data[j + 1];

						val3 = data[j + 2] > pInitData[i * original_frame.step + j + 2]?
								data[j + 2] - pInitData[i * original_frame.step + j + 2]:\
								pInitData[i * original_frame.step + j + 2] - data[j + 2];

						if ((val1 > 50) || (val2 > 50) || (val3 > 50))
						{
#if 0
							printf("[%d],0x%x,0x%x,0x%x,%d,%d,%d,%d,%d,%d\r\n", j, \
								val1,val2,val3,
								data[j], pInitData[i * original_frame.step + j], \
								data[j + 1], pInitData[i * original_frame.step + j + 1],
								data[j + 2], pInitData[i * original_frame.step + j + 2]);
#endif
							temp1 = j / channel;
							if (temp1 > FucPos[MOV_RIGHT])
							{
								CalSum[MOV_RIGHT]++;
							}
							else if (temp1 > FucPos[MOV_DOWN])
							{
								CalSum[MOV_DOWN]++;
							}
							else if (temp1 > FucPos[MOUSE_CLICK])
							{
								CalSum[MOUSE_CLICK]++;
							}
							else if (temp1 > FucPos[MOV_UP])
							{
								CalSum[MOV_UP]++;
							}
							else //MOV_LEFT
							{
								CalSum[MOV_LEFT]++;
							}
#if 1
							pInitData[i * original_frame.step + j] = data[j];
							pInitData[i * original_frame.step + j + 1] = data[j + 1];
							pInitData[i * original_frame.step + j + 2] = data[j + 2];
#endif
							data[j] = 0;
							data[j+1] = 0;
							data[j+2] = 255;
						}
					}
#endif


#endif

#endif

#ifdef MOUSE_MOVE_WITH_RED
					//if ((data[j] > 20) || (data[j + 2] < 150)) 
					if ((data[j] < 200) || (data[j + 2] < 170))
					{
#if 1
						data[j] = 0;
						data[j + 1] = 0;
						data[j + 2] = 0;
#endif
					}
					else
					{
						Mouse_Pos[0].x += j / channel;
						Mouse_Pos[0].y += i;
						count++;
					}
#endif
					j += channel;
				}
				data += original_frame.step;
			}

#ifdef MOUSE_MOVE_ON_PAPER
			temp1 = MOV_DOWN;
			if (CalSum[MOV_RIGHT] > CalSum[MOV_DOWN])
			{
				temp1 = MOV_RIGHT;
			}
			if (CalSum[MOUSE_CLICK] > CalSum[temp1])
			{
				temp1 = MOUSE_CLICK;
			}
			if (CalSum[MOV_UP] > CalSum[temp1])
			{
				temp1 = MOV_UP;
			}
			if (CalSum[MOV_LEFT] > CalSum[temp1])
			{
				temp1 = MOV_LEFT;
			}
			
			if (CalSum[temp1] > 20*(info.ValidArea[1].x-info.ValidArea[0].x)/ (FUNC_NUM_IN_TOTAL*5))
			{
				printf("action is %d,[%d]\r\n", temp1, CalSum[temp1]);
				for (k = 0; k < FUNC_NUM_IN_TOTAL; k++)
				{
					printf("--->%d,[%d]\r\n", k, CalSum[k]);
				}

				gpConfig->mouse_msg.func = (MOUSE_MESG_FUNC_AREA_t)temp1;
				gpConfig->mouse_msg.active = FUNC_ENABLE;

			}
#endif

#ifdef MOUSE_MOVE_WITH_RED
			//cal the average weight,and paint the mouse
			if (count)
			{
				Mouse_Pos[0].x = Mouse_Pos[0].x / count;
				Mouse_Pos[0].y = Mouse_Pos[0].y / count;


				if (gpConfig->pMKPlugin)
				{
#if 1

					//screen:	1500							800
					//scene:	original_frame.cols(640)		original_frame.rows(480, 60-420)

					i = Mouse_Pos[0].x * 1500 / original_frame.cols;
					//j = Pos[1].y * 800 / original_frame.rows;
					j = (Mouse_Pos[0].y - 60) * 800 / (420 - 60);

					if (gpConfig->MaxAreaValue.x > 1500)
					{
						i = Mouse_Pos[0].x * gpConfig->MaxAreaValue.x / original_frame.cols;
					}

					if (gpConfig->MaxAreaValue.y > 800)
					{
						j = (Mouse_Pos[0].y - 60) * gpConfig->MaxAreaValue.y / (420 - 60);
					}

					if ((Mouse_Pos[1].x != i) || (Mouse_Pos[1].y != j))
					{
						for (k = MOUSE_POS_BK_NUM - 1; k > 1; k--)
						{
							Mouse_Pos[k].x = Mouse_Pos[k - 1].x;
							Mouse_Pos[k].y = Mouse_Pos[k - 1].y;

						}

						Mouse_Pos[1].x = i;
						Mouse_Pos[1].y = j;

						i = 0;
						j = 0;
						for (k = 1; k < MOUSE_POS_BK_NUM; k++)
						{
							i += Mouse_Pos[k].x;
							j += Mouse_Pos[k].y;
						}

						MK_MoveMouse(i / (k - 1), j / (k - 1), gpConfig->pMKPlugin);
						printf("set %d[%d],%d[%d]\r\n", i, Mouse_Pos[1].x, j, Mouse_Pos[1].y);
					}
#endif
				}
				else
				{
					printf("pMKPlugin is not ready,should open #define TEST_2\r\n");
				}
			}
#endif
		}

		imshow(WIN_NAME, original_frame);
#else
		/*RGB Img data handle*/


		/*RGB TO GRAY*/
		cv::cvtColor(original_frame,
			original_frame,
			cv::COLOR_BGR2GRAY);
		/*GRAY Img data handle*/

		//GaussianBlur(original_frame, original_frame, original_frame.size(), 0, 0);
		blur(original_frame, original_frame, Size(3, 3));
		Canny(original_frame, original_frame, 3, 9, 3);

#if 0
		uchar* data = original_frame.data;
		for (i=0;i<original_frame.rows/2;i++)
		{
			for (j = 0; j < original_frame.cols*original_frame.channels()/2; j++)
			{
				data[j] = 0;
			}
			data += original_frame.step;
		}
#endif
		/*resize*/
		resize(original_frame, new_frame, new_frame.size());

		/*show picture*/
		imshow(WIN_NAME, new_frame);
#endif

		if (waitKey(20) > 0)
			break;

		if(FUNC_ENABLE == gpConfig->exit_flag)
			break;
	}
	cap.release();
	destroyAllWindows();
#endif

#ifdef OPENCV_OPENGL
	namedWindow(WIN_NAME, WINDOW_OPENGL);

	resizeWindow(WIN_NAME, win_width, win_height);

	setOpenGlContext(WIN_NAME);

	setOpenGlDrawCallback(WIN_NAME, on_opengl, NULL);

	cv::updateWindow(WIN_NAME);

	while (FUNC_DISABLE == gpConfig->exit_flag)
	{
		if (waitKey(20) > 0)
			break;
	}

	setOpenGlDrawCallback(WIN_NAME, 0, 0);
	destroyAllWindows();

#endif
}
#endif