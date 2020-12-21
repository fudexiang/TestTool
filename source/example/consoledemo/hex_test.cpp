#include "stdio.h"
#include "Interfaces.h"

void HexFileOperation(Threads_Control_t* p)
{
	uint8_t buffer[100];
	int len = 0, i;
	void* pHexPlugin;

	pHexPlugin = CreatePlugin(PLUGIN_HEX, clsidHexStr);

	if (pHexPlugin)
	{
		printf("create PLUGIN_HEX success[0x%x]\r\n",(int)pHexPlugin);

		HexToHex("C:\\Users\\DELL\\Desktop\\CortexM\\example_new\\scripts\\Func_call.hex", "2.hex", 0x10000000, pHexPlugin);
	}
	else
	{
		printf("create PLUGIN_HEX failed\r\n");
	}
}