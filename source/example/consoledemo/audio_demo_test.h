#pragma once
#ifndef _AUDIO_DEMO_TEST_H_
#define _AUDIO_DEMO_TEST_H_

#include "Common/DataType.h"

#define AUDIO_BUFFER_SIZE 0x10000
#define AUDIO_SERVER_ADDR "192.168.1.6"
#define LEFT_CHANNEL_ADDR 0x35
#define RIGHT_CHANNEL_ADDR 0x36

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

typedef enum
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


typedef struct
{
	SocktPlugin_t SocketPluginObject;
	TCPIP_Client_t Tunl_Config;
}Audio_Test_Config_t;

extern Audio_Test_Config_t gTest_config;
extern char gBuffer[AUDIO_BUFFER_SIZE];

//SIA9118 Register Info
#if 1

#define JX_SIA9118_STAT_INTERN1_MTP_UPDATE_POS   0
#define JX_SIA9118_STAT_INTERN1_MTP_UPDATE_MASK 0x1

#define JX_SIA9118_STAT_INTERN1_DCST_PFM_POS   1
#define JX_SIA9118_STAT_INTERN1_DCST_PFM_MASK 0x1

#define JX_SIA9118_STAT_INTERN1_PORST_POS   2
#define JX_SIA9118_STAT_INTERN1_PORST_MASK 0x1

#define JX_SIA9118_STAT_INTERN1_MUT_FIN_POS   3
#define JX_SIA9118_STAT_INTERN1_MUT_FIN_MASK 0x1

#define JX_SIA9118_STAT_INTERN1_UNMUT_FIN_POS   4
#define JX_SIA9118_STAT_INTERN1_UNMUT_FIN_MASK 0x1

#define JX_SIA9118_STAT_INTERN1_OVP_ST_POS   5
#define JX_SIA9118_STAT_INTERN1_OVP_ST_MASK 0x1

#define JX_SIA9118_STAT_INTERN1_DC_CLIP_ST_POS   6
#define JX_SIA9118_STAT_INTERN1_DC_CLIP_ST_MASK 0x1

#define JX_SIA9118_STAT_INTERN1_SENADC_RDY_POS   7
#define JX_SIA9118_STAT_INTERN1_SENADC_RDY_MASK 0x1

#define JX_SIA9118_STAT_INTERN1_AREF_RDY_POS   8
#define JX_SIA9118_STAT_INTERN1_AREF_RDY_MASK 0x1

#define JX_SIA9118_STAT_INTERN1_AMP_CLIPS_POS   9
#define JX_SIA9118_STAT_INTERN1_AMP_CLIPS_MASK 0x1

#define JX_SIA9118_STAT_INTERN1_LNMODE_ST_POS   10
#define JX_SIA9118_STAT_INTERN1_LNMODE_ST_MASK 0x1

#define JX_SIA9118_STAT_INTERN1_I2S_RX_RDERR_POS   11
#define JX_SIA9118_STAT_INTERN1_I2S_RX_RDERR_MASK 0x1

#define JX_SIA9118_STAT_INTERN1_I2S_RX_WRERR_POS   12
#define JX_SIA9118_STAT_INTERN1_I2S_RX_WRERR_MASK 0x1

#define JX_SIA9118_STAT_INTERN1_I2S_TX_RDERR_POS   13
#define JX_SIA9118_STAT_INTERN1_I2S_TX_RDERR_MASK 0x1

#define JX_SIA9118_STAT_INTERN1_I2S_TX_WRERR_POS   14
#define JX_SIA9118_STAT_INTERN1_I2S_TX_WRERR_MASK 0x1

#define JX_SIA9118_STAT_INTERN1_I2S_SYNC_ERR_POS   15
#define JX_SIA9118_STAT_INTERN1_I2S_SYNC_ERR_MASK 0x1

#define JX_SIA9118_STAT_INTERN2_DEVICE_STAT_POS   0
#define JX_SIA9118_STAT_INTERN2_DEVICE_STAT_MASK 0xF

#define JX_SIA9118_STAT_INTERN2_LPMODE_ST_POS   4
#define JX_SIA9118_STAT_INTERN2_LPMODE_ST_MASK 0x1

#define JX_SIA9118_STAT_INTERN2_BST_UNRDY_POS   5
#define JX_SIA9118_STAT_INTERN2_BST_UNRDY_MASK 0x1

#define JX_SIA9118_STAT_INTERN2_BST_INWIN_POS   6
#define JX_SIA9118_STAT_INTERN2_BST_INWIN_MASK 0x1

#define JX_SIA9118_STAT_INTERN2_AMP_UNSW_POS   7
#define JX_SIA9118_STAT_INTERN2_AMP_UNSW_MASK 0x1

#define JX_SIA9118_STAT_INTERN2_PLL_REF_NOCK_POS   8
#define JX_SIA9118_STAT_INTERN2_PLL_REF_NOCK_MASK 0x1

#define JX_SIA9118_STAT_INTERN2_PLL_REF_OOR_POS   9
#define JX_SIA9118_STAT_INTERN2_PLL_REF_OOR_MASK 0x1

#define JX_SIA9118_STAT_INTERN2_PLL_STABLE_POS   10
#define JX_SIA9118_STAT_INTERN2_PLL_STABLE_MASK 0x1


#define JX_SIA9118_STAT_INTERN2_POWDN_ST_POS   14
#define JX_SIA9118_STAT_INTERN2_POWDN_ST_MASK 0x1

#define JX_SIA9118_STAT_INTERN2_OPRT_ST_POS   15
#define JX_SIA9118_STAT_INTERN2_OPRT_ST_MASK 0x1

#define JX_SIA9118_ADC_TMP_TEMPS_POS   0
#define JX_SIA9118_ADC_TMP_TEMPS_MASK 0x3FF

#define JX_SIA9118_ADC_VDDP_ADC_VDDP_POS   0
#define JX_SIA9118_ADC_VDDP_ADC_VDDP_MASK 0x3FF

#define JX_SIA9118_ADC_VBAT_ADC_VBAT_POS   0
#define JX_SIA9118_ADC_VBAT_ADC_VBAT_MASK 0x3FF

#define JX_SIA9118_VERSION_MMSK_VER_POS   0
#define JX_SIA9118_VERSION_MMSK_VER_MASK 0xF

#define JX_SIA9118_VERSION_FMSK_VER_POS   4
#define JX_SIA9118_VERSION_FMSK_VER_MASK 0x7

#define JX_SIA9118_VERSION_NAME_VER_POS   7
#define JX_SIA9118_VERSION_NAME_VER_MASK 0x1FF

#define JX_SIA9118_INT_STAT_PORST_POS   0
#define JX_SIA9118_INT_STAT_PORST_MASK 0x1

#define JX_SIA9118_INT_STAT_BODS_POS   1
#define JX_SIA9118_INT_STAT_BODS_MASK 0x1

#define JX_SIA9118_INT_STAT_NOCLKS_POS   2
#define JX_SIA9118_INT_STAT_NOCLKS_MASK 0x1

#define JX_SIA9118_INT_STAT_OTPS_POS   3
#define JX_SIA9118_INT_STAT_OTPS_MASK 0x1

#define JX_SIA9118_INT_STAT_OVPS_POS   4
#define JX_SIA9118_INT_STAT_OVPS_MASK 0x1

#define JX_SIA9118_INT_STAT_UVPS_POS   5
#define JX_SIA9118_INT_STAT_UVPS_MASK 0x1

#define JX_SIA9118_INT_STAT_OCPS_POS   6
#define JX_SIA9118_INT_STAT_OCPS_MASK 0x1

#define JX_SIA9118_INT_STAT_TDMERRS_POS   7
#define JX_SIA9118_INT_STAT_TDMERRS_MASK 0x1

#define JX_SIA9118_INT_STAT_BSTOCPS_POS   8
#define JX_SIA9118_INT_STAT_BSTOCPS_MASK 0x1

#define JX_SIA9118_INT_STAT_STALARMS_POS   9
#define JX_SIA9118_INT_STAT_STALARMS_MASK 0x1


#define JX_SIA9118_INT_STAT_DUMMY0S_POS   12
#define JX_SIA9118_INT_STAT_DUMMY0S_MASK 0x1

#define JX_SIA9118_INT_STAT_DUMMY1S_POS   13
#define JX_SIA9118_INT_STAT_DUMMY1S_MASK 0x1

#define JX_SIA9118_INT_STAT_DUMMY2S_POS   14
#define JX_SIA9118_INT_STAT_DUMMY2S_MASK 0x1

#define JX_SIA9118_INT_STAT_DUMMY3S_POS   15
#define JX_SIA9118_INT_STAT_DUMMY3S_MASK 0x1

#define JX_SIA9118_INT_WC_REG_PORC_POS   0
#define JX_SIA9118_INT_WC_REG_PORC_MASK 0x1

#define JX_SIA9118_INT_WC_REG_BODC_POS   1
#define JX_SIA9118_INT_WC_REG_BODC_MASK 0x1

#define JX_SIA9118_INT_WC_REG_NOCLKC_POS   2
#define JX_SIA9118_INT_WC_REG_NOCLKC_MASK 0x1

#define JX_SIA9118_INT_WC_REG_OTPC_POS   3
#define JX_SIA9118_INT_WC_REG_OTPC_MASK 0x1

#define JX_SIA9118_INT_WC_REG_OVPC_POS   4
#define JX_SIA9118_INT_WC_REG_OVPC_MASK 0x1

#define JX_SIA9118_INT_WC_REG_UVPC_POS   5
#define JX_SIA9118_INT_WC_REG_UVPC_MASK 0x1

#define JX_SIA9118_INT_WC_REG_OCPC_POS   6
#define JX_SIA9118_INT_WC_REG_OCPC_MASK 0x1

#define JX_SIA9118_INT_WC_REG_TDMERRC_POS   7
#define JX_SIA9118_INT_WC_REG_TDMERRC_MASK 0x1

#define JX_SIA9118_INT_WC_REG_BSTOCPC_POS   8
#define JX_SIA9118_INT_WC_REG_BSTOCPC_MASK 0x1

#define JX_SIA9118_INT_WC_REG_STALARMC_POS   9
#define JX_SIA9118_INT_WC_REG_STALARMC_MASK 0x1


#define JX_SIA9118_INT_WC_REG_DUMMY0C_POS   12
#define JX_SIA9118_INT_WC_REG_DUMMY0C_MASK 0x1

#define JX_SIA9118_INT_WC_REG_DUMMY1C_POS   13
#define JX_SIA9118_INT_WC_REG_DUMMY1C_MASK 0x1

#define JX_SIA9118_INT_WC_REG_DUMMY2C_POS   14
#define JX_SIA9118_INT_WC_REG_DUMMY2C_MASK 0x1

#define JX_SIA9118_INT_WC_REG_DUMMY3C_POS   15
#define JX_SIA9118_INT_WC_REG_DUMMY3C_MASK 0x1

#define JX_SIA9118_INT_REG_PORI_POS   0
#define JX_SIA9118_INT_REG_PORI_MASK 0x1

#define JX_SIA9118_INT_REG_BODI_POS   1
#define JX_SIA9118_INT_REG_BODI_MASK 0x1

#define JX_SIA9118_INT_REG_NOCLKI_POS   2
#define JX_SIA9118_INT_REG_NOCLKI_MASK 0x1

#define JX_SIA9118_INT_REG_OTPI_POS   3
#define JX_SIA9118_INT_REG_OTPI_MASK 0x1

#define JX_SIA9118_INT_REG_OVPI_POS   4
#define JX_SIA9118_INT_REG_OVPI_MASK 0x1

#define JX_SIA9118_INT_REG_UVPI_POS   5
#define JX_SIA9118_INT_REG_UVPI_MASK 0x1

#define JX_SIA9118_INT_REG_OCPI_POS   6
#define JX_SIA9118_INT_REG_OCPI_MASK 0x1

#define JX_SIA9118_INT_REG_TDMERRI_POS   7
#define JX_SIA9118_INT_REG_TDMERRI_MASK 0x1

#define JX_SIA9118_INT_REG_BSTOCPI_POS   8
#define JX_SIA9118_INT_REG_BSTOCPI_MASK 0x1

#define JX_SIA9118_INT_REG_STALARMI_POS   9
#define JX_SIA9118_INT_REG_STALARMI_MASK 0x1


#define JX_SIA9118_INT_REG_DUMMY0I_POS   12
#define JX_SIA9118_INT_REG_DUMMY0I_MASK 0x1

#define JX_SIA9118_INT_REG_DUMMY1I_POS   13
#define JX_SIA9118_INT_REG_DUMMY1I_MASK 0x1

#define JX_SIA9118_INT_REG_DUMMY2I_POS   14
#define JX_SIA9118_INT_REG_DUMMY2I_MASK 0x1

#define JX_SIA9118_INT_REG_DUMMY3I_POS   15
#define JX_SIA9118_INT_REG_DUMMY3I_MASK 0x1

#define JX_SIA9118_INT_EN_PORE_POS   0
#define JX_SIA9118_INT_EN_PORE_MASK 0x1

#define JX_SIA9118_INT_EN_BODE_POS   1
#define JX_SIA9118_INT_EN_BODE_MASK 0x1

#define JX_SIA9118_INT_EN_NOCLKE_POS   2
#define JX_SIA9118_INT_EN_NOCLKE_MASK 0x1

#define JX_SIA9118_INT_EN_OTPE_POS   3
#define JX_SIA9118_INT_EN_OTPE_MASK 0x1

#define JX_SIA9118_INT_EN_OVPE_POS   4
#define JX_SIA9118_INT_EN_OVPE_MASK 0x1

#define JX_SIA9118_INT_EN_UVPE_POS   5
#define JX_SIA9118_INT_EN_UVPE_MASK 0x1

#define JX_SIA9118_INT_EN_OCPE_POS   6
#define JX_SIA9118_INT_EN_OCPE_MASK 0x1

#define JX_SIA9118_INT_EN_TDMERRE_POS   7
#define JX_SIA9118_INT_EN_TDMERRE_MASK 0x1

#define JX_SIA9118_INT_EN_BSTOCPE_POS   8
#define JX_SIA9118_INT_EN_BSTOCPE_MASK 0x1

#define JX_SIA9118_INT_EN_STALARME_POS   9
#define JX_SIA9118_INT_EN_STALARME_MASK 0x1


#define JX_SIA9118_INT_EN_DUMMY0E_POS   12
#define JX_SIA9118_INT_EN_DUMMY0E_MASK 0x1

#define JX_SIA9118_INT_EN_DUMMY1E_POS   13
#define JX_SIA9118_INT_EN_DUMMY1E_MASK 0x1

#define JX_SIA9118_INT_EN_DUMMY2E_POS   14
#define JX_SIA9118_INT_EN_DUMMY2E_MASK 0x1

#define JX_SIA9118_INT_EN_DUMMY3E_POS   15
#define JX_SIA9118_INT_EN_DUMMY3E_MASK 0x1

#define JX_SIA9118_SYS_CTR_POST_OCPBYP_POS   0
#define JX_SIA9118_SYS_CTR_POST_OCPBYP_MASK 0x1

#define JX_SIA9118_SYS_CTR_POST_OTPBYP_POS   1
#define JX_SIA9118_SYS_CTR_POST_OTPBYP_MASK 0x1

#define JX_SIA9118_SYS_CTR_POST_OVPBYP_POS   2
#define JX_SIA9118_SYS_CTR_POST_OVPBYP_MASK 0x1

#define JX_SIA9118_SYS_CTR_POST_UVPBYP_POS   3
#define JX_SIA9118_SYS_CTR_POST_UVPBYP_MASK 0x1

#define JX_SIA9118_SYS_CTR_POST_VDIGBODBYP_POS   4
#define JX_SIA9118_SYS_CTR_POST_VDIGBODBYP_MASK 0x1

#define JX_SIA9118_SYS_CTR_POST_PLLBYP_POS   5
#define JX_SIA9118_SYS_CTR_POST_PLLBYP_MASK 0x1

#define JX_SIA9118_SYS_CTR_POST_TDMERRBYP_POS   6
#define JX_SIA9118_SYS_CTR_POST_TDMERRBYP_MASK 0x1

#define JX_SIA9118_SYS_CTR_POST_VDIGBOD_TIME_POS   7
#define JX_SIA9118_SYS_CTR_POST_VDIGBOD_TIME_MASK 0x3

#define JX_SIA9118_SYS_CTR_POST_VDIGBOD_EN_POS   9
#define JX_SIA9118_SYS_CTR_POST_VDIGBOD_EN_MASK 0x1

#define JX_SIA9118_SYS_CTR_POST_OCP_SRC_POS   10
#define JX_SIA9118_SYS_CTR_POST_OCP_SRC_MASK 0x1

#define JX_SIA9118_SYS_CTR_POST_GLTCH_RST_POS   11
#define JX_SIA9118_SYS_CTR_POST_GLTCH_RST_MASK 0x1

#define JX_SIA9118_SYS_CTR_POST_NO_CLK_OOR_BYP_POS   12
#define JX_SIA9118_SYS_CTR_POST_NO_CLK_OOR_BYP_MASK 0x1

#define JX_SIA9118_SYS_CTR_POST_UOVP_SRC_POS   13
#define JX_SIA9118_SYS_CTR_POST_UOVP_SRC_MASK 0x1

#define JX_SIA9118_SYS_CTR_POST_CLKG_MUT_DIS_POS   14
#define JX_SIA9118_SYS_CTR_POST_CLKG_MUT_DIS_MASK 0x1

#define JX_SIA9118_SYS_CTR_POST_IIC_CONF_FIN_POS   15
#define JX_SIA9118_SYS_CTR_POST_IIC_CONF_FIN_MASK 0x1

#define JX_SIA9118_SYS_CTR_PRE_PWDN_POS   0
#define JX_SIA9118_SYS_CTR_PRE_PWDN_MASK 0x1

#define JX_SIA9118_SYS_CTR_PRE_I2CRST_POS   1
#define JX_SIA9118_SYS_CTR_PRE_I2CRST_MASK 0x1

#define JX_SIA9118_SYS_CTR_PRE_AMPEN_POS   2
#define JX_SIA9118_SYS_CTR_PRE_AMPEN_MASK 0x1

#define JX_SIA9118_SYS_CTR_PRE_DC_DIS_POS   3
#define JX_SIA9118_SYS_CTR_PRE_DC_DIS_MASK 0x1

#define JX_SIA9118_SYS_CTR_PRE_PLLREFSEL_POS   4
#define JX_SIA9118_SYS_CTR_PRE_PLLREFSEL_MASK 0x3

#define JX_SIA9118_SYS_CTR_PRE_BCK_INV_POS   6
#define JX_SIA9118_SYS_CTR_PRE_BCK_INV_MASK 0x1

#define JX_SIA9118_SYS_CTR_PRE_WS_INV_POS   7
#define JX_SIA9118_SYS_CTR_PRE_WS_INV_MASK 0x1

#define JX_SIA9118_SYS_CTR_PRE_TDM_EN_POS   8
#define JX_SIA9118_SYS_CTR_PRE_TDM_EN_MASK 0x1

#define JX_SIA9118_SYS_CTR_PRE_TXUHZ_POS   9
#define JX_SIA9118_SYS_CTR_PRE_TXUHZ_MASK 0x1

#define JX_SIA9118_SYS_CTR_PRE_INT_SEL_POS   10
#define JX_SIA9118_SYS_CTR_PRE_INT_SEL_MASK 0x1

#define JX_SIA9118_SYS_CTR_PRE_INT_DRM_POS   11
#define JX_SIA9118_SYS_CTR_PRE_INT_DRM_MASK 0x1

#define JX_SIA9118_SYS_CTR_PRE_INT_POR_POS   12
#define JX_SIA9118_SYS_CTR_PRE_INT_POR_MASK 0x1

#define JX_SIA9118_SYS_CTR_PRE_RST_REG_EN_POS   13
#define JX_SIA9118_SYS_CTR_PRE_RST_REG_EN_MASK 0x1


#define JX_SIA9118_SYS_CTR_PRE_INTCLKON_POS   15
#define JX_SIA9118_SYS_CTR_PRE_INTCLKON_MASK 0x1

#define JX_SIA9118_I2S_CONF1_AUDFS_POS   0
#define JX_SIA9118_I2S_CONF1_AUDFS_MASK 0xF

#define JX_SIA9118_I2S_CONF1_TDMBCK_POS   4
#define JX_SIA9118_I2S_CONF1_TDMBCK_MASK 0x7

#define JX_SIA9118_I2S_CONF1_TDMDSIZE_POS   7
#define JX_SIA9118_I2S_CONF1_TDMDSIZE_MASK 0x3

#define JX_SIA9118_I2S_CONF1_TDM_DLY_POS   9
#define JX_SIA9118_I2S_CONF1_TDM_DLY_MASK 0x1

#define JX_SIA9118_I2S_CONF1_TDM_JST_POS   10
#define JX_SIA9118_I2S_CONF1_TDM_JST_MASK 0x1

#define JX_SIA9118_I2S_CONF1_INPLEV_POS   11
#define JX_SIA9118_I2S_CONF1_INPLEV_MASK 0x1

#define JX_SIA9118_I2S_CONF1_SLTNU_POS   12
#define JX_SIA9118_I2S_CONF1_SLTNU_MASK 0x7

#define JX_SIA9118_I2S_CONF1_FS_LEN_POS   15
#define JX_SIA9118_I2S_CONF1_FS_LEN_MASK 0x1

#define JX_SIA9118_I2S_CONF2_TX0SEL_POS   0
#define JX_SIA9118_I2S_CONF2_TX0SEL_MASK 0x7

#define JX_SIA9118_I2S_CONF2_TX1SEL_POS   3
#define JX_SIA9118_I2S_CONF2_TX1SEL_MASK 0x7

#define JX_SIA9118_I2S_CONF2_INSEL_POS   6
#define JX_SIA9118_I2S_CONF2_INSEL_MASK 0x3

#define JX_SIA9118_I2S_CONF2_RX0SEL_POS   8
#define JX_SIA9118_I2S_CONF2_RX0SEL_MASK 0x7

#define JX_SIA9118_I2S_CONF2_RX1SEL_POS   11
#define JX_SIA9118_I2S_CONF2_RX1SEL_MASK 0x7

#define JX_SIA9118_I2S_CONF2_DLY_TDM_POS   14
#define JX_SIA9118_I2S_CONF2_DLY_TDM_MASK 0x3

#define JX_SIA9118_I2S_CONF3_TX0MAP_POS   0
#define JX_SIA9118_I2S_CONF3_TX0MAP_MASK 0x1F


#define JX_SIA9118_I2S_CONF3_TX1MAP_POS   8
#define JX_SIA9118_I2S_CONF3_TX1MAP_MASK 0x1F

#define JX_SIA9118_I2S_CONF3_GLTCH_OCP_FILT_POS   13
#define JX_SIA9118_I2S_CONF3_GLTCH_OCP_FILT_MASK 0x7

#define JX_SIA9118_PWM_BOD_HPFBYP_POS   0
#define JX_SIA9118_PWM_BOD_HPFBYP_MASK 0x1

#define JX_SIA9118_PWM_BOD_CLIPBYP_POS   1
#define JX_SIA9118_PWM_BOD_CLIPBYP_MASK 0x1

#define JX_SIA9118_PWM_BOD_PWM_HMUTE_POS   2
#define JX_SIA9118_PWM_BOD_PWM_HMUTE_MASK 0x1

#define JX_SIA9118_PWM_BOD_PWMBDELY_POS   3
#define JX_SIA9118_PWM_BOD_PWMBDELY_MASK 0x1F

#define JX_SIA9118_PWM_BOD_PWMB_POL_POS   8
#define JX_SIA9118_PWM_BOD_PWMB_POL_MASK 0x1

#define JX_SIA9118_PWM_BOD_PWMGAIN_POS   9
#define JX_SIA9118_PWM_BOD_PWMGAIN_MASK 0x3

#define JX_SIA9118_PWM_BOD_VDIG_BODTH_POS   11
#define JX_SIA9118_PWM_BOD_VDIG_BODTH_MASK 0x3

#define JX_SIA9118_PWM_BOD_GLTCH_FILT_POS   13
#define JX_SIA9118_PWM_BOD_GLTCH_FILT_MASK 0x1

#define JX_SIA9118_PWM_BOD_SLOPE_SEL_POS   14
#define JX_SIA9118_PWM_BOD_SLOPE_SEL_MASK 0x3

#define JX_SIA9118_DCDC_CONF1_VDCHR_POS   0
#define JX_SIA9118_DCDC_CONF1_VDCHR_MASK 0x3

#define JX_SIA9118_DCDC_CONF1_DCINSEL_POS   2
#define JX_SIA9118_DCDC_CONF1_DCINSEL_MASK 0x3

#define JX_SIA9118_DCDC_CONF1_DCMOD_POS   4
#define JX_SIA9118_DCDC_CONF1_DCMOD_MASK 0x3

#define JX_SIA9118_DCDC_CONF1_VSCL1_POS   6
#define JX_SIA9118_DCDC_CONF1_VSCL1_MASK 0x3

#define JX_SIA9118_DCDC_CONF1_VSCL2_POS   8
#define JX_SIA9118_DCDC_CONF1_VSCL2_MASK 0x3

#define JX_SIA9118_DCDC_CONF1_VSCL3_POS   10
#define JX_SIA9118_DCDC_CONF1_VSCL3_MASK 0x3

#define JX_SIA9118_DCDC_CONF1_DCRLST_POS   12
#define JX_SIA9118_DCDC_CONF1_DCRLST_MASK 0x7

#define JX_SIA9118_DCDC_CONF1_ADP_RLS_STP_POS   15
#define JX_SIA9118_DCDC_CONF1_ADP_RLS_STP_MASK 0x1

#define JX_SIA9118_DCDC_CONF2_DCMCC_POS   0
#define JX_SIA9118_DCDC_CONF2_DCMCC_MASK 0xF

#define JX_SIA9118_DCDC_CONF2_DCVO1_POS   4
#define JX_SIA9118_DCDC_CONF2_DCVO1_MASK 0x1F

#define JX_SIA9118_DCDC_CONF2_DCVO2_POS   9
#define JX_SIA9118_DCDC_CONF2_DCVO2_MASK 0x1F

#define JX_SIA9118_DCDC_CONF2_DC_RLS_STP_POS   14
#define JX_SIA9118_DCDC_CONF2_DC_RLS_STP_MASK 0x3

#define JX_SIA9118_DCDC_CONF3_DC_CLKSEL_POS   0
#define JX_SIA9118_DCDC_CONF3_DC_CLKSEL_MASK 0x3

#define JX_SIA9118_DCDC_CONF3_PFM_ADJ_MOD_POS   2
#define JX_SIA9118_DCDC_CONF3_PFM_ADJ_MOD_MASK 0x3

#define JX_SIA9118_DCDC_CONF3_ANTI_RING_T_POS   4
#define JX_SIA9118_DCDC_CONF3_ANTI_RING_T_MASK 0x3

#define JX_SIA9118_DCDC_CONF3_DC_SFT_T_POS   6
#define JX_SIA9118_DCDC_CONF3_DC_SFT_T_MASK 0x3

#define JX_SIA9118_DCDC_CONF3_DC_L_VAL_POS   8
#define JX_SIA9118_DCDC_CONF3_DC_L_VAL_MASK 0x3

#define JX_SIA9118_DCDC_CONF3_FORCE_PWM_POS   10
#define JX_SIA9118_DCDC_CONF3_FORCE_PWM_MASK 0x1

#define JX_SIA9118_DCDC_CONF3_FORCE_PFM_POS   11
#define JX_SIA9118_DCDC_CONF3_FORCE_PFM_MASK 0x1

#define JX_SIA9118_DCDC_CONF3_INIT_MOD_CHNG_POS   12
#define JX_SIA9118_DCDC_CONF3_INIT_MOD_CHNG_MASK 0x1

#define JX_SIA9118_DCDC_CONF3_DC_SLW_SPD_POS   13
#define JX_SIA9118_DCDC_CONF3_DC_SLW_SPD_MASK 0x1

#define JX_SIA9118_DCDC_CONF3_PWM_WIN_POS   14
#define JX_SIA9118_DCDC_CONF3_PWM_WIN_MASK 0x1

#define JX_SIA9118_DCDC_CONF3_DC_PE_ON_POS   15
#define JX_SIA9118_DCDC_CONF3_DC_PE_ON_MASK 0x1

#define JX_SIA9118_DCIN_SPKG_DCINGAIN_POS   0
#define JX_SIA9118_DCIN_SPKG_DCINGAIN_MASK 0xF

#define JX_SIA9118_DCIN_SPKG_SPKGAIN_POS   4
#define JX_SIA9118_DCIN_SPKG_SPKGAIN_MASK 0xF

#define JX_SIA9118_DCIN_SPKG_ADPDC_AHDT_POS   8
#define JX_SIA9118_DCIN_SPKG_ADPDC_AHDT_MASK 0x3

#define JX_SIA9118_DCIN_SPKG_DC_WIN_DBL_POS   10
#define JX_SIA9118_DCIN_SPKG_DC_WIN_DBL_MASK 0x1

#define JX_SIA9118_DCIN_SPKG_SPOWON_BYPS_POS   11
#define JX_SIA9118_DCIN_SPKG_SPOWON_BYPS_MASK 0x1

#define JX_SIA9118_DCIN_SPKG_DC_CMP_SRC_POS   12
#define JX_SIA9118_DCIN_SPKG_DC_CMP_SRC_MASK 0x1

#define JX_SIA9118_DCIN_SPKG_FIX_ABVBAT_POS   13
#define JX_SIA9118_DCIN_SPKG_FIX_ABVBAT_MASK 0x1

#define JX_SIA9118_DCIN_SPKG_BST_OVP_EN_POS   14
#define JX_SIA9118_DCIN_SPKG_BST_OVP_EN_MASK 0x1

#define JX_SIA9118_DCIN_SPKG_ADP_RLS_ENHANCE_POS   15
#define JX_SIA9118_DCIN_SPKG_ADP_RLS_ENHANCE_MASK 0x1

#define JX_SIA9118_SADC_CONF_SADC_CHTST_POS   0
#define JX_SIA9118_SADC_CONF_SADC_CHTST_MASK 0x3

#define JX_SIA9118_SADC_CONF_SADC_CK_SEL_POS   2
#define JX_SIA9118_SADC_CONF_SADC_CK_SEL_MASK 0x1

#define JX_SIA9118_SADC_CONF_TMP_CMP_EN_POS   3
#define JX_SIA9118_SADC_CONF_TMP_CMP_EN_MASK 0x1

#define JX_SIA9118_SADC_CONF_SADC_PD_TGAP_POS   4
#define JX_SIA9118_SADC_CONF_SADC_PD_TGAP_MASK 0x3

#define JX_SIA9118_SADC_CONF_OTPSRC_POS   6
#define JX_SIA9118_SADC_CONF_OTPSRC_MASK 0x1

#define JX_SIA9118_SADC_CONF_LPMODE_POS   7
#define JX_SIA9118_SADC_CONF_LPMODE_MASK 0x3

#define JX_SIA9118_SADC_CONF_VTHLPM_POS   9
#define JX_SIA9118_SADC_CONF_VTHLPM_MASK 0x7

#define JX_SIA9118_SADC_CONF_PLL_STBL_WIN_T_POS   12
#define JX_SIA9118_SADC_CONF_PLL_STBL_WIN_T_MASK 0x3

#define JX_SIA9118_SADC_CONF_DC_DUTY_DB_POS   14
#define JX_SIA9118_SADC_CONF_DC_DUTY_DB_MASK 0x3

#define JX_SIA9118_MUTE_VOL_SVOL_POS   0
#define JX_SIA9118_MUTE_VOL_SVOL_MASK 0x7F

#define JX_SIA9118_MUTE_VOL_SVOL_EN_POS   7
#define JX_SIA9118_MUTE_VOL_SVOL_EN_MASK 0x1

#define JX_SIA9118_MUTE_VOL_SMUT_POS   8
#define JX_SIA9118_MUTE_VOL_SMUT_MASK 0x1

#define JX_SIA9118_MUTE_VOL_SMTIME_POS   9
#define JX_SIA9118_MUTE_VOL_SMTIME_MASK 0x7

#define JX_SIA9118_MUTE_VOL_SM_INT_EN_POS   12
#define JX_SIA9118_MUTE_VOL_SM_INT_EN_MASK 0x1

#define JX_SIA9118_MUTE_VOL_EXMUT_TOEN_POS   13
#define JX_SIA9118_MUTE_VOL_EXMUT_TOEN_MASK 0x1

#define JX_SIA9118_MUTE_VOL_SLW_SMUT_POS   14
#define JX_SIA9118_MUTE_VOL_SLW_SMUT_MASK 0x1

#define JX_SIA9118_MUTE_VOL_SHIFT_MSB_IN_POS   15
#define JX_SIA9118_MUTE_VOL_SHIFT_MSB_IN_MASK 0x1

#define JX_SIA9118_PILOT_CONF_PIFORCE_POS   0
#define JX_SIA9118_PILOT_CONF_PIFORCE_MASK 0x1

#define JX_SIA9118_PILOT_CONF_ALT_SON_SEL_POS   1
#define JX_SIA9118_PILOT_CONF_ALT_SON_SEL_MASK 0x1

#define JX_SIA9118_PILOT_CONF_PILOT_DC_SEL_POS   2
#define JX_SIA9118_PILOT_CONF_PILOT_DC_SEL_MASK 0x1

#define JX_SIA9118_PILOT_CONF_PILOTBYP_POS   3
#define JX_SIA9118_PILOT_CONF_PILOTBYP_MASK 0x1

#define JX_SIA9118_PILOT_CONF_NOTCHBYP_POS   4
#define JX_SIA9118_PILOT_CONF_NOTCHBYP_MASK 0x1

#define JX_SIA9118_PILOT_CONF_DPSC_SFT_EN_POS   5
#define JX_SIA9118_PILOT_CONF_DPSC_SFT_EN_MASK 0x1

#define JX_SIA9118_PILOT_CONF_FSIN_SEL_POS   6
#define JX_SIA9118_PILOT_CONF_FSIN_SEL_MASK 0x3

#define JX_SIA9118_PILOT_CONF_Q_NOTCH_POS   8
#define JX_SIA9118_PILOT_CONF_Q_NOTCH_MASK 0xF

#define JX_SIA9118_PILOT_CONF_AMP_PILOT_POS   12
#define JX_SIA9118_PILOT_CONF_AMP_PILOT_MASK 0xF

#define JX_SIA9118_DPSC_CONF_DPSC_LV_POS   0
#define JX_SIA9118_DPSC_CONF_DPSC_LV_MASK 0x3F

#define JX_SIA9118_DPSC_CONF_DPSC_SFT_STP_POS   6
#define JX_SIA9118_DPSC_CONF_DPSC_SFT_STP_MASK 0x3

#define JX_SIA9118_DPSC_CONF_N_SEG_MIN_POS   8
#define JX_SIA9118_DPSC_CONF_N_SEG_MIN_MASK 0x1F

#define JX_SIA9118_DPSC_CONF_DPSC_RLST_POS   13
#define JX_SIA9118_DPSC_CONF_DPSC_RLST_MASK 0x7

#define JX_SIA9118_PLL_CONF1_N_PLL_IIC_POS   0
#define JX_SIA9118_PLL_CONF1_N_PLL_IIC_MASK 0x3FFF

#define JX_SIA9118_PLL_CONF1_PLL_RDY_SRC_POS   14
#define JX_SIA9118_PLL_CONF1_PLL_RDY_SRC_MASK 0x3

#define JX_SIA9118_PLL_ST_TIME_M_PLL_IIC_POS   0
#define JX_SIA9118_PLL_ST_TIME_M_PLL_IIC_MASK 0xF

#define JX_SIA9118_PLL_ST_TIME_PLLSET_BY_IIC_POS   4
#define JX_SIA9118_PLL_ST_TIME_PLLSET_BY_IIC_MASK 0x1

#define JX_SIA9118_PLL_ST_TIME_AMPEN_SW_DLY_POS   5
#define JX_SIA9118_PLL_ST_TIME_AMPEN_SW_DLY_MASK 0x3

#define JX_SIA9118_PLL_ST_TIME_MUT_PST_DLY_POS   7
#define JX_SIA9118_PLL_ST_TIME_MUT_PST_DLY_MASK 0x3

#define JX_SIA9118_PLL_ST_TIME_PLLRDY_DLY_POS   9
#define JX_SIA9118_PLL_ST_TIME_PLLRDY_DLY_MASK 0xF

#define JX_SIA9118_PLL_ST_TIME_OOR_REF_RNG_POS   13
#define JX_SIA9118_PLL_ST_TIME_OOR_REF_RNG_MASK 0x1

#define JX_SIA9118_PLL_ST_TIME_OOR_WIN_T_POS   14
#define JX_SIA9118_PLL_ST_TIME_OOR_WIN_T_MASK 0x1

#define JX_SIA9118_PLL_ST_TIME_DC_DAC_CUT_POS   15
#define JX_SIA9118_PLL_ST_TIME_DC_DAC_CUT_MASK 0x1

#define JX_SIA9118_LNMODE_CONF_LNM_EN_POS   0
#define JX_SIA9118_LNMODE_CONF_LNM_EN_MASK 0x3

#define JX_SIA9118_LNMODE_CONF_LNM_SRC_POS   2
#define JX_SIA9118_LNMODE_CONF_LNM_SRC_MASK 0x1

#define JX_SIA9118_LNMODE_CONF_LNM_VTH1_POS   3
#define JX_SIA9118_LNMODE_CONF_LNM_VTH1_MASK 0x7

#define JX_SIA9118_LNMODE_CONF_LNM_VTH2_POS   6
#define JX_SIA9118_LNMODE_CONF_LNM_VTH2_MASK 0x7

#define JX_SIA9118_LNMODE_CONF_LNM_DLY_POS   9
#define JX_SIA9118_LNMODE_CONF_LNM_DLY_MASK 0x1F

#define JX_SIA9118_LNMODE_CONF_LNM_TIME_POS   14
#define JX_SIA9118_LNMODE_CONF_LNM_TIME_MASK 0x3

#define JX_SIA9118_IVADC_CONF_VADC_DLY_POS   0
#define JX_SIA9118_IVADC_CONF_VADC_DLY_MASK 0x3F

#define JX_SIA9118_IVADC_CONF_VADC_SRC_POS   6
#define JX_SIA9118_IVADC_CONF_VADC_SRC_MASK 0x1

#define JX_SIA9118_IVADC_CONF_IADC_CMP_EN_POS   7
#define JX_SIA9118_IVADC_CONF_IADC_CMP_EN_MASK 0x1

#define JX_SIA9118_IVADC_CONF_IADC_DLY_POS   8
#define JX_SIA9118_IVADC_CONF_IADC_DLY_MASK 0xF

#define JX_SIA9118_IVADC_CONF_OCP_LIMIT_DIS_POS   12
#define JX_SIA9118_IVADC_CONF_OCP_LIMIT_DIS_MASK 0x1

#define JX_SIA9118_CHIPID_LSB_CHIPID_LSB_POS   0
#define JX_SIA9118_CHIPID_LSB_CHIPID_LSB_MASK 0xFFFF

#define JX_SIA9118_CHIPID_MSB_CHIPID_MSB_POS   0
#define JX_SIA9118_CHIPID_MSB_CHIPID_MSB_MASK 0xFFFF

#define JX_SIA9118_TCAL_RCOS_RC_OS_POS   0
#define JX_SIA9118_TCAL_RCOS_RC_OS_MASK 0xFFFF

#define JX_SIA9118_TCAL_TPERC_TPERC_MSB_POS   0
#define JX_SIA9118_TCAL_TPERC_TPERC_MSB_MASK 0xFFFF

#define JX_SIA9118_TCAL_LSB_TPERC_LSB_POS   0
#define JX_SIA9118_TCAL_LSB_TPERC_LSB_MASK 0xF

#define JX_SIA9118_TCAL_LSB_TREF_LSB_POS   4
#define JX_SIA9118_TCAL_LSB_TREF_LSB_MASK 0xF

#define JX_SIA9118_TCAL_TREF_TREF_MSB_POS   0
#define JX_SIA9118_TCAL_TREF_TREF_MSB_MASK 0xFFFF

#define JX_SIA9118_SADC_CT1_ADC_CT1_POS   0
#define JX_SIA9118_SADC_CT1_ADC_CT1_MASK 0xFFFF

#define JX_SIA9118_SADC_CT2_ADC_CT2_POS   0
#define JX_SIA9118_SADC_CT2_ADC_CT2_MASK 0xFFFF

#define JX_SIA9118_UNLOCK_UNLOCK_POS   0
#define JX_SIA9118_UNLOCK_UNLOCK_MASK 0xFFFF

#define JX_SIA9118_FLASH_BURNMTP_POS   0
#define JX_SIA9118_FLASH_BURNMTP_MASK 0xFFFF

#define JX_SIA9118_TEST1_TRIG_DUMMY0_POS   0
#define JX_SIA9118_TEST1_TRIG_DUMMY0_MASK 0x1

#define JX_SIA9118_TEST1_TRIG_DUMMY1_POS   1
#define JX_SIA9118_TEST1_TRIG_DUMMY1_MASK 0x1

#define JX_SIA9118_TEST1_TRIG_DUMMY2_POS   2
#define JX_SIA9118_TEST1_TRIG_DUMMY2_MASK 0x1

#define JX_SIA9118_TEST1_TRIG_DUMMY3_POS   3
#define JX_SIA9118_TEST1_TRIG_DUMMY3_MASK 0x1

#define JX_SIA9118_TEST1_CKREG_SCL_POS   4
#define JX_SIA9118_TEST1_CKREG_SCL_MASK 0x1


#define JX_SIA9118_TEST1_ANA_FRCE_EN_POS   8
#define JX_SIA9118_TEST1_ANA_FRCE_EN_MASK 0x1

#define JX_SIA9118_TEST1_AREF_FV_POS   9
#define JX_SIA9118_TEST1_AREF_FV_MASK 0x1

#define JX_SIA9118_TEST1_CLK1M_FV_POS   10
#define JX_SIA9118_TEST1_CLK1M_FV_MASK 0x1

#define JX_SIA9118_TEST1_PLL_FV_POS   11
#define JX_SIA9118_TEST1_PLL_FV_MASK 0x1

#define JX_SIA9118_TEST1_DCDC_FV_POS   12
#define JX_SIA9118_TEST1_DCDC_FV_MASK 0x1

#define JX_SIA9118_TEST1_DLOOP_FV_POS   13
#define JX_SIA9118_TEST1_DLOOP_FV_MASK 0x1

#define JX_SIA9118_TEST1_DDRV_FV_POS   14
#define JX_SIA9118_TEST1_DDRV_FV_MASK 0x1

#define JX_SIA9118_TEST1_SIVADC_FV_POS   15
#define JX_SIA9118_TEST1_SIVADC_FV_MASK 0x1
#endif

typedef enum nxpTfa9874BfEnumList {
	TFA9874_BF_PWDN = 0x0000,    	/*!< Powerdown selection                                */
	TFA9874_BF_I2CR = 0x0010,    		/*!< I2C Reset - Auto clear                             */
	TFA9874_BF_AMPE = 0x0030,    	/*!< Activate Amplifier                                 */
	TFA9874_BF_DCA = 0x0040,    		/*!< Activate DC-to-DC converter                        */
	TFA9874_BF_INTP = 0x0071,    		/*!< Interrupt config                                   */
	TFA9874_BF_BYPOCP = 0x00b0,    	/*!< Bypass OCP                                         */
	TFA9874_BF_TSTOCP = 0x00c0,    	/*!< OCP testing control                                */
	TFA9874_BF_MANSCONF = 0x0120,    /*!< I2C configured                                     */
	TFA9874_BF_MANAOOSC = 0x0140,    /*!< Internal osc off at PWDN                           */
	TFA9874_BF_MUTETO = 0x01d0,    	/*!< Time out SB mute sequence                          */
	TFA9874_BF_OPENMTP = 0x01e0,    	/*!< Control for FAIM protection                        */
	TFA9874_BF_AUDFS = 0x0203,    	/*!< Sample rate (fs)                                   */
	TFA9874_BF_INPLEV = 0x0240,    	/*!< TDM output attenuation                             */
	TFA9874_BF_FRACTDEL = 0x0255,    	/*!< V/I Fractional delay                               */
	TFA9874_BF_REV = 0x030f,    		/*!< Revision info                                      */
	TFA9874_BF_REFCKEXT = 0x0401,    	/*!< PLL external ref clock                             */
	TFA9874_BF_REFCKSEL = 0x0420,    	/*!< PLL internal ref clock                             */
	TFA9874_BF_SSFAIME = 0x05c0,    	/*!< Sub-system FAIM                                    */
	TFA9874_BF_AMPOCRT = 0x0802,    	/*!< Amplifier on-off criteria for shutdown             */
	TFA9874_BF_VDDS = 0x1000,    	/*!< POR                                                */
	TFA9874_BF_DCOCPOK = 0x1010,    	/*!< DCDC OCP nmos   (sticky register , clear on read)  */
	TFA9874_BF_OTDS = 0x1020,    		/*!< OTP alarm   (sticky register , clear on read)      */
	TFA9874_BF_OCDS = 0x1030,    	/*!< OCP  amplifier   (sticky register , clear on read) */
	TFA9874_BF_UVDS = 0x1040,    	/*!< UVP alarm  (sticky register , clear on read)       */
	TFA9874_BF_MANALARM = 0x1050,    /*!< Alarm state                                        */
	TFA9874_BF_TDMERR = 0x1060,    	/*!< TDM error                                          */
	TFA9874_BF_NOCLK = 0x1070,    	/*!< Lost clock  (sticky register , clear on read)      */
	TFA9874_BF_DCIL = 0x1100,    		/*!< DCDC current limiting                              */
	TFA9874_BF_DCDCA = 0x1110,    	/*!< DCDC active  (sticky register , clear on read)     */
	TFA9874_BF_DCHVBAT = 0x1130,    	/*!< DCDC level 1x                                      */
	TFA9874_BF_DCH114 = 0x1140,    	/*!< DCDC level 1.14x                                   */
	TFA9874_BF_DCH107 = 0x1150,    	/*!< DCDC level 1.07x                                   */
	TFA9874_BF_PLLS = 0x1160,    		/*!< PLL lock                                           */
	TFA9874_BF_CLKS = 0x1170,    		/*!< Clocks stable                                      */
	TFA9874_BF_TDMLUTER = 0x1180,    	/*!< TDM LUT error                                      */
	TFA9874_BF_TDMSTAT = 0x1192,    	/*!< TDM status bits                                    */
	TFA9874_BF_MTPB = 0x11c0,    	/*!< MTP busy                                           */
	TFA9874_BF_SWS = 0x11d0,    		/*!< Amplifier engage                                   */
	TFA9874_BF_AMPS = 0x11e0,    	/*!< Amplifier enable                                   */
	TFA9874_BF_AREFS = 0x11f0,    	/*!< References enable                                  */
	TFA9874_BF_OCPOAP = 0x1300,    	/*!< OCPOK pmos A                                       */
	TFA9874_BF_OCPOAN = 0x1310,    	/*!< OCPOK nmos A                                       */
	TFA9874_BF_OCPOBP = 0x1320,    	/*!< OCPOK pmos B                                       */
	TFA9874_BF_OCPOBN = 0x1330,    	/*!< OCPOK nmos B                                       */
	TFA9874_BF_OVDS = 0x1380,    	/*!< OVP alarm                                          */
	TFA9874_BF_CLIPS = 0x1390,    		/*!< Amplifier  clipping                                */
	TFA9874_BF_ADCCR = 0x13a0,    	/*!< Control ADC                                        */
	TFA9874_BF_MANWAIT1 = 0x13c0,    /*!< Wait HW I2C settings                               */
	TFA9874_BF_MANMUTE = 0x13e0,    	/*!< Audio mute sequence                                */
	TFA9874_BF_MANOPER = 0x13f0,    	/*!< Operating state                                    */
	TFA9874_BF_CLKOOR = 0x1420,    	/*!< External clock status                              */
	TFA9874_BF_MANSTATE = 0x1433,    	/*!< Device manager status                              */
	TFA9874_BF_DCMODE = 0x1471,    	/*!< DCDC mode status bits                              */
	TFA9874_BF_BATS = 0x1509,    		/*!< Battery voltage (V)                                */
	TFA9874_BF_TEMPS = 0x1608,    	/*!< IC Temperature (C)                                 */
	TFA9874_BF_VDDPS = 0x1709,    	/*!< IC VDDP voltage ( 1023*VDDP/13 V)                  */
	TFA9874_BF_TDME = 0x2040,    	/*!< Enable interface                                   */
	TFA9874_BF_TDMMODE = 0x2050,    	/*!< Slave/master                                       */
	TFA9874_BF_TDMCLINV = 0x2060,    	/*!< Reception data to BCK clock                        */
	TFA9874_BF_TDMFSLN = 0x2073,    	/*!< FS length (master mode only)                       */
	TFA9874_BF_TDMFSPOL = 0x20b0,    	/*!< FS polarity                                        */
	TFA9874_BF_TDMNBCK = 0x20c3,    	/*!< N-BCK's in FS                                      */
	TFA9874_BF_TDMSLOTS = 0x2103,    	/*!< N-slots in Frame                                   */
	TFA9874_BF_TDMSLLN = 0x2144,    	/*!< N-bits in slot                                     */
	TFA9874_BF_TDMBRMG = 0x2194,    	/*!< N-bits remaining                                   */
	TFA9874_BF_TDMDEL = 0x21e0,    	/*!< data delay to FS                                   */
	TFA9874_BF_TDMADJ = 0x21f0,    	/*!< data adjustment                                    */
	TFA9874_BF_TDMOOMP = 0x2201,    	/*!< Received audio compression                         */
	TFA9874_BF_TDMSSIZE = 0x2224,    	/*!< Sample size per slot                               */
	TFA9874_BF_TDMTXDFO = 0x2271,    /*!< Format unused bits                                 */
	TFA9874_BF_TDMTXUS0 = 0x2291,    /*!< Format unused slots DATAO                          */
	TFA9874_BF_TDMSPKE = 0x2300,    	/*!< Control audio tdm channel in 0 (spkr + dcdc)       */
	TFA9874_BF_TDMDCE = 0x2310,    	/*!< Control audio  tdm channel in 1  (dcdc)            */
	TFA9874_BF_TDMCSE = 0x2330,    	/*!< current sense vbat temperature and vddp feedback   */
	TFA9874_BF_TDMVSE = 0x2340,    	/*!< Voltage sense vbat temperature and vddp feedback   */
	TFA9874_BF_TDMSPKS = 0x2603,    	/*!< tdm slot for sink 0 (speaker + dcdc)               */
	TFA9874_BF_TDMDCS = 0x2643,   	/*!< tdm slot for  sink 1  (dcdc)                       */
	TFA9874_BF_TDMCSS = 0x26c3,    	/*!< Slot Position of current sense vbat temperature and vddp feedback */
	TFA9874_BF_TDMVSS = 0x2703,    	/*!< Slot Position of Voltage sense vbat temperature and vddp feedback */
	TFA9874_BF_ISTVDDS = 0x4000,    	/*!< Status POR                                         */
	TFA9874_BF_ISTBSTOC = 0x4010,    	/*!< Status DCDC OCP                                    */
	TFA9874_BF_ISTOTDS = 0x4020,    	/*!< Status OTP alarm                                   */
	TFA9874_BF_ISTOCPR = 0x4030,    	/*!< Status ocp alarm                                   */
	TFA9874_BF_ISTUVDS = 0x4040,    	/*!< Status UVP alarm                                   */
	TFA9874_BF_ISTMANALARM = 0x4050,    /*!< Status  nanager Alarm state                        */
	TFA9874_BF_ISTTDMER = 0x4060,    	/*!< Status tdm error                                   */
	TFA9874_BF_ISTNOCLK = 0x4070,    	/*!< Status lost clock                                  */
	TFA9874_BF_ICLVDDS = 0x4400,    	/*!< Clear POR                                          */
	TFA9874_BF_ICLBSTOC = 0x4410,    	/*!< Clear DCDC OCP                                     */
	TFA9874_BF_ICLOTDS = 0x4420,    	/*!< Clear OTP alarm                                    */
	TFA9874_BF_ICLOCPR = 0x4430,    	/*!< Clear ocp alarm                                    */
	TFA9874_BF_ICLUVDS = 0x4440,    	/*!< Clear UVP alarm                                    */
	TFA9874_BF_ICLMANALARM = 0x4450,    /*!< clear  nanager Alarm state                         */
	TFA9874_BF_ICLTDMER = 0x4460,    	/*!< Clear tdm error                                    */
	TFA9874_BF_ICLNOCLK = 0x4470,    	/*!< Clear lost clk                                     */
	TFA9874_BF_IEVDDS = 0x4800,    	/*!< Enable por                                         */
	TFA9874_BF_IEBSTOC = 0x4810,    	/*!< Enable DCDC OCP                                    */
	TFA9874_BF_IEOTDS = 0x4820,    	/*!< Enable OTP alarm                                   */
	TFA9874_BF_IEOCPR = 0x4830,    	/*!< Enable ocp alarm                                   */
	TFA9874_BF_IEUVDS = 0x4840,    	/*!< Enable UVP alarm                                   */
	TFA9874_BF_IEMANALARM = 0x4850,    /*!< Enable  nanager Alarm state                        */
	TFA9874_BF_IETDMER = 0x4860,    	/*!< Enable tdm error                                   */
	TFA9874_BF_IENOCLK = 0x4870,    	/*!< Enable lost clk                                    */
	TFA9874_BF_IPOVDDS = 0x4c00,    	/*!< Polarity por                                       */
	TFA9874_BF_IPOBSTOC = 0x4c10,    	/*!< Polarity DCDC OCP                                  */
	TFA9874_BF_IPOOTDS = 0x4c20,    	/*!< Polarity OTP alarm                                 */
	TFA9874_BF_IPOOCPR = 0x4c30,    	/*!< Polarity ocp alarm                                 */
	TFA9874_BF_IPOUVDS = 0x4c40,    	/*!< Polarity UVP alarm                                 */
	TFA9874_BF_IPOMANALARM = 0x4c50,    /*!< Polarity  nanager Alarm state                      */
	TFA9874_BF_IPOTDMER = 0x4c60,    	/*!< Polarity tdm error                                 */
	TFA9874_BF_IPONOCLK = 0x4c70,    	/*!< Polarity lost clk                                  */
	TFA9874_BF_BSSCR = 0x5001,    	/*!< Battery Safeguard attack time                      */
	TFA9874_BF_BSST = 0x5023,    		/*!< Battery Safeguard threshold voltage level          */
	TFA9874_BF_BSSRL = 0x5061,    	/*!< Battery Safeguard maximum reduction                */
	TFA9874_BF_VBATFLTL = 0x5080,    	/*!< vbat filter limit                                  */
	TFA9874_BF_BSSR = 0x50e0,    		/*!< Battery voltage read out                           */
	TFA9874_BF_BSSBY = 0x50f0,    	/*!< Bypass battery safeguard                           */
	TFA9874_BF_BSSS = 0x5100,    		/*!< Vbat prot steepness                                */
	TFA9874_BF_HPFBYP = 0x5150,    	/*!< Bypass HPF                                         */
	TFA9874_BF_DPSA = 0x5170,    		/*!< Enable DPSA                                        */
	TFA9874_BF_CLIPCTRL = 0x5222,    	/*!< Clip control setting                               */
	TFA9874_BF_AMPGAIN = 0x5257,    	/*!< Amplifier gain                                     */
	TFA9874_BF_SLOPEE = 0x52d0,    	/*!< Enables slope control                              */
	TFA9874_BF_SLOPESET = 0x52e0,    	/*!< Slope speed setting (bin. coded)                   */
	TFA9874_BF_TDMDCG = 0x6123,    	/*!< Second channel gain in case of stereo using a single coil. (Total gain depending on INPLEV). (In case of mono OR stereo using 2 separate DCDC channel 1 should be disabled using TDMDCE) */
	TFA9874_BF_TDMSPKG = 0x6163,    	/*!< Total gain depending on INPLEV setting (channel 0) */
	TFA9874_BF_LNMODE = 0x62e1,    	/*!< ctrl select mode                                   */
	TFA9874_BF_LPM1MODE = 0x64e1,    /*!< low power mode control                             */
	TFA9874_BF_TDMSRCMAP = 0x6802,    /*!< tdm source mapping                                 */
	TFA9874_BF_TDMSRCAS = 0x6831,    /*!< Sensed value  A                                    */
	TFA9874_BF_TDMSRCBS = 0x6851,    /*!< Sensed value  B                                    */
	TFA9874_BF_TDMSRCACLIP = 0x6871,    /*!< clip information  (analog /digital) for source0    */
	TFA9874_BF_TDMSRCBCLIP = 0x6891,    /*!< clip information  (analog /digital) for source1    */
	TFA9874_BF_LP1 = 0x6e10,    		/*!< low power mode 1 detection                         */
	TFA9874_BF_LA = 0x6e20,    		/*!< low amplitude detection                            */
	TFA9874_BF_VDDPH = 0x6e30,    	/*!< vddp greater than vbat                             */
	TFA9874_BF_DELCURCOMP = 0x6f02,    /*!< delay to allign compensation signal with current sense signal */
	TFA9874_BF_SIGCURCOMP = 0x6f40,    /*!< polarity of compensation for current sense         */
	TFA9874_BF_ENCURCOMP = 0x6f50,    /*!< enable current sense compensation                  */
	TFA9874_BF_LVLCLPPWM = 0x6f72,    /*!< set the amount of pwm pulse that may be skipped before clip-flag is triggered */
	TFA9874_BF_DCMCC = 0x7033,    	/*!< Max coil current                                   */
	TFA9874_BF_DCCV = 0x7071,    	/*!< Slope compensation current, represents LxF (inductance x frequency) value  */
	TFA9874_BF_DCIE = 0x7090,    		/*!< Adaptive boost mode                                */
	TFA9874_BF_DCSR = 0x70a0,    		/*!< Soft ramp up/down                                  */
	TFA9874_BF_DCDIS = 0x70e0,    	/*!< DCDC on/off                                        */
	TFA9874_BF_DCPWM = 0x70f0,    	/*!< DCDC PWM only mode                                 */
	TFA9874_BF_DCTRACK = 0x7430,    	/*!< Boost algorithm selection, effective only when boost_intelligent is set to 1 */
	TFA9874_BF_DCTRIP = 0x7444,    /*!< 1st Adaptive boost trip levels, effective only when DCIE is set to 1 */
	TFA9874_BF_DCHOLD = 0x7494,    /*!< Hold time for DCDC booster, effective only when boost_intelligent is set to 1 */
	TFA9874_BF_DCINT = 0x74e0,    /*!< Selection of data for adaptive boost algorithm, effective only when boost_intelligent is set to 1 */
	TFA9874_BF_DCTRIP2 = 0x7534,    /*!< 2nd Adaptive boost trip levels, effective only when DCIE is set to 1 */
	TFA9874_BF_DCTRIPT = 0x7584,    /*!< Track Adaptive boost trip levels, effective only when boost_intelligent is set to 1 */
	TFA9874_BF_DCTRIPHYSTE = 0x75f0,    /*!< Enable hysteresis on booster trip levels           */
	TFA9874_BF_DCVOF = 0x7635,    /*!< First boost voltage level                          */
	TFA9874_BF_DCVOS = 0x7695,    /*!< Second boost voltage level                         */
	TFA9874_BF_MTPK = 0xa107,    /*!< MTP KEY2 register                                  */
	TFA9874_BF_KEY1LOCKED = 0xa200,    /*!< Indicates KEY1 is locked                           */
	TFA9874_BF_KEY2LOCKED = 0xa210,    /*!< Indicates KEY2 is locked                           */
	TFA9874_BF_CIMTP = 0xa360,    /*!< Start copying data from I2C mtp registers to mtp   */
	TFA9874_BF_MTPRDMSB = 0xa50f,    /*!< MSB word of MTP manual read data                   */
	TFA9874_BF_MTPRDLSB = 0xa60f,    /*!< LSB word of MTP manual read data                   */
	TFA9874_BF_EXTTS = 0xb108,    /*!< External temperature (C)                           */
	TFA9874_BF_TROS = 0xb190,    /*!< Select temp Speaker calibration                    */
	TFA9874_BF_SWPROFIL = 0xee0f,    /*!< Software profile data                              */
	TFA9874_BF_SWVSTEP = 0xef0f,    /*!< Software vstep information                         */
	TFA9874_BF_MTPOTC = 0xf000,    /*!< Calibration schedule                               */
	TFA9874_BF_MTPEX = 0xf010,    /*!< Calibration Ron executed                           */
	TFA9874_BF_DCMCCAPI = 0xf020,    /*!< Calibration current limit DCDC                     */
	TFA9874_BF_DCMCCSB = 0xf030,    /*!< Sign bit for delta calibration current limit DCDC  */
	TFA9874_BF_USERDEF = 0xf042,    /*!< Calibration delta current limit DCDC               */
	TFA9874_BF_CUSTINFO = 0xf078,    /*!< Reserved space for allowing customer to store speaker information */
	TFA9874_BF_R25C = 0xf50f,    /*!< Ron resistance of  speaker coil                    */
} nxpTfa9874BfEnumList_t;

#endif