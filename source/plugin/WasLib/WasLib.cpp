#include <module/plugininc.h>		/* important */
#include "WasLib.h"

#define EXIT_ON_ERROR(hres)  \
    if (FAILED(hres)) { return RET_PARAM_ERROR; }

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioClient = __uuidof(IAudioClient);
const IID IID_IAudioCaptureClient = __uuidof(IAudioCaptureClient);

CWasLib::CWasLib()
{
	hr = NULL;
	//    REFERENCE_TIME  hnsActualDuration;
	bufferFrameCount = 0;
	hnsDefaultDevicePeriod = 0;

	//liFirstFire.QuadPart = -hnsDefaultDevicePeriod / 2; // negative means relative time
	//lTimeBetweenFires = (LONG)hnsDefaultDevicePeriod / 2 / (10 * 1000);
}

CWasLib::~CWasLib()
{

}

CodeRet_t AdjustFormatTo16Bits(WAVEFORMATEX* pwfx, AUDIO_FORMAT_USER_DEFINE_t format)
{
	uint8_t bits_per_sample = 0xff;

	switch (format)
	{
	case AUDIO_USER_FORMAT_U8:
		bits_per_sample = 8;
		break;
	case AUDIO_USER_FORMAT_S16_BE:
		break;
	case AUDIO_USER_FORMAT_S16_LE:
		bits_per_sample = 16;
		break;
	case AUDIO_USER_FORMAT_S24_3BE:
		break;
	case AUDIO_USER_FORMAT_S24_3LE:
		bits_per_sample = 24;
		break;
	case AUDIO_USER_FORMAT_S24_BE:
		break;
	case AUDIO_USER_FORMAT_S24_LE:
		bits_per_sample = 24;
		break;
	case AUDIO_USER_FORMAT_S32_BE:
		break;
	case AUDIO_USER_FORMAT_S32_LE:
		bits_per_sample = 32;
		break;
	case AUDIO_USER_FORMAT_FLOAT_BE:
		break;
	case AUDIO_USER_FORMAT_FLOAT_LE:
		break;
	}

	if (bits_per_sample > 32)
	{
		return RET_PARAM_ERROR;
	}

	if (pwfx->wFormatTag == WAVE_FORMAT_IEEE_FLOAT)
	{
		pwfx->wFormatTag = WAVE_FORMAT_PCM;
	}
	else if (pwfx->wFormatTag == WAVE_FORMAT_EXTENSIBLE)
	{
		PWAVEFORMATEXTENSIBLE pEx = reinterpret_cast<PWAVEFORMATEXTENSIBLE>(pwfx);
		if (IsEqualGUID(KSDATAFORMAT_SUBTYPE_IEEE_FLOAT, pEx->SubFormat))
		{
			pEx->SubFormat = KSDATAFORMAT_SUBTYPE_PCM;
			pEx->Samples.wValidBitsPerSample = bits_per_sample;
		}
	}
	else
	{
		return RET_PARAM_ERROR;
	}
	pwfx->wBitsPerSample = bits_per_sample;
	pwfx->nBlockAlign = pwfx->nChannels * pwfx->wBitsPerSample / 8;
	pwfx->nAvgBytesPerSec = pwfx->nBlockAlign * pwfx->nSamplesPerSec;

#if 0
	buffer[0] = 'p';
	buffer[1] = SND_PCM_FORMAT_S16_LE;
	buffer[2] = pwfx->nChannels;
	buffer[3] = (pwfx->nSamplesPerSec >> 24) & 0xff;
	buffer[4] = (pwfx->nSamplesPerSec >> 16) & 0xff;
	buffer[5] = (pwfx->nSamplesPerSec >> 8) & 0xff;
	buffer[6] = (pwfx->nSamplesPerSec >> 0) & 0xff;
	send(tuning_tool_sclient, (const char*)buffer, 7, 0);

	recv(tuning_tool_sclient, (char*)buffer, 255, 0);
#endif
	return RET_OK;
}

CodeRet_t CWasLib::WasLib_Init(AUDIO_FORMAT_USER_DEFINE_t format, AudioDataInfo_t* pAudioDataInfo)
{
	CoInitialize(NULL);

	hr = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&pEnumerator);
	EXIT_ON_ERROR(hr)

	hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
	EXIT_ON_ERROR(hr)

	hr = pDevice->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&pAudioClient);
	EXIT_ON_ERROR(hr)

	hr = pAudioClient->GetDevicePeriod(&hnsDefaultDevicePeriod, NULL);

	hr = pAudioClient->GetMixFormat(&pwfx);
	EXIT_ON_ERROR(hr)

	if (RET_OK != AdjustFormatTo16Bits(pwfx, format))
	{
		return RET_PARAM_ERROR;
	}

	//hTimerWakeUp = CreateWaitableTimer(NULL, FALSE, NULL);

	hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_LOOPBACK, 0, 0, pwfx, NULL);
	EXIT_ON_ERROR(hr)

	// Get the size of the allocated buffer.
	hr = pAudioClient->GetBufferSize(&bufferFrameCount);
	EXIT_ON_ERROR(hr)

	hr = pAudioClient->GetService(IID_IAudioCaptureClient, (void**)&pCaptureClient);
	EXIT_ON_ERROR(hr)

	//SetWaitableTimer(hTimerWakeUp, &liFirstFire, lTimeBetweenFires * 3, NULL, NULL, FALSE);

	// Calculate the actual duration of the allocated buffer.
	// hnsActualDuration = (double)REFTIMES_PER_SEC * bufferFrameCount / pwfx->nSamplesPerSec;

	hr = pAudioClient->Start();  // Start recording.
	EXIT_ON_ERROR(hr)

	pAudioDataInfo->buffer_max_len = hnsDefaultDevicePeriod;
	pAudioDataInfo->frame_num = bufferFrameCount;

	return RET_OK;
}

CodeRet_t CWasLib::WasLib_ReadData(AudioDataInfo_t *pAudioDataInfo)
{
	uint32_t len;
	DWORD flags;
	BYTE* pData = NULL;
	UINT32 packetLength = 0;
	UINT32 numFramesAvailable = 0;

	hr = pCaptureClient->GetNextPacketSize(&packetLength);
	EXIT_ON_ERROR(hr)

	if (packetLength != 0)
	{
		// Get the available data in the shared buffer.
		hr = pCaptureClient->GetBuffer(&pData, &numFramesAvailable, &flags, NULL, NULL);
		EXIT_ON_ERROR(hr)

		// Copy the available capture data to the audio sink.
		len = numFramesAvailable * pwfx->nBlockAlign;

		pAudioDataInfo->buffer_size_need = len;

		if (len > 0)
		{
			if (len > pAudioDataInfo->buffer_max_len)
			{
				len = pAudioDataInfo->buffer_max_len;
			}
			pAudioDataInfo->frame_num = numFramesAvailable;
			pAudioDataInfo->buffer_valid_size = len;
			memcpy(pAudioDataInfo->pBuffer, pData, len);
		}
		else
		{
			pAudioDataInfo->frame_num = 0;
			pAudioDataInfo->buffer_valid_size = 0;
		}
		pAudioDataInfo->flags = flags;

		hr = pCaptureClient->ReleaseBuffer(numFramesAvailable);
		EXIT_ON_ERROR(hr)
	}
	return RET_OK;
}

CodeRet_t CWasLib::WasLib_Exit(void)
{
	CoUninitialize();

	return RET_OK;
}

