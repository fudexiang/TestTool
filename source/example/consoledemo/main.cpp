#include "stdio.h"
#include "Interfaces.h"

extern void Audio_Demo_Test();

int test()
{
	uint16_t ret1= 0x12, ret2 = 0x21;

	Audio_Demo_Test();

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


