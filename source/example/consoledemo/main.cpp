#include "stdio.h"
#include "Interfaces.h"

extern void Audio_Demo_Test(Threads_Control_t *pControl);
extern void Audio_Register_Tool_Test(Threads_Control_t *pControl);


DWORD WINAPI ThreadFunc(LPVOID p)
{
	Threads_Control_t *pControl = (Threads_Control_t *)p;

#if 1
	Audio_Demo_Test(pControl);
#else
	Audio_Register_Tool_Test();
#endif

	pControl->Thread_count -= 1;
	return 0;
}

int main()
{
	
	HANDLE hThread;
	DWORD  threadId;
	uint8_t buffer[0x100];
	Threads_Control_t thread_control;

	LoadPlugins();

	thread_control.Thread_count = 1;
	thread_control.pause_flag = FUNC_DISABLE;
	thread_control.exit_flag = FUNC_DISABLE;

	hThread = CreateThread(NULL, 0, ThreadFunc, &thread_control, 0, &threadId); // ¡ä¡ä?¡§??3¨¬

	while (thread_control.Thread_count > 0)
	{
		printf("please input command:\n");

		scanf("%s", buffer);

		switch (buffer[0])
		{
		case 'q':
		case 'Q':
			thread_control.exit_flag = FUNC_ENABLE;
			while (thread_control.Thread_count > 0)
			{
				Sleep(1);
			}
			break;
		case 's':
		case 'S':
			thread_control.pause_flag = FUNC_ENABLE;
			break;
		case 'g':
		case 'G':
			thread_control.pause_flag = FUNC_DISABLE;
			break;
		}
	}

	UnLoadPlugins();

	return 0;

}


