#include "Interfaces.h"
#include "stdio.h"
#include "math.h"

#define GENERATE_WAV_FILE

#define DEFAULT_CHANNEL_NUM		1
#define DEFAULT_SAMPLE_RATE		8000
#define DEFAULT_BITS_PERSAMPLE	8

#define SLOT_NUM 3
//one slot for 1 second
#define AUDIO_RECORD_SECONDS	2
#define AUDIO_BUFFER_SIZE	(AUDIO_RECORD_SECONDS * DEFAULT_SAMPLE_RATE * DEFAULT_BITS_PERSAMPLE*DEFAULT_CHANNEL_NUM / 8)

#define NAME_BUFFER_SIZE	100

static uint8_t TestBuffer[SLOT_NUM][AUDIO_BUFFER_SIZE];

typedef struct wave_pcm_hdr
{
	char                riff[4];               // = "RIFF"
	uint32_t			size_8;              // = FileSize - 8
	char                wave[4];            // = "WAVE"
	char                fmt[4];          // = "fmt "

	uint32_t			dwFmtSize;        // = 下一个结构体的大小 : 16
	uint16_t				format_tag;        // = PCM : 1
	uint16_t				channels;               // = 通道数 : 1
	uint32_t			samples_per_sec;        // = 采样率
	uint32_t			avg_bytes_per_sec;        // = 每秒字节数
	uint16_t				block_align;              // = 每采样点字节数
	uint16_t				bits_per_sample;        // = 量化比特数: 8 | 16

	char                data[4];                    // = "data";
	uint32_t			data_size;        // = 纯数据长度 : FileSize - 44 
} wave_header;

struct wave_pcm_hdr wav_hdr =
{
		{ 'R', 'I', 'F', 'F' },
		AUDIO_RECORD_SECONDS*DEFAULT_CHANNEL_NUM* DEFAULT_SAMPLE_RATE* DEFAULT_CHANNEL_NUM* DEFAULT_BITS_PERSAMPLE / 8 + 36,
		{'W', 'A', 'V', 'E'},
		{'f', 'm', 't', ' '},
		16,
		1,
		DEFAULT_CHANNEL_NUM,
		DEFAULT_SAMPLE_RATE,
		DEFAULT_SAMPLE_RATE* DEFAULT_CHANNEL_NUM* DEFAULT_BITS_PERSAMPLE / 8,
		DEFAULT_CHANNEL_NUM* DEFAULT_BITS_PERSAMPLE / 8,
		DEFAULT_BITS_PERSAMPLE,
		{'d', 'a', 't', 'a'},
		AUDIO_RECORD_SECONDS*DEFAULT_CHANNEL_NUM* DEFAULT_SAMPLE_RATE* DEFAULT_CHANNEL_NUM* DEFAULT_BITS_PERSAMPLE / 8
};

#if 1
/*
	y = A * sin(w1 * t + Q1) + B * sin(w2 * t + Q2) + C
	T1 = 2 * PI/w1
*/
#define A	10
#define C	50
#define Q1	0
//#define Q1	((double)(PI*30/180))

#define B	20
#define Q2	0

#define freq_signal_1		1
#define freq_signal_2		16
#define freq_sample			(64 * freq_signal_1)

void output(complex *x,uint32_t size_x)
{
	int i;
	printf("The result are as follows：\n");
	for (i = 0; i < size_x; i++)
	{
		if(i<=(size_x/2))
			printf("freq = %f,",(double)i* (double)freq_sample/ (double)size_x);

		printf("[%d]%.4f,",i, x[i].real);
		if (x[i].img >= 0.0001)
			printf("+%.4fj\n", x[i].img);
		else if (fabs(x[i].img) < 0.0001)
			printf("\n");
		else 
			printf("%.4fj\n", x[i].img);
	}
}

void IIR_FIELTER(complex *src,complex *dst,uint32_t size)
{
#define NZEROS 2
#define GAIN   3.338387406e+01
#define PARAM_Y0	((double)-0.574061915)
#define PARAM_Y1	((double)1.4542435863)

	double xv[3] = {0,0,0};
	double yv[3] = {0,0,0};
	int i;

	for (i=0;i<size;i++)
	{
#if 0
		xv[0] = xv[1]; 
		xv[1] = xv[2];
		xv[2] = src[i].real / GAIN;
		yv[0] = yv[1]; 
		yv[1] = yv[2];
		yv[2] = (xv[0] + xv[2]) + 2 * xv[1] + (PARAM_Y0 * yv[0]) + (PARAM_Y1 * yv[1]);
#else
		xv[0] = xv[1];
		xv[1] = xv[2];
		xv[2] = src[i].real / GAIN;
		yv[0] = yv[1];
		yv[1] = yv[2];
		yv[2] = (xv[0] + xv[2]) - 2 * xv[1] + (PARAM_Y0 * yv[0]) + (PARAM_Y1 * yv[1]);

#endif
		dst[i].real = yv[2]* GAIN;
	}
}


void IIR_FIELTER_TEST(uint8_t* src, uint8_t* dst, uint32_t size)
{
#define GAIN		((double)1.057108315e+00)
#define PARAM_Y0	((double)-0.8948743446)
#define PARAM_Y1	((double)1.8890330794)

	double xv[3] = { 0,0,0 };
	double yv[3] = { 0,0,0 };
	int i;

	for (i = 0; i < size; i++)
	{
#if 0
		xv[0] = xv[1];
		xv[1] = xv[2];
		xv[2] = src[i].real / GAIN;
		yv[0] = yv[1];
		yv[1] = yv[2];
		yv[2] = (xv[0] + xv[2]) + 2 * xv[1] + (PARAM_Y0 * yv[0]) + (PARAM_Y1 * yv[1]);
#else
		xv[0] = xv[1];
		xv[1] = xv[2];
		xv[2] = (double)(src[i]) / GAIN;
		yv[0] = yv[1];
		yv[1] = yv[2];
		yv[2] = (xv[0] + xv[2]) - 2 * xv[1] + (PARAM_Y0 * yv[0]) + (PARAM_Y1 * yv[1]);

#endif
		dst[i] = (uint8_t)(yv[2] * GAIN);
	}
}
#endif

void GenerateSignals(uint8_t *pBuff, uint32_t len)
{

	uint32_t i;
	double temp1,temp2;
	double w1,w2;
	double delta_T = 0;

	void *pAudioPlugin,*pIIRPlugin;
	complex x[MAX_SAMPLE_N];
	complex x_for_filter[MAX_SAMPLE_N];

	double m_original_signal[MAX_SAMPLE_N];
	double m_signal_filter[MAX_SAMPLE_N];

	pAudioPlugin = CreatePlugin(PLUGIN_FFT, clsidFFTLib);
	pIIRPlugin = CreatePlugin(PLUGIN_FILTER, clsidFilterLib);
	if ((MAX_SAMPLE_N > len) && (pAudioPlugin) && (pIIRPlugin))
	{
		w1 = 2 * PI * freq_signal_1;
		w2 = 2 * PI * freq_signal_2;

		printf("signal freq = %f,sample freq = %f", (double)(freq_signal_1), (double)(freq_sample));

		printf("PI = %f\r\n", PI);

		printf("w = %f\r\n", w1);

		for (i = 0; i < len; i++)
		{
			delta_T = (double)i * 1 / (double)freq_sample;

			temp1 = w1 * delta_T + Q1;
			temp2 = w2 * delta_T + Q2;
			temp1 = A * sin(temp1) + B * sin(temp2) + C;

			pBuff[i] = (uint8_t)temp1;

			m_original_signal[i] = temp1;

			//x[i].real = pBuff[i];
			x[i].real = m_original_signal[i];
			x[i].img = 0;
			printf("sample1 value[%d] = %f\r\n", i, x[i].real);
			//printf("sample value[%d] = %d,%f\r\n", i, pBuff[i], temp1);

			x_for_filter[i].real = m_original_signal[i];
			//x_for_filter[i].real = x[i].real;
			x_for_filter[i].img = 0;
		}

		CalFFT(x, len, pAudioPlugin);

		output(x, len);
#if 0
		printf("--------------------->IIR Filter results\r\n");

		IIR_FIELTER(x_for_filter,x_for_filter, len);

		for (i = 0; i < len; i++)
		{
			printf("sample2 value[%d] = %f\r\n", i, x_for_filter[i].real);
		}

		CalFFT(x_for_filter, len, pAudioPlugin);

		output(x_for_filter, len);
#endif
		
#if 1
		FILTERINFO config;
		config.type.type = FILTER_2ND_LP;
		config.type.fl = 4;
		config.ts = (double)1 / (double)freq_sample;
		config.q = 1;

		printf("------------------->IIR init\r\n");

		printf("%d,%f,%f,%f\r\n", config.type.type, config.ts, config.type.fl, config.q);

		IIR_FilterConfig(config, pIIRPlugin);
		printf("------------------->IIR init done\r\n");

		for (int i = 0; i < len; i++)
		{
			m_signal_filter[i] = IIR_FilterCal(m_original_signal[i], pIIRPlugin);
		}

		printf("------------------->IIR Filter done\r\n");

		for (i = 0; i < len; i++)
		{
			printf("sample3 value[%d] = %f\r\n", i, m_signal_filter[i]);
			x_for_filter[i].real = m_signal_filter[i];
			x_for_filter[i].img = 0;
		}

		CalFFT(x_for_filter, len, pAudioPlugin);

		output(x_for_filter, len);
#endif
	}
	else
	{
		printf("error info MAX_SAMPLE_N = %d,len =%d\r\n", MAX_SAMPLE_N, len);
	}

}

void FileHandle(void* pIIRPlugin)
{
	uint32_t i;
	AudioBufferGroup_t BufferConfig;

	FILTERINFO config;

	FILE* fp, * fp2;


	for (i = 0; i < SLOT_NUM; i++)
	{
		BufferConfig.Slot[i].len = AUDIO_BUFFER_SIZE;
		BufferConfig.Slot[i].pAddr = &TestBuffer[i][0];
	}

	fp = fopen("1_NoIIR.wav", "r");
	fp2 = fopen("1_IIR.wav", "a+");

	if ((fp) && (fp2))
	{
		fread(BufferConfig.Slot[0].pAddr, sizeof(wav_hdr), 1, fp);
		i = fread(BufferConfig.Slot[0].pAddr, AUDIO_BUFFER_SIZE, 1, fp);

		if (AUDIO_BUFFER_SIZE != i)
		{
			printf("data len=%d\r\n", i);
		}

		fwrite(&wav_hdr, sizeof(wav_hdr), 1, fp2);
		if (pIIRPlugin)
		{
			config.ts = (double)1 / (double)(DEFAULT_SAMPLE_RATE);
#if 1
			//LP 3000
			config.type.type = FILTER_2ND_LP;
			config.type.fl = 100;
			config.q = 1;

			IIR_FilterConfig(config, pIIRPlugin);

			for (int i = 0; i < AUDIO_BUFFER_SIZE; i++)
			{
				BufferConfig.Slot[0].pAddr[i] = (uint8_t)(IIR_FilterCal(BufferConfig.Slot[0].pAddr[i], pIIRPlugin));
			}

#endif

#if 0
			//HP 100
			config.type.type = FILTER_2ND_HP;
			config.type.fl = 100;
			config.q = 1;

#if 1
			IIR_FilterConfig(config, pIIRPlugin);

			for (int i = 0; i < AUDIO_BUFFER_SIZE; i++)
			{
				BufferConfig.Slot[0].pAddr[i] = (uint8_t)(IIR_FilterCal(BufferConfig.Slot[0].pAddr[i], pIIRPlugin));
			}
#else
			IIR_FIELTER_TEST(BufferConfig.Slot[0].pAddr, BufferConfig.Slot[0].pAddr, AUDIO_BUFFER_SIZE);
#endif
#endif
			printf("------------------->IIR done\r\n");

		}
		fwrite(BufferConfig.Slot[0].pAddr, BufferConfig.Slot[0].len, 1, fp2);

		fclose(fp);
		fclose(fp2);
	}
	LogPrintf(log_info, "FileHandle done\r\n");
}

void Mytest11(void)
{
	uint32_t i;
	uint32_t count, num, bk;
	uint32_t num_copy;
	uint8_t DeviceName[NAME_BUFFER_SIZE];
	AudioFormat_t FormatConfig;
	AudioBufferGroup_t BufferConfig;

	FILTERINFO config;

	FILE *fp;
	void *pAudioPlugin,*pIIRPlugin;
	
	pIIRPlugin = CreatePlugin(PLUGIN_FILTER, clsidFilterLib);

	pAudioPlugin = CreatePlugin(PLUGIN_AUDIO, clsidAudioLib);

	FormatConfig.nCh = DEFAULT_CHANNEL_NUM;
	FormatConfig.nSampleRate = DEFAULT_SAMPLE_RATE;
	FormatConfig.BitsPerSample = DEFAULT_BITS_PERSAMPLE;

	BufferConfig.buffer_slot_num = SLOT_NUM;

	//test
#if 1
	GenerateSignals(DeviceName, freq_sample / freq_signal_1);
#endif

#if 0
	FileHandle(pIIRPlugin);
#endif

#if 0
	for (i=0;i< SLOT_NUM;i++)
	{
		BufferConfig.Slot[i].len = AUDIO_BUFFER_SIZE;
		BufferConfig.Slot[i].pAddr = &TestBuffer[i][0];
	}

	if (pAudioPlugin)
	{
		i = GetAudioDeviceName(DeviceName, NAME_BUFFER_SIZE - 1, pAudioPlugin);
		DeviceName[NAME_BUFFER_SIZE - 1] = '\0';

		LogPrintf(log_info, "audio sound devices name[%d]:%s", i, DeviceName);

		AudioRecordConfig(&FormatConfig, &BufferConfig , pAudioPlugin);

		LogPrintf(log_info, "press any key\r\n");
		scanf("%s", TestBuffer);

		StartAudioRecord(pAudioPlugin);

		count = 0;
		bk = 0;
		while (count < 3)
		{
			Sleep(1);

			num = GetAudioLatestFullSlotNum(pAudioPlugin);

			count = GetAudioSlotCount(pAudioPlugin);

			if (bk != count)
			{
				bk = count;
				LogPrintf(log_info, "count = %d, slot = %d\r\n", count, num);
#ifdef GENERATE_WAV_FILE
				LogPrintf(log_info, "len = %d\r\n", BufferConfig.Slot[num].len);

				fp = fopen("1_NoIIR.wav", "a+");

				if (fp)
				{
					fwrite(&wav_hdr, sizeof(wav_hdr), 1, fp);
#if 0
					for(i=0;i< BufferConfig.Slot[num].len;i++)
					{
						*(BufferConfig.Slot[num].pAddr + i) = 0xff;
					}
#else

					fwrite(BufferConfig.Slot[num].pAddr, BufferConfig.Slot[num].len, 1, fp);

#if 0
					if (pIIRPlugin)
					{
						config.type.type = FILTER_2ND_LP;
						config.type.fl = 3000;
						config.ts = (double)1 / (double)(DEFAULT_SAMPLE_RATE);
						config.q = 1;

						printf("------------------->IIR init\r\n");

						IIR_FilterConfig(config, pIIRPlugin);
						printf("------------------->IIR init done\r\n");

						for (int i = 0; i < BufferConfig.Slot[num].len; i++)
						{
							BufferConfig.Slot[num].pAddr[i] = (uint8_t)(IIR_FilterCal(BufferConfig.Slot[num].pAddr[i], pIIRPlugin));
						}
					}
					fwrite(BufferConfig.Slot[num].pAddr, BufferConfig.Slot[num].len, 1, fp2);
#endif


#endif
					LogPrintf(log_info, "finish writing data[%d][%d]\r\n", num_copy, BufferConfig.Slot[num].len - num_copy);
					fclose(fp);
					break;
				}
				else
				{
					LogPrintf(log_info, "create file failed\r\n");
				}
#else
				if (count > 3)
				{
					break;
				}
#endif
			}
		}
		CloseAudioRecord(pAudioPlugin);

#if 0
		fp = fopen("1.wav", "rb");
		if (fp)
		{
			int wav_header_size;
			WaveFmtBody* f;
			uint32_t magic;
			uint16_t format;
			int big_endian, native_format;
			uint32_t rate, i, ret;
			WAV_HEADER wav_header;

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
			fclose(fp);
		}

#endif
	}
	else
	{
		LogPrintf(log_error, "create audio plugin error");
	}
#endif
	LogPrintf(log_info, "Mytest11 done\r\n");
}