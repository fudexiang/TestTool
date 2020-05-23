#pragma once
#ifndef _WASLIB_H_
#define _WASLIB_H_

#include <module/classmacro.h>
#include <WasLib/IWasLib.h>
#include <mmdeviceapi.h>
#include <Audioclient.h>

class CWasLib : public IWasLib
{
	X3BEGIN_CLASS_DECLARE(CWasLib, clsidWasLib)
		X3DEFINE_INTERFACE_ENTRY(IWasLib)
		X3END_CLASS_DECLARE()

protected:
	CWasLib();
	virtual ~CWasLib();
public:

	HRESULT hr;
	WAVEFORMATEX* pwfx;
	UINT32 bufferFrameCount;
	//    REFERENCE_TIME  hnsActualDuration;
	REFERENCE_TIME  hnsDefaultDevicePeriod;
	IMMDevice* pDevice = NULL;
	IAudioClient* pAudioClient = NULL;
	IAudioCaptureClient* pCaptureClient = NULL;
	IMMDeviceEnumerator* pEnumerator = NULL;

	//HANDLE                  hTimerWakeUp = NULL;
	//HANDLE waitArray[1] = { hTimerWakeUp };
	//LARGE_INTEGER liFirstFire;
	//LONG lTimeBetweenFires;

	virtual CodeRet_t WasLib_Init(AUDIO_FORMAT_USER_DEFINE_t format, AudioDataInfo_t* pAudioDataInfo);
	virtual CodeRet_t WasLib_ReadData(AudioDataInfo_t* pAudioDataInfo);
	virtual CodeRet_t WasLib_Exit(void);

};

#endif