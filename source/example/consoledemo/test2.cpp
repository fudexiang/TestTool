#include "stdio.h"
#include "Interfaces.h"

void Mytest2(Threads_Control_t* p)
{
	void* pMKPlugin;

	int count = 0;
	POINT mouse_pos;

	pMKPlugin = CreatePlugin(PLUGIN_MKOPE, clsidMKOpeLib);
	p->pMKPlugin = pMKPlugin;

	p->MaxAreaValue.x = 0;
	p->MaxAreaValue.y = 0;

	while (FUNC_ENABLE != p->exit_flag)
	{
		GetCursorPos(&mouse_pos);

		if (p->MaxAreaValue.x < mouse_pos.x)
			p->MaxAreaValue.x = mouse_pos.x;

		if (p->MaxAreaValue.y < mouse_pos.y)
			p->MaxAreaValue.y = mouse_pos.y;

		//printf("mouse pos x = %d,y = %d[%d][%d]\r\n", (int)mouse_pos.x, (int)mouse_pos.y,p->MaxAreaValue.x, p->MaxAreaValue.y);

		if (FUNC_ENABLE == p->mouse_msg.active)
		{
			switch (p->mouse_msg.func)
			{
			case MOV_LEFT:
				MK_MoveMouse(mouse_pos.x-10, mouse_pos.y, pMKPlugin);
				printf("-->MOV_LEFT\r\n");
				break;
			case MOV_UP:
				MK_MoveMouse(mouse_pos.x, mouse_pos.y-10, pMKPlugin);
				printf("-->MOV_UP\r\n");
				break;
			case MOUSE_CLICK:
				MK_LeftClick(pMKPlugin);
				printf("-->MOUSE_CLICK\r\n");
				break;
			case MOV_DOWN:
				MK_MoveMouse(mouse_pos.x, mouse_pos.y+10, pMKPlugin);
				printf("-->MOV_DOWN\r\n");
				break;
			case MOV_RIGHT:
				MK_MoveMouse(mouse_pos.x+10, mouse_pos.y, pMKPlugin);
				printf("-->MOV_RIGHT\r\n");
				break;
			}
			p->mouse_msg.active = FUNC_DISABLE;
			Sleep(10);
		}

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