#pragma once
#ifndef _DATATYPE_H_
#define _DATATYPE_H_


#include "stdint.h"

enum Unicode_Size_t	{ UNICODE_TWO_BYTE,};

enum CodeRet_t	{ RET_OK , RET_PARAM_ERROR, RET_SIZE_ERROR, RET_ERROR,RET_OVERFLOW};

enum Case_type_t	{CAPTICAL_CASE,LOWER_CASE,};

#define NUM_ARRAY_STOP_FLAG	'*'

typedef enum
{
	STATUS_IDLE = 0,
	STATUS_INITED,
	STATUS_OPENED,
	STATUS_WORKING,
	STATUS_STOPED,
	STATUS_CLOSED,
	STATUS_ERROR,
}Device_Status_t;

//audio plugin
#define MAX_AUDIO_BUFFER_SLOT_NUM	10

typedef struct
{
	uint8_t* pAddr;
	uint32_t len;
}AudioBufferSlot_t;

typedef struct
{
	uint8_t buffer_slot_num;
	AudioBufferSlot_t Slot[MAX_AUDIO_BUFFER_SLOT_NUM];
}AudioBufferGroup_t;

typedef struct
{
	uint8_t nCh;
	uint8_t BitsPerSample;
	uint32_t nSampleRate;
}AudioFormat_t;

#if 1

typedef struct
{
	char	chunk_id[4];
	uint32_t 	chunk_size;
	char	format[4];

	char	sub_chunk1_id[4];
	uint32_t 	sub_chunk1_size;

	uint16_t 	audio_format;
	uint16_t 	num_channels;
	uint32_t	sample_rate;
	uint32_t 	byte_rate;
	uint16_t	block_align;
	uint16_t 	bits_per_sample;

	char sub_chunk2_id[4];
	uint32_t 	sub_chunk2_size;
} WAV_HEADER;


typedef struct {
	uint32_t magic;		/* 'RIFF' */
	uint32_t length;		/* filelen */
	uint32_t type;		/* 'WAVE' */
} WaveHeader;

typedef struct {
	uint32_t type;		/* 'data' */
	uint32_t length;		/* samplecount */
} WaveChunkHeader;

typedef struct {
	uint16_t format;		/* see WAV_FMT_* */
	uint16_t channels;
	uint32_t sample_fq;	/* frequence of sample */
	uint32_t byte_p_sec;
	uint16_t byte_p_spl;	/* samplesize; 1 or 2 bytes */
	uint16_t bit_p_spl;	/* 8, 12 or 16 bit */
} WaveFmtBody;

typedef struct {
	WaveFmtBody format;
	uint16_t ext_size;
	uint16_t bit_p_spl;
	uint32_t channel_mask;
	uint16_t guid_format;	/* WAV_FMT_* */
	uint8_t guid_tag[14];	/* WAV_GUID_TAG */
} WaveFmtExtensibleBody;

/*FFT*/
#define MAX_SAMPLE_N	256
#define PI				((double)3.1415926535898)
typedef struct 
{
	double real;
	double img;
}complex;

//FILTER,IIR
/*
#define FILTER_BYPASS 0
#define FILTER_1ST_LP 1
#define FILTER_1ST_HP 2
#define FILTER_2ND_LP 3
#define FILTER_2ND_HP 4
#define FILTER_2ND_BP 5
#define FILTER_2ND_BS 6
*/
typedef enum
{
	FILTER_BYPASS,
	FILTER_1ST_LP,
	FILTER_1ST_HP,
	FILTER_2ND_LP,
	FILTER_2ND_HP,
	FILTER_2ND_BP,
	FILTER_2ND_BS,
}FILTER_TYPE_t;

typedef struct
{
	FILTER_TYPE_t type;
	double fl;
}FILTERTYPE;	

typedef struct
{
	FILTERTYPE type;
	double ts;	
	double dbgain;
	double q;

	double amp;
	double a[3];
	double b[3];
	double x[3];
	double y[2];
}FILTERINFO, * LPFILTERINFO;

typedef struct
{
	int x;
	int y;
}Point_Location_t,Rect_t;

#endif

#endif