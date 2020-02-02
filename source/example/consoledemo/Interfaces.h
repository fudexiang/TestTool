#pragma once
#ifndef _APP_INTERFACE_H_
#define _APP_INTERFACE_H_

#include "Common/DataType.h"
#include "Common/Config.h"

#include <WINSOCK2.H>

enum PluginType
{
	PLUGIN_LOG,
	PLUGIN_SOCKET,
};

enum LogPrintLevel
{
	log_trace,
	log_debug,
	log_info,
	log_warn,
	log_error,
	log_critical,
};

typedef struct
{
	SOCKET Socket;
	void *pAudioSocketPluginObject;
}SocktPlugin_t;

typedef struct
{
	char *pBuffer;
	u_short port;
	SOCKET *pSocket;
}TCPIP_Client_t;

extern void LoadPlugins(void);
extern void UnLoadPlugins(void);

extern void *CreatePlugin(PluginType type, const char *name);
extern void LogPrintf(LogPrintLevel level, const char* fmt, ...);

extern CodeRet_t TCPIP_SocketClientConnect(TCPIP_Client_t *pClient, SocktPlugin_t *pConfig);

extern int TCPIP_SocketClientRece(char* pbuffer, int size, SocktPlugin_t *pConfig);
extern int TCPIP_SocketClientSend(char* pbuffer, int size, SocktPlugin_t *pConfig);
#endif