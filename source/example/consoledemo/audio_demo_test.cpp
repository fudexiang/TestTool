
#include "Interfaces.h"
#include "audio_demo_test.h"

#include "stdio.h"


#define EXPECT_AUDIO_SEND_SIZE_IN_ONE_PACKAGE 4096

#define HEAD_SIZE	3
#define END_SIZE	2

Audio_Test_Config_t gTest_config;
char gBuffer[AUDIO_BUFFER_SIZE];

FILE* fp;

int buffer_size;
uint16_t channels;
WAV_HEADER wav_header;

uint16_t bswap_16(uint16_t v)
{
	uint16_t ret = v & 0xff;
	ret = (ret << 8) | ((v >> 8) & 0xff);
	return ret;
}
uint32_t bswap_32(uint32_t v)
{
	uint32_t ret = v & 0xff;
	ret = (ret << 8) | ((v >> 8) & 0xff);
	ret = (ret << 8) | ((v >> 16) & 0xff);
	ret = (ret << 8) | ((v >> 24) & 0xff);
	return ret;
}

void wave_file_analysis(void)
{
	int wav_header_size;
	WaveFmtBody* f;
	uint32_t magic;
	uint16_t format;
	int big_endian, native_format;
	uint32_t rate, i, ret;

	wav_header_size = fread(&wav_header, 1, sizeof(wav_header), fp);
	LogPrintf(log_info, "wav_header_size = %d\n", wav_header_size);
	LogPrintf(log_info, "RIFF [%c][%c][%c][%c]\n", wav_header.chunk_id[0], wav_header.chunk_id[1], wav_header.chunk_id[2], wav_header.chunk_id[3]);
	LogPrintf(log_info, "file size = %d \n", wav_header.chunk_size);
	LogPrintf(log_info, "fortmats = [%c][%c][%c][%c]	\n", wav_header.format[0], wav_header.format[1], wav_header.format[2], wav_header.format[3]);
	LogPrintf(log_info, "[%c][%c][%c][%c]\n", wav_header.sub_chunk1_id[0], wav_header.sub_chunk1_id[1], wav_header.sub_chunk1_id[2], wav_header.sub_chunk1_id[3]);
	LogPrintf(log_info, "sub_chunk1_size = %d\n", wav_header.sub_chunk1_size);
	LogPrintf(log_info, "audio_format = %d\n", wav_header.audio_format);
	LogPrintf(log_info, "num_channels = %d\n", wav_header.num_channels);
	LogPrintf(log_info, "sample_rate = %d\n", wav_header.sample_rate);
	LogPrintf(log_info, "byte_rate = %d\n", wav_header.byte_rate);
	LogPrintf(log_info, "block_align = %d\n", wav_header.block_align);
	LogPrintf(log_info, "bits_per_sample = %d\n", wav_header.bits_per_sample);

	magic = COMPOSE_ID(wav_header.chunk_id[0], wav_header.chunk_id[1], wav_header.chunk_id[2], wav_header.chunk_id[3]);

	if (magic == WAV_RIFF)
		big_endian = 0;
	else if (magic == WAV_RIFX)
		big_endian = 1;
	else
	{
		LogPrintf(log_info, "wav big_endian format error\n");
		return;
	}

	magic = COMPOSE_ID(wav_header.format[0], wav_header.format[1], wav_header.format[2], wav_header.format[3]);
	if (magic != WAV_WAVE)
	{
		LogPrintf(log_info, "wav format error\n");
		return;
	}

	f = (WaveFmtBody*)(&wav_header.audio_format);
	format = TO_CPU_SHORT(f->format, big_endian);

	LogPrintf(log_info, "f->format = %d\n", f->format);
	if (format == WAV_FMT_EXTENSIBLE)
	{
		LogPrintf(log_info, "format error:WAV_FMT_EXTENSIBLE\n");
		return;
	}

	if (format != WAV_FMT_PCM && format != WAV_FMT_IEEE_FLOAT)
	{
		LogPrintf(log_info, "can't play WAVE-file format 0x%04x which is not PCM or FLOAT encoded", format);
		return;
	}
	channels = TO_CPU_SHORT(f->channels, big_endian);
	if (channels < 1)
	{
		LogPrintf(log_info, "can't play WAVE-files with %d tracks", channels);
		return;
	}
	//hwparams.channels = channels;

	LogPrintf(log_info, "f->bit_p_spl = %d\n", f->bit_p_spl);

	switch (TO_CPU_SHORT(f->bit_p_spl, big_endian))
	{
	case 8:
		//hwparams.format = SND_PCM_FORMAT_U8;
		native_format = SND_PCM_FORMAT_U8;
		break;
	case 16:
		if (big_endian)
			native_format = SND_PCM_FORMAT_S16_BE;
		else
			native_format = SND_PCM_FORMAT_S16_LE;
		//hwparams.format = native_format;
		break;
	case 24:
		switch (TO_CPU_SHORT(f->byte_p_spl, big_endian) / channels)
		{
		case 3:
			if (big_endian)
				native_format = SND_PCM_FORMAT_S24_3BE;
			else
				native_format = SND_PCM_FORMAT_S24_3LE;
			//hwparams.format = native_format;
			break;
		case 4:
			if (big_endian)
				native_format = SND_PCM_FORMAT_S24_BE;
			else
				native_format = SND_PCM_FORMAT_S24_LE;
			//hwparams.format = native_format;
			break;
		default:
			LogPrintf(log_info, " can't play WAVE-files with sample %d bits in %d bytes wide (%d channels)",
				TO_CPU_SHORT(f->bit_p_spl, big_endian),
				TO_CPU_SHORT(f->byte_p_spl, big_endian), channels);
			return;
		}
		break;
	case 32:
		if (format == WAV_FMT_PCM)
		{
			if (big_endian)
				native_format = SND_PCM_FORMAT_S32_BE;
			else
				native_format = SND_PCM_FORMAT_S32_LE;
			//hwparams.format = native_format;
		}
		else if (format == WAV_FMT_IEEE_FLOAT)
		{
			if (big_endian)
				native_format = SND_PCM_FORMAT_FLOAT_BE;
			else
				native_format = SND_PCM_FORMAT_FLOAT_LE;
			//hwparams.format = native_format;
		}
		break;
	default:
		LogPrintf(log_info, " can't play WAVE-files with sample %d bits wide"), TO_CPU_SHORT(f->bit_p_spl, big_endian);
		return;
	}

	rate = TO_CPU_INT(f->sample_fq, big_endian);

	LogPrintf(log_info, "f->sample_fq = %d,%d,%d\n", rate, f->sample_fq, big_endian);

	gBuffer[0] = 'p';
	gBuffer[1] = native_format;
	gBuffer[2] = channels;
	gBuffer[3] = (rate >> 24) & 0xff;
	gBuffer[4] = (rate >> 16) & 0xff;
	gBuffer[5] = (rate >> 8) & 0xff;
	gBuffer[6] = (rate >> 0) & 0xff;

	LogPrintf(log_info, "f->sample_fq = %d,%d,%d\n", native_format, channels, rate);

	TCPIP_SocketClientSend(gBuffer, 7, &(gTest_config.SocketPluginObject));

	ret = TCPIP_SocketClientRece(gBuffer, AUDIO_BUFFER_SIZE, &(gTest_config.SocketPluginObject));

#if 0
	printf("rece:");
	for (i = 0; i < ret; i++)
	{
		printf("0x%x ", buffer[i]);
	}
	printf("\n");
#endif

	buffer_size = gBuffer[4];
	buffer_size = buffer_size << 8;
	buffer_size |= gBuffer[3];
	buffer_size = buffer_size << 8;
	buffer_size |= gBuffer[2];
	buffer_size = buffer_size << 8;
	buffer_size |= gBuffer[1];

	LogPrintf(log_info, "buffer size0 = %d\n", buffer_size);
}

void Audio_Demo_Test(Threads_Control_t *pControl)
{
	uint32_t rate, i, ret;
	char ServerAddr[] = "192.168.1.5";
	int total_count, current_count = 0;
	uint32_t audio_data_end_pos, audio_data_start_pos;
	int total_size = 0, readlen = 0, sendlen = 0, count = 0, file_read_size = 0;

	gTest_config.Tunl_Config.pBuffer = ServerAddr;
	gTest_config.Tunl_Config.port = 2829;
	gTest_config.Tunl_Config.pSocket = &gTest_config.SocketPluginObject.Socket;

	gTest_config.SocketPluginObject.pAudioSocketPluginObject = CreatePlugin(PLUGIN_SOCKET, clsidSocketLib);

	if (RET_OK == TCPIP_SocketClientConnect(&(gTest_config.Tunl_Config), &(gTest_config.SocketPluginObject)))
	{
		fp = fopen("3.wav", "rb");

		if (fp == NULL)
		{
			LogPrintf(log_error, "music file is NULL \n");
			return ;
		}
		else
		{
			wave_file_analysis();

#if 0
			#define PERIOD_FRAMES	(1024*5)			
			#define bits_per_frame	16
			buffer_size = bits_per_frame * channels * PERIOD_FRAMES / 8;
			printf("buffer size1 = %d\n", buffer_size);
#endif

#if 1
			audio_data_start_pos = ftell(fp);
			fseek(fp, 0, SEEK_END);
			audio_data_end_pos = ftell(fp);
			
			LogPrintf(log_info, "data start pos: 0x%x,0x%x\n", audio_data_start_pos, ftell(fp));
			LogPrintf(log_info, "data end pos: 0x%x\n", audio_data_end_pos);

			total_count = (audio_data_end_pos - audio_data_start_pos) / buffer_size;
			LogPrintf(log_info, "count: 0x%x\n", total_count);
#endif
			uint32_t byte_count = 0;
			uint32_t slot_count = 0;

			fseek(fp, audio_data_start_pos, SEEK_SET);

			while (1)
			{
				if (FUNC_ENABLE == pControl->exit_flag)
				{
					LogPrintf(log_info, "exit audio play\n");
					break;
				}

				while (FUNC_ENABLE == pControl->pause_flag)
				{
					Sleep(1);
				}

				readlen = EXPECT_AUDIO_SEND_SIZE_IN_ONE_PACKAGE - HEAD_SIZE - END_SIZE;

				ret = fread(&gBuffer[3], 1, readlen, fp);

#if 1
				if (ret == 0)
				{
					LogPrintf(log_info, "end of music file input[require %d,%d,%d]! \n", readlen, total_size, buffer_size);
					LogPrintf(log_info, "read end pos:  0x%x\n", ftell(fp));

					fseek(fp, audio_data_start_pos, SEEK_SET);

					LogPrintf(log_info, "set start pos:  0x%x\n", ftell(fp));

					total_size = 0;
					readlen = EXPECT_AUDIO_SEND_SIZE_IN_ONE_PACKAGE - HEAD_SIZE - END_SIZE;

					ret = fread(&gBuffer[3], 1, readlen, fp);

				}
#endif


				total_size += readlen;

				if (total_size >= buffer_size)
				{
					slot_count++;
					if (slot_count == 20)
						slot_count = 0;
					total_size = total_size - buffer_size;
				}

				//Audio data format: 'a' + ... + 0xa5 + 0x5a
				gBuffer[0] = 'a';
				gBuffer[1] = readlen / 256;
				gBuffer[2] = readlen % 256;
				gBuffer[readlen + HEAD_SIZE] = 0xa5;
				gBuffer[readlen + HEAD_SIZE+1] = 0x5a;

				sendlen = TCPIP_SocketClientSend(gBuffer, readlen + HEAD_SIZE + END_SIZE, &(gTest_config.SocketPluginObject));

				//LogPrintf(log_info, "send [%d][%d] \n", sendlen, readlen + HEAD_SIZE + END_SIZE);

				TCPIP_SocketClientRece((char*)(&gBuffer[1]), 1, &(gTest_config.SocketPluginObject));

#if 0
				if (1 == get_capture_data_flag)
				{
					CaptureBuffer[0] = 'r';
					ret = send(tuning_tool_sclient, (const char*)CaptureBuffer, 1, 0);

					ret = recv(tuning_tool_sclient, (char*)CaptureBuffer, CAPTURE_SIZE_BYTES, 0);
					if (ret != 256)
					{
						LogPrintf(log_info, "miss data[rece %d] \n", ret);
					}
					else
					{
						LogPrintf(log_info, "get data number[%d] \n", ret);
					}

					get_capture_data_flag++;
				}
#endif
				Sleep(1);
			}

			fclose(fp);
		}
	}
	else
	{
		LogPrintf(log_critical,"Socket client connect failed");
	}

	TCPIP_CloseSocket(&gTest_config.SocketPluginObject);
}