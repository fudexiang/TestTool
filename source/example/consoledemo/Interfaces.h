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
	PLUGIN_AUDIO,
	PLUGIN_FFT,
	PLUGIN_FILTER,
	PLUGIN_HEX,
	PLUGIN_HTTP,
	PLUGIN_LOG,
	PLUGIN_MKOPE,
	PLUGIN_SOCKET,
	PLUGIN_SECURE,
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

	Rect_t MaxAreaValue;
	void* pMKPlugin;
}Threads_Control_t;

extern CodeRet_t AudioRecordConfig(AudioFormat_t* pFormatConfig, AudioBufferGroup_t* pBufferConfig, void* pPlugin);

extern void *CreatePlugin(PluginType_t type, const char *name);
extern CodeRet_t CalFFT(complex* pSample, uint32_t size, void* pPlugin);

extern int GetAudioLatestFullSlotNum(void* pPlugin);
extern int GetAudioSlotCount(void* pPlugin);
extern int GetAudioDeviceName(uint8_t* pText, int len, void* pPlugin);
extern int GetStrAscIIValues(char* pText, Unicode_Size_t uint_size, char* pBuffer, int buffer_len, void* pPlugin);
extern int GetMACInfo(uint8_t* pBuffer, uint8_t max, void* pPlugin);

extern void HexToStr(uint8_t val, char* pBuffer, Case_type_t type, void* pPlugin);
extern void Http_GetContext(const char* pURL, void* pBuffer, uint32_t max_len, void* pPlugin);
extern void Http_ResContext(void* pPlugin);

extern double IIR_FilterCal(double val, void* pPlugin);
extern void IIR_FilterConfig(FILTERINFO config, void* pPlugin);

extern void LoadPlugins(void);
extern void LogPrintf(LogPrintLevel level, const char* fmt, ...);

extern void MK_LeftClick(void* pPlugin);
extern void MK_RightClick(void* pPlugin);
extern void MK_MoveMouse(int x, int y, void* pPlugin);
extern void MK_EnterKey(char ButtonValue, void* pPlugin);

extern void StartAudioRecord(void* pPlugin);
extern void StopAudioRecord(void* pPlugin);
extern void CloseAudioRecord(void* pPlugin);

extern CodeRet_t TCPIP_SocketClientConnect(TCPIP_Client_t *pClient, SocktPlugin_t *pConfig);
extern int TCPIP_SocketClientRece(char* pbuffer, int size, SocktPlugin_t *pConfig);
extern int TCPIP_SocketClientSend(char* pbuffer, int size, SocktPlugin_t *pConfig);
extern void TCPIP_SocketGetIP(const char* pURL, const char* pIP, SocktPlugin_t* pConfig);
extern void TCPIP_CloseSocket(SocktPlugin_t *pConfig);
extern void UnLoadPlugins(void);

//#define URL_PART1 "https://www.sogou.com/tx?query="
#define URL_PART1	"https://www.so.com/s?ie=utf-8&src=hao_360so_a1004&shb=1&hsid=a571034dfb025d7d&q="

//#define URL_PART2	"%E5%A4%A7%E5%BA%B7%E5%86%9C%E4%B8%9A"
//#define URL_PART2	"%E5%AE%8F%E5%A4%A7%E7%88%86%E7%A0%B4"

#endif