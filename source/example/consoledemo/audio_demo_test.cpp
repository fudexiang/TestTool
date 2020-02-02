
#include "Interfaces.h"
#include "audio_demo_test.h"

#include "stdio.h"

#define AUDIO_BUFFER_SIZE 0x10000

#define EXPECT_AUDIO_SEND_SIZE_IN_ONE_PACKAGE 4096

#define HEAD_SIZE	3
#define END_SIZE	2

#define PERIOD_FRAMES	(1024*5)			
#define bits_per_frame	16

typedef struct
{
	SocktPlugin_t SocketPluginObject;
	TCPIP_Client_t Tunl_Config;
	SOCKET tuning_tool_sclient;
}Audio_Test_Config_t;

Audio_Test_Config_t test_config;
WAV_HEADER wav_header;

void Audio_Demo_Test()
{
	FILE* fp;

	int wav_header_size;
	WaveFmtBody* f;
	uint32_t magic;
	uint16_t format, channels;
	int big_endian, native_format;

	long pos_data;

	char buffer[AUDIO_BUFFER_SIZE];
	uint32_t rate, i, ret;

	char ServerAddr[] = "192.168.1.5";
	
	test_config.Tunl_Config.pBuffer = ServerAddr;
	test_config.Tunl_Config.port = 2829;
	test_config.Tunl_Config.pSocket = &test_config.tuning_tool_sclient;

	test_config.SocketPluginObject.pAudioSocketPluginObject = CreatePlugin(PLUGIN_SOCKET, clsidSocketLib);

	if (RET_OK == TCPIP_SocketClientConnect(&(test_config.Tunl_Config), &(test_config.SocketPluginObject)))
	{
		fp = fopen("3.wav", "rb");

		if (fp == NULL)
		{
			LogPrintf(log_error, "music file is NULL \n");
			return ;
		}
		else
		{
			wav_header_size = fread(&wav_header, 1, sizeof(wav_header), fp);
			LogPrintf(log_debug, "wav_header_size = %d\n", wav_header_size);
			LogPrintf(log_debug, "RIFF [%c][%c][%c][%c]\n", wav_header.chunk_id[0], wav_header.chunk_id[1], wav_header.chunk_id[2], wav_header.chunk_id[3]);
			LogPrintf(log_debug, "file size = %d \n", wav_header.chunk_size);
			LogPrintf(log_debug, "fortmats = [%c][%c][%c][%c]	\n", wav_header.format[0], wav_header.format[1], wav_header.format[2], wav_header.format[3]);
			LogPrintf(log_debug, "[%c][%c][%c][%c]\n", wav_header.sub_chunk1_id[0], wav_header.sub_chunk1_id[1], wav_header.sub_chunk1_id[2], wav_header.sub_chunk1_id[3]);
			LogPrintf(log_debug, "sub_chunk1_size = %d\n", wav_header.sub_chunk1_size);
			LogPrintf(log_debug, "audio_format = %d\n", wav_header.audio_format);
			LogPrintf(log_debug, "num_channels = %d\n", wav_header.num_channels);
			LogPrintf(log_debug, "sample_rate = %d\n", wav_header.sample_rate);
			LogPrintf(log_debug, "byte_rate = %d\n", wav_header.byte_rate);
			LogPrintf(log_debug, "block_align = %d\n", wav_header.block_align);
			LogPrintf(log_debug, "bits_per_sample = %d\n", wav_header.bits_per_sample);

			magic = COMPOSE_ID(wav_header.chunk_id[0], wav_header.chunk_id[1], wav_header.chunk_id[2], wav_header.chunk_id[3]);

			if (magic == WAV_RIFF)
				big_endian = 0;
			else if (magic == WAV_RIFX)
				big_endian = 1;
			else
			{
				LogPrintf(log_debug, "wav big_endian format error\n");
				return ;
			}

			magic = COMPOSE_ID(wav_header.format[0], wav_header.format[1], wav_header.format[2], wav_header.format[3]);
			if (magic != WAV_WAVE)
			{
				LogPrintf(log_debug, "wav format error\n");
				return ;
			}

			f = (WaveFmtBody*)(&wav_header.audio_format);
			format = TO_CPU_SHORT(f->format, big_endian);

			LogPrintf(log_debug, "f->format = %d\n", f->format);
			if (format == WAV_FMT_EXTENSIBLE)
			{
				LogPrintf(log_debug, "format error:WAV_FMT_EXTENSIBLE\n");
				return ;
			}

			if (format != WAV_FMT_PCM && format != WAV_FMT_IEEE_FLOAT)
			{
				LogPrintf(log_debug, "can't play WAVE-file format 0x%04x which is not PCM or FLOAT encoded", format);
				return ;
			}
			channels = TO_CPU_SHORT(f->channels, big_endian);
			if (channels < 1)
			{
				LogPrintf(log_debug, "can't play WAVE-files with %d tracks", channels);
				return ;
			}
			//hwparams.channels = channels;

			LogPrintf(log_debug, "f->bit_p_spl = %d\n", f->bit_p_spl);

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
					LogPrintf(log_debug, " can't play WAVE-files with sample %d bits in %d bytes wide (%d channels)",
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
				LogPrintf(log_debug, " can't play WAVE-files with sample %d bits wide"), TO_CPU_SHORT(f->bit_p_spl, big_endian);
				return;
			}

			rate = TO_CPU_INT(f->sample_fq, big_endian);

			LogPrintf(log_debug, "f->sample_fq = %d,%d,%d\n", rate, f->sample_fq, big_endian);

			buffer[0] = 'p';
			buffer[1] = native_format;
			buffer[2] = channels;
			buffer[3] = (rate >> 24) & 0xff;
			buffer[4] = (rate >> 16) & 0xff;
			buffer[5] = (rate >> 8) & 0xff;
			buffer[6] = (rate >> 0) & 0xff;

			LogPrintf(log_debug, "f->sample_fq = %d,%d,%d\n", native_format, channels, rate);

			TCPIP_SocketClientSend(buffer, 7, &(test_config.SocketPluginObject));

			ret = TCPIP_SocketClientRece(buffer, AUDIO_BUFFER_SIZE, &(test_config.SocketPluginObject));
#if 0
			printf("rece:");
			for (i = 0; i < ret; i++)
			{
				printf("0x%x ", buffer[i]);
			}
			printf("\n");
#endif
			int buffer_size;

			buffer_size = buffer[4];
			buffer_size = buffer_size << 8;
			buffer_size |= buffer[3];
			buffer_size = buffer_size << 8;
			buffer_size |= buffer[2];
			buffer_size = buffer_size << 8;
			buffer_size |= buffer[1];


			printf("buffer size = %d\n", buffer_size);

			buffer_size = bits_per_frame * channels * PERIOD_FRAMES / 8;

			buffer[0] = 'a';


			buffer[EXPECT_AUDIO_SEND_SIZE_IN_ONE_PACKAGE - END_SIZE] = 0xa5;
			buffer[EXPECT_AUDIO_SEND_SIZE_IN_ONE_PACKAGE - END_SIZE + 1] = 0x5a;

			int total_size = 0, readlen = 0, sendlen = 0, count = 0, file_read_size = 0;

			uint32_t audio_data_end_pos, audio_data_start_pos;

			audio_data_start_pos = ftell(fp);
			fseek(fp, 0, SEEK_END);
			audio_data_end_pos = ftell(fp);
			fseek(fp, audio_data_start_pos, SEEK_SET);
			LogPrintf(log_debug, "data start pos: 0x%x,0x%x\n", audio_data_start_pos, ftell(fp));

			LogPrintf(log_debug, "data end pos: 0x%x\n", audio_data_end_pos);

			int total_count, current_count = 0;

			total_count = (audio_data_end_pos - audio_data_start_pos) / buffer_size;

			LogPrintf(log_debug, "count: 0x%x\n", total_count);

			uint32_t byte_count = 0;
			uint32_t slot_count = 0;
			while (1)
			{

				readlen = EXPECT_AUDIO_SEND_SIZE_IN_ONE_PACKAGE - HEAD_SIZE - END_SIZE;

				ret = fread(&buffer[3], 1, readlen, fp);

#if 1
				if (ret == 0)
				{
					LogPrintf(log_debug, "end of music file input[require %d,%d,%d]! \n", readlen, total_size, buffer_size);
					LogPrintf(log_debug, "read end pos:  0x%x\n", ftell(fp));
					fseek(fp, audio_data_start_pos, SEEK_SET);

					LogPrintf(log_debug, "set start pos:  0x%x\n", ftell(fp));

					total_size = 0;
					readlen = EXPECT_AUDIO_SEND_SIZE_IN_ONE_PACKAGE - HEAD_SIZE - END_SIZE;

					ret = fread(&buffer[3], 1, readlen, fp);

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

				buffer[0] = 'a';
				buffer[1] = readlen / 256;
				buffer[2] = readlen % 256;


				sendlen = TCPIP_SocketClientSend(buffer, readlen + HEAD_SIZE + END_SIZE, &(test_config.SocketPluginObject));

				//LogPrintf(log_debug, "send [%d][%d] \n", sendlen, readlen + HEAD_SIZE + END_SIZE);

				TCPIP_SocketClientRece((char*)(&buffer[1]), 1, &(test_config.SocketPluginObject));
#if 0
				if (1 == get_capture_data_flag)
				{
					CaptureBuffer[0] = 'r';
					ret = send(tuning_tool_sclient, (const char*)CaptureBuffer, 1, 0);

					ret = recv(tuning_tool_sclient, (char*)CaptureBuffer, CAPTURE_SIZE_BYTES, 0);
					if (ret != 256)
					{
						LogPrintf(log_debug, "miss data[rece %d] \n", ret);
					}
					else
					{
						LogPrintf(log_debug, "get data number[%d] \n", ret);
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
	
}