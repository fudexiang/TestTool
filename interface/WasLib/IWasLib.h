#pragma once
#ifndef _IWASLIB_H_
#define _IWASLIB_H_

#include "Common/DataType.h"
#include "Common/Config.h"
#include "core/objptr.h"
#include "stdio.h"

class IWasLib : public x3::IObject
{
	X3DEFINE_IID(IWasLib);

	virtual CodeRet_t WasLib_Init(AUDIO_FORMAT_USER_DEFINE_t format, AudioDataInfo_t* pAudioDataInfo) = 0;
	virtual CodeRet_t WasLib_Exit(void) = 0;
	virtual CodeRet_t WasLib_ReadData(AudioDataInfo_t* pAudioDataInfo) = 0;
};


#endif