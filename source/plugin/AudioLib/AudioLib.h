#pragma once
#ifndef _AUDIOLIB_H_
#define _AUDIOLIB_H_

#include <module/classmacro.h>
#include <AudioLib/IAudioLib.h>

class CAudioLib: public IAudioLib
{
	X3BEGIN_CLASS_DECLARE(CAudioLib, clsidAudioLib)
		X3DEFINE_INTERFACE_ENTRY(IAudioLib)
		X3END_CLASS_DECLARE()

protected:
	CAudioLib();
	virtual ~CAudioLib();
	
public:
	HWAVEIN phwi;
	WAVEFORMATEX pwfx;
	WAVEHDR pwh[MAX_AUDIO_BUFFER_SLOT_NUM];

	Device_Status_t state;

	uint32_t count;
	uint8_t latest_full_slot;

	virtual int GetSoundInfo(char* pBuffer, int size);
	virtual CodeRet_t PrepareSoundHalConfig(AudioFormat_t* pFormatConfig, AudioBufferGroup_t* pBufferConfig);
	virtual void StartRecord(void);
	virtual void StopRecord(void);
	virtual uint32_t RecordSlotCount(void);
	virtual uint8_t LatestRecordSlot(void);
	virtual void StopClose(void);
};

#endif