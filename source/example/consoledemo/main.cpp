#include "stdio.h"
#include "Interfaces.h"

extern void Audio_Demo_Test(void);
extern void Audio_Register_Tool_Test(void);

int test()
{
	uint16_t ret1= 0x12, ret2 = 0x21;

#if 0
	Audio_Demo_Test();
#else
	Audio_Register_Tool_Test();
#endif

	printf("Goodbye[%x][%x]\n",ret1,ret2);
	
	return 0;
}

int main()
{
	LoadPlugins();

	test();

	UnLoadPlugins();

	return 0;

}


