#include "stdio.h"
#include "Interfaces.h"

void Mytest3(Threads_Control_t* p)
{
	uint8_t buffer[100];
	int len = 0,i;
	void* pSecurePlugin;

	pSecurePlugin = CreatePlugin(PLUGIN_SECURE, clsidSecureLib);

	len = GetMACInfo(buffer, 100, pSecurePlugin);

	printf("computer MAC[%d]:",len);
	for (i = 0; i < len; i++)
	{
		printf("0x%x ", buffer[i]);
	}
	printf("\r\n");
}