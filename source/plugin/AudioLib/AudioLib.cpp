#include "stdio.h"
#include <module/plugininc.h>		/* important */
#include "AudioLib.h"
#include "mmsystem.h"

#pragma comment(lib, "winmm.lib")

CAudioLib::CAudioLib()
{
	state = STATUS_IDLE;
}

CAudioLib::~CAudioLib()
{

}

int CAudioLib::GetSoundInfo(char *pBuffer,int size)
{
	int count = 0,len=0;
	MMRESULT mmResult;
	WAVEINCAPS waveIncaps;

	count = waveInGetNumDevs();

	mmResult = waveInGetDevCaps(0, &waveIncaps, sizeof(WAVEINCAPS));

	if (MMSYSERR_NOERROR == mmResult)
	{

		if (NULL != pBuffer)
		{
#if 0
			len = WideCharToMultiByte(CP_ACP, 0, waveIncaps.szPname, -1, NULL, 0, NULL, NULL);
			if (len < size)
			{
				pBuffer[len] = '\0';
			}
			else
			{
				len = size - 1;
				pBuffer[size - 1] = '\0';
			}
			WideCharToMultiByte(CP_ACP, 0, waveIncaps.szPname, -1, pBuffer, len, NULL, NULL);
#else

			len = WideCharToMultiByte(CP_ACP, 0, (LPCWCH)waveIncaps.szPname, wcslen((wchar_t const* )waveIncaps.szPname), NULL, 0, NULL, NULL);

			WideCharToMultiByte(CP_ACP, 0, (LPCWCH)waveIncaps.szPname, wcslen((wchar_t const* )waveIncaps.szPname), pBuffer, len, NULL, NULL);
			pBuffer[len] = '\0';
#endif
		}

	}
	else
	{
		//error
	}

	return count;
}

DWORD CALLBACK MicCallback(HWAVEIN hwavein, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	CAudioLib* pAudioLib = (CAudioLib*)dwInstance;
	switch (uMsg)
	{
	case WIM_OPEN:
		pAudioLib->state = STATUS_OPENED;

		break;

	case WIM_DATA:
		pAudioLib->count++;
		pAudioLib->latest_full_slot = ((LPWAVEHDR)dwParam1)->dwUser;

		switch (pAudioLib->state)
		{
		case STATUS_WORKING:
			waveInAddBuffer(hwavein, (LPWAVEHDR)dwParam1, sizeof(WAVEHDR));
			break;
		case STATUS_CLOSED:
			waveInReset(pAudioLib->phwi);
			break;
		}
		break;

	case WIM_CLOSE:
		pAudioLib->state = STATUS_CLOSED;

		break;
	default:
		break;
	}
	return 0;
}

void WaveInitFormat(LPWAVEFORMATEX m_WaveFormat, WORD nCh, DWORD nSampleRate, WORD BitsPerSample)
{
	m_WaveFormat->wFormatTag = WAVE_FORMAT_PCM;
	m_WaveFormat->nChannels = nCh;
	m_WaveFormat->nSamplesPerSec = nSampleRate;
	m_WaveFormat->nAvgBytesPerSec = nSampleRate * nCh * BitsPerSample / 8;
	m_WaveFormat->nBlockAlign = m_WaveFormat->nChannels * BitsPerSample / 8;
	m_WaveFormat->wBitsPerSample = BitsPerSample;
	m_WaveFormat->cbSize = 0;
}

CodeRet_t CAudioLib::PrepareSoundHalConfig(AudioFormat_t* pFormatConfig,AudioBufferGroup_t* pBufferConfig)
{
	int i;
	MMRESULT mmResult;
	CodeRet_t ret = RET_OK;

	WaveInitFormat(&pwfx, pFormatConfig->nCh, pFormatConfig->nSampleRate, pFormatConfig->BitsPerSample);

	mmResult = waveInOpen(&phwi, WAVE_MAPPER, &pwfx, (DWORD)(MicCallback), (DWORD)this, CALLBACK_FUNCTION);//3

	if (MMSYSERR_NOERROR == mmResult)
	{
		for (i = 0; i < pBufferConfig->buffer_slot_num; i++)
		{
			pwh[i].lpData = (LPSTR)pBufferConfig->Slot[i].pAddr;
			pwh[i].dwBufferLength = pBufferConfig->Slot[i].len;
			pwh[i].dwUser = i;
			pwh[i].dwFlags = 0;
			mmResult = waveInPrepareHeader(phwi, &pwh[i], sizeof(WAVEHDR));//4

			if (MMSYSERR_NOERROR != mmResult)
			{
				ret = RET_ERROR;
				break;
			}

			mmResult = waveInAddBuffer(phwi, &pwh[i], sizeof(WAVEHDR));

			if (MMSYSERR_NOERROR != mmResult)
			{
				ret = RET_ERROR;
				break;
			}

		}
	}
	else
	{
		ret = RET_ERROR;
	}

	return ret;
}

void CAudioLib::StartRecord(void)
{
	count = 0;
	state = STATUS_WORKING;

	waveInStart(phwi);
}

void CAudioLib::StopRecord(void)
{
	state = STATUS_STOPED;

	waveInStop(phwi);
}

void CAudioLib::StopClose(void)
{
	state = STATUS_CLOSED;
	waveInClose(phwi);
}

uint32_t CAudioLib::RecordSlotCount(void)
{
	return count;
}

uint8_t CAudioLib::LatestRecordSlot(void)
{
	return latest_full_slot;
}
