#include "stdio.h"
#include "Interfaces.h"
#include "windows.h"
#include <process.h>
#include <TCHAR.H>
#if 1
#include "opencv2/opencv.hpp"

#include <iostream>

using namespace cv;
#endif
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

void Mytest1(Threads_Control_t* gpConfig)
{
	HANDLE hThread;
	DWORD  threadId;

	hThread = CreateThread(NULL, 0, WinThread, gpConfig, 0, &threadId); 

#if 1
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

	while (FUNC_DISABLE == gpConfig->exit_flag)
	{
		Sleep(1000);
	}
}