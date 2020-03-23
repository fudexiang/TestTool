#include <module/plugininc.h>		/* important */
#include "MKOpeLib.h"

CMKOpeLib::CMKOpeLib()
{

}

CMKOpeLib::~CMKOpeLib()
{

}

void CMKOpeLib::EnterKey(char ButtonValue)
{
	INPUT    Input = { 0 };
	//down 
	/*
	Input.type = INPUT_KEYBOARD;
	Input.ki.wVk = ButtonValue;
	::SendInput(1, &Input, sizeof(INPUT));
	*/
	//up
	::ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_KEYBOARD;
	Input.ki.wVk = ButtonValue;
	Input.mi.dwFlags = KEYEVENTF_KEYUP;
	::SendInput(1, &Input, sizeof(INPUT));
}

void CMKOpeLib::LeftClick(void)
{
	INPUT    Input = { 0 };
	// left down 
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	::SendInput(1, &Input, sizeof(INPUT));

	// left up
	::ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	::SendInput(1, &Input, sizeof(INPUT));
}

void CMKOpeLib::RightClick(void)
{
	INPUT    Input = { 0 };
	// right down 
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	::SendInput(1, &Input, sizeof(INPUT));

	// right up
	::ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	::SendInput(1, &Input, sizeof(INPUT));
}

void CMKOpeLib::MoveMouse(int x, int y)
{
	SetCursorPos(x, y);
}
