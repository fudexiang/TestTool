
#include "Interfaces.h"

void *pAudioSocket;

void Audio_Demo_Test()
{
	pAudioSocket = CreatePlugin(PLUGIN_SOCKET, clsidSocketLib);
}