#pragma once
#ifndef _IAUDIOLIB_H_

#define _IAUDIOLIB_H_

#include "Common/DataType.h"
#include "Common/Config.h"
#include "core/objptr.h"

class IAudioLib : public x3::IObject
{
	X3DEFINE_IID(IAudioLib);

	virtual int GetSoundInfo(char* pBuffer, int size) = 0;
	virtual uint8_t LatestRecordSlot(void) = 0;

	virtual CodeRet_t PrepareSoundHalConfig(AudioFormat_t* pFormatConfig, AudioBufferGroup_t* pBufferConfig) = 0;
	virtual uint32_t RecordSlotCount(void) = 0;

	virtual void StartRecord(void) = 0;
	virtual void StopRecord(void) = 0;
	virtual void StopClose(void) = 0;


};


#endif