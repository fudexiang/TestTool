#include "stdio.h"
#include "Interfaces.h"
#include "windows.h"
#include <process.h>
#include <TCHAR.H>

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
//#include "opencv2/imgcodecs.hpp"
//#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;

#define WIN_NAME "video"

//#define CreateWins
//#define OPENCV_SHOW_IMG
#define OPENCV_CAMERA_OPE

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

#define POS_BK_NUM	10
Point_Location_t Pos[POS_BK_NUM];

void on_mouse(int EVENT, int x, int y, int flags, void* userdata)
{
	Mat hh;
	hh = *(Mat*)userdata;
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
	}
	break;

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
	int i, j,k,channel,count;
	VideoCapture cap;
	Mat original_frame,new_frame;

	new_frame = Mat::zeros(256, 256, CV_8UC3);

	cap.open(0);

	if (!cap.isOpened())
		return;

	cap.read(original_frame);

	printf("row = %d,col = %d,channels = %d\r\n", original_frame.rows, original_frame.cols, original_frame.channels());

#if 0
	printf("row0 = %d,col = %d,channels = %d\r\n", new_frame.rows, new_frame.cols, new_frame.channels());
	cv::cvtColor(original_frame,original_frame,cv::COLOR_BGR2GRAY);
	resize(original_frame, new_frame, new_frame.size());
	printf("row1 = %d,col = %d,channels = %d\r\n", new_frame.rows, new_frame.cols, new_frame.channels());
#endif

	namedWindow(WIN_NAME, 0);
#define ORIGINAL_IMG

#ifdef ORIGINAL_IMG
	setMouseCallback(WIN_NAME, on_mouse, &original_frame);
#else
	setMouseCallback(WIN_NAME, on_mouse, &new_frame);
#endif

	while (1)
	{
		/*get original img*/
		cap.read(original_frame);

#ifdef ORIGINAL_IMG
#if 1
		uchar* data = original_frame.data;
		uchar temp;
		channel = original_frame.channels();

		if (3 != channel)
			break;

		Pos[0].x = 0;
		Pos[0].y = 0;
		count = 0;

		for (i = 0; i < original_frame.rows; i++)
		{
			for (j = 0; j <= original_frame.cols/2;j++ )
			{
				temp = data[j * channel];
				data[j * channel] = data[(original_frame.cols-1-j) * channel];
				data[(original_frame.cols - 1 - j) * channel] = temp;

				temp = data[j * channel + 1];
				data[j * channel + 1] = data[(original_frame.cols - 1 - j) * channel + 1];
				data[(original_frame.cols - 1 - j) * channel + 1] = temp;

				temp = data[j * channel + 2];
				data[j * channel + 2] = data[(original_frame.cols - 1 - j) * channel + 2];
				data[(original_frame.cols - 1 - j) * channel + 2] = temp;
			}

			data += original_frame.step;
		}

		data = original_frame.data;
		for (i = 0; i < original_frame.rows; i++)
		{
			for (j = 0; j < original_frame.cols * channel; )
			{
				if ((data[j] > 20) || (data[j + 2] < 200)) 
				{
//					data[j] = 0;
//					data[j+1] = 0;
//					data[j+2] = 0;
				}
				else
				{
					Pos[0].x += j / channel;
					Pos[0].y += i;
					count++;
				}
				j += channel;
			}
			data += original_frame.step;
		}
		if (count)
		{
			Pos[0].x = Pos[0].x / count;
			Pos[0].y = Pos[0].y / count;


			if (gpConfig->pMKPlugin)
			{
#if 1

				//screen:	1500							800
				//scene:	original_frame.cols(640)		original_frame.rows(480, 60-420)	
				i = Pos[0].x * 1500 / original_frame.cols;
				//j = Pos[1].y * 800 / original_frame.rows;
				j = (Pos[0].y-60) * 800 / (420-60);

				if ((Pos[1].x != i) || (Pos[1].y != j))
				{
					for (k = POS_BK_NUM - 1; k >1 ; k--)
					{
						Pos[k].x = Pos[k - 1].x;
						Pos[k].y = Pos[k - 1].y;

					}

					Pos[1].x = i;
					Pos[1].y = j;

					i = 0;
					j = 0;
					for (k = 1; k < POS_BK_NUM; k++)
					{
						i += Pos[k].x;
						j += Pos[k].y;
					}

					MK_MoveMouse(i/(k-1), j/(k-1), gpConfig->pMKPlugin);
					printf("set %d[%d],%d[%d]\r\n", i, Pos[1].x,j, Pos[1].y);
				}
#endif
			}
			else
			{
				printf("pMKPlugin is not ready\r\n");
			}
		}

#endif
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

#if 1
		if (waitKey(20) > 0)
			break;
#endif

		if(FUNC_ENABLE == gpConfig->exit_flag)
			break;
	}
	cap.release();
	destroyAllWindows();
#endif
}