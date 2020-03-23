#include <portability/x3port.h>
#include <nonplugin/scanplugins.h>

#include <stdio.h>
#include "Loglib\ILogLib.h"
#include "Comlib\IComLib.h"
#include "HexStr\IHexStr.h"
#include "SocketLib\ISocketLib.h"
#include "HttpLib\IHttpLib.h"
#include "AudioLib\IAudioLib.h"
#include "FFT\IFFTLib.h"
#include "FilterLib\IFilterLib.h"
#include "MKOpeLib\IMKOpeLib.h"
#include "Interfaces.h"

static x3::Object<ILogLib> *pLocalLog;

#define LOG_FILE_PRINTF(str) if(pLocalLog){(*pLocalLog)->info(str);}

CodeRet_t AudioRecordConfig(AudioFormat_t * pFormatConfig, AudioBufferGroup_t * pBufferConfig,void* pPlugin)
{
	return (*(x3::Object<IAudioLib>*)(pPlugin))->PrepareSoundHalConfig(pFormatConfig, pBufferConfig);
}

void *CreatePlugin(PluginType_t type, const char *name)
{
	void *ret = NULL;
	switch (type)
	{
	case PLUGIN_AUDIO:
		ret = new x3::Object<IAudioLib>(name);

		break;
	case PLUGIN_FFT:
		ret = new x3::Object<IFFTLib>(name);

		break;
	case PLUGIN_FILTER:
		ret = new x3::Object<IFilterLib>(name);

		break;
	case PLUGIN_HEX:
		ret = new x3::Object<IHexStr>(name);

		break;
	case PLUGIN_HTTP:
		ret = new x3::Object<IHttpLib>(name);

		break;
	case PLUGIN_LOG:
		ret = new x3::Object<ILogLib>(name);

		break;
	case PLUGIN_MKOPE:
		ret = new x3::Object<IMKOpeLib>(name);

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

CodeRet_t CalFFT(complex* pSample, uint32_t size,void* pPlugin)
{
	return (*(x3::Object<IFFTLib>*)(pPlugin))->fft(pSample, size);
}

int GetAudioDeviceName(uint8_t *pText, int len, void* pPlugin)
{
	return (*(x3::Object<IAudioLib>*)(pPlugin))->GetSoundInfo((char *)pText, len);
}

int GetAudioLatestFullSlotNum(void* pPlugin)
{
	return (*(x3::Object<IAudioLib>*)(pPlugin))->LatestRecordSlot();
}

int GetAudioSlotCount(void* pPlugin)
{
	return (*(x3::Object<IAudioLib>*)(pPlugin))->RecordSlotCount();
}

int GetStrAscIIValues(char* pText, Unicode_Size_t uint_size, char* pBuffer, int buffer_len,void* pPlugin)
{
	return (*(x3::Object<IHexStr>*)(pPlugin))->UnicodeToUTF8_Values(pText, uint_size, pBuffer, buffer_len);
}

void HexToStr(uint8_t val, char* pBuffer, Case_type_t type, void* pPlugin)
{
	//pBuffer[0] = 'A';
	//pBuffer[1] = 'b';
	(*(x3::Object<IHexStr>*)(pPlugin))->HexToStr(val, pBuffer, type);
}

void Http_GetContext(const char* pURL, void* pBuffer, uint32_t max_len, void* pPlugin)
{
	(*(x3::Object<IHttpLib>*)(pPlugin))->HttpContext(pURL, pBuffer, max_len);
}

void Http_ResContext(void* pPlugin)
{
	(*(x3::Object<IHttpLib>*)(pPlugin))->HttpResClear();
}

void IIR_FilterConfig(FILTERINFO config,void* pPlugin)
{
	(*(x3::Object<IFilterLib>*)(pPlugin))->Init_Filter(config.type.type, config.ts,config.type.fl,config.q);
}

double IIR_FilterCal(double val,void* pPlugin)
{
	return (*(x3::Object<IFilterLib>*)(pPlugin))->Filter(val);
}

void LoadPlugins(void)
{
	x3::loadScanPlugins();

	pLocalLog = new x3::Object<ILogLib>(clsidLogLib);
	(*pLocalLog)->CreateLogFile();

}

void LogPrintf(LogPrintLevel level,const char* fmt, ...)
{
	char LogBuff[0x100];
	int len;
	va_list args;
	va_start(args, fmt);
	len = vsprintf_s(LogBuff, 0x100, fmt, args);
	va_end(args);

	switch (level)
	{
	case log_trace:
		(*pLocalLog)->trace(LogBuff);
		break;
	case log_debug:
		(*pLocalLog)->debug(LogBuff);
		break;
	case log_info:
		(*pLocalLog)->info(LogBuff);
		break;
	case log_warn:
		(*pLocalLog)->warn(LogBuff);
		break;
	case log_error:
		(*pLocalLog)->error(LogBuff);
		break;
	case log_critical:
		(*pLocalLog)->critical(LogBuff);
		break;
	}
}

void MK_LeftClick(void* pPlugin)
{
	(*(x3::Object<IMKOpeLib>*)(pPlugin))->LeftClick();
}
void MK_RightClick(void* pPlugin)
{
	(*(x3::Object<IMKOpeLib>*)(pPlugin))->RightClick();
}
void MK_MoveMouse(int x, int y, void* pPlugin)
{
	(*(x3::Object<IMKOpeLib>*)(pPlugin))->MoveMouse(x,y);
}
void MK_EnterKey(char ButtonValue, void* pPlugin)
{
	(*(x3::Object<IMKOpeLib>*)(pPlugin))->EnterKey(ButtonValue);
}

void StartAudioRecord(void* pPlugin)
{
	return (*(x3::Object<IAudioLib>*)(pPlugin))->StartRecord();
}

void StopAudioRecord(void* pPlugin)
{
	return (*(x3::Object<IAudioLib>*)(pPlugin))->StopRecord();
}

void CloseAudioRecord(void* pPlugin)
{
	return (*(x3::Object<IAudioLib>*)(pPlugin))->StopClose();
}

CodeRet_t TCPIP_SocketClientConnect(TCPIP_Client_t *pClient,SocktPlugin_t *pConfig)
{
	CodeRet_t ret;
	ret =(*(x3::Object<ISocketLib> *)(pConfig->pAudioSocketPluginObject))->InitSocketClient(&(pConfig->Socket));
	if (RET_ERROR == ret)
	{
		LOG_FILE_PRINTF("socket init failed\n");
		return ret;
	}

	ret = (*(x3::Object<ISocketLib> *)(pConfig->pAudioSocketPluginObject))->InitSocketClientConnect((const char*)(pClient->pBuffer), pClient->port, &(pConfig->Socket));
	if (RET_ERROR == ret)
	{
		LOG_FILE_PRINTF("socket connect server failed\n");
		return ret;
	}
	else
	{
		LOG_FILE_PRINTF("socket connect server ok\n");
	}

	return RET_OK;
}

int TCPIP_SocketClientRece(char* pbuffer, int size, SocktPlugin_t *pConfig)
{
	return  (*(x3::Object<ISocketLib> *)(pConfig->pAudioSocketPluginObject))->SocketLib_Rece(pbuffer,size, &(pConfig->Socket));
}

int TCPIP_SocketClientSend(char* pbuffer, int size, SocktPlugin_t *pConfig)
{
	(*(x3::Object<ISocketLib> *)(pConfig->pAudioSocketPluginObject))->SocketLib_Send(pbuffer, size, &(pConfig->Socket));
	return 0;
}

void TCPIP_SocketGetIP(const char* pURL, const char* pIP,SocktPlugin_t* pConfig)
{
	(*(x3::Object<ISocketLib>*)(pConfig->pAudioSocketPluginObject))->SocketLib_GetIP(pURL, pIP);
}

void TCPIP_CloseSocket(SocktPlugin_t *pConfig)
{
	(*(x3::Object<ISocketLib> *)(pConfig->pAudioSocketPluginObject))->SocketLib_Close(&(pConfig->Socket));
}

void UnLoadPlugins(void)
{
	(*pLocalLog)->SaveLogFile();
	x3::unloadScanPlugins();
}

