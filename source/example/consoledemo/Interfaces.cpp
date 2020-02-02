#include <portability/x3port.h>
#include <nonplugin/scanplugins.h>

#include <stdio.h>
#include "Loglib\ILogLib.h"
#include "Comlib\IComLib.h"
#include "HexStr\IHexStr.h"
#include "SocketLib\ISocketLib.h"

#include "Interfaces.h"

static x3::Object<ILogLib> *pLocalLog;

#define LOG_FILE_PRINTF(str) (*pLocalLog)->info(str)

void LoadPlugins(void)
{
	x3::loadScanPlugins();

	pLocalLog = new x3::Object<ILogLib>(clsidLogLib);
	(*pLocalLog)->CreateLogFile();
}

void UnLoadPlugins(void)
{
	(*pLocalLog)->SaveLogFile();
	x3::unloadScanPlugins();
}

void *CreatePlugin(PluginType type, const char *name)
{
	void *ret = NULL;
	switch (type)
	{
	case PLUGIN_LOG:
		ret = new x3::Object<ILogLib>(name);

		break;
	case PLUGIN_SOCKET:
		ret = new x3::Object<ISocketLib>(name);

		break;
	}

	if (NULL == ret)
	{
		LOG_FILE_PRINTF("create plugin failed\n");
	}
	else
	{
		LOG_FILE_PRINTF("create plugin ok\n");
	}

	return ret;
}

