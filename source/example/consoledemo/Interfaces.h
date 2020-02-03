#pragma once
#ifndef _APP_INTERFACE_H_
#define _APP_INTERFACE_H_

#include "Common/DataType.h"
#include "Common/Config.h"

#include <WINSOCK2.H>

typedef enum
{
	FUNC_ENABLE,
	FUNC_DISABLE,
}Func_Enable_t;

typedef enum 
{
	PLUGIN_LOG,
	PLUGIN_SOCKET,
}PluginType_t;

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

typedef struct
{
	int Thread_count;
	Func_Enable_t pause_flag;
	Func_Enable_t exit_flag;
}Threads_Control_t;

extern void LoadPlugins(void);
extern void UnLoadPlugins(void);

extern void *CreatePlugin(PluginType_t type, const char *name);
extern void LogPrintf(LogPrintLevel level, const char* fmt, ...);

extern CodeRet_t TCPIP_SocketClientConnect(TCPIP_Client_t *pClient, SocktPlugin_t *pConfig);

extern int TCPIP_SocketClientRece(char* pbuffer, int size, SocktPlugin_t *pConfig);
extern int TCPIP_SocketClientSend(char* pbuffer, int size, SocktPlugin_t *pConfig);
extern void TCPIP_CloseSocket(SocktPlugin_t *pConfig);
#endif