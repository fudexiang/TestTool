#include "Interfaces.h"

#define AUDIO_BUFFER_SIZE	4096

void WasTest(Threads_Control_t* gpConfig)
{
	uint32_t i = 0;
	void* pPluginObject;
	uint8_t Buffer[AUDIO_BUFFER_SIZE];
	//AUDIO_FORMAT_USER_DEFINE_t format = AUDIO_USER_FORMAT_S16_LE;
	AUDIO_FORMAT_USER_DEFINE_t format = AUDIO_USER_FORMAT_S32_LE;
	AudioDataInfo_t AudioDataInfo;


	pPluginObject = CreatePlugin(PLUGIN_WASLIB, clsidWasLib);

	if (pPluginObject)
	{
		AudioDataInfo.buffer_max_len = 0;
		AudioDataInfo.frame_num = 0;
		Was_Init(format, &AudioDataInfo, pPluginObject);
		
		LogPrintf(log_info, "default duration = %d,default frame count = %d\n", AudioDataInfo.buffer_max_len, AudioDataInfo.frame_num);

		AudioDataInfo.pBuffer = &Buffer[0];
		AudioDataInfo.buffer_max_len = AUDIO_BUFFER_SIZE;

		while (1)
		{
			AudioDataInfo.frame_num = 0;
			AudioDataInfo.buffer_valid_size = 0;
			AudioDataInfo.buffer_size_need = 0;
			AudioDataInfo.flags = 0;

			if (RET_OK == Was_ReadData(&AudioDataInfo, pPluginObject))
			{
				LogPrintf(log_info, "get num = %d,valid = %d,max = %d,flags = 0x%x\n", \
					AudioDataInfo.frame_num, \
					AudioDataInfo.buffer_valid_size, \
					AudioDataInfo.buffer_size_need, \
					AudioDataInfo.flags);
			}
			else
			{
				LogPrintf(log_warn, "read error in Was_ReadData\n");
			}

			Sleep(10);

			if (FUNC_ENABLE == gpConfig->exit_flag)
			{
				LogPrintf(log_info, "exit\n");

				break;
			}

			i++;
		}

		Was_Exit(pPluginObject);
	}

}