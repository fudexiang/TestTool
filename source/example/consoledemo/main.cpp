#include "stdio.h"
#include "Interfaces.h"

#include "test_config.h"
#include <process.h>

DWORD WINAPI ThreadFunc1(LPVOID p)
{
	Threads_Control_t *pControl = (Threads_Control_t *)p;

#ifdef HEX_TST
	HexFileOperation(pControl);
#endif

#ifdef TEST_1
	Mytest1(pControl);
#endif

#ifdef PLAY_MUSIC
	Audio_Demo_Test(pControl);
#endif

#ifdef REGISTER_TOOL
	Audio_Register_Tool_Test(pControl);
#endif

#if TEST_10
	MyWebtest();
#endif

#ifdef TEST_11
	Mytest11();
#endif

#ifdef AUDIO_PLAYER_CAPTURE
	WasTest(pControl);
#endif

	pControl->Thread_count -= 1;
	return 0;
}

DWORD WINAPI ThreadFunc2(LPVOID p)
{
	Threads_Control_t* pControl = (Threads_Control_t*)p;

#ifdef TEST_2
	Mytest2(pControl);
#endif

#ifdef TEST_3
	Mytest3(pControl);
#endif

#ifdef KEIL_MAP_FILE_OPERATE
	KeilMapFileOperation(pControl);
#endif

	pControl->Thread_count -= 1;
	return 0;
}

int main(int argc, char* argv[])
{
	
	HANDLE hThread;
	DWORD  threadId;
	uint8_t buffer[0x100];
	Threads_Control_t thread_control;

#if 0
	//create folder
	system("md abc");
#endif

	LoadPlugins();

	thread_control.pMKPlugin = NULL;
	thread_control.pause_flag = FUNC_DISABLE;
	thread_control.exit_flag = FUNC_DISABLE;

	thread_control.Thread_count = 1;
	hThread = CreateThread(NULL, 0, ThreadFunc1, &thread_control, 0, &threadId);

	thread_control.Thread_count++;
	hThread = CreateThread(NULL, 0, ThreadFunc2, &thread_control, 0, &threadId);

	while (thread_control.Thread_count > 0)
	{

		Sleep(1);

#ifdef PLAY_MUSIC
		printf("please input command 0:\n");

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
#endif

#ifdef END_APP_BY_Q
		printf("please q or Q to exit\n");

		scanf("%s", buffer);

		if (buffer[0] == 'q' || buffer[0] == 'Q')
		{
			thread_control.exit_flag = FUNC_ENABLE;
			while (thread_control.Thread_count > 0)
			{
				Sleep(1);
			}
			break;
		}
#endif

	}

	UnLoadPlugins();

	return 0;

}


