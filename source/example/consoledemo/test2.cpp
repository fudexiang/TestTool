#include "stdio.h"
#include "Interfaces.h"

void Mytest2(Threads_Control_t* p)
{
	void* pMKPlugin;

	int count = 0;
	POINT mouse_pos;

	pMKPlugin = CreatePlugin(PLUGIN_MKOPE, clsidMKOpeLib);
	p->pMKPlugin = pMKPlugin;

	while (FUNC_ENABLE != p->exit_flag)
	{
		GetCursorPos(&mouse_pos);

		printf("mouse pos x = %d,y = %d\r\n", mouse_pos.x, mouse_pos.y);

		Sleep(2000);

	}

#if 0
	if (pMKPlugin)
	{
		MK_MoveMouse(1466, 34, pMKPlugin);
		MK_LeftClick(pMKPlugin);
		MK_LeftClick(pMKPlugin);

		MK_MoveMouse(639, 21, pMKPlugin);
		MK_LeftClick(pMKPlugin);
		MK_LeftClick(pMKPlugin);


		Sleep(1000);
		MK_MoveMouse(610, 374, pMKPlugin);
		Sleep(1000);
		MK_LeftClick(pMKPlugin);
		Sleep(1000);
		MK_LeftClick(pMKPlugin);
		Sleep(1000);
		MK_EnterKey('F', pMKPlugin);
		Sleep(1000);

		MK_EnterKey('D', pMKPlugin);
		Sleep(1000);

		MK_EnterKey('X', pMKPlugin);

		MK_MoveMouse(114, 626, pMKPlugin);
		MK_RightClick(pMKPlugin);
	}
#endif
}