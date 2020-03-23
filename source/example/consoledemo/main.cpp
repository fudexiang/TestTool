#include "stdio.h"
#include "Interfaces.h"

//#define PLAY_MUSIC
//#define REGISTER_TOOL
//#define TEST_0
#define TEST_1
//#define TEST_2
//#define TEST_11

#ifdef PLAY_MUSIC
extern void Audio_Demo_Test(Threads_Control_t *pControl);
#endif
#ifdef REGISTER_TOOL
extern void Audio_Register_Tool_Test(Threads_Control_t *pControl);
#endif
#ifdef TEST_0
extern void MyWebtest(void);
#endif

#ifdef TEST_1
extern void Mytest1(Threads_Control_t* p);
#endif

#ifdef TEST_2
extern void Mytest2(Threads_Control_t *p);
#endif

#ifdef TEST_11
extern void Mytest11(void);
#endif



DWORD WINAPI ThreadFunc(LPVOID p)
{
	Threads_Control_t *pControl = (Threads_Control_t *)p;
#if TEST_0
	MyWebtest();
#endif

#ifdef TEST_1
	Mytest1(pControl);
#endif

#ifdef TEST_2
	Mytest2(pControl);
#endif
#ifdef PLAY_MUSIC
	Audio_Demo_Test(pControl);
#endif

#ifdef REGISTER_TOOL
	Audio_Register_Tool_Test(pControl);
#endif

#ifdef TEST_11
	Mytest11();
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

#if 1
	//system("md abc");
#endif

	LoadPlugins();

	thread_control.Thread_count = 1;
	thread_control.pause_flag = FUNC_DISABLE;
	thread_control.exit_flag = FUNC_DISABLE;

	hThread = CreateThread(NULL, 0, ThreadFunc, &thread_control, 0, &threadId); // ¡ä¡ä?¡§??3¨¬

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
#else
#if 1
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
#endif

	}

	UnLoadPlugins();

	return 0;

}


