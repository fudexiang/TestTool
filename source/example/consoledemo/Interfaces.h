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
	PLUGIN_HEX,
	PLUGIN_HTTP,
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

extern int GetStrAscIIValues(char* pText, Unicode_Size_t uint_size, char* pBuffer, int buffer_len, void* pPlugin);
extern void HexToStr(uint8_t val, char* pBuffer, Case_type_t type, void* pPlugin);

extern void Http_GetContext(const char* pURL, void* pBuffer, uint32_t max_len, void* pPlugin);
extern void Http_ResContext(void* pPlugin);

extern void LogPrintf(LogPrintLevel level, const char* fmt, ...);

extern CodeRet_t TCPIP_SocketClientConnect(TCPIP_Client_t *pClient, SocktPlugin_t *pConfig);

extern int TCPIP_SocketClientRece(char* pbuffer, int size, SocktPlugin_t *pConfig);
extern int TCPIP_SocketClientSend(char* pbuffer, int size, SocktPlugin_t *pConfig);
extern void TCPIP_SocketGetIP(const char* pURL, const char* pIP, SocktPlugin_t* pConfig);
extern void TCPIP_CloseSocket(SocktPlugin_t *pConfig);

//#define URL_PART1 "https://www.sogou.com/tx?query="
#define URL_PART1	"https://www.so.com/s?ie=utf-8&src=hao_360so_a1004&shb=1&hsid=a571034dfb025d7d&q="

//#define URL_PART2	"%E5%A4%A7%E5%BA%B7%E5%86%9C%E4%B8%9A"
//#define URL_PART2	"%E5%AE%8F%E5%A4%A7%E7%88%86%E7%A0%B4"

#endif