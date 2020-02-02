#pragma once
#ifndef _AUDIO_DEMO_TEST_H_
#define _AUDIO_DEMO_TEST_H_

#include "Common/DataType.h"

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

#if 1
#define COMPOSE_ID(a,b,c,d)	((a) | ((b)<<8) | ((c)<<16) | ((d)<<24))
#define LE_SHORT(v)		(v)
#define LE_INT(v)		(v)
#define BE_SHORT(v)		bswap_16(v)
#define BE_INT(v)		bswap_32(v)
#else
#define COMPOSE_ID(a,b,c,d)	((d) | ((c)<<8) | ((b)<<16) | ((a)<<24))
#define LE_SHORT(v)		bswap_16(v)
#define LE_INT(v)		bswap_32(v)
#define BE_SHORT(v)		(v)
#define BE_INT(v)		(v)
#endif


/* Note: the following macros evaluate the parameter v twice */
#define TO_CPU_SHORT(v, be) \
	((be) ? BE_SHORT(v) : LE_SHORT(v))
#define TO_CPU_INT(v, be) \
	((be) ? BE_INT(v) : LE_INT(v))

#define WAV_RIFF		COMPOSE_ID('R','I','F','F')
#define WAV_RIFX		COMPOSE_ID('R','I','F','X')
#define WAV_WAVE		COMPOSE_ID('W','A','V','E')
#define WAV_FMT			COMPOSE_ID('f','m','t',' ')
#define WAV_DATA		COMPOSE_ID('d','a','t','a')

/* WAVE fmt block constants from Microsoft mmreg.h header */
#define WAV_FMT_PCM             0x0001

#define WAV_FMT_IEEE_FLOAT      0x0003
#define WAV_FMT_DOLBY_AC3_SPDIF 0x0092
#define WAV_FMT_EXTENSIBLE      0xfffe

enum
{
	AUDIO_USER_FORMAT_U8 = 0,
	AUDIO_USER_FORMAT_S16_BE,
	AUDIO_USER_FORMAT_S16_LE,
	AUDIO_USER_FORMAT_S24_3BE,
	AUDIO_USER_FORMAT_S24_3LE,
	AUDIO_USER_FORMAT_S24_BE,
	AUDIO_USER_FORMAT_S24_LE,
	AUDIO_USER_FORMAT_S32_BE,
	AUDIO_USER_FORMAT_S32_LE,
	AUDIO_USER_FORMAT_FLOAT_BE,
	AUDIO_USER_FORMAT_FLOAT_LE,
}AUDIO_FORMAT_USER_DEFINE;

#define SND_PCM_FORMAT_U8			 AUDIO_USER_FORMAT_U8
#define SND_PCM_FORMAT_S16_BE		 AUDIO_USER_FORMAT_S16_BE
#define SND_PCM_FORMAT_S16_LE		 AUDIO_USER_FORMAT_S16_LE
#define SND_PCM_FORMAT_S24_3BE		 AUDIO_USER_FORMAT_S24_3BE
#define SND_PCM_FORMAT_S24_3LE		 AUDIO_USER_FORMAT_S24_3LE
#define SND_PCM_FORMAT_S24_BE		 AUDIO_USER_FORMAT_S24_BE
#define SND_PCM_FORMAT_S24_LE		 AUDIO_USER_FORMAT_S24_LE
#define SND_PCM_FORMAT_S32_BE		 AUDIO_USER_FORMAT_S32_BE
#define SND_PCM_FORMAT_S32_LE		 AUDIO_USER_FORMAT_S32_LE
#define SND_PCM_FORMAT_FLOAT_BE		 AUDIO_USER_FORMAT_FLOAT_BE
#define SND_PCM_FORMAT_FLOAT_LE		 AUDIO_USER_FORMAT_FLOAT_LE

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

#endif