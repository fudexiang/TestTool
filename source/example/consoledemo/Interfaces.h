#pragma once
#ifndef _APP_INTERFACE_H_
#define _APP_INTERFACE_H_

#include "Common/DataType.h"
#include "Common/Config.h"

enum PluginType
{
	PLUGIN_LOG,
	PLUGIN_SOCKET,
};

extern void LoadPlugins(void);
extern void UnLoadPlugins(void);

extern void *CreatePlugin(PluginType type, const char *name);

#endif